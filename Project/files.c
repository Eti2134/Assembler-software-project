#include "files.h"

/*Checks if a file is empty. If the file is empty, an error message is printed and the function returns 1. Otherwise, it returns 0.*/
int vaild_files(char *filename) {

	FILE *ifp = fopen(filename,"r"); /* Open the file for reading */
	if(ifp == NULL) { /* Check if the file was opened successfully */
		fprintf(stderr,"ERROR: Cannot open file %s\n", filename);
		return 1;
		}
	fseek(ifp,0,SEEK_END); /* Go to the end of the file */
	if(ftell(ifp) == 0) { /* Check if the file is empty */
	  fprintf(stderr,"ERROR: The file %s is empty.\n", filename);
	  fclose(ifp); /* Close the file */
	  return 1;
    }
    fseek(ifp,0,SEEK_SET); /* Go to the beginning of the file */
    return 0; /* Return 0 if the file is valid */
}

/*Creates an empty file with the given name.*/
void creat_files(char *file_name) {
    FILE *p_file = fopen(file_name, "w"); /* Creates a new file, an old file with the same name will be overwritten. */
    fclose(p_file);
    
}

/*Appends the given label to the end of the file with the given name.*/
void func_label_to_file(char *label, char *file_name) {
    FILE *pre;
    pre = fopen(file_name, "a+");  /* Open the file for writing. */
    fprintf(pre, "%s\n", label); /* Print the label. */
    fclose(pre);
}

/*Converts a given variable in a struct to a binary string representation and writes it to the end of a file with the given name.*/
void func_binary_to_file(struct myStruct myInstance,  char *file_name) {
    FILE *pre;
    int i = 0;
    pre = fopen(file_name, "a+"); /* Open the file for writing. */
	for (i = 13; i >= 0; i--) { /*A loop goes over the myInstance digits.*/
        fprintf(pre, "%d", (myInstance.myVariable >> i) & 1); /* Print the number. */
    }
    fprintf(pre, "\n");
    fclose(pre);
}

/*This function deletes a file, along with any associated files that were created during its processing, if and only if the file is empty. If the file has content, it prints a message indicating that the file has content and does not delete the file.*/
void func_delete_file(char *file_name) {
    FILE *eror_file;
    FILE *enternal_file;
    FILE *external_file;
    int size;
    char *new_name = (char*) malloc(strlen(file_name) + 5);
    strcpy(new_name, file_name);
    
    remove(file_name); /* Remove any associated .an files */
    
    /* Remove the ".an" extension from the file name */
    new_name[strlen(new_name) - 3] = '\0';
    /* Open the file and get its size */
    eror_file = fopen(new_name, "r");
    fseek(eror_file, 0, SEEK_END);
    size = ftell(eror_file);
    /* If the file has content, do not delete it */
    if (size != 0) {
        /* Remove any associated .ob files */
        new_name[strlen(file_name) - 3] = '.';
        new_name[strlen(file_name) - 2] = 'o';
        new_name[strlen(file_name) - 1] = 'b';
        remove(new_name);
        /* Check for and remove any associated .ent files */
        new_name[strlen(file_name) - 2] = 'e';
        new_name[strlen(file_name) - 1] = 'n';
        new_name[strlen(file_name)] = 't';
        new_name[strlen(file_name) + 1] = '\0';
        enternal_file = fopen(new_name, "r");
        if(enternal_file != NULL) {
		free(new_name);
            fclose(enternal_file);
            remove(new_name);
        }
        /* Check for and remove any associated .ex files */
        new_name[strlen(file_name) - 1] = 'x';
        external_file = fopen(new_name, "r");
        if(external_file != NULL) {
		free(new_name);
            fclose(external_file);
            remove(new_name);
        }
        return;
    }
    /* If the file is empty, delete it */
	free(new_name);
    fclose(eror_file);
    remove(new_name);
}

