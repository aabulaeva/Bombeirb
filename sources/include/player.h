/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef PLAYER_H_
#define PLAYER_H_

#include <map.h>
#include <constant.h>

struct player;

// Creates a new player with a given number of available bombs
struct player* player_init(int bomb_number);
void   player_free(struct player* player);
//gerer les bombes
struct bombe* player_bombe_update(struct player* player);
struct bombe* player_first(struct player* player);//racine
struct bombe* player_get_next(struct player* player,struct bombe* prec,struct bombe* bombe);//nouvelle bombe suivante apres bombe prec
struct bombe* player_next(struct player* player,struct bombe* prec);//bombe suivante de bombe
struct bombe* player_get_first(struct player* player,struct bombe* bombe);//premiere racine
struct bombe* player_get_last(struct player* player,struct bombe* bombe);//nouvelle queue
struct bombe* player_last(struct player* player);//queue

//gerer les bonus
void player_get_bonus(struct player* player, int type);



// Set the position of the player
void player_set_position(struct player *player, int x, int y);



// Returns the current position of the player
int player_get_x(struct player* player);
int player_get_y(struct player* player);

// Set the direction of the next move of the player
void player_set_current_way(struct player * player, enum direction direction);

// Set, Increase, Decrease the number of bomb,lives,range,key that player can use
int  player_get_nb_bomb(struct player * player);
void player_inc_nb_bomb(struct player * player);
void player_dec_nb_bomb(struct player * player);
int  player_get_nb_lives(struct player * player);
void player_inc_nb_lives(struct player * player);
void player_dec_nb_lives(struct player * player);
int player_get_range(struct player* player);
int player_inc_range(struct player* player);
int player_dec_range(struct player* player);
int player_inc_key(struct player* player);
int player_dec_key(struct player* player);
int player_get_key(struct player* player);

int player_princess(struct player* player,int type);

// Move the player according to the current direction
int player_move(struct player* player, struct map* map);

// Display the player on the screen
void player_display(struct player* player);


#endif /* PLAYER_H_ */
