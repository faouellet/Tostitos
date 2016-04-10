#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE SymbolCollector
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "toslang_sema_fixture.h"

BOOST_FIXTURE_TEST_SUITE( SemaTestSuite, TosLangSemaFixture )

//////////////////// CORRECT USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( CollectSymbolVar )
{
    auto symbolTable = std::make_shared<TosLang::FrontEnd::SymbolTable>();
    size_t errorCount = GetProgramSymbolTable("../asts/var/var_decl.ast", symbolTable);
    BOOST_REQUIRE_EQUAL(errorCount, 0);

    Symbol varSymbol;

    BOOST_REQUIRE(symbolTable->GetGlobalSymbol("MyIntVar", varSymbol));
    BOOST_REQUIRE(!varSymbol.IsFunction());
    BOOST_REQUIRE(varSymbol.GetName() == "MyIntVar");
    BOOST_REQUIRE(varSymbol.GetScopeID() == 0); // At global scope
    BOOST_REQUIRE(varSymbol.GetVariableType() == TosLang::Common::Type::NUMBER);

    BOOST_REQUIRE(symbolTable->GetGlobalSymbol("MyBoolVar", varSymbol));
    BOOST_REQUIRE(!varSymbol.IsFunction());
    BOOST_REQUIRE(varSymbol.GetName() == "MyBoolVar");
    BOOST_REQUIRE(varSymbol.GetScopeID() == 0); // At global scope
    BOOST_REQUIRE(varSymbol.GetVariableType() == TosLang::Common::Type::BOOL);

    BOOST_REQUIRE(symbolTable->GetGlobalSymbol("MyStringVar", varSymbol));
    BOOST_REQUIRE(!varSymbol.IsFunction());
    BOOST_REQUIRE(varSymbol.GetName() == "MyStringVar");
    BOOST_REQUIRE(varSymbol.GetScopeID() == 0); // At global scope
    BOOST_REQUIRE(varSymbol.GetVariableType() == TosLang::Common::Type::STRING);
}

BOOST_AUTO_TEST_CASE( CollectSymbolFunctionNoParam )
{
    auto symbolTable = std::make_shared<TosLang::FrontEnd::SymbolTable>();
    size_t errorCount = GetProgramSymbolTable("../asts/function/fn_def_void.ast", symbolTable);
    BOOST_REQUIRE_EQUAL(errorCount, 0);

    Symbol fnSymbol;
    BOOST_REQUIRE(symbolTable->GetGlobalSymbol("MyFunc", fnSymbol));
    
    BOOST_REQUIRE(fnSymbol.IsFunction());
    BOOST_REQUIRE(fnSymbol.GetName() == "MyFunc");
    BOOST_REQUIRE(fnSymbol.GetScopeID() == 0); // At global scope
    BOOST_REQUIRE(fnSymbol.GetFunctionParamTypes().size() == 0);
    BOOST_REQUIRE(fnSymbol.GetFunctionReturnType() == TosLang::Common::Type::VOID);
}

BOOST_AUTO_TEST_CASE( CollectSymbolFunctionMultiParam )
{
    auto symbolTable = std::make_shared<TosLang::FrontEnd::SymbolTable>();
    size_t errorCount = GetProgramSymbolTable("../asts/function/fn_def_multi_args.ast", symbolTable);
    BOOST_REQUIRE_EQUAL(errorCount, 0);

    Symbol fnSymbol;
    BOOST_REQUIRE(symbolTable->GetGlobalSymbol("MyFunc", fnSymbol));

    BOOST_REQUIRE(fnSymbol.IsFunction());
    BOOST_REQUIRE(fnSymbol.GetName() == "MyFunc");
    BOOST_REQUIRE(fnSymbol.GetScopeID() == 0); // At global scope
    BOOST_REQUIRE(fnSymbol.GetFunctionReturnType() == TosLang::Common::Type::NUMBER);

    auto paramTypes = fnSymbol.GetFunctionParamTypes();
    BOOST_REQUIRE(paramTypes.size() == 3);
    BOOST_REQUIRE(std::all_of(paramTypes.begin(), paramTypes.end(), [](const TosLang::Common::Type& t) { return t == TosLang::Common::Type::NUMBER; }));

    std::stack<size_t> scopeStack;
    scopeStack.push(0);
    scopeStack.push(1);

    for (size_t i = 0; i < 3; ++i)
    {
        Symbol paramSym;
        std::stringstream sStream;
        sStream << "arg";
        sStream << i + 1;
        BOOST_REQUIRE(symbolTable->GetLocalSymbol("MyFunc", sStream.str(), scopeStack, paramSym));
        
        BOOST_REQUIRE(!paramSym.IsFunction());
        BOOST_REQUIRE(paramSym.GetName() == sStream.str());
        BOOST_REQUIRE(paramSym.GetScopeID() == 1); // At function scope
        BOOST_REQUIRE(paramSym.GetVariableType() == TosLang::Common::Type::NUMBER);
    }
}

