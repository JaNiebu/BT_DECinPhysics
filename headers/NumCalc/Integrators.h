#ifndef Integrators
#define Integrators

#include "../headers.h"

using namespace std;

double LineIntegral( function< array<double,2>(array<double,2>,double)> field , double alpha , array<double,2> initial_point, array<double,2> final_point );
double LineFluxIntegral( function< array<double,2>(array<double,2>,double)> field , double alpha , array<double,2> initial_point, array<double,2> final_point );

#endif