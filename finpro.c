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

/* KUMPULAN FUNGSI-FUNGSI */

// fungsi untuk mengosongkan sisa karakter di buffer stdin setelah scanf("%lf")

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// fungsi untuk membaca input integer : menolak input desimal, huruf, dan karakter tidak valid

int scanInt(int *out) {
    char line[64];
    if (fgets(line, sizeof(line), stdin) == NULL) return 0;
    char *end;
    long val = strtol(line, &end, 10);
    if (end == line || (*end != '\n' && *end != '\0')) return 0;
    *out = (int)val;
    return 1;
}

// fungsi untuk membaca input double : menolak input huruf dan karakter tidak valid

int scanDouble(double *out) {
    char line[64];
    if (fgets(line, sizeof(line), stdin) == NULL) return 0;
    char *end;
    *out = strtod(line, &end);
    if (end == line || (*end != '\n' && *end != '\0')) return 0;
    return 1;
}

// fungsi untuk menerima input pilihan jenis fungsi (polinomial/euler)

void inputFunctionType(FunctionConfig *cfg) {
    int funcType;
    char *funcNames[] = {"Polinomial", "Euler"};
    printf("Pilihan Jenis Fungsi [ Masukkan Angka 1/2 ]\n\n");
    printf("[1] Fungsi Polinomial  :  ax^n + bx^(n-1) + ... + d\n");
    printf("[2] Fungsi Euler       :  e^(ax+b) + cx + d\n\n");
    do {
        printf("Pilihan Anda           :  ");
        while (!scanInt(&funcType)) {
            printf("\n");
            printf("Pilihan Anda tidak valid! Tolong masukkan angka 1/2.\n\n");
            printf("Pilihan Anda           :  ");
        }
        printf("\n");
        if (funcType != 1 && funcType != 2) {
            printf("Pilihan Anda tidak valid! Tolong masukkan angka 1/2.\n\n");
        }
    } while (funcType != 1 && funcType != 2);
    printf("Anda memilih Fungsi %s!\n\n", funcNames[funcType - 1]); 
    cfg->funcType = (FunctionType)funcType;
}

// fungsi untuk menerima input pilihan derajat polinomial - hanya digunakan bila user memilih fungsi polinomial (linier/kuadratik/kubik)

void inputPolynomialDegree(FunctionConfig *cfg) {
    int degree;
    char *degreeNames[] = {"Linier", "Kuadrat", "Kubik"};
    printf("Pilihan Derajat Polinomial [ Masukkan Angka 1/2/3 ]\n\n");
    printf("[1] Linier     :  ax + b\n");
    printf("[2] Kuadratik  :  ax^2 + bx + c\n");
    printf("[3] Kubik      :  ax^3 + bx^2 + cx + d\n\n");
    do {
        printf("Pilihan Anda   :  "); 
        while (!scanInt(&degree)) {
            printf("\n");
            printf("Pilihan Anda tidak valid! Tolong masukkan angka 1/2/3.\n\n");
            printf("Pilihan Anda   :  ");
        }
        printf("\n");
        if (degree != 1 && degree != 2 && degree != 3) {
            printf("Pilihan Anda tidak valid! Tolong masukkan angka 1/2/3.\n\n");
        }
    } while (degree != 1 && degree != 2 && degree != 3);
    printf("Anda memilih %s!\n\n", degreeNames[degree - 1]);
    cfg->degree = (PolynomialDegree)degree;
}

// fungsi untuk menerima input pilihan pengaturan tampilan desimal (4/6/8)

void inputDecimalNumbers(FunctionConfig *cfg) {
    int decimal;
    printf("Pilihan Pengaturan Tampilan Desimal [ Masukkan Angka 4/6/8 ]\n\n");
    printf("[4] 4 Angka Desimal  :  X.XXXX\n");
    printf("[6] 6 Angka Desimal  :  X.XXXXXX\n");
    printf("[8] 8 Angka Desimal  :  X.XXXXXXXX\n\n");
    do {
        printf("Pilihan Anda         :  "); 
        while (!scanInt(&decimal)) {
            printf("\n");
            printf("Pilihan Anda tidak valid! Tolong masukkan angka 4/6/8.\n\n");
            printf("Pilihan Anda         :  ");
        }
        printf("\n");
        if (decimal != 4 && decimal != 6 && decimal != 8) {
            printf("Pilihan Anda tidak valid! Tolong masukkan angka 4/6/8.\n\n");
        }
    } while (decimal != 4 && decimal != 6 && decimal != 8);
    printf("Anda memilih Tampilan %d Angka Desimal!\n\n", decimal);
    cfg->decimal = (DecimalNumbers)decimal;
}

// fungsi print fungsi linier untuk ditampilkan di fungsi input parameter/koefisien

void printLinearFunction(char *n, double *c, int decimal) {
    char fmt[100];
    sprintf(fmt, "Fungsi %%s Anda  :  f(x) = %%.%dfx + %%.%df\n", decimal, decimal);
    printf(fmt, n, c[0], c[1]);
}

// fungsi print fungsi kuadratik untuk ditampilkan di fungsi input parameter/koefisien

void printQuadraticFunction(char *n, double *c, int decimal) {
    char fmt[100];
    sprintf(fmt, "Fungsi %%s Anda  :  f(x) = %%.%dfx^2 + %%.%dfx + %%.%df\n", decimal, decimal, decimal);
    printf(fmt, n, c[0], c[1], c[2]);
}

// fungsi print fungsi kubik untuk ditampilkan di fungsi input parameter/koefisien

void printCubicFunction(char *n, double *c, int decimal) {
    char fmt[100];
    sprintf(fmt, "Fungsi %%s Anda  :  f(x) = %%.%dfx^3 + %%.%dfx^2 + %%.%dfx + %%.%df\n", decimal, decimal, decimal, decimal);
    printf(fmt, n, c[0], c[1], c[2], c[3]);
}

// fungsi print fungsi euler untuk ditampilkan di fungsi input parameter/koefisien

void printEulerFunction(char *n, double *c, int decimal) {
    char fmt[100];
    sprintf(fmt, "Fungsi %%s Anda  :  f(x) = e^(%%.%dfx + %%.%df) + %%.%dfx + %%.%df\n", decimal, decimal, decimal, decimal);
    printf(fmt, n, c[0], c[1], c[2], c[3]);
}

// fungsi print true root dari fungsi linier yang diberikan user di fungsi input parameter/koefisien

void printTrueRootLinear(double *c, int decimal, FunctionConfig *cfg) {
    char fmt[100];
    double xt = -(c[1] / c[0]);
    sprintf(fmt, "True Root f(x)      :  xt = %%.%df\n\n", decimal); printf(fmt, xt);
    printf("※   xt digunakan untuk mencari True Relative Error (%%et) dari setiap Metode.\n\n");
    cfg->xt_lin = xt;
}

// fungsi print true root terbesar dari fungsi kuadratik yang diberikan user di fungsi input parameter/koefisien

void printTrueRootQuadratic(double *c, int decimal, FunctionConfig *cfg) {
    char fmt[100];
    double disc = pow(c[1], 2) - 4 * c[0] * c[2];
    double x1 = (-c[1] + sqrt(disc)) / (2 * c[0]);
    double x2 = (-c[1] - sqrt(disc)) / (2 * c[0]);
    sprintf(fmt, "True Roots f(x)        :  xt_1 = %%.%df | xt_2 = %%.%df\n\n", decimal, decimal);
    printf(fmt, x1, x2);
    printf("※   xt digunakan untuk mencari True Relative Error (%%et) dari setiap Metode.\n");
    printf("    xt yang digunakan akan otomatis disesuaikan dengan akar terdekat dari xr.\n\n");
    cfg->xt_quad = (x1 >= x2) ? x1 : x2;
}

// fungsi print true root real terbesar dari fungsi kubik yang diberikan user di fungsi input parameter/koefisien

void printTrueRootCubic(double *c, int decimal, FunctionConfig *cfg) {
    char fmt[100];
    double xt;
    double p = (3 * c[0] * c[2] - pow(c[1], 2)) / (3 * pow(c[0], 2));
    double q = (2 * pow(c[1], 3) - 9 * c[0] * c[1] * c[2] + 27 * pow(c[0], 2) * c[3]) / (27 * pow(c[0], 3));
    double delta = pow(q / 2, 2) + pow(p / 3, 3);
    if (delta > 0) {
        double complex u = cpow(-q / 2 + csqrt(delta), 1.0 / 3.0);
        double complex v = cpow(-q / 2 - csqrt(delta), 1.0 / 3.0);
        xt = creal(u + v) - c[1] / (3 * c[0]);
        sprintf(fmt, "True Root f(x)     :  xt = %%.%df\n\n", decimal); printf(fmt, xt);
        printf("※   xt digunakan untuk mencari True Relative Error (%%et) dari setiap Metode.\n\n");
    } else {
        double r = 2 * sqrt(-p / 3.0);
        double theta = acos((3 * q / (2 * p)) * sqrt(-3.0 / p));
        double roots[3];
        double max_x = -INFINITY;
        for (int i = 0; i < 3; i++) {
            roots[i] = r * cos((theta + 2 * M_PI * i) / 3.0) - c[1] / (3 * c[0]);
            if (roots[i] > max_x) max_x = roots[i];
        }
        xt = max_x;
        int rounding = (int)pow(10, (decimal + 1));
        xt = round(xt * rounding) / rounding;
        sprintf(fmt, "True Roots f(x)    :  xt_1 = %%.%df | xt_2 = %%.%df | xt_3 = %%.%df\n\n",
            decimal, decimal, decimal);
        printf(fmt, roots[0], roots[1], roots[2]);
        printf("※   xt digunakan untuk mencari True Relative Error (%%et) dari setiap Metode.\n");
        printf("    xt yang digunakan akan otomatis disesuaikan dengan akar terdekat dari xr.\n\n");
    }
    cfg->xt_cube = xt;
}

