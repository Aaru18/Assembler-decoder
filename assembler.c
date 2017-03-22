#include<stdio.h>
#include<string.h>
#include "custom.h"

int addr=0;
/* Function to print all Available OP Codes */
void showOpCodes(){
    FILE* fp;
    genformat a;
    fp = fopen("OpCodes.txt", "r");
    printf("\n\tOP CODES\n\n");
    while(!feof(fp)){
        fscanf(fp, "%s %s", a.code, a.bin);
        printf("\t%s %s\n", a.code, a.bin);
    }
    printf("\n");
    fclose(fp);
    printf("\n\tPress Enter to Return to main Menu...");
    char ch;
    ch = getchar();
    ch = getchar();
}

/* Function to print all Available Register Codes */
void showRegCodes(){
    FILE* fp;
    genformat a;
    fp = fopen("regCodes.txt", "r");
    printf("\n\tREGISTER CODES\n\n");
    while(!feof(fp)){
        fscanf(fp, "%s %s", a.code, a.bin);
        if(a.code[0] == 'R'){
            printf("\t%s %s\n", a.code, a.bin);
        }
    }
    printf("\n");
    fclose(fp);
    printf("\n\tPress Enter to Return to main Menu...");
    char ch;
    ch = getchar();
    ch = getchar();
}

/* Function to convert a number to binary */
void toBinary(int n, char l[]){
    int c, k;
    int ctr=0;
    for(c = 9; c >= 0; c--)
    {
        k = n >> c;
        if (k & 1)
            l[ctr] = '1';
        else
            l[ctr] = '0';
        ctr++;
    }
    l[ctr] = '\0';
}

/* Function to perform first pass of assembly code */
void firstPass(char name[]){
    char inst[100];
    FILE* fp;
    FILE* fp1;
    genformat a;
    int line=1;
    int lcount=0;
    char l[20];
    char opCode[18];
    int i, ctr;
    fp = fopen(name, "r");
    fp1 = fopen("table.txt", "w");
    printf("\n\n\tFirst Pass\n\tLABELS found :");
    while(!feof(fp)){
        fscanf(fp, "%[^\n]\n", inst);
        ctr = 0;
        i = 0;
        while(inst[i] != ' ' && inst[i] != '\0'){
            opCode[ctr] = inst[i];
            i++;
            ctr++;
        }
        opCode[ctr] = '\0';
        if(opCode[0]=='L' && (opCode[2] == ':' || opCode[3] == ':')){
            lcount++;
            toBinary(line-lcount, l);
            opCode[strlen(opCode)-1] = '\0';
            printf(" %s", opCode);
            fprintf(fp1, "%s %s\n", opCode, l);
        }
        line++;
    }
    fclose(fp);
    fclose(fp1);
}

/* Function to convert Assembly code to binary */
void fileConvert(){
    int add;
    int addash;
    char name[25];
    char inst[100];
    FILE* fp;
    FILE* fp1;
    FILE* fp2;
    FILE* fp3;
    genformat a;
    char opCode[18], reg[18];
    int i, ctr, found,foundLabel, invalid=0;
    do{
        printf("\tEnter Filename to convert to Binary : ");
        scanf("%s", name);
        fp = fopen(name, "r");
        if(!fp){
            printf("\n\tInvalid Filename!\n\n");
        }
    }while(!fp);
    firstPass(name);
    printf("\n\tDone\n\tKEYWORDS found : ");
    fp2 = fopen("binaryCode.txt", "w");
    fp3 = fopen("assembly.txt", "w");

    while(!feof(fp)){
        fscanf(fp, "%[^\n]\n", inst);
        ctr = 0;
        i = 0;
        foundLabel=0;
        while(inst[i] != ' ' && inst[i] != '\0'){
            opCode[ctr] = inst[i];
            i++;
            ctr++;
        }
        if(!(opCode[0]=='L' && (opCode[2] == ':' || opCode[3] == ':'))){
            opCode[ctr] = '\0';
            found = 1;
            fp1 = fopen("OpCodes.txt", "r");
            while(!feof(fp1) && found){
                fscanf(fp1, "%s %s", a.code, a.bin);
                if(strcmp(a.code, opCode) == 0){
                    printf(" %s ",opCode);
                    if(strcmp(opCode,"MOV")==0 || strcmp(opCode,"COMP")==0)
                        add=6;
                    else if(strcmp(opCode,"MOVE")==0||strcmp(opCode,"MULT")==0||strcmp(opCode,"CMP")==0)
                        add=5;
                    else if(strcmp(opCode,"INC")==0)
                        add=3;
                    else if(strcmp(opCode,"HLT")==0)
                        add=1;
                    else
                        add=4;
                    fprintf(fp2, "%s", a.bin);
                    found = 0;
                    fclose(fp1);
                }
            }
            if(found){
                printf(" %s ",opCode);
                printf("\n\n\tInvalid Instruction!\n\tAborting Conversion");
                printf("\n\n\tPress Enter to Return to main Menu...");
                char ch;
                ch = getchar();
                ch = getchar();
                return;
            }
        }
        
        else{
            foundLabel=1;
            add=0;
            addash=1;
        }
        
        while(inst[i] != '\0'){
            while(inst[i] == ' ' || inst[i] == ','){
                i++;
            }
            ctr = 0;
            while(inst[i] != ' ' && inst[i] != ',' && inst[i] != '\0'){
                reg[ctr] = inst[i];
                i++;
                ctr++;
            }
            reg[ctr] = '\0';
            printf(" %s ",reg);
            if(!(reg[0]=='L'))
            {
                found = 1;
                fp1 = fopen("regCodes.txt", "r");
                while(!feof(fp1) && found){
                    if(!feof(fp1)){
                        fscanf(fp1, "%s %s", a.code, a.bin);
                        if(strcmp(a.code, reg) == 0){
                            fprintf(fp2, "%s", a.bin);
                            found = 0;
                            fclose(fp1);
                        }
                    }
                }
                
                if(found){
                    printf(" %s ",reg);
                    printf("\n\n\tInvalid Instruction!\n\tAborting Conversion");
                    printf("\n\n\tPress Enter to Return to main Menu...");
                    char ch;
                    ch = getchar();
                    ch = getchar();
                    return;
                }
            }
            
            else{
                fp1 = fopen("table.txt", "r");
                found = 1;
                while(!feof(fp1) && found){
                    if(!feof(fp1)){
                        fscanf(fp1, "%s %s", a.code, a.bin);
                        if(strcmp(a.code, reg) == 0){
                            fprintf(fp2, "%s", a.bin);
                            found = 0;
                            fclose(fp1);
                        }
                    }
                }
                if(found){
                    printf("\n\n\tInvalid Instruction!\n\tAborting Conversion");
                    printf("\n\n\tPress Enter to Return to main Menu...");
                    char ch;
                    ch = getchar();
                    ch = getchar();
                    return;
                }
            }
        }
        if(!foundLabel){
            fprintf(fp2, "\n");
            if(!addash)
                fprintf(fp3, "%d %s\n", addr, inst);
            else{
                fprintf(fp3, "%d %s\n",addr, inst);
                addash=0;
            }
        }
        else
            fprintf(fp3, "%d %s ", addr, inst);
        addr+=add;
    }
    fclose(fp3);
    fclose(fp2);
    fclose(fp);
    printf("\n\n\tConverting to binary...");
    printf("\n\tConverted");
    printf("\n\n\tPress Enter to Return to main Menu...");
    char ch;
    ch = getchar();
    ch = getchar();
}

