// Need this to use the getline C function on Linux. Works without this on MacOs. Not tested on Windows.
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "token.h"
#include "queue.h"
#include "stack.h"


/** 
 * Utilities function to print the token queues
 */
void print_token(const void* e, void* user_param);
void print_queue(FILE* f, Queue* q);


bool isSymbol(char c){
	return c=='+' || c=='-' || c=='*' || c=='/' || c=='^' || c=='(' || c==')';
}

Queue* stringToTokenQueue(const char* expression)
{

Queue* Queuefinal = create_queue();
const char* curpos = expression;

while(*curpos != '\0'){
	while(*curpos == ' ' || *curpos == '\n') 
	{
		curpos++;
	}
	int nbValNum =0;
	if(isSymbol(*curpos))
	{
		nbValNum = 1;
	}	
	else{
		while (curpos[nbValNum] != '\0' && !isSymbol(curpos[nbValNum]) && curpos[nbValNum]!=' ' && curpos[nbValNum] != '\n')
		{
			nbValNum++;
		}
	}
		Token* token = create_token_from_string(curpos, nbValNum);
		queue_push(Queuefinal, token);
		curpos += nbValNum;

	}
return Queuefinal; 
}











/** 
 * Function to be written by students
 */
void computeExpressions(FILE* input) {
	char* buffer = NULL; // SET buffer to NULL so that getline can allocate memory on its own
	size_t size = 0; // SET size to 0 so that getline can allocate memory on its own
	
	while(!feof(input)) // while we are not at the end of the file
	{
		if(getline(&buffer, &size, input) > 1 ) // getLine return the number of characters read, we do not want to read empty lines, so we check if the number of characters read is greater than 1
		{
			printf("\nInput : %s", buffer);
			Queue* tokenQueue = stringToTokenQueue(buffer);
			printf("Infix : ");
			print_queue(stdout, tokenQueue);
			printf("\n");
		

		//free the tokenQueue
		while(!queue_empty(tokenQueue))
		{
			Token* t = (Token*)queue_top(tokenQueue);
			delete_token(&t);
			queue_pop(tokenQueue);
		}
		delete_queue(&tokenQueue);

	}
	}

	free(buffer); //free the memory allocaded by the function "getline"
	
	


}







/** Main function for testing.
 * The main function expects one parameter that is the file where expressions to translate are
 * to be read.
 *
 * This file must contain a valid expression on each line
 *
 */
int main(int argc, char** argv){
	if (argc<2) {
		fprintf(stderr,"usage : %s filename\n", argv[0]);
		return 1;
	}
	
	FILE* input = fopen(argv[1], "r");

	if ( !input ) {
		perror(argv[1]);
		return 1;
	}

	computeExpressions(input);

	fclose(input);
	return 0;
}
 
void print_token(const void* e, void* user_param) {
	FILE* f = (FILE*)user_param;
	Token* t = (Token*)e;
	token_dump(f, t);
}

void print_queue(FILE* f, Queue* q) {
	fprintf(f, "(%d) --  ", queue_size(q));
	queue_map(q, print_token, f);
}