// fungsi print pesan tidak memiliki true root yang dapat dihitung secara analitik untuk fungsi euler

void printTrueRootEuler(double *c, int decimal, FunctionConfig *cfg) {
    printf("※   Fungsi Euler tidak memiliki True Root yang dapat dihitung secara analitik untuk mencari True Relative Error (%%et).\n\n");
}
             
// fungsi untuk menerima input parameter/koefisien

void inputFunctionParameter(FunctionConfig *cfg) {
    FunctionMeta meta;
    int dec = cfg->decimal;
    char fmt[100];
    switch (cfg->funcType) {
        case POLYNOMIAL:
            switch (cfg->degree) {
                case LINEAR:
                    meta = (FunctionMeta){
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
                    meta = (FunctionMeta){
                        .funcName = "Kuadratik",
                        .funcFormat = "ax^2 + bx + c",
                        .funcCondition = "a Tidak Boleh 0 dan Diskriminan Tidak Boleh Negatif",
                        .varNames = (char*[]){"a", "b", "c"},
                        .numCoef = 3,
                        .coef = cfg->param.quadratic,
                        .printFunction = printQuadraticFunction,
                        .printTrueRoot = printTrueRootQuadratic
                    };
                    break;
                case CUBIC:
                    meta = (FunctionMeta){
                        .funcName = "Kubik",
                        .funcFormat = "ax^3 + bx^2 + cx + d",
                        .funcCondition = "a Tidak Boleh 0",
                        .varNames = (char*[]){"a", "b", "c", "d"},
                        .numCoef = 4,
                        .coef = cfg->param.cubic,
                        .printFunction = printCubicFunction,
                        .printTrueRoot = printTrueRootCubic
                    };
                    break;
                default: return;
            }
            break;
        case EULER:
            meta = (FunctionMeta){
                .funcName = "Euler",
                .funcFormat = "e^(ax+b) + cx + d",
                .funcCondition = "a Tidak Boleh 0",
                .varNames = (char*[]){"a", "b", "c", "d"},
                .numCoef = 4,
                .coef = cfg->param.euler,
                .printFunction = printEulerFunction,
                .printTrueRoot = printTrueRootEuler
            };
            break;
        default: return;
    }
    printf("Masukkan Koefisien Fungsi %s [ %s | %s ]\n\n", meta.funcName, meta.funcFormat, meta.funcCondition);
    for (int i = 0; i < meta.numCoef; i++) {
        do {
            printf("%s = ", meta.varNames[i]);
            while (scanf("%lf", &meta.coef[i]) != 1) {
                clearInputBuffer();
                printf("\n");
                printf("Input tidak valid! Tolong masukkan bilangan.\n\n");
                printf("%s = ", meta.varNames[i]);
            }
            clearInputBuffer();
            if (strcmp(meta.funcName, "Linier") == 0 || i == 0) {
                if (meta.coef[i] == 0.00) {
                    printf("\nInput tidak valid! %s tidak boleh 0.\n\n", meta.varNames[i]);
                }
            }
        } while ((strcmp(meta.funcName, "Linier") == 0 || i == 0) && meta.coef[i] == 0.00);
    }
    if (strcmp(meta.funcName, "Kuadratik") == 0) {
        double disc = meta.coef[1] * meta.coef[1] - 4 * meta.coef[0] * meta.coef[2];
        while (disc < 0) {
            sprintf(fmt, "\nInput tidak valid! Diskriminan negatif (b^2 - 4ac = %%.%df).\n", dec); printf(fmt, disc);
            printf("Fungsi tidak memiliki akar real. Tolong input ulang koefisien.\n\n");
            for (int i = 0; i < meta.numCoef; i++) {
                do {
                    printf("%s = ", meta.varNames[i]);
                    while (scanf("%lf", &meta.coef[i]) != 1) {
                        clearInputBuffer();
                        printf("\n");
                        printf("Input tidak valid! Tolong masukkan bilangan.\n\n");
                        printf("%s = ", meta.varNames[i]);
                    }
                    clearInputBuffer();
                    if (i == 0 && meta.coef[i] == 0.00) {
                        printf("\nInput tidak valid! %s tidak boleh 0.\n\n", meta.varNames[i]);
                    }
                } while (i == 0 && meta.coef[i] == 0.00);
            }
            disc = meta.coef[1] * meta.coef[1] - 4 * meta.coef[0] * meta.coef[2];
        }
    }
    printf("\n");
    meta.printFunction(meta.funcName, meta.coef, dec);
    meta.printTrueRoot(meta.coef, dec, cfg);
}

// fungsi untuk menerima input pilihan metode komputasi numerik dengan toggle logic

void inputRootMethods(FunctionConfig *cfg) {
    char *methodNames[] = {"Bisection", "False-Position", "Newton-Raphson", "Secant"};
    int infinitePositive = (cfg->funcType == EULER && cfg->param.euler[2] == 0.00 && cfg->param.euler[3] >= 0.00);
    int pick = 1;
    int dec = cfg->decimal;
    char fmt[100];
    for (int i = 0; i < 4; i++) {
        cfg->methodSelected[i] = 0;
    }
    printf("Pilihan Metode Komputasi Numerik [ Masukkan Angka 0/1/2/3/4 ]\n\n");
    printf("Anda boleh memilih lebih dari satu Metode!\n");
    printf("※   Anda dapat membatalkan pilihan Metode dengan memasukkan angka yang sama. Semua pilihan Anda akan secara otomatis\n");
    printf("    tersimpan setelah pemilihan ke-9 kali.\n\n");
    if (infinitePositive) {
        sprintf(fmt, "Peringatan: c = %%.%df dan d = %%.%df menyebabkan f(x) = e^(%%.%dfx + %%.%df) selalu positif!\n", dec, dec, dec, dec);
        printf(fmt, cfg->param.euler[2], cfg->param.euler[3], cfg->param.euler[0], cfg->param.euler[1]);
        printf("※   Metode Bisection dan False-Position tidak dapat digunakan.\n\n");
    }
    printf("[1] Bisection        :  Membutuhkan xl (lower guess) dan xu (upper guess)%s\n", infinitePositive ? " [TIDAK TERSEDIA]" : "");
    printf("[2] False-Position   :  Membutuhkan xl (lower guess) dan xu (upper guess)%s\n", infinitePositive ? " [TIDAK TERSEDIA]" : "");
    printf("[3] Newton-Raphson   :  Membutuhkan xi (initial guess)\n");
    printf("[4] Secant           :  Membutuhkan xi-1 (initial guess 0) dan xi (initial guess 1)\n");
    printf("[0] Selesai\n\n");
    int method;
    do {
        printf("Pilihan ke-%d Anda    :  ", pick);
        while (!scanInt(&method)) {
            printf("\n");
            printf("Pilihan Anda tidak valid! Tolong masukkan angka 0/1/2/3/4.\n\n");
            printf("Pilihan ke-%d Anda    :  ", pick);
        }
        printf("\n");
        if (method >= 1 && method <= 4) {
            if (infinitePositive && (method == 1 || method == 2)) {
                printf("Metode %s tidak dapat digunakan! Tolong pilih Metode lain.\n\n", methodNames[method - 1]);
            } else {
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
                        if (cfg->methodSelected[j]) printf("%d. %s\n", n++, methodNames[j]);
                    }
                    printf("\n");
                    method = 0;
                }
            }
        } else if (method == 0) {
            int m = 0;
            for (int j = 0; j < 4; j++) {
                if (cfg->methodSelected[j]) { m = 1; break; }
            }
            if (m == 0) {
                printf("Anda belum membuat pilihan! Tolong masukkan angka 1/2/3/4.\n\n"); method = -1;
            } else {
                printf("Anda memilih Metode  :\n");
                int n = 1;
                for (int j = 0; j < 4; j++) {
                    if (cfg->methodSelected[j]) printf("%d. %s\n", n++, methodNames[j]);
                }
                printf("\n");
            }
        } else {
            printf("Pilihan tidak valid! Tolong masukkan angka 0/1/2/3/4.\n\n");
        }
    } while (method != 0);
}

// fungsi untuk menerima input pilihan mode/kriteria berhenti iterasi

