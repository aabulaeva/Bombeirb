/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef MAP_H_
#define MAP_H_

enum cell_type {
	CELL_EMPTY=0x00,   		 //  0000 0000
	CELL_SCENERY=0x10, 		//  0001 0000
	CELL_BOX=0x20,   		//  0010 0000
	CELL_DOOR=0x30,      	//  0011 0000
	CELL_BONUS=0x50, 		// 	0101 0000
	CELL_MONSTER=0x60, 		// 	0110 0000
	CELL_BOMB=0x70 ,
	CELL_EXP=0x80	   		// 	0111 0000




};
enum door_type{
	OPEN=1,
	CLOSE=0,
};

enum bonus_type {
	KEY=7,
	BONUS_BOMB_RANGE_DEC=4,
	BONUS_BOMB_RANGE_INC=3,
	BONUS_BOMB_NB_DEC=2,
	BONUS_BOMB_NB_INC=1,
	BONUS_MONSTER=5,
	BONUS_LIFE=6,
};

enum scenery_type {
	SCENERY_STONE = 1,    // 0001
	SCENERY_TREE  = 2,    // 0010
	SCENERY_PRINCESS = 4  // 0100
};
enum bombe_ttl {
	BOMB_TTL4 = 4,
	BOMB_TTL3 = 3,
	BOMB_TTL2 = 2,
	BOMB_TTL1 = 1,
};

enum compose_type {
	CELL_TREE     = CELL_SCENERY | SCENERY_TREE,
	CELL_STONE    = CELL_SCENERY | SCENERY_STONE,
	CELL_PRINCESS = CELL_SCENERY | SCENERY_PRINCESS,

  CELL_BOX_RANGEINC = CELL_BOX | BONUS_BOMB_RANGE_DEC,
	CELL_BOX_RANGEDEC = CELL_BOX | BONUS_BOMB_RANGE_INC,
	CELL_BOX_BOMBINC  = CELL_BOX | BONUS_BOMB_NB_DEC,
  CELL_BOX_BOMBDEC  = CELL_BOX | BONUS_BOMB_NB_INC,
  CELL_BOX_LIFE     = CELL_BOX | BONUS_MONSTER,
  CELL_BOX_MONSTER  = CELL_BOX | BONUS_LIFE,

	CELL_BOMB4 = CELL_BOMB | BOMB_TTL4,
	CELL_BOMB3 = CELL_BOMB | BOMB_TTL3,
	CELL_BOMB2 = CELL_BOMB | BOMB_TTL2,
	CELL_BOMB1 = CELL_BOMB | BOMB_TTL1,

	CELL_BONUS_BOMB_RANGE_DEC = CELL_BONUS | BONUS_BOMB_RANGE_DEC,
	CELL_BONUS_BOMB_RANGE_INC = CELL_BONUS | BONUS_BOMB_RANGE_INC,
	CELL_BONUS_BOMB_NB_DEC = CELL_BONUS | BONUS_BOMB_NB_DEC,
	CELL_BONUS_BOMB_NB_INC = CELL_BONUS | BONUS_BOMB_NB_INC,
	CELL_BONUS_MONSTER = CELL_BONUS | BONUS_MONSTER,
	CELL_BONUS_LIFE = CELL_BONUS | BONUS_LIFE,
	CELL_KEY=CELL_BONUS | KEY,

	CELL_DOOR_OPEN= CELL_DOOR | OPEN,
	CELL_DOOR_CLOSE= CELL_DOOR | CLOSE,

	CELL_DOOR_LEVEL0= CELL_DOOR_OPEN ,
	CELL_DOOR_LEVEL1= CELL_DOOR | 3,
	CELL_DOOR_LEVEL2= CELL_DOOR|5,
	CELL_DOOR_LEVEL3= CELL_DOOR |7,
	CELL_DOOR_LEVEL4= CELL_DOOR |9,
	CELL_DOOR_LEVEL5= CELL_DOOR |11,
	CELL_DOOR_LEVEL6= CELL_DOOR |13,
	CELL_DOOR_LEVEL7= CELL_DOOR |15,

	CELL_DOOR_CLOSE0= CELL_DOOR_CLOSE ,
	CELL_DOOR_CLOSE1= CELL_DOOR | 2,
	CELL_DOOR_CLOSE2= CELL_DOOR|4,
	CELL_DOOR_CLOSE3= CELL_DOOR |6,
	CELL_DOOR_CLOSE4= CELL_DOOR |8,
	CELL_DOOR_CLOSE5= CELL_DOOR |10,
	CELL_DOOR_CLOSE6= CELL_DOOR |12,
	CELL_DOOR_CLOSE7= CELL_DOOR |14,
};



struct map;

// Create a new empty map
struct map* map_new(int width, int height);
void map_free(struct map* map);


// Return the height and width of a map
int map_get_width(struct map* map);
int map_get_height(struct map* map);

// Return the type of a cell
enum cell_type map_get_cell_type(struct map* map, int x, int y);
enum compose_type map_get_compose_type(struct map* map, int x, int y);

// Set the type of a cell
void  map_set_cell_type(struct map* map, int x, int y, enum cell_type type);

// Test if (x,y) is within the map
int map_is_inside(struct map* map, int x, int y);

// Return a default static map
struct map* map_get_static();

// Display the map on the screen
void map_display(struct map* map);
void map_bonus(struct map* map,int x,int y);
struct map* mapp_load(int n);


#endif /* MAP_H_ */
