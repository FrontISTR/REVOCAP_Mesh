#pragma once

#include <cstdlib>

double drand(double max=1.0,double min=0.0)
{
	return min + rand() * (max-min) / RAND_MAX;
}