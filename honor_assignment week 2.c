/* coded by Dennis R. Wong
Oct 12, 2022
Seoul, Korea
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/* This program is used to calculate the probability of the occurrence of
no pair, two pair, three of a kind, full house, and 4 of a kind in
one million generated hands with 7 cards per hand. */

#define CARDS_PER_HAND      7       // we want 7 cards per hand
#define TOTAL_NO_OF_HANDS   1000000 // we want 1 million generated hands
#define CARDS_PER_DECK      52      // this is fixed value.

enum french_suits {SPADES,HEARTS,CLUBS,DIAMONDS};

struct card {
    enum french_suits suit;
    short pip;
};

// cards shuffler
struct card *shuffle(struct card * bs_deck_pointer, struct card * as_deck_pointer) {
    //container for cards before and after shuffle
    short before_shuffle[52], after_shuffle[52];

    //initialize members of before_shuffle {0,1,...,51}
    for(int j = 0; j < 52; j++) {
        before_shuffle[j] = j;
    }

    int card_taken = 0;
    int no_of_cards_left_in_before_shuffle = 52;
    for(int i = 0; i < 52; i++) {
        int pos = rand() % no_of_cards_left_in_before_shuffle;

        // Put card into after_shuffle deck
        after_shuffle[i] = before_shuffle[pos];
        *(as_deck_pointer + i) = *(bs_deck_pointer + before_shuffle[pos]);

        no_of_cards_left_in_before_shuffle--; // reduce one card from before_shuffle deck
        card_taken++;                         // number of card taken from before_shuffle deck

        // Eliminate card number that is already taken
        for (int k = pos; k < 52 - card_taken; k++) {
            before_shuffle[k] = before_shuffle[k+1];
        }
    }
}

// deal 7 cards to each hand
struct card * card_dealer(struct card * coh_pointer, struct card * as_deck_pointer, short card_position) {
    for(int i = 0; i < 7; i++) {
        *(coh_pointer + i) = *(as_deck_pointer + card_position);
        card_position++;
    }
}

void combination_checker(struct card * coh_pointer, int* occurence) // check what combination in a hand
{
    int counter[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};      // count howmany times pips appear, max 4
    int checker[5]  = {0,0,0,0,0};                      // count howmany kinds appear, {0 times ~ 4 times}

    for(int i = 0; i < CARDS_PER_HAND; i++)
        counter[(*(coh_pointer + i)).pip - 1] += 1;

    for(int j = 0; j < 13; j++)
        checker[counter[j]] += 1;

    if (checker[4] == 1)
        occurence[5]++;
    else if (checker[3] >= 1 && checker[2] >= 1)
        occurence[4]++;
    else if (checker[3] >= 1 && checker[2] >= 0)
        occurence[3]++;
    else if (checker[2] == 2)
        occurence[2]++;
    else if (checker[2] == 1)
        occurence[1]++;
    else if (checker[1] == 7)
        occurence[0]++;
}

int main(void) {
    // 52 is not divisible by 7, so we count max possible number of hands per shuffled deck.
    short possible_hand_per_deck = floor(CARDS_PER_DECK / CARDS_PER_HAND);
    int no_of_decks = ceil(TOTAL_NO_OF_HANDS / possible_hand_per_deck);

    // set random based on time
    srand(time(0));

    // prepare cards with position in order
    struct card bs_deck[52];                    // before shuffle(bs) deck / sorted deck {A♠,2♠,...Q♦,K♦}
    struct card card_profile;
    short i = 0;                                // i = card position in deck {0,1,...,51}, assume 0=A♠, 51=K♦
    for(int j = 1; j <= 13; j++) {              // card pip {1,2...,13}, assume 0=Ace, 11=Jack, 12=Queen, 13=King
        for(int k = 1; k <= 4; k++) {           // card suit {1,2,3,4}, assume 1=Spades(♠), 2=Hearts(♥), 3=Clubs(♣), 4=Diamonds(♦)
            card_profile.suit = k;
            card_profile.pip = j;
            bs_deck[i] = card_profile;
            i++;
        }
    }

    // prepare deck for after shuffle
    struct card as_deck[52];    // after shuffle(as) deck
    struct card cards_on_hand[7];

    // count howmany occurence in each type occurence[0] = no pair; ..., occurence[5] = full_house
    int occurence[6] = {0,0,0,0,0,0};

    // shuffle and check cards on hand
    short card_position;
    for(int l  = 0; l < no_of_decks; l++) {
        shuffle(&bs_deck[0], &as_deck[0]);
        card_position = 0;
        for(int j = 0; j < possible_hand_per_deck; j++) {
            card_dealer(&cards_on_hand[0], &as_deck[0], card_position);
            combination_checker(&cards_on_hand[0], &occurence[0]);
            card_position += 7;
        }
    }

    printf("Probability of each occurrence :\n");
    printf("1. no pair         = %lf\n", (double) occurence[0] / TOTAL_NO_OF_HANDS);
    printf("2. one pair        = %lf\n", (double) occurence[1] / TOTAL_NO_OF_HANDS);
    printf("3. two pair        = %lf\n", (double) occurence[2] / TOTAL_NO_OF_HANDS);
    printf("4. three of a kind = %lf\n", (double) occurence[3] / TOTAL_NO_OF_HANDS);
    printf("5. full house      = %lf\n", (double) occurence[4] / TOTAL_NO_OF_HANDS);
    printf("6. 4 of a kind     = %lf\n", (double) occurence[5] / TOTAL_NO_OF_HANDS);

    return 0;
}
