#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

#include "custom.h"

FILE* fpmain;
int reg[25]= {0};
int spReg[7]= {0};

/*spReg
[0] 0 --> positive
[0] 1 --> negative
[0] 2 --> zero
[1] 
[2] PC */
int spretemp=0;
char q[10] = {'-', '-','-', '-','-', '-','-', '-','-', '-'};
int qctr=0;
int last=0;
int add=0;

//to compare instruction with OpCodes
int compare(char inst[],char code[],int len){
	int equal;
	//printf("inst code%s %s\n",inst, code );
	for(int i=0; i<len; i++){
		if(inst[i] != code[i])
			return 0;
	}
	return 1;
}

//calculate power of 2
int pow2(int power){
	int value = 1;
	//printf("power%d\n", power);
	while(power>0){
		value*=2;
		power--;
	}
	return value;

}

//convert bitstring to decimal
int convertBin(char binVal[]){
	int len = strlen(binVal);
	int value=0;
	int power = 0;
	//printf("binval=%s\n", binVal);
	for(int i=len-1; i>=0; i--){
		if(binVal[i] == '1'){
			//printf("power = %d\n", power);
			value += pow2(power);
		}
		power++;
		//printf("value = %d\n", value);
	}
	//printf("valin%d\n", value);
	return value;
}

void mult(char inst[]){
	char addr[3][6];
	int i;
	int ctr;
	//printf("%s\n", inst);
	for(i=1;i<=5; i++)
		addr[0][i-1] = inst[i];
	ctr = 0;
	addr[0][i-1] = '\0';
	while(i<=10){
		addr[1][ctr] = inst[i];
		ctr++;
		i++;
	}
	addr[1][ctr] = '\0';
	ctr = 0;
	while(i<=15){
		addr[2][ctr] = inst[i];
		ctr++;
		i++;
	}
	addr[2][ctr] = '\0';
	int val[3];
	for(int j=0;j<3;j++){
		//printf("%s\n", addr[j]);
		val[j] = convertBin(addr[j]);
	}
	int temp;
	//printf("%d %d\n",val[1], val[2] );
	temp = reg[val[1]]*reg[val[0]];
	reg[val[2]] =temp;
	spretemp++;
	printf("\n\tALU : MULTIPLY\n");
	printf("\tMULTIPLYING R%d with R%d and STORING in R%d\n",val[0],val[1],val[2]);
}

void move(char inst[]){
	char addr[2][9];
	int i;
	int ctr;
	for(i=3;i<=10; i++)
		addr[0][i-3] = inst[i];
	addr[0][i-3] = '\0';
	ctr = 0;
	while(i<=15){
		addr[1][ctr] = inst[i];
		ctr++;
		i++;
	}
	addr[1][ctr] = '\0';
	int val[2];
	for(int j=0;j<2;j++){
		val[j] = convertBin(addr[j]);
	}
	reg[val[1]] = val[0];
	spretemp++;
	printf("\n\tMOVING %d to R%d\n",val[0],val[1]);
}

void mov(char inst[]){
	// printf("fsdfsdfsdfsd\n");
	char addr[2][6];
	int i;
	int ctr;
	for(i=6;i<=10; i++)
		addr[0][i-6] = inst[i];
	ctr = 0;
	addr[0][i-6] = '\0';
	//printf("add0 %s\n", addr[0]);
	while(i<=15){
		addr[1][ctr] = inst[i];
		ctr++;
		i++;
	}
	addr[1][ctr] = '\0';
	int val[2];
	for(int j=0;j<2;j++){
		//printf("addr j %s\n", addr[j]);
		val[j] = convertBin(addr[j]);
	}
	//printf("%d %d\n",val[0], val[1]);
	reg[val[1]] = reg[val[0]];
	spretemp++;
	printf("\n\tMOVING R%d to R%d\n",val[0],val[1]);
}

void cmp(char inst[]){
	char addr[2][9];
	int i;
	int ctr;
	for(i=3;i<=10; i++)
		addr[0][i-3] = inst[i];
	addr[0][i-3] = '\0';
	ctr = 0;
	while(i<=15){
		addr[1][ctr] = inst[i];
		ctr++;
		i++;
	}
	addr[1][ctr] = '\0';
	int val[2];
	for(int j=0;j<2;j++){
		val[j] = convertBin(addr[j]);
	}
	if(val[0]>reg[val[1]])
		spReg[0] = 1;
	else if(val[0]<reg[val[1]])
		spReg[0] = 0;
	else
		spReg[0] = 2;
	spretemp++;
	printf("\n\tALU : COMPARE\n");
	printf("\tCOMPARING R%d with %d\n",val[1],val[0]);
}

