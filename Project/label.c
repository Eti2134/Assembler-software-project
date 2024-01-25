#include "label.h"

/*Adds a label to the labels array, increasing the number of labels by one.*/
void add_label(struct LABELS **labels_ptr, char *lab, int number, int *num_labels) {
    *labels_ptr = realloc(*labels_ptr, (*num_labels + 1) * sizeof(struct LABELS)); /* Reallocate memory for one more label */
    (*labels_ptr)[*num_labels].lab = malloc(strlen(lab) + 1); /* Allocate memory for the string label and copy it */
    strcpy((*labels_ptr)[*num_labels].lab, lab); 
    (*labels_ptr)[*num_labels].number = number; /* Assign the number to the label*/    
    (*num_labels)++; /* Increment the number of labels. */
}

/*Deletes the last label from the labels array, decreasing the number of labels by one.*/
void delete_last_label(struct LABELS **labels_ptr, int *num_labels) {
    (*num_labels)--;  /* Decrement the number of labels. */
    free((*labels_ptr)[*num_labels].lab); /* Free the memory allocated for the string label. */
    *labels_ptr = realloc(*labels_ptr, (*num_labels) * sizeof(struct LABELS)); /* Reallocate memory to shrink the array by one element */
}

/*Adds an operand to the labels array, increasing the number of labels by one.*/
void add_operand(struct LABELS **labels_ptr, char *lab, int number, int *num_labels) {
	int j = 0;
	int stat = 0;
	for(j = 0; j < *num_labels; j++) { /* A loop that goes through all the labels already inserted. */
            if(!strcmp((*labels_ptr)[j].lab, lab)) { /* The label currently in the line is already in the label array. */
                if((*labels_ptr)[j].number == 1 || (*labels_ptr)[j].number == 0 || (*labels_ptr)[j].number >= 100) {
                    stat = -1;
                    break;
                }
		if((*labels_ptr)[j].number == 3 || (*labels_ptr)[j].number == 5) {
                    stat = -1;
                    break;
                }
            }
        }
        if(stat != -1) {
            add_label(labels_ptr, lab, 3 , num_labels); /* Inserting the label name and valeu 3 into the label array. */
            (*labels_ptr)[(*num_labels)-1].enternal = number; /* Enter the number of the line for next test. */ 
        }

}
