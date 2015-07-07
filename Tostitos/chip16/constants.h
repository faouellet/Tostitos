#ifndef CONSTANTS_H
#define CONSTANTS_H

/**
* \enum
* \brief Masks for the possible errors that can happen during the initialization
*/
enum { NoError = 0, EmptyROMError = 1, ROMOverflowError = 2, FileError = 4 };

/**
* \enum
* \brief Masks for the possible errors that can happen during the emulation
*/
enum { StackError = 1, UnknownOpError = 2, EmulationDone = 4 };

/**
* \enum
* \brief Useful size constants
* TODO: Should all of this be here?
*/
enum { HEADER_SIZE = 16, MEMORY_SIZE = 64 * 1024, STACK_START = 0xFDF0 };


#endif // CONSTANTS_H

