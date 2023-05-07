#include "first.h"

int IC = 100;
int DC = 0;
int NUM_LINE = 100;
char CURRENT_FILE_NAME[20];

/*The function reads the am file and for each line, sends it to the first pass function.If an error is found in the line, the function will call the function to print the errors.At the end of the function, all the labels inserted into the label array in the received rows will be checked.*/
int start_first_pass(char *filename) {
    struct LABELS *labels = NULL; /* A pointer to the label array that will hold all defined labels. */
    int flag = 0; /* A variable that is used as a flag, and turn on when an error is detected in the first pass, or in the tests at the end. */
    int flag_line = 0;
    int num_labels = 0; /* A variable that counts the number of labels inserted into the label array. */
    
     /* Variables for scaning each line. */
    char *line;
    char scan;
    
	FILE *ifp = fopen(filename, "r"); /* Opening the macro file. */
	/* variables for the index. */
    int i = 0;
    int j = 0;

    strcpy(CURRENT_FILE_NAME, filename); /* Copying the file name and saving it. */
    CURRENT_FILE_NAME[strlen(filename)-1] = 'n'; /* Changing the .am file name extension to the .an extension */
    creat_files(CURRENT_FILE_NAME); /* A call to the function that will create the file. */
    /* The results of the first pass will be printed to the file with the extension .an */
    line = (char *)malloc(MAX_LINE); /* Allocating space to a line. */
	while(!feof(ifp)) { /* A loop that will run as long as the am file has not reached its end - the entire first pass. */
	    j = 0; /* Initializing the index to receive the next line. */
	    memset(line,'\0', MAX_LINE); /* Resetting the line in preparation for receiving the next line. */
	    while(j < MAX_LINE ) { /* Read one character from file. */
		    scan = fgetc(ifp); /* Read one character from file. */
		    line[j] = scan; /* Add character to line buffer. */
		    j++; /* Increment line buffer index. */
		    if(scan == '\n') {  /* If end of line is reached.  */ 
		        line[++j] = '\0';  /* Add null terminator to line buffer. */
		        j = (MAX_LINE+1);  /* Set line buffer index to break out of loop. */
		    }
		    if(scan == EOF) {  /* If end of file is reached. */
		        line[++j] = '\0';  /* If end of file is reached. */
		        j = (MAX_LINE+1);  /* Set line buffer index to break out of loop.*/
		    }       
	    } /* End of get the line. */
	    if(line[0] == EOF ) {  /* If end of file is reached. */
	        break;  /*Break out of loop. */
	    }
	    
        flag_line = first_pass(line,  &labels, &num_labels); /* A call to the function that will do the first pass. */
        
        if(flag_line != 0) { /* There is an error in the line. */
            func_prints(flag_line, filename, &NUM_LINE); /* Printing the error, and the number of the line where it was discovered. */  
            flag = 1; /* There was at least one erorr during the first pass run. */   
        }
        NUM_LINE++; /* */
	} /* End of file */
	free(line);
	fclose(ifp);   
    /* A loop that goes through all the labels in the array and looks for whether there are labels that were not defined correctly. */
    for(i = 0; i < num_labels; i++) {      
        if(labels[i].number == 1 && labels[i].enternal >= 100) { /* Labels defined as entry labels but not defined in the file. */
            flag = labels[i].enternal; 
            func_prints(8, filename, &flag); /* Printing the error, and the number of the line where it was discovered. */
            flag = 1; /* The flag turn on and indicates an error. */
        }
        if(labels[i].number == 3 || labels[i].number == 5) { /* Labels inserted into the array but the labels, but not defined in any way. */
            flag = labels[i].enternal;
            func_prints(38, filename, &flag); /* Printing the error, and the number of the line where it was discovered. */
            flag = 1; /* The flag turn on and indicates an error. */
        }
    }
    if(IC+DC > 256) { /* Checking whether the total number of instruction words and actions exceeded the allowed number. */
        func_prints(39, filename, &NUM_LINE); /* Printing the error, and the number of the line where it was discovered. */
        flag = 1; /* The flag turn on and indicates an error. */
    }
    if(flag == 0) { /* If the flag is on and indicates an error, the program will not go to the second pass. */
        second_pass(CURRENT_FILE_NAME, &labels, num_labels, IC, DC); /* A call to the second pass function that will encode the words into a dot and slash. */
    }
    for(i = 0; i < num_labels; i++) { /* A loop that free each member of the label array. */
        free(labels[i].lab);
    }
    free(labels); /* Release the first pointer. */
    func_delete_file(CURRENT_FILE_NAME); /* The function will delete the unnecessary files. */
	return flag;
}