void comp(char inst[]){
	char addr[2][6];
	int i;
	int ctr;
	for(i=6;i<=10; i++)
		addr[0][i-6] = inst[i];
	addr[0][i-6] = '\0';
	ctr = 0;
	while(i<=15){
		addr[1][ctr] = inst[i];
		ctr++;
		i++;
	}
	addr[1][ctr] = '\0';
	int val[2];
	for(int j=0;j<2;j++){
		val[j] = convertBin(addr[j]);
	}
	if(reg[val[0]]>reg[val[1]])
		spReg[0] = 1;
	else if(reg[val[0]]<reg[val[1]])
		spReg[0] = 0;
	else
		spReg[0] = 2;
	spretemp++;
	printf("\n\tALU : COMPARE\n");
	printf("\tCOMPARING R%d with value of R%d\n",val[1],val[0]);
}

void jeq(char inst[]){
	char addr[11];
	int val;
	int i;
	if(spReg[0] == 2){
		for(i=6;i<=15; i++)
			addr[i-6] = inst[i];
		addr[i-6] = '\0';
		val =  convertBin(addr);
		fseek(fpmain, 16*val+1, SEEK_SET);
		spretemp=val;
		printf("\n\tMOVING to LABEL %d\n",val);
	}
	else
		spretemp++;
}

void jnq(char inst[]){
	char addr[11];
	int val;
	int i;
	if(spReg[0] != 2){
		for(i=6;i<=15; i++)
			addr[i-6] = inst[i];
		addr[i-6] = '\0';
		val =  convertBin(addr);
		fseek(fpmain, 16*val+1, SEEK_SET);
		spretemp=val;
		printf("\n\tMOVING to LABEL %d\n",val);
	}
	else
		spretemp++;
}

void jle(char inst[]){
	char addr[11];
	int val;
	int i;
	if(spReg[0] == 1 || spReg[0] == 2){
		for(i=6;i<=15; i++)
			addr[i-6] = inst[i];
		addr[i-6] = '\0';
		val =  convertBin(addr);
		fseek(fpmain, 16*val+1, SEEK_SET);
		spretemp=val;
		printf("\n\tMOVING to LABEL %d\n",val);
	}
	else
		spretemp++;
}

void jgt(char inst[]){
	char addr[11];
	int val;
	int i;
	if(spReg[0] == 0){
		for(i=6;i<=15; i++)
			addr[i-6] = inst[i];
		addr[i-6] = '\0';
		val =  convertBin(addr);
		fseek(fpmain, 16*val+1, SEEK_SET);
		spretemp=val;
		printf("\n\tMOVING to LABEL %d\n",val);
	}
	else
		spretemp++;
}

void jmp(char inst[]){
	char addr[11];
	int val;
	int i;
	for(i=6;i<=15; i++)
		addr[i-6] = inst[i];
	addr[i-6] = '\0';
	val =  convertBin(addr);
	fseek(fpmain, 16*val+1, SEEK_SET);
	spretemp=val;
	printf("\n\tMOVING to LABEL %d\n",val);
}

void jge(char inst[]){
	char addr[11];
	int val;
	int i;
	if(spReg[0] == 0 || spReg[0] == 2){
		for(i=6;i<=15; i++)
			addr[i-6] = inst[i];
		addr[i-6] = '\0';
		val =  convertBin(addr);
		fseek(fpmain, 16*val+1, SEEK_SET);
		spretemp=val;
		printf("\n\tMOVING to LABEL at line %d\n",val);
	}
	else
		spretemp++;
}

void inc(char inst[]){
	int i;
	int val;
	char addr[6];
	for(i=11;i<=15; i++)
		addr[i-11] = inst[i];
	addr[i-11] = '\0';
	val =  convertBin(addr);
	reg[val]++;
	spretemp++;
	printf("\n\tALU : ADDING\n");
	printf("\tINCREMENTING R%d\n",val);
}

void hlt(){
	printf("\n\tProgram Terminated!\n\n");
	fclose(fpmain);
}

//queue bag
void queue(char inst[]){
	if(qctr==10){
		spReg[1]=1;
	}
	else{
		int i;
		char addr[6];
		for(i=11;i<=15; i++)
			addr[i-11] = inst[i];
		addr[i-11] = '\0';
		int val =  convertBin(addr);
		//printf("%d\n", val);
		q[qctr] = reg[val]+48;
		qctr++;
		printf("\tENQUEUE value of R%d\n",val);
		spReg[4]=qctr-1;
	}
	spretemp++;
}

//deque bag
void deq(char inst[]){
	//printf("qctr%d\n", qctr);
	if(qctr==0){
		spReg[1]=1;
	}
	else{
		int i;
		if(spReg[6]-q[0]!=-1){
			spReg[1]=1;
		}
		spReg[6]=q[0]-48;
		for(i=1;i<qctr;i++){
			q[i-1]=q[i];
		}
		qctr--;
		q[qctr]='-';
		
		printf("\tDEQUEUE BAG\n");
		spReg[4]=qctr-1;
	}
	if(spReg[4]<0)
		spReg[4]=0;
	spretemp++;
}

