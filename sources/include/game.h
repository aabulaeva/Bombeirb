/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef GAME_H_
#define GAME_H_

#include <player.h>
#include <map.h>
#include <monster.h>

// Abstract data type
struct game;

// Create a new game
struct game* game_new();

// Free a game
void game_free(struct game* game);
struct bombe* game_get_first(struct game* game,struct bombe* bombe);

// Return the player and the monster's tab of the current game
struct player* game_get_player(struct game* game);
struct monster** game_get_monster(struct game* game);
//permet le deplacement des monstres
int game_monster(struct game* game);
// Return the current map
struct map* game_get_current_map(struct game* game);
//permet de diminuer la vie du player lorsqu'il rencontre un monstre
int game_player_monster(struct game* game);

// Display the game on the screen
void game_display(struct game* game);

// update
int game_update(struct game* game);
//update des etats de bombes
struct bombe* game_bombe_update(struct player* player,struct game* game);
//rencontre du player avec une porte
int game_player_win(struct player* player,int type);
//level augmente de 1
void game_get_level(struct game* game);
//renvoi le level
int game_level(struct game* game);
//on change de porte suivant le sous type de porte (ouverte ou ferme) et on appelle la fonction permettant de changer de level et donc de map
int game_change_map(struct game* game,int type);
//on change le level
void game_choice_level(struct game* game);
//on arrive au level 0
int game_set_level_dead(struct player* player);
//on circule dans le tableau de monstre afin de verifier la cellule qu'ils occupent,si cest une flamme,on appelle la fonction qui les tue
void game_dead_monster(struct game* game);
//met en pause le jeu
int  game_pause();

#endif /* GAME_H_ */