/*The function for each line, checks its content, sends it to appropriate input-validity functions, and checks its input for validity.*/
int first_pass(char *line, struct LABELS **labels_ptr,  int *num_labels) {
    /* Variables for the index.*/
    int i = 0;
    int j = 0;
    
    char *word; /* A variable in which each word in a line is stored */
    int lab = 0; /* A variable used as a flag if a label has been defined at the beginning of the line. */
    int stat = 0; /* A variable used as a flag for the labels. */
    int result; /* A variable that receives the result of the various tests on the words in the line. */
    
    /* Variables for scan labels */
    char *first_operand; /* A variable holding the label in the first operand/parameter. */
    char *second_operand; /* A variable holding the label in the second operand/parameter. */
    char *jump_operand; /* A variable that holds the label that is jumped to in the parameterized jump addressing method. */
    int result_first; /* A variable that holds the result of the source operand/parameter. */
    int result_second; /* A variable that holds the result of the target operand/parameter. */
    int adress_first; /* A variable that holds the types of addressing methods possible in the source argument. */
    int adress_second; /* A variable that holds the types of addressable methods possible in the target argument. */
    
    struct myStruct binary_word = {0}; /* A variable that holds the encoding of each word. */
    /*	for(i = 0; i < *num_labels; i++) { 
        printf("Label %d: %s, %d, %d\n", i+1, (*labels_ptr)[j].lab, (*labels_ptr)[j].number, (*labels_ptr)[j].enternal);
    } */
    /* Getting the first word in the line */
    word = (char *)malloc(sizeof(char)); 
    i = func_getspace(line, i); /* The index jump on the spaces. */
    while(!isspace(line[i]) != 0) { /* A loop that get the first word. */
        if(line[i] == ':') { /* If there is a label. */
            break;
        }
        word = (char *)realloc(word, sizeof(char)*(j+1));
        word[j] = line[i]; /* Inserting the character from the line into its place in the word. */
        i++;
        j++;
    } /* end of while*/
    word = (char *)realloc(word, j+1);
    word[j] = '\0';
    /* end of get the word */
    if(line[i] == ':') { /* Check label */
        result = vaild_label(word);  /* Checking whether the label is valid. */
        if(result != 0) { /* The label is not vaild */
           free(word);
           return result; /* return the number of mistake (9-14). */
        }
        if(i+1 >= strlen(line)) { /* There is no definition for label. */
            free(word);
            return 15; 
        }
        if(line[i+1] != ' ' && line[i+1] != '\t') { /* There is an extra character right after the label definition. */
            free(word);
           return 16; 
        } 
        for(j = 0; j < *num_labels; j++) { /* A loop that goes through all the labels already inserted. */
            if(!strcmp((*labels_ptr)[j].lab, word)) { /* The label currently in the line is already in the label array. */
                if((*labels_ptr)[j].number == 5) { /* This label was used for the jump addressing method. */
                    (*labels_ptr)[j].number = IC+DC; /* Inserting the line number of the label. */
                    (*labels_ptr)[j].enternal = 0;
                    stat = -1; /* Temporary flag for the condition after the loop. */
                    continue; /* The loop will continue to check if the label has appeared in the code on additional lines. */
                }
                if((*labels_ptr)[j].number == 3) { /* This label was used for operand or parameter. */
                    (*labels_ptr)[j].number = IC+DC; /* Inserting the line number of the label. */
                    (*labels_ptr)[j].enternal = 0;
                    stat = -1; /* Temporary flag for the condition after the loop. */
                    continue; /* The loop will continue to check if the label has appeared in the code on additional lines. */
                }
                if((*labels_ptr)[j].number == 0) { /* This label was inserted into the array as an external label. */
                    (*labels_ptr)[j].enternal = 2; /* Entering the value 2 for later tests, to prevent a duplicate error message. */
                    free(word);
		            return 17; /* Setting an external label in a file is not allowed. */
		        }
		        if((*labels_ptr)[j].number == 1 || (*labels_ptr)[j].enternal == 1 ) { /* This label was inserted into the array as an enternal label. */
		            (*labels_ptr)[j].number = IC+DC; /* Inserting the line number of the label. */
		            (*labels_ptr)[j].enternal = 1; /* Inserting the value 1 into the label, to point to an internal label in the second pass. */
		            stat = -1; /* Temporary flag for the condition after the loop. */
		            break; /* There is no need to continue checking the other labels. */  
		        } 
		        /* This labeling was defined a second time - an error. */
		        free(word);
		        (*labels_ptr)[j].enternal = 4; /* Entering the value 4 for later tests, to prevent a duplicate error message. */
		        return 18; /* The label has been defined twice.*/
	        }
	        
        } /* End of checking old labels. */
        if(stat != -1) { /* The label is a label that has not been previously defined in any way in the file. */
            add_label(labels_ptr, word, IC+DC, num_labels); /* Inserting the label name and word number into the label array. */
            (*labels_ptr)[(*num_labels)-1].enternal = 0; /* Inserting the value 0, which indicates a normal label. */
        }
        free(word); /* The label has been applied, the word can be free for next words. */
        i++; /* Advance the index to the next character. */
        lab = 1; /* The flag turn on. */
        /* Get the next word after the label */
        word = (char *)malloc(sizeof(char));
        while(line[i] == ' ' || line[i] == '\t') {
            i++;
        }
        j = 0;
        while(!isspace(line[i]) != 0) { /* A loop that get the second word after the label. */
            word = (char *)realloc(word, sizeof(char)*(j+1));
            word[j] = line[i]; /* Inserting the character from the line into its place in the word. */
            i++;
            j++;
        } /* end of while*/
        word = (char *)realloc(word, j+1);
        word[j] = '\0';
        /* End of get the word */
    
    } /* End of if label */
    
    /*
    From this line of code, the function runs on the word read after the label or the first word received if not a label.
    "lab" is used as a flag for the function with whether or not a label appeared at the beginning of the line.
    */
    
    if(word[0] == '.') { /* There is a directive sentence in the line. */
        if(strncmp(word,".data", 5) == 0) { /* Data directive sentence. */
            if(strlen(word) > 5) { /* There is no space immediately after the directive sentence. */
                free(word);
                return 19; /* There is no space immediately after the data directive sentence. */
            }
            result = data_instruction(line, i); /* Sending to a function that checks the correctness of the numbers, and encodes them. */
            free(word);
            return result; /* Returns the result of the function, 0 or the error number (31-37). */
        } /* End data directive sentence. */
        
        if(strncmp(word,".string", 7) == 0) { /* String directive sentence. */
            if(strlen(word) > 7) { /* There is no space immediately after the directive sentence. */
                free(word);
                return 20; /* There is no space immediately after the string directive sentence. */
            }
            lab = func_string(line, i);
            free(word);
            return lab; /* Returns the result of the function, 0 or the error number (40-43). */
        } /* End string directive sentence. */
        
        if(lab == 1) { /* If the flag is on - there was a label at the beginning of the line, the instruction sentences have no meaning. */
            free(word);
            delete_last_label(labels_ptr, num_labels); /* The function will delete the meaningless label. */
            return 0; /* Warning!! the line doesnt mean anything. */
        }
        if(strncmp(word,".entry", 6) == 0) { /* Directive sentence of inner label. */
            if(strlen(word) > 6) { /* There is no space immediately after the directive sentence. */
                free(word);
                return 21; /* There is no space immediately after the entry directive sentence. */
            }
            free(word);
            /* Get the next word after entry */
            word = (char *)malloc(sizeof(char)*LABEL_LENGTH); /* Allocating space for a label with a length of 30 characters + a string termination character. */
            memset(word, '\0', LABEL_LENGTH); /* Insert string-terminators for convenience. */
            while(line[i] == ' ' || line[i] == '\t') { /* A loop that skips the white characters in a line. */
                i++;
            }            
            j = 0;
            while(i < strlen(line) && !isspace(line[i]) != 0) { /* A loop that gets the word from the line. */
                if(j == LABEL_LENGTH-1 ) { /* Label name exceeded 30 characters. */
                    free(word);
                    return 9; /* A label name cannot be longer than 30 characters. */
                }
                word[j] = line[i]; /* Inserting the character from the line into its place in the word. */
                i++;
                j++;
            } /* End of while*/
            /* End of get the word */
            result = vaild_label(word);  /* Checking whether the label is valid. */
            if(result != 0) { /* The label is not vaild. */
                free(word);
                return result; /* return the number of mistake (9-14). */
            }          
            while(i < strlen(line)) { /* A loop that goes through the entire line from the current index. */
                if(!isspace(line[i]) != 0) { /* If a non-white character is found */
                    free(word);
                    return 3; /* There is an invalid character at the end of the line. */
                }
                i++;
            } /* End of loop */
            stat = 0;
            for(j = 0; j < *num_labels; j++) { /* A loop that goes through all the labels already inserted. */
                if(!strcmp((*labels_ptr)[j].lab, word)) { /* The label currently in the line is already in the label array. */
                    if((*labels_ptr)[j].number == 5 || (*labels_ptr)[j].number == 3) { /* This label was used for the jump addressing method. */
                        (*labels_ptr)[j].enternal = 1; /* Inserting the value 1 into the label, to point to an internal label in the second pass. */
                        (*labels_ptr)[j].number = IC+DC;
		                stat = -1; /* Temporary flag for the condition after the loop. */
		                continue; /* There is no need to continue checking the other labels. */  
                    }
                    if((*labels_ptr)[j].number == 0) { /* This label was inserted into the array as an external label. */
                        free(word);
                        return 22; /* A label defined as external cannot be an internal label. */
                    }
                    if((*labels_ptr)[j].enternal == 1) { /* This label was inserted into the array as an enternal label. */
                        free(word);
                        return 23; /* This label has already been set as internal. */
                    }
                    /* The label found is a label that alreay got defined. */
                    (*labels_ptr)[j].enternal = 1; /* Inserting the value 1 into the label, to point to an internal label in the second pass. */
                    stat = -1; /* Temporary flag for the condition after the loop. */
                    break;
	            }
            }
            if(stat != -1) { /* The label is a label that has not been previously defined in any way in the file. */
                add_label(labels_ptr, word, 1, num_labels); /* Inserting the label name and valeu 1  into the label array. */
                (*labels_ptr)[(*num_labels)-1].enternal = NUM_LINE; /*Entering the line number for the following tests. */
            }
            free(word);
            return 0;
        /* entry */
        } /* End directive sentence of inner label. */
        if(strncmp(word,".extern", 7) == 0) { /* Directive sentence of external label. */
            if(strlen(word) > 7) {  /* There is no space immediately after the directive sentence. */
                free(word);
                return 27; /* There is no space immediately after the extern directive sentence. */
            }
            free(word);
            /* Get the next word after extern. */
            word = (char *)malloc(sizeof(char)*LABEL_LENGTH); /* Allocating space for a label with a length of 30 characters + a string termination character. */
            memset(word, '\0', LABEL_LENGTH); /* Insert string-terminators for convenience. */
            while(line[i] == ' ' || line[i] == '\t') { /* A loop that skips the white characters in a line. */
                i++;
            }            
            j = 0;
            while(i < strlen(line) && !isspace(line[i]) != 0) { /* A loop that gets the word from the line. */
                if(j == LABEL_LENGTH-1 ) { /* Label name exceeded 30 characters. */
                    free(word);
                    return 9; /* A label name cannot be longer than 30 characters. */
                }
                word[j] = line[i]; /* Inserting the character from the line into its place in the word. */
                i++;
                j++;
            } /* End of while*/
            /* End of get the word */
            result = vaild_label(word); /* Checking whether the label is valid. */
            if(result != 0) { /* The label is not vaild */
                free(word);
                return result; /* Return the number of mistake (9-14). */
            }          
            while(i < strlen(line)) { /* A loop that goes through the entire line from the current index. */
                if(!isspace(line[i]) != 0) { /* If a non-white character is found */
                    free(word);
                    return 3; /* There is an invalid character at the end of the line. */
                }
                i++;
            } 
            for(j = 0; j < *num_labels; j++) { /* A loop that goes through all the labels already inserted. */
                if(!strcmp((*labels_ptr)[j].lab, word)) { /* The label currently in the line is already in the label array. */
                    if((*labels_ptr)[j].number == 1) { /* This label was inserted into the array as an enternal label. */
                        free(word);
		                return 24; /* This label has been defined as internal. */
                    }
                    if((*labels_ptr)[j].number == 5) { /* This label was used for the jump addressing method. */
                        free(word);
		                return 25; /* This label was used to address a jump with parameters. */
                    }
                    if((*labels_ptr)[j].number == 3) { /* This label was used for the jump addressing method. */
                        (*labels_ptr)[j].number = 0;
                        (*labels_ptr)[j].number = IC+DC;
                        stat = -1;
                        continue;
                    }
                    (*labels_ptr)[j].enternal = 2;
                    free(word);
		            return 26; /* This lable alreay defined */
	            }
            }
            i = 0; 
            if(stat != -1) {/* The label is a label that has not been previously defined in any way in the file. */
                add_label(labels_ptr, word, 0, num_labels); /* Inserting the label name and valeu 0  into the label array. */
                (*labels_ptr)[(*num_labels)-1].enternal = IC+DC; /*Entering the line number for the following tests. */
            }
        /* extern */
        /* The following commands will be executed, if no valid directive statement is detected. *** */
            free(word);
            return 0; /* The line was successfully received. */
        }
        free(word);
        return 28; /* Command prompt not recognized */
    } /* end of directive statement  */
    
    result = what_command(word); /* check which group the word belongs to */
    free(word);
    if(result == -1) { 
        return 29; /* The action word in the line is incorrect. */
    }
        
    OPCODE(&binary_word, result); /* Encodes the action name in the first word */
    IC++; /* Increase the IC by one. */
    
    if(result == 14 || result == 15 ) { /* The third group that does not accept operands. */
        while(i < strlen(line)) { /* A loop that goes through the entire line from the current index. */
            if(!isspace(line[i]) != 0 && line[i] != '\0') {
                return 3; /* There is anther char after the label. */
            }
            i++;
        }
        func_binary_to_file(binary_word,CURRENT_FILE_NAME); /* Printing the word coded with zeros and ones to a temporary file with the .en extension. */
        return 0; /* The line was successfully encoded. */
    }
    
    if((result >= 0 && result <= 3) || result == 6) {  /* The first group of operations that accept two operands. */
        first_operand = (char *)malloc(sizeof(char)*LABEL_LENGTH); /* Allocation of space for two possible labels. */
        memset(first_operand,'\0',LABEL_LENGTH); /* Insert string-terminators for convenience. */
        second_operand = (char *)malloc(sizeof(char)*LABEL_LENGTH);
        memset(second_operand,'\0',LABEL_LENGTH); /* Insert string-terminators for convenience. */
        if(result == 6) { /* It is lea - and it can get only method 1. */
            adress_first = 2; /* Adjustment of the possible orientation methods in lea - method 1 only. */
        } /* end lea*/
        else {/* get the first operand - it can get 0, 1, 3 */
            adress_first = 1; /* Adjustment of the possible orientation methods. */
        }
        result_first = find_operand(line, &i, &adress_first, first_operand); /* The function identifies the number/label/register. */
        if(adress_first == -1) { /* An error was detected in the line. */
            free(first_operand);
            free(second_operand);
            return result_first;  /* Returns the error number */
        }
        /* Get the second operand - it can get  1, 3*/
        i++;
        if(result == 1) { /* cmp can get 0, 1, 3*/
            adress_second = 4;  /* It is cmp - and it can get the maethods 0, 1 and 3. */
        } 
        else {/* get the second operand - it can get 1 methods 3 */
            adress_second = 3; /* Adjustment of the possible orientation methods. */
        }
        result_second = find_operand(line, &i, &adress_second, second_operand); /* The function identifies the number/label/register. */
        /* second time - check to delete *** */
        if(adress_first == -1) { /* An error was detected in the line. */
            free(first_operand);
            free(second_operand);
            return result_first; /* Returns the error number (). */
        }
        if(adress_second == -1) { /* An error was detected in the line. */
            free(first_operand);
            free(second_operand);
            return result_second; /* Returns the error number */
        }
        DESTENY(&binary_word, adress_second); /* Encodes the word according to the addressing method identified in the destination operand. */
        ORIN(&binary_word ,adress_first); /* Encodes the word according to the addressing method identified in the source operand. */
        func_binary_to_file(binary_word,CURRENT_FILE_NAME); /* Printing the word coded with zeros and ones to a temporary file with the .en extension. */
        writeBinary(&binary_word, 0); /* Reset the word for the following code words. */
        
        if(adress_second == 3 && adress_first == 3) { /* If both operands found are registers. */          
            writeBinary2_7(&binary_word,result_second); /* Encoding the desteny operand register number.*/            
            writeBinary8_13(&binary_word,result_first); /* Encoding the source operand register number. */
            func_binary_to_file(binary_word,CURRENT_FILE_NAME); /* Printing the word coded with zeros and ones to a temporary file with the .en extension. */
            IC++; /* Increase the IC by one. */
            free(first_operand);
            free(second_operand);
            return 0; /* The line was successfully encoded. */
        }
        switch (adress_first) { /* Coding the word according to the method of addressing found. */
            case 1: /* Direct address method - 1. */
                func_label_to_file(first_operand,CURRENT_FILE_NAME); /* Printing the label to temporary file with the .en extension. */
                add_operand(labels_ptr, first_operand, NUM_LINE, num_labels); /* Inserting the label as an operand with the line number. */ 
                break;
            case 0: /* Immediate addressing method - 0. */
                writeBinary2_14(&binary_word, result_first); /* Put the number discovered by the addressing method into the code word. */
                func_binary_to_file(binary_word,CURRENT_FILE_NAME); /* Printing the word coded with zeros and ones. */
                writeBinary(&binary_word, 0); /* Reset the word for the following code words. */
                break;
            case 3: /* Direct address method - 3. */
                writeBinary8_13(&binary_word,result_first); /* Encoding the source operand register number. */ 
                func_binary_to_file(binary_word,CURRENT_FILE_NAME); /* Printing the word coded with zeros and ones to a temporary file with the .en extension. */
                writeBinary(&binary_word, 0); /* Reset the word for the following code words. */
                break; 
       }
       IC++; /* Increase the IC by one. */
        switch (adress_second) { /* Coding the word according to the method of addressing found. */
            case 1: /* Direct address method - 1. */
                func_label_to_file(second_operand,CURRENT_FILE_NAME); /* Printing the label to temporary file with the .en extension. */
                add_operand(labels_ptr, second_operand, NUM_LINE, num_labels); /* Inserting the label as an operand with the line number. */
                break;
            case 0: /* Immediate addressing method - 0. */
                writeBinary2_14(&binary_word, result_second); /* Put the number discovered by the addressing method into the code word. */
                func_binary_to_file(binary_word,CURRENT_FILE_NAME); /* Printing the word coded with zeros and ones to a temporary file with the .en extension. */
                break;
            case 3: /* Direct address method - 3. */ 
                writeBinary2_7(&binary_word, result_second); /* Encoding the desteny operand register number.*/ 
                func_binary_to_file(binary_word,CURRENT_FILE_NAME); /* Printing the word coded with zeros and ones to a temporary file with the .en extension. */
                break; 
        }
        IC++; /* Increase the IC by one. */       
        add_operand(labels_ptr, second_operand, NUM_LINE, num_labels); /* Inserting the label as an operand with the line number. */ 
        free(first_operand);
        free(second_operand);
        return 0; /* The line was successfully encoded. */
    }  /* End of the instruction that accepts 2 operands. */
          
    if((result >= 4 && result <= 13) || result != 6) { /* Instructions that accept only one operand. */
    
        jump_operand = (char *)malloc(sizeof(char)*LABEL_LENGTH); /* Allocation of size 31 for an optional label. */
        memset(jump_operand,'\0',LABEL_LENGTH); /* Insert string-terminators for convenience. */
         
        if(result == 4 || result == 5 || result == 7 || result == 8 || result == 11) { /* Instructions whose addressing methods are 1 and 3. */
            adress_first = 3;
            result_first = find_operand(line, &i, &adress_first, jump_operand); /* The function identifies the label/register. */
            if(adress_first == -1) { /* An error was detected in the line. */
                free(jump_operand);
                return result_first; /* Returns the error number */
            }
        } /* End instructions 1 and 3. */

        if(result == 12) { /* The instruction prn whose addressing methods are 0, 1 and 3. */
            adress_first = 4;
            result_first = find_operand(line, &i, &adress_first, jump_operand); /* The function identifies the number/label/register. */
            if(adress_first == -1) { /* An error was detected in the line. */
                free(jump_operand);
                return result_first; /* Returns the error number */
            }
        } /* End prn */
        
        if(result == 9 || result == 10 ||result == 13) {/* it is prn and it can get 0, 1, 2, 3 */
            /* In instructions that have a jump addressing method, a total of three characters are possible. */
            adress_first = 5;
            result_first = find_operand(line, &i, &adress_first, jump_operand); /* The function identifies the number/label/register. */
            if(adress_first == -1) { /* An error was detected in the line. */
                free(jump_operand); 
                return result_first;
            }
        } /* end method 2*/
        if(adress_first != 5) { /* Coding addressing methods other than jump addressing. */
            
            DESTENY(&binary_word, adress_first); /* Encodes the word according to the addressing method identified in the destination operand. */
            func_binary_to_file(binary_word,CURRENT_FILE_NAME); /* Printing the word coded with zeros and ones to a temporary file with the .en extension. */
            writeBinary(&binary_word, 0); /* Reset the word for the following code word. */
            
            switch (adress_first) { /* Coding the word according to the method of addressing found. */
                case 1: /* Direct address method - 1. */
                    func_label_to_file(jump_operand,CURRENT_FILE_NAME); /* Printing the label to temporary file with the .en extension. */
                    add_operand(labels_ptr, jump_operand, NUM_LINE, num_labels); /* Inserting the label as an operand with the line number. */
                    break;
                case 0: /* Immediate addressing method - 0. */
                    writeBinary2_14(&binary_word, result_first); /* Put the number discovered by the addressing method into the code word. */
                    func_binary_to_file(binary_word,CURRENT_FILE_NAME); /* Printing the word coded with zeros and ones. */
                    break;
                case 3: /* Direct address method - 3. */  
                    writeBinary8_13(&binary_word,result_first); /* Encoding the source operand register number. */ 
                    func_binary_to_file(binary_word ,CURRENT_FILE_NAME); /* Printing the word coded with zeros and ones. */
                    break; 
            }
            IC++; /* Increase the IC by one. */
            free(jump_operand); 
            return 0; /* The line was successfully encoded. */
        } /* End of coding addressing methods other than jump addressing. */   
        else { /* Jump addressing method with parameters. */
            lab = 0;
            for(j = 0; j < *num_labels; j++) { /* A loop that goes through all the labels already inserted. */
                if(!strcmp((*labels_ptr)[j].lab, jump_operand)) { /* The label currently in the line is already in the label array. */
                    if((*labels_ptr)[j].number == 0) { /* This label has been defined as external. */
                        (*labels_ptr)[j].enternal = 2; /* Inserting the value 2 into the label, to point to an failed external label in the second pass. */ 
                        free(jump_operand);
		                return 30; /* A label defined as external, and in a line it is used for a jump addressing method. */
		            }
		            if((*labels_ptr)[j].number == 5 || (*labels_ptr)[j].number == 3) { /* The label was previously entered as an operand/parameter label. */
		                break; /* The label will re-enter the label array with the new line in which it appears. */
		            }
		            else {
		                lab = 1; /* Temporary flag for the condition after the loop. */
		            }
		        }    
		    } 
		    if(!lab) { /* The label is not an entry or extern label. */    
                add_label(labels_ptr, jump_operand, 5 , num_labels); /* Inserting the label name and valeu 0 into the label array. */
                (*labels_ptr)[(*num_labels)-1].enternal = NUM_LINE; /* Enter the value 5 to indicate a label used for a jump address method. */
            }
            
            DESTENY(&binary_word, 2);  /* Encodes the word in method 2. */
            word = (char *)malloc(sizeof(char)*(MAX_LINE-4)); /* Allocation of space for the word that will take the parameters. */
            memset(word,'\0',MAX_LINE-4); /* Insert string-terminators for convenience. */
            j = 0;
            i = result_first;/* If a jump address method is found, the returned value is the index in the line.*/
            while(i < strlen(line) && !isspace(line[i]) != 0 ) { /* A loop that gets the word from the line. */
                word[j] = line[i]; /* Inserting the character from the line into its place in the word. */
                i++;
                j++;
            }
            while(i < strlen(line)) { /* A loop that goes through the entire line from the current index. */
                if(isspace(line[i]) == 0 && line[i] != '\0') { /* There is a character that is not a white character. */
                    free(word);
                    free(jump_operand);
                    return 3; /* There is anther char after the bracket. */
                }
                i++;
            }            
            first_operand = (char *)malloc(sizeof(char)*LABEL_LENGTH); /* Allocation of size 31 for an optional label. */
            memset(first_operand,'\0',LABEL_LENGTH); /* Insert string-terminators for convenience. */
            second_operand = (char *)malloc(sizeof(char)*LABEL_LENGTH); /* Allocation of size 31 for an optional label. */
            memset(second_operand,'\0',LABEL_LENGTH); /* Insert string-terminators for convenience. */
            adress_first = 0;
            adress_second = 0;
            
            result = find_parameters(word, first_operand, second_operand, &result_first, &result_second, &adress_first, &adress_second);
            if(adress_first == -1) { /* An error was detected in the line. */
                free(word);
                free(first_operand);
                free(second_operand);
                free(jump_operand);
                return result; /* Returns the error number */
            }
            
            if(adress_second == -1) { /* An error was detected in the line. */
                free(word);
                free(first_operand);
                free(second_operand);
                free(jump_operand);
                return result; /* Returns the error number */
            }
            PARAMETER1(&binary_word, adress_second); /* Encodes the word according to the addressing method identified in the second parameters. */
            PARAMETER2(&binary_word, adress_first); /* Encodes the word according to the addressing method identified in the first parameters. */
            func_binary_to_file(binary_word ,CURRENT_FILE_NAME); /* Printing the word coded with zeros and ones. */
            
            func_label_to_file(jump_operand,CURRENT_FILE_NAME); /* Printing the label to temporary file with the .en extension. */
            IC++;  /* Increase the IC by one. */
            writeBinary(&binary_word, 0); /* Reset the word for the following code words. */
            if(adress_second == 3 && adress_first == 3) { /* If both parameters found are registers. */               
                writeBinary2_7(&binary_word,result_second); /* Encoding the desteny operand register number.*/             
                writeBinary8_13(&binary_word,result_first); /* Encoding the source operand register number. */
                func_binary_to_file(binary_word,CURRENT_FILE_NAME); /* Printing the word coded with zeros and ones. */
                IC++; /* Increase the IC by one. */
                
                free(word);
                free(first_operand);
                free(second_operand);
                free(jump_operand);
                return 0; /* The line was successfully encoded. */
            }
            switch (adress_first) {
                case 1: /* Direct address method - 1. */
                    func_label_to_file(first_operand,CURRENT_FILE_NAME); /* Printing the label to temporary file with the .en extension. */
                    add_operand(labels_ptr, first_operand, NUM_LINE, num_labels); /* Inserting the label as a parameter with the line number. */
                    break;
                case 0: /* Immediate addressing method - 0. */
                    writeBinary2_14(&binary_word, result_first); /* Put the number discovered by the addressing method into the code word. */
                    func_binary_to_file(binary_word,CURRENT_FILE_NAME); /* Printing the word coded with zeros and ones. */
                    writeBinary(&binary_word, 0); /* Reset the word for the following code words. */
                    break;
                case 3: /* Direct address method - 3. */  
                    writeBinary8_13(&binary_word,result_first); /* Encoding the source operand register number. */
                    func_binary_to_file(binary_word,CURRENT_FILE_NAME); /* Printing the word coded with zeros and ones. */
                    writeBinary(&binary_word, 0); /* Reset the word for the following code words. */
                    break; 
           }
           IC++; /* Increase the IC by one. */
            switch (adress_second) {
                case 1: /* Direct address method - 1. */
                    func_label_to_file(second_operand,CURRENT_FILE_NAME); /* Printing the label to temporary file with the .en extension. */
                    add_operand(labels_ptr, second_operand, NUM_LINE, num_labels); /* Inserting the label as a parameter with the line number. */
                    break;
                case 0: /* Immediate addressing method - 0. */
                    writeBinary2_14(&binary_word, result_second); /* Put the number discovered by the addressing method into the code word. */
                    func_binary_to_file(binary_word,CURRENT_FILE_NAME); /* Printing the word coded with zeros and ones. */
                    break;
                case 3: /* Direct address method - 3. */
                    writeBinary2_7(&binary_word, result_second); /* Encoding the desteny operand register number.*/ 
                    func_binary_to_file(binary_word,CURRENT_FILE_NAME); /* Printing the word coded with zeros and ones. */
                    break; 
           }
           IC++; /* Increase the IC by one. */   
           free(word); 
           free(first_operand);
           free(second_operand);
           free(jump_operand);
           return 0; /* The line was successfully encoded. */
                       
                
         } /* End of else - Jump address. */
    } /* End of instructions that accept one operand */
    return 0;

}