void line(){
	printf("\t");
	for(int i=0;i<33;i++){
		printf("-");
	}
}
void display(){
	printf("\t");
	line();
	printf("\t");
	line();
	printf("\n");
	printf("\t\t|\tReg\t|\tVal\t|\t\t|\tSpReg\t|\tVal\t|");
	printf("\t\tADDR\tINST\n\t");
	line();
	printf("\t");
	line();
	printf("\n");
	FILE* fp;
    char bin[30];
    int temp;
    int printFlag=0;
    fp = fopen("assembly.txt", "r");
	for(int i = 0; !feof(fp) || i<25; i++){
        printf("\t");
        if(!feof(fp)){
        	fscanf(fp, "%d %[^\n]\n", &temp, bin);
        	printFlag=1;
		}
        if(i==last){
        	spReg[2]=temp+add;
        }
		if(i<25)
			printf("\t|\tR%d\t|\t%d\t|\t",i, reg[i]);
		if(i==0)
			printf("\t|\tFR1\t|\t%d\t|\t",spReg[i]);
		if(i==1)
			printf("\t|\tFR2\t|\t%d\t|\t",spReg[i]);
		if(i==2)
			printf("\t|\tPC\t|\t%d\t|\t",spReg[i]);
		if(i==3)
			printf("\t|\tTOP\t|\t%d\t|\t",spReg[i]);
		if(i==4)
			printf("\t|\tEND\t|\t%d\t|\t",spReg[i]);
		if(i==5)
			printf("\t|\tSR\t|\t%d\t|\t",spReg[i]);
		if(i==6)
			printf("\t|\tLB\t|\t%d\t|\t",spReg[i]);
		if(i==7){
			line();
			printf("\t");
		}
		if(i == 11){
			printf("\tQUEUE : ");
			for(int p=0;p<10;p++)
				printf("%c ", q[p]);
		}
		if(printFlag){
			if(i==11){
				printf("\t\t");
			}
			else if(i>7)
				printf("\t\t\t\t\t\t");
        	printf("\t%d\t%s ",temp, bin);
        	printFlag=0;
		}
        if(i==last){
        	printf("<--");
        }
		printf("\n");
	}
	printf("\t");
	line();
	printf("\nPC : %d\n",spReg[2]);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	fclose(fp);
}

void decode(char inst[]){
	FILE* fp;
	genformat a;
	int l, found=0;
	//printf("inst %s\n", inst);
	fp = fopen("OpCodes.txt", "r");
	while((!feof(fp)) && !found){
        fscanf(fp, "%s %s", a.code, a.bin);
        
        l = strlen(a.bin);
        if(compare(inst, a.bin, l))
        	found = 1;
        //printf("%s %s\n", a.code, a.bin);
        //printf("found%d\n", found);
    }
    //printf("code %s %s\n", a.code, a.bin);
    last = spretemp;
    if(strcmp(a.code, "MULT")==0)
    	mult(inst);
    else if(strcmp(a.code, "MOVE")==0)
    	move(inst);
    else if(strcmp(a.code, "MOV")==0)
    	mov(inst);
    else if(strcmp(a.code, "COMP")==0)
    	comp(inst);
    else if(strcmp(a.code, "CMP")==0)
    	cmp(inst);
    else if(strcmp(a.code, "QUEUE")==0)
    	queue(inst);
    else if(strcmp(a.code, "DEQ")==0)
    	deq(inst);
    else if(strcmp(a.code, "JMP")==0)
    	jmp(inst);
    else if(strcmp(a.code, "JGT")==0)
    	jgt(inst);
    else if(strcmp(a.code, "JEQ")==0)
    	jeq(inst);
    else if(strcmp(a.code, "JNQ")==0)
    	jnq(inst);
    else if(strcmp(a.code, "HLT")==0)
    	hlt();
    else if(strcmp(a.code, "INC")==0)
    	inc(inst);
    else if(strcmp(a.code, "JLE")==0)
    	jle(inst);
    else if(strcmp(a.code, "JGE")==0)
    	jge(inst);
    spReg[5]=spReg[0];
    if(strcmp(a.code,"MOV")==0 || strcmp(a.code,"COMP")==0)
        add=6;
    else if(strcmp(a.code,"MOVE")==0||strcmp(a.code,"MULT")==0||strcmp(a.code,"CMP")==0)
        add=5;
    else if(strcmp(a.code,"INC")==0)
        add=3;
    else if(strcmp(a.code,"HLT")==0)
        add=1;
    else
        add=4;
    display();
    sleep(1);
    fclose(fp);
}

void main(){
	char inst[18];
	fpmain = fopen("binaryCode.txt", "r");
	while(!feof(fpmain)){
        fscanf(fpmain, "%s", inst);
        //printf("%s\n",inst);
        //printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
        //printf("%s\n", inst);
        if(!feof(fpmain))
        	decode(inst);
    }
}