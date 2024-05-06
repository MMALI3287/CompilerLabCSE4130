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

int is_letter(char c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
    {
        return 1;
    }
    return 0;
}

int is_alphanum(char c)
{
    if (is_digit(c) || is_letter(c))
    {
        return 1;
    }
    return 0;
}

int is_operator(char c)
{
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '=' || c == '!' || c == '>' || c == '<' || c == '&' || c == '|' || c == '^' || c == '~')
    {
        return 1;
    }
    return 0;
}

int is_separator(char c)
{
    if (c == ',' || c == ';')
    {
        return 1;
    }
    return 0;
}

int is_parenthesis(char c)
{
    if (c == '(' || c == ')')
    {
        return 1;
    }
    return 0;
}

int is_bracket(char c)
{
    if (c == '{' || c == '}')
    {
        return 1;
    }
    return 0;
}
int is_keyword(char word[])
{
    char keywords[32][10] = {
        "auto", "break", "case", "char", "const", "continue", "default",
        "do", "double", "else", "enum", "extern", "float", "for", "goto",
        "if", "int", "long", "register", "return", "short", "signed",
        "sizeof", "static", "struct", "switch", "typedef", "union",
        "unsigned", "void", "volatile", "while"};

    for (int i = 0; i < 32; i++)
    {
        if (strcmp(word, keywords[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int is_identifier(char word[])
{
    if (is_alphanum(word[0]))
    {
        return 1;
    }
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