/*The function gets a label name and checks if it is valid.*/
int vaild_label(char *label) {
    int i;
    if(strlen(label) > LABEL_LENGTH) { /* A label name cannot be longer than 30 characters. */
        return 9;    
    }
    if(what_command(label) != -1) {
        return 10; /* A label name cannot be an instruction name. */
    }
    if(!isalpha(label[0]) != 0) {
        return 11;  /* A label name cannot start with a non-letter character. */  
    }
    
    for(i = 1; i < strlen(label); i++) { /* A loop that checks whether one of the characters in a label is not a number or a letter. */
        if(!isalnum(label[i]) != 0) {
            return 12; /* A label name cannot contain characters other than letters or numbers. */  
        }
    }
    if(strlen(label) == 2 && label[0] == 'r' && label[1] <= 7 && label[1] >= 0) {
        return 13; /* A label name cannot be a register name. */
    }
    if(!strncmp(label, "data ", 5) || !strncmp(label, "string ", 7) || !strncmp(label, "entry ", 6) || !strncmp(label, "extern ", 7)) { 
        return 14; /* A label name cannot be the name of a directive statement. */
    }  
    return 0;
}

/*This function takes in a character array and an integer value as input parameters.The character array represents a string of characters and the integer value represents the index from where the function needs to start searching for a non-spaceor non-tab character. The function then iterates through the string from the specified index and returns the index of the first non-space or non-tab character encountered.*/
int func_getspace(char *word, int index) {
    int i = index;
    while(word[i] ==' ' || word[i] == '\t') {
        i++;
    }
    return i;
}

