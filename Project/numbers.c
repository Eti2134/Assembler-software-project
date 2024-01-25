#include "numbers.h"

/*Returns true if the given integer is negative, false otherwise.*/
bool isNegative(int num) {
    return num < 0;
}

/*Returns the two's complement of the given unsigned integer.*/
unsigned int twoComplement(unsigned int num) {
    return ~num + 1;
}

/*Sets the least significant 14 bits of the given myStruct instance to the binary representation of the given integer.If the integer is negative, its two's complement is used instead.*/
void writeBinary(struct myStruct *myInstance, int num) {
    if (isNegative(num)) {
        num = twoComplement(-num); /* convert to unsigned before taking two's complement */
    }
    myInstance->myVariable = num & 0x3FFF; /* make sure only the least significant 14 bits are set */
}

/*Sets the 12 least significant bits of the given myStruct instance to the binary representation of the given integer. If the integer is negative, its two's complement is used instead.*/
void writeBinary2_14(struct myStruct *myInstance, int num) {
	unsigned int shiftedNum;
    if (isNegative(num)) {
        num = twoComplement(-num); /* convert to unsigned before taking two's complement */
    }
     shiftedNum = (num & 0x3FFF) << 2; /* shift the least significant 14 bits to the left by 2 positions */
    myInstance->myVariable = shiftedNum; /* set the bits in the struct */
}

/*Sets the given value to the least significant 2 bits of the myVariable field of the given myStruct instance.If the value is outside the range of 0-2, nothing is done.*/
void ARE(struct myStruct *myInstance, int num) {
    if(num >= 0 && num <= 2) { /* make sure num is in the range of 0-2 */
        myInstance->myVariable = (myInstance->myVariable) | num; /* clear the first two bits and set the value */
    }
}

/*Sets the given value to the 2nd and 3rd least significant bits of the myVariable field of the given myStruct instance.If the value is outside the range of 0-3, nothing is done.*/
void DESTENY(struct myStruct *myInstance, int num) {
    if(num >= 0 && num <= 3) { /* make sure num is in the range of 0-3 */
        unsigned int value = num << 2;
        myInstance->myVariable = (myInstance->myVariable) | value; /* clear the 2-3 bits and set the value */
    }
}

/*Sets the given value to the 4th and 5th least significant bits of the myVariable field of the given myStruct instance.If the value is outside the range of 0-3, nothing is done.*/
void ORIN(struct myStruct *myInstance, int num) {
    unsigned int value = num << 4; /* shift the num value to the left to occupy the 4-5 bits */
    if(num >= 0 && num <= 3) { /* make sure num is in the range of 0-3 */
        myInstance->myVariable = (myInstance->myVariable) | value; /* clear the 4-5 bits and set the value */
    }
}

/* Sets the given value to the 6th-9th least significant bits of the myVariable field of the given myStruct instance.If the value is outside the range of 0-15, nothing is done.*/
void OPCODE(struct myStruct *myInstance, int num) {
    unsigned int value = num << 6; /* shift the num value to the left to occupy the 4-5 bits */
    if(num >= 0 && num <= 15) { /* make sure num is in the range of 0-3 */
        myInstance->myVariable = (myInstance->myVariable ) | value; /* clear the 4-5 bits and set the value */
    }
}

/*Sets the given value to the 11th and 12th least significant bits of the myVariable field of the given myStruct instance.If the value is outside the range of 0-3, nothing is done.*/
void PARAMETER1(struct myStruct *myInstance, int num) {
    if (num >= 0 && num <= 3) { /* make sure num is in the range of 0-3 */
        unsigned int value = num << 10; /* shift num to the left by 10 bits */
        myInstance->myVariable = (myInstance->myVariable) | value ; /* clear the bits 10-11 and set the value */
    }
}

/*Sets the given value to the 13th and 14th least significant bits of the myVariable field of the given myStruct instance.If the value is outside the range of 0-3, nothing is done.*/
void PARAMETER2(struct myStruct *myInstance, int num) {
    if (num >= 0 && num <= 3) { /* make sure num is in the range of 0-3 */
        unsigned int value = num << 12; /* shift num to the left by 12 bits */
        myInstance->myVariable = (myInstance->myVariable) | value ; /* clear the bits 10-11 and set the value */
    }
}

/*Writes the binary representation of an integer to bits 2-7 of the myVariable field of a given myStruct instance.*/
void writeBinary2_7(struct myStruct *myInstance, int num) {
    int shiftedNum = num << 2; /* shift the number to the left by 1 to occupy bits 2-7 */
    myInstance->myVariable &= ~(0 << 2); /* clear bits 2-7 */
    myInstance->myVariable |= shiftedNum; /* set bits 2-7 to the binary representation of num */
}

/*Writes the binary representation of an integer to bits 8-13 of the myVariable field of a given myStruct instance.*/
void writeBinary8_13(struct myStruct *myInstance, int num) {
    int shiftedNum = num << 8; /* shift the number to the left by 8 to occupy bits 8-13 */
    myInstance->myVariable &= ~(0 << 8); /* clear bits 8-13 */
    myInstance->myVariable |= shiftedNum; /* set bits 8-13 to the binary representation of num */
}

/*Adds a myStruct instance to an array of myStruct instances.*/
void add_member(struct myStruct **array, int *size, struct myStruct *myInstance) {
    (*size)++; /* increase the size of the array */
    *array = realloc(*array, (*size) * sizeof(struct myStruct));
    (*array)[*size-1] = *myInstance; /* add the new value to the last element of the array */
}

