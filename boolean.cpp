double boolean_not(double a)
{
	return !a;
}

double boolean_and(double a, double b)
{
	return (a&&b);
}

double boolean_or(double a, double b)
{
	return (a||b);
}

double boolean_xor(double a, double b)
{
	return ((a||b)&&(!(a&&b)));
}

double boolean_nand(double a, double b)
{
	return (!(a&&b));
}

double boolean_nor(double a, double b)
{
	return (!(a||b));
}

double boolean_imp(double a, double b)
{
	return ((!a)||(b));
}

double boolean_eq(double a, double b)
{
	return (a==b);
}


