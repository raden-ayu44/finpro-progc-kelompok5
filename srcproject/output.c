#include "finpro.h"
#include "output.h"
#include "evaluate.h"

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