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

//for error exit code
#include <stdlib.h>
#define MAX 100

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

// pushing and poppinga double functions
void pushDbl(double *stack, int *top, double val){
	stack[++(*top)] = val;
}

double popDbl(double *stack, int *top){
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

	//flag to track prev char (if operator or left parenthesis)
	int prevOperatorFlag = 1;

	/*while index i does not equal null,
        while loop traverses through each char in expression*/
	while(expr[i] != '\0'){
		//if index i is a digit or a decimal
		if(isdigit(expr[i]) || expr[i] == '.'){
			//use while loop to traverse from start to end of number
			while(isdigit(expr[i]) || expr[i] == '.'){
				rpn[rpnIndex++] = expr[i++];
			}
			//while loop ends, means end of number
			//add spaces between each number
			rpn[rpnIndex++] = ' ';
			//previous char was not operator or left paren - set flag to 0
			prevOperatorFlag = 0;
		}
		//if char == open parenthesis then push to opStack to override precedence
		else if(expr[i] == '('){
			push(opStack, &opTop, '(');
			//i++, go to next char
			i++;
			//prev char was left paren
			prevOperatorFlag = 1;
		}
		//if char == closing parenthesis ,
		else if(expr[i] == ')'){
			while(opTop >= 0 && opStack[opTop] != '('){
				rpn[rpnIndex++] = pop(opStack, &opTop);
				rpn[rpnIndex++] = ' ';
			}
			pop(opStack, &opTop);
			i++;
			//set op flag back to zero
			prevOperatorFlag = 0;
		}
		else if(isOperator(expr[i])){
			//used to check between unary and binary
			if(expr[i] == '-' && (prevOperatorFlag || expr[i+1] == '-' || expr[i+1] == '+')){
				rpn[rpnIndex++] = 'n';
				rpn[rpnIndex++] = ' ';
				i++;
			}else{
				// reg binary operators
				while(opTop >= 0 && precedence(opStack[opTop]) >= precedence(expr[i])){
					rpn[rpnIndex++] = pop(opStack, &opTop);
					rpn[rpnIndex++] = ' ';
				}
				push(opStack, &opTop, expr[i]);
				i++;
			}
			//prev char was an operator
			prevOperatorFlag = 1;
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
			prevOperatorFlag = 0;
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


//evalRPN - rpn evaluation function
double evalRPN(char *rpn){
	//create stack
	double stack[MAX];
	int top = -1;
	int i = 0;

	//while loop, while index in rpn array is not null
	while(rpn[i] != '\0'){
		//if index 'i' in rpn array is a digit or a decimal
		if(isdigit(rpn[i]) || rpn[i] == '.'){
			//double num = 0
			double num = 0.0;
			//int decimal flag
			int decFlag = 0;
			//dbl decimal div
			double decDiv = 1;

			//while 'i' is a digit, or a decima, and no dec flag
			while(isdigit(rpn[i]) || (rpn[i] == '.' && !decFlag)){
				//if i is a decimal, then decimal flag is set
				if(rpn[i] == '.'){
					decFlag = 1;
				}else{
					//build decimal number by multiplication
					num = num * 10 + (rpn[i] - '0');
					if(decFlag){
						//keep track of how many digits are after decimal point
						decDiv *= 10;
					}
				}
				i++;
			}
			//adjust decimal point by dividing number of dec places
			num /= decDiv;
			//push number onto stack
			pushDbl(stack, &top, num);
		}
		//if current char is an operator
		else if(isOperator(rpn[i])){
			//pop  first and sec operand to perform operation
			double second = popDbl(stack, &top);
			double first = popDbl(stack, &top);
			//apply operator
			double result = useOperator(rpn[i], first, second);
			//push result of operation to stack
			pushDbl(stack, &top, result);
			//move to next char in array
			i++;
		}

		//if array has unary marker set
		else if(rpn[i] == 'n'){
			//negate top of stack for unary negation
			double num = popDbl(stack, &top);
			//apply negation
			pushDbl(stack, &top, -num);
			i++;
		}

		//if char equals 'f' (function marker) then there is afunction next
		else if(rpn[i] == 'f'){

			//increment to next char (function)
			i++;
			//function array name
			char func[10];
			int j = 0;

			//while loop to get func name
			while(isalpha(rpn[i])){
				func[j++] = rpn[i++];
			}

			//set function array to null
			func[j] = '\0';
			//pop number for func from stack
			double op = popDbl(stack, &top);
			//function result
			double result = useFunction(func, op);
			//push result to stack
			pushDbl(stack, &top, result);
		}
		else{
			//if char is not digit, operator, or func then go to next char
			i++;
		}
	}
	//return final result
	return popDbl(stack, &top);
}

//checking valid character inputs
int isValidChar(char c){
	return(isdigit(c) || isalpha(c) || isOperator(c) || c == '.' || c == '(' || c == ')');
}

//checking if valid expression
void validateExpr(char *expr){
	//keeping track of parenthesis balance
	int balance = 0;

	//loop through expression input
	for(int i = 0; expr[i] != '\0'; i++) {
        	if (!isValidChar(expr[i])) {
            		printf("Error: Invalid character '%c' in expression.\n", expr[i]);
            		//exit if error with input
			exit(1);
        	}
		if(expr[i] == '('){
			balance++;
		}
		else if(expr[i] == ')'){
			balance--;
			if(balance < 0){
				printf("Error: Unmatched closing parenthesis.\n");
				exit(1);
			}
		}
    	}
	if(balance != 0){
		printf("Error: Mismatched parenthesis");
		exit(1);
	}
}

//main function
int main(){
	char expr[MAX];
   	char rpn[MAX];
	
	while(1){

		printf("Enter an expression (or type 'exit' to quit): ");
		fgets(expr, MAX, stdin);
		expr[strcspn(expr, "\n")] = '\0';
		
		if(strcmp(expr, "exit") == 0){
			//exit while loop
			break;
		}

		validateExpr(expr);

		infixToRPN(expr, rpn);
		double result = evalRPN(rpn);

		printf("Result: %lf\n", result);
	}

	printf("Program ended.\n");
	return 0;
}



