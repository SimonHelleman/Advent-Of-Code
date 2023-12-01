#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define LINE_SZ 512

int main(void)
{
    FILE *file = fopen("test_part2.txt", "r");

    int sum = 0;
    while (!feof(file))
    {
        char line[LINE_SZ];
        fgets(line, LINE_SZ, file);

        size_t len = strlen(line);
        int dig_num = 0;
        int digit[LINE_SZ];
        for (size_t i = 0; i < len; ++i)
        {
            if (isdigit(line[i]))
            {
                int dig = line[i] - '0';
                digit[dig_num++] = dig;
            }
        }

        if (dig_num > 0)
        {
            
            int first_digit = digit[0];
            int last_digit = 0;
            if (dig_num > 1)
            {
                last_digit = digit[dig_num - 1];
            }
            else
            {
                last_digit = first_digit;
            }

            int line_val = first_digit * 10 + last_digit;
            sum += line_val;
            printf("%d\n", line_val);
        }
    }

    fclose(file);
    printf("%d\n", sum);
    return 0;
}