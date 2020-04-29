#include <SDL/SDL_image.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <map.h>
#include <bombe.h>
#include <time.h>
#include <game.h>

struct bombe {
	int x, y;
	int ttl;
	struct bombe* next;
	struct map* map;
};

 struct bombe* bombeinit(struct player* player,struct map* map) { //initialise une bombe et la rajoute dans la chaine
 	struct bombe* bombe = malloc(sizeof(*bombe));
	bombe->map=map; //associe une map a une bombe
	bombe->next=NULL;
	bombe->ttl=4;
	bombe->x=player_get_x(player);
	bombe->y=player_get_y(player);
	if (player_first(player)==NULL) { //si la racine des bombes est nulles,1ere bombe initialisée
		player_get_first(player,bombe);//on rentre la racine
		player_get_last(player,bombe);//la racine est aussi la derniere bombe initialisée
		}
	else{
		struct bombe* prec=player_last(player); //on prend la derniere bombe lancé
		prec->next=bombe;	//onn rajoute la bombe initalisée a la suite de la derniere lancée
		player_get_last(player,bombe);//on initialise la derniere bombe lancée
		}
	return bombe;
}
struct map* bombe_get_map(struct bombe* bombe){
	assert(bombe);
	return bombe->map;
}

struct bombe* bombe_get_next(struct bombe* bombe){//permet d envoyer l adresse de la prochaine bombe
	assert(bombe);
	return bombe->next;
}

void bombe_chain(struct map* map,struct bombe* bombe,struct player* player){ //permet d'exploser les bombes de la chaine en commençant par la racine
	struct bombe* first =player_first(player);
	if (first != NULL) {
		bombe_set_orga(first->map,first,player);
		while (first->next != NULL) {
			assert(first->next);
			bombe_set_orga(first->next->map,first->next,player);
			first=first->next;
			}
	}
}

struct bombe*  bombe_set(struct player* player,struct map* map) { // permet de verifier si le player peut envoyer une bombe, de l'envoyer dans la chaine de bombe et mettre a jour les differents etats
 	assert(player);
	struct bombe* bombe;
	 if (player_get_nb_bomb(player)) {
			player_dec_nb_bomb(player);
			bombe =bombeinit(player,map);
			player_bombe_update(player);
			}
		else{
			struct bombe* last=player_last(player);//si on a plus de bombe et que la derniere bombe a explosé ,on vide la chaine de bombe
			if ((last!= NULL) &&(last->ttl==(-1))) {
				bombe_chain_free(player_first(player));
				player_get_first(player,NULL);
				player_get_last(player,NULL);
				bombe=player_first(player);
			}
		}
	return bombe;
}


