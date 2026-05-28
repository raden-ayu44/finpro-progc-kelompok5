#include "finpro.h"
#include "input.h"
#include "evaluate.h"
#include "output.h"
#include "methods.h"
#include "simdata.h"

/* PROGRAM UTAMA */

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
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