/*Execute the data instruction and add the numbers to the code word array.*/
int data_instruction(char *str, int index) {
    int num; /* A variable that holds the number.*/
    int sign = 1; /* A variable that holds the plus or minus sign. */
    int i = 0;
    char* p = (index+str);
    
    int num_count = 0; /* A variable that counts the number of codewords-the numbers entered into the array. */
    struct myStruct my1 = {0};  /* A variable that holds the encoding of each word. */
    struct myStruct *array = NULL; /* An array that stores all the code words - the numbers found. */

    while(*p != '\0') {
        
        while (*p == ' ' || *p == '\t' || (*p == ',' && *(p+1) == ' ')) { /* Skip spaces and tabs, and any number of spaces before a comma. */
            p++;
        }
        if(*p == '\n' && num_count == 0) { /* There is no character in the data statement */
            return 0;
        }
        if(*p == '\0') { /* check if its the end of the string */
            break;
        }
        if(*p == '-') { /* check for valid sign */
            if(sign == -1) { /* There was already a minus sign in that number. */
                free(array);
                return 31; /* Double minus sign. */
            }
            sign = -1; /* Put the value -1 to indicate a negative number. */
            p++;
        }
        else {
            if(*p == '+') { /* If a plus character is detected, input checks for the character after it. */
                if(sign == -1) { /* Checking whether a minus has already been marked. */
                    free(array);
                    return 32; /* Plus sign after minus. */
                }
                p++;
            }
            else {
                if(!isdigit(*p)) { /* Checking whether the character is valid. */
                    free(array);
                    return 33; /* Invalid character in the data statement. */
                 }
            }
        }
        while (*p == ' ' || *p == '\t' ) { /* A loop that skips the space. */
            p++;
        }
        
        /* parse number */
        num = 0; /* Initialize the number. */
        while(isdigit(*p)) { /* A loop runs as long as there is a digit in the line. */
            num = num * 10 + (*p - '0'); /* Increasing the number according to the identified digit. */
            p++;
        }
        if(sign == 0 || (num == 0 && *(p-1) != '0')) { /* Check for valid number. */
            free(array);
            return 34; /* The number is invalid. */
        }     
        if(sign * num > 16383 || sign * num < -8192) {
            return 35; /* The number out of range. */
        }
        writeBinary(&my1, sign * num); /* Put the number in the code word. */
        add_member(&array, &num_count, &my1); /* Inserting the code word into the code word array. */
        writeBinary(&my1, 0); /* Reset the word for the following code words. */
        

        /* check for comma */
        while (*p == ' ' || *p == '\t' ) { /* A loop that skips over space. */
            p++;
        }
        if(*p == ',') {
            p++;
            while(*p == ' ' || *p == '\t' ) {
                p++;
            }
            if (*p == ',') {
                free(array);
                return 36; /* Consecutive commas in the data instruction. */
            }
            sign = 1;
            } else {
                if(*p != EOF && *p !='\n') {
                    free(array);
                    return 37; /* There is an additional character after the number. */
                    }
               else break;
              }
    }   
    for(i = 0; i < num_count; i++) { /* A loop that exhausts the valid numbers entered into the temporary file with the extension .an */
        func_binary_to_file(array[i], CURRENT_FILE_NAME);
        DC++; /* Increase the DC by one for each valid number taken. */
    }    
    free(array);    
    return 0;
}

