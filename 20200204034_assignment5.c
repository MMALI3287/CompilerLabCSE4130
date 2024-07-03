#include<stdio.h>
#include <string.h>

int f = 0;
int i = 0;
int l;
char str[100];
void term();
void exp();
void factor();



void exp()
{
    term();
    if(f && i<l-1)
    {
        if(str[i] == '+' || str[i] == '-')
        {
            i++;
            f=1;
            term();
        }
        else
        {
            return;
        }
    }

}

void term()
{
    factor();
    if(f && i<l-1)
    {
        if(str[i] == '*' || str[i] == '/')
        {
            i++;
            f=1;
            factor();
        }
        else
        {
            return;
        }
    }
}


void factor()
{
    if(str[i]=='(')
    {
        i++;
        f=1;
        if(f && i<l-1)
        {
            exp();
            if(f && i<l-1)
            {
                if(str[i]==')')
                {
                    i++;
                    f=1;
                    return;
                }
                else
                {
                    f=0;
                    return;
                }
            }
            else
            {
                f=0;
                return;
            }
        }
        else
        {
            f=0;
            return;
        }
    }
    else if(str[i]>='a' && str[i]<='e')
    {
        i++;
        f=1;
        return;
    }
    else if(str[i]>='0' && str[i]<='9')
    {
        i++;
        f=1;
        return;
    }
    else
    {
        f=0;
        return;
    }
}


int main()
{
    while(1)
    {
        printf("Enter expression: ");
        fgets(str, sizeof(str), stdin);
        l = strlen(str);
        i = 0;
        exp();
        if(f==1 && i==l-1)
        {
            printf("Valid Expression\n");
        }
        else
        {
            printf("Invalid Expression\n");
        }
    }
}
