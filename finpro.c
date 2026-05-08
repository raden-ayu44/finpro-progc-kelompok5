//```c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_ITER 100        /* batas iterasi default */
#define DEFAULT_ES 0.0001   /* stopping error default (0.01%) */
#define TABLE_COLS 6

/* ENUM */

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

/* UNION */

typedef union {
    double linear[2];
    double quadratic[3];
    double cubic[4];
    double euler[4];
} FunctionParameter;

/* STRUCT */

typedef struct {
    FunctionType funcType;
    PolynomialDegree degree;
    DecimalNumbers decimal;
    FunctionParameter param;
    int methodSelected[4];
    double xl, xu, x0, x1;
    ExitChoice exitChoice;
} FunctionConfig;

typedef struct {
    StopMode mode;
    int maxIter;
    double stopError;
} StopCriteria;

typedef struct {
    char *funcName;      
    char *funcFormat;
    char *funcCondition;    
    char **varNames;    
    int numCoef;       
    double *coef;   
    void (*printTrueRoot)(double *coef, int decimal);
    void (*printFunction)(char *name, double *coef, int decimal);  
} FuncMeta;

/* FUNGSI INPUT */

void inputFunctionType(FunctionConfig *cfg) {
    int funcType;
    char *funcNames[] = {"Polinomial", "Euler"};
    printf("Pilihan Jenis Fungsi [ Masukkan Angka 1/2 ]\n\n");
    printf("[1] Fungsi Polinomial  :  ax^n + bx^(n-1) + ... + d\n");
    printf("[2] Fungsi Euler       :  e^(ax+b) + cx + d\n\n");
    printf("Pilihan Anda           :  "); scanf("%d", &funcType); printf("\n");
    while (funcType != 1 && funcType != 2) {
        printf("Pilihan Anda tidak valid! Tolong masukkan angka 1/2.\n\n"); 
        printf("Pilihan Anda           :  "); scanf("%d", &funcType); printf("\n");
    }
    cfg->funcType = (FunctionType)funcType;
    printf("Anda memilih Fungsi %s!\n\n", funcNames[funcType - 1]); 
}

void inputPolynomialDegree(FunctionConfig *cfg) {
    int degree;
    char *degreeNames[] = {"Linier", "Kuadrat", "Kubik"};
    printf("Pilihan Derajat Polinomial [ Masukkan Angka 1/2/3 ]\n\n");
    printf("[1] Linier     :  ax + b\n");
    printf("[2] Kuadratik  :  ax^2 + bx + c\n");
    printf("[3] Kubik      :  ax^3 + bx^2 + cx + d\n\n");
    printf("Pilihan Anda   :  "); scanf("%d", &degree); printf("\n");
    while (degree != 1 && degree != 2 && degree != 3) {
        printf("Pilihan Anda tidak valid! Tolong masukkan angka 1/2/3.\n\n"); 
        printf("Pilihan Anda   :  "); scanf("%d", &degree); printf("\n");
    }
    cfg->degree = (PolynomialDegree)degree;
    printf("Anda memilih %s!\n\n", degreeNames[degree - 1]);
}

void inputDecimalNumbers(FunctionConfig *cfg) {
    int decimal;
    printf("Pilihan Pengaturan Tampilan Desimal [ Masukkan Angka 4/6/8 ]\n\n");
    printf("[4] 4 Angka Desimal  :  X.XXXX\n");
    printf("[6] 6 Angka Desimal  :  X.XXXXXX\n");
    printf("[8] 8 Angka Desimal  :  X.XXXXXXXX\n\n");
    printf("Pilihan Anda         :  "); scanf("%d", &decimal); printf("\n");
    while (decimal != 4 && decimal != 6 && decimal != 8) {
        printf("Pilihan Anda tidak valid! Tolong masukkan angka 4/6/8.\n\n"); 
        printf("Pilihan Anda         :  "); scanf("%d", &decimal); printf("\n");
    }
    cfg->decimal = (DecimalNumbers)decimal;
    printf("Anda memilih Tampilan %d Angka Desimal!\n\n", decimal);
}

