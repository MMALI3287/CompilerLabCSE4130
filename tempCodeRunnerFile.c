#include <string.h>
#include <stdio.h>

int main()
{
    FILE *inputF, *outputF;
    char c, previousC;
    int multiComment = 0, singleComment = 0, printing = 0;

    inputF = fopen("read.c", "r");
    outputF = fopen("out.txt", "w");

    if (!inputF)
        printf("\nFile can't be opened!");
    else
    {
        while ((c = fgetc(inputF)) != EOF)
        {
            if (multiComment)
            {
                if (previousC == '*' && c == '/')
                    multiComment = 0;
            }
            else if (singleComment)
            {
                if (c == '\n')
                    singleComment = 0;
            }
            else if (printing)
            {
                fputc(c, outputF);
                if (previousC != '\\' && c == '"')
                {
                    printing = 0;
                }
            }
            else if (!singleComment && !multiComment)
            {
                if (previousC == '/' && c == '*')
                {
                    multiComment = 1;
                }
                else if (previousC == '/' && c == '/')
                {
                    singleComment = 1;
                }
                else if (previousC != '\\' && previousC == '(' && c == '"')
                {
                    printing = 1;
                    fputc(c, outputF);
                }
                else if (previousC == '/' && c != '\n' && c != '*' && c != '/')
                {
                    fputc('/', outputF);
                    fputc(c, outputF);
                }
                else if (previousC != ' ' && c == ' ')
                {
                    fputc(c, outputF);
                }
                else if (c != '\t' && c != '\n' && c != '/' && c != ' ')
                {
                    fputc(c, outputF);
                }
            }
            previousC = c;
        }
    }

    fclose(outputF);
    fclose(inputF);

    inputF = fopen("read.c", "r");

    printf("Input file: read.c\n\n");
    while ((c = fgetc(inputF)) != EOF)
        printf("%c", c);
    fclose(inputF);

    outputF = fopen("out.txt", "r");

    printf("\n\nOutput file: out.txt\n\n");
    while ((c = fgetc(outputF)) != EOF)
        printf("%c", c);
    fclose(outputF);
    return 0;
}