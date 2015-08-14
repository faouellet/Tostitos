#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace MachineEngine
{
	namespace ProcessorSpace
	{
		/**
		* \enum
		* \brief Masks for the possible errors that can happen during the initialization
		*/
		enum { NO_ERROR = 0, EMPTY_ROM_ERROR = 1, ROM_OVERFLOW_ERROR = 2, FILE_ERROR = 4 };

		/**
		* \enum
		* \brief Masks for the possible errors that can happen during the emulation
		*/
		enum { STACK_ERROR = 1, UNKNOWN_OP_ERROR = 2, EMULATION_DONE = 4 };

		/**
		* \enum
		* \brief Useful size constants
		* TODO: Should all of this be here?
		*/
		enum { HEADER_SIZE = 16, MEMORY_SIZE = 64 * 1024, STACK_START = 0xFDF0, STACK_END = 0xFFF0 };
	}
}

#endif // CONSTANTS_H

