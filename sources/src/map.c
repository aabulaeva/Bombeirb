/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <map.h>
#include <constant.h>
#include <misc.h>
#include <sprite.h>
#include <window.h>
#include <game.h>

struct map {
	int width;
	int height;
	unsigned char* grid;
};

#define CELL(i,j) ( (i) + (j) * map->width)

struct map* map_new(int width, int height)
{
	assert(width > 0 && height > 0);

	struct map* map = malloc(sizeof (*map));
	if (map == NULL )
		error("map_new : malloc map failed");

	map->width = width;
	map->height = height;

	map->grid = malloc(height * width);
	if (map->grid == NULL) {
		error("map_new : malloc grid failed");
	}

	// Grid cleaning
	int i, j;
	for (i = 0; i < width; i++)
	  for (j = 0; j < height; j++)
	    map->grid[CELL(i,j)] = CELL_EMPTY;

	return map;
}

int map_is_inside(struct map* map, int x, int y)
{	if ((x>= 0 ) && (y>=0) && (x<(map->width)) && (y<(map->height))) {
	assert(map);
	return 1;}
 	else {
		return 0;
	}
}

void map_free(struct map *map)
{
	if (map == NULL )
		return;
	free(map->grid);
	free(map);
}

int map_get_width(struct map* map)
{
	assert(map != NULL);
	return map->width;
}

int map_get_height(struct map* map)
{
	assert(map);
	return map->height;
}

enum cell_type map_get_cell_type(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 0xf0;
}
enum compose_type map_get_compose_type(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 0xff;
}

void map_set_cell_type(struct map* map, int x, int y, enum cell_type type)
{
	assert(map && map_is_inside(map, x, y));
	map->grid[CELL(x,y)] = type;
}

void display_bonus(struct map* map, int x, int y, unsigned char type)
{
	// bonus is encoded with the 4 most significant bits
	switch (type & 0x0f) {
	case BONUS_BOMB_RANGE_INC:
		window_display_image(sprite_get_bonus3(), x, y);
		break;

	case BONUS_BOMB_RANGE_DEC:
		window_display_image(sprite_get_bonus4(), x, y);
		break;

	case BONUS_BOMB_NB_DEC:
		window_display_image(sprite_get_bonus2(), x, y);
		break;

	case BONUS_BOMB_NB_INC:
		window_display_image(sprite_get_bonus1(), x, y);
		break;
	case BONUS_LIFE:
		window_display_image(sprite_get_bonus6(), x, y);
		break;
	case KEY:
		window_display_image(sprite_get_key(), x, y);
		break;


	}
}

void display_scenery(struct map* map, int x, int  y, unsigned char type)
{
	switch (type & 0x0f) { // sub-types are encoded with the 4 less significant bits
	case SCENERY_STONE:
		window_display_image(sprite_get_stone(), x, y);
		break;

	case SCENERY_TREE:
		window_display_image(sprite_get_tree(), x, y);
		break;
	case SCENERY_PRINCESS:
		window_display_image(sprite_get_princess(),x,y);
	}
}
void display_door(struct map* map, int x, int  y, unsigned char type)
{
	switch (type & 0xf1) { // sub-types are encoded with the 4 less significant bits
		case CELL_DOOR_OPEN:
			window_display_image(sprite_get_door_opened(), x, y);
			break;

		case CELL_DOOR_CLOSE:
			window_display_image(sprite_get_door_closed(), x, y);
			break;
	}
}
void display_bombe_ttl(struct map* map, int x, int  y, unsigned char type)
{
	switch (type & 0x0f) { // sub-types are encoded with the 4 less significant bits
	case BOMB_TTL4:
		window_display_image(sprite_get_bomb4(), x, y);
		break;

	case BOMB_TTL3:
		window_display_image(sprite_get_bomb3(), x, y);
		break;

	case BOMB_TTL2:
		window_display_image(sprite_get_bomb2(), x, y);
		break;

	case BOMB_TTL1:
		window_display_image(sprite_get_bomb1(), x, y);
		break;
	}
}

void map_display(struct map* map)
{
	assert(map != NULL);
	assert(map->height > 0 && map->width > 0);

	int x, y;
	for (int i = 0; i < map->width; i++) {
	  for (int j = 0; j < map->height; j++) {
	    x = i * SIZE_BLOC;
	    y = j * SIZE_BLOC;

	    unsigned char type = map->grid[CELL(i,j)];

	    switch (type & 0xf0) {
		case CELL_SCENERY:
		  display_scenery(map, x, y, type);
		  break;
	    case CELL_BOX:
	      window_display_image(sprite_get_box(), x, y);
	      break;
	    case CELL_BONUS:
	      display_bonus(map, x, y, type);
	      break;
	    case CELL_DOOR:
	      display_door(map,x,y,type);
	      break;

			case CELL_BOMB:
				display_bombe_ttl(map,x,y,type);
				break;
			case CELL_EXP:
				window_display_image(sprite_get_explosion(),x,y);

	    }
	  }
	}
}

