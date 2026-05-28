#ifndef INPUT_H
#define INPUT_H

#include "finpro.h"

void clearInputBuffer(void);
int scanInt(int *out);
int scanDouble(double *out);

void inputFunctionType(FunctionConfig *cfg);
void inputPolynomialDegree(FunctionConfig *cfg);
void inputDecimalNumbers(FunctionConfig *cfg);
void inputFunctionParameter(FunctionConfig *cfg);
void inputRootMethods(FunctionConfig *cfg);
void inputStopMode(FunctionConfig *cfg, StopCriteria *sc);
void inputExitChoice(FunctionConfig *cfg);
void inputBracket(FunctionConfig *cfg);
void inputGuessNewtonRaphson(FunctionConfig *cfg);
void inputGuessSecant(FunctionConfig *cfg);

#endif