#include "pluslogic.h"
#include "pluslogicint.h"
#include <iostream>

void pli_truth_table_gen(std::vector<std::string> var_names, std::vector<std::string> formulas, std::vector<PLVariable> variables, PLLogicMode mode)
{
	//PLLogicType ltype = pl_get_logic_type_data(mode);

	if(var_names.size() != 2)
		std::cout << "failure.";

	std::vector<PLVariable> vars = variables;
	std::vector<int> to_remove;

	for(int i = 0; i < vars.size(); i++)
	{
		for(int j = 0; j < var_names.size(); j++)
		{
			if(vars[i].name == var_names[j][0])
				to_remove.push_back(i);
		}
	}

	for(int i = 0; i < to_remove.size(); i++)
	{
		vars.erase(vars.begin()+i);
	}

	//Output the table headers
	for(int i = 0; i < var_names.size(); i++)
	{
		std::cout << var_names[i][0] << "\t";
			
	}

	for(int i = 0; i < formulas.size(); i++)
	{
		std::cout << formulas[i];

		if(i != (formulas.size() - 1))
			std::cout << "\t";
	}

	std::cout << std::endl << std::endl;

	double start = 0;
	double end = 0;
			     

	

	switch(mode)
	{
	case boolean:
	{
		start = 0;
		end = 1;
		
		break;
	}
	case lukasiewicz_ternary:
	{
		start = -1;
		end = 1;
		break;
	}
	default:
	{
		
		throw modeerror;
	}
	}

	for(int i = start; i <= end; i++)
	{
		for(int j = start; j <= end; j++)
		{
			vars.push_back(pl_create_variable(i, var_names[0][0]));
			vars.push_back(pl_create_variable(j, var_names[1][0]));

			std::cout << i << "\t" << j << "\t";

			for(int k = 0; k < formulas.size(); k++)
			{
				std::cout << pl_evaluate_math_expression(formulas[k], vars, mode);

				if(k != (formulas.size() - 1))
					std::cout << "\t";
			}

			std::cout << std::endl;

			vars.pop_back();
			vars.pop_back();
		}
	}
}

	
