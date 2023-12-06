#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#define LINE_SZ 512
#define RACE_SZ 10

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

    int time[RACE_SZ];
    int winning_dist[RACE_SZ];
    size_t num_races = 0;

    while (!feof(file))
    {
        char line[LINE_SZ];

        if (fgets(line, LINE_SZ, file) == NULL)
        {
            break; // handle new line at eof.
        }

        strtok(line, ":");

        if (strstr(line, "Time") != NULL)
        {
            num_races = get_numbers_from_string(strtok(NULL, ":"), time, RACE_SZ);
        }

        if (strstr(line, "Distance") != NULL)
        {
            num_races = get_numbers_from_string(strtok(NULL, ":"), winning_dist, RACE_SZ);
        }
    }

    fclose(file);

    int win_prod = -1;
    for (size_t i = 0; i < num_races; ++i)
    {
        int min_th = (int)ceil((time[i] - sqrt((time[i] * time[i]) - (4 * winning_dist[i]))) / 2.0);
        int max_th = (int)floor((time[i] + sqrt((time[i] * time[i]) - (4 * winning_dist[i]))) / 2.0);

        int num_win = max_th - min_th + 1;
        win_prod = win_prod < 0 ? num_win : win_prod * num_win;
    }

    printf("%d\n", win_prod);
    return 0;
}