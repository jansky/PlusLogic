#include "pluslogic.h"


PLLogicType pl_get_logic_type_data(PLLogicMode mode)
{
	PLLogicType nil;

	
	switch(mode)
	{
	case boolean:
	{
		PLLogicType t;

		t.pl_not = &boolean_not;
		t.pl_and = &boolean_and;
		t.pl_or = &boolean_or;
		t.pl_xor = &boolean_xor;
		t.pl_nor = &boolean_nor;
		t.pl_nand = &boolean_nand;
		t.pl_imp = &boolean_imp;
		t.pl_eq = &boolean_eq;

		t.permissible_numbers.push_back(0);
		t.permissible_numbers.push_back(1);

		return t;
		break;
	}
	default:
	{
		throw modeerror;

		return nil;
		break;
	}
	}
}
		      