// FUNGSI PRINT UNTUK FUNGSI INPUT PARAMETER

void printLinearFunction(char *n, double *c, int decimal) {
    char fmt[100];
    sprintf(fmt, "Fungsi %%s Anda          :  f(x) = %%.%dfx + %%.%df\n", decimal, decimal);
    printf(fmt, n, c[0], c[1]);
}

void printQuadraticFunction(char *n, double *c, int decimal) {
    char fmt[100];
    sprintf(fmt, "Fungsi %%s Anda       :  f(x) = %%.%dfx^2 + %%.%dfx + %%.%df\n", decimal, decimal, decimal);
    printf(fmt, n, c[0], c[1], c[2]);
}

void printCubicFunction(char *n, double *c, int decimal) {
    char fmt[100];
    sprintf(fmt, "Fungsi %%s Anda  :  f(x) = %%.%dfx^3 + %%.%dfx^2 + %%.%dfx + %%.%df\n", decimal, decimal, decimal, decimal);
    printf(fmt, n, c[0], c[1], c[2], c[3]);
}

void printEulerFunction(char *n, double *c, int decimal) {
    char fmt[100];
    sprintf(fmt, "Fungsi %%s Anda  :  f(x) = e^(%%.%dfx + %%.%df) + %%.%dfx + %%.%df\n", decimal, decimal, decimal, decimal);
    printf(fmt, n, c[0], c[1], c[2], c[3]);
}

void printTrueRootLinear(double *c, int decimal) {
    char fmt[100];
    double xt = -(c[1] / c[0]);
    sprintf(fmt, "True Root f(x)              :  xt = %%.%df\n", decimal); printf(fmt, xt);
    sprintf(fmt, "Root Pembanding True Error  :  xt = %%.%df\n\n", decimal); printf(fmt, xt);
}

void printTrueRootQuadratic(double *c, int decimal) {
    char fmt[100];
    double disc = c[1] * c[1] - 4 * c[0] * c[2];
    double x1 = (-c[1] + sqrt(disc)) / (2 * c[0]);
    double x2 = (-c[1] - sqrt(disc)) / (2 * c[0]);
    double xt;
    sprintf(fmt, "True Roots f(x)             :  xt_x1 = %%.%df | xt_x2 = %%.%df\n", decimal, decimal); printf(fmt, x1, x2);
    if (x1 >= x2) {
        xt = x1;
    } else {
        xt = x2;
    } 
    sprintf(fmt, "Root Pembanding True Error  :  xt = %%.%df\n\n", decimal); 
    printf(fmt, xt);
}

void printTrueRootNumeric(double *c, int decimal) {
    printf("※  Fungsi ini tidak menggunakan True Root untuk menghitung True Error,\n");
    printf("   melainkan menggunakan Root dari Komputasi Numerik dengan Error Approximate terkecil.\n\n");
}
             
// END OF: FUNGSI PRINT UNTUK FUNGSI INPUT PARAMETER