BOOST_AUTO_TEST_CASE( CollectSymbolFunctionWithLocalVar )
{
    auto symbolTable = std::make_shared<TosLang::FrontEnd::SymbolTable>();
    size_t errorCount = GetProgramSymbolTable("../asts/call/call_one_arg_var.ast", symbolTable);
    BOOST_REQUIRE_EQUAL(errorCount, 0);

    Symbol fnSymbol;
    
    // Validate the identity function symbol
    BOOST_REQUIRE(symbolTable->GetGlobalSymbol("identity", fnSymbol));
    BOOST_REQUIRE(fnSymbol.IsFunction());
    BOOST_REQUIRE(fnSymbol.GetName() == "identity");
    BOOST_REQUIRE(fnSymbol.GetScopeID() == 0); // At global scope
    BOOST_REQUIRE(fnSymbol.GetFunctionParamTypes().size() == 1);
    BOOST_REQUIRE(fnSymbol.GetFunctionReturnType() == TosLang::Common::Type::NUMBER);

    Symbol paramSym;
    std::stack<size_t> scopeStack;
    scopeStack.push(0); // Global scope ID
    scopeStack.push(1); // First defined function scope ID

    // Validate the identity function parameter
    BOOST_REQUIRE(symbolTable->GetLocalSymbol("identity", "i", scopeStack, paramSym));
    BOOST_REQUIRE(!paramSym.IsFunction());
    BOOST_REQUIRE(paramSym.GetName() == "i");
    BOOST_REQUIRE(paramSym.GetScopeID() == 1); // At function scope
    BOOST_REQUIRE(paramSym.GetVariableType() == TosLang::Common::Type::NUMBER);

    // Validate the main function symbol
    BOOST_REQUIRE(symbolTable->GetGlobalSymbol("main", fnSymbol));
    BOOST_REQUIRE(fnSymbol.IsFunction());
    BOOST_REQUIRE(fnSymbol.GetName() == "main");
    BOOST_REQUIRE(fnSymbol.GetScopeID() == 0); // At global scope
    BOOST_REQUIRE(fnSymbol.GetFunctionParamTypes().size() == 0);
    BOOST_REQUIRE(fnSymbol.GetFunctionReturnType() == TosLang::Common::Type::VOID);

    scopeStack.pop();   // Getting out of the identity function
    scopeStack.push(2); // Main function scope ID

    // Validate main's local variable
    BOOST_REQUIRE(symbolTable->GetLocalSymbol("main", "MyInt", scopeStack, paramSym));
    BOOST_REQUIRE(!paramSym.IsFunction());
    BOOST_REQUIRE(paramSym.GetName() == "MyInt");
    BOOST_REQUIRE(paramSym.GetScopeID() == 2); // At function scope
    BOOST_REQUIRE(paramSym.GetVariableType() == TosLang::Common::Type::NUMBER);
}

BOOST_AUTO_TEST_CASE( CollectSymbolNoErrorAccumulation )
{
    std::unique_ptr<ASTNode> rootNode = reader.Run("../asts/function/fn_def_void.ast");
    BOOST_REQUIRE(rootNode != nullptr);

    auto symbolTable = std::make_shared<TosLang::FrontEnd::SymbolTable>();
    TosLang::FrontEnd::SymbolCollector sCollector{ symbolTable };
    size_t errorCount = sCollector.Run(rootNode);
    BOOST_REQUIRE_EQUAL(errorCount, 0);

    errorCount = sCollector.Run(rootNode);
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

//////////////////// ERROR USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( CollectSymbolGlobalVarRedefinition )
{
    auto symbolTable = std::make_shared<TosLang::FrontEnd::SymbolTable>();
    size_t errorCount = GetProgramSymbolTable("../asts/var/bad_global_var_redef.ast", symbolTable);
    BOOST_REQUIRE_EQUAL(errorCount, 1);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "VAR ERROR: Trying to redefine an already defined variable at line 2, column 8");
}

BOOST_AUTO_TEST_CASE( CollectSymbolLocalVarRedefinition )
{
    auto symbolTable = std::make_shared<TosLang::FrontEnd::SymbolTable>();
    size_t errorCount = GetProgramSymbolTable("../asts/var/bad_local_var_redef.ast", symbolTable);
    BOOST_REQUIRE_EQUAL(errorCount, 1);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "VAR ERROR: Trying to redefine an already defined variable at line 3, column 5");
}

BOOST_AUTO_TEST_CASE( CollectSymbolFunctionRedefinition )
{
    auto symbolTable = std::make_shared<TosLang::FrontEnd::SymbolTable>();
    size_t errorCount = GetProgramSymbolTable("../asts/function/bad_fn_redef.ast", symbolTable);
    BOOST_REQUIRE_EQUAL(errorCount, 1);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "FUNCTION ERROR: Trying to redefine an already defined function at line 3, column 8");
}

BOOST_AUTO_TEST_CASE( CollectSymbolFunctionParamRedefinition )
{
    std::unique_ptr<ASTNode> rootNode = reader.Run("../asts/function/bad_fn_param_redef.ast");
    BOOST_REQUIRE(rootNode != nullptr);

    auto symbolTable = std::make_shared<TosLang::FrontEnd::SymbolTable>();
    TosLang::FrontEnd::SymbolCollector sCollector{ symbolTable };
    size_t errorCount = sCollector.Run(rootNode);
    BOOST_REQUIRE_EQUAL(errorCount, 1);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "VAR ERROR: Trying to redefine an already defined function parameter at line 1, column 16");
}

BOOST_AUTO_TEST_SUITE_END()
