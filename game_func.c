#include "game_func.h"

int bet_check(int cash, int pot){
    if (pot == 0 && cash <= 10){
        printf(ANSI_RED"Out of cash to bet. Game Over!\n\n"ANSI_RST);
        return -2;
    }
    int bet;
    printf(ANSI_BLUE"Do you want to bet? [Y/N]?"ANSI_RST);
    //clear_input_buffer();
    char ch;
    char y_n = scanf(" %c", &ch);
    if (y_n != 1) {
        clear_input_buffer();
        //ansi clear screen
        printf("\033[2J\033[H"); 
        printf(ANSI_RED"Invalid input:\nPlease try again.\n"ANSI_RST);
        return -1;
    }

    switch (ch)
    {
    case 'N':
    case 'n':
        //ansi clear screen
        printf("\033[2J\033[H");
        printf(ANSI_MAG"You chose to quit.\nYou earned %d cash!\n"ANSI_RST,cash);
        return -2;
    case 'Y': 
    case 'y':
        //ansi clear screen
        printf("\033[2J\033[H");
        break;
    default:
        //ansi clear screen
        printf("\033[2J\033[H");
        printf(ANSI_RED"Invalid input\nplease try again\n"ANSI_RST);
        return -1;
    }

    while(1){
        clear_input_buffer();
        printf(ANSI_BLUE);
        printf("Player's cash: %d\n", cash);
        printf("Game's pot: %d\n\n", pot);
        printf(ANSI_BLUE"please enter your bet (in multiplications of 10): "ANSI_RST);
        if (scanf(" %d", &bet) !=1){
            // not an integer 
            //ansi clear screen
            printf("\033[2J\033[H"); 
            printf(ANSI_RED"Invalid input:\nPlease enter an integer\n"ANSI_RST);
            continue;
        } 
        if (bet == 0 && pot == 0) {
            // bet with 0 cash
            //ansi clear screen
            printf("\033[2J\033[H"); 
            printf(ANSI_RED"Invalid input:\nYou can't bet '0' because there is no money in the pot\nPlease enter a valid bet\n"ANSI_RST);
            continue;;
        }
        if (bet < 0 || bet % 10 != 0) {
            // invalid integer 
            //ansi clear screen
            printf("\033[2J\033[H"); 
            printf(ANSI_RED"Invalid input:\n%d is not a positive multiplication of 10\nPlease enter a valid bet\n"ANSI_RST, bet);
            continue;
        }
        if (bet > cash) {
            // invalid bet
            //ansi clear screen
            printf("\033[2J\033[H"); 
            printf(ANSI_RED"Invalid input:\nYou don't have enough cash\nPlease enter a valid bet\n"ANSI_RST);
            continue;;
        }
        //ansi clear screen
        printf("\033[2J\033[H");    
        printf(ANSI_BLUE);
        printf("Player's cash: %d\n", cash-bet);
        printf("Game's pot: %d\n\n", pot+bet);
        printf(ANSI_RST);
        return bet;
    }

}

void deal(CardList *to, CardList *from, size_t how_many){
    my_assert(to == NULL, "deal: destination list cannot be null");
    my_assert(from == NULL, "deal: source list cannot be null");
    my_assert(how_many > from->len, "deal: not enough cards in the deck");

    for (size_t i=1; i<=how_many; i++){
        list_push(to, list_remove_at(from,rand() % from->len));
    }
}

// 3. Black Jack Check
int black_jack_check(CardList *list) {
    my_assert(list == NULL, "black_jack_check: list cannot be NULL");

    int sum_result = 0;
    int rank;
    int flag=0;

    for (Card *it = list->head; it; it = it->next) {
        rank = (it->data) >> 4;
        if (rank ==1 ) flag =1;
        if (rank>10) rank = 10;
        sum_result += rank;
    }
    if(flag && sum_result <= 11) sum_result += 10;

    return sum_result;
}

// 4. Hit or Stand:
// returns: -1:loss ; 0: tie ; 1: player win ; 2: black jack 
int hit_or_stand(CardList *player, CardList *dealer, CardList *deck){
    char choice='h';
    int sum;
    printf(ANSI_BLUE"What is your next move? Hit ('h') or Stand ('s')? "ANSI_RST);
    scanf(" %c", &choice);
    clear_input_buffer();
    //ansi clear screen
    printf("\033[2J\033[H");

    if ((choice =='s') | (choice =='S')){ 
        printf(ANSI_BLUE"Your hand: "ANSI_RST);
        list_print(player, false);
        return dealer_draw(player, dealer, deck);
    }
    else if ((choice == 'h')| (choice =='H')){
        deal(player, deck, 1);
        printf(ANSI_BLUE"Your hand: "ANSI_RST);
        list_print(player, false);
        printf(ANSI_BLUE"Dealer's hand: "ANSI_RST);
        list_print(dealer,true);
        puts("");
        sum = black_jack_check(player);
        if (sum==21) return 2;
        if (sum>21){
            printf(ANSI_MAG"bust!\n"ANSI_RST);
            return -1; // lose bet
        }
        return hit_or_stand(player, dealer, deck);
    }
    else{
        printf(ANSI_RED"Invalid input: type 'h' (Hit) or 's' (Stand)\n"ANSI_RST);
        printf(ANSI_BLUE"Player's hand: "ANSI_RST);
        list_print(player,false);
        printf(ANSI_BLUE"Dealer's hand: "ANSI_RST);
        list_print(dealer,true);
        puts("");
        return hit_or_stand(player, dealer, deck);

    }
}

/* 5. Dealer draw:
    returns: -1:loss ; 0: tie ; 1: player win ; 2: black jack */
int dealer_draw(CardList *player, CardList *dealer, CardList *deck){
    my_assert(player == NULL, "dealer_draw: player list cannot be NULL");
    my_assert(dealer == NULL, "dealer_draw: dealer list cannot be NULL");
    my_assert(deck == NULL, "dealer_draw: deck cannot be NULL");
    
    my_assert(deck->len == 0, "dealer_draw: not enough cards in the deck");

    int player_sum = black_jack_check(player);
    int dealer_sum=black_jack_check(dealer);

    while (dealer_sum <= player_sum){
        deal(dealer, deck, 1);
        dealer_sum = black_jack_check(dealer);
        if (dealer_sum >= 17) break;
    }
    printf(ANSI_BLUE"dealer's hand: "ANSI_RST);
    list_print(dealer, false);
    puts("");
    if (dealer_sum > 21){
        printf(ANSI_GREEN"dealer bust!\n");
        return 1;
    }
    if (dealer_sum < player_sum){
        printf(ANSI_GREEN"you win!\n");
        return 1;
    } 
    if (dealer_sum == player_sum){
        printf(ANSI_MAG"tie!\n");
        return 0;
    } 
    printf(ANSI_MAG"dealer win!\n");
    return -1;
}

// 6. Reset Cards:
void reset_cards(CardList *player, CardList *dealer, CardList *deck){
    my_assert(player == NULL, "reset_cards: player list cannot be NULL");
    my_assert(dealer == NULL, "reset_cards: dealer list cannot be NULL");
    my_assert(deck == NULL, "reset_cards: deck cannot be NULL");

    if (player->len > 0) deal(deck, player, player->len);
    if (dealer->len > 0) deal(deck, dealer, dealer->len);
}
