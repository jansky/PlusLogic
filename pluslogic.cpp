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
#include <iostream>
#include <sstream>




bool pl_is_value_numeric(std::string tocheck)
{
	try
	{
		std::stod(tocheck);
	}
	catch(std::invalid_argument)
	{
		return false;
	}
	return true;
}

bool pl_is_value_numeric(PLCalcToken tocheck)
{
	if(tocheck.type == number)
		return true;
	else
		return false;
}

bool pl_i_is_in_blacklist(std::vector<int>& blacklist, int i)
{
	for(int j : blacklist)
	{
		if(j == i)
			return true;
	}

	return false;
}


std::vector<std::string> PLStringSplit(const std::string &source, const char *delimiter, bool keepEmpty)
{
    std::vector<std::string> results;

    size_t prev = 0;
    size_t next = 0;

    while ((next = source.find_first_of(delimiter, prev)) != std::string::npos)
    {
        if (keepEmpty || (next - prev != 0))
        {
            results.push_back(source.substr(prev, next - prev));
        }
        prev = next + 1;
    }

    if (prev < source.size())
    {
        results.push_back(source.substr(prev));
    }

    return results;
}

PLCalcToken pl_iterate_tokens(std::vector<PLCalcToken>& tokens, PLLogicMode mode)
{
	std::vector<PLCalcToken> wcopy = tokens;

	PLLogicType ltype = pl_get_logic_type_data(mode);

	//Check all numbers to make sure they're within bounds

	for(int i = 0; i < wcopy.size(); i++)
	{
		if(pl_is_value_numeric(wcopy[i]))
		{
			bool found_number = false;

			
			for(int j = 0; j < ltype.permissible_numbers.size(); j++)
			{
				if(wcopy[i].value == ltype.permissible_numbers[j])
					found_number = true;
			}

			if(!found_number)
				throw modeerror;
		}
	}

	while(wcopy.size() > 1)
	{
		bool gofurther = true;
		PLCalcToken newvalue;
		std::vector<int> blacklist;
		std::vector<PLCalcToken> newtokenlist;
		int insertat = 0;
		bool foundoperation = false;

		for(int i = 0; i<wcopy.size(); i++)
		{
			if(wcopy[i].type == operand)
				foundoperation = true;
		}

		if(!foundoperation)
			throw operationerror;
			
		

		//Parentheses
		for(int i = 0; i<wcopy.size(); i++)
		{
			if(wcopy[i].type == operand && wcopy[i].value == leftper && gofurther == true)
			{
				int pneeded = 1;
				int pcount = 0;
				int l = i + 1;
				std::vector<PLCalcToken> parentheses;


				blacklist.push_back(i);
				try
				{
					while(pcount < pneeded && l < wcopy.size())
					{	
						if(wcopy[l].type == operand && wcopy[l].value == leftper)
							pneeded++;
						if(wcopy[l].type == operand && wcopy[l].value == rightper)
							pcount++;

						if((wcopy[l].type == operand && wcopy[l].value != rightper) || pneeded > 1 || wcopy[l].type == number)
							parentheses.push_back(wcopy[l]);

						blacklist.push_back(l);

						l++;

					}
				}
				catch(std::out_of_range)
				{
					throw syntaxerror;
				}

				if(pcount != pneeded)
					throw syntaxerror;

				if(!pl_is_value_numeric(parentheses[parentheses.size() -1]) && parentheses[parentheses.size() - 1].value == rightper)
					parentheses.pop_back();

				newvalue = pl_create_token(number, pl_iterate_tokens(parentheses).value);
				insertat = i;
				gofurther = false;
			}
		}

		//Negatives
		for(int i = 0; i<wcopy.size(); i++)
		{
			if(wcopy[i].type == operand && wcopy[i].value == minus && gofurther == true)
			{
				if(!pl_is_value_numeric(wcopy[i + 1]))
					throw syntaxerror;
				else
				{
					if(!pl_is_value_numeric(wcopy[i - 1]))
					{
						gofurther = false;
						newvalue = pl_create_token(number,0 - wcopy[i + 1].value);
						//blacklist.push_back(i - 1);
						blacklist.push_back(i + 1);
						blacklist.push_back(i);
						insertat = i;
					}

				}
			}
		}

		

		//NOT
		for(int i = 0; i<wcopy.size(); i++)
		{
			if(wcopy[i].type == operand && wcopy[i].value == pl_not && gofurther == true)
			{
				if(!pl_is_value_numeric(wcopy[i + 1]))
					throw syntaxerror;
				else
				{
				
					gofurther = false;
					newvalue = pl_create_token(number, ltype.pl_not(wcopy[i + 1].value));
					blacklist.push_back(i + 1);
					blacklist.push_back(i);
					insertat = i;

				}
			}
		}

		//Other unary functions

		for(int i = 0; i<wcopy.size(); i++)
		{
			if(wcopy[i].type == operand && wcopy[i].value == pl_undef && gofurther == true)
			{
				if(!pl_is_value_numeric(wcopy[i + 1]))
					throw syntaxerror;
				else
				{
				
					gofurther = false;
					newvalue = pl_create_token(number, ltype.pl_undef(wcopy[i + 1].value));
					blacklist.push_back(i + 1);
					blacklist.push_back(i);
					insertat = i;

				}
			}
		}

		for(int i = 0; i<wcopy.size(); i++)
		{
			if(wcopy[i].type == operand && wcopy[i].value == pl_notfalse && gofurther == true)
			{
				if(!pl_is_value_numeric(wcopy[i + 1]))
					throw syntaxerror;
				else
				{
				
					gofurther = false;
					newvalue = pl_create_token(number, ltype.pl_notfalse(wcopy[i + 1].value));
					blacklist.push_back(i + 1);
					blacklist.push_back(i);
					insertat = i;

				}
			}
		}

		for(int i = 0; i<wcopy.size(); i++)
		{
			if(wcopy[i].type == operand && wcopy[i].value == pl_istrue && gofurther == true)
			{
				if(!pl_is_value_numeric(wcopy[i + 1]))
					throw syntaxerror;
				else
				{
				
					gofurther = false;
					newvalue = pl_create_token(number, ltype.pl_istrue(wcopy[i + 1].value));
					blacklist.push_back(i + 1);
					blacklist.push_back(i);
					insertat = i;

				}
			}
		}


		

		//Conjunctions
		for(int i = 0; i<wcopy.size(); i++)
		{
			if(wcopy[i].type == operand && wcopy[i].value == pl_and && gofurther == true)
			{
				if(!pl_is_value_numeric(wcopy[i - 1]) || !pl_is_value_numeric(wcopy[i + 1]))
					throw syntaxerror;
				else
				{
					gofurther = false;
					newvalue = pl_create_token(number, ltype.pl_and(wcopy[i-1].value, wcopy[i+1].value));
					blacklist.push_back(i - 1);
					blacklist.push_back(i + 1);
					blacklist.push_back(i);
					insertat = i;

				}
			}
		}

		for(int i = 0; i<wcopy.size(); i++)
		{
			if(wcopy[i].type == operand && wcopy[i].value == pl_nand && gofurther == true)
			{
				if(!pl_is_value_numeric(wcopy[i - 1]) || !pl_is_value_numeric(wcopy[i + 1]))
					throw syntaxerror;
				else
				{
					gofurther = false;
					newvalue = pl_create_token(number, ltype.pl_nand(wcopy[i-1].value, wcopy[i+1].value));
					blacklist.push_back(i - 1);
					blacklist.push_back(i + 1);
					blacklist.push_back(i);
					insertat = i;

				}
			}
		}

		//Disjunctions

		      
		
		for(int i = 0; i<wcopy.size(); i++)
		{
			if(wcopy[i].type == operand && wcopy[i].value == pl_or && gofurther == true)
			{
				if(!pl_is_value_numeric(wcopy[i - 1]) || !pl_is_value_numeric(wcopy[i + 1]))
					throw syntaxerror;
				else
				{
					gofurther = false;
					newvalue = pl_create_token(number, ltype.pl_or(wcopy[i - 1].value,wcopy[i + 1].value));
					blacklist.push_back(i - 1);
					blacklist.push_back(i + 1);
					blacklist.push_back(i);
					insertat = i;

				}
			}
		}

	        
		for(int i = 0; i<wcopy.size(); i++)
		{
			if(wcopy[i].type == operand && wcopy[i].value == pl_xor && gofurther == true)
			{
				if(!pl_is_value_numeric(wcopy[i - 1]) || !pl_is_value_numeric(wcopy[i + 1]))
					throw syntaxerror;
				else
				{
					gofurther = false;
					newvalue = pl_create_token(number, ltype.pl_xor(wcopy[i - 1].value, wcopy[i + 1].value));
					blacklist.push_back(i - 1);
					blacklist.push_back(i + 1);
					blacklist.push_back(i);
					insertat = i;

				}
			}
		}

		for(int i = 0; i<wcopy.size(); i++)
		{
			if(wcopy[i].type == operand && wcopy[i].value == pl_nor && gofurther == true)
			{
				if(!pl_is_value_numeric(wcopy[i - 1]) || !pl_is_value_numeric(wcopy[i + 1]))
					throw syntaxerror;
				else
				{
					gofurther = false;
					newvalue = pl_create_token(number, ltype.pl_nor(wcopy[i - 1].value, wcopy[i + 1].value));
					blacklist.push_back(i - 1);
					blacklist.push_back(i + 1);
					blacklist.push_back(i);
					insertat = i;

				}
			}
		}

		//Implication & Equality

		for(int i = 0; i<wcopy.size(); i++)
		{
			if(wcopy[i].type == operand && wcopy[i].value == pl_imp && gofurther == true)
			{
				if(!pl_is_value_numeric(wcopy[i - 1]) || !pl_is_value_numeric(wcopy[i + 1]))
					throw syntaxerror;
				else
				{
					gofurther = false;
					newvalue = pl_create_token(number, ltype.pl_imp(wcopy[i - 1].value,wcopy[i + 1].value));
					blacklist.push_back(i - 1);
					blacklist.push_back(i + 1);
					blacklist.push_back(i);
					insertat = i;

				}
			}
		}

		for(int i = 0; i<wcopy.size(); i++)
		{
			if(wcopy[i].type == operand && wcopy[i].value == pl_eq && gofurther == true)
			{
				if(!pl_is_value_numeric(wcopy[i - 1]) || !pl_is_value_numeric(wcopy[i + 1]))
					throw syntaxerror;
				else
				{
					gofurther = false;
					newvalue = pl_create_token(number, ltype.pl_eq(wcopy[i - 1].value,wcopy[i + 1].value));
					blacklist.push_back(i - 1);
					blacklist.push_back(i + 1);
					blacklist.push_back(i);
					insertat = i;

				}
			}
		}

		//Rewrite the list of tokens

		for(int i = 0; i < wcopy.size();i++)
		{
			if(i == insertat)
				newtokenlist.push_back(newvalue);
			else if(pl_i_is_in_blacklist(blacklist, i))
				;
			else
				newtokenlist.push_back(wcopy[i]);
		}

		wcopy = newtokenlist;




	}

	return wcopy[0];
}



