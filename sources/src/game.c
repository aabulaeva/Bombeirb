/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <assert.h>
#include <time.h>

#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>
#include <bombe.h>
#include <monster.h>
#include <map.h>



struct game {
	struct map** maps;       // the game's map
	short levels;        // nb maps of the game
	short level;
	struct player* player;
	struct monster** monster; //le pointeur sur un tableau de pointeur de structure monstre

};

struct game* game_new(void) {
	sprite_load(); // load sprites into process memory
	struct game* game = malloc(sizeof(*game));
	game->maps=NULL;
	game->levels = 8;
	game->maps = malloc(game->levels*sizeof(struct map*));
	game->level = 1;
	game->monster=malloc(28*sizeof(struct monster*)); //on prend un tableau de la taille de 2fois le level du game,je considere que c est le level 1 et non 0,donc pour tous les levels il y aura un decallage de 1;
	game->player = player_init(1);
	player_set_position(game->player, 1, 0);
	for (size_t j = 0; j < game->levels ; j++){
			if (j==0 ) {
				game->maps[j]=map_get_static();
				}
			else
				game->maps[j]=mapp_load(j);
				}
	if (game_level(game) != 7) {
		for (size_t i = 0; i < 28 ; i++) { //boucle creant tous les monstres et les associant a des levels differents donc a des maps differentes
			int y=rand()%12;
			int x=rand()%12;
			if (i==0) {
			 while (map_get_cell_type(game->maps[0],x,y) != CELL_EMPTY) {
		 		x=rand()%12;
				y=rand()%12;
		 		}
				game->monster[i]=monster_init(x,y,game->monster[i],game->player);
 				monster_set_position(game->monster[i],x,y);
				}
			if (i==1 || i==2) {
				while (map_get_cell_type(game->maps[1],x,y) != CELL_EMPTY) {
					x=rand()%12;
					y=rand()%12;
 				}
				game->monster[i]=monster_init(x,y,game->monster[i],game->player);
				monster_set_position(game->monster[i],x,y);
 				}

			if (i==3 || i==4 || i==5) {
				while (map_get_cell_type(game->maps[2],x,y) != CELL_EMPTY) {
					x=rand()%12;
					y=rand()%12;
	 			}
				game->monster[i]=monster_init(x,y,game->monster[i],game->player);
				monster_set_position(game->monster[i],x,y);
	 			}
		 	if (i==6 || i==7 || i==8 || i==9) {
		  	while (map_get_cell_type(game->maps[3],x,y) != CELL_EMPTY) {
		 		x=rand()%12;
		 		y=rand()%12;
		  	}
		 		game->monster[i]=monster_init(x,y,game->monster[i],game->player);
		 		monster_set_position(game->monster[i],x,y);
		  	}
			if (i==10 || i==11 || i==12 || i==13 || i==14) {
 		  	while (map_get_cell_type(game->maps[4],x,y) != CELL_EMPTY) {
 		 		x=rand()%12;
 		 		y=rand()%12;
 		  	}
 		 		game->monster[i]=monster_init(x,y,game->monster[i],game->player);
 		 		monster_set_position(game->monster[i],x,y);
 		  	}
			if (i==15 || i==16 || i==17 || i==18 || i==19 || i==20) {
 		  	while (map_get_cell_type(game->maps[5],x,y) != CELL_EMPTY) {
 		 		x=rand()%12;
 		 		y=rand()%12;
 		  	}
 		 		game->monster[i]=monster_init(x,y,game->monster[i],game->player);
 		 		monster_set_position(game->monster[i],x,y);
 		  	}
			if (i==21 || i==22 || i==23 || i==24 || i==25 || i==26 || i==27) {
 		  	while (map_get_cell_type(game->maps[6],x,y) != CELL_EMPTY) {
 		 		x=rand()%12;
 		 		y=rand()%12;
 		  	}
 		 		game->monster[i]=monster_init(x,y,game->monster[i],game->player);
 		 		monster_set_position(game->monster[i],x,y);
 		  	}
			}
		}
	return game;
}
int game_set_level_dead(struct player* player){ //change le level en 0 et nous renvoi vers la map dead
	if (player_get_nb_lives(player)==0) {
		return 1;
		}
	else
		return 0;
}

