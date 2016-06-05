#ifndef MODULE__TOSTITOS
#define MODULE__TOSTITOS

#include "controlflowgraph.h"

#include "../Utils/cfgprinter.h"

#include <unordered_map>

namespace TosLang
{
    namespace BackEnd
    {
        template <class InstT>
        using CFGPtr = std::shared_ptr<ControlFlowGraph<InstT>>;

        template <class InstT>
        using FunctionCFGs = std::unordered_map<std::string, CFGPtr<InstT>>;
        
        /*
        * \class Module
        * \brief TODO
        */
        template <class InstT>
        class Module
        {
        public:
            using iterator = typename FunctionCFGs<InstT>::iterator;
            using const_iterator = typename FunctionCFGs<InstT>::const_iterator;

        public:
            iterator begin() { return mFuncCFGs.begin(); }
            iterator end() { return mFuncCFGs.end(); }
            const_iterator begin() const  { return mFuncCFGs.begin(); }
            const_iterator end() const { return mFuncCFGs.end(); }

        public:
            Module()
            {
                mGlobalBlock.reset(new BasicBlock<InstT>{"Global"});
            }
            
        public:
            //const CFGPtr<InstT>& GetFunction(const std::string& name) const
            //{
            //    auto cfgIt = mFuncCFGs.find(name);
            //    if (cfgIt != mFuncCFGs.end())
            //        return cfgIt->second;
            //    else
            //        return nullptr;
            //}

        public:
            /*
            * \fn           Print
            * \brief        Prints the module to a stream
            * \param name   Output stream
            */
            template <typename OS>
            void Print(OS& stream) const
            {
                // TODO: Print global variables
                stream << "Globals" << std::endl;
                for (auto instIt = mGlobalBlock->inst_begin(), instEnd = mGlobalBlock->inst_end(); instIt != instEnd; ++instIt)
                    stream << **instIt << std::endl;

                // Print the CFGs
                Utils::CFGPrinter<OS, InstT> printer{ stream };
                for (auto& funcCFG : mFuncCFGs)
                {
                    stream << "CFG for " << funcCFG.first << ":\n";
                    printer.Visit(funcCFG.second, /*postOrderVisit=*/false);
                    stream << std::endl;
                }
            }

        public:
            /*
            * \fn           InsertFunction
            * \brief        Inserts a function in the module
            * \param name   The name of the function
            * \param cfg    The control flow graph associated with the function
            */
            void InsertFunction(const std::string& name, const CFGPtr<InstT>& cfg)
            {
                mFuncCFGs[name] = cfg;
            }

            /*
            * \fn           InsertGlobalVar
            * \brief        Inserts a global variable in the module
            * \param name   The name of the variable
            */
            const InstT* InsertGlobalVar(const InstT& inst)
            {
                mGlobalBlock->InsertInstruction(inst);
                return mGlobalBlock->GetTerminator();
            }

        private:
            FunctionCFGs<InstT> mFuncCFGs;  /*!< Functions in the translation unit. TODO: First == globals container? */
            BlockPtr<InstT> mGlobalBlock;          /*!< TODO */
        };
    }
}

#endif // MODULE__TOSTITOS
