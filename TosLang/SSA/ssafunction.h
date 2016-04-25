#ifndef SSA_FUNCTION__TOSTITOS
#define SSA_FUNCTION__TOSTITOS

#include "ssainstruction.h"
#include "../CFG/controlflowgraph.h"

#include <cassert>

namespace TosLang
{
    namespace BackEnd
    {
        class SSAFunction : public ControlFlowGraph<SSAInstruction>
        {
        public:
            /*
            * \fn SSAFunction
            * \brief Dtor
            */
            virtual ~SSAFunction() = default;

        public:
            friend std::ostream& operator<<(std::ostream& stream, const SSAFunction& op);

        public:
            /*
            * \fn           AddArguments
            * \brief        Adds an argument to the function
            * \param val    Argument to be added
            */
            void AddArguments(const SSAValue& val) { mArguments.push_back(val); }

            /*
            * \fn           GetArgument
            * \brief        Fetch the argument at the given index
            * \param idx    Index of the argument
            * \return       Function argument
            */
            SSAValue& GetArgument(const size_t idx) { assert(idx < mArguments.size()); return mArguments[idx]; }

            /*
            * \fn           GetNbArguments
            * \brief        Gets the number of arguments of the function
            * \return       Number of arguments of the function
            */
            size_t GetNbArguments() const { return mArguments.size(); }

        private:
            std::vector<SSAValue> mArguments;   /*!< Function's arguments */
        };
    }
}

#endif // SSA_FUNCTION__TOSTITOS
