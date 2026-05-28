#ifndef FINPRO_H
#define FINPRO_H

#define _USE_MATH_DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include <float.h>
#include <windows.h>

#define MAX_ITER 100        // batas iterasi default dan constraint iter maksimum
#define DEFAULT_ES 0.0001   // stopping error default (0.01%) dan constraint ea minimum

/* INISIALISASI ENUM, UNION, STRUCT */

// enum digunakan untuk memberi nama konstanta integer agar kode lebih mudah dibaca dan di-switch-case

typedef enum {
    POLYNOMIAL = 1,
    EULER
} FunctionType;

typedef enum {
    LINEAR = 1,
    QUADRATIC,
    CUBIC
} PolynomialDegree;

typedef enum {
    FOUR = 4,
    SIX = 6,
    EIGHT = 8
} DecimalNumbers;

typedef enum {
    BISECTION = 1,
    FALSE_POSITION,
    NEWTON_RAPHSON,
    SECANT
} RootMethods;

typedef enum {
    ITER = 1,
    EA,
    BOTH
} StopMode;

typedef enum {
    EXIT = 0,
    CALCULATE
} ExitChoice;

// union digunakan karena program hanya menyimpan satu jenis fungsi dalam satu waktu,
// sehingga memori dapat digunakan bersama antar member tanpa pemborosan

typedef union {
    double linear[2];
    double quadratic[3];
    double cubic[4];
    double euler[4];
} FunctionParameter;

// struct digunakan untuk mengelompokkan variabel-variabel yang saling berkaitan menjadi satu tipe data
// pointer ke struct digunakan agar perubahan nilai di dalam fungsi dapat mempengaruhi data aslinya

typedef struct {
    FunctionType funcType;
    PolynomialDegree degree;
    DecimalNumbers decimal;
    FunctionParameter param;
    int methodSelected[4];
    double xl, xu, xi_nr, xi_1, xi_sec;
    double xt_lin, xt_quad, xt_cube;
    ExitChoice exitChoice;
} FunctionConfig;

typedef struct {
    char *funcName, *funcFormat, *funcCondition, **varNames;    
    int numCoef;       
    double *coef;   
    void (*printFunction)(char *name, double *coef, int decimal);  
    void (*printTrueRoot)(double *coef, int decimal, FunctionConfig *cfg);
} FunctionMeta;

typedef struct {
    StopMode mode;
    int maxIter;
    double stopError;
} StopCriteria;

typedef struct {
    int iter;
    double xl, xu, xi_1, xi, xr, ea, et;
} IterationResult;

typedef struct {
    IterationResult *rows;
    int count, capacity;
    double root, finalEa, finalEt;
} MethodResult;

typedef struct {
    const char *name;
    int iterations, convStatus;
    double xr, ea, et;
} SummaryEntry;














#endif // FINPRO_H