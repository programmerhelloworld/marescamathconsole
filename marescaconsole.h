/*
 *
 * Francesco Maresca's Maresca Math Console 
 * "marescaconsole.h"
 *
 */
#include <iostream>
#include <cstdio>
#include <sstream>
#include <stdlib.h> 
//#include <cstring> - Not needed for now (It may be useful in future)
#include <stack>
#include <cctype>
#include <cmath>
using namespace std;

#ifndef MARESCA_H
#define MARESCA_H

using INTEGER = int;
using DECIMAL = double;


// Output
void display_console(string content){
	cout << content;
}

// Input
void input_console(string *input){
	cin >> *input;
}

// Clearing output
void clear_output(string *output){
	*output = "";
}

// From string to decimal
void string_to_decimal(string still_string, DECIMAL *value){
	*value = atof(still_string.c_str());
}

void decimal_to_string(DECIMAL number, string *output){
    ostringstream oss;
    oss << number;
    *output = oss.str();
}


// Evaluate expression section of functions (Marescalc originated)
bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

// Function to apply arithmetic operations
DECIMAL applyOperation(DECIMAL a, DECIMAL b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        default: return 0;
    }
}

// Function to handle precedence of operators
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

void eval(string expression, DECIMAL *result){
	
	// Expression evalutation function from Marescalc
	std::stack<DECIMAL> values;  // Stack for numbers
    std::stack<char> ops;       // Stack for operators

    bool isNegative = false;    // Flag to handle negative numbers
    DECIMAL decimalPlace = 0.1;  // Decimal place tracker for floating-point numbers

    for (int i = 0; i < expression.length(); i++) {
        char c = expression[i];

        // Skip whitespace
        if (isspace(c)) continue;

        // Handle negative numbers
        if (c == '-' && (i == 0 || expression[i - 1] == '(' || expression[i - 1] == '+' || expression[i - 1] == '-' || expression[i - 1] == '*' || expression[i - 1] == '/')) {
            isNegative = true;
            continue;  // Skip the current negative sign, it will be handled later
        }

        // If current character is a number or a decimal point
        if (isDigit(c) || c == '.') {
            DECIMAL value = 0.0;

            // If it's a digit, build the integer part
            while (i < expression.length() && isDigit(expression[i])) {
                value = value * 10 + (expression[i] - '0');
                i++;
            }

            // If a decimal point is found, handle fractional part
            if (i < expression.length() && expression[i] == '.') {
                i++;  // Skip the decimal point
                DECIMAL decimalFactor = 1;

                while (i < expression.length() && isDigit(expression[i])) {
                    decimalFactor *= 10;
                    value += (expression[i] - '0') / decimalFactor;
                    i++;
                }
            }

            // Adjust for negative numbers
            if (isNegative) {
                value = -value;
                isNegative = false;
            }

            // Push the number onto the values stack
            values.push(value);

            i--; // Adjust for the extra increment inside the loop
        }
        // If current character is '('
        else if (c == '(') {
            ops.push(c);
        }
        // If current character is ')'
        else if (c == ')') {
            while (!ops.empty() && ops.top() != '(') {
                DECIMAL val2 = values.top(); values.pop();
                DECIMAL val1 = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOperation(val1, val2, op));
            }
            ops.pop(); // Pop '(' from stack
        }
        // If current character is an operator
        else if (c == '+' || c == '-' || c == '*' || c == '/') {
            while (!ops.empty() && precedence(ops.top()) >= precedence(c)) {
                DECIMAL val2 = values.top(); values.pop();
                DECIMAL val1 = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOperation(val1, val2, op));
            }
            ops.push(c);
        }
    }

    // Apply remaining operators
    while (!ops.empty()) {
        DECIMAL val2 = values.top(); values.pop();
        DECIMAL val1 = values.top(); values.pop();
        char op = ops.top(); ops.pop();
        values.push(applyOperation(val1, val2, op));
    }

    *result = values.top();
}

int input_analysis(string input, string *output){
	// Sqrt
    if ((input.substr(0, 5) == "sqrt(") && (input[input.length()-2] == ')') && (input[input.length()-1] == ';')) 
    {
        string value = input.substr(5, input.length() - 6);
        DECIMAL number;
        string_to_decimal(value, &number);
        number = sqrt(number); 
        decimal_to_string(number, output); // Decimal put as output 
        //display_console(*output + "\n");
    }  

	// System
	else if (input.substr(0, 12) == "system.call(" && input.substr(input.length() - 2, 2) == ");")
	{
	    string command = input.substr(12, input.length() - 14); // extract inside ( ... )
	    
	    if (command.empty()) {
	        display_console("Error - no command provided inside system.call().\n");
	    } else {
	        display_console("WARNING: This function may harm your system if you write malicious commands!!!!!\nAre you sure you want to continue? 'yes' to validate\n>>");
	        string ans;
	        input_console(&ans);
	        if (ans == "Yes" || ans == "yes" || ans == "yEs" || ans == "yeS" || ans == "YEs" || ans == "yES" || ans == "YES" || ans == "YeS"){
	            system(command.c_str());
	        } else {
	            display_console("No command inserted.\n");
	        }
	    }
	}
	else if (input.substr(0, 12) == "system.call(")
	{
	    display_console("Error - missing closing ');'\n");
	}

	// Eval
	else if ((input.substr(0, 5) == "eval(") && (input.substr(input.length()-2, 2) == ");")){
	    string expr = input.substr(5, input.length()-7); 
	    double result;
	    eval(expr, &result);
	    decimal_to_string(result, output);
	}


    // Quit
    else if ((input == "quit;") || (input == "exit;"))
    {
    	display_console("\n\nQuitting program!\n");
    	*output = "QUIT_PROGRAM=TRUE;";
    }

    // Semicolon check
    else if (input.substr(input.length()-1,1) != ";"){
    	*output = "Error - no semicolon!";
    }

    return 0;
}


#endif