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

struct struct_header
{

    char col1[255]; //H
    char col2[255]; //pgmname
    char col3[255]; //start addr
    int col4; //length
};
struct struct_text
{

    char col1[255];//T
    char col2[255];//start
    int col3;//length
    char col4[255];//obj
};
struct struct_end
{

    char col1[255];//E
    char col2[255];//start addr
};

struct struct_symtab symtab[255];
struct struct_sic sic[255];
struct struct_header headerrec;
struct struct_end endrec;
struct struct_text textrec[255];

int trec_count=0;
int ctrec_length=0;
int symtabsize = 0;
int locctr=0;
int linecount=0;
int startaddr=0;
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
    for (int i = 2; i < strlen(sym) - 1; ++i) {

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
                printf("\nrep%s-%s-%i",symtab[j].label,buffer,k);
                strcpy(sic[k].operand,buffer);

                int m=0;
                int p=0;
                while (p<strlen(textrec[trec_count].col4))
                {

                    if (textrec[trec_count].col4[p]=='^')
                    {
                            m++;
                            if (m+1==k)
                            {
                                break;
                            }
                    }
                    ++p;
                }
                char str1[255]="";
                char str2[255]="";
                strncpy(str1,textrec[trec_count].col4,p+3);
                strcat(str1,buffer);
                strcpy(str2,textrec[trec_count].col4+p+3);
                strcpy(textrec[trec_count].col4, str1);
                strcat(textrec[trec_count].col4, str2);
                printf("s1 %s- s2 %s",str1,str2);
//                textrec[trec_count].col4[m+3]=
                //printf("m-%i-%i\n",m,p);
                //strcat(textrec[trec_count].col4,buffer);
                //strcat(textrec[trec_count].col4,"^");
                ctrec_length+=2;
                i++;
            }
        }
        j++;
    }
}

void printheader()
{
    printf("\nHEADER\n");
    printf("%s-%s-%s-%i\n",headerrec.col1,headerrec.col2,headerrec.col3,headerrec.col4);
}

void printend()
{

    printf("\nEND\n");
    printf("%s-%s\n",endrec.col1,endrec.col2);
}

void printtext()
{

    printf("\nTEXT\n");
    int i=0;
    while (i<trec_count)
    {
        printf("%s-%s-%i-%s\n",textrec[i].col1,textrec[i].col2,textrec[i].col3,textrec[i].col4);
        ++i;
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
            strcpy(headerrec.col1,"H");
            strcpy(headerrec.col2,label);
            strcpy(headerrec.col3,operand);
            strcpy(endrec.col1,"E");
            strcpy(endrec.col2,operand);
            strcpy(textrec[trec_count].col1,"T");
            strcpy(textrec[trec_count].col2,operand);
            strcpy(textrec[trec_count].col4,"");
//            trec_count++;
//            strcpy(textrec[trec_count].col3,);
//            strcpy(textrec[trec_count].col4,"");
            locctr=atoi(operand);
            startaddr = locctr;
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
            //printf("insym %s\n",opcode);
            char buffer[255];
            locctr+=1;
            snprintf(buffer, 10, "%d", inoptab(opcode));
            strcat(textrec[trec_count].col4,"^");
            strcat(textrec[trec_count].col4,buffer);
            ctrec_length+=1;
            strcpy(sic[linecount].opcode,buffer);
        }
        else if (strcmp(opcode,"WORD")==0)
        {
            locctr+=3;
            char val[255]="";
            for (int i = 0; i < strlen(operand) - 3; ++i) {
                val[i]=operand[i+2];
            }
            //printf("operand %s %s %i\n",val,operand,strlen(operand));
            strcpy(sic[linecount].opcode,val);
            strcat(textrec[trec_count].col4,val);
            ctrec_length+=3;
            linecount++;
            //printf("-w%s%i\n",opcode,locctr);
            continue;
        }
        else if (strcmp(opcode,"BYTE")==0)
        {
            locctr+=1;
            char val[255]="";
            for (int i = 0; i < strlen(operand) - 3; ++i) {
                val[i]=operand[i+2];
            }
            printf("operand %s %s\n",val,operand);
            strcpy(sic[linecount].opcode,val);
            strcat(textrec[trec_count].col4,val);
            ctrec_length+=1;
            linecount++;
            continue;
        }
        else if (strcmp(opcode,"RESW")==0)
        {
            substring(operand);
            locctr+=(3*atoi(operand));
            textrec[trec_count].col3=ctrec_length;
            ctrec_length=0;
            trec_count++;
            strcpy(textrec[trec_count].col1,"T");
            strcpy(textrec[trec_count].col4,"");
            char buffer[255];
            locctr+=1;
            snprintf(buffer, 255, "%d", locctr);
            strcpy(textrec[trec_count].col2,buffer);
            //printf("-rw%s%i\n",opcode,locctr);
            continue;
        }
        else if (strcmp(opcode,"RESB")==0)
        {
            substring(operand);
            locctr+=atoi(operand);
            textrec[trec_count].col3=ctrec_length;
            ctrec_length=0;
            trec_count++;
            strcpy(textrec[trec_count].col1,"T");
            strcpy(textrec[trec_count].col4,"");
            char buffer[255];
            locctr+=1;
            snprintf(buffer, 255, "%d", locctr);
            strcpy(textrec[trec_count].col2,buffer);
            //printf("-rb%s%i\n",opcode,locctr);
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
                    strcat(textrec[trec_count].col4,buffer);
                    ctrec_length+=2;
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
    textrec[trec_count].col3=ctrec_length;
    trec_count++;

    headerrec.col4=locctr-startaddr;
    printsymtab();
    printsic();

    printheader();
    printtext();
    printend();
}