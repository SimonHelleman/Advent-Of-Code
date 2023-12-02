#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#define LINE_SZ 512

#if defined(_WIN32) || defined(_WIN64)
// sigh.....
#define strtok_r strtok_s
#endif

typedef struct
{
    int red;
    int green;
    int blue;
} CubeSet;

int main(void)
{
    FILE *file = fopen("input.txt", "r");

    int power_sum = 0;
    while (!feof(file))
    {

        char line[LINE_SZ];

        fgets(line, LINE_SZ, file);

        char *round_state;
        const char *round_delim = ":;";
        char *round_token = strtok_r(line, round_delim, &round_state);

        // First token is always game number
        // Already have it so just move on.
        round_token = strtok_r(NULL, round_delim, &round_state);

        CubeSet max = { 0, 0, 0 };

        while (round_token != NULL)
        {
            CubeSet set = { 0, 0, 0 };

            char round[LINE_SZ]; 
            strncpy(round, round_token, LINE_SZ);
            
            char *colour_state;
            char *colour_token = strtok_r(round_token, ",", &colour_state);

            while (colour_token != NULL)
            {
                if (strstr(colour_token, "red"))
                {
                    sscanf(colour_token, "%d", &set.red);
                }

                if (strstr(colour_token, "green"))
                {
                    sscanf(colour_token, "%d", &set.green);
                }

                if (strstr(colour_token, "blue"))
                {
                    sscanf(colour_token, "%d", &set.blue);
                }
                colour_token = strtok_r(NULL, ",", &colour_state);
            }

            round_token = strtok_r(NULL, round_delim, &round_state);

            max.red = set.red > max.red ? set.red : max.red;
            max.green = set.green > max.green ? set.green : max.green;
            max.blue = set.blue > max.blue ? set.blue : max.blue;
            
        }

        power_sum += max.red * max.green * max.blue;
    }

    printf("%d\n", power_sum);

    fclose(file);
    return 0;
}