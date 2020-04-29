#include <SDL/SDL_image.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <map.h>
#include <bombe.h>
#include <time.h>

struct monster { //structure du monstre
	int x, y;
	enum direction direction;
	int stalker;
	int dead;
};



 struct monster* monster_init(int x, int y,struct monster* monster,struct player* player){//initialisation du monstre
   monster=malloc(sizeof(*monster));
	 if (!monster) {
	 	error("memory");
	 	}
	 monster->x = x;
 	 monster->y = y;
   monster->direction=EAST;
	 monster->stalker=rand()%2;
	 monster->dead=0;
   return monster;
 }



void monster_set_position(struct monster *monster, int x, int y) {//renvoi la position du monstre
	assert(monster);
	monster->x = x;
	monster->y = y;
}


void monster_free(struct monster* monster) {//libere la place occupée par le monstre
	assert(monster);
	free(monster);
}

int monster_get_x(struct monster* monster) {//renvoi l absisse du monstre
	assert(monster != NULL);
	return monster->x;
}

int monster_get_y(struct monster* monster) {//renvoi l ordonnée du monstre
	assert(monster != NULL);
	return monster->y;
}

void monster_set_current_way(struct monster* monster, enum direction way) { //renvoi la direction du monstre
	assert(monster);
	monster->direction = way;
}

static int monster_move_aux(struct monster* monster, struct map* map, int x, int y) { // permet au monstre d'interagir et de ne pas manger le decor
	if (!map_is_inside(map, x, y))
		return 0;
	switch (map_get_cell_type(map, x, y)) {
		case CELL_SCENERY:
			return 0;
			break;
		case CELL_BOX:
			return 0;
			break;
		case CELL_BONUS:
    	return 0;
			break;
		case CELL_EXP:
			monster_dead(monster);
			break;
		case CELL_DOOR:
			return 0;
		default:
			break;
		}
	return 1;
}


int monster_move(struct monster* monster, struct map* map,int a,struct player* player) {//permet au monstre de bouger de façon aleatoire ou de suivre le joueur
	int x = monster->x;
	int y = monster->y;
	int move = rand() % 4;
  srand(time(NULL));
	int mo;
	int xs=monster->stalker;
	int d=monster->dead;
	if (xs==0 && d==0) {
		switch (move) {
			case NORTH:
				if  (monster_move_aux(monster, map, x, y - 1)) {
					monster_set_current_way(monster,NORTH);
					monster->y=y-a;
			  	mo = 0;
					return mo;
					}
			break;

			case SOUTH:
				if (monster_move_aux(monster, map, x, y + 1)) {
					monster_set_current_way(monster,SOUTH);
					monster->y=y+a;
			 		mo = 0;
			 		return mo;
					}
			break;

			case WEST:
				if (monster_move_aux(monster, map, x - 1, y)) {
					monster_set_current_way(monster,WEST);
					monster->x=x-a;
					mo = 0;
					return mo;
				}
			break;

			case EAST:
				if (monster_move_aux(monster, map, x + 1, y)) {
					monster_set_current_way(monster,EAST);
					monster->x=x+a;
					mo = 0;
					return mo;
				}
			break;
			}
		}
	else if (xs != 0 && d==0) {
		int xp=player_get_x(player);
		int yp=player_get_y(player);
		if (x<xp && monster_move_aux(monster, map, x + 1, y)) {
			monster_set_current_way(monster,EAST);
			monster->x=x+a;
			mo = 0;
			return mo;
			}
		else if (x>xp && monster_move_aux(monster, map, x - 1, y)) {
			monster_set_current_way(monster,WEST);
			monster->x=x-a;
			mo = 0;
			return mo;
			}
		else if (y<yp && monster_move_aux(monster, map, x, y + 1)){
			monster_set_current_way(monster,SOUTH);
			monster->y=y+a;
			mo = 0;
			return mo;
			}
		else if (y>yp && monster_move_aux(monster, map, x, y - 1)) {
			monster_set_current_way(monster,NORTH);
			monster->y=y-a;
			mo = 0;
			return mo;
			}
		else{
			return 0;
			}
		}
		else
			return 0;
return 0;
}
int monster_dead(struct monster* monster ){
	assert(monster);
	monster->dead=1;
	return monster->dead;
}
int monster_state(struct monster* monster ){
	assert(monster);
	return monster->dead;
}

void monster_display(struct monster* monster) {//permet d afficher le monstre
	assert(monster);
	window_display_image(sprite_get_monster(monster->direction),monster->x * SIZE_BLOC, monster->y * SIZE_BLOC);
	}
