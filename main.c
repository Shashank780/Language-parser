#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_TOKENS (1000000)
#define MAX_LINE_LENGTH (1000000)
#define EXIT_SUCCESS 0

char** stack[1000];
int pointer=0;

void push(char* c){
    stack[pointer++]=(char**)c;
}

char* pop(){
    return (char*)stack[--pointer];
}

bool empty(){
    return pointer==0;
}

int stacksize[1000];
int ptr=0;

void push_size(int n){
    stacksize[ptr++]=n;
}

int pop_size(){
    return stacksize[--ptr];
}

char tokens[MAX_TOKENS];
int num_tokens;
int prgptr=0;

void tree();
void result();

/**
Reads the passed input file line by line.
Removes all space type characters from the lines, including \n (Carriage Return). 
Modifies the global tokens and num_tokens variables. 
*/


void tokenize(char* filename) {

    FILE* in_file = fopen(filename, "r");

    char line[MAX_TOKENS];
    while (fgets(line, MAX_LINE_LENGTH, in_file)) {
        int leng=strlen(line);
        bool ignore=false;
        for (int i = 0; i < leng; ++i) {
            if (line[i] == '\t' || line[i] == 10 /* ASCII 10 = Line Feed */ || line[i] == '\n'|| line[i]== '\0'||line[i]==13)continue;
            if(i+3<leng&&(line[i]=='i'&&line[i+1]=='n'&&line[i+2]=='t'&&line[i+3]==' ')){
                ignore=true;
            }
            if(i+4<leng&&(line[i]=='r'&&line[i+1]=='e'&&line[i+2]=='a'&&line[i+3]=='d'&&line[i+4]==' ')){
                ignore=true;
            }
            if(i+5<leng&&(line[i]=='w'&&line[i+1]=='r'&&line[i+2]=='i'&&line[i+3]=='t'&&line[i+4]=='e'&&line[i+5]==' ')){
                ignore=true;
            }
            if(line[i]==' '){
                if(!ignore)continue;
                else ignore=false;
            }
            tokens[num_tokens] = line[i];
            num_tokens++;
        }
        memset(line, '.', sizeof(line));
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        perror("Pass the name of the input file as the first parameter. e.g.: ./simulator input.txt");
        exit(EXIT_FAILURE);
    }
   
    tokenize(argv[1]);
    if(num_tokens==0){printf("Empty program\n"); return(0);}
    char* c=(char*)malloc(2*sizeof(char));
    c[0]='_';c[1]='S';
    push(c);
    push_size(2);
    printf("\n");
    //printf("[");
    tree();
    //printf("]");
    if(num_tokens>2){
        if(tokens[num_tokens-1]==';'&&tokens[num_tokens-2]=='}')printf("]");
    }
     result(argv[1]);
    return EXIT_SUCCESS;
}

void start(){
    char* c;
    c=(char*)malloc(sizeof(char));
    c[0]=']';
    push(c);push_size(1);
    c=(char*)malloc(2*sizeof(char));
    c[0]='_';c[1]='F';
    push(c);push_size(2);
    c=(char*)malloc(sizeof(char));
    c[0]=';';
    push(c);push_size(1);
    c=(char*)malloc(2*sizeof(char));
    c[0]='_';c[1]='D';
    push(c);push_size(2);
    c=(char*)malloc(sizeof(char));
    c[0]=' ';
    push(c);push_size(1);
    c=(char*)malloc(3*sizeof(char));
    c[0]='i';c[1]='n';c[2]='t';
    push(c);push_size(3);
    tree();
}

void declare(){
    char* c;
    c=(char*)malloc(sizeof(char));
    c[0]=']';
    push(c);push_size(1);
    int i=prgptr;
    for(;i<num_tokens;i++){
        if(tokens[i]==';')break;
        if(tokens[i]==','){
            c=(char*)malloc(2*sizeof(char));
            c[0]='_';c[1]='D';
            push(c);push_size(2);
            c=(char*)malloc(sizeof(char));
            c[0]=',';
            push(c);push_size(1);
            tokens[i]='.';
            break;
        }
    }
    c=(char*)malloc(2*sizeof(char));
    c[0]='_';c[1]='V';
    push(c);push_size(2);
    tree();
}