void inputStopMode(FunctionConfig *cfg, StopCriteria *sc) {
    char *criteriaNames[] = {"Input Max Iteration", "Input % Stopping Error", "Input Keduanya"};
    int stop;
    char fmt[100];
    int dec = cfg->decimal;
    printf("Pilihan Mode Berhenti Iterasi [ Masukkan Angka 1/2/3 ]\n\n");
    printf("[1] Input Max Iteration     :  Pencarian akar berhenti saat iter = max_iter\n");
    printf("[2] Input %% Stopping Error  :  Pencarian akar berhenti saat %%ea < %%es\n");
    printf("[3] Input Keduanya          :  Pencarian akar berhenti saat salah satunya terpenuhi\n\n");
    do {
        printf("Pilihan Anda                :  ");
        while (!scanInt(&stop)) {
            printf("\n");
            printf("Pilihan Anda tidak valid! Tolong masukkan angka 1/2/3.\n\n");
            printf("Pilihan Anda                :  ");
        }
        printf("\n");
        if (stop != 1 && stop != 2 && stop != 3) {
            printf("Pilihan Anda tidak valid! Tolong masukkan angka 1/2/3.\n\n");
        }
    } while (stop != 1 && stop != 2 && stop != 3);
    printf("Anda memilih %s!\n\n", criteriaNames[stop - 1]);
    if (stop == ITER || stop == BOTH) {
        do {
            printf("max_iter [ 1 - %d ] = ", MAX_ITER);
            while (!scanInt(&sc->maxIter)) {
                printf("\n");
                printf("Input tidak valid! max_iter harus di antara 1 - %d.\n\n", MAX_ITER);
                printf("max_iter [ 1 - %d ] = ", MAX_ITER);
            }
            printf("\n");
            if (sc->maxIter < 1 || sc->maxIter > MAX_ITER) {
                printf("Input tidak valid! max_iter harus di antara 1 - %d.\n\n", MAX_ITER);
            }
        } while (sc->maxIter < 1 || sc->maxIter > MAX_ITER);
        if (stop == ITER) {
            printf("※   Pencarian akar berhenti saat iterasi mencapai ke-%d kali.\n\n", sc->maxIter);
        }
    }
    if (stop == EA || stop == BOTH) {
        do {
            printf("%%es [ %g%% - 100.00%% ] = ", DEFAULT_ES * 100);
            while (!scanDouble(&sc->stopError)) {
                printf("\n");
                printf("Input tidak valid! %%es harus di antara %g%% (%g) - 100.00%%.\n\n", DEFAULT_ES * 100, DEFAULT_ES);
                printf("%%es [ %g%% - 100.00%% ] = ", DEFAULT_ES * 100);
            }
            printf("\n");
            if (sc->stopError < DEFAULT_ES * 100 || sc->stopError > 100.0) {
                printf("Input tidak valid! %%es harus di antara %g%% (%g) - 100.00%%.\n\n", DEFAULT_ES * 100, DEFAULT_ES);
            }
        } while (sc->stopError < DEFAULT_ES * 100 || sc->stopError > 100.0);
        if (stop == EA) {
            sprintf(fmt, "※   Pencarian akar berhenti saat approximate error (ea) jatuh di bawah %%g%%%% (%%.%df).\n\n", dec);
            printf(fmt, sc->stopError, sc->stopError / 100);
        }
    }
    if (stop == BOTH) {
        printf("※   Pencarian akar berhenti saat iterasi mencapai ke-%d kali atau saat approximate error (ea) jatuh di bawah\n", sc->maxIter);
        sprintf(fmt, "    %%g%%%% (%%.%df).\n\n", dec);
        printf(fmt, sc->stopError, sc->stopError / 100);
    }
    sc->mode = (StopMode)stop;
}

// fungsi untuk menerima input pilihan lanjut/keluar

void inputExitChoice(FunctionConfig *cfg) {
    int exitChoice;
    printf("Apakah Anda ingin melanjutkan? [ Masukkan Angka 1/0 ]\n\n"); 
    printf("[1] Lanjut\n");
    printf("[0] Keluar Program\n\n");
    do {
        printf("Pilihan Anda  :  "); 
        while (!scanInt(&exitChoice)) {
            printf("\n");
            printf("Pilihan Anda tidak valid! Tolong masukkan angka 1/0.\n\n");
            printf("Pilihan Anda  :  ");
        }
        printf("\n");
        if (exitChoice != 0 && exitChoice != 1) {
            printf("Pilihan Anda tidak valid! Tolong masukkan angka 1/0.\n\n");
        }
    } while (exitChoice != 0 && exitChoice != 1);
    cfg->exitChoice = (ExitChoice)exitChoice;
    if (exitChoice == CALCULATE) {
        printf("Hore! Anda memilih Lanjut!\n\n");
    } else {
        printf("Anda memilih Keluar Program!\n\n"); 
        printf("Terima kasih sudah menggunakan Kalkulator Root Finding kami!\n\n");
    }    
}

// fungsi untuk mengevaluasi fungsi yang diberikan user, digunakan untuk validasi input bracket bisection & false-position,
// dan digunakan untuk perhitungan di keempat metode

double evaluateFunction(FunctionConfig *cfg, double x) {
    double result = 0.0;
    switch (cfg->funcType) {
        case POLYNOMIAL:
            switch (cfg->degree) {
                case LINEAR:
                    result = cfg->param.linear[0] * x + cfg->param.linear[1];
                    break;
                case QUADRATIC:
                    result = cfg->param.quadratic[0] * pow(x,2) + cfg->param.quadratic[1] * x + cfg->param.quadratic[2];
                    break;
                case CUBIC:
                    result = cfg->param.cubic[0] * pow(x,3) + cfg->param.cubic[1] * pow(x,2) + cfg->param.cubic[2] * x + cfg->param.cubic[3];
                    break;
                default:
                   result = 0.0;
            }
            break;
        case EULER: {
            double exponent = cfg->param.euler[0] * x + cfg->param.euler[1];
            result = exp(exponent) + cfg->param.euler[2] * x + cfg->param.euler[3];
            break;
        }
        default:
            return 0.0;
    }
    if (!isfinite(result)) {
        return (result > 0) ? DBL_MAX : -DBL_MAX;
    }
    return result;
}

// fungsi untuk menurunkan fungsi yang diberikan user, digunakan untuk perhitungan newton-raphson

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

// fungsi untuk menerima input lower guess dan upper guess untuk bisection & false-position

void inputBracket(FunctionConfig *cfg) {
    char fmt[100];
    int dec = cfg->decimal;
    printf("Masukkan Lower Guess (xl) dan Upper Guess (xu) [ Bisection & False-Position ]\n\n"); 
    printf("※   Program akan melakukan verifikasi terlebih dahulu untuk memastikan terdapat root di interval yang diberikan!\n\n");
    do {
        printf("xl = ");
        while (scanf("%lf", &cfg->xl) != 1) {
            clearInputBuffer();
            printf("\n");
            printf("Input tidak valid! Tolong masukkan bilangan.\n\n");
            printf("xl = ");
        }
        clearInputBuffer();
        sprintf(fmt, "f(xl) = %%.%df\n\n", dec); printf(fmt, evaluateFunction(cfg, cfg->xl));
        printf("xu = ");
        while (scanf("%lf", &cfg->xu) != 1) {
            clearInputBuffer();
            printf("Input tidak valid! Tolong masukkan bilangan.\n\n");
            printf("xu = ");
        }
        clearInputBuffer();
        sprintf(fmt, "f(xu) = %%.%df\n\n", dec); printf(fmt, evaluateFunction(cfg, cfg->xu));
        if (evaluateFunction(cfg, cfg->xl) * evaluateFunction(cfg, cfg->xu) > 0) {
            sprintf(fmt, "f(%%.%df) * f(%%.%df) > 0 tidak memenuhi kondisi!\n", dec, dec); printf(fmt, cfg->xl, cfg->xu);
            sprintf(fmt, "Bracket: [%%.%df, %%.%df] tidak dapat digunakan!\n", dec, dec); printf(fmt, cfg->xl, cfg->xu);
            printf("Tolong masukkan kembali xl dan xu.\n\n");
        }
    } while (evaluateFunction(cfg, cfg->xl) * evaluateFunction(cfg, cfg->xu) > 0);
    sprintf(fmt, "f(%%.%df) * f(%%.%df) < 0 memenuhi kondisi!\n", dec, dec); printf(fmt, cfg->xl, cfg->xu);
    sprintf(fmt, "Bracket: [%%.%df, %%.%df] dapat digunakan!\n\n", dec, dec); printf(fmt, cfg->xl, cfg->xu);
}

// fungsi untuk menerima input initial guess newton-raphson, berbeda dengan secant

void inputGuessNewtonRaphson(FunctionConfig *cfg) {
    int dec = cfg->decimal;
    char fmt[100];
    printf("Masukkan Initial Guess (xi) [ Newton-Raphson ]\n\n");
    do {
        printf("xi = ");
        while (scanf("%lf", &cfg->xi_nr) != 1) {
            clearInputBuffer();
            printf("\n");
            printf("Input tidak valid! Tolong masukkan bilangan.\n\n");
            printf("xi = ");
        }
        clearInputBuffer();
        double dfx = evaluateDerivative(cfg, cfg->xi_nr);
        if (fabs(dfx) < 1e-10) {
            sprintf(fmt, "\nInput tidak valid! f'(%%.%df) = 0. Turunan tidak boleh nol.\n", dec);
            printf(fmt, cfg->xi_nr);
            printf("Tolong pilih initial guess yang lain.\n\n");
        } else {
            break;
        }
    } while (1);
    printf("\n");
}

// fungsi untuk menerima input initial guesses secant, berbeda dengan newton-raphson

void inputGuessSecant(FunctionConfig *cfg) {
    printf("Masukkan Initial Guesses (xi-1) dan (xi) [ Secant | xi-1 dan xi Tidak Boleh Sama ]\n\n");
    do {
        printf("xi-1 = ");
        while (scanf("%lf", &cfg->xi_1) != 1) {
            clearInputBuffer();
            printf("\n");
            printf("Input tidak valid! Tolong masukkan bilangan.\n\n");
            printf("xi-1 = ");
        }
        clearInputBuffer();
        printf("xi = ");
        while (scanf("%lf", &cfg->xi_sec) != 1) {
            clearInputBuffer();
            printf("\n");
            printf("Input tidak valid! Tolong masukkan bilangan.\n\n");
            printf("xi = ");
        }
        clearInputBuffer();
        printf("\n");
        if (cfg->xi_1 == cfg->xi_sec) {
            printf("Input tidak valid! xi-1 dan xi tidak boleh sama.\n\n");
        }
    } while (cfg->xi_1 == cfg->xi_sec);
}

// fungsi untuk menentukan logika keluar loop iterasi do-while berdasarkan mode berhenti
// menggunakan constraint paksa berhenti iterasi apabila nilai xr melebihi 10000 atau iterasi melebihi 100 (batas iterasi maksimum default)
// atau ea lebih kecil dari 0.0001 (batas ea minimum default)

