#ifndef SIMDATA_H
#define SIMDATA_H

#include "finpro.h"


void inputShowGraph(FunctionConfig *cfg, StopCriteria *sc, MethodResult results[], int index);
void writeSimData(const FunctionConfig *cfg, const StopCriteria *sc, const MethodResult results[4]);




#endif // SIMDATA_H