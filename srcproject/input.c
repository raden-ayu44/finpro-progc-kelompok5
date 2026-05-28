#include "finpro.h"
#include "input.h"
#include "evaluate.h"
#include "output.h"


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