int shouldContinue(StopCriteria sc, int iter, double ea, double xr) {
    if (!isfinite(xr) || !isfinite(ea) || fabs(xr) > 10000.0) { 
        return 0; 
    }
    if (iter > 1 && fabs(ea) < DEFAULT_ES) {
        return 0;
    }
    switch (sc.mode) {
        case ITER: 
            return iter < sc.maxIter;
        case EA:   
            return iter < MAX_ITER && ea > sc.stopError;
        case BOTH: 
            return iter < sc.maxIter && ea > sc.stopError;
        default:   
            return 0;
    }
}

// fungsi untuk menentukan apabila perlu menambahkan alokasi memori berdasarkan counter dalam loop iterasi do-while

int shouldRealloc(MethodResult *res) {
    return res->count >= res->capacity;
}

// fungsi untuk mencari semua akar real polinomial dan mengembalikan yang paling dekat dengan xr

double findClosestRoot(FunctionConfig *cfg, double xr) {
    double roots[3];
    int nRoots = 0;
    switch (cfg->degree) {
        case LINEAR: {
            roots[0] = -(cfg->param.linear[1] / cfg->param.linear[0]);
            nRoots = 1;
            break;
        }
        case QUADRATIC: {
            double a = cfg->param.quadratic[0];
            double b = cfg->param.quadratic[1];
            double c = cfg->param.quadratic[2];
            double disc = b*b - 4*a*c;
            if (disc < 0) {
                return cfg->xt_quad;
            }
            roots[0] = (-b + sqrt(disc)) / (2*a);
            roots[1] = (-b - sqrt(disc)) / (2*a);
            nRoots = 2;
            break;
        }
        case CUBIC: {
            double a = cfg->param.cubic[0];
            double b = cfg->param.cubic[1];
            double c = cfg->param.cubic[2];
            double d = cfg->param.cubic[3];
            double p = (3*a*c - b*b) / (3*a*a);
            double q = (2*b*b*b - 9*a*b*c + 27*a*a*d) / (27*a*a*a);
            double delta = pow(q/2, 2) + pow(p/3, 3);
            if (delta > 0) {
                double complex u = cpow(-q/2 + csqrt(delta), 1.0/3.0);
                double complex v = cpow(-q/2 - csqrt(delta), 1.0/3.0);
                roots[0] = creal(u + v) - b/(3*a);
                nRoots = 1;
            } else {
                double r     = 2 * sqrt(-p/3.0);
                double theta = acos((3*q / (2*p)) * sqrt(-3.0/p));
                for (int i = 0; i < 3; i++) {
                    roots[i] = r * cos((theta + 2*M_PI*i) / 3.0) - b/(3*a);
                }
                nRoots = 3;
            }
            break;
        }
        default:
            return 0;
    }
    double closest = roots[0];
    for (int i = 1; i < nRoots; i++) {
        if (fabs(roots[i] - xr) < fabs(closest - xr))
            closest = roots[i];
    }
    return closest;
}

// fungsi untuk menghitung true error khusus untuk fungsi polinomial

double calcTrueError(FunctionConfig *cfg, double xr) {
    double xt = findClosestRoot(cfg, xr);
    return fabs(xt - xr);
}

// fungsi untuk menghitung true error relative khusus untuk fungsi polinomial

double calcTrueErrorRelative(FunctionConfig *cfg, double xr) {
    double xt = findClosestRoot(cfg, xr);
    if (fabs(xt) < 1e-12) return 0;   // root at zero — et undefined, avoid div/0
    return fabs(xt - xr) / fabs(xt) * 100;
}

// fungsi komputasi numerik metode bisection

MethodResult methodBisection(FunctionConfig *cfg, StopCriteria sc) {
    MethodResult res;
    res.count = 0;
    // alokasi kapasitas awal: dinamis untuk mode EA, tetap untuk mode ITER/BOTH
    if (sc.mode == EA) {
        res.capacity = 10;
    } else {
        res.capacity = sc.maxIter;
    }
    res.rows = malloc(res.capacity * sizeof(IterationResult));
    double xl = cfg->xl, xu = cfg->xu;
    double xr = 0, xrold, ea = 100.0, et = 0.0, eaold = -1.0, xl_save, xu_save;
    int iter = 0, eaSameCount = 0;
    char eaStr[50] = "init", eaOldStr[50] = "none";
    do {
        xrold = xr;
        // hitung titik tengah sebagai estimasi akar baru
        xr = (xl + xu) / 2;
        xl_save = xl;
        xu_save = xu;
        // perbaharui bracket berdasarkan tanda f(xl)*f(xr)
        if (evaluateFunction(cfg, xl) * evaluateFunction(cfg, xr) < 0) {
            xu = xr;
        }
        else if (evaluateFunction(cfg, xl) * evaluateFunction(cfg, xr) > 0) {
            xl = xr;
        }
        // hitung approximate error (hindari pembagian nol jika xr mendekati nol)
        if (fabs(xr) > 1e-12) {
            ea = fabs((xr - xrold) / xr) * 100;
        }
        // hitung true error hanya untuk fungsi polinomial karena euler tidak memiliki true root analitik
        if (cfg->funcType == POLYNOMIAL) {
            et = calcTrueErrorRelative(cfg, xr);
        }
        // deteksi stagnasi: jika ea tidak berubah di tampilan desimal selama 3 iterasi berturut-turut
        // dan ea masih nonzero, hentikan paksa karena iterasi tidak lagi menghasilkan perubahan bermakna
        snprintf(eaStr, 50, "%.*f", cfg->decimal, ea);
        if (strcmp(eaStr, eaOldStr) == 0 && iter > 2) {
            eaSameCount++;
            if (eaSameCount > 2 && fabs(ea) > DEFAULT_ES) { break; }
        } else {
            eaSameCount = 0;
        }
        eaold = ea;
        // realloc jika kapasitas array penuh
        if (shouldRealloc(&res)) {
            res.capacity *= 2;
            res.rows = realloc(res.rows, res.capacity * sizeof(IterationResult));
        }
        // simpan hasil iterasi ke array
        iter++;
        res.rows[res.count].iter = iter;
        res.rows[res.count].xl = xl_save;
        res.rows[res.count].xu = xu_save;
        res.rows[res.count].xr = xr;
        res.rows[res.count].ea = ea;
        res.rows[res.count].et = et;
        res.count++;
        strcpy(eaOldStr, eaStr);
    } while (shouldContinue(sc, iter, ea, xr));        
    res.root = xr;
    res.finalEa = ea;
    res.finalEt = et;   
    return res;
}

// fungsi komputasi numerik metode false-position

MethodResult methodFalsePosition(FunctionConfig *cfg, StopCriteria sc) {
    MethodResult res;
    res.count = 0;
    // alokasi kapasitas awal: dinamis untuk mode EA, tetap untuk mode ITER/BOTH
    if (sc.mode == EA) {
        res.capacity = 10;
    } else {
        res.capacity = sc.maxIter;
    }
    res.rows = malloc(res.capacity * sizeof(IterationResult));
    double xl = cfg->xl, xu = cfg->xu;
    double xr = 0, xrold, ea = 100.0, et = 0.0, eaold = -1.0, xl_save, xu_save;
    int iter = 0, eaSameCount = 0;
    char eaStr[50] = "init", eaOldStr[50] = "none";
    do {
        xrold = xr;
        // hentikan jika f(xl) dan f(xu) hampir sama untuk menghindari pembagian nol
        if (fabs(evaluateFunction(cfg, xl) - evaluateFunction(cfg, xu)) < 1e-12) { break; }
        // hitung estimasi akar baru dengan interpolasi linear antara (xl, f(xl)) dan (xu, f(xu))
        xr = xu - (evaluateFunction(cfg, xu) * (xl - xu) / (evaluateFunction(cfg, xl) - evaluateFunction(cfg, xu)));
        xl_save = xl;
        xu_save = xu;
        // perbaharui bracket berdasarkan tanda f(xl)*f(xr)
        if (evaluateFunction(cfg, xl) * evaluateFunction(cfg, xr) < 0) {
            xu = xr;
        }
        else if (evaluateFunction(cfg, xl) * evaluateFunction(cfg, xr) > 0) {
            xl = xr;
        }
        // hitung approximate error (hindari pembagian nol jika xr mendekati nol)
        if (fabs(xr) > 1e-12) {
            ea = fabs((xr - xrold) / xr) * 100;
        }
        // hitung true error hanya untuk fungsi polinomial karena euler tidak memiliki true root analitik
        if (cfg->funcType == POLYNOMIAL) {
            et = calcTrueErrorRelative(cfg, xr);
        }
        // deteksi stagnasi: jika ea tidak berubah di tampilan desimal selama 3 iterasi berturut-turut
        // dan ea masih nonzero, hentikan paksa karena iterasi tidak lagi menghasilkan perubahan bermakna
        snprintf(eaStr, 50, "%.*f", cfg->decimal, ea);
        if (strcmp(eaStr, eaOldStr) == 0 && iter > 2) {
            eaSameCount++;
            if (eaSameCount > 2 && fabs(ea) > DEFAULT_ES) { break; }
        } else {
            eaSameCount = 0;
        }
        eaold = ea;
        // realloc jika kapasitas array penuh
        if (shouldRealloc(&res)) {
            res.capacity *= 2;
            res.rows = realloc(res.rows, res.capacity * sizeof(IterationResult));
        }
        // simpan hasil iterasi ke array
        iter++;
        res.rows[res.count].iter = iter;
        res.rows[res.count].xl = xl_save;
        res.rows[res.count].xu = xu_save;
        res.rows[res.count].xr = xr;
        res.rows[res.count].ea = ea;
        res.rows[res.count].et = et;
        res.count++;
        strcpy(eaOldStr, eaStr);
    } while (shouldContinue(sc, iter, ea, xr));
    res.root = xr;
    res.finalEa = ea;
    res.finalEt = et;   
    return res;
}