void write(){
    char* c;
    c=(char*)malloc(sizeof(char));
    c[0]=']';
    push(c);push_size(1);
    if(isdigit(tokens[prgptr+5])){
            c=(char*)malloc(2*sizeof(char));
            c[0]='_';c[1]='N';
            push(c);push_size(2);
    }
    else{
            c=(char*)malloc(2*sizeof(char));
            c[0]='_';c[1]='V';
            push(c);push_size(2);
    }
    c=(char*)malloc(sizeof(char));
    c[0]=' ';
    push(c);push_size(1);
    c=(char*)malloc(5*sizeof(char));
    c[0]='w';c[1]='r';c[2]='i';c[3]='t';c[4]='e';
    push(c);push_size(5);
    tree();
}

void read(){
    char* c;
    c=(char*)malloc(2*sizeof(char));
    c[0]='_';c[1]='V';
    push(c);push_size(2);    
    c=(char*)malloc(sizeof(char));
    c[0]=' ';
    push(c);push_size(1);
    c=(char*)malloc(4*sizeof(char));
    c[0]='r';c[1]='e';c[2]='a';c[3]='d';
    push(c);push_size(4);
    tree();
}

void assign(){
    char* c;
    c=(char*)malloc(sizeof(char));
    c[0]=']';
    push(c);push_size(1);
    c=(char*)malloc(2*sizeof(char));
    c[0]='_';c[1]='E';
    push(c);push_size(2);
    c=(char*)malloc(sizeof(char));
    c[0]='=';
    push(c);push_size(1);
    c=(char*)malloc(2*sizeof(char));
    c[0]='_';c[1]='V';
    push(c);push_size(2);
    tree();
}

void rest(){
    char* c;
    c=(char*)malloc(sizeof(char));
    c[0]=']';
    push(c);push_size(1);
    if(prgptr!=num_tokens&&tokens[prgptr]!='}'){
        int i=prgptr;
        c=(char*)malloc(2*sizeof(char));
        c[0]='_';c[1]='F';
        push(c);push_size(2);
        c=(char*)malloc(sizeof(char));
        c[0]=';';
        push(c);push_size(1);
        int j=0;
        if(i+4<num_tokens&&(tokens[i]=='r'&&tokens[i+1]=='e'&&tokens[i+2]=='a'&&tokens[i+3]=='d'&&tokens[i+4]==' ')){
            c=(char*)malloc(2*sizeof(char));
            c[0]='_';c[1]='R';
            push(c);push_size(2);
            j++;

        }
        if(i+5<num_tokens&&(tokens[i]=='w'&&tokens[i+1]=='r'&&tokens[i+2]=='i'&&tokens[i+3]=='t'&&tokens[i+4]=='e'&&tokens[i+5]==' ')){
            c=(char*)malloc(2*sizeof(char));
            c[0]='_';c[1]='W';
            push(c);push_size(2);
            j++;

        }
        if(i+3<num_tokens&&(tokens[i]=='f'&&tokens[i+1]=='o'&&tokens[i+2]=='r'&&tokens[i+3]=='(')){
            c=(char*)malloc(2*sizeof(char));
            c[0]='_';c[1]='L';
            push(c);push_size(2);
            j++;
        }
        if(j==0){
            c=(char*)malloc(2*sizeof(char));
            c[0]='_';c[1]='M';
            push(c);push_size(2);
        }
    }
    else printf("[]");
    tree();
}

