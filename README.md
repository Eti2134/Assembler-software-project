# Assembler-software-project

🚀 A C-based compiler for our fantasy code,
   By Yael Gluskin and Eti Gotliv bestowed.

## Assignments Overview
### Imaginary Programming Language Compiler
The assembler builds a file containing machine code, from a given file of a program written in assembly language that will be defined here.


## Table of Contents
- [Features](#features)
- [Usage](#usage)
  - [Types of Sentences](#types-of-sentences)
   
- [Example](#example)

## Features
- Converts assembly language code into machine code.



## Usage

 ### Types of Sentences 

#### 1. Empty Sentence

An empty sentence has no instructions or data and is typically used for formatting purposes.
This is a line that contains only whitespace characters, i.e. contains only the characters '\t' and ' ' (tabs and spaces).
It is possible that there is no character in the line (except the character '\n' meaning the line is empty).


## 2. Instruction Sentence

An instruction sentence contains assembly language instructions that will be translated into machine code.


## 3. Directive Sentence

A directive sentence provides directives to the assembler, guiding it in processing the code or defining data.

## 4. Comment Sentence

A comment sentence is used to add comments and explanations to the code. It is ignored by the assembler.

```assembly
; This code prints 'Hello, World!' to the console
```

## Example
```assembly
; Provide a simple example of assembly code
	MAIN:	add r3, LIST
 	LOOP:	prn #48
		lea STR, r6
		inc r6
		mov *r6 , 	K
		sub r1, r4
		cmp r3, #-6
		bne END
		dec K 
		jmp LOOP 
	END:	stop 
	STR:	.string "abcd" d"  	 	 
	LIST:	.data 6,-9
		.data -100
	K:	.data 31

