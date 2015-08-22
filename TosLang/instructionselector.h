#ifndef INSTRUCTION_SELECTOR_H__TOSTITOS
#define INSTRUCTION_SELECTOR_H__TOSTITOS

#include "astvisitor.h"

#include "ruletable.h"

#include <map>

class InstructionSelector : ASTVisitor<InstructionSelector>
{
    friend class ASTVisitor<InstructionSelector>;

public:
    InstructionSelector() = default;
    ~InstructionSelector() = default;

public:
    std::vector<Instruction> Execute(const std::unique_ptr<ASTNode>& root);
        
protected:
    // Declarations
    void HandleProgramDecl();
    void HandleVarDecl();

    // Expressions
    void HandleBooleanExpr();
    void HandleNumberExpr();
    
private:
    std::map<const unsigned, Rule> mNodeLabels;
    std::vector<Instruction> mProgram;
};

#endif // INSTRUCTION_SELECTOR_H__TOSTITOS