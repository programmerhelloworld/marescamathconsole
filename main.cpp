/*
 *
 * Francesco Maresca's Maresca Math Console 
 * "main.cpp"
 *
 */
#include "marescaconsole.h"


int main(){
	string input, output; 
	//DECIMAL value;

	info();

while (1 == 1)
{

	display_console(">> ");
	input_console(&input);
	input_analysis(input, &output);
	if (output == "QUIT_PROGRAM=TRUE;"){
		return 1; // Requested quit flag
	} else {
		display_console(output);
		display_console("\n\n");
		clear_output(&output);
	}
	
}



}