/**
 * @brief Creates two empty files with the extensions ".ent" and ".ext" respectively based on the given file name.
 *
 * @param file_name a string representing the name of the file to be labeled.
 * @return void.
 */
void labels_file(char *file_name) {
     FILE *enternal_file;
     FILE *external_file;
     char *new_name = (char*) malloc(strlen(file_name) + 5);
     strcpy(new_name,file_name);
     /* Create .ent file */
     new_name[strlen(file_name) - 2] = 'e';
     new_name[strlen(file_name) - 1] = 'n';
     new_name[strlen(file_name)] = 't';
     new_name[strlen(file_name) +1] = '\0';
     enternal_file = fopen(new_name, "w");
     fclose(enternal_file);
     /* Create .ext file */
     new_name[strlen(file_name) - 1] = 'x';
     external_file = fopen(new_name, "w");
	free(new_name);
     fclose(external_file);
}

/*Deletes empty files labeled with the extensions ".ent" and ".ext" respectively based on the given file name.*/
void delete_labels_file(char *file_name) {
      FILE *internal_file; /* File pointer for .ent file */
      FILE *external_file; /* File pointer for .ext file */
      long int size; /* Variable to hold the size of the file */
      char *new_name = (char*) malloc(strlen(file_name) + 5); /* Create a new string variable to store the file name */
      /* Check and delete .ent file */
      strcpy(new_name, file_name); /* Copy the original file name to the new string variable */
      new_name[strlen(file_name) - 2] = 'e'; /* Replace the last two characters with "ent" */
      new_name[strlen(file_name) - 1] = 'n';
      new_name[strlen(file_name)] = 't';
      new_name[strlen(file_name)+1] = '\0';
      internal_file = fopen(new_name, "r"); /* Open the .ent file */
      fseek(internal_file, 0, SEEK_END); /* Go to the end of the file */
      size = ftell(internal_file); /* Get the size of the file */
      fclose(internal_file); /* Close the file */
      if(size == 0) { /* If the file is empty */
          remove(new_name); /* Delete the file */
      }

      /* Check and delete .ext file */
      new_name[strlen(file_name) - 1] = 'x'; /* Replace the last character with "ext" */
      external_file = fopen(new_name, "r"); /* Open the .ext file */
      fseek(external_file, 0, SEEK_END); /* Go to the end of the file */
      size = ftell(external_file); /* Get the size of the file */
      fclose(external_file); /* Close the file */
      
      if(size == 0) { /* If the file is empty */
          remove(new_name); /* Delete the file */
      }
	  free(new_name);
}

/*Writes the given label and line number to a .ext file with the same name as file_name.*/
void print_extern(char *file_name, char *label, int num_line) {
    FILE *external_file;
    char *new_name = (char*) malloc(strlen(file_name) + 5);
    strcpy(new_name, file_name);
    strcpy(new_name + strlen(new_name) - 2, "ext");  /* Replace the last two characters of the file name with "ext" to create the external file name. */
    external_file = fopen(new_name, "a+"); /* Open the external file in append mode. */
    fprintf(external_file, "%s\t%d\n", label, num_line); /* Print the label and the number of line to the external file. */
	free(new_name);
    fclose(external_file);
}

/*Writes the given label and line number to a .ent file with the same name as file_name.*/
void print_entrnal(char *file_name, char *label, int num_line) {
	FILE *entrnal_file;
	char *new_name = (char*) malloc(strlen(file_name) + 5); /* Replace the last two characters of the file name with "ext" to create the external file name. */
	strcpy(new_name, file_name); /* Open the external file in append mode. */
	strcpy(new_name + strlen(new_name) - 2, "ent"); /* Print the label and the number of line to the external file. */
    entrnal_file = fopen(new_name, "a+");
    fprintf(entrnal_file, "%s\t%d\n", label, num_line);
	free(new_name);
    fclose(entrnal_file);
}
