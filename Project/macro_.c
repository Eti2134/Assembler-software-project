#include "macro_.h"

/*This function adds a new macro to a stack of macros. It creates a new node, sets its name and definition to the given values, and adds it to the top of the stack.The function returns a pointer to the new top of the stack.*/
MAKRO *push(MAKRO *stack, char *name, char *definition) {
    MAKRO *new_node = (MAKRO *)malloc(sizeof(MAKRO)); /* Allocate memory for new node. */
    strcpy(new_node->name, name); /* Copy name to new node. */
    new_node->definition = (char *)malloc(strlen(definition) + 1); /* Allocate memory for definition and copy to new node. */
    strcpy(new_node->definition, definition); 
    new_node->next = stack; /* Set next pointer to point to current top of stack. */
    return new_node; /* Return pointer to new top of stack. */
}

/*This function removes the topmost macro from a stack of macros.It frees the memory allocated for the removed node and returns a pointer to the new top of the stack.*/
MAKRO *pop(MAKRO *stack) {
    MAKRO *popped_node = stack;  /* pointer to node being popped. */
    stack = stack->next; /* set stack to point to next node. */
    free(popped_node->definition); /* free memory allocated for definition. */
    free(popped_node); /*  free memory allocated for node. */
    return stack; /* return pointer to new top of stack. */
}

/* * This function reads a macro definition from a file and adds it to a stack of macros.It takes as input a string, a filename, a pointer to a macro stack, an index, and a pointer to a long integer.It returns a pointer to the updated macro stack.*/
MAKRO *get_macro(char *input, char *file_name, MAKRO *macro, int index, long int *tell, int *num_line) {
    /* varivels for name and defnition */
    
    FILE *ifp = fopen(file_name,"r"); /* The file from which the text is read, .as extension. */
    long int current_position; /* variable for tracking the scaning in the file. */
    /* variables for the index. */
    int i;
    int j;
    /* Variables for scaning each line. */
    char line[MAX_LINE] = {0};
    char scan;
    /* Variables for the macro:*/
    char mcr_word[MAX_MACRO+1]; /* A variable that holds the name of the macro. */
    char *mcr_def; /* A variable that holds the macro definition. */
    MAKRO *stack = macro; 
    MAKRO *p; /* A variable that points to the last macro inserted into the macro array. */
    
    p = stack; 
    current_position = *tell; /* Updating the current location in the file. */
    i = index; /* Moving the index to a local variable. */
    j = 0;
    memset(mcr_word,'\0', MAX_MACRO+1);
    while(isspace(input[i]) || input[i] == '\t') {
        i++;
    }

    /* get the macro name*/
    while(j < MAX_MACRO && input[i] != '\n' && input[i] != '\t' && input[i] != ' ') { /* A loop that goes from the current index to the end of the line. */        
        mcr_word[j] = input[i]; /* Inserting the character from the line into its place in the  mcr word. */
        i++;
        j++; 
    } /* End of get the word */
    
    while(i < strlen(input) ) { /* A loop that goes through the entire line from the current index. */
        if(!isspace(input[i]) != 0) {  /* There is an invalid character at the end of the line. */
            *tell = -3; /* There is an invalid character at the end of the line. */
            return stack;  /* Exiting the function. */
        }
        i++;
    }    
    j = vaild_macro(mcr_word); /* Checking whether the label is valid. */
    if(j != 0) { /* Checking whether the macro name is the name of an instruction or directive statement.n*/
        *tell = -1*j; /* Enter the error number. */
        return stack;  /* Exiting the function. */
	            
    }
    j = is_macro_defined(mcr_word, stack); /* Checking if the macro name has already been defined. */
     if(j != 0) {
        *tell = -7; /* Enter the error number. */
        return stack; /* Exiting the function. */
    }
    mcr_def = (char *)malloc(sizeof(char)*2);
    memset(mcr_def,'\0',2);
    fseek(ifp, current_position ,SEEK_SET);
    
	while(!feof(ifp) ){ /* A loop that will run as long as the am file has not reached its end - the entire pre assembler . */
	    
	    (*num_line)++;
	    j = 0; /* Initializing the index to receive the next line. */
	    fseek(ifp, 0 ,SEEK_CUR); 
	    while(j < MAX_LINE ){
		    scan = fgetc(ifp); /* Read one character from file. */
		    line[j] = scan;
		    j++;
		    if(scan == '\t') {
		        j--;
		    }
		    if(scan == '\n'){  /* If end of line is reached.  */ 
		        line[++j] = '\0';
		        break;
		    }
		    if(scan == EOF) {  /* If end of file is reached. */
		        line[++j] = '\0'; /* If end of file is reached. */
		        break;
		    }       
	    } /* End of geting the line */
	    if(j == MAX_LINE ){ /* The line exceeding 80 characters.*/
	        *tell = -1;  /* Enter the error number. */
	        free(mcr_def);
            return stack;  /* Exiting the function. */
        }
        if(strstr((const char*)line,"endmcr") == NULL) {
            j = j + strlen(line);
            if(check_line(line) == 0) { /* If the line is not empty or starts with ';' */
                mcr_def = (char *)realloc(mcr_def, strlen(mcr_def)+strlen(line));
                strcat(mcr_def, line);
            }
        }
        else {
            mcr_def = (char *)realloc(mcr_def, sizeof(char)*(strlen(mcr_def)+1));
            mcr_def[strlen(mcr_def)] = '\0';
            break;
        }
        memset(line, '\0', MAX_LINE);
     } /* End of ehile */ 
	    
     current_position = ftell(ifp);
     *tell = current_position;
     fclose(ifp);
     stack = push(stack, mcr_word, mcr_def);
     p = stack;
     while(p != NULL) { 
        p = p->next;   
    } 
    free(mcr_def);
    return stack;
}

