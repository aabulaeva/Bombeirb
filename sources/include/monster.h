#ifndef MONSTER_H_
#define MONSTER_H_

#include <map.h>
#include <constant.h>

struct monster;

// Creates a new player with a given number of available bombs
struct monster* monster_init(int x, int y,struct monster* monster,struct player* player);
void   monster_free(struct monster* monster);

// Set the position of the player
void monster_set_position(struct monster *monster, int x, int y);

// Returns the current position of the player
int monster_get_x(struct monster* monster);
int monster_get_y(struct monster* monster);


// Set the direction of the next move of the player
void monster_set_current_way(struct monster * monster, enum direction direction);

// Move the player according to the current direction
int monster_move(struct monster* monster, struct map* map,int a,struct player* player);

// Display the player on the screen
void monster_display(struct monster* monster);
//monstre mort (ne bouge plus mais reste sur la carte,ne blesse plus le player)
int monster_dead(struct monster* monster);
//renvoi l'etat d'un mostre vivant ou mort
int monster_state(struct monster* monster );


#endif /* MONSTER_H_ */
