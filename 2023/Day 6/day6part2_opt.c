#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
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

// Leaving this as unused since it *does* work as long
// as the resulting number isn't huge. In other words,
// I am proud of my math.
uint64_t combine_digits_math(int *arr, size_t arr_len)
{
    int n = 0;
    uint64_t ret = 0;
    // 10^n ((t/10^n) + d)
    for (size_t i = arr_len - 1; i != (size_t)(-1); --i)
    {
        while (arr[i] > 0)
        {
            // fun fact, I tried using floats, and they weren't
            // precise enough, double worked fine, at first
            // until trying to parse the distance in the input...
            // long double still wasn't precise enough :(
            long double digit = (long double)(arr[i] % 10);
            long double power = pow(10, n++);
            ret = (uint64_t)(power * (((double)ret / power) + digit));
            arr[i] /= 10;
        }
    }
    return ret;
}

// Worse solution that works
uint64_t combine_digits(int *arr, size_t arr_len)
{
    size_t digit_cnt = 0;
    for (size_t i = 0; i < arr_len; ++i)
    {
        int val = arr[i];
        while (val > 0)
        {
            ++digit_cnt;
            val /= 10;
        }
    }

    uint8_t *digit = (uint8_t *)malloc(digit_cnt * sizeof(uint8_t));
    size_t index = digit_cnt;
    for (size_t i = arr_len - 1; i != (size_t)(-1); --i)
    {
        while (arr[i] > 0)
        {
            digit[--index] = (uint8_t)(arr[i] % 10);
            arr[i] /= 10;
        }
    }

    uint64_t ret = 0;
    for (size_t i = 0; i < digit_cnt; ++i)
    {
        ret = (ret * 10) + digit[i];
    }

    free(digit);

    return ret;
}

int main(void)
{
    FILE *file = fopen("input.txt", "r");

    int time_component[RACE_SZ];
    int winning_dist_component[RACE_SZ];
    size_t num_comp = 0;

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
            num_comp = get_numbers_from_string(strtok(NULL, ":"), time_component, RACE_SZ);
        }

        if (strstr(line, "Distance") != NULL)
        {
            num_comp = get_numbers_from_string(strtok(NULL, ":"), winning_dist_component, RACE_SZ);
        }
    }

    fclose(file);

    double race_time = (double)combine_digits(time_component, num_comp);
    double winning_dist = (double)combine_digits(winning_dist_component, num_comp);

    // d = t_h(T - t_h)
    // d > -t_h^2 + t_hT
    // After rearranging with the quadratic formula...
    uint64_t min_th = (uint64_t)ceil((race_time - sqrt((race_time * race_time) - (4 * winning_dist))) / 2.0);
    uint64_t max_th = (uint64_t)floor((race_time + sqrt((race_time * race_time) - (4 * winning_dist))) / 2.0);

    uint64_t num_win = max_th - min_th + 1;
    printf("%lld %lld %lld\n", min_th, max_th, num_win);
    return 0;
}