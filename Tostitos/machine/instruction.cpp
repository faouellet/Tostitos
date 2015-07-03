#include "instruction.h"

using namespace MachineEngine::ProcessorSpace;

const char * Instruction::OP_CODES[] = {
	"ADD", "DIV", "MUL", "SUB", 
	"LOAD", "STORE", 
	"PUSH", "POP", 
	"FORK", "BARRIER", "SLEEP",
	"EXP", "PRINT" };
