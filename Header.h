#pragma once
#include <iomanip>
#include <iostream>
#include <locale>
#include <string>
using namespace std;

class Str_eq
{
	char sym;
	double value;
	class Str_eq* stack;
	class Str_eq* next;
public:
	bool except;
	string eq;
	Str_eq()
	{
		stack = NULL;
		except = false;
	}
	void eq_in();
	void stack_to_postfix();
	bool ifoperator(char s);
	bool ifoperand(char s);
	int priorityity(char s);
	double counting_postfix();
	bool wrong_input_checking();
};