void inputFunctionParameter(FunctionConfig *cfg) {
    FuncMeta meta;
    int dec = cfg->decimal;
    switch (cfg->funcType) {
        case POLYNOMIAL:
	        switch (cfg->degree) {
		        case LINEAR:
			        meta = (FuncMeta){
                        .funcName = "Linier",
                        .funcFormat = "ax + b",
                        .funcCondition = "a dan b Tidak Boleh 0",
                        .varNames = (char*[]){"a", "b"},
                        .numCoef = 2,
                        .coef = cfg->param.linear,
                        .printFunction = printLinearFunction,
                        .printTrueRoot = printTrueRootLinear
                    };
                    break;
                case QUADRATIC:
			        meta = (FuncMeta){
                        .funcName = "Kuadratik",
                        .funcFormat = "ax^2 + bx + c",
                        .funcCondition = "a dan b Tidak Boleh 0",
                        .varNames = (char*[]){"a", "b", "c"},
                        .numCoef = 3,
                        .coef = cfg->param.quadratic,
                        .printFunction = printQuadraticFunction,
                        .printTrueRoot = printTrueRootQuadratic
                    };
                    break;
                case CUBIC:
			        meta = (FuncMeta){
                        .funcName = "Kubik",
                        .funcFormat = "ax^3 + bx^2 + cx + d",
                        .funcCondition = "a Tidak Boleh 0",
                        .varNames = (char*[]){"a", "b", "c", "d"},
                        .numCoef = 4,
                        .coef = cfg->param.cubic,
                        .printFunction = printCubicFunction,
                        .printTrueRoot = printTrueRootNumeric
                    };
                    break;
                default: return;
            }
            break;
        case EULER:
            meta = (FuncMeta){
                .funcName = "Euler",
                .funcFormat = "e^(ax+b) + cx + d",
                .funcCondition = "a Tidak Boleh 0",
                .varNames = (char*[]){"a", "b", "c", "d"},
                .numCoef = 4,
                .coef = cfg->param.euler,
                .printFunction = printEulerFunction,
                .printTrueRoot = printTrueRootNumeric
            };
            break; 
        default: return;  
    }
    printf("Masukkan Koefisien Fungsi %s [ %s | %s ]\n\n", meta.funcName, meta.funcFormat, meta.funcCondition);
    for (int i = 0; i < meta.numCoef; i++) {
        printf("%s = ", meta.varNames[i]); scanf("%lf", &meta.coef[i]);
        if (strcmp(meta.funcName, "Linier") == 0 || i == 0) {
            while (meta.coef[i] == 0.00) {
                printf("\n"); printf("Input tidak valid! %s tidak boleh 0.", meta.varNames[i]); printf("\n\n"); 
                printf("%s = ", meta.varNames[i]); scanf("%lf", &meta.coef[i]);
            }
        }
    }
    if (strcmp(meta.funcName, "Kuadratik") == 0) {
        double disc = meta.coef[1] * meta.coef[1] - 4 * meta.coef[0] * meta.coef[2];
        while (disc < 0) {
            printf("\n"); printf("Input tidak valid! Diskriminan negatif (b^2 - 4ac = %.f).\n", disc);
            printf("Fungsi tidak memiliki akar real. Tolong input ulang koefisien.\n\n");
            for (int i = 0; i < meta.numCoef; i++) {
                printf("%s = ", meta.varNames[i]); scanf("%lf", &meta.coef[i]);
                if (i == 0) {
                    while (meta.coef[i] == 0.00) {
                        printf("\n"); printf("Input tidak valid! %s tidak boleh 0.", meta.varNames[i]); printf("\n");
                        printf("%s = ", meta.varNames[i]); scanf("%lf", &meta.coef[i]);
                    }
                }
            }
            disc = meta.coef[1] * meta.coef[1] - 4 * meta.coef[0] * meta.coef[2];
        }
    }    
    printf("\n");
    meta.printFunction(meta.funcName, meta.coef, dec);
    meta.printTrueRoot(meta.coef, dec);
}

