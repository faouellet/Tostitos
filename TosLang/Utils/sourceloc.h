#ifndef SOURCE_LOC_H__TOSTITOS
#define SOURCE_LOC_H__TOSTITOS

namespace TosLang
{
    namespace Utils
    {
        /*
        * \class SourceLocation
        * \brief Contains everything there is to know about the current file being processed by the compiler
        */
        class SourceLocation
        {
        public:
            /*
            * \fn       SourceLocation
            * \brief    Constructor
            */
            SourceLocation() : mCurrentLine{ 0 }, mCurrentColumn{ 0 } { }

            /*
            * \fn           SourceLocation
            * \brief        Constructor
            * \param line   Line number in the source file (1-based)
            * \param column Column number in the source file (1-based)
            */
            SourceLocation(const unsigned line, const unsigned column) : mCurrentLine{ line }, mCurrentColumn { column } { }

        public:
            /*
            * \fn       Init
            * \brief    Initializes the SourceLocation to a valid state representing the start of a file
            */
            void Init();

            /*
            * \fn               Advance
            * \brief            Advances the SourceLocation to in the source file
            * \param nextLine   Will advancing change the current line?
            */
            void Advance(bool nextLine = false);

        public:
            /*
            * \fn       GetCurrentColumn
            * \brief    Gives the current number
            * \return   The current column
            */
            const unsigned GetCurrentColumn() const { return mCurrentColumn; }

            /*
            * \fn       GetCurrentLine
            * \brief    Gives the current number
            * \return   The current line
            */
            const unsigned GetCurrentLine() const { return mCurrentLine; }

        private:
            unsigned mCurrentLine;      /*!< Current line in the file the lexer is at */
            unsigned mCurrentColumn;    /*!< Current column in the file the lexer is at */

            // TODO: std::string mFilename ??
        };
    }
}

#endif // SOURCE_LOC_H__TOSTITOS
