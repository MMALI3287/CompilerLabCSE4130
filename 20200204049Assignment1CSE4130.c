#include <string.h>
#include <stdio.h>
#include <ctype.h>

const char *keywords[32] = {"auto", "break", "case", "char", "const", "continue", "default",
                            "do", "double", "else", "enum", "extern", "float", "for", "goto",
                            "if", "int", "long", "register", "return", "short", "signed",
                            "sizeof", "static", "struct", "switch", "typedef", "union",
                            "unsigned", "void", "volatile", "while"};

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
    int stringlength = strlen(num);
    for (int i = 0; i < stringlength; i++)
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

int is_operator(char c, char next_c)
{
    switch (c)
    {
    case '=':
    case '<':
    case '>':
    case '!':
    case '&':
    case '|':
        switch (next_c)
        {
        case '=':
        case '&':
        case '|':
            return 2;
        }
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
    case '^':
    case '~':
        return 1;
    default:
        return 0;
    }
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
    if (c == '{' || c == '}' || c == '[' || c == ']')
    {
        return 1;
    }
    return 0;
}

int is_keyword(char word[])
{
    int length = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < length; i++)
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
    int stringlength = strlen(word);
    for (int i = 0; i < stringlength; i++)
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
    if (length > 2 && strcmp(word + length - 2, ".h") == 0)
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
    char c, previousC, nextC;

    while ((c = fgetc(inputF)) != EOF)
    {
        nextC = fgetc(inputF);
        ungetc(nextC, inputF);
        int operator_length = is_operator(c, nextC);
        if ((isspace(previousC) || operator_length || is_bracket(c) || is_separator(c) || is_parenthesis(c)) && isspace(c))
            continue;
        else if (operator_length || is_bracket(c) || is_separator(c) || is_parenthesis(c))
        {
            if (!isspace(previousC) && !is_operator(previousC, '_') && !is_bracket(previousC) && !is_separator(previousC) && !is_parenthesis(previousC))
                fputc(' ', outputF);
            fputc(c, outputF);
            if (operator_length == 2)
            {
                fputc(nextC, outputF);
                fgetc(inputF);
            }
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
    char c, previousC, nextC;
    char word[100];
    int i = 0;

    while ((c = fgetc(inputF)) != EOF)
    {
        nextC = fgetc(inputF);
        ungetc(nextC, inputF);
        int operator_length = is_operator(c, nextC);
        if (operator_length || is_bracket(c) || is_separator(c) || is_parenthesis(c) || isspace(c))
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
            if (operator_length == 1)
                fprintf(outputF, "[op %c] ", c);
            else if (operator_length == 2)
            {
                fprintf(outputF, "[op %c%c] ", c, nextC);
                fgetc(inputF);
            }
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

void printInputOutput(FILE *inputF, FILE *outputF)
{
    char c;
    rewind(inputF);
    rewind(outputF);
    printf("Input file: read.c\n\n");
    while ((c = fgetc(inputF)) != EOF)
        printf("%c", c);
    printf("\n\nOutput file: out.txt\n\n");
    while ((c = fgetc(outputF)) != EOF)
        printf("%c", c);
}

int main()
{
    FILE *readR = fopen("read.c", "r");
    FILE *outW = fopen("out.txt", "w");

    removeComments(readR, outW);

    fclose(outW);

    FILE *outR = fopen("out.txt", "r");
    FILE *out1W = fopen("out1.txt", "w");

    lexemSeparation(outR, out1W);

    fclose(out1W);

    FILE *out1R = fopen("out1.txt", "r");
    FILE *out2W = fopen("out2.txt", "w");

    lexemChategorization(out1R, out2W);

    fclose(out2W);

    FILE *out2R = fopen("out2.txt", "r");

    printInputOutput(readR, out2R);

    fclose(readR);
    fclose(out2R);

    return 0;
}