#include <stdio.h>
#include <string.h>

int is_digit(char c)
{
    if (c >= '0' && c <= '9')
    {
        return 1;
    }
    return 0;
}

int validNum(char num[])
{
    char state = 'p';
    for (int i = 0; i < strlen(num); i++)
    {
        char c = num[i];

        if (state == 'p' && is_digit(c))
            state = 'd';
        else if (state == 'p' && c == '.')
            state = 'r';
        else if (state == 'd' && c == '.')
            state = 'r';
        else if (state == 'd' && is_digit(c))
            continue;
        else if (state == 'r' && is_digit(c))
            state = 's';
        else if (state == 's' && is_digit(c))
            continue;
        else
            return 0;
    }

    if (state == 's' || state == 'd')
        return 1;

    return 0;
}

int main()
{
    // char a[20];

    // while (1)
    // {
    //     printf("\nEnter number: ");
    //     scanf("%s", &a);
    //     if (validNum(a))
    //     {
    //         printf("Valid Number");
    //     }
    //     else
    //         printf("Invalid Number");
    // }
}