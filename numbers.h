#include "macro_.h"

typedef enum {
    false = 0,
    true = 1
} bool;

struct myStruct {
    unsigned int myVariable : 14;
};

/* 
 * Returns true if the given integer is negative, false otherwise.
 *
 * @param num - The integer being checked.
 */
	bool isNegative(int num);

/* 
 * Returns the two's complement of the given unsigned integer.
 *
 * @param num - The integer to get the two's complement.
 */
	unsigned int twoComplement(unsigned int num);


/*
 * Sets the least significant 14 bits of the given myStruct instance to the binary representation of the given integer.
 * If the integer is negative, its two's complement is used instead.
 *
 * @param myInstance - Pointer to the myStruct instance to modify.
 * @param num - The integer to convert to binary and store in myInstance.
*/
	void writeBinary(struct myStruct *myInstance, int num);


/* Sets the given value to the least significant 2 bits of the myVariable field of the given myStruct instance.
 * If the value is outside the range of 0-2, nothing is done.
 *
 * @param myInstance - Pointer to the myStruct instance to modify.
 * @param num - The value to set to the least significant 2 bits of myInstance->myVariable.
 */
	void ARE(struct myStruct *myInstance, int num);


/* Sets the given value to the 2nd and 3rd least significant bits of the myVariable field of the given myStruct instance.
 * If the value is outside the range of 0-3, nothing is done.
 *
 * @param myInstance - Pointer to the myStruct instance to modify.
 * @param num - The value to set to the 2nd and 3rd least significant bits of myInstance->myVariable.
 */
	void DESTENY(struct myStruct *myInstance, int num);


/* Sets the given value to the 4th and 5th least significant bits of the myVariable field of the given myStruct instance.
 * If the value is outside the range of 0-3, nothing is done.
 *
 * @param myInstance - Pointer to the myStruct instance to modify.
 * @param num - The value to set to the 4th and 5th least significant bits of myInstance->myVariable.
 */
	void ORIN(struct myStruct *myInstance, int num);


/* Sets the given value to the 6th-9th least significant bits of the myVariable field of the given myStruct instance.
 * If the value is outside the range of 0-15, nothing is done.
 * @param myInstance - Pointer to the myStruct instance to modify.
 * @param num - The value to set to the 6th-9th least significant bits of myInstance->myVariable.
 */
	void OPCODE(struct myStruct *myInstance, int num);


/* Sets the given value to the 11th and 12th least significant bits of the myVariable field of the given myStruct instance.
 * If the value is outside the range of 0-3, nothing is done.
 *
 * @param myInstance - Pointer to the myStruct instance to modify.
 * @param num - The value to set to the 11th and 12th least significant bits of myInstance->myVariable.
 */
	void PARAMETER1(struct myStruct *myInstance, int num);

/* Sets the given value to the 13th and 14th least significant bits of the myVariable field of the given myStruct instance.
 * If the value is outside the range of 0-3, nothing is done.
 *
 * @param myInstance - Pointer to the myStruct instance to modify.
 * @param num - The value to set to the 13th and 14th least significant bits of myInstance->myVariable.
 */
	void PARAMETER2(struct myStruct *myInstance, int num);

/* 
 * Sets the 12 least significant bits of the given myStruct instance to the binary representation of the given integer. 
 * If the integer is negative, its two's complement is used instead.
 *
 *   @param myInstance - Pointer to the myStruct instance to modify.
 *   @param num - The integer to convert to binary and store in myInstance.
 */
	void writeBinary2_14(struct myStruct *myInstance, int num);

/*
 * Writes the binary representation of an integer to bits 2-7 of the myVariable field of a given myStruct instance.
 *
 * @param myInstance a pointer to the myStruct instance to modify.
 * @param num the integer whose binary representation to write.
 */
	void writeBinary2_7(struct myStruct *myInstance, int num);

/*
 * Writes the binary representation of an integer to bits 8-13 of the myVariable field of a given myStruct instance.
 *
 * @param myInstance a pointer to the myStruct instance to modify.
 * @param num the integer whose binary representation to write.
 */
	void writeBinary8_13(struct myStruct *myInstance, int num);


/*
 * Adds a myStruct instance to an array of myStruct instances.
 *
 * @param array a pointer to the array of myStruct instances to add to.
 * @param size a pointer to the size of the array.
 * @param myInstance a pointer to the myStruct instance to add to the array.
 */
	void add_member(struct myStruct **array, int *size, struct myStruct *myInstance);
