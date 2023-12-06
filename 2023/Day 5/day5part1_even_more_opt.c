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
            for (size_t i = 0; i < seeds_len; ++i)
            {
                is_mapped[i] = false;
            }
        }

        uint64_t dest;
        uint64_t src;
        uint64_t range;

        if (sscanf(line, "%lld %lld %lld", &dest, &src, &range) == 3)
        {

            //printf("%lld %lld %lld\n", dest, src, range);
            for (size_t i = 0; i < seeds_len; ++i)
            {
                if (seeds[i] >= src && seeds[i] <= src + range && !is_mapped[i])
                {
                    //printf("mapped %lld -> %lld\n", seeds[i], dest + (seeds[i] - src));
                    uint64_t pos_in_range = seeds[i] - src;
                    seeds[i] = dest + pos_in_range;
                    is_mapped[i] = true;
                }
            }
        }
    }


    if (seeds_len > 0)
    {
        uint64_t min = seeds[0];
        //printf("FINAL MAPPINGS: ");
        for (size_t i = 0; i < seeds_len; ++i)
        {
            min = seeds[i] < min ? seeds[i] : min;
            //printf("%lld ", seeds[i]);
        }
        //printf("\n");
        printf("%lld\n", min);
    }

    // So the hash table helped a lot. But then after looking at part 2,
    // I realized that my solution was still not nearly optimized enough
    // so I busted out a good old pen and paper (actually OneNote :P)
    // and realized I had been thinking of this all wrong and no searching
    // is required. Now this solution computes instantly :). Kinda embarrassing
    // but I still have not caved and looked at anyone else's solution.

    fclose(file);
    free(is_mapped);
    free(seeds);
    return 0;
}