void map_bonus(struct map* map,int x,int y){
	int r= rand();
	switch (r%7) {
		case 0:
			map_set_cell_type(map,x,y,0x51);
			break;
		case 1:
			map_set_cell_type(map,x,y,0x52);
			break;
		case 2:
			map_set_cell_type(map,x,y,0x53);
			break;
		case 3:
			map_set_cell_type(map,x,y,0x54);
			break;
		case 5:
		 	map_set_cell_type(map,x,y,CELL_EMPTY);
			break;
		case 6:
			map_set_cell_type(map,x,y,CELL_BONUS_LIFE);
			break;



	}
}
struct map* mapp_load(int n){
	struct map* map;
	int i,j,num;
	FILE* fichier= NULL;

	int width=0;
	int height=0;
	switch (n) {
		case 1:
			fichier = fopen("data/map_1","r+" );
			fscanf(fichier,"%d:%d", &width,&height);
			map=map_new(width,height);
			for (i = 0; i < width; i++) {
				for ( j = 0; j < height; j++) {
					fscanf(fichier,"%d",&num);
					map->grid[CELL(j,i)]=num;
				}
			}
			fclose(fichier);
			break;
		case 2:
			fichier = fopen("data/map_2","r+" );
			fscanf(fichier,"%d:%d", &width,&height);
			map=map_new(width,height);
			for (i = 0; i < width; i++) {
				for ( j = 0; j < height; j++) {
					fscanf(fichier,"%d",&num);
					map->grid[CELL(j,i)]=num;
				}
			}
			fclose(fichier);
			break;

		case 3:
			fichier = fopen("data/map_3","r+" );
			fscanf(fichier,"%d:%d", &width,&height);
			map=map_new(width,height);
			for (i = 0; i < width; i++) {
				for ( j = 0; j < height; j++) {
					fscanf(fichier,"%d",&num);
					map->grid[CELL(j,i)]=num;
					}
				}
			fclose(fichier);
			break;
		case 4:
			fichier= fopen("data/map_4","r+" );
			fscanf(fichier,"%d:%d", &width,&height);
			map=map_new(width,height);
			for (i = 0; i < width; i++) {
				for ( j = 0; j < height; j++) {
					fscanf(fichier,"%d",&num);
					map->grid[CELL(j,i)]=num;
					}
				}
			fclose(fichier);
			break;
		case 5:
			fichier = fopen("data/map_5","r+" );
			fscanf(fichier,"%d:%d", &width,&height);
			map=map_new(width,height);
			for (i = 0; i < width; i++) {
				for ( j = 0; j < height; j++) {
					fscanf(fichier,"%d",&num);
					map->grid[CELL(j,i)]=num;
					}
				}
			fclose(fichier);
			break;
		case 6:
			fichier = fopen("data/map_6","r+" );
			fscanf(fichier,"%d:%d", &width,&height);
			map=map_new(width,height);
			for (i = 0; i < width; i++) {
				for ( j = 0; j < height; j++) {
					fscanf(fichier,"%d",&num);
					map->grid[CELL(j,i)]=num;
					}
				}
			fclose(fichier);
			break;
		case 7:
			fichier = fopen("data/map_7","r+" );
			fscanf(fichier,"%d:%d", &width,&height);
			map=map_new(width,height);
			for (i = 0; i < width; i++) {
				for ( j = 0; j < height; j++) {
					fscanf(fichier,"%d",&num);
					map->grid[CELL(j,i)]=num;
					}
				}
			fclose(fichier);
			break;
		}
return map;
}

struct map* map_get_static()
{
	struct map* map = map_new(STATIC_MAP_WIDTH, STATIC_MAP_HEIGHT);

	unsigned char themap[STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT] = {
	  CELL_STONE, CELL_STONE , CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE,
	  CELL_STONE, CELL_STONE , CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE,
	  CELL_STONE, CELL_STONE , CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE,
	  CELL_EMPTY, CELL_EMPTY , CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
	  CELL_EXP, CELL_EXP, CELL_EMPTY, CELL_BOMB4, CELL_BOMB4, CELL_BOMB4, CELL_EMPTY, CELL_EXP, CELL_EMPTY, CELL_BOMB4, CELL_BOMB4, CELL_EMPTY,
	  CELL_EXP, CELL_EMPTY, CELL_EXP, CELL_BOMB4, CELL_EMPTY, CELL_EMPTY, CELL_EXP, CELL_EMPTY, CELL_EXP, CELL_BOMB4, CELL_EMPTY, CELL_BOMB4,
	  CELL_EXP, CELL_EMPTY, CELL_EXP, CELL_BOMB4, CELL_BOMB4, CELL_EMPTY, CELL_EXP, CELL_EXP, CELL_EXP, CELL_BOMB4, CELL_EMPTY, CELL_BOMB4,
	  CELL_EXP, CELL_EMPTY, CELL_EXP, CELL_BOMB4, CELL_EMPTY, CELL_EMPTY, CELL_EXP, CELL_EMPTY, CELL_EXP, CELL_BOMB4, CELL_EMPTY, CELL_BOMB4,
	  CELL_EXP, CELL_EXP, CELL_EMPTY, CELL_BOMB4, CELL_BOMB4, CELL_BOMB4, CELL_EXP, CELL_EMPTY, CELL_EXP, CELL_BOMB4, CELL_BOMB4, CELL_EMPTY,
		CELL_EMPTY, CELL_EMPTY , CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
	  CELL_STONE, CELL_STONE , CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE,
	  CELL_STONE, CELL_STONE , CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE,
		};

	for (int i = 0; i < STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT; i++)
		map->grid[i] = themap[i];

	return map;
}
