#ifndef EVALUATE_H
#define EVALUATE_H
#include "finpro.h"

double evaluateFunction(FunctionConfig *cfg, double x);
double evaluateDerivative(FunctionConfig *cfg, double x);
double findClosestRoot(FunctionConfig *cfg, double xr);
double calcTrueError(FunctionConfig *cfg, double xr);
double calcTrueErrorRelative(FunctionConfig *cfg, double xr);


#endif