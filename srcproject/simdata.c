#include "finpro.h"
#include "simdata.h"
#include "input.h"

// fungsi untuk menulis data hasil komputasi numerik ke file sim_data.txt,
// digunakan sebagai jembatan data antara finpro.c dan simulation.exe

void writeSimData(const FunctionConfig *cfg, const StopCriteria *sc, const MethodResult results[4]) {
    const char *path = "sim_data.txt";     
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
        snprintf(cmdBuf, sizeof(cmdBuf), "simulation.exe");

        // gunakan CreateProcess bukan system() agar stdin finpro tidak dirampas oleh cmd.exe
        // WaitForSingleObject memblokir finpro hingga jendela simulasi ditutup user
        STARTUPINFO si = {0};
        PROCESS_INFORMATION pi = {0};
        si.cb = sizeof(si);
        CreateProcess(NULL, cmdBuf, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        fflush(stdout);
    } else if (showGraph == 0) {
        printf("Grafik Simulasi tidak ditampilkan.\n");
    }
}