int game_level(struct game* game){
	assert(game);
	return game->level;
}
void game_get_level(struct game* game){
	assert(game);
	game->level ++;
}
int game_change_map(struct game* game,int type){//permet d'obtenir la valeur de la porte ouverte,le sous type de la porte
	int l=game_level(game);
	int x=0x0f;
	switch (type & x ) {
		case 1:{
			if (l==0) {//le player est soit au niveau 0 ou au niveau 1
				game_get_level(game);
				}
			else
				game->level --;
				break;
				}
		case 3:{
			if (l==1) {
				game->level ++;
				}
			else{
				game->level=1;
				}
			break;
			}
		case 5:{
			if (l==2) {
				game_get_level(game);
				}
			else{
				game->level --;
				}
			break;
			}
		case 7:{
			if (l==3) {
				game_get_level(game);
				}
			else{
				game->level --;
			}
			break;
			}
		case 9:{
			if (l==4) {
				game_get_level(game);
				}
			else{
				game->level --;
			}
			break;
			}
		case 11:{
			if (l==5) {
				game_get_level(game);
				}
		else{
			game->level --;
			}
			break;
			}
		default:
			game->level=1;
			break;
		}
return game->level;
}

void game_free(struct game* game) { //on libere la place occupée par le jeu et toutes les structures-champs du jeu
	assert(game);
	player_free(game->player);
	for (int i = 0; i < game->levels; i++)
		map_free(game->maps[i]);
	for (size_t i = 0; i < 28 ; i++) {
		monster_free(game->monster[i]);
		}
	}



struct map* game_get_current_map(struct game* game) {//renvoi la carte courrante en fonction du level
	assert(game);
	return game->maps[game->level];
}
int game_player_win(struct player* player,int type){//le player est a la porte et on verifie l'etat de la porte
	int x=0x31; //une porte ouverte
		switch (type&x) {
			case CELL_DOOR_OPEN:
				return 1;
				break;
			case CELL_DOOR_CLOSE:{
				return 2;
				break;
				}
			}
	return 0;
}

struct player* game_get_player(struct game* game) { //le jeu a obtenu un joueur et peut renvoyer le pointeur du joueur
	assert(game);
	return game->player;
}
struct monster** game_get_monster(struct game* game) {//le jeu a obtenu un tableau de pointeur de  monstre et peut renvoyer l adresse du tableau
	assert(game->monster);
	return game->monster;
}
int game_player_monster(struct game* game){//permet d enlever une vie au player s'il croise un monstre,ou s'il se retrouve sur une cellule enflammées
if (game_level(game) != 7) { //pas de monstres au niveau 7,WIN
	int xp=player_get_x(game->player);
	int yp=player_get_y(game->player);

for (size_t i = 0; i < 28 ; i++) {
	int x=monster_get_x(game->monster[i]);
	int y=monster_get_y(game->monster[i]);
	if ((i==0) && (game_level(game) == 0)) {
		if ((x==xp) && (y==yp) && monster_state(game->monster[i])==0){//monstre pas mort et actif dans sa map
			player_dec_nb_lives(game->player);
			return 1;
			}
			if (map_get_compose_type(game_get_current_map(game),xp,yp)==0x80) { //player dans une cellule enflamée
				player_dec_nb_lives(game->player);
				return 1;
			}
		}

	if (((i==1) || (i==2)) && (game_level(game) == 1)) {
			if ((x==xp) && (y==yp) && monster_state(game->monster[i])==0){
				player_dec_nb_lives(game->player);
				return 1;
				}
				if (map_get_compose_type(game_get_current_map(game),xp,yp)==0x80) {
					player_dec_nb_lives(game->player);
					return 1;
				}
			}
	if ((i==3 || i==4 || i==5) && (game_level(game) == 2)) {
			if ((x==xp) && (y==yp) && monster_state(game->monster[i])==0){
				player_dec_nb_lives(game->player);
				return 1;
				}
				if (map_get_compose_type(game_get_current_map(game),xp,yp)==0x80) {
					player_dec_nb_lives(game->player);
					return 1;
				}
			}

	 if ((i==6 || i==7 || i==8 || i==9) && (game_level(game) == 3)) {
		 if ((x==xp) && (y==yp) && monster_state(game->monster[i])==0){
 			player_dec_nb_lives(game->player);
 			return 1;
 			}
 			if (map_get_compose_type(game_get_current_map(game),xp,yp)==0x80) {
 				player_dec_nb_lives(game->player);
 				return 1;
 				}
			}
		if ((i==10 || i==11 || i==12 || i==13 || i==14) && (game_level(game) == 4)) {
			if ((x==xp) && (y==yp) && monster_state(game->monster[i])==0){
				player_dec_nb_lives(game->player);
				return 1;
				}
				if (map_get_compose_type(game_get_current_map(game),xp,yp)==0x80) {
					player_dec_nb_lives(game->player);
					return 1;
				}
		}
		if ((i==15 || i==16 || i==17 || i==18 || i==19 || i==20) && (game_level(game) == 5)) {
			if ((x==xp) && (y==yp) && monster_state(game->monster[i])==0){
				player_dec_nb_lives(game->player);
				return 1;
				}
				if (map_get_compose_type(game_get_current_map(game),xp,yp)==0x80) {
					player_dec_nb_lives(game->player);
					return 1;
				}
		}
		if ((i==21 || i==22 || i==23 || i==24 || i==25 || i==26 || i==27) && (game_level(game) == 6)) {
			if ((x==xp) && (y==yp) && monster_state(game->monster[i])==0){
				player_dec_nb_lives(game->player);
				return 1;
				}
				if (map_get_compose_type(game_get_current_map(game),xp,yp)==0x80) {
				player_dec_nb_lives(game->player);
				return 1;
				}
			}
		}
	}
return 0;
}
void game_dead_monster(struct game* game){ //on verifie pour chaque monstre s'il vient de mourrir,s'il est deja mort il le reste
	for (size_t i = 0; i < 28 ; i++) {
		int x=monster_get_x(game->monster[i]);
		int y=monster_get_y(game->monster[i]);
		if (map_get_compose_type(game_get_current_map(game),x,y)==0x80) { //le monstre ne bouge plus et n eblesse plus le player
			monster_dead(game->monster[i]);
			}
		}
	}
