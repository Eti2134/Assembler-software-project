
	Final Project, in a laboratory course in systems programming 20465.

Hosted by Yael Gluskin and Ester Gotliv


Documentation:
	This program accepts an assembler file with the .as extension and reads 
	it and creates a file with the .ob extension from it.

Pre-assembler:
	The program reads the source file .as and interprets the macros it contains 
	in the file with the extension .am. Incorrect retirement of macros or a line that 
	exceeds 80 characters, will cause the work on the particular file to end, and the 
	macro retirement file will also be deleted.

First pass:
	The program reads each line from the .am file created in the program, and for each line 
	checks the correctness of its input. A line in which an error of any kind appears, 
	the error detail and the line number (according to the macro file) will be printed and 
	the program will not continue to check the correctness of the input of the rest of the line.
	The program for each line will immediately print to a temporary file with the extension .an the 
	words it has identified in digits one and zero, for labels, the name of the label will be printed.
	At the end of reading the entire file, 2 more tests will be added:
		* Checking the definition of all the labels in the file
		* Checking whether the number of words in the file does not exceed 256. (actually 155)
	If at least one error is discovered, either in the first pass or in the tests that follow, 
	the program will not go to the second pass. and delete the temporary file, 
	while leaving an error file indicating the errors detected.

Second pass:
	The program will print each code word from the temporary file with the extension .an to the object file 
	in the form of a dot and slash. For the labels, it will do a double conversion to a 14-digit number and 
	immediately to a code word consisting of a dot and slash.
	There will be no errors in the second pass.
	During the second pass, files for external and internal labels will be created, if there are none, 
	the file that will be created will be deleted.
	In any case, a successful program will create .am and .ob files.


Work assumptions during the writing of the plan:
	* The name of a macro cannot be the name of a register, directive or action word, 
	  it can contain letters and numbers. The first letter can also be a number or letter.
	* A line over 80 characters is invalid and will cause the program to immediately terminate its operation on this file.
	* It is forbidden to jump to an external label using the jump addressing method with parameters 
	  (the label to which you jump and not the labels in the parameters).
	* Data directive that does not have any valid number.
	* A string directive with no characters in brackets is valid, but no code word will be generated for a string termination character.
	* All errors found in the program will be printed to an error file as the file name, without any extension.
	* Given a label that is not defined in any way in the file, an error line will only appear 
	  the first time it appears (there is no point in issuing an error message more than once for the same label).
# Assembler-software-project
# Assembler-software-project
# Assembler-software-project
# Assembler-software-project
