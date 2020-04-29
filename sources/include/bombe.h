/******************************************************************************/
#ifndef BOMBE_H_
#define BOMBE_H_

#include <map.h>
#include <constant.h>
#include <assert.h>
#include <time.h>

#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>
struct bombe;
 //initialisation d un bombe dans la chaine
 struct bombe* bombeinit(struct player* player,struct map* map);
 //renvoi le pointeur sur la bombe suivante
 struct bombe* bombe_get_next(struct bombe* bombe);
 //appel de fonction permettant de changer les differents etats pour toutes les bombes
 void bombe_chain(struct map* map,struct bombe* bombe,struct player* player);
//permet de lancer l'initialisation d'une bombe si le player a une bombe
 struct bombe* bombe_set(struct player* player,struct map* map);
 //fonction changement d etats
 int bombe_set_orga(struct map* map, struct bombe* bombe,struct player* player);
 //permet de liberer la place occupée par une bombe ou une chaine
 void bombe_free(struct bombe* bombe);
 int bombe_chain_free(struct bombe* bombe);
 //affecte une map a une bombe
 struct map* bombe_get_map(struct bombe* bombe);


#endif
