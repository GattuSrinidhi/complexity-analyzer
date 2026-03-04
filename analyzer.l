%{
#include <stdio.h>
#include <string.h>

int loops = 0;
int decisions = 0;
int depth = 0;
int max_depth = 0;

int function_count = 0;
int recursion_detected = 0;

char current_function[100];

extern FILE *yyin;

void analyze_line(char *line){

    if(strstr(line,"for(") || strstr(line,"while("))
    {
        loops++;
        depth++;
        if(depth > max_depth)
            max_depth = depth;
    }

    if(strstr(line,"if(") || strstr(line,"else") || strstr(line,"switch("))
        decisions++;

    if(strstr(line,"}"))
    {
        if(depth>0)
            depth--;
    }

    if(strstr(line,"(") && strstr(line,")") && strstr(line,"{"))
    {
        char name[100];
        sscanf(line,"%[^ (]",name);

        if(strcmp(name,"if")!=0 &&
           strcmp(name,"for")!=0 &&
           strcmp(name,"while")!=0 &&
           strcmp(name,"switch")!=0)
        {
            function_count++;
            strcpy(current_function,name);
        }
    }

    if(strstr(line,current_function) && strstr(line,"(") && strstr(line,")") && !strstr(line,"{"))
        recursion_detected = 1;
}
%}

%%

.*\n   { analyze_line(yytext); }

.      ;

%%

int yywrap(){
    return 1;
}

int main(int argc,char **argv){

    if(argc<2){
        printf("Usage: analyzer <file.c>\n");
        return 1;
    }

    FILE *file=fopen(argv[1],"r");

    if(!file){
        printf("Cannot open file\n");
        return 1;
    }

    yyin=file;

    yylex();

    char complexity[20];

    if(max_depth==0)
        strcpy(complexity,"O(1)");
    else if(max_depth==1)
        strcpy(complexity,"O(n)");
    else if(max_depth==2)
        strcpy(complexity,"O(n^2)");
    else
        strcpy(complexity,"O(n^k)");

FILE *out = fopen("result.json","w");

fprintf(out,"{\n");
fprintf(out,"  \"functions\": %d,\n",function_count);
fprintf(out,"  \"recursion\": %d,\n",recursion_detected);
fprintf(out,"  \"loops\": %d,\n",loops);
fprintf(out,"  \"decisions\": %d,\n",decisions);
fprintf(out,"  \"max_depth\": %d,\n",max_depth);
fprintf(out,"  \"complexity\": \"%s\"\n",complexity);
fprintf(out,"}\n");

fclose(out);

printf("Analysis complete. Results saved to result.json\n");

    return 0;
}