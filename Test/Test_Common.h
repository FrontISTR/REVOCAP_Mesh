#pragma once

#include <cfloat>
#include <ctime>
#include <cstdlib>

void irand(void)
{
	srand(static_cast<unsigned int>(time(NULL)));
}

double drand(double max=1.0,double min=0.0)
{
	return min + rand() * (max-min) / RAND_MAX;
}