void inputRootMethods(FunctionConfig *cfg) {
    char *methodNames[] = {"Bisection", "False-Position", "Newton-Raphson", "Secant"};
    int pick = 1;
    for (int i = 0; i < 4; i++) {
        cfg->methodSelected[i] = 0;
    }
    printf("Pilihan Metode Komputasi Numerik [ Masukkan Angka 0/1/2/3/4 ]\n\n");
    printf("Anda boleh memilih lebih dari satu Metode!\n");
    printf("※  Anda dapat membatalkan pilihan Metode dengan memasukkan angka yang sama.\n");
    printf("   Semua pilihan Anda akan otomatis tersimpan setelah pemilihan ke-9 kali.\n\n");
    printf("[1] Bisection       :  Membutuhkan xl (batas bawah) dan xu (batas atas)\n");
    printf("[2] False-Position  :  Membutuhkan xl (batas bawah) dan xu (batas atas)\n");
    printf("[3] Newton-Raphson  :  Membutuhkan x0 (tebakan awal)\n");
    printf("[4] Secant          :  Membutuhkan x0 (tebakan pertama) dan x1 (tebakan kedua)\n");
    printf("[0] Selesai\n\n");
    int method;
    do {
        printf("Pilihan ke-%d Anda   :  ", pick); scanf("%d", &method); printf("\n");
        if (method >= 1 && method <= 4) {
            cfg->methodSelected[method - 1] ^= 1;
            if (cfg->methodSelected[method - 1] == 0) {
                printf("Anda membatalkan Metode %s!\n\n", methodNames[method - 1]); pick--;
            } else {
                printf("Anda memilih Metode %s!\n\n", methodNames[method - 1]);
            }
            pick++;
            if (pick > 9) {
                printf("Anda memilih Metode :\n");
                int n = 1;
                for (int j = 0; j < 4; j++) {
                    if (cfg->methodSelected[j]) {
                        printf("%d. %s\n", n++, methodNames[j]);
                    }
                }
                printf("\n");
                method = 0;
            }
        } else if (method == 0) {
            int m = 0;
            for (int j = 0; j < 4; j++) {
                if (cfg->methodSelected[j]) {
                    m = 1; break;
                }
            }
            if (m == 0) {
                printf("Anda belum membuat pilihan! Tolong masukkan angka 1/2/3/4.\n\n"); method = -1;
            } else {
                printf("Anda memilih Metode :\n");
                int n = 1;
                for (int j = 0; j < 4; j++) {
                    if (cfg->methodSelected[j]) {
                        printf("%d. %s\n", n++, methodNames[j]); 
                    }
                }
                printf("\n");
            }
        } else {
            printf("Pilihan tidak valid! Tolong masukkan angka 0/1/2/3/4.\n\n");
        }
    } while (method != 0);
}

void inputStopMode(FunctionConfig *cfg, StopCriteria *sc) {
    char *criteriaNames[] = {"Input Max Iteration", "Input % Stopping Error", "Keduanya"};
    int stop;
    char fmt[100];
    int dec = cfg->decimal;
    printf("Pilihan Mode Berhenti Iterasi [ Masukkan Angka 1/2/3 ]\n\n");
    printf("[1] Input Max Iteration     :  Pencarian akar berhenti saat iter = max_iter\n");
    printf("[2] Input %% Stopping Error  :  Pencarian akar berhenti saat %%ea > %%es\n");
    printf("[3] Keduanya                :  Pencarian akar berhenti saat salah satu terpenuhi\n\n");
    printf("Pilihan Anda                :  "); scanf("%d", &stop); printf("\n");
    while (stop != 1 && stop != 2 && stop != 3) {
        printf("Pilihan Anda tidak valid! Tolong masukkan angka 1/2/3.\n\n"); 
        printf("Pilihan Anda                :  "); scanf("%d", &stop); printf("\n");
    }
    sc->mode = (StopMode)stop;
    printf("Anda memilih %s!\n\n", criteriaNames[stop - 1]);
    if (stop == ITER || stop == BOTH) {
        printf("max_iter [ 1 - %d ] = ", MAX_ITER); scanf("%d", &sc->maxIter); printf("\n");
        while (sc->maxIter < 1 || sc->maxIter > MAX_ITER) {
            printf("Input tidak valid! max_iter harus di antara 1 - %d.\n\n", MAX_ITER);
            printf("max_iter [ 1 - %d ] = ", MAX_ITER); scanf("%d", &sc->maxIter); printf("\n");
        }
        if (stop == ITER) {
            printf("※  Pencarian akar berhenti saat iterasi mencapai ke-%d kali.\n\n", sc->maxIter);
        }
    }
    if (stop == EA || stop == BOTH) {
        printf("%%es [ %g%% - 100.00%% ] = ", DEFAULT_ES * 100); scanf("%lf", &sc->stopError); printf("\n");
        while (sc->stopError < DEFAULT_ES *100 || sc->stopError > 100.0) {
            printf("Input tidak valid! %%es harus di antara %g%% (%g) - 100.00%%.\n\n", DEFAULT_ES * 100, DEFAULT_ES);
            printf("%%es [ %g%% - 100.00%% ] = ", DEFAULT_ES * 100); scanf("%lf", &sc->stopError); printf("\n");
        }
        if (stop == EA) {
            sprintf(fmt, "※  Pencarian akar berhenti saat approximate error melebihi %%g%%%% (%%.%df).\n\n", dec);
            printf(fmt, sc->stopError, sc->stopError / 100);
        }
    } 
    if (stop == BOTH) {
        printf("※  Pencarian akar berhenti saat iterasi mencapai ke-%d kali atau\n", sc->maxIter);
        sprintf(fmt,"   saat approximate error melebihi %%g%%%% (%%.%df).\n\n", dec);
        printf(fmt, sc->stopError, sc->stopError / 100);
    }
}

