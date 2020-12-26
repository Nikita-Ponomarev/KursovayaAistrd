#include "pch.h"
#include "CppUnitTest.h"
#include <stdexcept>
#include "../KursovayaAistrd/Header.h"
#include "..\KursovayaAistrd\Source.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest4
{
	TEST_CLASS(UnitTest4)
	{
	public:
		TEST_METHOD(wrong_input_checking)
		{
			Str_eq equ;
			equ.eq.push_back('+');
			equ.eq.push_back(' ');
			equ.eq.push_back('3');
			equ.eq.push_back('7');
			equ.eq.push_back('*');
			equ.eq.push_back(' ');
			equ.eq.push_back('5');
			equ.eq.push_back('6');
			equ.eq.push_back(' ');
			equ.eq.push_back('7');

			Assert::AreEqual(true, equ.wrong_input_checking());
		}

		TEST_METHOD(counting_postfix)
		{
			Str_eq equ;
			equ.eq.push_back('3');
			equ.eq.push_back('7');
			equ.eq.push_back('*');
			equ.eq.push_back('5');
			equ.eq.push_back('6');
			equ.stack_to_postfix();
			double answer = 2072;
			Assert::AreEqual(answer, equ.counting_postfix());
		}

	};
}