/*Converts a string to its binary representation and adds it to the temporary file with the extension .an.*/
int func_string(char *name, int index) {
    int i  = index;
    
    int num_count = 0; /* A variable that counts the number of code words-the letters entered into the array. */
    struct myStruct *arr = NULL; /* An array that stores all the code words - the letters found. */
    struct myStruct my1 = {0};  /* A variable that holds the encoding of each word. */
    if(name[i] != ' ') {
        return 40; /* There is no space between the instruction for quotation marks. */
    }
    while(name[i] == ' ' || name[i] == '\t') { /* Skip over spaces and tabs. */
        i++;
    }
    if(name[i] != '"' ) {
        return 41; /* Incorrect opening of quotation marks.*/
    }
    i++;
    if(name[i] == '"') {
        return 0;  /* A data instruction that does not have a single letter. */
    }
     while(name[i] != '"' && name[i] != '\0') { /* Loop through characters until closing double quote is found. */
        writeBinary(&my1, name[i]); /* Encode character and add it to array. */
        add_member(&arr, &num_count, &my1);
        writeBinary(&my1, 0); /* Reset the word for the following code words. */
                i++;
    }
    if(name[i] != '"') {
        free(arr);
        return 42; /* Incorrect closing quotation marks. */
    }
    i++;
    while(name[i] == ' ' || name[i] == '\t') {  /* Skip over spaces and tabs. */
        i++;               
    }
    if(name[i] != '\0' && name[i] != ' ' && name[i] != '\t' && name[i] != '\n') { /* Check for end of line */
            free(arr);
            return 43; /* The command line string is not properly terminated. */
    }
    for(i = 0; i < num_count; i++) { /* A loop that exhausts the valid letters entered into the temporary file with the extension .an */
        func_binary_to_file(arr[i],CURRENT_FILE_NAME); /* Encode and write each valid letter to the temporary file. */
        DC++; /* Increase the DC by one for each valid number taken. */
    }
    func_binary_to_file(my1,CURRENT_FILE_NAME); /* Print an additional code word for the character '\0'. */
    DC++; /* Increase the DCone more tine for '\0'. */
    free(arr);
    return 0;
}