// fungsi komputasi numerik metode newton-raphson

MethodResult methodNewtonRaphson(FunctionConfig *cfg, StopCriteria sc) {
    MethodResult res;
    res.count = 0;
    // alokasi kapasitas awal: dinamis untuk mode EA, tetap untuk mode ITER/BOTH
    if (sc.mode == EA) {
        res.capacity = 10;
    } else {
        res.capacity = sc.maxIter;
    }
    res.rows = malloc(res.capacity * sizeof(IterationResult));
    double x = cfg->xi_nr;
    double xr = 0, xrold, ea = 100.0, et = 0.0, eaold = -1.0;
    int iter = 0, eaSameCount = 0;
    char eaStr[50] = "init", eaOldStr[50] = "none";
    do {
        xrold = x;
        double fx  = evaluateFunction(cfg, x);
        double dfx = evaluateDerivative(cfg, x);
        // hentikan jika turunan mendekati nol untuk menghindari pembagian nol
        if (fabs(dfx) < 1e-12) { break; }
        // hitung estimasi akar baru dengan rumus newton-raphson: xr = x - f(x)/f'(x)
        xr = x - (fx / dfx);
        x = xr;
        // hitung approximate error (hindari pembagian nol jika xr mendekati nol)
        if (fabs(xr) > 1e-12) {
            ea = fabs((xr - xrold) / xr) * 100;
        }
        // hitung true error hanya untuk fungsi polinomial karena euler tidak memiliki true root analitik
        if (cfg->funcType == POLYNOMIAL) {
            et = calcTrueErrorRelative(cfg, xr);
        }
        // deteksi stagnasi: jika ea tidak berubah di tampilan desimal selama 3 iterasi berturut-turut
        // dan ea masih nonzero, hentikan paksa karena iterasi tidak lagi menghasilkan perubahan bermakna
        snprintf(eaStr, 50, "%.*f", cfg->decimal, ea);
        if (strcmp(eaStr, eaOldStr) == 0 && iter > 2) {
            eaSameCount++;
            if (eaSameCount > 2 && fabs(ea) > DEFAULT_ES) { break; }
        } else {
            eaSameCount = 0;
        }
        eaold = ea;
        // realloc jika kapasitas array penuh
        if (shouldRealloc(&res)) {
            res.capacity *= 2;
            res.rows = realloc(res.rows, res.capacity * sizeof(IterationResult));
        }
        // simpan hasil iterasi ke array
        iter++;
        res.rows[res.count].iter = iter;
        res.rows[res.count].xi = xrold;
        res.rows[res.count].xr = xr;
        res.rows[res.count].ea = ea;
        res.rows[res.count].et = et;
        res.count++;
        strcpy(eaOldStr, eaStr);
    } while (shouldContinue(sc, iter, ea, xr));
    res.root = xr;
    res.finalEa = ea;
    res.finalEt = et;   
    return res;
}

// fungsi komputasi numerik metode secant

MethodResult methodSecant(FunctionConfig *cfg, StopCriteria sc) {
    MethodResult res;
    res.count = 0;
    // alokasi kapasitas awal: dinamis untuk mode EA, tetap untuk mode ITER/BOTH
    if (sc.mode == EA) {
        res.capacity = 10;
    } else {
        res.capacity = sc.maxIter;
    }
    res.rows = malloc(res.capacity * sizeof(IterationResult));
    double x0 = cfg->xi_1, x1 = cfg->xi_sec;
    double xr = 0, xrold, ea = 100.0, et = 0.0, eaold = -1.0, xi_1_save, xi_save;
    int iter = 0, eaSameCount = 0;
    char eaStr[50] = "init", eaOldStr[50] = "none";
    do {
        xrold = x1;
        // hentikan jika f(x0) dan f(x1) hampir sama untuk menghindari pembagian nol
        if (fabs(evaluateFunction(cfg, x0) - evaluateFunction(cfg, x1)) < 1e-12) { break; }
        // hitung estimasi akar baru dengan rumus secant: interpolasi linear antara dua titik terakhir
        xr = x1 - (evaluateFunction(cfg, x1) * (x0 - x1) / (evaluateFunction(cfg, x0) - evaluateFunction(cfg, x1)));
        xi_1_save = x0;
        xi_save = x1;
        // geser window: x0 menjadi x1 lama, x1 menjadi xr baru
        x0 = x1;
        x1 = xr;
        // hitung approximate error (hindari pembagian nol jika xr mendekati nol)
        if (fabs(xr) > 1e-12) {
            ea = fabs((xr - xrold) / xr) * 100;
        }
        // hitung true error hanya untuk fungsi polinomial karena euler tidak memiliki true root analitik
        if (cfg->funcType == POLYNOMIAL) {
            et = calcTrueErrorRelative(cfg, xr);
        }
        // deteksi stagnasi: jika ea tidak berubah di tampilan desimal selama 3 iterasi berturut-turut
        // dan ea masih nonzero, hentikan paksa karena iterasi tidak lagi menghasilkan perubahan bermakna
        snprintf(eaStr, 50, "%.*f", cfg->decimal, ea);
        if (strcmp(eaStr, eaOldStr) == 0 && iter > 2) {
            eaSameCount++;
            if (eaSameCount > 2 && fabs(ea) > DEFAULT_ES) { break; }
        } else {
            eaSameCount = 0;
        }
        eaold = ea;
        // realloc jika kapasitas array penuh
        if (shouldRealloc(&res)) {
            res.capacity *= 2;
            res.rows = realloc(res.rows, res.capacity * sizeof(IterationResult));
        }
        // simpan hasil iterasi ke array
        iter++;
        res.rows[res.count].iter = iter;
        res.rows[res.count].xi_1 = xi_1_save;
        res.rows[res.count].xi = xi_save;
        res.rows[res.count].xr = xr;
        res.rows[res.count].ea = ea;
        res.rows[res.count].et = et;
        res.count++;
        strcpy(eaOldStr, eaStr);
    } while (shouldContinue(sc, iter, ea, xr));
    res.root = xr;
    res.finalEa = ea;
    res.finalEt = et;   
    return res;
}

// fungsi untuk print string, bisa digunakan di semua jenis tabel

void printCenteredStr(const char *str, int colWidth) {
    int len = strlen(str);
    int totalPad = colWidth - len;
    int padLeft = totalPad / 2;
    int padRight = totalPad - padLeft;
    printf("%*s%s%*s\U00002502", padLeft, "", str, padRight, "");
}

// fungsi untuk print integer dan double, bisa digunakan di semua jenis tabel

void printCenteredVal(double val, int dec, int colWidth) {
    char fmt[100];
    snprintf(fmt, 100, "%.*f", dec, val);
    printCenteredStr(fmt, colWidth);
}

// fungsi untuk menghitung lebar kolom tabel berdasarkan panjang string terpanjang, khusus untuk tabel iterasi komnum

int columnWidthIter(MethodResult *res, int dec) {
    int maxLen = 0;
    char fmt[100];
    for (int i = 0; i < res->count; i++) {
        int len;
        len = snprintf(fmt, 100, "%.*f", dec, res->rows[i].xr);
        if (len > maxLen) maxLen = len;
        len = snprintf(fmt, 100, "%.*f", dec, res->rows[i].ea);
        if (len > maxLen) maxLen = len;
        len = snprintf(fmt, 100, "%.*f", dec, res->rows[i].et);
        if (len > maxLen) maxLen = len;
        len = snprintf(fmt, 100, "%.*f", dec, res->rows[i].xl);
        if (len > maxLen) maxLen = len;
        len = snprintf(fmt, 100, "%.*f", dec, res->rows[i].xu);
        if (len > maxLen) maxLen = len;
        len = snprintf(fmt, 100, "%.*f", dec, res->rows[i].xi);
        if (len > maxLen) maxLen = len;
        len = snprintf(fmt, 100, "%.*f", dec, res->rows[i].xi_1);
        if (len > maxLen) maxLen = len;
    }
    return maxLen + 4;
}

// fungsi untuk print garis horizontal tabel, bisa digunakan di semua jenis tabel

void printHLine(int critCol, const char *left, const char *mid, const char *right, const char *line, int col, int nCols) {
    printf("%s", left);
    for (int i = 0; i < critCol; i++) printf("%s", line);
    for (int i = 0; i < nCols; i++) {
        printf("%s", mid);
        for (int j = 0; j < col; j++) printf("%s", line);
    }
    printf("%s\n", right);
}

// fungsi untuk print analisis dan kesimpulan konvergensi, berlaku untuk semua jenis metode

