#ifndef MODULE__TOSTITOS
#define MODULE__TOSTITOS

#include "controlflowgraph.h"

#include <unordered_map>

namespace TosLang
{
    namespace BackEnd
    {
        using CFGPtr = std::shared_ptr<ControlFlowGraph>;
        using FunctionCFGs = std::unordered_map<std::string, CFGPtr>;
        
        /*
        * \class Module
        * \brief TODO
        */
        class Module
        {
        public:
            Module() : mCurrentMemorySlot{ 0 } { }

        public:
            using iterator = FunctionCFGs::iterator;
            using const_iterator = FunctionCFGs::const_iterator;

        public:
            iterator begin() { return mFuncCFGs.begin(); }
            iterator end() { return mFuncCFGs.end(); }
            const_iterator begin() const  { return mFuncCFGs.begin(); }
            const_iterator end() const { return mFuncCFGs.end(); }

        public:
            const CFGPtr& GetFunction(const std::string& name) const;

        public:
            /*
            * \fn           InsertFunction
            * \brief        Inserts a function in the module
            * \param name   The name of the function
            * \param cfg    The control flow graph associated with the function
            */
            void InsertFunction(const std::string& name, const CFGPtr& cfg);

            /*
            * \fn           InsertArrayVariable
            * \brief        Insert string variable in the Module
            * \param name   Name of the string
            * \param value  Value of the string
            * \return       Memory slot that the string was assigned
            */
            unsigned InsertArrayVariable(const std::string& name, const std::string& value);

        private:
            /*
            * \class IArrayVar
            * \brief Interface of array variable used as a facade by the Module
            */
            class IArrayVar
            {
            public:
                virtual ~IArrayVar() { }
            };

            /*
            * \class ArrayVar
            * \brief Encapsulate a a variable of array or string type. Since those can't be 
            *        put into a register, we need to identify them for the later stages of 
            *        code generation when we'll do the memory layout of the program.
            */
            template <typename T>
            class ArrayVar : public IArrayVar
            {
            public:
                using array_type = T;

            public:
                explicit ArrayVar(const std::string& value);
                explicit ArrayVar(const std::vector<T>& value);
                virtual ~ArrayVar() { }

            private:
                std::vector<T> mValues;
            };

        private:
            unsigned mCurrentMemorySlot;
            FunctionCFGs mFuncCFGs;
            std::unordered_map<std::string, std::unique_ptr<IArrayVar>> mArrayVars;
        };


        template<typename T>
        inline Module::ArrayVar<T>::ArrayVar(const std::string & value) : mValues(value.begin(), value.end()) { }

        template<typename T>
        inline Module::ArrayVar<T>::ArrayVar(const std::vector<T>& value) : mValues{ value } { }
    }
}

#endif // MODULE__TOSTITOS
