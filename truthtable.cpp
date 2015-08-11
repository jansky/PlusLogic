#include "pluslogic.h"
#include "pluslogicint.h"
#include <iostream>

void pli_truth_table_gen(std::vector<std::string> var_names, std::vector<std::string> formulas, std::vector<PLVariable> variables, PLLogicMode mode)
{
	PLLogicType ltype = pl_get_logic_type_data(mode);

	if(var_names.size() != ltype.permissible_numbers.size())
		throw modeerror;

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
			     

	

	switch(mode)
	{
	case boolean:
	{
		for(int i = 0; i <= 1; i++)
		{
			for(int j = 0; j <= 1; j++)
			{
				vars.push_back(pl_create_variable(i, var_names[0][0]));
				vars.push_back(pl_create_variable(j, var_names[1][0]));

				std::cout << i << "\t" << j << "\t";

				for(int k = 0; k < formulas.size(); k++)
				{
					std::cout << pl_evaluate_math_expression(formulas[k], vars, boolean);

					if(k != (formulas.size() - 1))
						std::cout << "\t";
				}

				std::cout << std::endl;

				vars.pop_back();
				vars.pop_back();
			}
		}

		break;
	}
	default:
	{
		
		throw modeerror;
	}
	}
}

	