int bombe_set_orga(struct map* map, struct bombe* bombe,struct player* player){ //initialise les differents etats selon le ttl restant avec le range
	int timer= bombe->ttl;
	int range=player_get_range(player);
	if (timer==4 && player_get_nb_lives(player)){
		map_set_cell_type(map,bombe->x,bombe->y,CELL_BOMB4);
		bombe->ttl--;
		}
	else if (timer==3 && player_get_nb_lives(player)){
		map_set_cell_type(map,bombe->x,bombe->y,CELL_BOMB3);
		bombe->ttl--;
		}
	else if (timer==2 && player_get_nb_lives(player)){
		map_set_cell_type(map,bombe->x,bombe->y,CELL_BOMB2);
		bombe->ttl--;
		}
	else if (timer==1 && player_get_nb_lives(player)){
		map_set_cell_type(map,bombe->x,bombe->y,CELL_BOMB1);
		bombe->ttl--;
		}
	else if (timer==0 && player_get_nb_lives(player) ){
		int c=1;


		for (size_t i = 1; i < range +1; i++) { //permet a la bombe d avoir une portée et qui evolue en fonction des obstacles selon chaque direction
			if (c&&(map_is_inside(map,bombe->x,bombe->y))&& (map_get_cell_type(map,bombe->x,bombe->y)!=0x10) && (map_get_cell_type(map,bombe->x,bombe->y)!=0x30)&& (map_get_compose_type(map,bombe->x,bombe->y)!=0x57) && (map_get_compose_type(map,bombe->x,bombe->y)!=0x80)&& (map_get_compose_type(map,bombe->x,bombe->y)!=CELL_BOMB4)) {
				switch (map_get_cell_type(map,bombe->x,bombe->y)) {
					case 0x10:{
						map_set_cell_type(map,bombe->x,bombe->y,CELL_EXP);
						c--;
						break;
						}
					case 0x20:{
						map_bonus(map,bombe->x,bombe->y);
						c--;
						break;
						}
					case 0x30:{
						c--;
						break;
						}

					default:{
						map_set_cell_type(map,bombe->x,bombe->y,CELL_EXP);
						break;
						}
					}
				}
			else if (map_is_inside(map,bombe->x,bombe->y)&&map_get_compose_type(map,bombe->x,bombe->y)==0x11){
					c--;
					break;
					}
			else
				c--;

		}
		c=1;

		for (size_t i = 1; i < range +1; i++) {

			if (c&&(map_is_inside(map,bombe->x+i,bombe->y))&& (map_get_cell_type(map,bombe->x+i,bombe->y)!=0x10) && (map_get_cell_type(map,bombe->x+i,bombe->y)!=0x30)&& (map_get_compose_type(map,bombe->x+i,bombe->y)!=0x57)&& (map_get_compose_type(map,bombe->x+i,bombe->y)!=0x80)&& (map_get_compose_type(map,bombe->x+i,bombe->y)!=CELL_BOMB4)) {
				switch (map_get_cell_type(map,bombe->x+i,bombe->y)) {
					case 0x30:
						c--;
						break;
					case 0x10:
						map_set_cell_type(map,bombe->x+i,bombe->y,CELL_EXP);
						c--;
						break;
					case 0x20:
						map_bonus(map,bombe->x+i,bombe->y);
						c--;
						break;
					default:
						map_set_cell_type(map,bombe->x+i,bombe->y,CELL_EXP);
						break;
					}
				}
				else if (map_is_inside(map,bombe->x+i,bombe->y)&&map_get_cell_type(map,bombe->x+i,bombe->y)==0x10){
					c--;
					break;
					}
				else
					c--;
				}
				c=1;



			for (size_t i = 1; i < range +1; i++) {

			if (c&&(map_is_inside(map,bombe->x-i,bombe->y))&& (map_get_cell_type(map,bombe->x-i,bombe->y)!=0x10) && (map_get_cell_type(map,bombe->x-i,bombe->y)!=0x30)&& (map_get_compose_type(map,bombe->x-i,bombe->y)!=0x11) && (map_get_compose_type(map,bombe->x-i,bombe->y)!=0x57)&& (map_get_compose_type(map,bombe->x-i,bombe->y)!=0x80)&& (map_get_compose_type(map,bombe->x-i,bombe->y)!=CELL_BOMB4)) {
				switch (map_get_cell_type(map,bombe->x-i,bombe->y)) {
					case 0x30:
						c--;
						break;
					case 0x10:
						map_set_cell_type(map,bombe->x-i,bombe->y,CELL_EXP);
						c--;
						break;
					case 0x20:
						map_bonus(map,bombe->x-i,bombe->y);
						c--;
						break;
					default:
						map_set_cell_type(map,bombe->x-i,bombe->y,CELL_EXP);
						break;
					}
				}
				else if (map_is_inside(map,bombe->x-i,bombe->y)&&map_get_cell_type(map,bombe->x-i,bombe->y)==0x10){
					c--;
					break;
					}
				else
					c--;
				}
				c=1;



			for (size_t i = 1; i < range +1; i++) {

			if (c&&(map_is_inside(map,bombe->x,bombe->y+i))&& (map_get_cell_type(map,bombe->x,bombe->y+i)!=0x10) && (map_get_cell_type(map,bombe->x,bombe->y+i)!=0x30)&& (map_get_compose_type(map,bombe->x,bombe->y+i)!=0x57)&& (map_get_compose_type(map,bombe->x,bombe->y+i)!=0x80)&& (map_get_compose_type(map,bombe->x,bombe->y+i)!=CELL_BOMB4)) {
				switch (map_get_cell_type(map,bombe->x,bombe->y+i)) {
					case 0x30:
						c--;
						break;
					case 0x20:
						map_bonus(map,bombe->x,bombe->y+i);
						c--;
						break;
					case 0x10:
						map_set_cell_type(map,bombe->x,bombe->y+i,CELL_EXP);
						c--;
						break;
					default:
						map_set_cell_type(map,bombe->x,bombe->y+i,CELL_EXP);
						break;
					}
				}
				else if (map_is_inside(map,bombe->x,bombe->y+i)&&map_get_cell_type(map,bombe->x,bombe->y+i)==0x10){
					c--;
					break;
					}
				else
					c--;
				}
				c=1;



			for (size_t i = 1; i < range +1; i++) {

			if (c&&(map_is_inside(map,bombe->x,bombe->y-i))&& (map_get_cell_type(map,bombe->x,bombe->y-i)!=0x10)&& (map_get_cell_type(map,bombe->x,bombe->y-i)!=0x30)&& (map_get_compose_type(map,bombe->x,bombe->y-i)!=0x57)&& (map_get_compose_type(map,bombe->x,bombe->y-i)!=0x80)&& (map_get_compose_type(map,bombe->x,bombe->y-i)!=CELL_BOMB4)) {
				switch (map_get_cell_type(map,bombe->x,bombe->y-i)) {
					case 0x30:
						c--;
						break;
					case 0x20:
						map_bonus(map,bombe->x,bombe->y-i);
						c--;
						break;
					case 0x10:
						map_set_cell_type(map,bombe->x,bombe->y-i,CELL_EXP);
						c--;
						break;
					default:
						map_set_cell_type(map,bombe->x,bombe->y-i,CELL_EXP);
						break;
					}
					}
				else if (map_is_inside(map,bombe->x,bombe->y-i)&&map_get_cell_type(map,bombe->x,bombe->y-i)!=0x10){
					c--;
					break;
					}
				else
					c--;
				}
				c=1;


		bombe->ttl--;
		}
		else if (timer==-1 && player_get_nb_lives(player)){ //remplace les cellules enflamées par des cellules vides
			bombe->ttl--;
			for (size_t i = 1; i < range +1; i++) {
				if ((map_is_inside(map,bombe->x,bombe->y))&& (map_get_cell_type(map,bombe->x,bombe->y)==0x80)) {
					map_set_cell_type(map,bombe->x,bombe->y,CELL_EMPTY);
					}
			if ((map_is_inside(map,bombe->x+i,bombe->y))&& (map_get_cell_type(map,bombe->x+i,bombe->y)==0x80)) {
					map_set_cell_type(map,bombe->x+i,bombe->y,CELL_EMPTY);
					}
			if ((map_is_inside(map,bombe->x-i,bombe->y))&& (map_get_cell_type(map,bombe->x-i,bombe->y)==0x80)) {
					map_set_cell_type(map,bombe->x-i,bombe->y,CELL_EMPTY);
				}
			if ((map_is_inside(map,bombe->x,bombe->y+i))&& (map_get_cell_type(map,bombe->x,bombe->y+i)==0x80)) {
					map_set_cell_type(map,bombe->x,bombe->y+i,CELL_EMPTY);
				}
			if ((map_is_inside(map,bombe->x,bombe->y-i))&& (map_get_cell_type(map,bombe->x,bombe->y-i)==0x80)) {
					map_set_cell_type(map,bombe->x,bombe->y-i,CELL_EMPTY);
				}
			}

		}
		if (bombe->ttl==-1) {
			player_inc_nb_bomb(player);
		}

return bombe->ttl;
}
int bombe_chain_free(struct bombe* bombe){ //permet de liberer la place occupée par chaque bombe de la chaine,appelée avec la racine en argument
	assert(bombe);
	while (bombe != NULL) {
		bombe_free(bombe);
		bombe=bombe->next;
	}
	return 1;
}

void bombe_free(struct bombe* bombe){ //libere la place occupée par une bombe
	assert(bombe);
	free(bombe);
}