int  game_pause(){ //fonction permettant de mettre en pause
	SDL_Event event;
  while (SDL_PollEvent(&event)) {
	 switch (event.type) {
	   case SDL_QUIT:
	     return 1;
	   case SDL_KEYDOWN:
	     switch(event.key.keysym.sym) {
	     	case SDLK_PAUSE:
		 	 		return 1;
		  		break;
	     	default:
		  		break;
				}
	      break;
	    }
  }
return 0;
}

void game_banner_display(struct game* game) { //permet d afficher le petit tableau avec les informations sur les equipements du player
	assert(game);
	struct map* map = game_get_current_map(game);
	int y = (map_get_height(map)) * SIZE_BLOC;
	for (int i = 0; i < map_get_width(map); i++)
		window_display_image(sprite_get_banner_line(), i * SIZE_BLOC, y);

	int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 6 * SIZE_BLOC) / 7;
	int x = white_bloc;
	y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;
	window_display_image(sprite_get_banner_life(), x, y);

	x = white_bloc + SIZE_BLOC;
		int l=player_get_nb_lives(game_get_player(game));
	window_display_image(sprite_get_number(l), x, y);

	x = 2 * white_bloc + 2 * SIZE_BLOC;
	window_display_image(sprite_get_banner_bomb(), x, y);

	x = 2 * white_bloc + 3 * SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_nb_bomb(game_get_player(game))), x, y);

	x = 3 * white_bloc + 4 * SIZE_BLOC;
	window_display_image(sprite_get_banner_range(), x, y);

	x = 3 * white_bloc + 5 * SIZE_BLOC;

	window_display_image(sprite_get_number(player_get_range(game->player)), x, y);

	x = 4 * white_bloc + 5 * SIZE_BLOC;
	window_display_image(sprite_get_key(), x, y);

	x = 4  * white_bloc + 6 * SIZE_BLOC;
		window_display_image(sprite_get_number(player_get_key(game->player)), x, y);
	x = 5 * white_bloc + 6 * SIZE_BLOC;
		window_display_image(sprite_get_flag(), x, y);

	x = 5  * white_bloc + 7 * SIZE_BLOC;
		window_display_image(sprite_get_number(game->level), x, y);
}

