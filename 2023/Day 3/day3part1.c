#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#define LINE_SZ 256

// Arbituary grid size?? nah
#define GRID_WIDTH 140//10
#define GRID_HEIGHT GRID_WIDTH

int main(void)
{
    FILE *file = fopen("input.txt", "r");

    char grid[GRID_HEIGHT][GRID_WIDTH + 1];

    for (int i = 0; !feof(file); ++i)
    {
        char line[LINE_SZ];
        fgets(line, LINE_SZ, file);
        size_t line_len = strlen(line);
        if (i < GRID_HEIGHT - 1)
        {
            line[line_len - 1] = '\0'; // Remove \n
        }

        if (i < GRID_HEIGHT)
        {
            strncpy(grid[i], line, GRID_WIDTH + 1);
        }
        //printf("%d: %s\n", i, grid[i]);
    }
    fclose(file);

    int part_sum = 0;
    for (int i = 0; i < GRID_HEIGHT; ++i)
    {
        char last_char = '.';
        int digit_start = 0;
        bool found_part = false;
        for (int j = 0; j < GRID_WIDTH; ++j)
        {

            if (!isdigit(grid[i][j]))
            {
                found_part = false;
            }

            if (isdigit(grid[i][j]) && !found_part)
            {
                if (!isdigit(last_char))
                {
                    digit_start = j;
                }

                // I never promised that my AOC solutions would be "nice"

                // Check up
                if (i > 0)
                {
                    if (grid[i - 1][j] != '.' && !isdigit(grid[i - 1][j]))
                    {
                        found_part = true;
                        int part_number;
                        sscanf(&grid[i][digit_start], "%d", &part_number);
                        part_sum += part_number;

                        //printf("(%d %d), %d\n", i, j, part_number); 
                    }
                }

                // Check down
                if (i < GRID_HEIGHT - 1)
                {
                    if (grid[i + 1][j] != '.' && !isdigit(grid[i + 1][j]))
                    {
                        found_part = true;
                        int part_number;
                        sscanf(&grid[i][digit_start], "%d", &part_number);
                        part_sum += part_number;

                        //printf("(%d %d), %d\n", i, j, part_number); 
                    }
                }

                // Check left
                if (j > 0)
                {
                    if (grid[i][j - 1] != '.' && !isdigit(grid[i][j - 1]))
                    {
                        found_part = true;
                        int part_number;
                        sscanf(&grid[i][digit_start], "%d", &part_number);
                        part_sum += part_number;

                        //printf("(%d %d), %d\n", i, j, part_number); 
                    }
                }

                // Check right
                if (j < GRID_WIDTH - 1)
                {
                    if (grid[i][j + 1] != '.' && !isdigit(grid[i][j + 1]))
                    {
                        found_part = true;
                        int part_number;
                        sscanf(&grid[i][digit_start], "%d", &part_number);
                        part_sum += part_number;

                        //printf("(%d %d), %d\n", i, j, part_number); 
                    }
                }

                // Top left
                if (i > 0 && j > 0)
                {
                    if (grid[i - 1][j - 1] != '.' && !isdigit(grid[i - 1][j - 1]))
                    {
                        found_part = true;
                        int part_number;
                        sscanf(&grid[i][digit_start], "%d", &part_number);
                        part_sum += part_number;

                        //printf("(%d %d), %d\n", i, j, part_number); 
                    }
                }

                // Top right
                if (i > 0 && j < GRID_WIDTH - 1)
                {
                    if (grid[i - 1][j + 1] != '.' && !isdigit(grid[i - 1][j + 1]))
                    {
                        found_part = true;
                        int part_number;
                        sscanf(&grid[i][digit_start], "%d", &part_number);
                        part_sum += part_number;

                        //printf("(%d %d), %d\n", i, j, part_number); 
                    }
                }

                // Bottom left
                if (i < GRID_HEIGHT - 1 && j > 0)
                {
                    if (grid[i + 1][j - 1] != '.' && !isdigit(grid[i + 1][j - 1]))
                    {
                        found_part = true;
                        int part_number;
                        sscanf(&grid[i][digit_start], "%d", &part_number);
                        part_sum += part_number;

                        //printf("(%d %d), %d\n", i, j, part_number); 
                    }
                }

                // Bottom right
                if (i < GRID_HEIGHT - 1 && j < GRID_WIDTH - 1)
                {
                    if (grid[i + 1][j + 1] != '.' && !isdigit(grid[i + 1][j + 1]))
                    {
                        found_part = true;
                        int part_number;
                        sscanf(&grid[i][digit_start], "%d", &part_number);
                        part_sum += part_number;

                        //printf("(%d %d), %d\n", i, j, part_number); 
                    }
                }

            }

            last_char = grid[i][j];
        }
    }

    printf("%d\n", part_sum);


    return 0;
}