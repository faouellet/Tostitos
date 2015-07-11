#ifndef UTILS_H
#define UTILS_H

namespace MachineEngine
{
	namespace ProcessorSpace
	{
		/**
		* \namespace Utils
		* \brief Contains a mix of useful templated tools.
		*        Credits goes to Patrice Roy for teaching me some of these tools
		*        His website : <a href="http://h-deb.clg.qc.ca/">h-deb</a>
		*/
		namespace Utils
		{
			/**
			* \struct IfElse
			* \brief Defines a type depending on the condition by using partial specialization
			*/
			template <bool Cond, class If, class Else>
			struct IfElse;

			template <class If, class Else>
			struct IfElse<true, If, Else>
			{
				typedef If type;
			};

			template <class If, class Else>
			struct IfElse<false, If, Else>
			{
				typedef Else type;
			};

			/**
			* \class Error
			* \brief Dummy class
			*/
			class Error;

			/**
			* \struct UInt_
			* \brief Defines an unsigned type that will at most contain the specified
			*        number of bytes
			*/
			template<int N>
			struct UInt_
			{
				typedef typename IfElse<
					sizeof(char) == N, unsigned char,
					typename IfElse<
					sizeof(short) == N, unsigned short,
					typename IfElse<
					sizeof(int) == N, unsigned int,
					typename IfElse<
					sizeof(long) == N, unsigned long,
					typename IfElse<
					sizeof(long long) == N, unsigned long long,
					Error
					>::type
					>::type
					>::type
					>::type
				>::type type;
			};

			/**
			* \struct UInt_
			* \brief Defines an signed type that will at most contain the specified
			*        number of bytes
			*/
			template<int N>
			struct Int_
			{
				typedef typename IfElse<
					sizeof(char) == N, char,
					typename IfElse<
					sizeof(short) == N, short,
					typename IfElse<
					sizeof(int) == N, int,
					typename IfElse<
					sizeof(long) == N, long,
					typename IfElse<
					sizeof(long long) == N, long long,
					Error
					>::type
					>::type
					>::type
					>::type
				>::type type;
			};

			typedef UInt_<1>::type UInt8;	/**< Unsigned 8 bits integer type */
			typedef UInt_<2>::type UInt16;	/**< Unsigned 16 bits integer type */
			typedef UInt_<4>::type UInt32;	/**< Unsigned 32 bits integer type */

			typedef Int_<2>::type Int16;	/**< Signed 16 bits integer type */
		};
	}
}

#endif // UTILS_H