PLCalcToken pl_create_token(PLTokenType type, long double value)
{
	PLCalcToken t;

	t.type = type;
	t.value = value;

	return t;
}

PLVariable pl_create_variable(long double value, char name)
{
	PLVariable var;
	std::vector<PLCalcToken> tokens;

	tokens.push_back(pl_create_token(number, value));

	var.tokens = tokens;
	var.name = name;

	return var;
}

PLVariable pl_create_variable(std::vector<PLCalcToken> value, char name)
{
	PLVariable var;
	

	var.tokens = value;
	var.name = name;

	//Check that variable definition is not endlessly recursive
	for(int i = 0; i < value.size(); i++)
	{
		if(value[i].type == variable && value[i].value == (long double)name)
			throw variableerror;
	}

	return var;
}

long double pl_evaluate_math_expression(std::string expr, PLLogicMode mode)
{
	/*std::vector<std::string> stringtokens = PLStringSplit(expr);
	std::vector<PLCalcToken> tokens;

	for(std::string strtoken : stringtokens)
	{
		if(!pl_is_value_numeric(strtoken))
		{
			PLOperand op = pl_get_operand_from_string(strtoken);

			if(op == invalid)
				throw syntaxerror;
			else
				tokens.push_back(pl_create_token(operand, op));

		}
		else
		{
			tokens.push_back(pl_create_token(number, std::stod(strtoken)));
		}
	}
	*/

	std::vector<PLCalcToken> tokens;
	tokens = pl_parse_string(expr, false);

	PLCalcToken last = pl_iterate_tokens(tokens, mode);

	if(last.type != number)
		throw lasttokenerror;

	return last.value;




}

