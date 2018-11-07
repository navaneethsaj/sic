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
    if (sym[0]=='C' && sym[1]=='\'')
    {
        return 0;
    }

    if (sym[0]=='X' && sym[1]=='\'')
    {
        return 0;
    }
    return 1;
}

int inoptab(char sym[255])
{
    FILE *fp = fopen("optab.txt","r");
    char operand[255]="";
    char opcode[255]="";
    int op;
    while (strcmp(operand,"END")!=0)
    {
        fscanf(fp,"%s%s",operand,opcode);
        //printf("%s-%s",operand,opcode);
        if (strcmp(operand,sym)==0)
        {
            op = atoi(opcode);
            return op;
        }
    }
    return -1;
}

void substring(char *sym)
{

    char sub[255];
    for (int i = 2; i < strlen(sym)-1; ++i) {

        sub[i-2]=sym[i];
    }
    strcpy(sym,sub);
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
        if (symtab[j].flag == 0)
        {
            int i=0;
            while (i<symtab[j].lsize)
            {
                int k = symtab[j].list[i];
                char buffer[255];
                snprintf(buffer, 255, "%d", symtab[j].address);
                printf("\nrep%s-%s",symtab[j].label,buffer);
                strcpy(sic[k].operand,buffer);
                i++;
            }
        }
        j++;
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

        //operator
        if (inoptab(opcode) >= 0)
        {
            printf("insym %s\n",opcode);
            char buffer[255];
            locctr+=1;
            snprintf(buffer, 10, "%d", inoptab(opcode));
            strcpy(sic[linecount].opcode,buffer);
        }
        else if (strcmp(opcode,"WORD")==0)
        {
            locctr+=3;
            printf("-w%s%i\n",opcode,locctr);
            continue;
        }
        else if (strcmp(opcode,"BYTE")==0)
        {
            locctr+=1;
            printf("-b%s%i\n",opcode,locctr);
            continue;
        }
        else if (strcmp(opcode,"RESW")==0)
        {
            substring(operand);
            locctr+=(3*atoi(operand));
            printf("-rw%s%i\n",opcode,locctr);
            continue;
        }
        else if (strcmp(opcode,"RESB")==0)
        {
            substring(operand);
            locctr+=atoi(operand);
            printf("-rb%s%i\n",opcode,locctr);
            continue;
        }
        else
        {
            printf("\n\nInvalid operator\n\n");
            return 0;
        }

        //operand
        if (issymbol(operand) == 1)
        {
            //the operand is a symbol
            locctr+=2;
            if (insymtab(operand) >= 0)
            {

                //operand already in symtab.
                //add operand address to list in symtab[j]


                //strcpy(symtab[insymtab(operand)].label,operand);
                //symtab[insymtab(operand)].list[symtab[insymtab(operand)].lsize]=linecount;
                //symtab[symtabsize].lsize++;

                if (symtab[insymtab(operand)].flag == 0)
                {
                    strcpy(sic[linecount].operand,operand);
                    symtab[insymtab(operand)].list[symtab[insymtab(operand)].lsize]=linecount;
                    symtab[insymtab(operand)].lsize++;
                    //printf("\nno address present\n");
                }
                else if (symtab[insymtab(operand)].flag == 1)
                {
                    //printf("\naddress present\n");
                    char buffer[255];
                    snprintf(buffer, 10, "%d", symtab[insymtab(operand)].address);
                    strcpy(sic[linecount].operand,buffer);
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
            locctr+=2;
            substring(operand);
            strcpy(sic[linecount].operand,operand);
        }
        printf("%i\n",locctr);

        linecount++;
    }

    repopulateaddr();
    printsymtab();
    printsic();
}