void printAnalysis(FunctionConfig *cfg, MethodResult *res, StopCriteria sc, const char *methodName, int *convStatus) {
    int dec = cfg->decimal;
    char fmt[200];
    *convStatus = 0;
    printf("\n");
    IterationResult last = res->rows[res->count - 1];
    double final_f = evaluateFunction(cfg, last.xr);
    if (res->count == 1) {
        // iterasi pertama langsung memenuhi kriteria berhenti
        printf("※   Analisis    :  ea langsung memenuhi kriteria berhenti pada iterasi pertama.\n");
        printf("    Kesimpulan  :  ");
        sprintf(fmt, "xr konvergen sangat cepat untuk metode %s pada xr = %%.%df dengan ea = %%.%df%%%%.\n\n", methodName, dec, dec);
        printf(fmt, last.xr, last.ea);
        *convStatus = 1;
    } else {
        // ambil dua iterasi terakhir untuk mendeteksi tren ea
        IterationResult prev  = res->rows[res->count - 2];
        IterationResult prev2 = (res->count >= 3) ? res->rows[res->count - 3] : prev;
        // konvergen jika ea turun di dua langkah terakhir, divergen jika naik
        int converge   = (last.ea < prev.ea) && (prev.ea < prev2.ea);
        int diverge    = (last.ea > prev.ea) && (prev.ea > prev2.ea);
        // khusus euler: deteksi kasus asimtotik — xr melayang jauh tapi f(xr) tidak mencapai nol
        int isDrifting = (fabs(last.xr) > 20.0 && fabs(final_f) < 1e-4 && final_f > 0 && cfg->funcType == EULER);
        printf("※   Analisis    :  ");
        if (isDrifting) {
            printf("xr terus mengecil menuju -inf tanpa menyentuh nol (Asimtotik).\n");
            printf("    Kesimpulan  :  Metode %s gagal menemukan akar real karena f(x) selalu positif.\n\n", methodName);
            *convStatus = 0;
        } else if (converge) {
            printf("ea terus menurun secara konsisten.\n");
            printf("    Kesimpulan  :  ");
            sprintf(fmt, "xr konvergen untuk metode %s pada xr = %%.%df dengan ea = %%.%df%%%%.\n\n", methodName, dec, dec); 
            printf(fmt, last.xr, last.ea);
            *convStatus = 1;
        } else if (diverge) {
            printf("ea terus menaik secara konsisten.\n");
            printf("    Kesimpulan  :  ");
            sprintf(fmt, "xr divergen untuk metode %s dengan jumlah iterasi %d dan ea terakhir = %%.%df%%%%.\n\n", methodName, res->count, dec); 
            printf(fmt, last.ea);
            *convStatus = 0;
        } else {
            // ea tidak menunjukkan tren yang jelas — tidak dapat disimpulkan
            printf("ea tidak konsisten menurun atau menaik.\n");
            printf("    Kesimpulan  :  ");
            printf("Tidak dapat menyimpulkan apabila xr konvergen sangat lambat atau xr divergen untuk metode %s.\n\n", methodName);
            *convStatus = 0;
        }
    }
}

// fungsi untuk print tabel bisection dan false-position, digabung karena kolom yang ditampilkan sama

void printBisectionFalsePosition(FunctionConfig *cfg, StopCriteria sc, MethodResult *res) {
    int dec = cfg->decimal;
    int col = columnWidthIter(res, dec);
    int nCols;
    // kolom et hanya ditampilkan untuk fungsi polinomial karena euler tidak memiliki true root
    int hasTrue = (cfg->funcType == POLYNOMIAL);
    if (hasTrue) { nCols = 5; } else { nCols = 4; }
    // print baris header tabel
    printHLine(11, "\U0000250C", "\U0000252C", "\U00002510", "\U00002500", col, nCols);
    printf("\U00002502  Iterasi  \U00002502");
    printCenteredStr("xl", col);
    printCenteredStr("xu", col);
    printCenteredStr("xr", col);
    printCenteredStr("ea (%)", col);
    if (hasTrue) {
        printCenteredStr("et (%)", col);
    }
    printf("\n");
    printHLine(11, "\U0000251C", "\U0000253C", "\U00002524", "\U00002500", col, nCols);
    // print setiap baris iterasi
    for (int i = 0; i < res->count; i++) {
        char buff[10];
        snprintf(buff, 10, "%d", res->rows[i].iter);
        printf("\U00002502");
        printCenteredStr(buff, 11);
        printCenteredVal(res->rows[i].xl, dec, col);
        printCenteredVal(res->rows[i].xu, dec, col);
        printCenteredVal(res->rows[i].xr, dec, col);
        printCenteredVal(res->rows[i].ea, dec, col);
        if (hasTrue) {
            printCenteredVal(res->rows[i].et, dec, col);
        }
        printf("\n");
        // print garis pemisah antar baris kecuali setelah baris terakhir
        if (i < res->count - 1) {
            printHLine(11, "\U0000251C", "\U0000253C", "\U00002524", "\U00002500", col, nCols);
        }
    }
    printHLine(11, "\U00002514", "\U00002534", "\U00002518", "\U00002500", col, nCols);
}

// fungsi untuk print tabel newton-raphson

void printNewtonRaphson(FunctionConfig *cfg, StopCriteria sc, MethodResult *res) {
    int dec = cfg->decimal;
    int col = columnWidthIter(res, dec);
    int nCols;
    // kolom et hanya ditampilkan untuk fungsi polinomial karena euler tidak memiliki true root
    int hasTrue = (cfg->funcType == POLYNOMIAL);
    if (hasTrue) { nCols = 4; } else { nCols = 3; }
    // print baris header tabel
    printHLine(11, "\U0000250C", "\U0000252C", "\U00002510", "\U00002500", col, nCols);
    printf("\U00002502  Iterasi  \U00002502");
    printCenteredStr("xi", col);
    printCenteredStr("xr", col);
    printCenteredStr("ea (%)", col);
    if (hasTrue) {
        printCenteredStr("et (%)", col);
    }
    printf("\n");
    printHLine(11, "\U0000251C", "\U0000253C", "\U00002524", "\U00002500", col, nCols);
    // print setiap baris iterasi
    for (int i = 0; i < res->count; i++) {
        char buff[10];
        snprintf(buff, 10, "%d", res->rows[i].iter);
        printf("\U00002502");
        printCenteredStr(buff, 11);
        printCenteredVal(res->rows[i].xi, dec, col);
        printCenteredVal(res->rows[i].xr, dec, col);
        printCenteredVal(res->rows[i].ea, dec, col);
        if (hasTrue) {
            printCenteredVal(res->rows[i].et, dec, col);
        }
        printf("\n");
        // print garis pemisah antar baris kecuali setelah baris terakhir
        if (i < res->count - 1) {
            printHLine(11, "\U0000251C", "\U0000253C", "\U00002524", "\U00002500", col, nCols);
        }
    }
    printHLine(11, "\U00002514", "\U00002534", "\U00002518", "\U00002500", col, nCols);
}

// fungsi untuk print tabel secant

void printSecant(FunctionConfig *cfg, StopCriteria sc, MethodResult *res) {
    int dec = cfg->decimal;
    int col = columnWidthIter(res, dec);
    int nCols;
    // kolom et hanya ditampilkan untuk fungsi polinomial karena euler tidak memiliki true root
    int hasTrue = (cfg->funcType == POLYNOMIAL);
    if (hasTrue) { nCols = 5; } else { nCols = 4; }
    // print baris header tabel
    printHLine(11, "\U0000250C", "\U0000252C", "\U00002510", "\U00002500", col, nCols);
    printf("\U00002502  Iterasi  \U00002502");
    printCenteredStr("xi-1", col);
    printCenteredStr("xi", col);
    printCenteredStr("xr", col);
    printCenteredStr("ea (%)", col);
    if (hasTrue) {
        printCenteredStr("et (%)", col);
    }
    printf("\n");
    printHLine(11, "\U0000251C", "\U0000253C", "\U00002524", "\U00002500", col, nCols);
    // print setiap baris iterasi
    for (int i = 0; i < res->count; i++) {
        char buff[10];
        snprintf(buff, 10, "%d", res->rows[i].iter);
        printf("\U00002502");
        printCenteredStr(buff, 11);
        printCenteredVal(res->rows[i].xi_1, dec, col);
        printCenteredVal(res->rows[i].xi, dec, col);
        printCenteredVal(res->rows[i].xr, dec, col);
        printCenteredVal(res->rows[i].ea, dec, col);
        if (hasTrue) {
            printCenteredVal(res->rows[i].et, dec, col);
        }
        printf("\n");
        // print garis pemisah antar baris kecuali setelah baris terakhir
        if (i < res->count - 1) {
            printHLine(11, "\U0000251C", "\U0000253C", "\U00002524", "\U00002500", col, nCols);
        }
    }
    printHLine(11, "\U00002514", "\U00002534", "\U00002518", "\U00002500", col, nCols);
}

// fungsi untuk menghitung lebar kolom tabel berdasarkan panjang string terpanjang, khusus untuk tabel rangkuman hasil semua metode

int columnWidthSumm(SummaryEntry *summary, int count, int dec) {
    int maxLen = 0;
    char fmt[100];
    for (int i = 0; i < count; i++) {
        int len;
        len = snprintf(fmt, 100, "%.*f", dec, summary[i].xr);
        if (len > maxLen) maxLen = len;
        len = snprintf(fmt, 100, "%.*f", dec, summary[i].ea);
        if (len > maxLen) maxLen = len;
        len = snprintf(fmt, 100, "%.*f", dec, summary[i].et);
        if (len > maxLen) maxLen = len;
        len = 11;
        if (len > maxLen) maxLen = len;
    }
    return maxLen + 4;
}

// fungsi untuk print tabel rangkuman hasil semua metode

