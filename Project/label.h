#include "numbers.h"

struct LABELS {
    char *lab;
    int number;
    int enternal;
};

/*
 * Adds a label to the labels array, increasing the number of labels by one.
 * 
 * @param labels_ptr a pointer to a pointer of the LABELS struct representing the array of labels.
 * @param lab a string representing the label to be added.
 * @param number an integer representing the value to be assigned to the label.
 * @param num_labels a pointer to an integer representing the number of labels in the array.
*/
	void add_label(struct LABELS **labels_ptr, char *lab, int number, int *num_labels); 

/**
 * Deletes the last label from the labels array, decreasing the number of labels by one.
 * 
 * @param labels_ptr a pointer to a pointer of the LABELS struct representing the array of labels.
 * @param num_labels a pointer to an integer representing the number of labels in the array.
*/
void delete_last_label(struct LABELS **labels_ptr, int *num_labels);  /* Label libarry */


/*
 * Adds an operand to the labels array, increasing the number of labels by one.
 * 
 * @param labels_ptr a pointer to a pointer of the LABELS struct representing the array of labels.
 * @param lab a string representing the label to be added.
 * @param number an integer representing the value to be assigned to the label.
 * @param num_labels a pointer to an integer representing the number of labels in the array.
*/
void add_operand(struct LABELS **labels_ptr, char *lab, int number, int *num_labels);




