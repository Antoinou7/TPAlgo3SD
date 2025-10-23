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
	
	if(*curpos == ' ' || *curpos == '\n') 
	{
		curpos++;
	}
	
	if (*curpos == '\0') break;

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

	if(nbValNum > 0){

		Token* token = create_token_from_string(curpos, nbValNum);
		queue_push(Queuefinal, token);
		curpos += nbValNum;
	}
	else{
		curpos++;
	}
	}
return Queuefinal; 
}



Queue* shuntingYard(Queue* infix)
{
Queue* postfix = create_queue();
Stack* operator = create_stack(queue_size(infix));

while(!queue_empty(infix))
{
	Token* token =(Token*) queue_top(infix);
	if(token_is_number(token))
	{
		queue_push(postfix, token);
	}
	else if (token_is_operator(token))
	{
		while(!stack_empty(operator) &&
		token_is_operator(stack_top(operator)) &&
	(token_operator_priority(stack_top(operator)) > token_operator_priority(token) ||
	(token_operator_priority(stack_top(operator)) == token_operator_priority(token) && token_operator_leftAssociative(token))) &&
	(token_is_parenthesis(stack_top(operator)) ? token_parenthesis((stack_top(operator))) != '(': true))
	{
		queue_push(postfix, stack_top(operator));
		stack_pop(operator);
	}
	stack_push(operator, token);
	}
	else if(token_is_parenthesis(token) && token_parenthesis(token) == '(')
	{
	stack_push(operator, token);
	}
	else if(token_is_parenthesis(token) && token_parenthesis(token) == ')')
	{
		while(!stack_empty(operator) && token_is_parenthesis(stack_top(operator)) ?
		token_parenthesis((stack_top(operator))) != '(' : true)
		{
			queue_push(postfix, stack_top(operator));
			stack_pop(operator);
		}
		if (!stack_empty(operator)){  
                 Token* tok = (Token*)stack_top(operator);
                 stack_pop(operator);
                 delete_token(&tok);
             }
			 delete_token(&token);
	}
	queue_pop(infix);
}

if(queue_size(infix)==0)
{
	while(!stack_empty(operator))
	{
		queue_push(postfix, stack_top(operator));
		stack_pop(operator);
	}
}
	delete_stack(&operator);
	return postfix;
}

Token* evaluateOperator(Token* arg1, Token* op, Token* arg2)
{
	if (token_is_number(arg1) && token_is_operator(op) && token_is_number(arg2))
	{
		float value1 = token_value(arg1);
		float value2 = token_value(arg2);
		char operator = token_operator(op);
		float result = 0;
		if (operator == '+')
		{
			result = value1 + value2;
		}
		else if (operator == '-')
		{
			result = value1 - value2;
		}
		else if (operator == '*')
		{
			result = value1 * value2;
		}
		else if (operator == '/')
		{
			result = value1 / value2;
		}
		else if (operator == '^')
		{
			result = pow(value1, value2);
		}
		else
		{
			fprintf(stderr, "%c isn't a supported operator. The only supported are + - / * ^\n", operator);
			delete_token(&arg1);
			delete_token(&arg2);
			delete_token(&op);
			exit(1);
		}
		delete_token(&arg1);
		delete_token(&arg2);
		delete_token(&op);
		return create_token_from_value(result);
	}
	else 
	{
		fprintf(stderr,"Error, one of the three arguments is a wrong type\n");
		exit(2);

	}

}

float evaluateExpression(Queue* postfix)
{
	Stack* operand = create_stack(queue_size(postfix)); // create stack to store operands
	Token* evaluatedOperation;

	while(!queue_empty(postfix))
	{
		if(!stack_empty(operand) && token_is_operator(queue_top(postfix)))
		{
			Token* operand2 = (Token*)stack_top(operand);
			stack_pop(operand);
			if(!stack_empty(operand)) // After stack_pop, the stack can be empty and we want to avoid that so we check if the stack is empty
			{
			Token* operand1 = (Token*)stack_top(operand);
			stack_pop(operand);
			Token* operator = (Token*)queue_top(postfix);
			evaluatedOperation = evaluateOperator(operand1, operator, operand2);
			stack_push(operand, evaluatedOperation); // push result back onto the stack  ***
			}
		}
		else if (!token_is_operator(queue_top(postfix)))  // else if token is an operand
		{
			stack_push(operand, queue_top(postfix)); // push token onto the stack
		}
		queue_pop(postfix); // remove token from the queue
	}
	float result;
	if(!stack_empty(operand) && token_is_number(stack_top(operand))) // if stack is not empty and top of stack is a number
	{
		result = token_value(stack_top(operand)); // Result is a Token that we just pushed in the stack (cf.***), we now want to return a float value
	}
	else
	{
		delete_stack(&operand);
		delete_token(&evaluatedOperation);
		fprintf(stderr, "Error, the stack is empty or the top of the stack is not a number\n");
		exit(3);
	}
	stack_pop(operand);
	delete_stack(&operand);
	delete_token(&evaluatedOperation);
	return result;

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
			if (buffer[strlen(buffer)-1] != '\n') putchar('\n'); 	// if the last character is not a \n, we add one
			
			Queue* tokenQueue = stringToTokenQueue(buffer);
			printf("Infix : ");
			print_queue(stdout, tokenQueue);
			printf("\n");
			
			Queue* postfix = shuntingYard(tokenQueue);
			if(postfix != NULL)
			{
			printf("Postfix : ");
			print_queue(stdout, postfix);
			printf("\n");
			float result = evaluateExpression(postfix);
			printf("Evaluate : %6f\n", result);

			//free the postfix
		while(!queue_empty(postfix))
		{
			Token* p = (Token*)queue_top(postfix);
			delete_token(&p);
			queue_pop(postfix);
		}
		delete_queue(&postfix);
			}
		

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

	free(buffer); //free the memory allocated by the function "getline"
	

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