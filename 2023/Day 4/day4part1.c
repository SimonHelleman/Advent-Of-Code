#include <ctype.h>
#include <string.h>
#include <stdio.h>
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

    int score_sum = 0;
    while (!feof(file))
    {

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

        int score = 0;

        for (size_t i = 0; i < num_actual; ++i)
        {
            // Linear search for a match :P
            for (size_t j = 0; j < num_winning; ++j)
            {
                if (actual_numbers[i] == winning_numbers[j])
                {
                    score = score > 0 ? score * 2 : 1;
                    break;
                }
            }
        }

        score_sum += score;
        //printf("score: %d\n", score);
    }

    printf("%d\n", score_sum);

    fclose(file);
}