void exp1(){
    char* c;
    c=(char*)malloc(sizeof(char));
    c[0]=']';
    push(c);push_size(1);
    int i;
    c=(char*)malloc(2*sizeof(char));
    c[0]='_';c[1]='B';
    push(c);push_size(2);
    int bracket=0;
    for(i=prgptr;i<num_tokens;i++)if(tokens[i]==';'||(i+1<num_tokens&&tokens[i]==')'&&tokens[i+1]=='{'))break;
    for(i=i-1;i>=prgptr;i--){
        if(tokens[i]==')')bracket++;
        if(tokens[i]=='(')bracket--;
        if(tokens[i]=='>'&&bracket==0){
            tokens[i]='.';
            c=(char*)malloc(sizeof(char));
            c[0]='>';
            push(c);push_size(1);
            c=(char*)malloc(2*sizeof(char));
            c[0]='_';c[1]='E';
            push(c);push_size(2);
            break;
        }
        if(i-1>=prgptr&&tokens[i-1]=='='&&tokens[i]=='='&&bracket==0){
            tokens[i]='.';
            tokens[i-1]='.';
            c=(char*)malloc(2*sizeof(char));
            c[0]='=';c[1]='=';
            push(c);push_size(2);
            c=(char*)malloc(2*sizeof(char));
            c[0]='_';c[1]='E';
            push(c);push_size(2);
            break;           
        }
    }
    tree();
}

void exp22(){
    char* c;
    c=(char*)malloc(sizeof(char));
    c[0]=']';
    push(c);push_size(1);
    int i;
    c=(char*)malloc(2*sizeof(char));
    c[0]='_';c[1]='A';
    push(c);push_size(2);
    int bracket=0;
    for(i=prgptr;i<num_tokens;i++)if(tokens[i]==';'||(i+1<num_tokens&&tokens[i]==')'&&tokens[i+1]=='{'))break;
    for(i=i-1;i>=prgptr;i--){
        if(tokens[i]==')')bracket++;
        if(tokens[i]=='(')bracket--;
        if(tokens[i]=='+'&&bracket==0){
            tokens[i]='.';
            c=(char*)malloc(sizeof(char));
            c[0]='+';
            push(c);push_size(1);
            c=(char*)malloc(2*sizeof(char));
            c[0]='_';c[1]='B';
            push(c);push_size(2);
            break;
        }
        if(tokens[i]=='-'&&bracket==0){
            tokens[i]='.';
            c=(char*)malloc(sizeof(char));
            c[0]='-';
            push(c);push_size(1);
            c=(char*)malloc(2*sizeof(char));
            c[0]='_';c[1]='B';
            push(c);push_size(2);
            break;           
        }
    }
    tree();
}

void exp3(){
    char* c;
    c=(char*)malloc(sizeof(char));
    c[0]=']';
    push(c);push_size(1);
    int i;
    c=(char*)malloc(2*sizeof(char));
    c[0]='_';c[1]='C';
    push(c);push_size(2);
    int bracket=0;
    for(i=prgptr;i<num_tokens;i++)if(tokens[i]==';'||(i+1<num_tokens&&tokens[i]==')'&&tokens[i+1]=='{'))break;
    for(i=i-1;i>=prgptr;i--){
        if(tokens[i]==')')bracket++;
        if(tokens[i]=='(')bracket--;
        if(tokens[i]=='*'&&bracket==0){
            tokens[i]='.';
            c=(char*)malloc(sizeof(char));
            c[0]='*';
            push(c);push_size(1);
            c=(char*)malloc(2*sizeof(char));
            c[0]='_';c[1]='A';
            push(c);push_size(2);
            break;
        }
        if(tokens[i]=='/'&&bracket==0){
            tokens[i]='.';
            c=(char*)malloc(sizeof(char));
            c[0]='/';
            push(c);push_size(1);
            c=(char*)malloc(2*sizeof(char));
            c[0]='_';c[1]='A';
            push(c);push_size(2);
            break;           
        }
    }
    tree();
}

