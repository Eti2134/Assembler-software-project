#include "second_pass.h"

/*
 * The function reads the am file and for each line, sends it to the first pass function.
 * If an error is found in the line, the function will call the function to print the errors.
 * At the end of the function, all the labels inserted into the label array in the received rows will be checked.
 * 
 * @param filename - pointer to the name of the .am file from which the first pass is obtained.
 * @return 0 if successful, otherwise 1
 */
	int start_first_pass(char *filename); /* get the an file */


/*
 * The function for each line, checks its content, sends it to appropriate input-validity functions, and checks its input for validity. 
 *
 * @param line - pointer to the line on which the entire first pass will be checked.
 * @param labels_ptr - pointer to an array of labels, into which the labels found in each line will be inserted.
 * @param num_labels - pointer to the counter of labels inserted into the array
 * @return 0 if successful, error code otherwise (31-37)
 */
	int first_pass(char *line, struct LABELS **labels_ptr, int *num_labels);  

/**  
 * The function gets a label name and checks if it is valid.
 * 
 * @param label - the name of the label 
 * @return 0 if successful, otherwise error code (9-14)
 */


	int vaild_label(char *label);

/**
 * Function Description: This function takes in a character array and an integer value as input parameters.
 * The character array represents a string of characters and the integer value represents the index from where the function needs to start searching for a non-space
 * or non-tab character. The function then iterates through the string from the specified index and returns the index of the first non-space 
 * or non-tab character encountered.
 * @param word - A pointer to a character array (string) that needs to be searched for a non-space or non-tab character.
 * @param index - An integer value that represents the starting index from where the function needs to start searching for a non-space or non-tab character.
 * @returnAn integer value that represents the index of the first non-space or non-tab character encountered.
*/

	int func_getspace(char *word, int index);

/*
 * Execute the data instruction and add the numbers to the code word array.
 * 
 * @param str - pointer to the string containing the data instruction
 * @param index - index of the first character of the data instruction in the string
 * @return 0 if successful, error code otherwise (31-37)
 */
	int data_instruction(char *str, int index);

/*
 * Converts a string to its binary representation and adds it to the temporary file with the extension .an.
 *
 * @param name - a pointer to the string to be converted
 * @param index - the index of the beginning of the string in the source code line
 * @return 0 if the string was converted successfully, an error code otherwise (38-41)
*/
	int func_string(char *name, int index);


/* The function sends the row and index to discover each operand in turn. It returns the name of the number or register or zero if it is a label
 *
 * @param line - a pointer to the line where the file is held.
 * @param index - index to point to the position in the line.
 * @param address_case - variable indicating at the beginning of the function the possible addressing methods for the file:
 * 1 - source operand 0, 1 and 3
 * 2 - source operand 1 (lea only)
 * 3 - target operand - 1, 3
 * 4 - target operand 0, 1, 2, 3
 * 5 - Operand 1, 2 and 3
 * At the end of the file, it indicates the addressing method used, or the number -1 if there was an error.
 * 
 * @param pot_labels - Pointer to a potential label that might come in handy.
 * @return In immediate addressing and register addressing, the received number, and in direct addressing or jump addressing 0.
*/
	int find_operand(char *line, int *index, int *case_adress, char *pot_labels);


/*
 *	 This function receives the content of the parameters in the jump addressing method, it first checks the correctness of the brackets and the comma, and then checks the correctness of the parameters.
 * 
 * @param ptr - pointer to the string that holds the brackets
 * @param pot_label1 - pointer to a potential label empty string that will be the first parameter in parentheses.
 * @param pot_label2 - pointer to an empty string with a potential label to be the second parameter in parentheses.
 * @param first_result - the number received in the first parameter or the register number
 * @param second_result - the number received in the second parameter or the register number
 * @param case_adress1 - the addressing method found in the first parameter or minus one with there was an error in the parameter.
 * @param case_adress2 - the addressing method found in the second parameter or minus one with there was an error in the parameter.
 * 
 * @return  0  in succsess otherwith the number of the error detected
 */

	
	int find_parameters(char *ptr, char *pot_label1, char *pot_label2, int *first_result ,int *second_result, int *case_adress1, int *case_adress2);

/*
 * A function that get a line, an index and a case type and checks if it is correct and if not returns minus 1
 *
 * @param line - a pointer to the line where the file is held.
 * @param index - index to point to the position in the line.
 * @param address_case - pointer to the case of the function
 * @return the number found or the number of erorr
*/	

	int Address1_fixed_number(char *line, int *index, int *address_case);

	

	


