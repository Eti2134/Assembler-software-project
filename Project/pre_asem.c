#include "pre_asem.h"

/*Runs the pre-assembly and first pass on a list of input files.*/
int start_progrem(int argc, char *argv[]) {
	char *name;
	int i = 1, len;
	int num = 0;
	int status = 0;
	char *o_file;
	if(argc == 1){/*Check that enough variables have been entered*/
	    fprintf(stderr,"ERROR:There are no files to modify.\n");
	    return 1;	
	}
	while( i< argc){
	    len = strlen(argv[i]);/*The length of the file name*/
	    name = (char*)malloc(len+3);/*Allocation of space for the name plus the suffix*/
	    strcpy(name, argv[i]);
	    name = strcat(name,".as");/*Opening a file with an as extension*/
	    status = vaild_files(name);
	    if(status == 0) {/*If the file to be transferred does not open*/
	        o_file = (char*)malloc(len+4);
	        strcpy(o_file, argv[i]);
	        creat_files(o_file); 
	        o_file = strcat(o_file, ".am"); /*Creating a file with am*/
	        creat_files(o_file); /*Opening a file with am*/
	        status = pre_asem(name, o_file, &num);
	        if(status != 0) {/*If the generated file does not open*/
	            func_prints(num, o_file, &status);
	            remove(o_file);
	            i++;
	            continue;
	        }
	        start_first_pass(o_file);
	        free(o_file);
	    } 	    
	    i++;
	    free(name);
	    
	}   
	return 0;
}

/*pre-processes an assembly file, handling macro commands and creating a new file without macro commands.*/
int pre_asem(char *filename, char *file_new, int *num_erorr) {
	char line[MAX_LINE + 1] = {0};
	char scan;
	int j = 0;
	FILE *ifp = fopen(filename,"r");
	int status;
    	int num_line = 99; /* A variable that counts the lines received from the file. */
   	/* varibals for macro*/
    	MAKRO *stack = NULL;
    	MAKRO *p;
    	/*int num_macros; */
    	char *macro_name;
    	int name_length;
    	int i = 0;
    	long int current_position = 0; /* Variable for tracking the scaning in the file. */
    	long int *position_ptr = &current_position; /* pointer to current_position. */
    	char word[MAX_LINE]; /*a varible to chaek if one of the word is like name of macro*/
    	memset(word,'\0',MAX_LINE);
    
	while(!feof(ifp)) { /* As long as the file does not come to the end. */
	    j = 0;
	    num_line++;
	    while(j <= MAX_LINE ){
		    scan = fgetc(ifp);
		    line[j] = scan;
		    j++;
		    if(scan == '\n'){/*If the character is equal to a line break, a string termination character will be used*/
		        line[++j] = '\0';
		        break;
		    }
		    if(scan == EOF){/*If the character is equal to EOF, a string end character will be used*/
		        line[++j] = '\0';
		        break;
		    }       
	    } /* End of inner while. */
	    if(line[0] == EOF ){/*If the line is empty*/
	        break;
	    }
	    if(j >= MAX_LINE) {
	        if(stack != NULL) {
                pop(stack);
            }
            *num_erorr = 1;
	        return num_line;        
	    }
	    
        status = 0;
        p = stack;
    
        i = 0;
        while(i <= strlen(line) && (line[i] == ' ' || line[i] == '\t')) {
	        i++;
        }
        if(strncmp((const char *)(line+i), "endmcr", 6) == 0) {
            if(stack != NULL) {
                pop(stack);
            }
            *num_erorr = 2;  
            return num_line;                      
        }
        
        /*Check if ther is a new mcr */
        if(strncmp((const char *)(line+i), "mcr ", 4) == 0) {
            status = fseek(ifp, 0, SEEK_CUR);
            current_position = ftell(ifp);
            stack = get_macro(line, filename, stack, i+4, position_ptr, &num_line);
            if(*position_ptr < 0) {
                if(stack != NULL) {
                    pop(stack);
                }
                *num_erorr = abs(*position_ptr);
                return num_line;
            }
            current_position = *position_ptr;
            fseek(ifp, current_position, SEEK_SET);
            p = stack;
            status = 1;
                        
        }
        j = 0;
        /* do a loop that get the first word - Ic can chaenge it to: s = func(line, index)*/
        while(status == 0 && j < MAX_LINE && j < strlen(line) && line[i] != '\n' && line[i] != '\t' && line[i] != ' ') {
            word[j] = line[i];
            i++;
            j++; 
        }
        word[j+1] = '\0'; 
        /* check if the word is like one of the macros names*/
        while(p != NULL) { /**/
            macro_name = p->name;
            name_length = strlen(macro_name);
            if(strncmp(macro_name, word, name_length) == 0) {
                status = 1;
                macro_match(p->definition, file_new);
                /* Found a macro match, Do something here, e.g. print the macro definition  */
                break;
            }
            p = p->next;
        }   
        if(status == 0) {
            macro_match(line, file_new);
         }
         memset(line, '\0', MAX_LINE+1);
    }
    fclose(ifp);
    if(stack != NULL) {
        pop(stack);
    }
    
    return 0;
}

/* This function adds a given input string to the macros file if the input - The string is not empty and does not end with a semicolon. The function skips Any leading whitespace characters in the input string.*/
void macro_match(char *input, char *file_name) {
    FILE *pre;
    int i = 0;
    pre = fopen(file_name, "a+"); /* Openning the file in a mode. */
    while(i <= strlen(input) && (input[i] == ' ' || input[i] == '\t')) { /* loop that skups space */
	        i++;
    }
    if(input[i] != ';' && i+1 != strlen(input) && input[i] != '\0' ) {
        fprintf(pre, "%s", input); 
    }
    fclose(pre);
}