void exp4(){
    char* c;
    c=(char*)malloc(sizeof(char));
    c[0]=']';
    push(c);push_size(1);
    if(tokens[prgptr]=='('){
        c=(char*)malloc(sizeof(char));
        c[0]=')';
        push(c);push_size(1);
        c=(char*)malloc(2*sizeof(char));
        c[0]='_';c[1]='E';
        push(c);push_size(2);
        c=(char*)malloc(sizeof(char));
        c[0]='(';
        push(c);push_size(1);
        int brackets=1;
        tokens[prgptr]=='.';
        for(int i=prgptr+1;i<num_tokens;i++){
            if(tokens[i]==';'||(i+1<num_tokens&&tokens[i]==')'&&tokens[i+1]=='{'))break;
            if(tokens[i]=='(')brackets++;
            if(tokens[i]==')')brackets--;
            if(brackets==0){
                tokens[i]='.';
                break;
            }
        }  
    }
    else{
        if(isdigit(tokens[prgptr])){
            c=(char*)malloc(2*sizeof(char));
            c[0]='_';c[1]='N';
            push(c);push_size(2);
        }
        else{
            c=(char*)malloc(2*sizeof(char));
            c[0]='_';c[1]='V';
            push(c);push_size(2);
        }
    }
    tree();
}

void foor(){
    char* c;
    c=(char*)malloc(sizeof(char));
    c[0]=']';
    push(c);push_size(1);
    c=(char*)malloc(sizeof(char));c[0]='}';push(c);push_size(1);
    c=(char*)malloc(2*sizeof(char));c[0]='_';c[1]='F';push(c);push_size(2);
    c=(char*)malloc(sizeof(char));c[0]='{';push(c);push_size(1);
    c=(char*)malloc(sizeof(char));c[0]=')';push(c);push_size(1);
    c=(char*)malloc(2*sizeof(char));c[0]='_';c[1]='M';push(c);push_size(2);
    c=(char*)malloc(sizeof(char));c[0]=';';push(c);push_size(1);
    c=(char*)malloc(2*sizeof(char));c[0]='_';c[1]='E';push(c);push_size(2);
    c=(char*)malloc(sizeof(char));c[0]=';';push(c); push_size(1);
    c=(char*)malloc(2*sizeof(char));c[0]='_';c[1]='M';push(c);push_size(2);
    c=(char*)malloc(sizeof(char));c[0]='(';push(c);push_size(1);
    c=(char*)malloc(3*sizeof(char));c[0]='f';c[1]='o';c[2]='r';push(c);push_size(3);
    tree();
}

void var(){
    char* c;
    c=(char*)malloc(sizeof(char));
    c[0]=']';
    push(c);push_size(1);
    int j=0;
    for(int i=prgptr;i<num_tokens;i++){
        if(!isalpha(tokens[i]))break;
        j++;
    }
    c=(char*)malloc(j*sizeof(char));
    for(int i=prgptr;i<prgptr+j;i++)c[i-prgptr]=tokens[i];
    push(c);push_size(j);
    tree();
}

void cons(){
    char* c;
    c=(char*)malloc(sizeof(char));
    c[0]=']';
    push(c);push_size(1);
    int j=0;
    for(int i=prgptr;i<num_tokens;i++){
        if(!isdigit(tokens[i]))break;
        j++;
    }
    c=(char*)malloc(j*sizeof(char));
    for(int i=prgptr;i<prgptr+j;i++)c[i-prgptr]=tokens[i];
    push(c);push_size(j);
    tree();
}

void tree(){
    if(empty())return;
    char* c=pop();
    int siz=pop_size();
    if(c[0]=='_'){
        printf("[%c",c[1]);
        switch(c[1]){
        case 'S':
            start();
            break;
        case 'D':
            declare();
            break;
        case 'F':
            rest();
            break;
        case 'R':
            read();
            break;
        case 'W':
            write();
            break;
        case 'M':
            assign();
            break;
        case 'E':
            exp1();
            break;
        case 'B':
            exp22();
            break;
        case 'A':
            exp3();
            break;
        case 'C':
            exp4();
            break;
        case 'V':
            var();
            break;
        case 'L':
            foor();
            break;
        case 'N':
            cons();
            break;
        }
    }
    else{
        if(c[0]==']')printf("]");
        else{
            if(c[0]!=' ')printf("[");
            for(int i=0;i<siz;i++){
                 if(tokens[prgptr+i]!='.'){
                     if(c[i]!=tokens[prgptr+i]){
                         printf("\nCannot be derived\n");
                         exit(1);
                     }
                  }
                if(c[i]!=' ')printf("%c",c[i]);
            }
            prgptr+=siz;
            if(c[0]!=' ')printf("]");   
        }
        tree();
    }
}

