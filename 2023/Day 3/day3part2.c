#define _CRT_SECURE_NO_WARNINGS
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
    FILE* file = fopen("input.txt", "r");

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
        //printf("%03d: %s\n", i, grid[i]);
    }
    fclose(file);

    int gear_ratio_sum = 0;
    for (int i = 0; i < GRID_HEIGHT; ++i)
    {
        for (int j = 0; j < GRID_WIDTH; ++j)
        {
            int num_adjacent_gears = 0;
            int gear_pos[2][2];
            bool up_passed = false;
            bool down_passed = false;

            if (grid[i][j] == '*')
            {
                // Check up
                if (i > 0)
                {
                    if (isdigit(grid[i - 1][j]))
                    {
                        gear_pos[num_adjacent_gears][0] = i - 1;
                        gear_pos[num_adjacent_gears][1] = j;
                        ++num_adjacent_gears;
                        up_passed = true;
                    }
                }

                // Check down
                if (i < GRID_HEIGHT - 1)
                {
                    if (isdigit(grid[i + 1][j]))
                    {
                        gear_pos[num_adjacent_gears][0] = i + 1;
                        gear_pos[num_adjacent_gears][1] = j;
                        ++num_adjacent_gears;
                        down_passed = true;
                    }
                }

                // Check left
                if (j > 0)
                {
                    if (isdigit(grid[i][j - 1]))
                    {
                        gear_pos[num_adjacent_gears][0] = i;
                        gear_pos[num_adjacent_gears][1] = j - 1;
                        ++num_adjacent_gears;
                    }
                }

                // Check right
                if (j < GRID_WIDTH - 1)
                {
                    if (isdigit(grid[i][j + 1]))
                    {
                        gear_pos[num_adjacent_gears][0] = i;
                        gear_pos[num_adjacent_gears][1] = j + 1;
                        ++num_adjacent_gears;
                    }
                }

                // Top left
                if (i > 0 && j > 0 && !up_passed)
                {
                    if (isdigit(grid[i - 1][j - 1]))
                    {
                        gear_pos[num_adjacent_gears][0] = i - 1;
                        gear_pos[num_adjacent_gears][1] = j - 1;
                        ++num_adjacent_gears;
                    }
                }

                // Top right
                if (i > 0 && j < GRID_WIDTH - 1 && !up_passed)
                {
                    if (isdigit(grid[i - 1][j + 1]))
                    {
                        gear_pos[num_adjacent_gears][0] = i - 1;
                        gear_pos[num_adjacent_gears][1] = j + 1;
                        ++num_adjacent_gears;
                    }
                }

                // Bottom left
                if (i < GRID_HEIGHT - 1 && j > 0 && !down_passed)
                {
                    if (isdigit(grid[i + 1][j - 1]))
                    {
                        gear_pos[num_adjacent_gears][0] = i + 1;
                        gear_pos[num_adjacent_gears][1] = j - 1;
                        ++num_adjacent_gears;
                    }
                }

                // Bottom right
                if (i < GRID_HEIGHT - 1 && j < GRID_WIDTH - 1 && !down_passed)
                {
                    if (isdigit(grid[i + 1][j + 1]))
                    {
                        gear_pos[num_adjacent_gears][0] = i + 1;
                        gear_pos[num_adjacent_gears][1] = j + 1;
                        ++num_adjacent_gears;
                    }
                }
            }

            if (num_adjacent_gears == 2)
            {
                const int gear1_row = gear_pos[0][0];
                int gear1_col = gear_pos[0][1];

                int gear2_col = gear_pos[1][1];
                const int gear2_row = gear_pos[1][0];

                // Move left until no longer at a digit which means the
                // digit starts at x + 1 assuming it doesn't hit the wall where it will
                // just be 0
                while (isdigit(grid[gear1_row][gear1_col]) && gear1_col >= 0)
                {
                    --gear1_col;
                }

                gear1_col = gear1_col >= 0 ? gear1_col + 1 : 0;

                while (isdigit(grid[gear2_row][gear2_col]) && gear2_col >= 0)
                {
                    --gear2_col;
                }

                gear2_col = gear2_col >= 0 ? gear2_col + 1 : 0;

                int gear1_val;
                int gear2_val;
                sscanf(&grid[gear1_row][gear1_col], "%d", &gear1_val);
                sscanf(&grid[gear2_row][gear2_col], "%d", &gear2_val);

                //printf("(%d %d) -> %03d, ", gear1_row, gear1_col, gear1_val);
                //printf("(%d %d) -> %03d\n", gear2_row, gear2_col, gear2_val);

                gear_ratio_sum += gear1_val * gear2_val;
            }
        }
    }

    printf("%d\n", gear_ratio_sum);

    return 0;
}