/* Function to print the binary code of Assembly code */
void showBinary(){
    FILE* fp;
    char bin[30];
    fp = fopen("binaryCode.txt", "r");
    if(!fp){
        printf("\n\tConvert file to binary first!\n");
        printf("\n\tPress Enter to Return to main Menu...");
        char ch;
        ch = getchar();
        ch = getchar();
        return;
    }
    printf("\n\tBINARY CODE\n\n");
    while(!feof(fp)){
        fscanf(fp, "%[^\n]\n", bin);
        printf("\t%s\n", bin);
    }
    fclose(fp);
    printf("\n\tPress Enter to Return to main Menu...");
    char ch;
    ch = getchar();
    ch = getchar();
}

/* Function to print symbol table */
void showTable(){
    FILE* fp;
    genformat a;
    fp = fopen("table.txt", "r");
    if(!fp){
        printf("\n\tConvert file to binary first!\n");
        printf("\n\tPress Enter to Return to main Menu...");
        char ch;
        ch = getchar();
        ch = getchar();
        return;
    }
    printf("\n\tSYMBOL TABLE\n\n");
    while(!feof(fp)){
        fscanf(fp, "%s %s", a.code, a.bin);
        if(!feof(fp))
            printf("\t%s %s\n", a.code, a.bin);
    }
    printf("\n");
    fclose(fp);
    printf("\n\tPress Enter to Return to main Menu...");
    char ch;
    ch = getchar();
    ch = getchar();
}

/* Function to print assembly code */
void showAssembly(){
    FILE* fp;
    char bin[30];
    fp = fopen("assembly.txt", "r");
    if(!fp){
        printf("\n\tConvert file to binary first!\n");
        printf("\n\tPress Enter to Return to main Menu...");
        char ch;
        ch = getchar();
        ch = getchar();
        return;
    }
    printf("\n\tASSEMBLY CODE\n\n");
    while(!feof(fp)){
        fscanf(fp, "%[^\n]\n", bin);
        printf("\t%s\n", bin);
    }
    fclose(fp);
    printf("\n\tPress any Key to Return to main Menu...");
    char ch;
    ch = getchar();
    ch = getchar();
}

/* Main Function */
void main(){
    int ch=0;
    do{
        printf("\n\n\t");
        for(int i=0;i<33; i++)
            printf("-");
        printf("\n\t|\t      Menu\t        |\n\t");
        for(int i=0;i<33; i++)
            printf("-");
        printf("\n\t|  1  | Show OP Codes\t\t|\n");
        printf("\t|  2  | Show Register Codes\t|\n");
        printf("\t|  3  | Convert File to binary\t|\n");
        printf("\t|  4  | Show Assembly Code\t|\n");
        printf("\t|  5  | Show Binary Code\t|\n");
        printf("\t|  6  | Show Symbol Table\t|\n");
        printf("\t|  0  | Exit\t\t\t|\n\t");
        for(int i=0;i<33; i++)
            printf("-");
        printf("\n\n\tEnter Choice : ");
        scanf("%d", &ch);
        printf("\n");
        switch(ch){
            case 1:showOpCodes();
                break;
            case 2:showRegCodes();
                break;
            case 3:fileConvert();
                break;
            case 4:showAssembly();
                break;
            case 5:showBinary();
                break;
            case 6:showTable();
                break;
            case 0:break;
            default: printf("Invalid Input!\n");
                break;
        }
    }while(ch != 0);
}