void result(char* filename){
    
    FILE* in_file = fopen(filename, "r");
    FILE* in_fileout = fopen("output.c", "w");

    fprintf(in_fileout,"#include <stdio.h>\n\nint main(){\n");

    char tok[50];
    char c1 = fgetc(in_file);
    int pos=ftell(in_file);
    int skipf=0;
    int extraint=0;
    int ccount=0;
    int icount=0;
    while (c1 != EOF){ 
        if(c1!=' '){
         ccount++;}
        
            //error chech for variable name
        if(extraint>1){
                printf("only one declaration statement allowed\n");
                exit(0);
            }
            if(icount>1){
                printf("declaration should be the first statement\n");
                exit(0);
            }
        if(c1=='i'){
        if(fgetc(in_file)=='n' && fgetc(in_file)=='t'){
            extraint++;
            icount=ccount;
            
            fgetc(in_file);
            while( fgetc(in_file)!=';'){
            fscanf(in_file,"%[^,;\n]s",tok);
            
            if(!(strcmp(tok,"write"))||!(strcmp(tok,"read"))||!(strcmp(tok,"for"))||!(strcmp(tok,"int"))){
                printf("%s ",tok);
                printf("variable name not allowed\n");
                exit(0);
            }
            
            } 
            fseek(in_file,pos,SEEK_SET);
        }
        else{
            fseek(in_file,pos,SEEK_SET);
            }  
        
    }
            
            if(c1=='w'){
            if(fgetc(in_file)=='r' && fgetc(in_file)=='i' && fgetc(in_file)=='t' && fgetc(in_file)=='e'){
                fscanf(in_file,"%s",tok);
                tok[strlen(tok)-1]='\0';
                if(tok[0]=='0'||tok[0]=='1'||tok[0]=='2'||tok[0]=='3'||tok[0]=='4'||tok[0]=='5'||tok[0]=='6'||tok[0]=='7'||tok[0]=='8'||tok[0]=='9'){
                fprintf(in_fileout,"printf(\"%s\")",tok);
                c1=';';
                }
                else{
                    fprintf(in_fileout,"printf(\"%%d\\n\",%s)",tok);
                    c1=';';
                }
            }
            else{
            fseek(in_file,pos,SEEK_SET);
        
            }   
        }
    


    if(c1=='r'){
            if(fgetc(in_file)=='e' && fgetc(in_file)=='a' && fgetc(in_file)=='d'){
                fscanf(in_file,"%s",tok);
                tok[strlen(tok)-1]='\0';
                if(tok[0]=='0'||tok[0]=='1'||tok[0]=='2'||tok[0]=='3'||tok[0]=='4'||tok[0]=='5'||tok[0]=='6'||tok[0]=='7'||tok[0]=='8'||tok[0]=='9'){
                printf("can't read constants\n");
                exit(0);
                }
                else{
                    fprintf(in_fileout,"scanf(\"%%d\",&%s)",tok);
                    c1=';';
                }
            }
            else{
            fseek(in_file,pos,SEEK_SET);
        
            }   
        }
            
            
            
            if(c1=='f'){
            if(fgetc(in_file)=='o' && fgetc(in_file)=='r'){
                skipf=1;
            }
            fseek(in_file,pos,SEEK_SET);
        }
        
            if(skipf!=2){
            fputc(c1, in_fileout);
            if(c1=='}' && skipf==1){
            skipf=2;
            }
            c1 = fgetc(in_file);
            pos=ftell(in_file);
            }
            else{
                if(c1!=';'){
                    printf("missing ; after for\n");
                    exit(0);
                }
            c1 = fgetc(in_file);
            pos=ftell(in_file);
            skipf=0;
            }
            
        }
    
        fprintf(in_fileout,"\nreturn 0;\n}");
        fclose(in_file);
        fclose(in_fileout);
         if(ccount==0){
            printf("empty file not accepted\n");
            exit(0);
         }

        system("gcc output.c && ./a.out ");
        //remove("output.c");
}



