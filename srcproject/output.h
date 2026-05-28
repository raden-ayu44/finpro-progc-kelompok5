#ifndef OUTPUT_H
#define OUTPUT_H

#include "finpro.h"

void printCenteredStr(const char *str, int colWidth);
void printCenteredVal(double val, int dec, int colWidth);
int columnWidthIter(MethodResult *res, int dec);
void printHLine(int critCol, const char *left, const char *mid, const char *right,
                const char *line, int col, int nCols);
void printAnalysis(FunctionConfig *cfg, MethodResult *res, StopCriteria sc,
                   const char *methodName, int *convStatus);
void printBisectionFalsePosition(FunctionConfig *cfg, StopCriteria sc, MethodResult *res);
void printNewtonRaphson(FunctionConfig *cfg, StopCriteria sc, MethodResult *res);
void printSecant(FunctionConfig *cfg, StopCriteria sc, MethodResult *res);
int columnWidthSumm(SummaryEntry *summary, int count, int dec);
void printSummaryTable(FunctionConfig *cfg, StopCriteria sc,
                       SummaryEntry summary[], int count);
void printScoringTable(FunctionConfig *cfg, SummaryEntry summary[], int count);
void printLinearFunction(char *n, double *c, int decimal);
void printQuadraticFunction(char *n, double *c, int decimal);
void printCubicFunction(char *n, double *c, int decimal);
void printEulerFunction(char *n, double *c, int decimal);
void printTrueRootLinear(double *c, int decimal, FunctionConfig *cfg);
void printTrueRootQuadratic(double *c, int decimal, FunctionConfig *cfg);
void printTrueRootCubic(double *c, int decimal, FunctionConfig *cfg);
void printTrueRootEuler(double *c, int decimal, FunctionConfig *cfg);



#endif 