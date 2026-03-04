#ifndef GAME_FUNC_HEADER_H    // Check if MY_HEADER_H is not defined
#define GAME_FUNC_HEADER_H    // Define MY_HEADER_H to prevent re-inclusion

#include "list_func.h"

// game functions:
int bet_check(int cash, int pot);
int black_jack_check(CardList *list);
void deal(CardList *to, CardList *from, size_t how_many);
void reset_cards(CardList *player, CardList *dealer, CardList *deck);
int hit_or_stand(CardList *player, CardList *dealer, CardList *deck);
int dealer_draw(CardList *player, CardList *dealer, CardList *deck);

#endif  // End of the include guard