void inputExitChoice(FunctionConfig *cfg) {
    int exitChoice;
    printf("Apakah Anda ingin melanjutkan? [ Masukkan Angka 1/0 ]\n\n"); 
    printf("[1] Lanjut\n");
    printf("[0] Keluar Program\n\n");
    printf("Pilihan Anda  :  "); scanf("%d", &exitChoice); printf("\n");
    while (exitChoice != 0 && exitChoice != 1) {
        printf("Pilihan tidak valid! Tolong masukkan angka 1/0.\n\n"); 
        printf("Pilihan Anda  :  "); scanf("%d", &exitChoice ); printf("\n");
    }
    cfg->exitChoice = (ExitChoice)exitChoice;
    if (exitChoice == CALCULATE) {
        printf("Hore! Anda memilih Lanjut!\n\n");
    } else {
        printf("Anda memilih Keluar Program!\n\n"); 
        printf("Terima kasih sudah menggunakan Kalkulator Root Finding kami!\n\n");
    }    
}

/* FUNGSI EVALUASI NILAI X DAN FUNGSI TURUNAN */

double evaluateFunction(FunctionConfig *cfg, double x) {
    switch (cfg->funcType) {
        case POLYNOMIAL:
	        switch (cfg->degree) {
		        case LINEAR:
			        return cfg->param.linear[0] * x + cfg->param.linear[1];
                case QUADRATIC:
	                return cfg->param.quadratic[0] * pow(x,2) + cfg->param.quadratic[1] * x + cfg->param.quadratic[2];
                case CUBIC:
	                return cfg->param.cubic[0] * pow(x,3) + cfg->param.cubic[1] * pow(x,2) + cfg->param.cubic[2] * x + cfg->param.cubic[3];
                default:
                   return 0.0; 
            }
        case EULER:
            return exp(cfg->param.euler[0] * x + cfg->param.euler[1]) + cfg->param.euler[2] * x + cfg->param.euler[3];
        default:
            return 0.0;
    }
}

double evaluateDerivative(FunctionConfig *cfg, double x) {
    switch (cfg->funcType) {
        case POLYNOMIAL:
	        switch (cfg->degree) {
		        case LINEAR:
			        return cfg->param.linear[0];
                case QUADRATIC:
	                return 2 * cfg->param.quadratic[0] * x + cfg->param.quadratic[1];
                case CUBIC:
	                return 3 * cfg->param.cubic[0] * pow(x,2) + 2 * cfg->param.cubic[1] * x + cfg->param.cubic[2];
                default:
                   return 0.0; 
            }
        case EULER:
            return cfg->param.euler[0] * exp(cfg->param.euler[0] * x + cfg->param.euler[1]) + cfg->param.euler[2];
        default:
            return 0.0;
    }                          
}

/* FUNGSI INPUT BRACKET DAN GUESS */