long double pl_evaluate_math_expression(std::vector<PLCalcToken> expr, PLLogicMode mode)
{
	PLCalcToken last = pl_iterate_tokens(expr, mode);

	if(last.type != number)
		throw lasttokenerror;

	return last.value;
}

int pl_get_variable_by_name(std::vector<PLVariable> vars, char name)
{
	for(int i = 0; i < vars.size(); i++)
	{
		if(vars[i].name == name)
		{
			//5std::cout << vars[i].name << std::endl;
			return i;
		}
	}

	throw variableerror;

	return 0;
}

std::vector<PLCalcToken> pl_variable_substitute(std::vector<PLCalcToken> tok, std::vector<PLVariable> vars)
{
	std::vector<PLCalcToken> tokens = tok;
	bool foundvar = false;

	for(int i = 0; i < tokens.size(); i++)
	{
			//std::cout << tokens[i].value << std::endl;

			if(tokens[i].type == variable)
			{
				int varid = pl_get_variable_by_name(vars, (char)tokens[i].value);

				//std::cout << (vars[varid]).tokens[0].value << std::endl;

				//std::cout << tokens[i + 1].value << std::endl;

				tokens.erase(tokens.begin()+i);

				tokens.insert(tokens.begin()+i, vars[varid].tokens.begin(), vars[varid].tokens.end());

				//std::cout << tokens[i].value << std::endl;

				//std::cout << tokens[i + 1].value << std::endl;

				foundvar = true;
				goto outofloop;
			}
	}

	outofloop:

	if(foundvar)
		return pl_variable_substitute(tokens, vars);
	else
		return tokens;
}

