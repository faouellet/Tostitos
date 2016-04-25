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

    Symbol intVarSymbol{ TosLang::Common::Type::NUMBER, 0, "MyIntVar" };
    BOOST_REQUIRE(symbolTable->IsVariableSymbolValid(intVarSymbol));

    Symbol boolVarSymbol{ TosLang::Common::Type::BOOL, 0, "MyBoolVar" };
    BOOST_REQUIRE(symbolTable->IsVariableSymbolValid(boolVarSymbol));

    Symbol strVarSymbol{ TosLang::Common::Type::STRING, 0, "MyStringVar" };
    BOOST_REQUIRE(symbolTable->IsVariableSymbolValid(strVarSymbol));
}

BOOST_AUTO_TEST_CASE( CollectSymbolFunctionNoParam )
{
    auto symbolTable = std::make_shared<TosLang::FrontEnd::SymbolTable>();
    size_t errorCount = GetProgramSymbolTable("../asts/function/fn_def_void.ast", symbolTable);
    BOOST_REQUIRE_EQUAL(errorCount, 0);

    Symbol fnSymbol{ TosLang::Common::Type::VOID, 0, "MyFunc" };
    BOOST_REQUIRE(symbolTable->IsFunctionSymbolValid(fnSymbol));
}

BOOST_AUTO_TEST_CASE( CollectSymbolFunctionMultiParam )
{
    auto symbolTable = std::make_shared<TosLang::FrontEnd::SymbolTable>();
    size_t errorCount = GetProgramSymbolTable("../asts/function/fn_def_multi_args.ast", symbolTable);
    BOOST_REQUIRE_EQUAL(errorCount, 0);

    Symbol fnSymbol{ TosLang::Common::Type::NUMBER, 0, "MyFunc" };
    BOOST_REQUIRE(symbolTable->IsFunctionSymbolValid(fnSymbol));

    auto paramTypes = fnSymbol.GetFunctionParamTypes();
    BOOST_REQUIRE(paramTypes.size() == 3);
    BOOST_REQUIRE(std::all_of(paramTypes.begin(), paramTypes.end(), [](const TosLang::Common::Type& t) { return t == TosLang::Common::Type::NUMBER; }));

    std::deque<size_t> scopeStack;
    scopeStack.push_front(0);
    scopeStack.push_front(1);

    for (size_t i = 0; i < 3; ++i)
    {
        std::stringstream sStream;
        sStream << "arg";
        sStream << i + 1;
        Symbol paramSym{ TosLang::Common::Type::NUMBER, 1, sStream.str() };
        BOOST_REQUIRE(symbolTable->IsVariableSymbolValid(paramSym));
    }
}

BOOST_AUTO_TEST_CASE( CollectSymbolFunctionWithLocalVar )
{
    auto symbolTable = std::make_shared<TosLang::FrontEnd::SymbolTable>();
    size_t errorCount = GetProgramSymbolTable("../asts/call/call_one_arg_var.ast", symbolTable);
    BOOST_REQUIRE_EQUAL(errorCount, 0);

    // Validate the identity function symbol
    std::vector<TosLang::Common::Type> identityTypes{ TosLang::Common::Type::NUMBER };
    Symbol identityFnSymbol{ identityTypes, 0, "identity" };
    BOOST_REQUIRE(symbolTable->IsFunctionSymbolValid(identityFnSymbol));
    
    // Validate the identity function parameter
    Symbol paramSym{ TosLang::Common::Type::NUMBER, 1, "i" };
    BOOST_REQUIRE(symbolTable->IsVariableSymbolValid(paramSym));
    
    // Validate the main function symbol
    std::vector<TosLang::Common::Type> mainTypes{ TosLang::Common::Type::VOID };
    Symbol mainFnSymbol{ mainTypes, 0, "main" };
    BOOST_REQUIRE(symbolTable->IsFunctionSymbolValid(mainFnSymbol));
    
    // Validate main's local variable
    Symbol mainVarSymbol{ TosLang::Common::Type::NUMBER, 2, "MyInt" };
    BOOST_REQUIRE(symbolTable->IsVariableSymbolValid(mainVarSymbol));
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
