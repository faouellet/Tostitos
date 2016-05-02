#ifndef TOSLANG_CODEGEN_FIXTURE_H__TOSTITOS
#define TOSLANG_CODEGEN_FIXTURE_H__TOSTITOS

#include "AST/ast.h"
//#include "CodeGen/instructionselector.h"
#include "Sema/symbolcollector.h"
#include "Utils/astreader.h"
#include "Utils/cfgprinter.h"
#include "testutils.h"

#include <memory>

using namespace TosLang::FrontEnd;

/*
* \struct TosLangFixture
* \brief  Fixture used to test the semantic passes of the TosLang compiler
*/
struct TosLangCodegenFixture
{
    /*
    * \fn               GetProgramSymbolTable
    * \brief            Parse a TosLang program and collect the symbols contained in the resulting AST
    * \param filename   Name of a file containing a TosLang program
    * \param symTable   The symbol table to be filled
    * \return           The number of errors that happened during symbols collection
    */
    const size_t GetProgramSymbolTable(const std::string& filename, std::shared_ptr<SymbolTable>& symTable)
    {
        programAST = reader.Run(filename);
        BOOST_REQUIRE(programAST != nullptr);

        TosLang::FrontEnd::SymbolCollector sCollector{ symTable };
        return sCollector.Run(programAST);
    }

    /*
    * \fn               GenerateProgramCFG
    * \brief            Generates the CFG corresponding to a TosLang program and outputs it to a file
    * \param inputFile  Name of a file containing a TosLang AST
    * \param outputFile Name of the file to output the TosLang CFG
    */
    void GenerateProgramCFG(const std::string& inputFile, const std::string&)
    {
        auto symTable = std::make_shared<SymbolTable>();
        size_t errorCount = GetProgramSymbolTable(inputFile, symTable);
        BOOST_REQUIRE_EQUAL(errorCount, 0);

        /*TosLang::BackEnd::InstructionSelector iSel;
        auto module = iSel.Run(programAST, symTable);

        std::ofstream printStream(outputFile);
        module->Print(printStream);*/
    }

    std::unique_ptr<ASTNode> programAST;    /*!< Program abstract syntax tree */
    TosLang::Utils::ASTReader reader;       /*!< Use to acquire ASTs stored in files */
};

#endif // TOSLANG_CODEGEN_FIXTURE_H__TOSTITOS