long double pl_evaluate_math_expression(std::string expr, std::vector<PLVariable> vars, PLLogicMode mode)
{
	

	std::vector<PLCalcToken> tokens;
	tokens = pl_parse_string(expr, true);
	//Not really but we need this to be true for the sake of the while loop
	bool foundvar = true;

	//Perform the substitution
	tokens = pl_variable_substitute(tokens, vars);
	


	PLCalcToken last = pl_iterate_tokens(tokens, mode);

	if(last.type != number)
		throw lasttokenerror;

	return last.value;




}

long double pl_evaluate_math_expression(std::vector<PLCalcToken> expr, std::vector<PLVariable> vars, PLLogicMode mode)
{
	

	std::vector<PLCalcToken> tokens = expr;
	//Not really but we need this to be true for the sake of the while loop
	bool foundvar = true;

	//Perform the substitution
	tokens = pl_variable_substitute(tokens, vars);
	


	PLCalcToken last = pl_iterate_tokens(tokens, mode);

	if(last.type != number)
		throw lasttokenerror;

	return last.value;

}

std::string pl_tokens_to_string(std::vector<PLCalcToken> tokens)
{
	std::string result = "";

	for(int i = 0; i < tokens.size(); i++)
	{
		if(tokens[i].type == operand)
		{
			

			if(tokens[i].value == pl_not)
				result += "~";
			else if(tokens[i].value == pl_and)
				result += "&";
			else if(tokens[i].value == pl_or)
				result += "|";
			else if(tokens[i].value == pl_xor)
				result += "^";
			else if(tokens[i].value == pl_nand)
				result += "!&";
			else if(tokens[i].value == pl_imp)
				result += " -> ";
			else if(tokens[i].value == pl_eq)
				result += "=";
		
			else if(tokens[i].value == leftper)
				result += "(";
			else if(tokens[i].value == rightper)
				result += ")";
		
			else
				throw syntaxerror;
		}
		else if(tokens[i].type == variable)
		{
			result += char(tokens[i].value);
		}
		else if(tokens[i].type == number)
		{
			std::stringstream ss;
			ss << tokens[i].value;

			result += ss.str();
		}
		else
			throw syntaxerror;
	}

	return result;
}

