#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define LINE_SZ 512
#define STAGES 7

typedef struct
{
    uint64_t start;
    uint64_t range;
} Seed;

typedef struct
{
    uint64_t src;
    uint64_t dest;
    uint64_t range;
} Mapping;

typedef struct
{
    Mapping *map;
    size_t map_len;
    size_t map_cap;
} StageMap;

int main(void)
{
    FILE *file = fopen("input.txt", "r");

    char line[LINE_SZ];
    fgets(line, LINE_SZ, file);

    size_t seed_val_cap = 100;
    size_t seed_val_len = 0;
    uint64_t *seed_val = (uint64_t *)malloc(seed_val_cap * sizeof(uint64_t));

    char *tok = strtok(line, ": \n");
    
    while (tok != NULL)
    {
        tok = strtok(NULL, ": \n");

        if (tok != NULL)
        {
            if (seed_val_len == seed_val_cap)
            {
                seed_val_cap *= 2;
                seed_val = (uint64_t *)realloc(seed_val, seed_val_cap * sizeof(uint64_t));
                printf("realloc\n");
            }

            sscanf(tok, "%lld", &seed_val[seed_val_len++]);
            printf("'%s'\n", tok);
        }
    }

    size_t seed_set_len = seed_val_len / 2;
    Seed *seed_set = (Seed *)malloc(seed_set_len * sizeof(Seed));

    size_t arr_index = 0;
    for (size_t i = 0; i < seed_val_len - 1; i += 2)
    {
        seed_set[arr_index].start = seed_val[i];
        seed_set[arr_index].range = seed_val[i + 1];
        printf("%lld %lld\n", seed_set[arr_index].start, seed_set[arr_index].range);
        ++arr_index;
    }

    free(seed_val);

    int current_stage = -1;

    StageMap stage_map[STAGES];
    while (!feof(file))
    {
        if (fgets(line, LINE_SZ, file) == NULL)
        {
            // end of file blank line
            break;
        }

        if (strstr(line, "map") != NULL)
        {
            ++current_stage;
            stage_map[current_stage].map_cap = 10;
            stage_map[current_stage].map_len = 0;
            stage_map[current_stage].map = malloc(10 * sizeof(Mapping));
        }

        uint64_t dest;
        uint64_t src;
        uint64_t range;

        if (sscanf(line, "%lld %lld %lld", &dest, &src, &range) == 3)
        {
            if (stage_map[current_stage].map_len == stage_map[current_stage].map_cap)
            {
                size_t new_cap = stage_map[current_stage].map_cap * 2;
                stage_map[current_stage].map_cap = new_cap;
                stage_map[current_stage].map = realloc(stage_map[current_stage].map, new_cap * sizeof(Mapping));
            }
            Mapping map = { src, dest, range };
            stage_map[current_stage].map[stage_map[current_stage].map_len++] = map;
        }
    }
    fclose(file);

    // BRUTEEEEE FORCEEEEEE!!!!!!!!!!!
    // yipppeeeeeeee
    uint64_t min_val = 0;
    for (size_t i = 0; i < seed_set_len; ++i)
    {
        for (size_t j = seed_set[i].start; j <= seed_set[i].start + seed_set[i].range; ++j)
        {
            uint64_t seed = j;
            for (size_t k = 0; k < STAGES; ++k)
            {
                for (size_t l = 0; l < stage_map[k].map_len; ++l)
                {
                    Mapping m = stage_map[k].map[l];
                    if (seed >= m.src && seed <= m.src + m.range)
                    {
                        //printf("mapped %lld -> %lld\n", seed, m.dest + (seed - m.src));
                        uint64_t pos_in_range = seed - m.src;
                        seed = m.dest + pos_in_range;
                        break;
                    }
                }
                if (k == STAGES - 1)
                {
                    if (j == seed_set[0].start)
                    {
                        min_val = seed;
                    }
                    else if (k == STAGES - 1)
                    {
                        min_val = seed < min_val ? seed : min_val;
                    }
                }
            }
        }
        printf("seed set %lld complete\n", i);
    }



    printf("%lld\n", min_val);

    free(seed_set);

    for (size_t i = 0; i < STAGES; ++i)
    {
        free(stage_map[i].map);
    }
    return 0;
}