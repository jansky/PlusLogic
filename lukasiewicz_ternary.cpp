#include "pluslogic.h"

double lukasiewicz_not(double a)
{
	if(a == -1)
		return 1;
	else if(a == 0)
		return 0;
	else
		return -1;
}

double lukasiewicz_and(double a, double b)
{
	if(a == -1 || b == -1)
		return -1;

	if(a == 0 || b == 0)
		return 0;

	if(a == 1 && b == 1)
		return 1;

	return 2;
}

double lukasiewicz_or(double a, double b)
{
	if(a == -1 && b == -1)
		return -1;

	if(a == 1 || b == 1)
		return 1;

	return 0;
}

double lukasiewicz_imp(double a, double b)
{
	if(b == 1)
		return 1;

	if(a == -1)
		return 1;

	if(a == 0 && b == 0)
		return 1;

	if(b == 0)
		return 0;

	if(a == 0)
		return 0;

	return -1;
}


double lukasiewicz_nand(double a, double b)
{
	return lukasiewicz_not(lukasiewicz_and(a,b));
}

double lukasiewicz_nor(double a, double b)
{
	return lukasiewicz_not(lukasiewicz_or(a,b));
}

double lukasiewicz_eq(double a, double b)
{
	return lukasiewicz_and(lukasiewicz_imp(a,b),lukasiewicz_imp(b,a));
}

double lukasiewicz_undef(double a)
{
	if(a == 0)
		return 1;
	else
		return -1;
}

double lukasiewicz_notfalse(double a)
{
	if(a == -1)
		return -1;
	else
		return 1;
}

double lukasiewicz_istrue(double a)
{
	if(a == 1)
		return 1;
	else
		return -1;
}




	

	
