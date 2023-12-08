#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define LINE_SZ 512

#define NUM_CARDS 13
#define CARDS_PER_HAND 5

const char *g_card = "23456789TJQKA";

typedef enum
{
    HighCard,
    OnePair,
    TwoPair,
    ThreeKind,
    FullHouse,
    FourKind,
    FiveKind
} HandType;

typedef struct
{
    char card[CARDS_PER_HAND + 1];
    unsigned int bid;
    HandType type;
} Hand;

// I don't want to search
// and this is C, I don't have a fancy map to put
// these in.
int map_card_to_index(char c)
{
    switch(c)
    {
    case '2':
        return 0;
    case '3':
        return 1;
    case '4':
        return 2;
    case '5':
        return 3;
    case '6':
        return 4;
    case '7':
        return 5;
    case '8':
        return 6;
    case '9':
        return 7;
    case 'T':
        return 8;
    case 'J':
        return 9;
    case 'Q':
        return 10;
    case 'K':
        return 11;
    case 'A':
        return 12;
    }
    return -1;
}

int compare_hand_by_type(const void *arg0, const void *arg1)
{
    const Hand *a = (const Hand *)arg0;
    const Hand *b = (const Hand *)arg1;
    
    if (a->type < b->type) return -1;
    if (a->type > b->type) return 1;
    return 0;
}

int compare_hand_by_cards(const void *arg0, const void *arg1)
{
    const Hand *a = (const Hand *)arg0;
    const Hand *b = (const Hand *)arg1;
    
    for (int i = 0; i < CARDS_PER_HAND; ++i)
    {
        if (a->card[i] != b->card[i])
        {
            return map_card_to_index(a->card[i]) - map_card_to_index(b->card[i]);
        }
    }

    return 0;
}

int main(void)
{
    FILE *file = fopen("input.txt", "r");

    size_t hand_cap = 100;
    size_t hand_len = 0;

    Hand *hand = (Hand *)malloc(hand_cap * sizeof(Hand)); 

    while (!feof(file))
    {
        char line[LINE_SZ];

        // handle new line at eof
        if (fgets(line, LINE_SZ, file) == NULL) break;

        if (hand_len == hand_cap)
        {
            hand_cap *= 2;
            hand = (Hand *)realloc(hand, hand_cap * sizeof(Hand));
        }

        sscanf(line, "%5s %d", hand[hand_len].card, &hand[hand_len].bid);

        hand[hand_len].type = HighCard;

        ++hand_len;
    }

    fclose(file);

    for (size_t i = 0; i < hand_len; ++i)
    {
        Hand *h = &hand[i];

        int card_cnt[NUM_CARDS];
        memset(card_cnt, 0, sizeof(card_cnt));

        for (int j = 0; j < CARDS_PER_HAND; ++j)
        {
            ++card_cnt[map_card_to_index(h->card[j])];
        }

        for (int j = 0; j < NUM_CARDS; ++j)
        {
            if (card_cnt[j] == 5)
            {
                h->type = FiveKind;
                break;
            }
            
            if (card_cnt[j] == 4)
            {
                h->type = FourKind;
                break;
            }

            if (card_cnt[j] == 3)
            {
                char other[2];
                int n = 0;
                // Find the other two cards
                for (int c = 0; c < CARDS_PER_HAND; ++c)
                {
                    if (h->card[c] != g_card[j])
                    {
                        other[n++] = h->card[c];
                    }
                }

                h->type = other[0] == other[1] ? FullHouse : ThreeKind;
                break;
            }

            if (card_cnt[j] == 2)
            {
                bool valid = true;
                for (int k = j; k < NUM_CARDS; ++k)
                {
                    if (card_cnt[k] > 2)
                    {
                        // There is set of three, skip
                        valid = false;
                        break;
                    }
                }

                if (valid)
                {
                    char other[3];
                    int n = 0;
                    for (int c = 0; c < CARDS_PER_HAND; ++c)
                    {
                        if (h->card[c] != g_card[j])
                        {
                            other[n++] = h->card[c];
                        }
                    }

                    h->type = other[0] == other[1] || other[0] == other[2] || other[1] == other[2] ? TwoPair : OnePair;
                    break;
                }
            }
        }
    }

    qsort(hand, hand_len, sizeof(Hand), compare_hand_by_type);

    size_t type_cnt[7];
    memset(type_cnt, 0, sizeof(type_cnt));

    for (size_t i = 0; i < hand_len; ++i)
    {
        ++type_cnt[hand[i].type];
    }

    size_t start_idx = 0;
    for (size_t i = 0; i < 7; ++i)
    {
        if (type_cnt[i] > 0)
        {
            qsort(&hand[start_idx], type_cnt[i], sizeof(Hand), compare_hand_by_cards);
            start_idx += type_cnt[i];
        }
    }

    size_t total = 0;
    for (size_t i = 0; i < hand_len; ++i)
    {
        total += hand[i].bid * (i + 1);
    }

    printf("%lld\n", total);

    free(hand);
    return 0;
}