/* The check_line function checks whether the line in the macro is empty or starts with a semicolon (';').If the input is not empty or does not start with a semicolon, the  function returns 0. Otherwise, it returns 1.*/
int check_line(char *input) {
    int i = 0;
    while(i <= strlen(input) && (input[i] == ' ' || input[i] == '\t')) { /* the macro is empty or starts with a semicolon */
	        i++;
        }
    if(input[i] == ';') { /* start : */
        return 1;
	}
    if(i+1 == strlen(input) || input[i] == '\0' ) {
        return 1;
	}
	return 0;

}

/*The function prints the line number and the error to the error file, according to the data it received.*/
void func_prints(int num, char *file_name, int *num_line) {
    char prints[200][100] = {
                    "No error. ",
                    "A line exceeding 80 characters. ",
                    "Incorrect macro attack: there is 'endmacr' before 'mcr'. ",
                    "There is an invalid character at the end of the line.",
                    "A macro name cannot be the name of a directive statement.",
                    "A macro name cannot be an instruction name.",
                    "A macro name cannot contain characters other than letters or numbers.",
                    "The macro name has already been defined. ",
                    "The label was not defined in the file or was set as extern. ",
                    "A label name cannot be longer than 30 characters. ",
                    "A label name cannot be an instruction name.",
                    "A label name cannot start with a non-letter character. ",
                    "A label name cannot contain characters other than letters or numbers.  ",
                    "A label name cannot be a register name. ",
                    "A label name cannot be the name of a directive statement. ",
                    "There is no definition for label. ",
                    "There is an extra character right after the label definition. ",
                    "External label definition. ",
                    "A previously defined label definition. ",
                    "There is no space immediately after the data directive sentence. ",
                    "There is no space immediately after the string directive sentence. ",
                    "There is no space immediately after the entry directive sentence. ",
                    "The label was previously defined as external, and in this line as internal. ",
                    "This label has already been set as internal. ",
                    "The label was previously defined as internal, and this line is defined as external. ",
                    "This label was used to address a jump with parameters. ",
                    "This label has already been defined, and is now defined as external. ",
                    "There is no space immediately after the extern directive sentence. ",
                    "The directive sentence is incorrect. ",
                    "The action word in the line is incorrect. ",
                    "A label defined as external, and in a line it is used for a jump addressing method. ",
                    "Double minus sign. ",
                    "Plus sign after minus. ",
                    "Invalid character in the data statement. ",
                    "The number is invalid. ",
                    "The number out of range. ",
                    "Consecutive commas in the data instruction. ",
                    "There is an additional character after the number. ",
                    "The label on the line has not been defined in the file in any way. ",
                    "There are more than 256 words.",
                    "There is no space between the instruction for quotation marks. ",
                    "Incorrect opening of quotation marks. ",
                    "Incorrect closing quotation marks.  ",
                    "The command line string is not properly terminated. ",
                    "A macro name cannot be a register name.",
	 	    "Immediate addressing method is not valid in this operation.",
  		    "The line doesnt ends corectlly.",
		    "No opening brackets.",
                    "There is a space inside the brackets.",
                    "No closing brackets.",
                    "There is a new bracket opening.",
                    "The brackets end before the comma.",
                    "Register addressing method is not valid in this operand.",
                    "Instructions of this type contain an additional openend, which is not found.",
                    "Missing comma after first operand.",
                    "Invalid comma.",
                    "it cont be a label - an recognize oprator.",
                    "Invalid label name.",
                    "Jump addressing method is illegal in this instruction.",
		    "Invalid immediate addressing method in this action word.",
 		    "There is already a minus.",
 		    "Illegal char.",
		    "The line endss.",
		    "Missing parameter.",
		    "There is a space in the wrong plac.",
 		    "No space."
		    "There is no comma in the brcket.",
		    "There more than one comma in the brcket."
                    };
                        				   
    fprintf(stdout ,"%d:\t%s\n", (*num_line)-99, prints[num]);
}