void game_display(struct game* game) { //affichage du jeu et des parametres
	assert(game);
	window_clear();
	game_banner_display(game);
	map_display(game_get_current_map(game));
	player_display(game->player);
	if (game_level(game) != 7) {
		for (size_t i = 0; i < 28 ; i++) {
			if ((i==0) && (game_level(game) == 0)) {
				monster_display(game->monster[i]);
				}
			if (((i==1) || (i==2)) && (game_level(game) == 1)) {
				monster_display(game->monster[i]);
				}
			if ((i==3 || i==4 || i==5) && (game_level(game) == 2)) {
				monster_display(game->monster[i]);
				}

		 if ((i==6 || i==7 || i==8 || i==9) && (game_level(game) == 3)) {
			 monster_display(game->monster[i]);
		  }
			if ((i==10 || i==11 || i==12 || i==13 || i==14) && (game_level(game) == 4)) {
				monster_display(game->monster[i]);
 		  	}
			if ((i==15 || i==16 || i==17 || i==18 || i==19 || i==20) && (game_level(game) == 5)) {
 		  	monster_display(game->monster[i]);
 		  	}
			if ((i==21 || i==22 || i==23 || i==24 || i==25 || i==26 || i==27) && (game_level(game) == 6)) {
				monster_display(game->monster[i]);
 		  	}
			}
		}
	window_refresh();
}

static short input_keyboard(struct game* game) {//plan de controle du jeu
	SDL_Event event;
	struct player* player = game_get_player(game);
	struct map* map = game_get_current_map(game);
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return 1;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				return 1;
			case SDLK_UP:
				player_set_current_way(player, NORTH);
				player_move(player, map);
				break;
			case SDLK_DOWN:
				player_set_current_way(player, SOUTH);
				player_move(player, map);
				break;
			case SDLK_RIGHT:
				player_set_current_way(player, EAST);
				player_move(player, map);
				break;
			case SDLK_LEFT:
				player_set_current_way(player, WEST);
				player_move(player, map);
				break;
			case SDLK_SPACE:{
				bombe_set(player,map);
				break;}
			case SDLK_PAUSE:{ //met le jeu en pause
			  window_display_image(sprite_get_pause(),130,130);
			  window_refresh();
			  int tmp = 0;
			  while(!tmp){
					tmp=game_pause(); //returne 0 tant qu'on a pas appuié de nouveaux sur pause
			    }
				break;
				}

			default:
				break;
			}

			break;
			}
		}
	return 0;
}