/*A function that get a line, an index and a case type and checks if it is correct and if not returns minus 1*/
int Address1_fixed_number(char *line, int *index, int *address_case) {
    int i = *index;
    int cursor = 0; /* A marker intended to indicate different states in a line, for the function.*/ 
    int num = 0;
    char *p = (line+(*index));
        if(*address_case != 1 && *address_case != 4 && *address_case != 6) {
            *address_case = -1;
            return 59; /* Invalid immediate addressing method in this action word. */
        }
        cursor = 1;
        if(*p == '-' ) {
            cursor = -1;
            p++;
            i++;
            }
        if(*p == '+') {
            if(cursor == -1) { /* If there was already a minus */
                *address_case = -1;
                return 60; /*There is already a minus*/
            }
            p++;
            i++;
        }
        if(isdigit(*p) == 0) { /* The character is not a number */
            *address_case = -1;
            return 61; /* Illegal char */
        }
        while(!isdigit(*p) == 0) { /* As long as digits are detected, the loop will connect the digits to the number. */
            num = num * 10 + (*p - '0');
            p++;
            i++;
        }
        if( num*cursor > 4095 || num*cursor < -2047) {
            *address_case = -1;
            return 35;/*The number out of range*/
        }
        if(*address_case == 4 || *address_case == 1) {
            while(*p == ' ' || *p =='\t') {
                p++;
                i++;
            }
        }
        if(*p == '\0' || *p =='\n') {   /* Check if there only space to the end */
            if(*address_case == 1) { /* prn,cmp case */
                *address_case = -1;
                return 62; /* the line endss */ 
            }
            else {
                
                *address_case = 0;
                *index = i;
                return(num*cursor);
            }
        }
        /* In case there is a character after the number or after the space promotion. */
        if(*address_case != 1) { /* Only in methods where the operand is a source operand, there can be a character after the space. */
            *address_case = -1;
            return 46; /* the line doesnt ends corectlly  */     
        }
         *address_case = 0;
         *index = i;
         return (num*cursor);
    
}

