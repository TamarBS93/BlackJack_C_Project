#include <stdio.h> //printf, scanf etc.
#include <math.h> // pow
#include <time.h> // time

#include "game_func.h"

// MAIN:
int main(void) {
    CardList deck, player_hand, dealer_hand;
     
    // Deck creation:
    list_init(&deck);
    // data = rank << 4 | suit
    for (int i=1; i<14 ; i++){
        list_push(&deck, new_card((i<<4) | 1));
        list_push(&deck, new_card((i<<4) | 2));
        list_push(&deck, new_card((i<<4) | 4));
        list_push(&deck, new_card((i<<4) | 8));
    }
    my_assert(deck.len != 52, "Deck initialization failed: incorrect number of cards");

    // 1. Game Initiation:
    int cash, pot;
    cash = 1000;
    pot = 0;
    list_init(&player_hand);
    list_init(&dealer_hand);
    srand(time(NULL));

    //ansi clear screen
    printf("\033[2J\033[H");

    // Game On
    int bet;
    while (1){
      
        printf(ANSI_BLUE);
        printf("Player's cash: %d\n", cash);
        printf("Game's pot: %d\n\n", pot);
        printf(ANSI_RST);
        // 2. Betting:
        bet = bet_check(cash, pot); // -2: quit, -1: invalid input else: player's bet
        if (bet == -2) {
            free_list(&deck);
            break;
        }
        else if (bet == -1) continue;
        else {
            cash -= bet;
            pot += bet;
        }
        // 3. Initial Deal:
        deal(&player_hand,&deck,2);
        deal(&dealer_hand,&deck,2);
        printf(ANSI_BLUE"Player's hand: "ANSI_RST);
        list_print(&player_hand,false);
        printf(ANSI_BLUE"Dealer's hand: "ANSI_RST);
        list_print(&dealer_hand,true);
        puts("");

        if (black_jack_check(&player_hand) == 21){
            printf(ANSI_GREEN"Black Jack!\n");
            printf("You had %d and won %g\n"ANSI_RST, cash ,(float)pot*2.5);
            cash += pot*2.5;
            pot = 0;
            reset_cards(&player_hand, &dealer_hand, &deck);
        }
        else{
            int result = hit_or_stand(&player_hand, &dealer_hand, &deck);
            // -1:loss ; 0: tie ; 1: player win ; 2: black jack 
            switch (result)
            {
            case -1: //loss
                reset_cards(&player_hand, &dealer_hand, &deck);
                printf(ANSI_MAG"you lost your bet (%d).\n"ANSI_RST, bet);
                pot = 0;
                break;
            case 0: // tie
                printf("You don't win any cash\n"ANSI_RST);
                reset_cards(&player_hand, &dealer_hand, &deck);
                break;
            case 1: // player win
                printf("You had %d and won %d!\n"ANSI_RST, cash, pot*2);
                reset_cards(&player_hand, &dealer_hand, &deck);
                cash += pot*2;
                pot = 0;
                break; 
            case 2: // black jack
                printf(ANSI_GREEN"Black Jack!\n");
                printf("You had %d and won %g\n"ANSI_RST, cash ,(float)pot*2.5);
                reset_cards(&player_hand, &dealer_hand, &deck);
                cash += pot*2.5;
                pot = 0;
                break; 
            // default:
            //     break;
            }
        }
    }
 
    return 0;
}


