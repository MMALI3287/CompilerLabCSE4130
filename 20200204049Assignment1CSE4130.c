#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

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

int isOper(char c)
{
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '=' || c == '!' || c == '>' || c == '<' || c == '&' || c == '|' || c == '^' || c == '~')
        return 1;
    return 0;
}

int isSepar(char c)
{
    if (c == ',' || c == ';' || c == ':' || c == '\'' || c == '\"')
        return 1;
    return 0;
}

int isParen(char c)
{
    if (c == '(' || c == ')' || c == '[' || c == ']')
        return 1;
    return 0;
}

int isBrace(char c)
{
    if (c == '{' || c == '}')
        return 1;
    return 0;
}

int tablei = 0;
typedef struct
{
    char name[20][20];
    char idType[20][20];
    char dataType[20][20];
    char scope[20][20];
    char value[20][20];
} SymbolTable;

SymbolTable sTable;

void insert(char n[], char iT[], char dT[], char scp[], char v[])
{
    strcpy(sTable.name[tablei], n);
    strcpy(sTable.idType[tablei], iT);
    strcpy(sTable.dataType[tablei], dT);
    strcpy(sTable.scope[tablei], scp);
    strcpy(sTable.value[tablei], v);
    tablei++;
}
void update(char n[], char scp[], char v[])
{
    for (int i = 0; i < tablei; i++)
    {
        if (strcmp(sTable.name[i], n) == 0 && strcmp(sTable.scope[i], scp) == 0)
        {
            strcpy(sTable.value[i], v);
            break;
        }
    }
}
void delete()
{
    for (int i = 0; i < tablei; i++)
    {
        strcpy(sTable.name[i], "");
        strcpy(sTable.idType[i], "");
        strcpy(sTable.dataType[i], "");
        strcpy(sTable.scope[i], "");
        strcpy(sTable.value[i], "");
    }
}

int search(char n[], char scp[])
{
    for (int i = 0; i < tablei; i++)
    {
        if (strcmp(sTable.name[i], n) == 0)
        {
            if (strcmp(sTable.idType[i], "func") == 0)
            {
                return (i + 1);
            }
            else if (strcmp(sTable.scope[i], scp) == 0)
            {
                return (i + 1);
            }
        }
    }
    return -1;
}
void display()
{
    printf("\nSl.No.\tName\tId Type\tData Type\tScope\tValue\n-------------------------------------------------------\n");
    for (int i = 0; i < tablei; i++)
    {
        printf("%d\t|%s\t|%s\t|%s\t\t|%s\t|%s\n", (i + 1), sTable.name[i], sTable.idType[i], sTable.dataType[i], sTable.scope[i], sTable.value[i]);
    }
}

void onlyIdLexemes()
{
    FILE *fp1, *fp2, *fp3;
    fp1 = fopen("out2.txt", "r");
    fp2 = fopen("out3.txt", "w");
    char str[1024];
    bool prevId = false;
    while (fscanf(fp1, "%s", str) != EOF)
    {
        if (prevId)
        {
            fprintf(fp2, "%s ", str);
            prevId = false;
        }
        else if (str[0] == '[' && str[1] == 'i' && str[2] == 'd')
        {
            fprintf(fp2, "%s ", str);
            prevId = true;
        }
        else if (str[0] == '[')
        {
            fprintf(fp2, "[", str);
        }
        else
        {
            fprintf(fp2, "%s ", str);
        }
    }
    fclose(fp1);
    fclose(fp2);
    fp3 = fopen("out3.txt", "r");
    char c;
    printf("\n\nOnly Id lexemes:\n");
    while ((c = fgetc(fp3)) != EOF)
    {
        printf("%c", c);
    }
    fclose(fp3);
}

