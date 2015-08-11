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
#pragma once

#include <sstream>
#include <vector>
#include <stdexcept>
#include <cmath>


enum PLTokenType{operand,number,variable};
enum PLOperand{invalid,pl_not,pl_and,pl_or,pl_xor,pl_nor,pl_nand,pl_imp,pl_eq,leftper,rightper,minus};
enum PLException{syntaxerror,lasttokenerror,domainerror,operationerror,variableerror,modeerror};
enum PLLogicMode{boolean};

typedef double (*PLLogicFunc)(double, double);
typedef double (*PLLogicUnaryFunc)(double);

typedef struct PLCalcToken
{
	PLTokenType type;
	long double value;
} PLCalcToken;

typedef struct PLVariable
{
    char name;
    std::vector<PLCalcToken> tokens;
    
} PLVariable;

typedef struct PLLogicType
{
	PLLogicUnaryFunc pl_not;
	PLLogicFunc pl_and;
	PLLogicFunc pl_or;
	PLLogicFunc pl_xor;
	PLLogicFunc pl_nand;
	PLLogicFunc pl_nor;
	PLLogicFunc pl_imp;
	PLLogicFunc pl_eq;

	std::vector<double> permissible_numbers;
} PLLogicType;


bool pl_is_value_numeric(std::string tocheck);
bool pl_is_value_numeric(PLCalcToken tocheck);
bool pl_i_is_in_blacklist(std::vector<int>& blacklist, int i);






PLCalcToken pl_iterate_tokens(std::vector<PLCalcToken>& tokens, PLLogicMode mode = boolean);

std::vector<std::string> PLStringSplit(const std::string &source, const char *delimiter = " ", bool keepEmpty = false);

PLCalcToken pl_create_token(PLTokenType type, long double value);

PLVariable pl_create_variable(long double value, char name);
PLVariable pl_create_variable(std::vector<PLCalcToken> value, char name);
std::vector<PLCalcToken> pl_variable_substitute(std::vector<PLCalcToken> tok, std::vector<PLVariable> vars);
int pl_get_variable_by_name(std::vector<PLVariable> vars, char name);

long double pl_evaluate_math_expression(std::string expr, PLLogicMode mode = boolean);
long double pl_evaluate_math_expression(std::vector<PLCalcToken> expr, PLLogicMode mode = boolean);
long double pl_evaluate_math_expression(std::string expr, std::vector<PLVariable> vars, PLLogicMode mode = boolean);
long double pl_evaluate_math_expression(std::vector<PLCalcToken> expr, std::vector<PLVariable> vars, PLLogicMode mode = boolean);

std::vector<PLCalcToken> pl_parse_string(std::string s,bool varsenabled=true);

std::string pl_tokens_to_string(std::vector<PLCalcToken> tokens);

PLLogicType pl_get_logic_type_data(PLLogicMode mode);

/* Boolean Logic Functions */

double boolean_not(double a);
double boolean_and(double a, double b);
double boolean_or(double a, double b);
double boolean_xor(double a, double b);
double boolean_nand(double a, double b);
double boolean_imp(double a, double b);
double boolean_eq(double a, double b);
double boolean_nor(double a, double b);



