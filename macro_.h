#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LINE 81
#define MAX_MACRO 76
#define LABEL_LENGTH 31

/*
 * A struct for all macros that contains the name of the macro and the definitions of the actions of that macro and points to the next macro
*/
	typedef struct makro {
	    char name[MAX_MACRO];
	    char *definition;
	    struct makro *next;
	} MAKRO;

/**
 * This function adds a new macro to a stack of macros. 
 * It creates a new node, sets its name and definition to the given values, and adds it to the top of the stack.
 * The function returns a pointer to the new top of the stack.
 * @param stack - pointer to the top of the stack 
 * @param name - pointer to the name of the makro
 * @param definition - pointer to the definition of the makro
 */
	MAKRO *push(MAKRO *stack, char *name, char *definition);


/**
 *This function removes the topmost macro from a stack of macros.
 * It frees the memory allocated for the removed node and returns a pointer to the new top of the stack.
 * @param stack - pointer to the top of the stack
 */
	MAKRO *pop(MAKRO *stack);

/*
 * This function reads a macro definition from a file and adds it to a stack of macros.
 * It takes as input a string, a filename, a pointer to a macro stack, an index, and a pointer to a long integer.
 * It returns a pointer to the updated macro stack.
 * @param input -
 * @param file_name - pointer to the name of the file from which the text is read, .as extension 
 * @param macro - pointer to an array of macros, into which the macros found inserted.
 * @param index - A number indicating the index in the line
 * @param tell -  long int for tracking the scaning in the file
 * @param num_line - the number of the line from the source file being read
 */
	MAKRO *get_macro(char *input, char *file_name, MAKRO *macro, int index, long int *tell ,int *num_line);

/**
 * The check_line function checks whether the line in the macro is empty or starts with a semicolon (';').
 * If the input is not empty or does not start with a semicolon, the  function returns 0. Otherwise, it returns 1.
 * @param input - pointer to the line
 */
	int check_line(char *input);


/**
 * The function prints the line number and the error to the error file, according to the data it received.
 * @param num - the number of the erorr
 * @param num - pounter to the name of the file
 * @param num_line - pointer to the number of line the erorr occurred
 */
	void func_prints(int num, char *file_name, int *num_line);


/**
 * Check if a macro name is a name of guidance or instruction.
 * @param macro - name of the macro to search for
 * @return 20-21 if the macro is a name of guidance or instruction, 0 otherwise
 */
	int vaild_macro(char *macro);


/**
 * The function checks if the sent word is equal to one of the instruction words.
 * @param input_word - The checked name
 * @return 0-15 If it is an action name, otherwise -1
 */
	int what_command(char *input_word);


 /**
 * Check if a macro with the given name is allready defined in the stack.
 * @param name - name of the macro to search for
 * @param stack - pointer to the top of the stack
 * @return 1 if the macro is defined, 0 otherwise
 */
	int is_macro_defined(char *name, MAKRO *stack);


