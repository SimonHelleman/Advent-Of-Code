#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define LINE_SZ 512

int main(void)
{
    FILE *file = fopen("input.txt", "r");

    char line[LINE_SZ];
    fgets(line, LINE_SZ, file);

    size_t seeds_cap = 100;
    size_t seeds_len = 0;
    uint64_t *seeds = (uint64_t *)malloc(seeds_cap * sizeof(uint64_t));

    char *tok = strtok(line, ": \n");
    
    while (tok != NULL)
    {
        tok = strtok(NULL, ": \n");

        if (tok != NULL)
        {
            if (seeds_len == seeds_cap)
            {
                seeds_cap *= 2;
                seeds = (uint64_t *)realloc(seeds, seeds_cap * sizeof(uint64_t));
                printf("realloc\n");
            }

            sscanf(tok, "%lld", &seeds[seeds_len++]);
            printf("'%s'\n", tok);
        }
    }

    int heading = 0;

    // I would love to do this with an integer and just set the bits
    // of seeds that have been mapped but then I would be limited
    // to a max of 32 or 64 seeds.
    bool *is_mapped = (bool *)malloc(seeds_len * sizeof(bool));

    while (!feof(file))
    {
        if (fgets(line, LINE_SZ, file) == NULL)
        {
            // end of file blank line
            break;
        }

        if (strstr(line, "map") != NULL)
        {
            //printf("new heading\n");
            for (size_t i = 0; i < seeds_len; ++i)
            {
                is_mapped[i] = false;
            }
            ++heading;
        }

        uint64_t dest;
        uint64_t src;
        uint64_t range;

        if (sscanf(line, "%lld %lld %lld", &dest, &src, &range) == 3)
        {
            //printf("%lld %lld %lld\n", dest, src, range);
            for (size_t i = 0; i < seeds_len; ++i)
            {
                uint64_t num_increment = 0;
                if (!is_mapped[i])
                {
                    for (uint64_t j = src; j < src + range; ++j)
                    {
                        if (seeds[i] == j)
                        {
                            //printf("mapped %lld -> %lld\n", seeds[i], dest + num_increment);
                            seeds[i] = dest + num_increment;
                            is_mapped[i] = true; // Prevent double mapping
                            break;
                        }
                        ++num_increment;
                    }
                }
            }
        }
    }


    if (seeds_len > 0)
    {
        uint64_t min = seeds[0];
        printf("FINAL MAPPINGS: ");
        for (size_t i = 0; i < seeds_len; ++i)
        {
            min = seeds[i] < min ? seeds[i] : min;
            printf("%lld ", seeds[i]);
        }
        printf("\n");
        printf("%lld\n", min);
    }

    // -O2 with prints ~2:04 wow... time complexity is real
    // -O2 no prints ~3:16... I wonder if my laptop is thermal throttling, idk

    // After a very small optimization, -O2 no prints ~1:04...
    // The next optimziation I would explore is making a hash table for
    // the seeds

    fclose(file);
    free(is_mapped);
    free(seeds);
    return 0;
}