/*This function receives the content of the parameters in the jump addressing method, it first checks the correctness of the brackets and the comma, and then checks the correctness of the parameters.*/
int find_parameters(char *ptr, char *pot_label1, char *pot_label2, int *first_result,int *second_result, int *case_adress1, int *case_adress2) {
    
	int i, commap = -1, len = 0, amocomma = 0, clo = 0;
	char *firstWord, *secondWord;
        int result1 = 0;
	int result2 = 0;
	if(ptr == NULL){
		return  63;/*Missing parameter*/
		*case_adress1 = -1;
		*case_adress2 = -1;
	}

	if (*ptr != '(') { /* No opening brackets. */
	    *case_adress1 = -1;
		*case_adress2 = -1;
		return 47;
	}

	/*the length of the string*/
	while (ptr[len] != '\0' && clo == 0){
		if(ptr[len] == ' ' && clo == 0){
		    *case_adress1 = -1;
		    *case_adress2 = -1;
			return 48; /* There is a space inside the brackets */
		}
		if(ptr[len] == ')'){
			clo = len;
		}
	len++;
	}
	while(ptr[len] != '\0'){
		if(!isspace(*ptr)){
		    len++;
		}
		else{
		    return 64;/*There is a space in the wrong place*/
		    *case_adress1 = -1;
		    *case_adress2 = -1;
		}
	}
    
	/* Check if the string ends with ) */
	if (ptr[clo] != ')'){
	    *case_adress1 = -1;
		*case_adress2 = -1;
		return 49;/*"No closing brackets."*/
	}

	/* Find the the comma*/
	for ( i = 0; i < len; i++){
		if (ptr[i] == ',' ){
			if(commap == -1){
			commap = i;
			}
			amocomma++;
		}
	}
	
	if(amocomma != 1){
	    *case_adress1 = -1;
		*case_adress2 = -1;
		return 67;/*There more than one comma in the brcket*/
	}

	/* Check if there is a comma between the parentheses*/
	if (commap == -1) {
	    *case_adress1 = -1;
		*case_adress2 = -1;
		return 66;/*There is no comma in the brcket*/
	}

    
	firstWord = (char*)malloc((len*sizeof(char)));
	if(firstWord==NULL){
		/*fprintf(stderr,"ERROR: No space.\n");*/
		free(firstWord);
		*case_adress1 = -1;
		*case_adress2 = -1;
		return 65;/*No space*/
	}
	secondWord = (char*)malloc((len*sizeof(char)));
	if(secondWord==NULL){
		/*fprintf(stderr,"ERROR: No space.\n");*/
		free(firstWord);
		free(secondWord);
		*case_adress1 = -1;
		*case_adress2 = -1;
		return 65;/* No space */
	}

	/* Copy the first word into the first variable*/
	for (i = 1; i < commap; i++){
		if(ptr[i] == '(') { /* There is a new bracket opening */
		    free(firstWord);
		    free(secondWord); 
		    *case_adress1 = -1;
		    *case_adress2 = -1;
		    return 50;/* There is a new bracket opening */
		}
		if(ptr[i] == ')') { /* The brackets end before the comma*/
		    free(firstWord);
		    free(secondWord); 
		    *case_adress1 = -1;
		    *case_adress2 = -1;
		    return 51;/* The brackets end before the comma*/
		}
		firstWord[i-1] = ptr[i];
	}
	firstWord[commap-1] = '\0';

	/* Copy the second word into the second variable*/
	for (i = commap+1; i < len-1; i++){
	    if(ptr[i] == '(') { /* There is a new bracket opening */
		    free(firstWord);
		    free(secondWord);
		    *case_adress1 = -1;
		    *case_adress2 = -1;
		    return 50;/* There is a new bracket opening */
		}
		if(ptr[i] == ')') { /* The brackets end before the comma*/
		    free(firstWord);
		    free(secondWord); 
		    *case_adress1 = -1;
		    *case_adress2 = -1;
		    return 51;/* The brackets end before the comma*/
		}
		secondWord[i-commap-1] = ptr[i];
	}
    secondWord[i-commap-1] = '\0';

	clo = 4; /* A variable now used to convey the type of address. */
	switch(firstWord[0]) {
        case '#' : 
            result1 = Address1_fixed_number((firstWord+1), &result1, &clo);
            
            if(clo == -1 ) {
                free(firstWord);
		        free(secondWord); 
		        *case_adress1 = -1;
                return result1; /* return the number of the mistake */
            }
            *first_result = result1;
            *case_adress1 = 0;
            break;
        case 'r' :
            if(firstWord[1] >= '0' && firstWord[1] <= '7' && strlen(firstWord) == 2) {
                /*cursor = 1; */
                *first_result = firstWord[1] -48; 
                *case_adress1 = 3;
                break;
            }
        default :
            *case_adress1 = 1;
            strcpy(pot_label1, firstWord);
            /* Send to a fnc who check if the label legal */
            break;
	}
	clo = 4;
	switch(secondWord[0]) {
        case '#' : 
            result2 = Address1_fixed_number((secondWord+1), &result2, &clo);
            if(clo == -1 ) {
                free(firstWord);
		        free(secondWord); 
		        *case_adress2 = -1;
                return result2; /* return the number of the mistake */
            }
            *second_result = result2; 
            *case_adress2 = 0;
            break;
        case 'r' :
            if(secondWord[1] >= '0' && secondWord[1] <= '7' && strlen(secondWord) == 2) {
                *second_result = secondWord[1] - 48; 
                *case_adress2 = 3;
                break;
            }
        default :
            *case_adress2 = 1;
            strcpy(pot_label2, secondWord);
            /* Send to a fnc who check if the label legal */
            break;
	}
	free(secondWord);
	free(firstWord);
	return 0;
}

