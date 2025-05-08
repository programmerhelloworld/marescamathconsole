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
#include <map>
//#include "AudioPlayer.h"
using namespace std;

#ifndef MARESCA_H
#define MARESCA_H

using INTEGER = int;
using DECIMAL = double;

/*
const char *error_audio = "assets/error1.mp3";
AudioPlayer* ap_err_aux = AudioPlayer::file(error_audio);
*/

map<string, DECIMAL> variables;
map<string, INTEGER> variables2;

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

void error_sound(){
  //  ap_err_aux->play();
}
// From string to decimal
void string_to_decimal(string still_string, DECIMAL *value){
	*value = atof(still_string.c_str());
}

void help(){
    display_console("\nCOMMANDS LIST: \n");
    display_console("eval (expression) - gives the result of an expression\n");
    display_console("cube - displays a 3d cube ;D\n");
    display_console("sqrt (number) - gives square root of a number\n");
    display_console("system.call(terminal/cmd prompt command) - system command (could harm pc if malicous command)\n");
    display_console("exit | quit - closes the program\n\n");
}

void decimal_to_string(DECIMAL number, string *output){
    ostringstream oss;
    oss << number;
    *output = oss.str();
}


void cube(){
    display_console("\n\n   -----------------\n");   
    display_console("  /                / |\n");
    display_console(" /                /  |\n");
    display_console("/                /   |\n");
    display_console("-----------------    |\n");    
    display_console("|                |   |\n");
    display_console("|                |   /\n");
    display_console("|                |  /\n");
    display_console("|                | /\n");
    display_console("----------------- /\n");




}

void info(){
    display_console("-------------------------------------------\n");

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

        // Decimal variable definition (e.g. DECIMAL pi = 3.14;)
    else if ((input.substr(0, 8) == "DECIMAL ") && input[input.length() - 1] == ';') {
        string statement = input.substr(8, input.length() - 9); // remove 'decimal ' and final ';'
        size_t equal_pos = statement.find('=');
        if (equal_pos != string::npos) {
            string var_name = statement.substr(0, equal_pos);
            string var_value = statement.substr(equal_pos + 1);
/*
            // Trim whitespaces
            var_name.erase(remove_if(var_name.begin(), var_name.end(), ::isspace), var_name.end());
            var_value.erase(remove_if(var_value.begin(), var_value.end(), ::isspace), var_value.end());
*/
            DECIMAL val;
            string_to_decimal(var_value, &val);
            variables[var_name] = val;
            *output = "Decimal variable '" + var_name + "' set to " + var_value;
        } else {
            *output = "Error - invalid decimal variable declaration.";
        }
    }

        // INTEGER variable definition (e.g. INTEGER x = 5;)
    else if ((input.substr(0, 8) == "INTEGER ") && input[input.length() - 1] == ';') {
        string statement = input.substr(8, input.length() - 9); // remove 'INTEGER ' and final ';'
        size_t equal_pos = statement.find('=');
        if (equal_pos != string::npos) {
            string var_name = statement.substr(0, equal_pos);
            string var_value = statement.substr(equal_pos + 1);
/*
            // Trim whitespaces
            var_name.erase(remove_if(var_name.begin(), var_name.end(), ::isspace), var_name.end());
            var_value.erase(remove_if(var_value.begin(), var_value.end(), ::isspace), var_value.end());
*/
            DECIMAL val;
            string_to_decimal(var_value, &val);
            variables2[var_name] = val;
            *output = "Integer variable '" + var_name + "' set to " + var_value;
        } else {
            *output = "Error - invalid INTEGER variable declaration.";
        }
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

    else if (input == "help;"){
        help();
    }
    else if (input == "cube;"){
        cube();
    }

    // Semicolon check
    else if (input.substr(input.length()-1,1) != ";"){
    	*output = "Error - no semicolon!";
    } else {
        *output = "Command not recognized";
    }

    return 0;
}


#endif