void printSummaryTable(FunctionConfig *cfg, StopCriteria sc, SummaryEntry summary[], int count) {
    int dec = cfg->decimal;
    int col = columnWidthSumm(summary, count, dec);
    int nCols;
    char fmt[200];
    // kolom et hanya ditampilkan untuk fungsi polinomial karena euler tidak memiliki true root
    int hasTrue = (cfg->funcType == POLYNOMIAL);
    if (hasTrue) { nCols = 5; } else { nCols = 4; }
    // print judul dan informasi kriteria berhenti yang digunakan
    printf("Tabel Kesimpulan Hasil Komputasi Numerik\n\n");
    if (sc.mode == ITER || sc.mode == BOTH) { printf("Iterasi Maksimum : %d\n", sc.maxIter); }
    if (sc.mode == EA || sc.mode == BOTH) { sprintf(fmt, "%%%% Stopping Error : %%g%%%%\n", dec); printf(fmt, sc.stopError); }
    printf("\n");
    // print baris header tabel
    printHLine(18, "\U0000250C", "\U0000252C", "\U00002510", "\U00002500", col, nCols);
    printf("\U00002502");
    printCenteredStr("Metode", 18);
    printCenteredStr("Iterasi", col);
    printCenteredStr("xr", col);
    printCenteredStr("ea (%)", col);
    if (hasTrue) {
        printCenteredStr("et (%)", col);
    }
    printCenteredStr("Konvergensi", col);
    printf("\n");
    printHLine(18, "\U0000251C", "\U0000253C", "\U00002524", "\U00002500", col, nCols);
    // print satu baris per metode yang dipilih
    for (int i = 0; i < count; i++) {
        char buff[10];
        sprintf(buff, "%d", summary[i].iterations);
        printf("\U00002502");
        printCenteredStr(summary[i].name, 18);
        printCenteredStr(buff, col);
        printCenteredVal(summary[i].xr, dec, col);
        printCenteredVal(summary[i].ea, dec, col);
        if (hasTrue) {
            printCenteredVal(summary[i].et, dec, col);
        }
        printCenteredStr(summary[i].convStatus ? "Konvergen" : "Divergen", col);
        printf("\n");
        if (i < count - 1) {
            printHLine(18, "\U0000251C", "\U0000253C", "\U00002524", "\U00002500", col, nCols);
        }
    }
    printHLine(18, "\U00002514", "\U00002534", "\U00002518", "\U00002500", col, nCols);
    printf("\n");
}

// fungsi untuk print tabel skoring metode, hanya digunakan apabila user memilih lebih dari satu metode

void printScoringTable(FunctionConfig *cfg, SummaryEntry summary[], int count) {
    int col = 18;
    // kolom %et hanya ditampilkan untuk fungsi polinomial karena euler tidak memiliki true root
    int hasTrue = (cfg->funcType == POLYNOMIAL);
    char fmt[100];
    int totals[4] = {0};
    printf("Tabel Skoring Metode Komputasi Numerik\n\n");
    printf("※   Konvergen = 1 | Divergen = 0 | Terkecil = %d | Terbesar = 1\n\n", count);
    // print baris header tabel
    printHLine(23, "\U0000250C", "\U0000252C", "\U00002510", "\U00002500", col, count);
    printf("\U00002502");
    printCenteredStr("Kriteria", 23);
    for(int i = 0; i < count; i++) printCenteredStr(summary[i].name, col);
    printf("\n");
    printHLine(23, "\U0000251C", "\U0000253C", "\U00002524", "\U00002500", col, count);
    // kriteria 1: positif konvergensi (1 jika konvergen, 0 jika divergen)
    printf("\U00002502");
    printCenteredStr("Positif Konvergensi", 23);
    for(int i = 0; i < count; i++) {
        sprintf(fmt, "%d", summary[i].convStatus);
        printCenteredStr(fmt, col);
        totals[i] += summary[i].convStatus;
    }
    printf("\n");
    printHLine(23, "\U0000251C", "\U0000253C", "\U00002524", "\U00002500", col, count);
    // kriteria 2: rank iterasi terkecil (semakin sedikit iterasi semakin tinggi skor)
    printf("\U00002502");
    printCenteredStr("Iterasi Terkecil", 23);
    for(int i = 0; i < count; i++) {
        int rank = 1;
        for(int j = 0; j < count; j++) {
            if (summary[i].iterations < summary[j].iterations) rank++;
        }
        sprintf(fmt, "%d", rank);
        printCenteredStr(fmt, col);
        totals[i] += rank;
    }
    printf("\n");
    printHLine(23, "\U0000251C", "\U0000253C", "\U00002524", "\U00002500", col, count);
    // kriteria 3: rank %ea terkecil (semakin kecil ea semakin tinggi skor)
    printf("\U00002502");
    printCenteredStr("%ea Terkecil", 23);
    for(int i = 0; i < count; i++) {
        int rank = 1;
        for(int j = 0; j < count; j++) {
            if (summary[i].ea < summary[j].ea) rank++;
        }
        sprintf(fmt, "%d", rank);
        printCenteredStr(fmt, col);
        totals[i] += rank;
    }
    printf("\n");
    // kriteria 4: rank %et terkecil — hanya untuk polinomial
    if (hasTrue) {
        printHLine(23, "\U0000251C", "\U0000253C", "\U00002524", "\U00002500", col, count);
        printf("\U00002502");
        printCenteredStr("%et Terkecil", 23);
        for(int i = 0; i < count; i++) {
            int rank = 1;
            for(int j = 0; j < count; j++) {
                if (summary[i].et < summary[j].et) rank++;
            }
            sprintf(fmt, "%d", rank);
            printCenteredStr(fmt, col);
            totals[i] += rank;
        }
        printf("\n");
    }
    // hitung total skor dan cari metode terbaik
    // jika skor sama, metode dengan ea lebih kecil dipilih sebagai pemenang
    printHLine(23, "\U0000251C", "\U0000253C", "\U00002524", "\U00002500", col, count);
    printf("\U00002502");
    printCenteredStr("Total", 23);
    int maxTotal = -1;
    int bestIdx = 0;
    for(int i = 0; i < count; i++) {
        sprintf(fmt, "%d", totals[i]);
        printCenteredStr(fmt, col);
        if (totals[i] > maxTotal || (totals[i] == maxTotal && summary[i].ea < summary[bestIdx].ea)) {
            maxTotal = totals[i];
            bestIdx = i;
        }
    }
    printf("\n");
    printHLine(23, "\U00002514", "\U00002534", "\U00002518", "\U00002500", col, count);
    printf("\n");
    // print kesimpulan: jika pemenang divergen, pilih metode konvergen dengan skor tertinggi
    if (!summary[bestIdx].convStatus) {
        int foundConverged = 0;
        int bestConvergedIdx = 0;
        for (int i = 0; i < count; i++) {
            if (summary[i].convStatus) {
                if (!foundConverged || totals[i] > totals[bestConvergedIdx]) {
                    bestConvergedIdx = i;
                    foundConverged = 1;
                }
            }
        }
        if (foundConverged) {
            printf("※   Kesimpulan  :  Metode %s adalah metode yang memberikan hasil komputasi paling baik dan cocok untuk f(x).\n", summary[bestConvergedIdx].name);
            printf("                   (Metode %s memiliki skor lebih tinggi namun dinyatakan Divergen dan tidak diperhitungkan.)\n\n", summary[bestIdx].name);
        } else {
            // semua metode divergen
            printf("※   Kesimpulan  :  Tidak ada metode yang berhasil konvergen untuk f(x) ini.\n");
            printf("                   Semua metode yang dipilih divergen atau gagal menemukan akar real.\n\n");
        }
    } else {
        printf("※   Kesimpulan  :  Metode %s adalah metode yang memberikan hasil komputasi paling baik dan cocok untuk f(x).\n\n", summary[bestIdx].name);
    }
}

// fungsi untuk menulis data hasil komputasi numerik ke file sim_data.txt,
// digunakan sebagai jembatan data antara finpro.c dan simulation.exe

void writeSimData(const FunctionConfig *cfg, const StopCriteria *sc, const MethodResult results[4]) {
    const char *path = "D:/CPP VSC/raylib/sim_data.txt";     
    FILE *f = fopen(path, "w");
    if (!f) {
        printf("※   Peringatan: Gagal menulis sim_data.txt (%s)\n\n", path);
        return;
    }
    // tulis jenis fungsi dan derajat polinomial (jika polinomial)
    fprintf(f, "FUNCTYPE    %s\n", cfg->funcType == EULER ? "EULER" : "POLY");
    if (cfg->funcType == POLYNOMIAL) {
        fprintf(f, "DEGREE      %d\n", (int)cfg->degree);
    }
    // ekstrak koefisien keempat variabel a/b/c/d — padding 0 untuk derajat yang lebih rendah
    double a = 0, b = 0, c = 0, d = 0;
    switch (cfg->funcType) {
        case POLYNOMIAL:
            switch (cfg->degree) {
                case LINEAR:    a = cfg->param.linear[0];    b = cfg->param.linear[1];    break;
                case QUADRATIC: a = cfg->param.quadratic[0]; b = cfg->param.quadratic[1];
                                c = cfg->param.quadratic[2]; break;
                case CUBIC:     a = cfg->param.cubic[0];     b = cfg->param.cubic[1];
                                c = cfg->param.cubic[2];     d = cfg->param.cubic[3];     break;
                default: break;
            }
            break;
        case EULER:
            a = cfg->param.euler[0]; b = cfg->param.euler[1];
            c = cfg->param.euler[2]; d = cfg->param.euler[3];
            break;
        default: break;
    }
    // tulis koefisien, tampilan desimal, metode yang dipilih, dan akar akhir per metode
    fprintf(f, "COEFFS      %.10f %.10f %.10f %.10f\n", a, b, c, d);
    fprintf(f, "DECIMAL     %d\n", (int)cfg->decimal);
    fprintf(f, "METHODS     %d %d %d %d\n",
        cfg->methodSelected[0], cfg->methodSelected[1],
        cfg->methodSelected[2], cfg->methodSelected[3]);
    fprintf(f, "ROOTS       %.10f %.10f %.10f %.10f\n",
        cfg->methodSelected[0] ? results[0].root : 0.0,
        cfg->methodSelected[1] ? results[1].root : 0.0,
        cfg->methodSelected[2] ? results[2].root : 0.0,
        cfg->methodSelected[3] ? results[3].root : 0.0);
    // tulis data iterasi per metode — blok tetap ditulis meski n = 0 agar reader seragam
    {
        int n = cfg->methodSelected[0] ? results[0].count : 0;
        fprintf(f, "BISECTION   %d\n", n);
        for (int i = 0; i < n; i++) {
            const IterationResult *r = &results[0].rows[i];
            fprintf(f, "%.10f %.10f %.10f %.10f %.10f\n",
                r->xl, r->xu, r->xr, r->ea, r->et);
        }
    }
    {
        int n = cfg->methodSelected[1] ? results[1].count : 0;
        fprintf(f, "FALSEPOS    %d\n", n);
        for (int i = 0; i < n; i++) {
            const IterationResult *r = &results[1].rows[i];
            fprintf(f, "%.10f %.10f %.10f %.10f %.10f\n",
                r->xl, r->xu, r->xr, r->ea, r->et);
        }
    }
    {
        int n = cfg->methodSelected[2] ? results[2].count : 0;
        fprintf(f, "NEWTON      %d\n", n);
        for (int i = 0; i < n; i++) {
            const IterationResult *r = &results[2].rows[i];
            // newton tidak memiliki xl/xu, hanya xi dan xr
            fprintf(f, "%.10f %.10f %.10f %.10f\n",
                r->xi, r->xr, r->ea, r->et);
        }
    }
    {
        int n = cfg->methodSelected[3] ? results[3].count : 0;
        fprintf(f, "SECANT      %d\n", n);
        for (int i = 0; i < n; i++) {
            const IterationResult *r = &results[3].rows[i];
            fprintf(f, "%.10f %.10f %.10f %.10f %.10f\n",
                r->xi_1, r->xi, r->xr, r->ea, r->et);
        }
    }
    fclose(f);
}

