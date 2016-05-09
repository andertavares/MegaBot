#include <Random.h>

Random::Random()
{
	maxDouble		= 0;
	maxInt			= 0;
	lastDouble	= 0;
	lastInt			= 0;
	srand((unsigned int)time(NULL));
}

Random::Random(double maxDouble)
{
	this->maxDouble = maxDouble;
	lastDouble			= 0;
	srand((unsigned int)time(NULL));
}

Random::Random(int maxInt)
{
	this->maxInt	= maxInt;
	lastInt				= 0;	
	srand((unsigned int)time(NULL));
}

void Random::setMaxDouble(double maxDouble)
{
	this->maxDouble = maxDouble;
}

void Random::setMaxInt(int maxInt)
{
	this->maxInt = maxInt;
}

/**
*	Generate a random double in [0,1[
*/		
double Random::genDouble()
{
	return ((double) rand() / (RAND_MAX + 1.0));
}

/**
* generate a random double in [0, maxDouble[
*/
double Random::nextDouble()
{
	lastDouble = maxDouble * genDouble();
	return lastDouble;
}

/**
* generate a random integer in [0, maxInt[
*/
int Random::nextInt()
{
	lastInt = (int)(maxInt * genDouble());
	return lastInt;
}

/**
* generate a random double in [0, maxDouble[
* different from lastDouble
*/
double Random::nextAnotherDouble()
{
	double random;
	double last = lastDouble;
	
	do
	{
		random = nextDouble();
	}
	while (random == last);

	return random;
}

/**
* generate a random integer in [0, maxInt[
* different from lastInt
*/
int Random::nextAnotherInt()
{
	int random;
	int last = lastInt;

	do
	{
		random = nextInt();
	}
	while (random == last);

	return random;
}