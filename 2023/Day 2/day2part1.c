#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#define LINE_SZ 512

typedef struct
{
    int red;
    int green;
    int blue;
} CubeSet;

int main(void)
{
    FILE *file = fopen("input.txt", "r");

    int game_id_sum = 0;
    int game_number = 0;
    while (!feof(file))
    {
        ++game_number;

        char line[LINE_SZ];

        fgets(line, LINE_SZ, file);

        char *round_state;
        const char *round_delim = ":;";
        char *round_token = strtok_r(line, round_delim, &round_state);


        // First token is always game number
        // Already have it so just move on.
        round_token = strtok_r(NULL, round_delim, &round_state);

        //printf("Game %d:\n", game_number);

        bool possible = true;
        while (round_token != NULL && possible)
        {
            CubeSet set = { 0, 0, 0 };
            //printf("   %s\n", round_token);

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
            //printf("    r=%d g=%d b=%d\n", set.red, set.green, set.blue);

            if (set.red > 12 || set.green > 13 || set.blue > 14)
            {
                possible = false;
            }
        }
        if (possible)
        {
            //printf("    possible\n");
            game_id_sum += game_number;
        }
    }

    printf("%d\n", game_id_sum);

    fclose(file);
    return 0;
}