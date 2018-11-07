#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct struct_symtab
{
    int flag;  //0=>repopulate address
    char label[255];
    int address;
    //char address[255];
    int lsize;
    int list[255];
};
struct struct_sic
{

    char opcode[255];
    char operand[255];
};

struct struct_symtab symtab[255];
struct struct_sic sic[255];
int symtabsize = 0;
int locctr=0;
int linecount=0;
int insymtab(char sym[255])
{

    int j=0;
    while(j<symtabsize)
    {

        if(strcmp(sym,symtab[j].label)==0)
        {
            return j;
        }
        j++;
    }
    return -1;
}

int issymbol(char sym[255])
{

    return 1;
}

int inoptab(char sym[255])
{

    return 1;
}

void printsymtab()
{
    printf("\nSymTab\n");
    printf("symtabsize %i\n",symtabsize);
    int j=0;
    while (j<symtabsize)
    {
        printf("\n%i-%s-%i--",symtab[j].flag,symtab[j].label,symtab[j].address);
        int i=0;
        while (i<symtab[j].lsize)
        {

            printf("-%i",symtab[j].list[i]);
            ++i;
        }
        j++;
    }
}
void printsic()
{
    printf("\nSIC\n");
    int j=0;
    while (j<linecount)
    {
        printf("%s-%s\n",sic[j].opcode,sic[j].operand);
        j++;
    }
}

void repopulateaddr()
{

    int j=0;
    while (j<symtabsize)
    {


    }
}

int main()
{

    char label[255]="";
    char opcode[255]="";
    char operand[255]="";

    FILE *sicfile = fopen("sic.txt","r");
    FILE *opfile = fopen("optab.txt","r");

    while (strcmp(opcode,"END") != 0)
    {

        fscanf(sicfile,"%s%s%s",&label,&opcode,&operand);
        printf("%s-%s-%s\n",label,opcode,operand);

        if (strcmp(opcode,"START") == 0)
        {

            locctr=atoi(operand);
            printf("%i locctr\n",locctr);
            continue;
        }

        //label
        if (strcmp(label,"NULL") != 0)
        {
            //label is not null

            if (insymtab(label) >= 0)
            {

                //label already in symtab . just assign address
                //well do repopulating operand address at end
                strcpy(symtab[insymtab(label)].label,label);
                symtab[insymtab(label)].address = locctr;
                symtab[insymtab(label)].flag = 0;
            }
            else
            {

                //label not in symtab. So insert to symtab and assign address
                strcpy(symtab[symtabsize].label,label);
                symtab[symtabsize].address = locctr;
                symtab[symtabsize].flag = 1;
                symtabsize++;
            }
        }
        printf("%i",locctr);

        //operator
        if (inoptab(opcode) == 1)
        {

            strcpy(sic[linecount].opcode,opcode);
        }
        printf("-%i-",locctr);
        locctr++;

        //operand
        if (issymbol(operand) == 1)
        {
            //the operand is a symbol

            if (insymtab(operand) >= 0)
            {

                //operand already in symtab.
                //add operand address to list in symtab[j]
                strcpy(sic[linecount].operand,operand);

                strcpy(symtab[insymtab(operand)].label,operand);
                symtab[insymtab(operand)].list[symtab[insymtab(operand)].lsize]=linecount;
                symtab[symtabsize].lsize++;

                if (symtab[insymtab(operand)].flag == 0)
                {

                }
                else if (symtab[insymtab(operand)].flag == 1)
                {

                    //address is already present
                    //take down address
                }
            }
            else
            {
                strcpy(sic[linecount].operand,operand);
                //operand not in symtab.
                //add operand to symtab and add operand address to symtab.list
                strcpy(symtab[symtabsize].label,operand);
                symtab[symtabsize].address = locctr;
                symtab[symtabsize].flag = 0;
                symtab[symtabsize].lsize=0;
                symtab[symtabsize].list[symtab[symtabsize].lsize]=linecount;
                symtab[symtabsize].lsize++;
                symtabsize++;
            }

        }
        else
        {
            strcpy(sic[linecount].operand,operand);
        }
        printf("%i\n",locctr);
        locctr++;

        linecount++;
    }

    printsymtab();
    printsic();
}