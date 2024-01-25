#include "files.h"


/* 
 * The function performs different actions depending on the content of each row.
 * If a line starts with a binary number (0 or 1), it writes the binary number to the output file.
 * If a line contains a label defined as an outer label, it encodes it with an outer ARE and 
 * writes it to the output file. If a line contains a label defined as an internal label, 
 * it encodes it with an internal ARE and writes it to the output file.
 * If label is not set, it sets the value of the integer variable pointed to by "stat" to an error code.
 * 
 * @param filename - pouiter to the name of the file
 * @param labels_ptr - a pointer to the array of labels
 * @param num_labels - the numbers of the label that in the array
 * @param IC_number - the number of the IC
 * @param DC_number - the number of the DC
*/
	int second_pass(char *filename, struct LABELS **labels_ptr, int num_labels, int IC_number, int DC_number);

