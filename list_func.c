#include "list_func.h"

void my_assert(bool condition, const char *error_message) {
    if (condition) {
        printf(ANSI_RED);
        fprintf(stderr, "%s\n", error_message);
        printf(ANSI_RST);
        exit(EXIT_FAILURE);
    }
}

void list_init(CardList *list) {
    my_assert(list == NULL, "list_init: list cannot be NULL");

    list->head = NULL;
    list->len = 0;
}

Card* new_card(uint8_t data) {
    Card *card = malloc(sizeof (Card));
    my_assert(card == NULL, "new_card: malloc failed");
    card->data = data;
    card->next = NULL;

    return card;
}

void list_push(CardList *list, Card *card) {
    my_assert(list == NULL, "list_push: list cannot be NULL");

    card->next = list->head;
    list->head = card;
    list->len += 1;
}

void print_card(Card* card){
    my_assert(card == NULL, "print_card: cannot print a NULL card");

    uint8_t value, suit;
    value = card->data >> 4;
    suit = card->data & 15; // 00001111 = 15 is the mask to only get the suit 
    if (suit == 1 || suit == 4) printf(ANSI_RED);

    switch (value)
    {
    case 1: printf("A"); break;
    case 11: printf("J"); break;
    case 12: printf("Q"); break;
    case 13: printf("K"); break;
    default: printf("%d",value); break;
    }
    switch (suit)
    {
    case 1: printf("\u2665"); break; // Hearts
    case 2: printf("\u2663"); break; // Clubs
    case 4: printf("\u2666"); break; // Diamond
    case 8: printf("\u2660"); break; // Spades
    default: 
        my_assert(1,"print card: error in card suit\n");
        break;
    }
    printf(ANSI_RST);
    

}

void list_print(CardList *list, bool dealer) {
    Card *it = list->head;
    size_t i=0;
    while (it) {
        if (dealer==true && i==list->len-1){
            printf("??");
            break;
        }
        print_card(it);
        it = it->next;
        if (it == NULL) break;
        printf(ANSI_BLUE" & "ANSI_RST);
        i++;
    }
    printf("\n");
}

void clear_input_buffer(){
    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        // Keep reading characters until we encounter a newline or EOF
    }
}

//remove card at a specific position "pos"
Card*  list_remove_at(CardList *list, size_t pos) {
    my_assert(list == NULL, "list_remove_at: list cannot be NULL");
    my_assert(list->len == 0, "list_remove_at: cannot remove from an empty list");
    my_assert(pos >= list->len, "list_remove_at: invalid position, out of bounds");

    Card *prev = NULL;
    Card *curr = list->head;

    for (size_t i = 0; curr != NULL && i < pos; ++i){
        prev = curr;
        curr = curr->next;
    }

    //if curr is NULL it means this position doesn't 
    //exist. return NULL for failure
    if (curr == NULL) {
        return NULL;
    }

    //if curr points to the card at the head of the list
    //update the head
    if (prev == NULL) {
        list->head = curr->next;
    }
    //else update prev->next
    else {
        prev->next = curr->next;
    }
    
    curr->next = NULL;
    list->len -=1;

    return curr;
}

void free_list(CardList* list) {
    struct Card* temp;
    
    while (list->head != NULL) {
        temp = list->head;      // Store current card in temp
        list->head = (list->head)->next; // Move head to the next node
        free(temp);        // Free the current card
    }
}