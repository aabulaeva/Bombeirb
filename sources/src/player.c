/*******************************************************************************
* This file is part of Bombeirb.
* Copyright (C) 2018 by Laurent Réveillère
******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>

#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <map.h>
#include <bombe.h>
#include <monster.h>



struct player {
	int x, y;
	enum direction direction;
	int bombs;
	int lives;
	struct bombe* first; //permet de stocker la premiere bombe
	struct bombe* last;  //permet de stocker la dernier bombe
	int range;
	int key;
};

struct player* player_init(int bombs) { //initialisation du player
	struct player* player = malloc(sizeof(*player));
	if (!player)
	error("Memory error");

	player->direction = NORTH;
	player->bombs = bombs;
	player->lives=8;
	player->first=NULL;
	player->last=NULL;

	player->range=1;
	player->key=0;
	return player;
}



struct bombe* player_get_next(struct player* player,struct bombe* prec,struct bombe* bombe){ //permet de rajouter la bombe suivante au niveau du player
	assert(player);
	struct bombe* intem= bombe_get_next(prec); //permet d obtenir la bombe suivante (null) au niveau de la bombe
	intem=bombe;				//on remplace la bombe nulle par bombe
	return intem;
}
struct bombe* player_next(struct player* player,struct bombe* prec){
	assert(player);
	struct bombe* intem= bombe_get_next(prec); //on affiche la bombe suivante
	return intem;
}

void player_set_position(struct player *player, int x, int y) { //position d un player
	assert(player);
	player->x = x;
	player->y = y;
}

struct bombe*  player_first(struct player* player){ //affiche la bombe first du player,racine de toutes les bombes
	assert(player);
	if (player->first != NULL) {
		return player->first;
	}
	else{
		return player->first;
	}
}

struct bombe* player_get_first(struct player* player,struct bombe* bombe){ //permet d associer une bombe comme racine de toutes les bombes
	assert(player);
	player->first=bombe;
	return player->first;
}
struct bombe* player_get_last(struct player* player,struct bombe* bombe){ //permet de rajouter une bombe a la suite de la derniere bombe rajoutée
	assert(player);
	player->last=bombe;
	return player->last;
}
struct bombe* player_last(struct player* player){ //permet d afficher la derniere bombe rajoutée
	assert(player);
	return player->last;
}
int player_get_range(struct player* player){ // renvoi la portéé des bombes
	assert(player);
	return player->range;
}
int player_dec_range(struct player* player){//reduit la portée des bombes
	assert(player);
	if (player->range > 1) {
	return player->range --;
	}
	return player->range;
}
int player_inc_range(struct player* player){//augmente la portée des bombes
	assert(player);
	if (player->range < 8) {
	return player->range ++;
}
return player->range;
}
int player_dec_key(struct player* player){ //enleve une clé en cas d utilisation
	assert(player);
	if (player->key > 0) {
	return player->key --;
	}
	return player->key;
}
int player_inc_key(struct player* player){ //rajoute une clé si on l a trouve dans une boite
	assert(player);
	if (player->key < 9) {
	return player->key ++;
}
return player->key;
}
int player_get_key(struct player* player){//renvoi le nombre de clé
	assert(player);
	return player->key;
}


struct bombe* player_bombe_update(struct player* player){ //si la premiere bombe n est pas vide on appelle la chaine de bombe(appeler la fonction qui va afficher les differents etats)
	struct bombe* bombe=player->first;
		if (bombe != NULL) {
			bombe_chain(bombe_get_map(bombe),bombe,player);
			}
	return bombe;
}

void player_free(struct player* player) { //libere la place occupée par le player
	assert(player);
	free(player);
}

int player_get_x(struct player* player) {//renvoi l absisse du player
	assert(player != NULL);
	return player->x;
}

int player_get_y(struct player* player) { //renvoi l'ordonnée du player
	assert(player != NULL);
	return player->y;
}

void player_set_current_way(struct player* player, enum direction way) { //renvoi la direction du player
	assert(player);
	player->direction = way;
}

int player_get_nb_bomb(struct player* player) { //renvoi le nombre de bombe du player
	assert(player);
	return player->bombs;
}
	int player_get_nb_lives(struct player* player){ //renvoi le nombre de vie du player
	assert (player);
	return player->lives;
	}


void player_inc_nb_bomb(struct player* player) { //augmente le nombre de bombe
	assert(player);
	if (player->bombs < 8) {

	player->bombs += 1;
	}
}

void player_inc_nb_lives(struct player* player) { //augmente le nombre de vie
	assert(player);
	if (player->lives<8) {
		player->lives += 1;
	}
}

void player_dec_nb_lives(struct player* player) { //diminue le nombre de vie
	assert(player);
	if (player->lives==0) {
		game_set_level_dead(player);
		}
	else
		player->lives -= 1;
}

void player_dec_nb_bomb(struct player* player) {  //diminue le nombre de bombe
	assert(player);
	if (player->bombs>0) {
		player->bombs -= 1;
	}
}

void player_get_bonus(struct player* player, int type){ //permet d appeler la bonne fonction selon le type de bonus
	switch (type) {
		case CELL_BONUS_BOMB_RANGE_INC:
			player_inc_range(player);
			break;
		case CELL_BONUS_BOMB_RANGE_DEC:
			player_dec_range(player);
			break;

		case CELL_BONUS_BOMB_NB_DEC:
			if (player_get_nb_bomb(player)>1) {
				player_dec_nb_bomb(player);
				}
			break;

		case CELL_BONUS_BOMB_NB_INC:
			player_inc_nb_bomb(player);
			break;
		case CELL_BONUS_LIFE:
			player_inc_nb_lives(player);
			break;
		case CELL_KEY:
			player_inc_key(player);
			break;
	}
}

static int player_move_box(struct player* player, struct map* map, int x, int y); //fonction permettant de bouger les boites


static int player_move_aux(struct player* player, struct map* map, int x, int y) { //permet au player de se deplacer sans manger le decor,de gerer les interactions avec les differentes cases
	if (!map_is_inside(map, x, y))
		return 0;
	int type=map_get_compose_type(map,x,y);
	switch (map_get_cell_type(map, x, y)) {
		case CELL_SCENERY://le decor
		{switch (type) {
			case CELL_PRINCESS:
				player_princess(player,type);
				return 1;
				break;
			default:
				return 0;
				break;
				}
			return 0;
			break;
		}
		case CELL_BOX:{//les boites
			int a= player_move_box(player,map,x,y);
			if (a==0){
				return 0;
			}
			else{
				return 1;
				break;
				}
			}
		case CELL_BONUS:{//les bonus et la fonction qui renvoi vers les fonctions associées
			player_get_bonus(player,type);
			map_set_cell_type(map,x,y,CELL_EMPTY);
			break;
			}
		case CELL_DOOR: //cellule porte avec la fonction qui permet de gerer l'ouverture de la porte
			return game_player_win(player,type);
			break;

		default:
			break;
		}
	return 1;
}

static int player_move_box(struct player* player, struct map* map, int x, int y){ //fonction permettant de deplacer les boites
	switch (player->direction) {
			case NORTH:{
				if (!map_is_inside(map, x, y -1))
					return 0;
				switch (map_get_cell_type(map, x, y - 1)) {
					case CELL_EMPTY:{
						map_set_cell_type(map,x,y-1,CELL_BOX);
						map_set_cell_type(map,x,y,CELL_EMPTY);
						return 1;
						break;
						}
					default:{
						return 0;
						break;
						}
					}
					break;
				}

				case SOUTH:{
					if (!map_is_inside(map, x, y +1))
						return 0;
					switch (map_get_cell_type(map, x, y + 1)) {
						case CELL_EMPTY:{
							map_set_cell_type(map,x,y+1,CELL_BOX);
							map_set_cell_type(map,x,y,CELL_EMPTY);
							return 1;
							break;
							}
							default:{
								return 0;
								break;
							}
						}
						break;
					}

			case WEST:{
				if (!map_is_inside(map, x-1, y))
					return 0;
				switch (map_get_cell_type(map, x - 1, y)) {
					case CELL_EMPTY:{
						map_set_cell_type(map,x-1,y,CELL_BOX);
						map_set_cell_type(map,x,y,CELL_EMPTY);
						return 1;
						break;
						}
					default:{
						return 0;
						break;
						}
					}
					break;
				}

			case EAST:{
				if (!map_is_inside(map, x+1, y))
						return 0;
				switch (map_get_cell_type(map, x + 1, y)) {
						case CELL_EMPTY:{
							map_set_cell_type(map,x+1,y,CELL_BOX);
							map_set_cell_type(map,x,y,CELL_EMPTY);
							return 1;
							break;
							}
						default: {
							return 0;
							break;
							}
						}
						break;
					}
			}
	return 1;
	}



int player_move(struct player* player, struct map* map) { //permet au joueur de bouger
	int x = player->x;
	int y = player->y;
	int move = 0;

	switch (player->direction) {
		case NORTH:
			if (player_move_aux(player, map, x, y - 1)) {
				player->y--;
				move = 1;
				}
			break;

		case SOUTH:
			if (player_move_aux(player, map, x, y + 1)) {
				player->y++;
				move = 1;
				}
			break;

		case WEST:
			if (player_move_aux(player, map, x - 1, y)) {
				player->x--;
				move = 1;
				}
			break;

		case EAST:
			if (player_move_aux(player, map, x + 1, y)) {
				player->x++;
				move = 1;
				}
			break;
		}
	if (move) {
		switch (map_get_cell_type(map, x , y)){
			case CELL_BONUS:
				break;
			case CELL_BOMB:
				break;
			case CELL_EXP:
				break;
			case CELL_DOOR:
				break;
			case CELL_SCENERY:
			break;
			default:{
				map_set_cell_type(map, x, y, CELL_EMPTY);
				break;
				}
			}
		}
return move;
}
int player_princess(struct player* player,int type){
	if (type==CELL_PRINCESS)
		return 1;
	else
		return 0;
}

void player_display(struct player* player) { //affiche le player
	assert(player);
	window_display_image(sprite_get_player(player->direction),player->x * SIZE_BLOC, player->y * SIZE_BLOC);
}