bool isDataType(char *str)
{
    char temp[1024];
    strcpy(temp, str);
    memmove(temp, temp + 1, strlen(temp)); // Remove first Character that is [
    temp[strlen(temp) - 1] = '\0';         // Remove last Character that is ]
    char dataTypes[3][10] =
        {
            "double", "float", "int"};
    for (int i = 0; i < 3; i++)
    {
        if (strcmp(temp, dataTypes[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

void symbolTableGenerate()
{
    FILE *fp1;
    fp1 = fopen("out3.txt", "r");
    if (fp1 == NULL)
    {
        perror("Error opening file1");
        return;
    }

    char str[1024];
    char identName[1024];
    char dataType[1024];
    char scope[1024] = "global";

    while (fscanf(fp1, "%s", str) != EOF)
    {
        if (isDataType(str))
        {
            // Handling variable declarations
            if (strchr(str, '['))
            {
                // Remove [ and ] characters from dataType
                memmove(dataType, str + 1, strlen(str) - 2);
                dataType[strlen(str) - 2] = '\0';

                // Read identifier name correctly
                fscanf(fp1, "%s", str);                       // Read the next token into `str`
                memmove(identName, str + 1, strlen(str) - 2); // Remove [ and ] characters
                identName[strlen(str) - 2] = '\0';            // Null-terminate the identifier name

                // Handling function declaration
                if (strchr(identName, '('))
                {
                    // Extract function name
                    char *parenPos = strchr(identName, '(');
                    if (parenPos != NULL)
                    {
                        *parenPos = '\0'; // Null terminate at '(' to get function name
                    }
                    insert(identName, "func", dataType, scope, " ");
                    strcpy(scope, identName);

                    // Read parameters and add them to symbol table
                    while (true)
                    {
                        fscanf(fp1, "%s", str);
                        if (!isDataType(str))
                            break;

                        // Remove [ and ] characters from dataType
                        memmove(dataType, str + 1, strlen(str) - 2);
                        dataType[strlen(str) - 2] = '\0';

                        // Read parameter name correctly
                        fscanf(fp1, "%s", str);                       // Read the next token into `str`
                        memmove(identName, str + 1, strlen(str) - 2); // Remove [ and ] characters
                        identName[strlen(str) - 2] = '\0';            // Null-terminate the identifier name
                        insert(identName, "var", dataType, scope, " ");
                    }
                }
                else if (strchr(identName, '='))
                {
                    // Variable initialization
                    memmove(identName, identName + 1, strlen(identName) - 2);
                    identName[strlen(identName) - 2] = '\0';
                    fscanf(fp1, "%s", str);

                    if (!(strchr(str, 'i') && strchr(str, 'd')))
                    {
                        insert(identName, "var", dataType, scope, str);
                    }
                }
                else
                {
                    // Variable declaration without initialization
                    insert(identName, "var", dataType, scope, " ");
                }
            }
        }
        // Handling identifier updates
        else if (str[0] == '[' && str[1] == 'i' && str[2] == 'd')
        {
            // Handling identifier updates
            fscanf(fp1, "%s", str);                       // Read the next token into `str`
            memmove(identName, str + 1, strlen(str) - 2); // Remove [ and ] characters
            identName[strlen(str) - 2] = '\0';            // Null-terminate the identifier name

            fscanf(fp1, "%s", str);
            if (strchr(str, '='))
            {
                fscanf(fp1, "%s", str);
                if (!(strchr(str, 'i') && strchr(str, 'd')))
                {
                    update(identName, scope, str);
                }
            }
        }
        // Scope Management
        else if (strchr(str, '}'))
        {
            // Handling end of scope
            strcpy(scope, "global");
        }
    }

    printf("\n\nSymbol Table:\n");
    display();

    fclose(fp1);
}

void modifiedTokenStream()
{
    FILE *fp1, *fp2, *fp3;
    fp1 = fopen("out3.txt", "r");
    fp2 = fopen("out4.txt", "w");

    if (fp1 == NULL || fp2 == NULL)
    {
        perror("Error opening file2");
        return;
    }

    char str[1024];
    char identName[1024];
    char scope[1024] = "global";

    while (fscanf(fp1, "%s", str) != EOF)
    {
        if (str[0] == '[' && str[1] == 'i' && str[2] == 'd')
        {
            fscanf(fp1, "%s", str);
            str[strlen(str) - 1] = '\0'; // Remove trailing ']'
            strcpy(identName, str);

            // Lookup identifier in symbol table and replace with its index
            fprintf(fp2, "[id %d] ", search(identName, scope));

            fscanf(fp1, "%s", str);

            if (str[1] == '(')
            {
                // Update scope if it's a function definition
                if (strcmp(scope, "global") == 0)
                {
                    strcpy(scope, identName);
                }
            }

            fprintf(fp2, "%s ", str); // Write the next token to fp2
        }
        else if (str[1] == '}')
        {
            // End of scope detected, revert to global scope
            strcpy(scope, "global");
            fprintf(fp2, "%s ", str);
        }
        else
        {
            // Regular token, copy to fp2 as is
            fprintf(fp2, "%s ", str);
        }
    }

    fclose(fp1);
    fclose(fp2);

    // Output modified token stream from fp2 to console
    fp3 = fopen("out4.txt", "r");

    if (fp3 == NULL)
    {
        perror("Error opening file3");
        return;
    }

    printf("\nModified Token Stream:\n");
    int c;
    while ((c = fgetc(fp3)) != EOF)
    {
        printf("%c", c);
    }

    fclose(fp3);
}

// Assignment 4

void commentsRemovalInline()
{
    FILE *fp1, *fp2;
    fp1 = fopen("read.c", "r");
    fp2 = fopen("out5.txt", "w");

    if (fp1 == NULL || fp2 == NULL)
    {
        perror("Error opening file4");
        return;
    }

    char c;
    while ((c = fgetc(fp1)) != EOF)
    {
        if (c == '/')
        {
            char next = fgetc(fp1);

            if (next == '/')
            {
                // Single-line comment, skip until end of line
                while ((c = fgetc(fp1)) != '\n' && c != EOF)
                    ;
                // Ensure newline is printed in output
                if (c == '\n')
                    fputc(c, fp2);
            }
            else if (next == '*')
            {
                // Multi-line comment, skip until closing */
                while (1)
                {
                    while ((c = fgetc(fp1)) != '*')
                    {
                        if (c == EOF)
                        {
                            fprintf(stderr, "Error: Unclosed multi-line comment.\n");
                            fclose(fp1);
                            fclose(fp2);
                            return;
                        }
                    }
                    if ((c = fgetc(fp1)) == '/')
                    {
                        break;
                    }
                    else if (c == EOF)
                    {
                        fprintf(stderr, "Error: Unclosed multi-line comment.\n");
                        fclose(fp1);
                        fclose(fp2);
                        return;
                    }
                }
            }
            else
            {
                // Not a comment, print '/' and move back one character
                fputc('/', fp2);
                fseek(fp1, -1, SEEK_CUR);
            }
        }
        else if (c == ' ')
        {
            // Handle consecutive spaces
            fputc(c, fp2);
            while ((c = fgetc(fp1)) == ' ')
                ;
            if (c != EOF)
                fseek(fp1, -1, SEEK_CUR);
        }
        else if (c != '\t')
        {
            // Copy normal characters to output
            fputc(c, fp2);
        }
        else
        {
            // Copy newline characters to output
            fputc(c, fp2);
        }
    }

    fclose(fp1);
    fclose(fp2);
}

void addLineNumber()
{
    FILE *fp1, *fp2;
    fp1 = fopen("out5.txt", "r");
    fp2 = fopen("outLine.txt", "w");

    if (fp1 == NULL || fp2 == NULL)
    {
        perror("Error opening file5");
        return;
    }

    char c;
    bool quote = false;
    int lineNo = 1;
    fprintf(fp2, "%d ", lineNo); // Write initial line number

    while ((c = fgetc(fp1)) != EOF)
    {
        fputc(c, fp2); // Write character to fp2

        if (c == '\n')
        {
            fprintf(fp2, "%d ", ++lineNo); // Write line number after newline
        }
    }

    fclose(fp1);
    fclose(fp2);
}

void leximSeperateInline()
{
    FILE *fp1, *fp2;
    fp1 = fopen("outLine.txt", "r");
    fp2 = fopen("outLine2.txt", "w");

    if (fp1 == NULL || fp2 == NULL)
    {
        perror("Error opening file6");
        return;
    }

    char c;
    char prev = ' ';
    bool newLine = true;

    while ((c = fgetc(fp1)) != EOF)
    {
        if (newLine)
        {
            // Handle beginning of a new line
            fputc(c, fp2);
            while ((c = fgetc(fp1)) != ' ')
            {
                fputc(c, fp2);
            }
            fputc(c, fp2); // Write the space after the token
            newLine = false;
        }
        else if (c == '\n')
        {
            // Handle end of line
            fputc(c, fp2);
            newLine = true;
        }
        else if (c == ' ')
        {
            // Handle consecutive spaces
            if (prev != ' ')
            {
                fputc(c, fp2);
                prev = c;
            }
        }
        else if (isOper(c) || isSepar(c) || isParen(c) || isBrace(c))
        {
            // Handle operators, separators, parentheses, braces
            if (prev != ' ')
                fputc(' ', fp2); // Ensure space before these characters
            fputc(c, fp2);
            prev = c;
        }
        else
        {
            // Handle normal characters
            fputc(c, fp2);
            prev = c;
        }
    }

    fclose(fp1);
    fclose(fp2);
}

void unbalancedParenDetect()
{
    FILE *fp1;
    fp1 = fopen("outLine2.txt", "r");

    if (fp1 == NULL)
    {
        perror("Error opening file7");
        return;
    }

    char buffer[512];
    int lineNo = 1;
    int braceCount = 0;
    int parenCount = 0;

    while (fgets(buffer, sizeof(buffer), fp1) != NULL)
    {
        char *token = strtok(buffer, " \t\n");
        while (token != NULL)
        {
            if (isdigit(token[0]))
            {
                lineNo = atoi(token);
            }
            else if (token[0] == '{')
            {
                braceCount++;
            }
            else if (token[0] == '}')
            {
                braceCount--;
                if (braceCount < 0)
                {
                    printf("Misplaced '}' at line %d\n", lineNo);
                    fclose(fp1);
                    return;
                }
            }
            else if (token[0] == '(')
            {
                parenCount++;
            }
            else if (token[0] == ')')
            {
                parenCount--;
                if (parenCount < 0)
                {
                    printf("Misplaced ')' at line %d\n", lineNo);
                    fclose(fp1);
                    return;
                }
            }
            token = strtok(NULL, " \t\n");
        }
        lineNo++; // Increment line number for the next line
    }

    if (braceCount > 0)
    {
        printf("Unmatched '{' at end of file\n");
    }
    else if (parenCount > 0)
    {
        printf("Unmatched '(' at end of file\n");
    }

    fclose(fp1);
}

void unmatchedElseDetect()
{
    FILE *fp1;
    fp1 = fopen("outLine2.txt", "r");

    if (fp1 == NULL)
    {
        perror("Error opening file8");
        return;
    }

    char buffer[512];
    int lineNo = 1;
    int ifCount = 0;

    while (fgets(buffer, sizeof(buffer), fp1) != NULL)
    {
        char *token = strtok(buffer, " \t\n");
        while (token != NULL)
        {
            if (isdigit(token[0]))
            {
                lineNo = atoi(token);
            }
            else if (strcmp("if", token) == 0)
            {
                ifCount++;
            }
            else if (strcmp("else", token) == 0)
            {
                ifCount--;
                if (ifCount < 0)
                {
                    printf("Unmatched 'else' at line %d\n", lineNo);
                    fclose(fp1);
                    return;
                }
            }
            token = strtok(NULL, " \t\n");
        }
        lineNo++; // Increment line number for the next line
    }

    if (ifCount > 0)
    {
        printf("Unmatched 'if' at end of file\n");
    }

    fclose(fp1);
}

void forErrorDetect()
{
    FILE *fp1;
    fp1 = fopen("outLine2.txt", "r");

    if (fp1 == NULL)
    {
        perror("Error opening file");
        return;
    }

    char buffer[512];
    while (fgets(buffer, sizeof(buffer), fp1) != NULL)
    {
        char *token = strtok(buffer, " \t\n");
        int lineNo = atoi(token);
        token = strtok(NULL, " \t\n");
        int semiColCount = 0;

        while (token != NULL)
        {
            if (strcmp("for", token) == 0)
            {
                token = strtok(NULL, " \t\n"); // Move to the next token after "for"

                while (token != NULL && token[0] != ')')
                {
                    if (token[0] == ';')
                    {
                        semiColCount++;
                    }
                    token = strtok(NULL, " \t\n");
                }

                if (semiColCount > 2)
                {
                    printf("Duplicate ';' in 'for' at line %d\n", lineNo);
                }
                else if (semiColCount < 2)
                {
                    printf("Insufficient ';' in 'for' at line %d\n", lineNo);
                }

                semiColCount = 0; // Reset semi-colon count for the next loop
            }

            token = strtok(NULL, " \t\n");
        }
    }

    fclose(fp1);
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

    onlyIdLexemes();
    symbolTableGenerate();
    modifiedTokenStream();

    commentsRemovalInline();
    addLineNumber();

    leximSeperateInline();
    printf("\n\n");
    unbalancedParenDetect();
    unmatchedElseDetect();
    forErrorDetect();

    return 0;
}