#include <string.h>
#include <stdio.h>

int main()
{
    FILE *p1, *p2;
    char line[256];
    int line_number = 1;
    p1 = fopen("read.c", "r");
    p2 = fopen("out.txt", "w");
    if (!p1)
        printf("\nFile can't be opened!");
    else
    {
        while (fgets(line, sizeof(line), p1))
        {
            fprintf(p2, "%d: %s", line_number, line);
            line_number++;
        }
    }
    fclose(p1);
    fclose(p2);

    char c;
    p2 = fopen("out.txt", "r");
    while ((c = fgetc(p2)) != EOF)
        printf("%c", c);

    fclose(p2);
    return 0;
}