// fungsi untuk menerima input pilihan tampil/tidak tampil grafik simulasi,
// lalu menulis sim_data.txt dan menjalankan simulation.exe apabila user memilih ya

void inputShowGraph(FunctionConfig *cfg, StopCriteria *sc, MethodResult results[], int index) {
    int showGraph;
    printf("Apakah Anda ingin melihat Grafik Simulasi? [ Masukkan Angka 1/0 ]\n\n");
    printf("[1] Ya\n");
    printf("[0] Tidak\n\n");
    do {
        printf("Pilihan Anda  :  ");
        while (!scanInt(&showGraph)) {
            printf("\n");
            printf("Pilihan Anda tidak valid! Tolong masukkan angka 1/0.\n\n");
            printf("Pilihan Anda  :  ");
        }
        printf("\n");
        if (showGraph != 0 && showGraph != 1) {
            printf("Pilihan Anda tidak valid! Tolong masukkan angka 1/0.\n\n");
        }
    } while (showGraph != 0 && showGraph != 1);

    if (showGraph == 1 && index > 0) {
        printf("Membuka Grafik Simulasi...\n\n");
        // tulis sim_data.txt terlebih dahulu sebelum meluncurkan simulation.exe
        writeSimData(cfg, sc, results);

        char cmdBuf[512];
        snprintf(cmdBuf, sizeof(cmdBuf), "\"D:/CPP VSC/raylib/simulation.exe\"");

        // gunakan CreateProcess bukan system() agar stdin finpro tidak dirampas oleh cmd.exe
        // WaitForSingleObject memblokir finpro hingga jendela simulasi ditutup user
        STARTUPINFO si = {0};
        PROCESS_INFORMATION pi = {0};
        si.cb = sizeof(si);
        CreateProcess(NULL, cmdBuf, NULL, NULL, FALSE, 0, NULL, "D:/CPP VSC/raylib", &si, &pi);
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        fflush(stdout);
    } else if (showGraph == 0) {
        printf("Grafik Simulasi tidak ditampilkan.\n");
    }
}

/* PROGRAM UTAMA */

int main() {
    FunctionConfig cfg;
    StopCriteria sc;
    SummaryEntry summaryList[4];
    int index, conv;

    // print header program
    printf("========================================================================================================================\n\n");
    printf("                                  PROGRAM KALKULATOR ROOT FINDING - POLINOMIAL & EULER\n");
    printf("                                         Final Project Praktikum Pemrograman C\n\n");
    printf("Oleh: Kelompok 5\n");
    printf("- Raden Ayu Athifah Qurrota'aini (2406408230)\n");
    printf("- Annabell Della Sumantri        (2406415040)\n");
    printf("- Keira Khairani Haqi            (2406419562)\n");
    printf("- Dwidra Audric Farras           (2406426265)\n\n");
    printf("========================================================================================================================\n\n");

    printf("Selamat datang di Kalkulator Root Finding!\n\n");
    printf("========================================================================================================================\n\n");

    do {
        // reset semua variabel di awal setiap loop agar tidak ada data sisa dari iterasi sebelumnya
        cfg = (FunctionConfig){0};
        sc = (StopCriteria){BOTH, MAX_ITER, DEFAULT_ES};
        memset(summaryList, 0, sizeof(summaryList));
        index = 0; 

        // input jenis fungsi, derajat polinomial (jika polinomial), dan tampilan desimal
        inputFunctionType(&cfg);
        printf("========================================================================================================================\n\n");

        if (cfg.funcType == POLYNOMIAL) {
            inputPolynomialDegree(&cfg);
            printf("========================================================================================================================\n\n");
        }

        inputDecimalNumbers(&cfg);
        printf("========================================================================================================================\n\n");

        // input koefisien fungsi, pilihan metode, dan kriteria berhenti iterasi
        inputFunctionParameter(&cfg);
        printf("========================================================================================================================\n\n");

        inputRootMethods(&cfg);
        printf("========================================================================================================================\n\n");
        
        
        inputStopMode(&cfg, &sc);
        printf("========================================================================================================================\n\n");

        // input bracket hanya jika bisection atau false-position dipilih
        if (cfg.methodSelected[BISECTION - 1] || cfg.methodSelected[FALSE_POSITION - 1]) {
            inputBracket(&cfg);
            printf("========================================================================================================================\n\n");
        }

        // input initial guess hanya jika newton-raphson dipilih
        if (cfg.methodSelected[NEWTON_RAPHSON - 1]) {
            inputGuessNewtonRaphson(&cfg);
            printf("========================================================================================================================\n\n");
        }

        // input initial guesses hanya jika secant dipilih
        if (cfg.methodSelected[SECANT - 1]) {
            inputGuessSecant(&cfg);
            printf("========================================================================================================================\n\n");
        }

        // jalankan setiap metode yang dipilih, print tabel iterasi dan analisis konvergensi,
        // lalu simpan hasilnya ke summaryList untuk ditampilkan di tabel rangkuman
        MethodResult results[4];
        memset(results, 0, sizeof(results));
 
        if (cfg.methodSelected[BISECTION - 1]) {
            results[0] = methodBisection(&cfg, sc);
            printf("Tabel Iterasi Metode Bisection\n\n");
            printBisectionFalsePosition(&cfg, sc, &results[0]);
            printAnalysis(&cfg, &results[0], sc, "Bisection", &conv);
            summaryList[index].name        = "Bisection";
            summaryList[index].iterations  = results[0].count;
            summaryList[index].xr          = results[0].root;
            summaryList[index].ea          = results[0].finalEa;
            summaryList[index].et          = results[0].finalEt;
            summaryList[index].convStatus  = conv;
            index++;
            printf("========================================================================================================================\n\n");
        }
 
        if (cfg.methodSelected[FALSE_POSITION - 1]) {
            results[1] = methodFalsePosition(&cfg, sc);
            printf("Tabel Iterasi Metode False Position\n\n");
            printBisectionFalsePosition(&cfg, sc, &results[1]);
            printAnalysis(&cfg, &results[1], sc, "False-Position", &conv);
            summaryList[index].name        = "False-Position";
            summaryList[index].iterations  = results[1].count;
            summaryList[index].xr          = results[1].root;
            summaryList[index].ea          = results[1].finalEa;
            summaryList[index].et          = results[1].finalEt;
            summaryList[index].convStatus  = conv;
            index++;
            printf("========================================================================================================================\n\n");
        }
 
        if (cfg.methodSelected[NEWTON_RAPHSON - 1]) {
            results[2] = methodNewtonRaphson(&cfg, sc);
            printf("Tabel Iterasi Newton Raphson\n\n");
            printNewtonRaphson(&cfg, sc, &results[2]);
            printAnalysis(&cfg, &results[2], sc, "Newton-Raphson", &conv);
            summaryList[index].name        = "Newton-Raphson";
            summaryList[index].iterations  = results[2].count;
            summaryList[index].xr          = results[2].root;
            summaryList[index].ea          = results[2].finalEa;
            summaryList[index].et          = results[2].finalEt;
            summaryList[index].convStatus  = conv;
            index++;
            printf("========================================================================================================================\n\n");
        }
 
        if (cfg.methodSelected[SECANT - 1]) {
            results[3] = methodSecant(&cfg, sc);
            printf("Tabel Iterasi Metode Secant\n\n");
            printSecant(&cfg, sc, &results[3]);
            printAnalysis(&cfg, &results[3], sc, "Secant", &conv);
            summaryList[index].name        = "Secant";
            summaryList[index].iterations  = results[3].count;
            summaryList[index].xr          = results[3].root;
            summaryList[index].ea          = results[3].finalEa;
            summaryList[index].et          = results[3].finalEt;
            summaryList[index].convStatus  = conv;
            index++;
            printf("========================================================================================================================\n\n");
        }
 
        // print tabel rangkuman semua metode yang dipilih,
        // dan tabel skoring hanya jika lebih dari satu metode dipilih
        if (index > 0) {
            printSummaryTable(&cfg, sc, summaryList, index);
            if (index > 1) {
                printScoringTable(&cfg, summaryList, index);
            }
            printf("========================================================================================================================\n\n");
        }
 
        // tanya user apakah ingin menampilkan simulasi pencarian akar
        inputShowGraph(&cfg, &sc, results, index);
        printf("\n========================================================================================================================\n\n");

        // tanya user apakah ingin menghitung lagi atau keluar
        inputExitChoice(&cfg);
        printf("========================================================================================================================\n\n");
    } while (cfg.exitChoice == CALCULATE);
    return 0;
}
