#include <string.h>
#include <stdio.h>
#include <ctype.h>

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
    if (c == ',' || c == ';' || c == ':' || c == '\'' || c == '\"')
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
    char state = 'p';
    for (int i = 0; i < strlen(word); i++)
    {
        char c = word[i];
        if (state == 'p' && (is_letter(c) || c == '_'))
            state = 'l';
        else if (state == 'l' && (is_alphanum(c) || c == '_'))
            continue;
        else
            return 0;
    }
    return 1;
}

int is_preprocessor(char word[])
{
    if (word[0] == '#')
    {
        return 1;
    }
    return 0;
}

int is_library(char word[])
{
    int length = strlen(word);
    if (length >= 2 && strcmp(word + length - 2, ".h") == 0)
    {
        return 1;
    }
    return 0;
}

void removeComments(FILE *inputF, FILE *outputF)
{

    char c, previousC, printedchar;
    int comment = 0, printing = 0;
    if (!inputF)
        printf("\nFile can't be opened!");
    else
    {
        while ((c = fgetc(inputF)) != EOF)
        {
            if (comment == 2)
            {
                if (previousC == '*' && c == '/')
                    comment = 0;
            }
            else if (comment == 1)
            {
                if (c == '\n')
                    comment = 0;
            }
            else if (printing)
            {
                fputc(c, outputF);
                if (previousC != '\\' && c == '"')
                {
                    printing = 0;
                }
            }
            else if (!comment)
            {
                if (previousC == '/' && c == '*')
                {
                    comment = 2;
                }
                else if (previousC == '/' && c == '/')
                {
                    comment = 1;
                }
                else if (previousC != '\\' && previousC == '(' && c == '"')
                {
                    printing = 1;
                    fputc(c, outputF);
                    printedchar = c;
                }
                else if (previousC == '/' && c != '\n' && c != '*' && c != '/')
                {
                    fputc('/', outputF);
                    fputc(c, outputF);
                    printedchar = c;
                }
                else if (!isspace(previousC) && isspace(c) && !isspace(printedchar))
                {
                    fputc(' ', outputF);
                    printedchar = c;
                }
                else if (c != '\t' && c != '\n' && c != '/' && !isspace(c))
                {
                    fputc(c, outputF);
                    printedchar = c;
                }
            }
            previousC = c;
        }
    }
}

void lexemSeparation(FILE *inputF, FILE *outputF)
{
    char c, previousC;

    while ((c = fgetc(inputF)) != EOF)
    {
        if ((isspace(previousC) || is_operator(previousC) || is_bracket(previousC) || is_separator(previousC) || is_parenthesis(previousC)) && isspace(c))
            continue;
        else if (is_operator(c) || is_bracket(c) || is_separator(c) || is_parenthesis(c))
        {
            if (!isspace(previousC) && !is_operator(previousC) && !is_bracket(previousC) && !is_separator(previousC) && !is_parenthesis(previousC))
                fputc(' ', outputF);
            fputc(c, outputF);
            fputc(' ', outputF);
        }
        else
        {
            fputc(c, outputF);
        }
        previousC = c;
    }
}

void lexemChategorization(FILE *inputF, FILE *outputF)
{
    char c, previousC;
    char word[100];
    int i = 0;

    while ((c = fgetc(inputF)) != EOF)
    {
        if (is_operator(c) || is_bracket(c) || is_separator(c) || is_parenthesis(c) || isspace(c))
        {
            if (i > 0)
            {
                word[i] = '\0';
                if (is_keyword(word))
                    fprintf(outputF, "[kw %s] ", word);
                else if (is_identifier(word))
                    fprintf(outputF, "[id %s] ", word);
                else if (validNum(word))
                    fprintf(outputF, "[num %s] ", word);
                else if (is_preprocessor(word))
                    fprintf(outputF, "[pre %s] ", word);
                else if (is_library(word))
                    fprintf(outputF, "[lib %s] ", word);
                else
                    fprintf(outputF, "[unkn %s] ", word);
                i = 0;
            }
            if (is_operator(c))
                fprintf(outputF, "[op %c] ", c);
            else if (is_bracket(c))
                fprintf(outputF, "[brac %c] ", c);
            else if (is_separator(c))
                fprintf(outputF, "[sep %c] ", c);
            else if (is_parenthesis(c))
                fprintf(outputF, "[par %c] ", c);
        }
        else
        {
            word[i++] = c;
        }
        previousC = c;
    }
}

void printInputOutput()
{
    char c;
    FILE *inputF = fopen("read.c", "r");
    printf("Input file: read.c\n\n");
    while ((c = fgetc(inputF)) != EOF)
        printf("%c", c);
    fclose(inputF);
    FILE *outputF = fopen("out2.txt", "r");
    printf("\n\nOutput file: out.txt\n\n");
    while ((c = fgetc(outputF)) != EOF)
        printf("%c", c);
    fclose(outputF);
}
int main()
{
    FILE *inputF, *outputF;

    inputF = fopen("read.c", "r");
    outputF = fopen("out.txt", "w");

    removeComments(inputF, outputF);

    fclose(outputF);
    fclose(inputF);

    inputF = fopen("out.txt", "r");
    outputF = fopen("out1.txt", "w");

    lexemSeparation(inputF, outputF);

    fclose(inputF);
    fclose(outputF);

    inputF = fopen("out1.txt", "r");
    outputF = fopen("out2.txt", "w");

    lexemChategorization(inputF, outputF);

    fclose(inputF);
    fclose(outputF);

    printInputOutput();

    return 0;
}