#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define LINE_SZ         512
#define WINNING_NUM_SZ  100
#define ACTUAL_NUM_SZ   100

size_t get_numbers_from_string(const char *str, int *arr, size_t arr_len)
{
    // Handle leading spaces
    while (!isdigit(str[0]) && str[0] != '\0')
    {
        ++str;
    }

    size_t count = 0;
    while (str != NULL && str[0] != '\0' && count < arr_len)
    {
        int num;
        sscanf(str, "%d", &num);
        arr[count] = num;
        str = strstr(str, " ");

        if (str != NULL)
        {
            // Handle trailing spaces
            while (!isdigit(str[0]) && str[0] != '\0')
            {
                ++str;
            }
        }
        
        ++count;
    }

    return count;
}


int main(void)
{
    FILE *file = fopen("input.txt", "r");

    size_t copies_cap = 256;
    int *copies = (int *)calloc(copies_cap, sizeof(int));

    unsigned int card_index = 0;
    while (!feof(file))
    {
        copies[card_index] += 1;
        char line[LINE_SZ];
        fgets(line, LINE_SZ, file);

        int winning_numbers[WINNING_NUM_SZ];
        int actual_numbers[ACTUAL_NUM_SZ];

        const char *delim = ":|";
        // First token is the card number, don't care
        strtok(line, delim);
        
        char *winning_str = strtok(NULL, delim);
        char *actual_cards_str = strtok(NULL, delim);

        size_t num_winning = get_numbers_from_string(winning_str, winning_numbers, WINNING_NUM_SZ);
        size_t num_actual = get_numbers_from_string(actual_cards_str, actual_numbers, ACTUAL_NUM_SZ);

        // Time complexity?? nah
        for (int card = 0; card < copies[card_index]; ++card)
        {
            unsigned int matches = 0;
            for (size_t i = 0; i < num_actual; ++i)
            {
                // Linear search for a match :P
                for (size_t j = 0; j < num_winning; ++j)
                {
                    if (actual_numbers[i] == winning_numbers[j])
                    {
                        if (card_index + (++matches) == copies_cap)
                        {
                            size_t old_cap = copies_cap;
                            copies_cap *= 2;
                            copies = (int *)realloc(copies, copies_cap * sizeof(int));
                            memset(copies + old_cap, 0, (copies_cap - old_cap) * sizeof(int));
                        }
                        //printf("card: %d -> new copy of card %cd\n", card_index + 1, card_index + matches + 1);
                        ++copies[card_index + matches];
                        break;
                    }
                }
            }
        }
        ++card_index;
    }
    
    int total_cards = 0;
    for (unsigned int i = 0; i < card_index; ++i)
    {
        //printf("%lld -> %d copies\n", i + 1, copies[i]);
        total_cards += copies[i];
    }
    printf("%d\n", total_cards);

    free(copies);
    fclose(file);
}