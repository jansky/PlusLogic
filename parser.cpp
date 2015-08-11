/*
    PlusLogic Library (libpluslogic)
    Copyright (C) 2015 Ian Duncan

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "pluslogic.h"
#include <string>
#include <ctype.h>
#include <iostream>

std::vector<PLCalcToken> pl_parse_string(std::string s,bool varsenabled)
{
	std::vector<PLCalcToken> tokens;

	std::string curtoken;

	bool on_number = false;

	int i = 0;

	while(i < s.length())
	{
		//std::cout << i << std::endl;
		if(isdigit(s[i]) || s[i] == '.')
		{
			on_number = true;

			curtoken += s[i];

			i++;
		}
		else
		{
			if(on_number == true)
			{
				tokens.push_back(pl_create_token(number, std::stod(curtoken)));

				curtoken = "";
				on_number = false;
			}

			if(s[i] != ' ')
			{
				//enum PLOperand{invalid,pl_not,pl_and,pl_or,pl_xor,pl_nand,pl_imp,pl_eq};
				//std::cout << "!!" << std::endl;
				
				if(s[i] == '&')
				{
					tokens.push_back(pl_create_token(operand, pl_and));
				}
			
				else if(s[i] == '|')
				{
					tokens.push_back(pl_create_token(operand, pl_or));
				}
				else if(s[i] == '^')
				{
					tokens.push_back(pl_create_token(operand, pl_xor));
				}
				else if(s[i] == '=')
				{
					tokens.push_back(pl_create_token(operand, pl_eq));
				}
				else if(s[i] == '(')
				{
					tokens.push_back(pl_create_token(operand, leftper));
				}
				else if(s[i] == ')')
				{
					tokens.push_back(pl_create_token(operand, rightper));
				}		
				else if(s[i] == '-')
				{
					if(s.length() > i + 1 && s[i + 1] == '>')
					{
						tokens.push_back(pl_create_token(operand, pl_imp));
						i+=1;
					}
					else
					{
						tokens.push_back(pl_create_token(operand, minus));
					}
				}
				else if(s[i] == '~')
				{
					if(s.length() > i + 1 && s[i + 1] == '&')
					{
						tokens.push_back(pl_create_token(operand, pl_nand));
						i+=1;
					}
					else if(s.length() > i + 1 && s[i + 1] == '|')
					{
						tokens.push_back(pl_create_token(operand, pl_nor));
						i+=1;
					}
					else
					{
						tokens.push_back(pl_create_token(operand, pl_not));
					}
				}
					
				else
				{
					//std::cout << "~~" << std::endl;

					if(varsenabled)
					{
						//std::cout << s[i] << std::endl;
						//Variables are enabled, treat char as a variable
						tokens.push_back(pl_create_token(variable, (double)s[i]));
					}
					else
					{
						//No variables, unrecognized character
						throw syntaxerror;
					}
				}
			}

			
			i++;


			


		}
	}

	if(on_number == true)
	{
		tokens.push_back(pl_create_token(number, std::stod(curtoken)));

		curtoken = "";
		on_number = false;
	}

	//std::cout << tokens.size() << std::endl;
	return tokens;
}
