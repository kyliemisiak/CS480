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

// "-" used as unary and binary operator

/* Mathematic expression evaluation implemented in code, do not use eval
 or internal eval functions
*/

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

double useOperator(char op, double first, double second){
	//use switch case to switch between operators
	switch(op){
		case '+': return first + second;
                case '-': return first - second;
                case '*': return first * second;
                case '/': return first / second;
                case '^': return pow(first, second);
		default: printf("Error: Invalid Operator '%c'\n", op);
	}
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


//main function
int main(){
	return 0;
}
