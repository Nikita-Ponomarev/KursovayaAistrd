#include "Header.h"
#include <iomanip>
#include <iostream>
#include <locale>
#include <math.h>
using namespace std;
int main()
{
	int choice;
	Str_eq eq;
start:
	cout << "***************************************\n";
	cout << "Enter '1' to calculate or '2' to exit: \n";
	cout << "***************************************\n";
	cin >> choice;
	cin.clear();
	cin.ignore(cin.rdbuf()->in_avail());
	switch (choice)
	{
	case 1:
	{
		eq.eq_in();
		if (!eq.wrong_input_checking())
		{
			eq.stack_to_postfix();
			if (eq.except == false)
			{
				cout << "Result:\n" << eq.counting_postfix() << endl;
			}
		}
		goto start;
	}
	case 2:
	{
		exit(0);
	}
	default:
		goto start;
	}
}
void Str_eq::eq_in()
{
	cout << "Enter the equation: " << endl;
	getline(cin, eq);
}
bool Str_eq::wrong_input_checking()
{
	string temp;
	temp.empty();
	for (int c = 0; c < eq.size(); c++)
	{
		while (eq[c] != ' ')
		{
			temp += eq[c];
			c++;
			if (c == eq.size())
			{
				break;
			}
		}
	}
	if (ifoperator(temp[0]) && temp[0] != '-' && temp[0]!='(')
	{
		cout << "Error! Invalid symbol: " << temp[1] << endl;
		return true;
	}
	if (temp[0] == '-' && !ifoperand(temp[1]))
	{
		cout << "Error! Invalid symbol: " << temp[2] << endl;
		return true;
	}
	for (int c = 0; c < temp.size(); c++)
	{
		if (ifoperator(temp[c]) && temp[c] != '(' && temp[c] != ')')
		{
			if (ifoperator(temp[c + 1]) && temp[c + 1] != '-' && temp[c + 1] != '(' && temp[c + 1] != ')')
			{
				cout << "Error! Invalid symbol: " << temp[c] << endl;
				return true;
			}
			if (ifoperator(temp[c + 1]) && temp[c + 1] == '-' && (!ifoperand(temp[c + 2]) && temp[c + 2] != 'e' && (temp[c + 2] != 'p' && temp[c + 3] != 'i')))
			{
				cout << "Error! Invalid symbol: " << temp[c] << endl;
				return true;
			}
			if (c + 1 == temp.size() && temp[c] != '(' && temp[c] != ')')
			{
				cout << "Error! Invalid symbol: " << temp[c] << endl;
				return true;
			}
			if (ifoperand(temp[c + 1]) && c > 0 && ifoperand(temp[c - 1]))
			{
				temp.insert(c + 1, " ");
			}
			if (temp[c] == '-' && c > 0 && ifoperand(temp[c + 1]) && (temp[c - 1] == '(' || temp[c - 1] == ')'))
			{
				temp.insert(c + 1, " ");
			}
		}
		if (c + 1 == temp.size() && temp[c] == '(')
		{
			cout << "Error! Invalid symbol: " << temp[c] << endl;
			return true;
		}
		if (ifoperand(temp[c]))
		{
			while (temp[c] == '.' || ifoperand(temp[c]))
			{
				if (c == temp.size())
				{
					break;
				}
				if (temp[c] == '.' && temp[c + 1] == '.')
				{
					cout << "Error! Invalid symbol: " << temp[c] << endl;
					return true;
				}
				c++;
			}
			if (!ifoperator(temp[c]) && temp[c] != ' ')
			{
				if (c < temp.size())
				{
					cout << "Error! Invalid symbol: " << temp[c] << endl;
					return true;
				}
			}
			c--;
		}	
	}
	eq.empty();
	eq = temp;
	return false;
}
void Str_eq::stack_to_postfix()
{
	string temp;
	temp.empty();
	Str_eq* prev;
	prev = stack;
	for (int c = 0; c<eq.size(); c++)
	{
		if (eq[c] == ' ')
			continue;
		else if (ifoperand(eq[c]))
		{
			while (!ifoperator(eq[c]) && eq[c] != ' ')
			{
				temp += eq[c];
				c++;
				if (c == eq.size())
				{
					break;
				}
			}
			temp += ' ';
			c--;
		}
		else if (ifoperator(eq[c]))
		{
			if (eq[c] == '(')
			{
				prev = new Str_eq;
				prev->next = stack;
				stack = prev;
				stack->sym = eq[c];
			}
			else if (eq[c] == ')')
			{
				while (stack->sym != '(')
				{
					temp += stack->sym;
					temp += ' ';
					stack = stack->next;
				}
				if (stack->sym == '(')
				{
					stack = stack->next;
				}
			}
			else if (eq[c] == '-' && ifoperand(eq[c + 1]))
			{
				temp += '-';
				c++;
				while (!ifoperator(eq[c]) && eq[c] != ' ')
				{
					temp += eq[c];
					c++;
					if (c == eq.size())
					{
						break;
					}
				}
				temp += ' ';
				c--;
			}
			else
			{
				if (stack != NULL)
				{
					if (priorityity(eq[c]) <= priorityity(stack->sym))
					{
						while (priorityity(eq[c]) <= priorityity(stack->sym))
						{
							temp += stack->sym;
							temp += ' ';
							stack = stack->next;
							if (stack == NULL)
								break;
						}
						prev = new Str_eq;
						prev->next = stack;
						stack = prev;
						stack->sym = eq[c];
					}
					else
					{
						prev = new Str_eq;
						prev->next = stack;
						stack = prev;
						stack->sym = eq[c];
					}
				}
				else
				{
					prev = new Str_eq;
					prev->next = stack;
					stack = prev;
					stack->sym = eq[c];
				}
			}
		}
		else if (eq[c] == 'p' && eq[c+1]=='i')
		{
			for (int i = 0; i < 2; i++)
			{
				temp += eq[c];
				c++;
			}
			temp += ' ';
			c--;
		}
		else if (eq[c] == 'e')
		{
			temp += eq[c];
			temp += ' ';
		}
		else if (eq[c] == 'c' && eq[c + 1] == 'o' && eq[c+2]=='s')
		{
			if (ifoperand(eq[c + 3]))
			{
				for (int i = 0; i < 3; i++)
				{
					temp += eq[c];
					c++;
				}
				while (!ifoperator(eq[c]) && eq[c] != ' ')
				{
					temp += eq[c];
					c++;
					if (c == eq.size())
					{
						break;
					}
				}
				temp += ' ';
				c--;
			}
			else if (eq[c+3] == '-' && ifoperand(eq[c + 4]))
			{
				for (int i = 0; i < 3; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += '-';
				c++;
				while (!ifoperator(eq[c]) && eq[c] != ' ')
				{
					temp += eq[c];
					c++;
					if (c == eq.size())
					{
						break;
					}
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 3] == '-' && eq[c + 4] == 'p' && eq[c + 5] == 'i')
			{
				for (int i = 0; i < 6; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 3] == '-' && eq[c + 4] == 'e')
			{
				for (int i = 0; i < 5; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 3] == 'p' && eq[c + 4] == 'i')
			{
				for (int i = 0; i < 5; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 3] == 'e')
			{
				for (int i = 0; i < 4; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += ' ';
				c--;
			}
			else
			{
				cout << "Error! Invalid symbol: " << eq[c] << endl;
				except = true;
				stack = prev;
				while (stack != NULL)
				{
					stack = stack->next;
					delete prev;
					prev = stack;
				}
				return;
			}
		}
		else if (eq[c] == 's' && eq[c + 1] == 'i' && eq[c + 2] == 'n')
		{
			if (ifoperand(eq[c + 3]))
			{
				for (int i = 0; i < 3; i++)
				{
					temp += eq[c];
					c++;
				}
				while (!ifoperator(eq[c]) && eq[c] != ' ')
				{
					temp += eq[c];
					c++;
					if (c == eq.size())
					{
						break;
					}
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 3] == '-' && ifoperand(eq[c + 4]))
			{
				for (int i = 0; i < 3; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += '-';
				c++;
				while (!ifoperator(eq[c]) && eq[c] != ' ')
				{
					temp += eq[c];
					c++;
					if (c == eq.size())
					{
						break;
					}
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 3] == '-' && eq[c + 4] == 'p' && eq[c + 5] == 'i')
			{
				for (int i = 0; i < 6; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 3] == '-' && eq[c + 4] == 'e')
			{
				for (int i = 0; i < 5; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 3] == 'p' && eq[c + 4] == 'i')
			{
				for (int i = 0; i < 5; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 3] == 'e')
			{
				for (int i = 0; i < 4; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += ' ';
				c--;
			}
			else
			{
				cout << "Error! Invalid symbol: " << eq[c] << endl;
				except = true;
				stack = prev;
				while (stack != NULL)
				{
					stack = stack->next;
					delete prev;
					prev = stack;
				}
				return;
			}
		}
		else if (eq[c] == 't' && eq[c + 1] == 'g')
		{
			if (ifoperand(eq[c + 2]))
			{
				for (int i = 0; i < 2; i++)
				{
					temp += eq[c];
					c++;
				}
				while (!ifoperator(eq[c]) && eq[c] != ' ')
				{
					temp += eq[c];
					c++;
					if (c == eq.size())
					{
						break;
					}
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 2] == '-' && ifoperand(eq[c + 3]))
			{
				for (int i = 0; i < 2; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += '-';
				c++;
				while (!ifoperator(eq[c]) && eq[c] != ' ')
				{
					temp += eq[c];
					c++;
					if (c == eq.size())
					{
						break;
					}
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 2] == '-' && eq[c + 3] == 'p' && eq[c + 4] == 'i')
			{
				for (int i = 0; i < 5; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 2] == '-' && eq[c + 3] == 'e')
			{
				for (int i = 0; i < 4; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 3] == 'p' && eq[c + 4] == 'i')
			{
				for (int i = 0; i < 4; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 2] == 'e')
			{
				for (int i = 0; i < 3; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += ' ';
				c--;
			}
			else
			{
				cout << "Error! Invalid symbol: " << eq[c] << endl;
				except = true;
				stack = prev;
				while (stack != NULL)
				{
					stack = stack->next;
					delete prev;
					prev = stack;
				}
				return;
			}
		}
		else if (eq[c] == 'c' && eq[c + 1] == 't' && eq[c + 2] == 'g')
		{
			if (ifoperand(eq[c + 3]))
			{
				for (int i = 0; i < 3; i++)
				{
					temp += eq[c];
					c++;
				}
				while (!ifoperator(eq[c]) && eq[c] != ' ')
				{
					temp += eq[c];
					c++;
					if (c == eq.size())
					{
						break;
					}
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 3] == '-' && ifoperand(eq[c + 4]))
			{
				for (int i = 0; i < 3; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += '-';
				c++;
				while (!ifoperator(eq[c]) && eq[c] != ' ')
				{
					temp += eq[c];
					c++;
					if (c == eq.size())
					{
						break;
					}
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 3] == '-' && eq[c + 4] == 'p' && eq[c + 5] == 'i')
			{
				for (int i = 0; i < 6; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 3] == '-' && eq[c + 4] == 'e')
			{
				for (int i = 0; i < 5; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 3] == 'p' && eq[c + 4] == 'i')
			{
				for (int i = 0; i < 5; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 3] == 'e')
			{
				for (int i = 0; i < 4; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += ' ';
				c--;
			}
			else
			{
				cout << "Error! Invalid symbol: " << eq[c] << endl;
				except = true;
				stack = prev;
				while (stack != NULL)
				{
					stack = stack->next;
					delete prev;
					prev = stack;
				}
				return;
			}
		}
		else if (eq[c] == 'l' && eq[c + 1] == 'n')
		{
			if (ifoperand(eq[c + 2]))
			{
				for (int i = 0; i < 2; i++)
				{
					temp += eq[c];
					c++;
				}
				while (!ifoperator(eq[c]) && eq[c] != ' ')
				{
					temp += eq[c];
					c++;
					if (c == eq.size())
					{
						break;
					}
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 2] == '-' && ifoperand(eq[c + 3]))
			{
				for (int i = 0; i < 2; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += '-';
				c++;
				while (!ifoperator(eq[c]) && eq[c] != ' ')
				{
					temp += eq[c];
					c++;
					if (c == eq.size())
					{
						break;
					}
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 2] == '-' && eq[c + 3] == 'p' && eq[c + 4] == 'i')
			{
				for (int i = 0; i < 5; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 2] == '-' && eq[c + 3] == 'e')
			{
				for (int i = 0; i < 4; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 3] == 'p' && eq[c + 4] == 'i')
			{
				for (int i = 0; i < 4; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 2] == 'e')
			{
				for (int i = 0; i < 3; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += ' ';
				c--;
			}
			else
			{
				cout << "Error! Invalid symbol: " << eq[c] << endl;
				except = true;
				stack = prev;
				while (stack != NULL)
				{
					stack = stack->next;
					delete prev;
					prev = stack;
				}
				return;
			}
		}
		else if (eq[c] == 'l' && eq[c + 1] == 'o' && eq[c + 2] == 'g')
		{
			if (ifoperand(eq[c + 3]))
			{
				for (int i = 0; i < 3; i++)
				{
					temp += eq[c];
					c++;
				}
				while (!ifoperator(eq[c]) && eq[c] != ' ')
				{
					temp += eq[c];
					c++;
					if (c == eq.size())
					{
						break;
					}
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 3] == '-' && ifoperand(eq[c + 4]))
			{
				for (int i = 0; i < 3; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += '-';
				c++;
				while (!ifoperator(eq[c]) && eq[c] != ' ')
				{
					temp += eq[c];
					c++;
					if (c == eq.size())
					{
						break;
					}
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 3] == '-' && eq[c + 4] == 'p' && eq[c + 5] == 'i')
			{
				for (int i = 0; i < 6; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 3] == '-' && eq[c + 4] == 'e')
			{
				for (int i = 0; i < 5; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 3] == 'p' && eq[c + 4] == 'i')
			{
				for (int i = 0; i < 5; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 3] == 'e')
			{
				for (int i = 0; i < 4; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += ' ';
				c--;
			}
			else
			{
				cout << "Error! Invalid symbol: " << eq[c] << endl;
				except = true;
				stack = prev;
				while (stack != NULL)
				{
					stack = stack->next;
					delete prev;
					prev = stack;
				}
				return;
			}
		}
		else if (eq[c] == 's' && eq[c + 1] == 'q' && eq[c + 2] == 'r' && eq[c+3]=='t')
		{
			if (ifoperand(eq[c + 4]))
			{
				for (int i = 0; i < 4; i++)
				{
					temp += eq[c];
					c++;
				}
				while (!ifoperator(eq[c]) && eq[c] != ' ')
				{
					temp += eq[c];
					c++;
					if (c == eq.size())
					{
						break;
					}
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 4] == '-' && ifoperand(eq[c + 5]))
			{
				for (int i = 0; i < 4; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += '-';
				c++;
				while (!ifoperator(eq[c]) && eq[c] != ' ')
				{
					temp += eq[c];
					c++;
					if (c == eq.size())
					{
						break;
					}
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 4] == '-' && eq[c + 5] == 'p' && eq[c + 6] == 'i')
			{
				for (int i = 0; i < 7; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 4] == '-' && eq[c + 5] == 'e')
			{
				for (int i = 0; i < 6; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 4] == 'p' && eq[c + 5] == 'i')
			{
				for (int i = 0; i < 6; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 4] == 'e')
			{
				for (int i = 0; i < 5; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += ' ';
				c--;
			}
			else
			{
				cout << "Error! Invalid symbol: " << eq[c] << endl;
				except = true;
				stack = prev;
				while (stack != NULL)
				{
					stack = stack->next;
					delete prev;
					prev = stack;
				}
				return;
			}
		}
		else if (eq[c] == 'a' && eq[c + 1] == 'b' && eq[c + 2] == 's')
		{
			if (ifoperand(eq[c + 3]))
			{
				for (int i = 0; i < 3; i++)
				{
					temp += eq[c];
					c++;
				}
				while (!ifoperator(eq[c]) && eq[c] != ' ')
				{
					temp += eq[c];
					c++;
					if (c == eq.size())
					{
						break;
					}
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 3] == '-' && ifoperand(eq[c + 4]))
			{
				for (int i = 0; i < 3; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += '-';
				c++;
				while (!ifoperator(eq[c]) && eq[c] != ' ')
				{
					temp += eq[c];
					c++;
					if (c == eq.size())
					{
						break;
					}
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 3] == '-' && eq[c + 4] == 'p' && eq[c + 5] == 'i')
			{
				for (int i = 0; i < 6; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 3] == '-' && eq[c + 4] == 'e')
			{
				for (int i = 0; i < 5; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 3] == 'p' && eq[c + 4] == 'i')
			{
				for (int i = 0; i < 5; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += ' ';
				c--;
			}
			else if (eq[c + 3] == 'e')
			{
				for (int i = 0; i < 4; i++)
				{
					temp += eq[c];
					c++;
				}
				temp += ' ';
				c--;
			}
			else
			{
				cout << "Error! Invalid symbol: " << eq[c] << endl;
				except = true;
				stack = prev;
				while (stack != NULL)
				{
					stack = stack->next;
					delete prev;
					prev = stack;
				}
				return;
			}
		}
		else
		{
			cout << "Error! Invalid symbol: " << eq[c] << endl;
			except = true;
			stack = prev;
			while (stack != NULL)
			{
				stack = stack->next;
				delete prev;
				prev = stack;
			}
			return;
		}
	}
	while (stack != NULL)
	{
		temp += stack->sym;
		temp += ' ';
		stack = stack->next;
	}
	stack = prev;
	while (stack != NULL)
	{
		stack = stack->next;
		delete prev;
		prev = stack;
	}
	delete prev;
	eq.empty();
	eq = temp;
	cout << "The equation in postfix form:\n";
	cout << eq << endl;
}
double Str_eq::counting_postfix()
{
	double result = 0;
	Str_eq* prev;
	prev = stack;
	for (int c = 0; c < eq.size(); c++)
	{
		if (eq[c] == ' ')
			continue;
		if (ifoperand(eq[c]) || eq[c] == '.')
		{
			string val;
			val.empty();
			while (!ifoperator(eq[c]) && eq[c] != ' ')
			{
				val += eq[c];
				c++;
				if (c == eq.size())
					break;
			}
			prev = new Str_eq;
			prev->next = stack;
			stack = prev;
			stack->value = stod(val);
			result = stack->value;
		}
		else if (ifoperator(eq[c]) && !ifoperand(eq[c+1]))
		{
			double val1 = stack->value;
			stack = stack->next;
			delete prev;
			double val2 = stack->value;
			prev = stack;
			stack = stack->next;
			delete prev;
			switch(eq[c])
			{
                case '+': result = val2 + val1; break;
				case '-': result = val2 - val1; break;
				case '*': result = val2 * val1; break;
				case '/': result = val2 / val1; break;
				case '^': result = pow(val2, val1); break;
			}
			prev = new Str_eq;
			prev->next = stack;
			stack = prev;
			stack->value = result;
		}
		else if (ifoperator(eq[c]) && ifoperand(eq[c + 1]))
		{
			string val;
			val.empty();
			val += '-';
			c++;
			while (!ifoperator(eq[c]) && eq[c] != ' ')
			{
				val += eq[c];
				c++;
				if (c == eq.size())
					break;
			}
			prev = new Str_eq;
			prev->next = stack;
			stack = prev;
			stack->value = stod(val);
			result = stack->value;
		}
		else if (eq[c] == 'p')
		{
			string val;
			val.empty();
			for (int i = 0; i < 2; i++)
			{
				val += eq[c];
				c++;
			}
			prev = new Str_eq;
			prev->next = stack;
			stack = prev;
			stack->value = 3.14;
			c--;
			result = stack->value;
		}
		else if (eq[c] == 'e')
		{
			string val;
			val.empty();
			val += eq[c];
			prev = new Str_eq;
			prev->next = stack;
			stack = prev;
			stack->value = 2.7182818284;
			result = stack->value;
		}
		else if (eq[c] == 'c' && eq[c + 1] == 'o')
		{
			string val;
			val.empty();
			while (!ifoperand(eq[c]) && !ifoperator(eq[c]) && eq[c]!='.' && eq[c]!='p'&&eq[c]!='e')
			{
				c++;
			}
			while (eq[c] != ' ')
			{
				val += eq[c];
				c++;
			}
			prev = new Str_eq;
			prev->next = stack;
			stack = prev;
			if (val[0] == 'p')
			{
				stack->value = cos(3.14);
			}
			else if (val[0] == 'e')
			{
				stack->value = cos(2.7182818284);
			}
			else if (val[1] == 'p')
			{
				stack->value = cos(-3.14);
			}
			else if (val[1] == 'e')
			{
				stack->value = cos(-2.7182818284);
			}
			else
				stack->value = cos(stod(val));
			c--;
			result = stack->value;
		}
		else if (eq[c] == 's' && eq[c + 1] == 'i')
		{
			string val;
			val.empty();
			while (!ifoperand(eq[c]) && !ifoperator(eq[c]) && eq[c] != '.' && eq[c] != 'p' && eq[c] != 'e')
			{
				c++;
			}
			while (eq[c] != ' ')
			{
				val += eq[c];
				c++;
			}
			prev = new Str_eq;
			prev->next = stack;
			stack = prev;
			if (val[0] == 'p')
			{
				stack->value = sin(3.14);
			}
			else if (val[0] == 'e')
			{
				stack->value = sin(2.7182818284);
			}
			else if (val[1] == 'p')
			{
				stack->value = sin(-3.14);
			}
			else if (val[1] == 'e')
			{
				stack->value = sin(-2.7182818284);
			}
			else
				stack->value = sin(stod(val));
			c--;
			result = stack->value;
		}
		else if (eq[c] == 't' && eq[c + 1] == 'g')
		{
			string val;
			val.empty();
			while (!ifoperand(eq[c]) && !ifoperator(eq[c]) && eq[c] != '.' && eq[c] != 'p' && eq[c] != 'e')
			{
				c++;
			}
			while (eq[c] != ' ')
			{
				val += eq[c];
				c++;
			}
			prev = new Str_eq;
			prev->next = stack;
			stack = prev;
			if (val[0] == 'p')
			{
				stack->value = tan(3.14);
			}
			else if (val[0] == 'e')
			{
				stack->value = tan(2.7182818284);
			}
			else if (val[1] == 'p')
			{
				stack->value = tan(-3.14);
			}
			else if (val[1] == 'e')
			{
				stack->value = tan(-2.7182818284);
			}
			else
				stack->value = tan(stod(val));
			c--;
			result = stack->value;
		}
		else if (eq[c] == 'c' && eq[c + 1] == 't')
		{
			string val;
			val.empty();
			while (!ifoperand(eq[c]) && !ifoperator(eq[c]) && eq[c] != '.' && eq[c] != 'p' && eq[c] != 'e')
			{
				c++;
			}
			while (eq[c] != ' ')
			{
				val += eq[c];
				c++;
			}
			prev = new Str_eq;
			prev->next = stack;
			stack = prev;
			if (val[0] == 'p')
			{
				stack->value = 1/(tan(3.14));
			}
			else if (val[0] == 'e')
			{
				stack->value = 1 / (tan(2.7182818284));
			}
			else if (val[1] == 'p')
			{
				stack->value = 1/(tan(-3.14));
			}
			else if (val[1] == 'e')
			{
				stack->value = 1/(tan(-2.7182818284));
			}
			else
				stack->value = 1/(tan(stod(val)));
			c--;
			result = stack->value;
		}
		else if (eq[c] == 'l' && eq[c + 1] == 'n')
		{
			string val;
			val.empty();
			while (!ifoperand(eq[c]) && !ifoperator(eq[c]) && eq[c] != '.' && eq[c] != 'p' && eq[c] != 'e')
			{
				c++;
			}
			while (eq[c] != ' ')
			{
				val += eq[c];
				c++;
			}
			prev = new Str_eq;
			prev->next = stack;
			stack = prev;
			if (val[0] == 'p')
			{
				stack->value = log(3.14);
			}
			else if (val[0] == 'e')
			{
				stack->value = log(2.7182818284);
			}
			else if (val[1] == 'p')
			{
				stack->value = log(-3.14);
			}
			else if (val[1] == 'e')
			{
				stack->value = log(-2.7182818284);
			}
			else
				stack->value = log(stod(val));
			c--;
			result = stack->value;
		}
		else if (eq[c] == 'l' && eq[c + 1] == 'o')
		{
			string val;
			val.empty();
			while (!ifoperand(eq[c]) && !ifoperator(eq[c]) && eq[c] != '.' && eq[c] != 'p' && eq[c] != 'e')
			{
				c++;
			}
			while (eq[c] != ' ')
			{
				val += eq[c];
				c++;
			}
			prev = new Str_eq;
			prev->next = stack;
			stack = prev;
			if (val[0] == 'p')
			{
				stack->value = log10(3.14);
			}
			else if (val[0] == 'e')
			{
				stack->value = log10(2.7182818284);
			}
			else if (val[1] == 'p')
			{
				stack->value = log10(-3.14);
			}
			else if (val[1] == 'e')
			{
				stack->value = log10(-2.7182818284);
			}
			else
				stack->value = log10(stod(val));
			c--;
			result = stack->value;
		}
		else if (eq[c] == 's' && eq[c + 1] == 'q')
		{
			string val;
			val.empty();
			while (!ifoperand(eq[c]) && !ifoperator(eq[c]) && eq[c] != '.' && eq[c] != 'p' && eq[c] != 'e')
			{
				c++;
			}
			while (eq[c] != ' ')
			{
				val += eq[c];
				c++;
			}
			prev = new Str_eq;
			prev->next = stack;
			stack = prev;
			if (val[0] == 'p')
			{
				stack->value = sqrt(3.14);
			}
			else if (val[0] == 'e')
			{
				stack->value = sqrt(2.7182818284);
			}
			else if (val[1] == 'p')
			{
				stack->value = sqrt(-3.14);
			}
			else if (val[1] == 'e')
			{
				stack->value = sqrt(-2.7182818284);
			}
			else
				stack->value = sqrt(stod(val));
			c--;
			result = stack->value;
		}
		else if (eq[c] == 'a' && eq[c + 1] == 'b')
		{
			string val;
			val.empty();
			while (!ifoperand(eq[c]) && !ifoperator(eq[c]) && eq[c] != '.' && eq[c] != 'p' && eq[c] != 'e')
			{
				c++;
			}
			while (eq[c] != ' ')
			{
				val += eq[c];
				c++;
			}
			prev = new Str_eq;
			prev->next = stack;
			stack = prev;
			if (val[0] == 'p')
			{
				stack->value = abs(3.14);
			}
			else if (val[0] == 'e')
			{
				stack->value = abs(2.7182818284);
			}
			else if (val[1] == 'p')
			{
				stack->value = abs(-3.14);
			}
			else if (val[1] == 'e')
			{
				stack->value = abs(-2.7182818284);
			}
			else
				stack->value = abs(stod(val));
			c--;
			result = stack->value;
		}
	}
	while (stack != NULL)
	{
		prev = stack;
		stack = stack->next;
		delete prev;
	}
	eq.empty();
	return result;
}
bool Str_eq::ifoperator(char s)
{
	switch (s)
	{
	case '+':
	case '-':
	case '*':
	case '/':
	case '^':
	case ')':
	case '(':
		return true;
		break;
	default:
		return false;
	}
}
bool Str_eq::ifoperand(char s)
{
	switch (s)
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		return true;
	default:
		return false;
	}
}
int Str_eq::priorityity(char s)
{
	switch (s)
	{
	case'+':
		return 1;
		break;
	case'-':
		return 1;
		break;
	case'*':
		return 2;
		break;
	case'/':
		return 2;
		break;
	case'^':
		return 3;
		break;
	case')':
		return 0;
		break;
	case'(':
		return 0;
		break;

	}
}