# Assembler-software-project

🚀 A C-based compiler for our fantasy code,
   By [Yael Gluskin] and [Eti Gotliv] bestowed.

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



### 1. Instruction Sentence

An instruction sentence contains assembly language instructions that will be translated into machine code.
#### There are 15 instruction words:
| word | code | number of operands  |
| ------ | ------ | ------ | 
|0| mov | 2
| 1| cmp |2
| 2| add |2
| 3| sub |2
| 4| not|2
| 5| clr|2
| 6| lea|2
| 7| inc|1
| 8| dec|1
| 9| jmp|1
| 10| bne|1
| 11| red|1
| 12| prn|1
| 13| jsr|1
| 14| rts|0
| 15|stop|0 |



### 2. Directive Sentence

A directive sentence provides directives to the assembler, guiding it in processing the code or defining data.
#### The directive sentence has the following structure:
|directive | explanation | example
| ------ | ------ | ------ | 
|.data | A directive to allocate space for parameters | ``` .data 7, −57, +17, 9 ```
|.string | A directive to group space into a string | ``` STR: .string "abcde"  ```
| .entry | Characterizes the labels as one that is stored in this file. | ``` .entry SHALOM  ```
| .extern | Characterizes the labels as one that is not defined in this file. | ``` .extern BYE  ``` |

At the beginning of the sentence a definition of a label can appear and then the word of instruction:


### 3. Comment Sentence

A comment sentence is used to add comments and explanations to the code. It is ignored by the assembler.

```assembly
; This code prints 'Hello, World!' to the console
```
### 4. Empty Sentence

An empty sentence has no instructions or data and is typically used for formatting purposes.
This is a line that contains only whitespace characters, i.e. contains only the characters '\t' and ' ' (tabs and spaces).
It is possible that there is no character in the line (except the character '\n' meaning the line is empty).

## Example
An example of assembly language code becomes machine language
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
	STR:	.string "abcd"  	 	 
	LIST:	.data 6,-9
		.data -100
	K:	.data 31
```
[Yael Gluskin]: <https://github.com/YaelGluskin>
[Eti Gotliv]: <https://github.com/EtiGotliv>
