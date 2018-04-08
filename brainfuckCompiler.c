#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

void finalizeProgram(FILE ** fileProgram);
void initializeProgram(FILE ** fileProgram);
void translateBrainfuck(char * sourcePointer);
int checkIfBracketsMatch(char * sourcePointer);

int main(int argc, char *argv[]){
	FILE *bfSource = NULL;
	char *sourcePointer = NULL;	

	//get the filename from the arguments
	if(argc == 1){
		printf("missing source file\n");
		return -1;
	}
	if(argc > 2){
		printf("too many arguments\n");
		return -1;
	}
	char * programFilename = argv[1];
	
	//read the file into memory
	bfSource = fopen(programFilename, "rb");
	if(bfSource != NULL){
		struct stat st;
		stat(programFilename,&st);
		size_t sourceSize = st.st_size; 
		sourcePointer = (char*)calloc(sourceSize+1, 1);
		fread(sourcePointer, 1, sourceSize, bfSource);
		//memcpy(sourcePointer, bfSource, sourceSize);
		if(checkIfBracketsMatch(sourcePointer)){
			translateBrainfuck(sourcePointer);
		}
		else
			printf("loops don\'t match\n");
		fclose(bfSource);
	}
	else
		printf("error opening source file\n");

	//
}

int checkIfBracketsMatch(char * sourcePointer){
	int openBrackets = 0;
	int closeBrackets = 0;
	while(*sourcePointer != '\0'){
		if(*sourcePointer == '[')
			++openBrackets;
		if(*sourcePointer == ']')
			++closeBrackets;
		
		if(closeBrackets > openBrackets)
			return 0;
		sourcePointer++;
	}
	if(openBrackets != closeBrackets)
		return 0;
	return 1;
}

void translateBrainfuck(char * sourcePointer){
	FILE * bfProgram = NULL;
	bfProgram = fopen("brainfuckProgram.c","w");
	if(bfProgram != NULL){
		initializeProgram(&bfProgram);
		size_t s_len = 0;
		char * plus = "++(*p);\n";
		char * minus = "--(*p);\n";
		char * right = "++p;\n";
		char * left = "--p;\n";
		char * start = "while(*p){\n";
		char * end = "}\n";
		char * write = "putchar((int)*p);\n";
		char * read = "*p = (char)getchar();\n";
		printf("%c\n",*sourcePointer);
		while(*sourcePointer != '\0'){
			printf("we tryna convert\n");
			switch(*sourcePointer){
				case '+':
					s_len = strlen(plus);
					fwrite(plus, 1, s_len, bfProgram);	
					break;
				case '-':
					s_len = strlen(minus);
					fwrite(minus, 1, s_len, bfProgram);	
					break;
				case '>':
					s_len = strlen(right);
					fwrite(right, 1, s_len, bfProgram);	
					break;
				case '<':
					s_len = strlen(left);
					fwrite(left, 1, s_len, bfProgram);	
					break;
				case '[':
					s_len = strlen(start);
					fwrite(start, 1, s_len, bfProgram);	
					break;
				case ']':
					s_len = strlen(end);
					fwrite(end, 1, s_len, bfProgram);	
					break;
				case '.':
					s_len = strlen(write);
					fwrite(write, 1, s_len, bfProgram);	
					break;
				case ',':
					s_len = strlen(read);
					fwrite(read, 1, s_len, bfProgram);	
					break;
				default:
					printf("non-brainfuck symbol\n");	
			}				
			sourcePointer++;
		}
		finalizeProgram(&bfProgram);	
	}
	else
		printf("error opening program file\n");
}

void initializeProgram(FILE ** fileProgram){
	char * s = "#include <stdio.h>\n#include <stdlib.h>\nint main(){\nchar* p = (char*)calloc(10000,1);\n";
	size_t s_len = strlen(s);
	fwrite(s, 1, s_len, *fileProgram);
}

void finalizeProgram(FILE ** fileProgram){
	char * s = "}\n";
	size_t s_len = strlen(s);
	fwrite(s, 1, s_len, *fileProgram);
}
