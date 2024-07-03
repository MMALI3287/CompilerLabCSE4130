#include <stdio.h>
#include <string.h>
#include <ctype.h>

int isValid = 1;
int ind = 0;
int length;
char input[100];

void expression();
void terminal();
void factor();

void expression()
{
    terminal();
    while (isValid && ind < length - 1)
    {
        if (input[ind] == '+' || input[ind] == '-')
        {
            ind++;
            terminal();
        }
        else
        {
            isValid = 0;
        }
    }
}

void terminal()
{
    factor();
    while (isValid && ind < length - 1)
    {
        if (input[ind] == '*' || input[ind] == '/')
        {
            ind++;
            factor();
        }
        else
        {
            isValid = 0;
        }
    }
}

void factor()
{
    if (input[ind] == '(')
    {
        ind++;
        expression();
        if (isValid && ind < length - 1 && input[ind] == ')')
        {
            ind++;
        }
        else
        {
            isValid = 0;
        }
    }
    else if (isalpha(input[ind]) && input[ind] >= 'a' && input[ind] <= 'e')
    {
        ind++;
    }
    else if (isdigit(input[ind]))
    {
        ind++;
    }
    else
    {
        isValid = 0;
    }
}

int main()
{
    while (1)
    {
        printf("Enter expression: ");
        fgets(input, sizeof(input), stdin);
        length = strlen(input);

        ind = 0;
        isValid = 1;

        expression();

        if (isValid && ind == length - 1)
        {
            printf("Valid Expression\n");
        }
        else
        {
            printf("Invalid Expression\n");
        }
    }
    return 0;
}
