#include "pluslogic.h"

double pl_unary_unimp(double a)
{
	throw modeerror;
	return 0;
}

double pl_unimp(double a, double b)
{
	throw modeerror;
	return 0;
}


PLLogicType pl_get_logic_type_data(PLLogicMode mode)
{
	PLLogicType nil;

	
	switch(mode)
	{
	case boolean:
	{
		PLLogicType t;

		t.pl_not = &boolean_not;
		t.pl_undef = &pl_unary_unimp;
		t.pl_notfalse = &pl_unary_unimp;
		t.pl_istrue = &pl_unary_unimp;
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
	case lukasiewicz_ternary:
	{
		PLLogicType t;

		t.pl_not = &lukasiewicz_not;
		t.pl_undef = &lukasiewicz_undef;
		t.pl_notfalse = &lukasiewicz_notfalse;
		t.pl_istrue = &lukasiewicz_istrue;
		t.pl_and = &lukasiewicz_and;
		t.pl_or = &lukasiewicz_or;
		t.pl_xor = &pl_unimp;
		t.pl_nor = &lukasiewicz_nor;
		t.pl_nand = &lukasiewicz_nand;
		t.pl_imp = &lukasiewicz_imp;
		t.pl_eq = &lukasiewicz_eq;

		t.permissible_numbers.push_back(-1);
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
		      
