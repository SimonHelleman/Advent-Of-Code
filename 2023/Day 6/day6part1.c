#include <ctype.h>
#include <string.h>
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
    // d = t_held(t - t_held)
    for (size_t i = 0; i < num_races; ++i)
    {
        int win_cnt = 0;
        for (int t_held = 0; t_held <= time[i]; ++t_held)
        {
            int dist_traveled = t_held * (time[i] - t_held);
            if (dist_traveled > winning_dist[i])
            {
                ++win_cnt;
            }
        }
        win_prod = win_prod < 0 ? win_cnt : win_prod * win_cnt;
    }

    printf("%d\n", win_prod);
    return 0;
}