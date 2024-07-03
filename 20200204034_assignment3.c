#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<stdbool.h>

int tablei = 0;
struct sTable
{
    char name[20][20];
    char idType[20][20];
    char dataType[20][20];
    char scope[20][20];
    char value[20][20];
} sTable;

void insert(char n[], char iT[], char dT[], char scp[], char v[])
{
    strcpy(sTable.name[tablei],n);
    strcpy(sTable.idType[tablei],iT);
    strcpy(sTable.dataType[tablei],dT);
    strcpy(sTable.scope[tablei],scp);
    strcpy(sTable.value[tablei],v);
    tablei++;
}
void update(char n[], char scp[], char v[])
{
    for(int i=0; i<tablei; i++)
    {
        if(strcmp(sTable.name[i],n)==0 && strcmp(sTable.scope[i],scp)==0)
        {
            strcpy(sTable.value[i],v);
            break;
        }
    }
}
void delete()
{
    for(int i = 0 ; i<tablei ; i++)
    {
        free(sTable.name[i]);
    }
}

int search(char n[],char scp[])
{
    for(int i = 0; i<tablei; i++)
    {
        if(strcmp(sTable.name[i],n)==0)
        {
            if(strcmp(sTable.idType[i],"func")==0)
            {
                return (i+1);
            }
            else if(strcmp(sTable.scope[i],scp)==0)
            {
                return (i+1);
            }
        }
    }
    return -1;
}
void display()
{
    printf("\nSl.No.\tName\tId Type\tData Type\tScope\tValue\n-------------------------------------------------------\n");
    for(int i = 0; i<tablei ; i++)
    {
        printf("%d\t|%s\t|%s\t|%s\t\t|%s\t|%s\n",(i+1),sTable.name[i],sTable.idType[i],sTable.dataType[i],sTable.scope[i],sTable.value[i]);
    }
}

void onlyIdLexemes()
{
    FILE *fp1,*fp2,*fp3;
    fp1 = fopen("input.txt","r");
    fp2 = fopen("output.txt","w");
    char str[1024];
    bool prevId = false;
    while (fscanf(fp1, "%s", str) != EOF)
    {
        if(prevId)
        {
            fprintf(fp2,"%s ",str);
            prevId = false;
        }
        else if(str[0] == '[' && str[1]=='i' && str[2]=='d')
        {
            fprintf(fp2,"%s ",str);
            prevId = true;
        }
        else if(str[0] == '[')
        {
            fprintf(fp2,"[",str);
        }
        else
        {
            fprintf(fp2,"%s ",str);
        }
    }
    fclose(fp1);
    fclose(fp2);
    fp3 = fopen("output.txt","r");
    char c;
    printf("Only Id lexemes:\n");
    while((c=fgetc(fp3))!=EOF)
    {
        printf("%c",c);
    }
    fclose(fp3);
}

bool isDataType(char *str)
{
    char temp[1024];
    strcpy(temp,str);
    memmove(temp, temp+1, strlen(temp)); //Remove first Character that is [
    temp[strlen(temp)-1] = '\0'; //Remove last Character that is ]
    char dataTypes[3][10] =
    {
        "double", "float", "int"
    };
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
    fp1 = fopen("output.txt","r");
    char str[1024];
    char identName[1024];
    char dataType[1024];
    char scope[1024] = "global";
    while (fscanf(fp1, "%s", str) != EOF)
    {
        if(isDataType(str))
        {
            memmove(str, str+1, strlen(str)); //Remove first Character that is [
            str[strlen(str)-1] = '\0'; //Remove last Character that is ]
            strcpy(dataType,str);
            fscanf(fp1, "%s", str);
            fscanf(fp1, "%s", str);
            str[strlen(str)-1] = '\0';
            strcpy(identName,str);
            fscanf(fp1, "%s", str);
            if(str[1]=='(')
            {
                insert(identName,"func",dataType,scope," ");
                strcpy(scope,identName);
                fscanf(fp1, "%s", str);
                while(true)
                {
                    if(isDataType(str))
                    {
                        memmove(str, str+1, strlen(str)); //Remove first Character that is [
                        str[strlen(str)-1] = '\0'; //Remove last Character that is ]
                        strcpy(dataType,str);
                        fscanf(fp1, "%s", str);
                        fscanf(fp1, "%s", str);
                        str[strlen(str)-1] = '\0';
                        strcpy(identName,str);
                        insert(identName,"var",dataType,scope," ");
                    }
                    break;
                }
            }
            else if(str[1]=='=')
            {
                fscanf(fp1, "%s", str);
                if(str[1]!='i')
                {
                    memmove(str, str+1, strlen(str)); //Remove first Character that is [
                    str[strlen(str)-1] = '\0'; //Remove last Character that is ]
                    insert(identName,"var",dataType,scope,str);
                    fscanf(fp1,"%s",str);
                }
            }
            else if(str[1]==';')
            {
                insert(identName,"var",dataType,scope," ");
            }
        }
        else if(str[0] == '[' && str[1]=='i' && str[2]=='d')
        {
            fscanf(fp1, "%s", str);
            str[strlen(str)-1] = '\0';
            strcpy(identName,str);
            fscanf(fp1, "%s", str);
            if(str[1]=='=')
            {
                fscanf(fp1, "%s", str);
                if(!(str[1]=='i' && str[2]=='d'))
                {
                    memmove(str, str+1, strlen(str)); //Remove first Character that is [
                    str[strlen(str)-1] = '\0'; //Remove last Character that is ]
                    update(identName,scope,str);
                }
            }
        }
        else if(str[1]=='}')
        {
            strcpy(scope,"global");
        }
    }
    printf("\n\nSymbol Table:\n");
    display();
    fclose(fp1);
}

void modifiedTokenStream()
{
    FILE *fp1,*fp2,*fp3;
    fp1 = fopen("output.txt","r");
    fp2 = fopen("output2.txt","w");
    char str[1024];
    char identName[1024];
    char dataType[1024];
    char scope[1024] = "global";
    while (fscanf(fp1, "%s", str) != EOF)
    {
        if(str[0] == '[' && str[1]== 'i' && str[2]=='d')
        {
            fscanf(fp1, "%s", str);
            str[strlen(str)-1] = '\0';
            strcpy(identName,str);
            fprintf(fp2,"[id %d] ",search(identName,scope));
            fscanf(fp1, "%s", str);
            if(str[1]=='(')
            {
                if(strcmp(scope,"global")==0)
                    strcpy(scope,identName);
            }
            fprintf(fp2, "%s ", str);
        }
        else if(str[1] == '}')
        {
            strcpy(scope,"global");
            fprintf(fp2, "%s ", str);
        }
        else
        {
            fprintf(fp2, "%s ", str);
        }
    }
    fclose(fp1);
    fclose(fp2);
    fp3 = fopen("output2.txt","r");
    char c;
    printf("\nModified Token Stream:\n");
    while((c=fgetc(fp3))!=EOF)
    {
        printf("%c",c);
    }
    fclose(fp3);
}

int main()
{
    onlyIdLexemes();
    symbolTableGenerate();
    modifiedTokenStream();
    return 0;
}