void inputBracket(FunctionConfig *cfg) {
    char fmt[100];
    int dec = cfg->decimal;
    printf("Masukkan Batas Bawah (xl) dan Batas Atas (xu) [ Bisection & False-Position ]\n\n"); 
    printf("※  Program akan melakukan verifikasi terlebih dahulu untuk memastikan terdapat root\n");
    printf("   di interval yang diberikan!\n\n");
    printf("xl = "); scanf("%lf", &cfg->xl);
    sprintf(fmt, "f(xl) = %%.%df", dec); printf(fmt, evaluateFunction(cfg, cfg->xl)); printf("\n\n");
    printf("xu = "); scanf("%lf", &cfg->xu);
    sprintf(fmt, "f(xu) = %%.%df", dec); printf(fmt, evaluateFunction(cfg, cfg->xu)); printf("\n\n");
    while (evaluateFunction(cfg, cfg->xl) * evaluateFunction(cfg, cfg->xu) > 0) {
        sprintf(fmt, "f(%%.%df) * f(%%.%df) > 0 tidak memenuhi kondisi!\n", dec, dec); printf(fmt, cfg->xl, cfg->xu); 
        sprintf(fmt, "Bracket: [%%.%df, %%.%df] tidak dapat digunakan!\n", dec, dec); printf(fmt, cfg->xl, cfg->xu); 
        printf("Tolong masukkan kembali xl dan xu.\n\n");
        printf("xl = "); scanf("%lf", &cfg->xl);
        sprintf(fmt, "f(xl) = %%.%df", dec); printf(fmt, evaluateFunction(cfg, cfg->xl)); printf("\n\n");
        printf("xu = "); scanf("%lf", &cfg->xu);
        sprintf(fmt, "f(xu) = %%.%df", dec); printf(fmt, evaluateFunction(cfg, cfg->xu)); printf("\n\n");
    }
    sprintf(fmt, "f(%%.%df) * f(%%.%df) < 0 memenuhi kondisi!\n", dec, dec); printf(fmt, cfg->xl, cfg->xu);
    sprintf(fmt, "Bracket: [%%.%df, %%.%df] dapat digunakan!\n\n", dec, dec); printf(fmt, cfg->xl, cfg->xu); 
}

void inputGuess(FunctionConfig *cfg) {
    if (cfg->methodSelected[NEWTON_RAPHSON - 1]) {
        printf("Masukkan Initial Guess (x0) [ Newton-Raphson ]\n\n");
        printf("x0 = "); scanf("%lf", &cfg->x0); printf("\n"); 
    }
    if (cfg->methodSelected[SECANT - 1]) {
        printf("Masukkan Initial Guesses (x0) dan (x1) [ Secant | x0 dan x1 Tidak Boleh Sama ]\n\n");
        printf("x0 = "); scanf("%lf", &cfg->x0);  
        printf("x1 = "); scanf("%lf", &cfg->x1); printf("\n"); 
        while (cfg->x0 == cfg->x1) {
            printf("Input tidak valid! x0 dan x1 tidak boleh sama.\n\n");
            printf("x0 = "); scanf("%lf", &cfg->x0);
            printf("x1 = "); scanf("%lf", &cfg->x1); printf("\n");
        }
    }
}

/* FUNGSI KOMPUTASI NUMERIK */

double methodBisection(FunctionConfig *cfg, double xl, double xu, StopCriteria sc) {
    double xr, xrold, ea = 100.0;
    int iter = 0;
    xr = (xl + xu) / 2;
    do {
        xrold = xr;
        if (evaluateFunction(cfg, xl) * evaluateFunction(cfg, xr) < 0) {
            xu = xr;
            xr = (xl + xu) / 2;
        }
        else if (evaluateFunction(cfg, xl) * evaluateFunction(cfg, xr) > 0) {
            xl = xr;
            xr = (xl + xu) / 2;
        }
        iter++;
        if (evaluateFunction(cfg, xl) * evaluateFunction(cfg, xr) != 0) {
            ea = fabs((xr - xrold) / xr) * 100;
        } 
    } while (iter < sc.maxIter && ea > sc.stopError);        
    return xr;
}