/*The function sends the row and index to discover each operand in turn. It returns the name of the number or register or zero if it is a label*/
int find_operand(char *line, int *index, int *case_adress, char *pot_labels) {
    int i = *index;
    int adress = *case_adress;
    int j = 0;
    int cursor = -1; /* A variable for holding different values */
    char *potential_labels;
    while(line[i] ==' ' || line[i] == '\t') {
        i++;
    }
    switch(line[i]) {
        case '#' :
            i++;
            if(*case_adress == 2 || *case_adress == 3 || *case_adress == 5) {
                *case_adress = -1;
                return 45; /* Immediate addressing method is not valid in this operation. */
            }
            cursor = Address1_fixed_number(line, &i, case_adress);
            
            if(*case_adress == -1) { /* No correct addressing method was found */
                *index = i;
                return cursor; /* Returns the error number */
            }
            
            if(adress == 4) { /* The second operand of the instruction word cmp */
                while(i < strlen(line)) { /*A loop that goes through the entire line to its end*/
                    if(!isspace(line[i]) != 0) { /* If there is a character that is not a white character*/
                        *case_adress = -1;
                        return 3; /* There is an invalid character at the end of the line */
                    }
                 i++;    
                }
            } /* End cmp */
            *case_adress = 0; /* The number of the addressing method found in the operand. */
            *index = i;
            return cursor; /* Returns the constant number. */
        case 'r':
            i++;
            if(line[i] >= '0' && line[i] <= '7') { /* the method will return -1 if it is not register, 0 if all good and anther number if there is a mistak */
                cursor = line[i] - 48;
                i++;
                if(i+1 == strlen(line) ||line[i+1] == ' ' || line[i+1] == '\t' || line[i+1] == ',') {
                    if(*case_adress == 2) { /* The first operand for the instruction lea */
                        *case_adress = -1;
                        return 52; /* Register addressing method is not valid in this operand. */
                    } /* End lea */
                    while(i < strlen(line) && (line[i] == ' ' || line[i] == '\t')) { 
                        i++;
                    }
                    if(*case_adress == 1) { /* First operand of the instructions of the first type. */
                        if(i == strlen(line)) {
                            *case_adress = -1;
                            return 53;  /* Instructions of this type contain an additional openend, which is not found */   
                        }
                        if(line[i] != ',') {
                            *case_adress = -1;
                            return 54;  /* Missing comma after first operand */   
                        }
                        else {
                            *index = i;
                            *case_adress = 3;
                            return cursor ; /* Returning the number of the register*/
                        }
                    } /* End of first operand */
                    while(i < strlen(line) && (line[i] == ' ' || line[i] == '\t')) { 
                        i++;
                    }
                    if(line[i] == '\0' || line[i] =='\n' || i+1 == strlen(line)) {   /* Check if there only space to the end */
                        *case_adress = 3;
                        return cursor; /* Returning the number of the register*/
                    }
                    else {
                        *case_adress = -1;
                        if(line[i] == ',') {
                            return 55; /* Invalid comma */    
                        }
                        return 3; /* There is an invalid character at the end of the line */
                    }
                } /* end register */
                else { /* Probably labels starting with r and number */
                    i--;    
                }
                i--;
            } /* end start of register */
        default:
            if(!isalpha(line[i]) != 0) {
                *case_adress = -1; /* No correct addressing method was found */
                return 56; /* it cont be a label - an recognize oprator*/
            }
            j = 0;
            potential_labels = (char *)malloc(sizeof(char));
            while(i < strlen(line) && !isspace(line[i]) != 0) {
                if(line[i] == ',') {
                    if(*case_adress != 1) {
                        *case_adress = -1;
                        free(potential_labels);  
                        return 55; /* Invalid comma */
                    }
                    break;
                }
                if(line[i] == '(') {
                    if(*case_adress == 5) {
                        *index = i;
                        *case_adress = 5;
                        potential_labels[j] = '\0'; /* end of get the word */
                        cursor = vaild_label(potential_labels);/* chaeck if the label legal*/ 
                        if(cursor != 0) {
                            *case_adress = -1;
                            return 57;   /* Invalid label name */      
                        }
                        strcpy(pot_labels, potential_labels);
                        free(potential_labels);  
                        return i; 
                    } 
                    if(*case_adress != 5) {
                        *case_adress = -1;
                        free(potential_labels);  
                        return 58; /* Jump addressing method is illegal in this instruction. */
                    }    
                }
                potential_labels = (char *)realloc(potential_labels, sizeof(char)*(j+1));
                potential_labels[j] = line[i];
                i++;
                j++;
            } /* end of while*/
            potential_labels = (char *)realloc(potential_labels, j+1);
            potential_labels[j] = '\0'; /* end of get the word */
            cursor = vaild_label(potential_labels);/* chaeck if the label legal*/ 
            if(cursor != 0) {
                *case_adress = -1;
                 return 57;   /* Invalid label name */      
            }
            while(i < strlen(line)) {
                if(!isspace(line[i]) != 0) { /* Checking whether there is another character further down the line. */
                    if(*case_adress != 1 && *case_adress != 2) {
                        *case_adress = -1;
                        return 3; /* There is an invalid character at the end of the line */
                    }
                    break;
                }
                i++;
                
            } /* End of while */
            *index = i; /* The value of the index is now in theline */
            *case_adress = 1; /* Direct address method */
            strcpy(pot_labels, potential_labels);
            free(potential_labels);
            return 0;
    }
}

