#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define LINE_SZ 512
#define HASH_SZ 512//128

#define SEED_NULL UINT64_MAX
#define SEED_AVAIL SEED_NULL - 1


size_t g_collision_count = 0;

bool insert_seed(uint64_t seed, uint64_t *hash_table, size_t table_size)
{
    size_t index = seed % table_size;

    for (size_t i = 0; i < table_size; ++i)
    {
        size_t insert_index = (i + index) % table_size;
        if (hash_table[insert_index] == SEED_NULL || hash_table[insert_index] == SEED_AVAIL)
        {
            hash_table[insert_index] = seed;
            return true;
        }
        else
        {
            ++g_collision_count;
        }
    }

    return false;
}

bool lookup_seed(uint64_t seed, uint64_t *hash_table, size_t table_size)
{
    size_t index = seed % table_size;
    for (size_t i = 0; i < table_size; ++i)
    {
        size_t test_index = (i + index) % table_size;
        if (hash_table[test_index] == SEED_NULL)
        {
            return false;
        }

        if (hash_table[test_index] == SEED_AVAIL)
        {
            continue;
        }

        if (hash_table[test_index] != SEED_NULL)
        {
            if (hash_table[test_index] == seed)
            {
                return true;
            }
        }
    }
    return false;
}

bool delete_seed(uint64_t seed, uint64_t *hash_table, size_t table_size)
{
    size_t index = seed % table_size;
    for (size_t i = 0; i < table_size; ++i)
    {
        size_t test_index = (i + index) % table_size;
        if (hash_table[test_index] == SEED_NULL)
        {
            return false;
        }

        if (hash_table[test_index] == SEED_AVAIL)
        {
            continue;
        }

        if (hash_table[test_index] != SEED_NULL)
        {
            if (hash_table[test_index] == seed)
            {
                hash_table[test_index] = SEED_AVAIL;
                return true;
            }
        }
    }
    return false;
}


int main(void)
{
    FILE *file = fopen("input.txt", "r");

    char line[LINE_SZ];
    fgets(line, LINE_SZ, file);

    // Not gonna bother handling resizing the hash table
    uint64_t seed_hash[HASH_SZ];

    for (size_t i = 0; i < HASH_SZ; ++i)
    {
        seed_hash[i] = UINT64_MAX;
    }

    char *tok = strtok(line, ": \n");
    
    while (tok != NULL)
    {
        tok = strtok(NULL, ": \n");

        if (tok != NULL)
        {
            uint64_t seed;
            sscanf(tok, "%lld", &seed);

            // Just for sanity remove after verifying
            if (seed == SEED_NULL || seed == SEED_AVAIL)
            {
                printf("bruh\n");
            }

            insert_seed(seed, seed_hash, HASH_SZ);
            printf("'%s'\n", tok);
        }
    }

    int cnt = 0;
    for (size_t i = 0; i < HASH_SZ; ++i)
    {
        if (seed_hash[i] != SEED_NULL)
        {
            ++cnt;
            printf("%lld\n", seed_hash[i]);
        }
    }

    printf("%d\n", cnt);

    uint64_t available_to_map_hash[HASH_SZ];

    while (!feof(file))
    {
        if (fgets(line, LINE_SZ, file) == NULL)
        {
            // end of file blank line
            break;
        }

        if (strstr(line, "map") != NULL) // New heading, reset hash
        {
            memcpy(available_to_map_hash, seed_hash, HASH_SZ * sizeof(uint64_t));
        }

        uint64_t dest;
        uint64_t src;
        uint64_t range;

        if (sscanf(line, "%lld %lld %lld", &dest, &src, &range) == 3)
        {
            //printf("%lld %lld %lld\n", dest, src, range);
            uint64_t num_increment = 0;
            for (uint64_t i = src; i < src + range; ++i)
            {
                if (lookup_seed(i, available_to_map_hash, HASH_SZ))
                {
                    //printf("mapped %lld -> %lld\n", i, dest + num_increment);

                    delete_seed(i, available_to_map_hash, HASH_SZ);
                    delete_seed(i, seed_hash, HASH_SZ);

                    insert_seed(dest + num_increment, seed_hash, HASH_SZ);
                }
                ++num_increment;
            }
        }
    }
    printf("\n");
    uint64_t min = SEED_NULL;
    for (size_t i = 0; i < HASH_SZ; ++i)
    {
        if (seed_hash[i] != SEED_AVAIL && seed_hash[i] != SEED_NULL)
        {
            min = seed_hash[i] < min ? seed_hash[i] : min;
        }
    }

    printf("%lld collisions\n", g_collision_count);
    printf("\n%lld\n", min);

    // -O2 no prints hash size 128 elements (1 KiB) ~ 40 seconds with 33 collisions
    // -O2 no prints hash size 512 elements (4 KiB) ~ 17 seconds with 2 collisions

    fclose(file);
    return 0;
}