int game_monster(struct game* game){ //permet aux monstres de bouger dans la map actuelle
	if (game_level(game) != 7) {
		for (size_t i = 0; i < 28 ; i++) {
			if ((i==0) && (game_level(game) == 0)) {
				monster_move(game->monster[i],game->maps[0],1,game->player);
			}
			if (((i==1) || (i==2)) && (game_level(game) == 1)) {
				monster_move(game->monster[i],game->maps[1],1,game->player);
			}
			if ((i==3 || i==4 || i==5) && (game_level(game) == 2)) {
				monster_move(game->monster[i],game->maps[2],1,game->player);
			}
		 	if ((i==6 || i==7 || i==8 || i==9) && (game_level(game) == 3)) {
			 	monster_move(game->monster[i],game->maps[3],1,game->player);
			}
			if ((i==10 || i==11 || i==12 || i==13 || i==14) && (game_level(game) == 4)) {
				monster_move(game->monster[i],game->maps[4],1,game->player);
 		  }
			if ((i==15 || i==16 || i==17 || i==18 || i==19 || i==20) && (game_level(game) == 5)) {
 		  	monster_move(game->monster[i],game->maps[5],1,game->player);
 		  }
			if ((i==21 || i==22 || i==23 || i==24 || i==25 || i==26 || i==27) && (game_level(game) == 6)) {
				monster_move(game->monster[i],game->maps[6],1,game->player);
 		  }
		}
	}
return 1;
}
void game_choice_level(struct game* game){ //fonction permettant d'appeler la fonction qui change le level selon le type de porte
	int type=map_get_compose_type(game_get_current_map(game),player_get_x(game->player),player_get_y(game->player));
	if (game_player_win(game->player,type)==2) { //porte ferme
		if (player_get_key(game->player)) { //on donne une clé
			player_dec_key(game->player);
			int y=0x0f;
			switch (type & y) {
				case 0:{
					map_set_cell_type(game_get_current_map(game),player_get_x(game->player),player_get_y(game->player),CELL_DOOR_LEVEL0);//on ouvre la porte
					game_change_map(game,CELL_DOOR_LEVEL0); //on va changer de level
					break;
					}
				case 2:{
					map_set_cell_type(game_get_current_map(game),player_get_x(game->player),player_get_y(game->player),CELL_DOOR_LEVEL1);
					game_change_map(game,CELL_DOOR_LEVEL1);
					break;
					}
				case 4:{
					map_set_cell_type(game_get_current_map(game),player_get_x(game->player),player_get_y(game->player),CELL_DOOR_LEVEL2);
					game_change_map(game,CELL_DOOR_LEVEL2);
					break;
				}
				case 6:{
					map_set_cell_type(game_get_current_map(game),player_get_x(game->player),player_get_y(game->player),CELL_DOOR_LEVEL3);
					game_change_map(game,CELL_DOOR_LEVEL3);
					break;
				}
				case 8:{
					map_set_cell_type(game_get_current_map(game),player_get_x(game->player),player_get_y(game->player),CELL_DOOR_LEVEL4);
					game_change_map(game,CELL_DOOR_LEVEL4);
					break;
				}
				case 10:{
					map_set_cell_type(game_get_current_map(game),player_get_x(game->player),player_get_y(game->player),CELL_DOOR_LEVEL5);
					game_change_map(game,CELL_DOOR_LEVEL5);
					break;
				}
				case 12:{
					map_set_cell_type(game_get_current_map(game),player_get_x(game->player),player_get_y(game->player),CELL_DOOR_LEVEL6);
					game_change_map(game,CELL_DOOR_LEVEL6);
					break;
				}
			}
		}
	}
	if (game_player_win(game->player,type)==1) {//porte ouverte
		int y=0x0f;
		switch (type & y) {
			case 1:{
				map_set_cell_type(game_get_current_map(game),player_get_x(game->player),player_get_y(game->player),CELL_DOOR_LEVEL0);
				game_change_map(game,CELL_DOOR_LEVEL0);//on va changer de level
				break;
				}
			case 3:{
				map_set_cell_type(game_get_current_map(game),player_get_x(game->player),player_get_y(game->player),CELL_DOOR_LEVEL1);
				game_change_map(game,CELL_DOOR_LEVEL1);
				break;
				}
			case 5:{
				map_set_cell_type(game_get_current_map(game),player_get_x(game->player),player_get_y(game->player),CELL_DOOR_LEVEL2);
				game_change_map(game,CELL_DOOR_LEVEL2);
				break;
				}
			case 7:{
					map_set_cell_type(game_get_current_map(game),player_get_x(game->player),player_get_y(game->player),CELL_DOOR_LEVEL3);
					game_change_map(game,CELL_DOOR_LEVEL3);
					break;
					}
			case 9:{
				map_set_cell_type(game_get_current_map(game),player_get_x(game->player),player_get_y(game->player),CELL_DOOR_LEVEL4);
				game_change_map(game,CELL_DOOR_LEVEL4);
				break;
				}
			case 11:{
				map_set_cell_type(game_get_current_map(game),player_get_x(game->player),player_get_y(game->player),CELL_DOOR_LEVEL5);
				game_change_map(game,CELL_DOOR_LEVEL5);
				break;
				}
			default:{
				map_set_cell_type(game_get_current_map(game),player_get_x(game->player),player_get_y(game->player),CELL_DOOR_LEVEL6);
				game_change_map(game,CELL_DOOR_LEVEL6);
				break;
				}
			}
		}
	}

void game_monster_move(struct game* game,int timer){//permet aux monstres de bouger a des frequences differentes selon le niveau actuel
	switch (game_level(game)) {
		case 6:{
			if (timer%10==0) {
				game_monster(game);
				}
			break;
			}
		case 5:{
			if (timer%20==0) {
				game_monster(game);
				}
			break;
			}
		case 4:{
			if (timer%30==0) {
				game_monster(game);
				}
			break;
			}
		case 3:{
			if (timer%100==0) {
				game_monster(game);
				}
			break;
			}
		case 2:{
			if (timer%90==0) {
				game_monster(game);
				}
			break;
			}
		case 1:{
			if (timer%90==0) {
				game_monster(game);
				}
			break;
			}
		}
}
int game_update(struct game* game) { //garde le jeu a jour et permet aux fonctions dependantes du temps de fonctionner
	struct map* map=game_get_current_map(game);
	if (input_keyboard(game)){
		game_monster(game);
		return 1; // exit game
		}
		int timer=SDL_GetTicks();
		game_monster_move(game,timer);
		game_dead_monster(game);

	if (timer%10 ==0) {
		game_player_monster(game);
		player_bombe_update(game->player);
		if (game_set_level_dead(game->player)) {
			game->level=0;
			}
		}
	if (player_princess(game->player,map_get_compose_type(map,player_get_x(game->player),player_get_y(game->player))))
		game->level=7;
	if (map_get_cell_type(game_get_current_map(game),player_get_x(game->player),player_get_y(game->player))==CELL_DOOR) {
		game_choice_level(game);
		}
	return 0;
}