double methodFalsePosition (FunctionConfig *cfg, double xl, double xu, StopCriteria sc) {
    double xr, xrold, ea = 100.0;
    int iter = 0;
    if (evaluateFunction(cfg, xl) * evaluateFunction(cfg, xu) < 0) {
        xr = xu - (evaluateFunction(cfg, xu) * (xl - xu) / (evaluateFunction(cfg, xl) - evaluateFunction(cfg, xu)));
        do {
            xrold = xr;
            if (evaluateFunction(cfg, xl) * evaluateFunction(cfg, xr) < 0) {
                xu = xr;
                xr = xu - (evaluateFunction(cfg, xu) * (xl - xu) / (evaluateFunction(cfg, xl) - evaluateFunction(cfg, xu)));
            }
            else if (evaluateFunction(cfg, xl) * evaluateFunction(cfg, xr) > 0) {
                xl = xr;
                xr = xu - (evaluateFunction(cfg, xu) * (xl - xu) / (evaluateFunction(cfg, xl) - evaluateFunction(cfg, xu)));
            }
            iter++;
            if (evaluateFunction(cfg, xl) * evaluateFunction(cfg, xr) != 0) {
                ea = fabs((xr - xrold) / xr) * 100;
            } 
        } while (iter < sc.maxIter && ea > sc.stopError);        
        return xr;
    } else {
        printf("BRACKET TIDAK BISA DIGUNAKAN");
        return 0.0;
    } 
}

double methodNewtonRaphson(FunctionConfig *cfg, double x, StopCriteria sc) {
    double xr, xrold, ea = 100.0;
    int iter = 0;
    do {
        xrold = x;
        xr = x - (evaluateFunction(cfg, x) / evaluateDerivative(cfg, x));
        x = xr;
        iter++;
        if (xr != 0) {
            ea = fabs((xr - xrold) / xr) * 100;
        }
    } while (iter < sc.maxIter && ea > sc.stopError);        
    return xr;
}

double methodSecant(FunctionConfig *cfg, double x0, double x1, StopCriteria sc) {
    double xr, xrold, ea = 100.0;
    int iter = 0;
    do {
        xrold = x1;
        xr = x1 - (evaluateFunction(cfg, x1) * (x1 - x0) / (evaluateFunction(cfg, x0) - evaluateFunction(cfg, x1)));
        x0 = x1;
        x1 = xr;
        iter++;
        if (xr != 0) {
            ea = fabs((xr - xrold) / xr) * 100;
        }
    } while (iter < sc.maxIter && ea > sc.stopError);        
    return xr;
}

/* PROGRAM UTAMA */

int main() {
    FunctionConfig cfg;
    StopCriteria sc;

    printf("===============================================================================================\n\n");
    printf("                       PROGRAM KALKULATOR ROOT FINDING - POLINOMIAL & EULER             \n");
    printf("                               Final Project Praktikum Pemrograman C                    \n\n");
    printf("Oleh: Kelompok 5\n");
    printf("- Raden Ayu Athifah Qurrota'aini (2406408230)\n");
    printf("- Annabell Della Sumantri        (2406415040)\n");
    printf("- Keira Khairani Haqi            (2406419562)\n");
    printf("- Dwidra Audric Farras           (2406426265)\n\n");
    printf("===============================================================================================\n\n");

    printf("Selamat datang di Kalkulator Root Finding!\n\n");
    printf("===============================================================================================\n\n");

    do {
        cfg = (FunctionConfig){0};
        sc = (StopCriteria){BOTH, MAX_ITER, DEFAULT_ES};

        inputFunctionType(&cfg);
        printf("===============================================================================================\n\n");

        if (cfg.funcType == POLYNOMIAL) {
            inputPolynomialDegree(&cfg);
            printf("===============================================================================================\n\n");
        }

        inputDecimalNumbers(&cfg);
        printf("===============================================================================================\n\n");

        inputFunctionParameter(&cfg);
        printf("===============================================================================================\n\n");

        inputRootMethods(&cfg);
        printf("===============================================================================================\n\n");
        
        inputStopMode(&cfg, &sc);
        printf("===============================================================================================\n\n");

        if (cfg.methodSelected[BISECTION - 1] || cfg.methodSelected[FALSE_POSITION - 1]) {
            inputBracket(&cfg);
            printf("===============================================================================================\n\n");
        }

        if (cfg.methodSelected[NEWTON_RAPHSON - 1] || cfg.methodSelected[SECANT - 1]) {
            inputGuess(&cfg);
            printf("===============================================================================================\n\n");
        }

        /*
        panggil fungsi printMethods (tabel)
        */

        inputExitChoice(&cfg);
        printf("===============================================================================================\n\n");
    } while (cfg.exitChoice == CALCULATE);

    return 0;
}
//```
