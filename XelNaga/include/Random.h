#pragma once
#include <BWAPI.h>
#include <cstdlib>
#include <ctime>
#include <cmath>

class Random
{
public:
	Random();
	Random(double);
	Random(int);

	void setMaxDouble				(double);
	void setMaxInt					(int);

	double	genDouble				();
	
	double	nextDouble			();
	int			nextInt					();

	double	nextAnotherDouble	();
	int			nextAnotherInt		();

private:
	double	maxDouble;
	int			maxInt;

	double	lastDouble;
	int			lastInt;
};