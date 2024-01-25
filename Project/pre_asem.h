#include "first.h"


/*
 * Runs the pre-assembly and first pass on a list of input files.
 *
 * @param argc The number of arguments passed to the function.
 * @param argv An array of C-strings, where each string represents the name of an input file.
 * @return Returns 0 on success, or a non-zero value on failure.
 *
 * This function takes a list of input file names and runs the pre-assembly and first pass
 * stages of the assembly process on each file. If a file is not valid, an error file is created
 * and the function moves on to the next file. If the assembly is successful, a machine code file
 * is generated for the input file.
 *
 */
	int start_progrem(int argc, char *argv[]);

/**
    * pre_asem: pre-processes an assembly file, handling macro commands and creating a new file without macro commands.
    *
    * @param filename The name of the input file to be preprocessed.
    * @param file_new The name of the output file without macros.
    * @param num_erorr pointer to the number of errors encountered during preprocessing.
    *
    * @return If the preprocessing was successful, the function returns 0. Otherwise, it returns the line number where the error occurred.
    */
	int pre_asem(char *filename, char *file_new, int *num_erorr);

/**
   *
   * Description: This function adds a given input string to the macros file if the input -
   * The string is not empty and does not end with a semicolon. The function skips
   * Any leading whitespace characters in the input string.
   *
   * @param input: pointer to a null-terminated string representing the input to be matched and Attached to the file.
   * @param file_name: pointer to a null-terminated string representing the name of the file to which it belongs Input will be attached.
   *
   */
	void macro_match(char *input, char *file_name) ;
