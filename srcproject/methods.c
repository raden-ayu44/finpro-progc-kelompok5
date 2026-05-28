#include "finpro.h"
#include "methods.h"
#include "evaluate.h"


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