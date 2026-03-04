#ifndef LIST_FUNC_HEADER_H    // Check if MY_HEADER_H is not defined
#define LIST_FUNC_HEADER_H    // Define MY_HEADER_H to prevent re-inclusion

#include <stddef.h> //size_t
#include <stdint.h> //uint8_t
#include <stdbool.h> // true & false
#include <stdio.h> //printf, scanf etc.
#include <stdlib.h> //malloc, exit, srand and rand
#include <stdbool.h> // true & false

#define ANSI_RED "\033[0;31m"
#define ANSI_BLUE "\033[0;34m"
#define ANSI_GREEN "\033[0;32m"
#define ANSI_MAG "\033[0;35m"
#define ANSI_RST "\033[0;30m"

typedef struct Card Card;
struct Card {
    uint8_t data;
    Card *next;
};

typedef struct CardList CardList;
struct CardList {
    Card *head;
    size_t len;
};
//general helper functions
void my_assert(bool condition, const char *error_message);

//list & card functions
void list_init(CardList *list);
Card* new_card(uint8_t data);
void list_push(CardList *list, Card *card);
Card* list_remove_at(CardList *list, size_t pos);

//other list & card functions
void print_card(Card* card);
void list_print(CardList *list, bool dealer);
void clear_input_buffer();
void free_list(CardList* list);

#endif  // End of the include guard
