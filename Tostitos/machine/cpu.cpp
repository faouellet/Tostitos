#include "cpu.h"
#include "instruction.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <regex>

using namespace MachineEngine::ProcessorSpace;

bool CPU::AcquireROM(const std::string & FilePath, std::shared_ptr<ThreadContext> & Context)
{
    std::ifstream FileStream(FilePath);
    if (!FileStream.is_open())
        return false;

    std::regex PushNumber("PUSH ([[:digit:]]+)");
    std::regex PushString("PUSH (.+)");
    std::smatch Match;
    std::string Line;
    while (std::getline(FileStream, Line))
    {
        if (std::regex_match(Line, Match, PushNumber))
        {
            Context->Program.emplace_back(std::stoi(Match[1]));
        }
        else if (std::regex_match(Line, Match, PushString))
        {
            size_t Index = std::find(mStrings.begin(), mStrings.end(), Match[1]) - mStrings.begin();
            if (Index == mStrings.size())
                mStrings.push_back(Match[1]);
            Context->Program.emplace_back(Index);
        }
        else
        {
            Context->Program.emplace_back(Line);
        }
    }

	Context->Memory.resize(8);

    return true;
}

ThreadAction CPU::InterpretOne()
{
    if (mCurrentContext->PC == mCurrentContext->Program.size())
        return ThreadAction::DONE;

    Instruction I = mCurrentContext->Program[mCurrentContext->PC++];
    ThreadAction CurrentAction = ThreadAction::RUN;

    switch (I.GetOptype())
    {
        case OpType::ADD:
        {
            Compute(std::plus<unsigned int>());
            break;
        }
        case OpType::DIV:
        {
            Compute(std::divides<unsigned int>());
            break;
        }
        case OpType::MUL:
        {
            Compute(std::multiplies<unsigned int>());
            break;
        }
        case OpType::SUB:
        {
            Compute(std::minus<unsigned int>());
            break;
        }
		case OpType::LOAD:
		{
			unsigned int Addr = mCurrentContext->Stack.top();
			mCurrentContext->Stack.pop();
			if (Addr < mCurrentContext->Memory.size())
				mCurrentContext->Stack.push(mCurrentContext->Memory[Addr]);
            break;
        }
        case OpType::STORE:
        {
            unsigned int Addr = mCurrentContext->Stack.top();
            mCurrentContext->Stack.pop();
            unsigned int Val = mCurrentContext->Stack.top();
            mCurrentContext->Stack.pop();
			if (Addr < mCurrentContext->Memory.size())
				mCurrentContext->Memory[Addr] = Val;
            break;
        }
        case OpType::PUSH:
        {
            mCurrentContext->Stack.push(I.GetValue());
            break;
        }
        case OpType::POP:
        {
            mCurrentContext->Stack.pop();
            break;
        }
        case OpType::FORK:
        {
            unsigned int FileID = mCurrentContext->Stack.top();
            mCurrentContext->Stack.pop();
            Filename = mStrings[FileID];
            CurrentAction = ThreadAction::FORK;
            break;
        }
        case OpType::BARRIER:
        {
            CurrentAction = ThreadAction::BARRIER;
            break;
        }
        case OpType::SLEEP:
        {
            CurrentAction = ThreadAction::SLEEP;
            break;
        }
        case OpType::EXP:
        {
            CurrentAction = ThreadAction::EXP;
            break;
        }
        case OpType::PRINT:
        {
            std::cout << mCurrentContext->Stack.top() << std::endl;
            break;
        }
    }

    return CurrentAction;
}

void CPU::Compute(std::function<unsigned int(unsigned int, unsigned int)> Func)
{
    unsigned int Operand1 = mCurrentContext->Stack.top();
    mCurrentContext->Stack.pop();
    unsigned int Operand2 = mCurrentContext->Stack.top();
    mCurrentContext->Stack.pop();
    mCurrentContext->Stack.push(Func(Operand1, Operand2));
}
