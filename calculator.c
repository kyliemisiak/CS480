// File: CS 480 Lab 2 Calculator
// Author: Kylianna Misiak
// Source code file

/*  Calculator application with ability to do basic arithmetic/trig/
logarithmic operations w/ real numbers and "()" and "{}"
*/

// No external software used besides Math library
#include <stdio.h>
#include <math.h>
//for strcmp in useFunction
#include <string.h>

//for isalpha in infixToRPN
#include <ctype.h>

#define MAX 100

// "-" used as unary and binary operator

// Software should check for correct inputs

// Evaluate properly based on precedence for operators
int precedence(char op) {
	switch(op){
		case '+': return 1;
		case '-': return 1;
		case '*': return 2;
		case '/': return 2;
		case '^': return 3;
		//not an operator, return zero precedence
		default: return 0;
	}
}

//function to check if char in expression is operator
int isOperator(char c){
	return(c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

double useOperator(char op, double first, double second){
	//use switch case to switch between operators
	switch(op){
		case '+': return first + second;
                case '-': return first - second;
                case '*': return first * second;
                case '/': return first / second;
                case '^': return pow(first, second);
		default: return 0.0;
	}
}

//function to check if string is func
int isFunction(char *func){
	return (strcmp(func, "sin") == 0|| strcmp(func, "cos") == 0 || strcmp(func, "tan") == 0 || strcmp(func, "cot") == 0 || strcmp(func, "ln") == 0 || strcmp(func, "log10") == 0);
}

double useFunction(char *func, double arg){
	if(strcmp(func, "sin") == 0) return sin(arg);
	if(strcmp(func, "cos") == 0) return cos(arg);
	if(strcmp(func, "tan") == 0) return tan(arg);
	if(strcmp(func, "cot") == 0) return 1.0 / tan(arg);
	if(strcmp(func, "ln") == 0) return log(arg);
	if(strcmp(func, "log10") == 0) return log10(arg);
	else return 0.0;
}

//create push and pop functions to use for stacks so don't have to repeat code
void push(char *stack, int *top, char val){
	stack[++(*top)] = val;
}

char pop(char *stack, int *top){
	return stack[(*top)--];
}



/* Mathematic expression evaluation implemented in code, do not use eval
 or internal eval functions
*/

//using the shunting yard algorithm (converting infix expression to RPN)
void infixToRPN(char *expr, char*rpn){
	char opStack[MAX];
	int opTop = -1;
	int rpnIndex = 0;
	int i = 0;
	
	/*while index i does not equal null,
        while loop traverses through each char in expression*/
	while(expr[i] != '\0'){
		//if index i is a digit or a decimal
		if(isdigit(expr[i]) || expr[i] == '.'){
			//use while loop to traverse from start to end of number
			while(isdigit(expr[i]) || expr[i] =='.'){
				rpn[rpnIndex++] = expr[i++];
			}
			//while loop ends, means end of number
			//add spaces between each number
			rpn[rpnIndex++] = ' ';
		}
		//if char == open parenthesis then push to opStack to override precedence
		else if(expr[i] == '('){
			push(opStack, &opTop, '(');
			//i++, go to next char
			i++;
		}
		//if char == closing parenthesis , 
		else if(expr[i] == ')'){
			while(opTop >= 0 && opStack[opTop] != '('){
				rpn[rpnIndex++] = opStack[opTop--];
				rpn[rpnIndex++] = ' ';
			}
			pop(opStack, &opTop);
			i++;
		}
		else if(isOperator(expr[i])){
			while(opTop >= 0 && precedence(opStack[opTop]) >= precedence(expr[i])){
				rpn[rpnIndex++] = opStack[opTop--];
				rpn[rpnIndex++] = ' ';
			}
			push(opStack, &opTop, expr[i]);
			i++;
		}
		//checks if char at i is a letter
		//if char is a letter then apply function
		else if(isalpha(expr[i])){
			char func[10];
			int  j = 0;
			while(isalpha(expr[i])){
				func[j++] = expr[i++];
			}
			func[j] = '\0';
			
			if(isFunction(func)){
				//marker 'f' for function
				rpn[rpnIndex++] = 'f';
				//add space btwn marker and function name
				rpn[rpnIndex++] = ' ';
				for(int k = 0; func[k] != '\0'; k++){
					rpn[rpnIndex++] = func[k];
				}
				rpn[rpnIndex++] = ' ';
			}
		}
		else{
			i++;
		}
	}
	while(opTop >= 0){
		rpn[rpnIndex++] = pop(opStack, &opTop);
		rpn[rpnIndex++] = ' ';
	}
	
	//set rpn expression to null 
	rpn[rpnIndex] = '\0';
}



//main function
int main(){
	char expr[MAX];
   	char rpn[MAX];
    
   	 // Test Case 1
    	strcpy(expr, "3 + 4");
    	infixToRPN(expr, rpn);
    	printf("Infix: %s\nPostfix: %s\n\n", expr, rpn);
   	 // Test Case 2
   	strcpy(expr, "3 * 4");
        infixToRPN(expr, rpn);
	printf("Infix: %s\nPostfix: %s\n\n", expr, rpn);

	// Test Case 3
    strcpy(expr, "(3 + 4) * 2");
    infixToRPN(expr, rpn);
    printf("Infix: %s\nPostfix: %s\n\n", expr, rpn); 
    // Test Case 4:
    strcpy(expr, "3 + 4 * 2");
    infixToRPN(expr, rpn);
    printf("Infix: %s\nPostfix: %s\n\n", expr, rpn);
    // Test Case 5
    strcpy(expr, "(3 + 4) * (5 - 2)");
    infixToRPN(expr, rpn);
    printf("Infix: %s\nPostfix: %s\n\n", expr, rpn); 
    // Test Case 6 
	printf("Postfix after function: %s\n", rpn);    
strcpy(expr, "sin(30 + 2)");
	printf("Postfix after function: %s\n", rpn);
    infixToRPN(expr, rpn);
    printf("Infix: %s\nPostfix: %s\n\n", expr, rpn); 
  
    // Test Case 7
    strcpy(expr, "((3 + 4) * 2) + 5");
    infixToRPN(expr, rpn);
    printf("Infix: %s\nPostfix: %s\n\n", expr, rpn);	
	//printf("Enter an expression: ");
	//fgets(expr, MAX, stdin);
	//expr[strcspn(expr, "\n")] = '\0';



	return 0;
}
