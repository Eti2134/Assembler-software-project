#include "label.h"


/*
 * Checks if a file is empty. If the file is empty, an error message is printed and the function returns 1. Otherwise, it returns 0.
 * 
 * @param filename a string representing the name of the file to be checked.
 * 
 * @return 0 if the file is not empty, 1 otherwise.
*/
	int vaild_files(char *filename);


/*
 * Creates an empty file with the given name.
 * 
 * @param file_name a string representing the name of the file to be created.
*/
	void creat_files(char *filename);


/*
 * Appends the given label to the end of the file with the given name.
 *
 * @param label a string representing the label to be added to the file.
 * @param file_name a string representing the name of the file to which the label should be added.
*/
	void func_label_to_file(char *label, char *file_name);


/*
 * Converts a given variable in a struct to a binary string representation and writes it to the end of a file with the given name.
 *
 * @param myInstance a struct containing the variable to be converted to binary and written to the file.
 * @param file_name a string representing the name of the file to which the binary string should be written.
*/
	void func_binary_to_file(struct myStruct myInstance, char *file_name); 

/*
 * This function deletes a file, along with any associated files that were created during its processing, if and only if the file
 * is empty. If the file has content, it prints a message indicating that the file has content and does not delete the file.
 *
 * @param file_name - The name of the file to delete
 */
	void func_delete_file(char *file_name);

/**
 * Creates empty .ent and .ext files associated with the given file
 *
 * @param file_name a string representing the name of the file to be checked for empty labeled files.
 */
	void labels_file(char *file_name);


/*
 * Deletes empty files labeled with the extensions ".ent" and ".ext" respectively based on the given file name.
 *
 * @param file_name a string representing the name of the file to be checked for empty labeled files.
 */
	void delete_labels_file(char *file_name);

/**
 * Writes the given label and line number to a .ext file with the same name as file_name.
 *
 * @param file_name a string representing the name of the file to write the label and line number to.
 * @param label a string representing the label to be written.
 * @param num_line an integer representing the line number to be written.
 */
	void print_extern(char *file_name, char *label, int num_line);


/**
 * Writes the given label and line number to a .ent file with the same name as file_name.
 *
 * @param file_name a string representing the name of the file to write the label and line number to.
 * @param label a string representing the label to be written.
 * @param num_line an integer representing the line number to be written.
 */
	void print_entrnal(char *file_name, char *label, int num_line);