/*Check if a macro name is a name of guidance or instruction.*/
int vaild_macro(char *macro) {
    int i;
    if(!strncmp(macro, "data ", 5) || !strncmp(macro, "string ", 7) || !strncmp(macro, "entry ", 6) || !strncmp(macro, "extern ", 7)) { 
        return 4; /* A macro name cannot be the name of a directive statement.*/
    }
    if(what_command(macro) != -1) {
        return 5; /* A macro name cannot be an instruction name. */
    }
    if(strlen(macro) == 2 && macro[0] == 'r' && macro[1] <= '7' && macro[1] >= '0') {
        return 44; /* A macro name cannot be a register name. */
    }
    for(i = 0; i < strlen(macro); i++) { /* A loop that checks whether one of the characters in a macro is not a number or a letter. */
        if(!isalnum(macro[i]) != 0) {
            return 6; /* A macro name cannot contain characters other than letters or numbers. */  
        }
    }
    return 0;
}

/*The function checks if the sent word is equal to one of the instruction words.*/
int what_command(char *input_word) {
    if(strcmp(input_word, "mov") == 0 ) 
        return  0;
    if(strcmp(input_word, "cmp") == 0 )
        return  1;
    if(strcmp(input_word, "add") == 0 )
        return  2;
    if(strcmp(input_word, "sub") == 0 )
        return  3;
    if(strcmp(input_word, "lea") == 0 )
        return  6;
    if(strcmp(input_word, "not") == 0 )
        return  4;
    if(strcmp(input_word, "clr") == 0 )
        return 5;
    if(strcmp(input_word, "inc") == 0 )
        return 7;
    if(strcmp(input_word, "dec") == 0 )
        return 8;
    if(strcmp(input_word, "jmp") == 0 )
        return 9;    
    if(strcmp(input_word, "bne") == 0 )
        return 10;
    if(strcmp(input_word, "red") == 0 )
        return 11;
    if(strcmp(input_word, "prn") == 0 )
        return 12;
    if(strcmp(input_word, "jsr") == 0 )
        return 13;    
    if(strcmp(input_word, "rts") == 0 )
        return  14;
    if(strcmp(input_word, "stop") == 0 ) {
        return 15;
    }    
    return -1; /* If no suitable word is found. */
 }

/*Check if a macro with the given name is allready defined in the stack.*/
int is_macro_defined(char *name, MAKRO *stack) {
   MAKRO *current;
    if (stack == NULL) {  /* check if stack is empty */
        return 0;
    }
    current = stack;
    while (current != NULL) { /* A loop that ran until the macro definitions ran out. */
        if (strcmp(name, current->name) == 0) {  /* check if current macro matches the given name */
            return 7;  /* found a match */
        }
        current = current->next;  /* move on to the next macro. */
    }
    return 0;  /* no match found */
}

