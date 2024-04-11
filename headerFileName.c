#include <string.h>
#include <stdio.h>

int main()
{
    FILE *p1, *p2;
    char line[256];
    p1 = fopen("read.c", "r");
    p2 = fopen("out.txt", "w");
    if (!p1)
        printf("\nFile can't be opened!");
    else
    {
        while (fgets(line, sizeof(line), p1))
        {
            if (strstr(line, "#include") != NULL)
            {
                char *start = strchr(line, '<') + 1;
                char *end = strchr(line, '>');
                size_t len = end - start;
                char library_name[256];
                strncpy(library_name, start, len);
                library_name[len] = '\0';
                fprintf(p2, "%s\n", library_name);
            }
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