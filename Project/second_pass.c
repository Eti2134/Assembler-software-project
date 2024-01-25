#include "second_pass.h"

/* The function performs different actions depending on the content of each row. If a line starts with a binary number (0 or 1), it writes the binary number to the output file. If a line contains a label defined as an outer label, it encodes it with an outer ARE and writes it to the output file. If a line contains a label defined as an internal label, it encodes it with an internal ARE and writes it to the output file.If label is not set, it sets the value of the integer variable pointed to by "stat" to an error code.*/
int second_pass(char *filename, struct LABELS **labels_ptr, int num_labels, int IC_number, int DC_number) {
    char *ob_file;
    char *line;
    int i;
    int j = 0;
    int k =0;
    struct myStruct binary_word = {0};
    FILE *pass_file;
    FILE *object_file;
    
    line = malloc(sizeof(char)*LABEL_LENGTH);
    memset(line, '\0', LABEL_LENGTH);
    ob_file = malloc(sizeof(char)*strlen(filename) );
    strcpy(ob_file, filename);
    ob_file[strlen(filename) - 2] = 'o';
    ob_file[strlen(filename) - 1] = 'b';    
    pass_file = fopen(filename, "r");
    object_file = fopen(ob_file, "w");
    labels_file(filename);  
	 
    fprintf(object_file,"\t%d\t%d\n", IC_number-100, DC_number);
    
    while(fgets(line, LABEL_LENGTH, pass_file) != NULL) {
        fprintf(object_file,"%d\t", k+100);
        k++;
        line[strlen(line) - 1] = '\0'; /* Deletes the character \n and replaces it with \0 */
        if(line[0] == '0' || line[0] == '1') {/*Replacing the zero and the one with a period and a slash*/
            for (i = 0; i < 14; i++) {
                if(line[i] == '1') {
                    fputc('/', object_file);
                } else {
                    fputc('.', object_file);
                }
            }
            fputc('\n', object_file);
            continue;
        }
        for(i = 0; i < num_labels; i++) { /* A loop that goes through all the defined labels. */
            if(!strcmp((*labels_ptr)[i].lab, line)) { /* If the label is found. */
                if((*labels_ptr)[i].number == 0 && (*labels_ptr)[i].enternal >= 100 ) { /* Checking whether the label is external. */
                    j = 0; /* Encodes it in an external ARE */
                    while(j < 13) {
                        fputc('.', object_file);
                        j++;
                    }
                    fputc('/', object_file);
                    fputc('\n', object_file);
                    print_extern(filename, line, 100+k);
                    break;
                }
                if((*labels_ptr)[i].enternal == 1) { /* Checking whether the label is internal. */
                    ARE(&binary_word, 2);  /* Codes it in an internal ARE */
                    writeBinary2_14(&binary_word,(*labels_ptr)[i].number); /* Encodes the label address */
                    
                    for(i = 13; i >= 0; i--) { /*  Prints the label to the object file */
		                if( (binary_word.myVariable >> i) & 1 ) {/*Replacing the zero and the one with a period and a slash*/
			                fputc('/', object_file);            
		                }
		                else{
			                fputc('.', object_file);
		                }
                    }
                    fputc('\n', object_file);       
                    writeBinary(&binary_word, 0);
                    print_entrnal(filename, line, 100+k);
                    break;
                }
                 /* A label that has not been defined as external or internal */
                 if((*labels_ptr)[i].number != 0) {
                    writeBinary2_14(&binary_word, (*labels_ptr)[i].number); /* Encodes the label address */
                    for(i = 13; i >= 0; i--) { /*  Prints the label to the object file */
		                if( (binary_word.myVariable >> i) & 1 ) {/*Replacing the zero and the one with a period and a slash*/
			                fputc('/', object_file);            
		                }
		                else{
			                fputc('.', object_file);
		                }
                    }
                    fputc('\n', object_file);
                    writeBinary(&binary_word, 0);
                    break;
                 }
	
            } /* End code label */
                
        }
    }
    
    fclose(object_file);/*Closing the files*/
    fclose(pass_file);/*Closing the files*/
    delete_labels_file(filename); /* Call the func to delete the previes files */
    return 0;
}
