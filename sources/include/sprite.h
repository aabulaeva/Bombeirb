/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef SPRITE_H_
#define SPRITE_H_

#include <SDL/SDL.h>
#include <map.h>
#include <constant.h>

// load game's sprites
void		 sprite_load();

SDL_Surface* sprite_get_player(enum direction direction);
SDL_Surface* sprite_get_monster(enum direction direction);

SDL_Surface* sprite_get_bonus(enum bonus_type bonus_type);
SDL_Surface* sprite_get_tree();
SDL_Surface* sprite_get_box();
SDL_Surface* sprite_get_key();
SDL_Surface* sprite_get_stone();
SDL_Surface* sprite_get_door_opened();
SDL_Surface* sprite_get_door_closed();
SDL_Surface* sprite_get_bomb4();
SDL_Surface* sprite_get_bomb3();
SDL_Surface* sprite_get_bomb2();
SDL_Surface* sprite_get_bomb1();
SDL_Surface* sprite_get_bonus1();
SDL_Surface* sprite_get_bonus2();
SDL_Surface* sprite_get_bonus3();
SDL_Surface* sprite_get_bonus4();
SDL_Surface* sprite_get_bonus6();

SDL_Surface* sprite_get_explosion();
SDL_Surface* sprite_get_flag();
SDL_Surface* sprite_get_princess();




SDL_Surface* sprite_get_number(short number);
SDL_Surface* sprite_get_banner_life();
SDL_Surface* sprite_get_banner_bomb();
SDL_Surface* sprite_get_banner_line();
SDL_Surface* sprite_get_banner_range();
SDL_Surface* sprite_get_pause();

#endif /* SPRITE_H_ */
