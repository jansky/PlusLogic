/*
    PlusLogic Interpreter
    Copyright (C) 2015 Ian Duncan

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "pluslogic.h"
#include "pluslogicint.h"
#include <string.h>
#include <iostream>
#include <limits>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <fstream>
#include <readline/readline.h>
#include <readline/history.h>

std::string historypath;
std::vector<PLVariable> variables;
std::vector<PLIFunction> functions;
PLLogicMode mode;


PLIFunction pli_create_function(std::string name, std::vector<PLCalcToken> tokens, std::vector<char> variable_names)
{
	PLIFunction func;

	func.name = name;
	func.tokens = tokens;
	func.variable_names = variable_names;

	return func;
}

int pli_get_function_id_by_name(std::string name)
{
	for(int i = 0; i < functions.size(); i++)
	{
		if(functions[i].name == name)
			return i;
	}

	throw variableerror;
	return 0;
}

long double pli_evaluate_function(PLIFunction function, std::string args)
{
	//Make a copy of the variables list
	std::vector<PLVariable> specvars = variables;

	//Delete variables that are function arguments
	for(int i = 0; i < function.variable_names.size(); i++)
	{
		for(int j = 0; j < specvars.size(); j++)
		{
			if(specvars[j].name == function.variable_names[i])
			{
				specvars.erase(specvars.begin()+j);
				break;
			}
		}
	}

	//Parse the arguments
	std::vector<std::string> arguments = PLStringSplit(args, ",", false);

	if(arguments.size() != function.variable_names.size())
		throw variableerror;

	for(int i = 0; i < arguments.size(); i++)
	{
		specvars.push_back(pl_create_variable(pl_parse_string(arguments[i], true), function.variable_names[i]));
	}

	//Evaluate
	return pl_evaluate_math_expression(function.tokens, specvars, mode);
}

void exceptionresult(PLException err, bool exitonexception)
{
				int exitresult = 1;
				
				if(err == syntaxerror)
					std::cout << "Error: Syntax Error while evaluating input." << std::endl;
				else if(err == lasttokenerror)
				{
					std::cout << "Error: Last token was not a number." << std::endl;
					exitresult = 2;
				}
				else if(err == domainerror)
				{
					std::cout << "Error: Domain error." << std::endl;
					exitresult = 3;
				}
				else if(err == operationerror)
				{
					std::cout << "Error: No operations remain on stack." << std::endl;
					exitresult = 4;
				}
				else if(err == variableerror)
				{
					std::cout << "Error: Variable error." << std::endl;
					exitresult = 8;
				}
				else if(err == modeerror)
				{
					std::cout << "Error: Logic mode error." << std::endl;
					exitresult = 9;
				}
				else
				{
					std::cout << "Error: An unknown exception was caught." << std::endl;
					exitresult = 5;
				}
				
				if(exitonexception)
					exit(exitresult);
}

bool pli_save_vars_funcs_to_file(std::string fname)
{
	
		std::ofstream save(fname);

		if(save)
		{

		save << "# Save file generated by PlusLogic v0.99a\n\n";

		

		//Save all variables
		for(int i = 0; i < variables.size(); i++)
		{


			save << "set ";
			save << variables[i].name << " ";
			save << pl_tokens_to_string(variables[i].tokens) << std::endl;

			
		}

		//Save all functions
		for(int i = 0; i < functions.size(); i++)
		{
			save << "func ";
			save << functions[i].name << " ";

			//Write arguments
			for(int j = 0; j < functions[i].variable_names.size(); j++)
			{
				save << functions[i].variable_names[j];

				if(j != functions[i].variable_names.size() - 1)
					save << ",";
			}

			save << ";";
			save << pl_tokens_to_string(functions[i].tokens) << "\n";
		}
	}
	else
	{
		std::cout << "Error: Could not open file." << std::endl;
		return false;
	}

		save.close();
	



	

	
	

	return true;

}

long double pli_load_vars_funcs_from_file(std::string fname, long double ans,bool exitonexception)
{
	

	std::ifstream save(fname);
	long double newans = ans;

	if(save)
	{
		for( std::string line; std::getline( save, line ); )
		{
			
			

			if(!line.empty() && line[0] != '#')
			{
				
				newans = evaluateInput(line, newans, exitonexception);
			}
		}

	}
	else
	{
		std::cout << "Error: Could not open file." << std::endl;

		if(exitonexception)
			exit(9);
		
	}



	return newans;

}

int main(int argc, char **argv)
{
	
	historypath = getenv("HOME");
	historypath += "/.pluslogicint_history";
	mode = boolean;

	long double ans = 0;


	//Read from ~/.pluscalcintrc file if it exists

	std::string rcpath = getenv("HOME");
	rcpath += "/.pluslogicintrc";

	//std::cout << rcpath << std::endl;


	//Get the PL_LOGIC_MODE env variable, if it exists
	char *logic_mode = getenv("PL_LOGIC_MODE");

	if(logic_mode != NULL)
		evaluateInput("mode " + std::string(logic_mode), ans, true);

		

	

	
	if(argc > 1 && std::string(argv[1]) != "-l" && std::string(argv[1]) != "-e")
	{
		if(pli_file_exists(rcpath))
		{
			evaluateInput("load " + rcpath, ans, false);
		}
		evaluateCmdInput(argc, argv);
	}
	else
	{
		
		
		read_history(historypath.c_str());
		
		
		
		

		if(argc > 2 && std::string(argv[1]) == "-e")
		{
			

			evaluateInput("load " + std::string(argv[2]), ans, true);
				
			exit(0);


		}
	
		std::cout << "PlusLogic Interpreter v0.99a" << std::endl << "Copyright (c) 2015 Ian Duncan" << std::endl << "This is free software with ABSOLUTELY NO WARRANTY.\nEnter 'license' for more information." <<  std::endl << "Enter 'quit' to exit and 'clear' to clear the history file (~/.pluslogicint_history)." << std::endl << std::endl;
		
		if(pli_file_exists(rcpath))
		{
			evaluateInput("load " + rcpath, ans, false);
		}

		
		pli_report_logic_mode();

		std::cout << std::endl;

		if(argc > 2 && std::string(argv[1]) == "-l")
		{
			

			//Load files
			for(int i = 2; i < argc; i++)
			{
				ans = evaluateInput("load " + std::string(argv[i]), ans, false);
				
			}


		}

		

		while(1)
		{
			char *rawinput;
			char prompt[3] = "> ";
			//std::string input;

			//std::cout << "> ";
			//std::getline(std::cin, input);
			
			rawinput = readline(prompt);

			if(strlen(rawinput) == 0)
				;
			else
			{
			
				if(!rawinput || std::string(rawinput) == "quit")
					exit(0);
				
				add_history(rawinput);
				write_history(historypath.c_str());

				ans = evaluateInput(std::string(rawinput), ans, false);
				
				free(rawinput);
			}
			
			

		}
	}

	return 0;
}

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}



long double evaluateInput(std::string input, long double ans, bool exitonexception)
{
		
		std::cout.precision(std::numeric_limits<long double>::digits10 + 1);
		
		long double newans = 0;
		
		
		
		if(input == "quit")
			exit(0);
		else if(input == "license")
		{
		
		    std::cout << "PlusLogic Interpreter v0.99a" << std::endl;
    		std::cout << "Copyright (C) 2015 Ian Duncan" << std::endl << std::endl;

    		std::cout << "This program is free software: you can redistribute it and/or modify" << std::endl;
		    std::cout << "it under the terms of the GNU General Public License as published by" << std::endl;
		    std::cout << "the Free Software Foundation, either version 3 of the License, or" << std::endl;
		    std::cout << "(at your option) any later version." << std::endl << std::endl;

		    std::cout << "This program is distributed in the hope that it will be useful," << std::endl;
		    std::cout << "but WITHOUT ANY WARRANTY; without even the implied warranty of" << std::endl;
		    std::cout << "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the" << std::endl;
		    std::cout << "GNU General Public License for more details." << std::endl << std::endl;

		    std::cout << "You should have received a copy of the GNU General Public License" << std::endl;
		    std::cout << "along with this program.  If not, see <http://www.gnu.org/licenses/>." << std::endl << std::endl;
		}
		else if(input == "clear")
		{
			clear_history();
			if(history_truncate_file(historypath.c_str(), 0) != 0)
			{
				std::cout << "History clear NOT successful. Entries may remain in file." << std::endl;
				if(exitonexception)
					exit(7);
			}
			else
			{
				
				std::cout << "History clear successful." << std::endl;
			}
		}
		else
		{
			try
			{
				std::vector<std::string> inputvec;

				inputvec = PLStringSplit(input);

				//We need to set a variable
				if(inputvec[0] == "set" && inputvec.size() > 2)
				{

					char name = (inputvec[1])[0];

					//Remove variable entries with the same name
					for(int i = 0; i < variables.size(); i++)
					{
						if(variables[i].name == name)
						{
							variables.erase(variables.begin()+i);
							break;
						}
					}

					//Build the string to tokenize
					std::string varraw = "";

					for(int i = 2; i < inputvec.size(); i++)
					{
						varraw += inputvec[i] + " ";
					}

					replaceAll(varraw,"ans",std::to_string(ans));

					variables.push_back(pl_create_variable(pl_parse_string(varraw, true), name));


				}
				//We need to get user input to define a variable
				else if(inputvec[0] == "input" && inputvec.size() >= 2)
				{
					char name = (inputvec[1])[0];

					//Remove variable entries with the same name
					for(int i = 0; i < variables.size(); i++)
					{
						if(variables[i].name == name)
						{
							variables.erase(variables.begin()+i);
							break;
						}
					}

					std::string uinput;

					if(inputvec.size() == 2)
						std::cout << name << "? ";
					else
					{
						//Build the prompt
						std::string prompt = "";

						for(int i = 2; i < inputvec.size(); i++)
						{
							prompt += inputvec[i] + " ";
						}

						std::cout << prompt;
					}

					std::cin >> uinput;

					variables.push_back(pl_create_variable(pl_parse_string(uinput, true), name));


				}
				//We need to print a string and a newline
				else if(inputvec[0] == "print" && inputvec.size() > 1)
				{
						//Build the string
						std::string toprint = "";

						for(int i = 1; i < inputvec.size(); i++)
						{
							toprint += inputvec[i] + " ";
						}

						std::cout << toprint << std::endl;
				}
				//We need to print a string without a newline
				else if(inputvec[0] == "printr" && inputvec.size() > 1)
				{
						//Build the string
						std::string toprint = "";

						for(int i = 1; i < inputvec.size(); i++)
						{
							toprint += inputvec[i] + " ";
						}

						std::cout << toprint;
				}
				//We need to define a function
				else if(inputvec[0] == "func" && inputvec.size () > 2)
				{
					std::string name = inputvec[1];

					//Remove function entries with the same name
					for(int i = 0; i < functions.size(); i++)
					{
						if(functions[i].name == name)
						{
							functions.erase(functions.begin()+i);
							break;
						}
					}

					//Build the string to tokenize
					std::string fargsdef = "";

					for(int i = 2; i < inputvec.size(); i++)
					{
						fargsdef += inputvec[i] + " ";
					}

					//Split the arguments and definition
					std::vector<std::string> argsdef = PLStringSplit(fargsdef, ";", false);

					if(argsdef.size() != 2)
						throw syntaxerror;

					std::vector<char> variable_names;

					//Parse the list of arguments
					std::vector<std::string> args = PLStringSplit(argsdef[0], ",", false);

					if(args.size() < 1)
						throw syntaxerror;

					for(int i = 0; i < args.size(); i++)
					{
						char argname = (args[i])[0];

						//Check for duplicate arguments
						for(int j = 0; j < variable_names.size(); j++)
						{
							if(variable_names[j] == argname)
								throw syntaxerror;
						}

						variable_names.push_back(argname);
					}

					std::string def = argsdef[1];
					replaceAll(def,"ans",std::to_string(ans));

					//Create the function and add it to the list
					functions.push_back(pli_create_function(name, pl_parse_string(def, true), variable_names));




				}
				//We need to execute a function
				else if(inputvec[0] == "call" && inputvec.size() > 2)
				{
					std::string name = inputvec[1];

					//Join the arguments together
					std::string args = "";

					for(int i = 2; i < inputvec.size(); i++)
					{
						args += inputvec[i] + " ";
					}

					newans = pli_evaluate_function(functions[pli_get_function_id_by_name(name)], args);

					if(fmod(newans, 1) == 0)
						printf("%0.0Lf\n", newans);
					else
						std::cout <<  newans << std::endl;



				}
				//We need to generate a truth table
				else if(inputvec[0] == "truthtable" && inputvec.size() >= 2)
				{
					//Build the string to tokenize
					std::string fargsdef = "";

					for(int i = 1; i < inputvec.size(); i++)
					{
						fargsdef += inputvec[i] + " ";
					}

					//Split the arguments and definition
					std::vector<std::string> argsdef = PLStringSplit(fargsdef, ";", false);

					if(argsdef.size() != 2)
						throw syntaxerror;

					std::vector<char> variable_names;

					//Parse the list of arguments
					std::vector<std::string> args = PLStringSplit(argsdef[0], ",", false);

					if(args.size() < 1)
						throw syntaxerror;

					std::vector<std::string> formulas = PLStringSplit(argsdef[1], ",", false);

					if(formulas.size() < 1)
						throw syntaxerror;

					
					pli_truth_table_gen(args, formulas, variables, mode);

					
				}
				//We need to generate a table of values
				else if(inputvec[0] == "tablegen" && inputvec.size() == 4)
				{
					std::string name = inputvec[1];

					int begin = atoi(inputvec[2].c_str());
					int end = atoi(inputvec[3].c_str());

					if(end < begin)
						throw syntaxerror;

					for(int i = begin; i <= end; i++)
					{
						std::stringstream ss;
						ss << i;

						newans = pli_evaluate_function(functions[pli_get_function_id_by_name(name)], ss.str());

						if((begin < 0 || end < 0) && i >= 0)
							std::cout << " " << i << "\t";
						else
							std::cout << i << "\t";

						if(fmod(newans, 1) == 0)
							if( newans >= 0)
								printf(" %0.0Lf\n", newans);
							else
								printf("%0.0Lf\n", newans);
						else
						{
							if( newans >= 0)
								std::cout <<  " " << newans << std::endl;
							else
								std::cout <<  newans << std::endl;
						}
					}
				}
				//We need to save and functions to a file
				else if(inputvec[0] == "save" && inputvec.size() == 2)
				{
					pli_save_vars_funcs_to_file(inputvec[1]);
				}
				//We need to load a file
				else if(inputvec[0] == "load" && inputvec.size() == 2)
				{
					newans = pli_load_vars_funcs_from_file(inputvec[1], ans);
				}
				//We need to set or display the trig mode
				else if(inputvec[0] == "mode")
				{
					if(inputvec.size() == 2)
					{
						if(inputvec[1] == "boolean")
						{
							mode = boolean;
						}
						else if(inputvec[1] == "lukasiewicz_ternary")
						{
							mode = lukasiewicz_ternary;
						}
						else
						{
							std::cout << "Info: Valid logic modes: boolean, lukasiewicz_ternary." << std::endl;
							throw syntaxerror;
						}
					}

					pli_report_logic_mode();
				}
				else
				{
					replaceAll(input,"ans",std::to_string(ans));
					newans = pl_evaluate_math_expression(input, variables, mode);
					
					if(fmod(newans, 1) == 0)
						printf("%0.0Lf\n", newans);
					else
						std::cout <<  newans << std::endl;
				}
			}
			catch(PLException err)
			{
				exceptionresult(err, exitonexception);
			}
			catch(...)
			{
				std::cout << "Error: Unknown error." << std::endl;
			}
			
		}
		
		return newans;
}

void evaluateCmdInput(int argc, char **argv)
{
	std::string input;
	
	for(int i = 1; i < argc; i++)
	{
		input += std::string(argv[i]);
	}
	
	evaluateInput(input, 0, true);
}

void pli_report_logic_mode()
{
	switch(mode)
	{
	case boolean:
	{
		std::cout << "Logic mode: boolean" << std::endl;
		break;
	}
	case lukasiewicz_ternary:
	{
		std::cout << "Logic mode: lukasiewicz_ternary" << std::endl;
		break;
		
	}
	}
}

bool pli_file_exists(std::string fname)
{
	FILE *file;
    if (file = fopen(fname.c_str(), "r"))
    {
        fclose(file);
        return 1;
    }
    return 0;
}
