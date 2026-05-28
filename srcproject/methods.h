#ifndef METHODS_H
#define METHODS_H

#include "finpro.h"

MethodResult methodBisection(FunctionConfig *cfg, StopCriteria sc);
MethodResult methodFalsePosition(FunctionConfig *cfg, StopCriteria sc);
MethodResult methodNewtonRaphson(FunctionConfig *cfg, StopCriteria sc);
MethodResult methodSecant(FunctionConfig *cfg, StopCriteria sc);
int shouldContinue(StopCriteria sc, int iter, double ea, double xr);
int shouldRealloc(MethodResult *res);

#endif