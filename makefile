run:start.o macro_.o numbers.o label.o files.o second_pass.o first.o pre_asem.o
	gcc -g -Wall -ansi -pedantic  start.o macro_.o numbers.o label.o files.o second_pass.o first.o pre_asem.o -o run
start.o: start.c 
	gcc -Wall -ansi -pedantic -c start.c -o start.o
macro_.o: macro_.c macro_.h 
	gcc -Wall -ansi -pedantic -c macro_.c -o macro_.o
numbers.o: numbers.c numbers.h
	gcc -Wall -ansi -pedantic -c numbers.c -o numbers.o
label.o: label.c label.h
	gcc -Wall -ansi -pedantic -c label.c -o label.o
files.o: files.c files.h
	gcc -Wall -ansi -pedantic -c files.c -o files.o
second_pass.o: second_pass.c second_pass.h
	gcc -Wall -ansi -pedantic -c second_pass.c -o second_pass.o
first.o: first.c first.h
	gcc -Wall -ansi -pedantic -c first.c -o first.o
pre_asem.o: pre_asem.c pre_asem.h
	gcc -Wall -ansi -pedantic -c pre_asem.c -o pre_asem.o


