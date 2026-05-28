#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* DEFINISI WARNA */

#define COL_BISECTION RED
#define COL_FALSEPOS  BLUE
#define COL_NEWTON    GREEN
#define COL_SECANT    YELLOW
#define COL_GREY      (Color){150, 150, 150, 255}
#define COL_PURPLE    (Color){150, 0, 255, 255}
#define COL_PINK      (Color){255, 0, 200, 255}
#define COL_DARKGREY  (Color){80, 80, 80, 255}

/* VARIABEL GLOBAL UNTUK EVALUASI FUNGSI */

// diisi oleh readSimData() sebelum window dibuka

static int   g_funcType;   // 1 = POLY, 2 = EULER
static int   g_degree;     // 1 = linier, 2 = kuadratik, 3 = kubik (khusus POLY)
static float g_a, g_b, g_c, g_d;

// fungsi untuk mengevaluasi f(x) berdasarkan jenis fungsi dan derajat polinomial

float evalF(float x, float a, float b, float c, float d) {
    if (g_funcType == 2) {
        return expf(a * x + b) + c * x + d;
    }
    switch (g_degree) {
        case 1:  return a * x + b;
        case 2:  return a * x * x + b * x + c;
        default: return a * x * x * x + b * x * x + c * x + d;
    }
}

// fungsi untuk mengevaluasi f'(x), digunakan untuk metode newton-raphson

float evalDF(float x, float a, float b, float c) {
    if (g_funcType == 2) {
        return a * expf(a * x + b) + c;
    }
    switch (g_degree) {
        case 1:  return a;
        case 2:  return 2.0f * a * x + b;
        default: return 3.0f * a * x * x + 2.0f * b * x + c;
    }
}

/* KUMPULAN FUNGSI BANTU RAYLIB */

// fungsi untuk menggambar teks menggunakan font kustom

void DrawTextMC(Font font, const char *text, int x, int y, int size, Color color) {
    DrawTextEx(font, text, (Vector2){x, y}, size, 1, color);
}

// fungsi untuk mengukur lebar teks menggunakan font kustom

int MeasureTextMC(Font font, const char *text, int size) {
    return (int)MeasureTextEx(font, text, size, 1).x;
}

// fungsi untuk membatasi nilai integer agar berada di antara lo dan hi

static inline int iclamp(int v, int lo, int hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

// variabel batas area grafik, diisi oleh SetGraphBounds()

static int _gX, _gY, _gW, _gH;

// fungsi untuk menyimpan batas area grafik ke variabel global

static inline void SetGraphBounds(int x, int y, int w, int h) {
    _gX = x; _gY = y; _gW = w; _gH = h;
}

// fungsi untuk mengecek apakah koordinat piksel berada di dalam area grafik

static inline bool InGraph(int x, int y) {
    return x >= _gX && x <= _gX + _gW && y >= _gY && y <= _gY + _gH;
}

// fungsi untuk menggambar garis vertikal putus-putus, digunakan untuk konstruksi geometri tiap metode

void DrawDashedVLine(int x, int y0, int y1, Color col) {
    x  = iclamp(x,  _gX, _gX + _gW);
    y0 = iclamp(y0, _gY, _gY + _gH);
    y1 = iclamp(y1, _gY, _gY + _gH);
    int dash = 6, gap = 4;
    if (y0 > y1) { int t = y0; y0 = y1; y1 = t; }
    for (int y = y0; y < y1; y += dash + gap)
        DrawLine(x, y, x, (y + dash < y1 ? y + dash : y1), col);
}

// fungsi untuk menggambar garis horizontal putus-putus, digunakan untuk konstruksi geometri metode newton-raphson dan secant

void DrawDashedHLine(int x0, int x1, int y, Color col) {
    y  = iclamp(y,  _gY, _gY + _gH);
    x0 = iclamp(x0, _gX, _gX + _gW);
    x1 = iclamp(x1, _gX, _gX + _gW);
    int dash = 6, gap = 4;
    if (x0 > x1) { int t = x0; x0 = x1; x1 = t; }
    for (int x = x0; x < x1; x += dash + gap)
        DrawLine(x, y, (x + dash < x1 ? x + dash : x1), y, col);
}

// fungsi untuk menggambar garis dengan clipping ke batas area grafik (Cohen-Sutherland)

static bool DrawLineG(int x0, int y0, int x1, int y1, Color col) {
    int xmin = _gX, xmax = _gX + _gW, ymin = _gY, ymax = _gY + _gH;
    #define OC(x,y) (((x)<xmin?1:0)|((x)>xmax?2:0)|((y)<ymin?8:0)|((y)>ymax?4:0))
    int oc0 = OC(x0, y0), oc1 = OC(x1, y1);
    for (int iter = 0; iter < 8; iter++) {
        if (!(oc0 | oc1)) { DrawLine(x0, y0, x1, y1, col); return true; }
        if (oc0 & oc1) return false;
        int oc = oc0 ? oc0 : oc1, nx, ny;
        if      (oc & 4) { nx = x0 + (int)((float)(x1-x0)*(ymax-y0)/(y1-y0+1e-6f)); ny = ymax; }
        else if (oc & 8) { nx = x0 + (int)((float)(x1-x0)*(ymin-y0)/(y1-y0+1e-6f)); ny = ymin; }
        else if (oc & 2) { ny = y0 + (int)((float)(y1-y0)*(xmax-x0)/(x1-x0+1e-6f)); nx = xmax; }
        else             { ny = y0 + (int)((float)(y1-y0)*(xmin-x0)/(x1-x0+1e-6f)); nx = xmin; }
        if (oc == oc0) { x0 = nx; y0 = ny; oc0 = OC(nx, ny); }
        else           { x1 = nx; y1 = ny; oc1 = OC(nx, ny); }
    }
    #undef OC
    return false;
}

// fungsi untuk menggambar garis tebal dengan clipping ke batas area grafik

static void DrawLineGEx(int x0, int y0, int x1, int y1, float thick, Color col) {
    int xmin = _gX, xmax = _gX + _gW, ymin = _gY, ymax = _gY + _gH;
    #define OC2(x,y) (((x)<xmin?1:0)|((x)>xmax?2:0)|((y)<ymin?8:0)|((y)>ymax?4:0))
    int oc0 = OC2(x0, y0), oc1 = OC2(x1, y1);
    for (int iter = 0; iter < 8; iter++) {
        if (!(oc0 | oc1)) { DrawLineEx((Vector2){x0,y0}, (Vector2){x1,y1}, thick, col); return; }
        if (oc0 & oc1) return;
        int oc = oc0 ? oc0 : oc1, nx, ny;
        if      (oc & 4) { nx = x0 + (int)((float)(x1-x0)*(ymax-y0)/(y1-y0+1e-6f)); ny = ymax; }
        else if (oc & 8) { nx = x0 + (int)((float)(x1-x0)*(ymin-y0)/(y1-y0+1e-6f)); ny = ymin; }
        else if (oc & 2) { ny = y0 + (int)((float)(y1-y0)*(xmax-x0)/(x1-x0+1e-6f)); nx = xmax; }
        else             { ny = y0 + (int)((float)(y1-y0)*(xmin-x0)/(x1-x0+1e-6f)); nx = xmin; }
        if (oc == oc0) { x0 = nx; y0 = ny; oc0 = OC2(nx, ny); }
        else           { x1 = nx; y1 = ny; oc1 = OC2(nx, ny); }
    }
    #undef OC2
}

/* INISIALISASI STRUCT DATA SIMULASI */

// struct untuk menyimpan array data iterasi per metode

typedef struct {
    float *xl, *xu, *xr;           // bisection & false-position: batas bawah, atas, dan tengah
    float *xi, *xr_nr;             // newton-raphson: tebakan awal dan tebakan baru
    float *xm1, *xi_sc, *xr_sc;   // secant: tebakan xi-1, xi, dan xr
    float *et;                     // true relative error per iterasi (0.0 untuk euler)
    int    n;                      // jumlah iterasi
} MethodData;

// struct utama untuk menyimpan semua data yang dibaca dari sim_data.txt

typedef struct {
    int        funcType;            // 1 = POLY, 2 = EULER
    int        degree;              // 1/2/3, khusus POLY
    float      a, b, c, d;         // koefisien fungsi
    int        decimal;             // pengaturan tampilan desimal
    int        methodSelected[4];  // metode yang dipilih: [0]=bis [1]=fp [2]=nr [3]=sec
    float      slideRoots[4];      // xr konvergen terakhir per metode
    MethodData methods[4];
} SimData;

// fungsi untuk membebaskan semua alokasi memori dinamis di dalam SimData

static void freeSimData(SimData *sd) {
    for (int m = 0; m < 4; m++) {
        free(sd->methods[m].xl);
        free(sd->methods[m].xu);
        free(sd->methods[m].xr);
        free(sd->methods[m].xi);
        free(sd->methods[m].xr_nr);
        free(sd->methods[m].xm1);
        free(sd->methods[m].xi_sc);
        free(sd->methods[m].xr_sc);
        free(sd->methods[m].et);
    }
}

// fungsi untuk membaca sim_data.txt yang ditulis oleh finpro.exe dan mengisi SimData

static int readSimData(const char *path, SimData *sd) {
    FILE *f = fopen(path, "r");
    if (!f) { fprintf(stderr, "Tidak dapat membuka %s\n", path); return 0; }

    char key[32];
    memset(sd, 0, sizeof(*sd));

    while (fscanf(f, "%31s", key) == 1) {

        if (strcmp(key, "FUNCTYPE") == 0) {
            char val[16];
            fscanf(f, "%15s", val);
            sd->funcType = (strcmp(val, "EULER") == 0) ? 2 : 1;

        } else if (strcmp(key, "DEGREE") == 0) {
            fscanf(f, "%d", &sd->degree);

        } else if (strcmp(key, "COEFFS") == 0) {
            fscanf(f, "%f %f %f %f", &sd->a, &sd->b, &sd->c, &sd->d);

        } else if (strcmp(key, "DECIMAL") == 0) {
            fscanf(f, "%d", &sd->decimal);

        } else if (strcmp(key, "METHODS") == 0) {
            fscanf(f, "%d %d %d %d",
                &sd->methodSelected[0], &sd->methodSelected[1],
                &sd->methodSelected[2], &sd->methodSelected[3]);

        } else if (strcmp(key, "ROOTS") == 0) {
            fscanf(f, "%f %f %f %f",
                &sd->slideRoots[0], &sd->slideRoots[1],
                &sd->slideRoots[2], &sd->slideRoots[3]);

        } else if (strcmp(key, "BISECTION") == 0) {
            int n; fscanf(f, "%d", &n);
            sd->methods[0].n  = n;
            sd->methods[0].xl = malloc(n * sizeof(float));
            sd->methods[0].xu = malloc(n * sizeof(float));
            sd->methods[0].xr = malloc(n * sizeof(float));
            sd->methods[0].et = malloc(n * sizeof(float));
            for (int i = 0; i < n; i++) {
                float xl, xu, xr, ea, et;
                fscanf(f, "%f %f %f %f %f", &xl, &xu, &xr, &ea, &et);
                sd->methods[0].xl[i] = xl;
                sd->methods[0].xu[i] = xu;
                sd->methods[0].xr[i] = xr;
                sd->methods[0].et[i] = et;
            }

        } else if (strcmp(key, "FALSEPOS") == 0) {
            int n; fscanf(f, "%d", &n);
            sd->methods[1].n  = n;
            sd->methods[1].xl = malloc(n * sizeof(float));
            sd->methods[1].xu = malloc(n * sizeof(float));
            sd->methods[1].xr = malloc(n * sizeof(float));
            sd->methods[1].et = malloc(n * sizeof(float));
            for (int i = 0; i < n; i++) {
                float xl, xu, xr, ea, et;
                fscanf(f, "%f %f %f %f %f", &xl, &xu, &xr, &ea, &et);
                sd->methods[1].xl[i] = xl;
                sd->methods[1].xu[i] = xu;
                sd->methods[1].xr[i] = xr;
                sd->methods[1].et[i] = et;
            }

        } else if (strcmp(key, "NEWTON") == 0) {
            int n; fscanf(f, "%d", &n);
            sd->methods[2].n     = n;
            sd->methods[2].xi    = malloc(n * sizeof(float));
            sd->methods[2].xr_nr = malloc(n * sizeof(float));
            sd->methods[2].et    = malloc(n * sizeof(float));
            for (int i = 0; i < n; i++) {
                float xi, xr, ea, et;
                fscanf(f, "%f %f %f %f", &xi, &xr, &ea, &et);
                sd->methods[2].xi[i]    = xi;
                sd->methods[2].xr_nr[i] = xr;
                sd->methods[2].et[i]    = et;
            }

        } else if (strcmp(key, "SECANT") == 0) {
            int n; fscanf(f, "%d", &n);
            sd->methods[3].n     = n;
            sd->methods[3].xm1   = malloc(n * sizeof(float));
            sd->methods[3].xi_sc = malloc(n * sizeof(float));
            sd->methods[3].xr_sc = malloc(n * sizeof(float));
            sd->methods[3].et    = malloc(n * sizeof(float));
            for (int i = 0; i < n; i++) {
                float xm1, xi, xr, ea, et;
                fscanf(f, "%f %f %f %f %f", &xm1, &xi, &xr, &ea, &et);
                sd->methods[3].xm1[i]   = xm1;
                sd->methods[3].xi_sc[i] = xi;
                sd->methods[3].xr_sc[i] = xr;
                sd->methods[3].et[i]    = et;
            }
        }
    }
    fclose(f);
    return 1;
}

// fungsi untuk menghitung rentang tampilan grafik secara otomatis berdasarkan data iterasi

static void calcViewRange(const SimData *sd,
    float *xMin, float *xMax, float *yMin, float *yMax,
    float *xStep, float *yStep)
{
    float lo = 1e18f, hi = -1e18f;

    for (int m = 0; m < 4; m++) {
        if (!sd->methodSelected[m]) continue;
        const MethodData *md = &sd->methods[m];
        int n = md->n;
        if (m == 0 || m == 1) {
            for (int i = 0; i < n; i++) {
                if (md->xl[i] < lo) lo = md->xl[i];
                if (md->xu[i] > hi) hi = md->xu[i];
                if (md->xr[i] < lo) lo = md->xr[i];
                if (md->xr[i] > hi) hi = md->xr[i];
            }
        } else if (m == 2) {
            for (int i = 0; i < n; i++) {
                if (md->xi[i]    < lo) lo = md->xi[i];
                if (md->xi[i]    > hi) hi = md->xi[i];
                if (md->xr_nr[i] < lo) lo = md->xr_nr[i];
                if (md->xr_nr[i] > hi) hi = md->xr_nr[i];
            }
        } else {
            for (int i = 0; i < n; i++) {
                if (md->xm1[i]   < lo) lo = md->xm1[i];
                if (md->xm1[i]   > hi) hi = md->xm1[i];
                if (md->xi_sc[i] < lo) lo = md->xi_sc[i];
                if (md->xi_sc[i] > hi) hi = md->xi_sc[i];
                if (md->xr_sc[i] < lo) lo = md->xr_sc[i];
                if (md->xr_sc[i] > hi) hi = md->xr_sc[i];
            }
        }
    }

    float spread = hi - lo;
    if (spread < 1e-6f) spread = 1.0f;
    float pad = spread * 0.20f;
    if (pad < 0.5f) pad = 0.5f;

    *xMin = lo - pad;
    *xMax = hi + pad;

    // hitung rentang y dengan menyampling evalF di seluruh rentang x
    float ylo = 1e18f, yhi = -1e18f;
    int   samples = 200;
    for (int i = 0; i <= samples; i++) {
        float x = *xMin + (float)i / samples * (*xMax - *xMin);
        float y = evalF(x, sd->a, sd->b, sd->c, sd->d);
        if (!isfinite(y)) continue;
        if (y < ylo) ylo = y;
        if (y > yhi) yhi = y;
    }
    if (ylo > 0) ylo = 0;
    if (yhi < 0) yhi = 0;

    float ypad = (yhi - ylo) * 0.15f;
    if (ypad < 0.5f) ypad = 0.5f;
    *yMin = ylo - ypad;
    *yMax = yhi + ypad;

    // hitung jarak tick yang rapi, target sekitar 6 tick per sumbu
    float xrange = *xMax - *xMin;
    float yrange = *yMax - *yMin;
    float xraw = xrange / 6.0f;
    float yraw = yrange / 6.0f;
    float xmag = powf(10.0f, floorf(log10f(xraw)));
    float ymag = powf(10.0f, floorf(log10f(yraw)));
    float xnorm = xraw / xmag;
    float ynorm = yraw / ymag;
    *xStep = (xnorm < 1.5f ? 1.0f : xnorm < 3.5f ? 2.0f : xnorm < 7.5f ? 5.0f : 10.0f) * xmag;
    *yStep = (ynorm < 1.5f ? 1.0f : ynorm < 3.5f ? 2.0f : ynorm < 7.5f ? 5.0f : 10.0f) * ymag;
    if (*xStep < 1e-6f) *xStep = 0.1f;
    if (*yStep < 1e-6f) *yStep = 0.5f;
}

// fungsi untuk membuat string persamaan fungsi yang ditampilkan di atas grafik

static void buildEqText(const SimData *sd, char *buf, int bufsz) {
    if (sd->funcType == 2) {
        snprintf(buf, bufsz,
            "f(x) = e^(%.2fx + %.2f) + %.2fx + %.2f",
            sd->a, sd->b, sd->c, sd->d);
    } else {
        switch (sd->degree) {
            case 1:
                snprintf(buf, bufsz, "f(x) = %.2fx + %.2f", sd->a, sd->b);
                break;
            case 2:
                snprintf(buf, bufsz,
                    "f(x) = %.2fx^2 + %.2fx + %.2f", sd->a, sd->b, sd->c);
                break;
            default:
                snprintf(buf, bufsz,
                    "f(x) = %.2fx^3 + %.2fx^2 + %.2fx + %.2f",
                    sd->a, sd->b, sd->c, sd->d);
        }
    }
}

/* PROGRAM UTAMA */

int main(void) {

    // baca sim_data.txt dari finpro.exe
    SimData sd;
    if (!readSimData("sim_data.txt", &sd)) {
        fprintf(stderr, "Gagal membaca sim_data.txt — keluar.\n");
        return 1;
    }

    // isi variabel global agar evalF dan evalDF dapat berjalan dengan benar
    g_funcType = sd.funcType;
    g_degree   = sd.degree;
    g_a = sd.a; g_b = sd.b; g_c = sd.c; g_d = sd.d;

    // hitung rentang tampilan grafik secara otomatis
    float xMin, xMax, yMin, yMax, xStep, yStep;
    calcViewRange(&sd, &xMin, &xMax, &yMin, &yMax, &xStep, &yStep);

    // buat string persamaan fungsi
    char eqText[128];
    buildEqText(&sd, eqText, sizeof(eqText));

    // inisialisasi window dan font
    const int W = 800, H = 600;
    InitWindow(W, H, "Root Finding Simulation");
    SetTargetFPS(60);
    Font font = LoadFont("resources/Minecraft.ttf");

    int gX = 50, gY = 129, gW = 700, gH = 400;
    SetGraphBounds(gX, gY, gW, gH);
    Rectangle btnPrev = {292, 545, 72, 36};
    Rectangle btnNext = {436, 545, 72, 36};

    #define SX(x) (gX + (int)(((x) - xMin) / (xMax - xMin) * gW))
    #define SY(y) (gY + gH - (int)(((y) - yMin) / (yMax - yMin) * gH))

    // hitung jumlah slide: POLY mendapat slide rangkuman et% jika lebih dari satu metode, EULER tidak
    int methodCount = 0;
    for (int i = 0; i < 4; i++)
        if (sd.methodSelected[i]) methodCount++;

    int hasSummary  = (sd.funcType == 1) ? 1 : 0;
    int totalSlides = methodCount + hasSummary;
    int currentSlide = 0;

    // variabel animasi iterasi per slide
    int animStep = 0;
    float animTimer = 0.0f, animInterval = 0.75f;
    bool animPlaying = false, animDone = false;

    int methodIterCounts[4];
    for (int i = 0; i < 4; i++)
        methodIterCounts[i] = sd.methods[i].n;

    const char *methodNames[] = {
        "Bisection", "False-Position", "Newton-Raphson", "Secant"
    };
    Color methodColors[] = {
        COL_BISECTION, COL_FALSEPOS, COL_NEWTON, COL_SECANT
    };

    // peta slide ke indeks metode: 0-3 untuk metode, -1 untuk slide rangkuman
    int slideToMethod[5];
    {
        int idx = 0;
        for (int i = 0; i < 4; i++)
            if (sd.methodSelected[i])
                slideToMethod[idx++] = i;
        if (hasSummary)
            slideToMethod[idx] = -1;
    }

    int tickLen = 10;

    // loop utama rendering
    while (!WindowShouldClose()) {

        float dt = GetFrameTime();
        Vector2 mouse = GetMousePosition();
        bool nextActive = currentSlide < totalSlides - 1;
        bool prevActive = currentSlide > 0;

        // input navigasi slide
        if (nextActive && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            if (CheckCollisionPointRec(mouse, btnNext)) {
                currentSlide++;
                animStep = 0; animTimer = 0; animPlaying = false; animDone = false;
            }
        if (prevActive && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            if (CheckCollisionPointRec(mouse, btnPrev)) {
                currentSlide--;
                animStep = 0; animTimer = 0; animPlaying = false; animDone = false;
            }

        // input animasi: SPACE atau klik area grafik untuk play, R untuk reset
        if (IsKeyPressed(KEY_SPACE) && !animPlaying && !animDone)
            animPlaying = true;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
            CheckCollisionPointRec(mouse, (Rectangle){gX, gY, gW, gH}) &&
            !animPlaying && !animDone)
            animPlaying = true;
        if (IsKeyPressed(KEY_R))
            { animStep = 0; animTimer = 0; animPlaying = false; animDone = false; }

        // update langkah animasi berdasarkan timer
        if (animPlaying) {
            animTimer += dt;
            int cm = slideToMethod[currentSlide];
            int maxSteps = (cm >= 0) ? methodIterCounts[cm] : 0;
            if (animTimer >= animInterval) {
                animTimer -= animInterval;
                if (animStep < maxSteps - 1) animStep++;
                else { animPlaying = false; animDone = true; }
            }
        }

        int currentMethod = slideToMethod[currentSlide];

        BeginDrawing();
        ClearBackground(BLACK);

        // judul dan persamaan fungsi
        DrawTextMC(font, "ROOT FINDING SIMULATION", 25, 25, 28, WHITE);
        if (currentMethod >= 0)
            DrawTextMC(font, eqText, 25, 64, 18, COL_GREY);
        else
            DrawTextMC(font, "True Relative Error vs Iteration", 25, 64, 18, COL_GREY);

        // legenda metode di kanan atas, hanya metode yang dipilih ditampilkan
        int legendY[] = {25, 48, 71, 92};
        int legendSlot = 0;
        for (int i = 0; i < 4; i++) {
            if (!sd.methodSelected[i]) continue;
            Color col = (currentMethod == i || currentMethod == -1)
                        ? methodColors[i] : COL_GREY;
            int tw = MeasureTextMC(font, methodNames[i], 16);
            DrawTextMC(font, methodNames[i], W - 20 - tw, legendY[legendSlot], 16, col);
            legendSlot++;
        }

        // kotak area grafik
        DrawRectangleLines(gX, gY, gW, gH, COL_GREY);

        int xZero = iclamp(SX(0.0f), gX, gX + gW);
        int yZero = iclamp(SY(0.0f), gY, gY + gH);

        // sumbu, tick, dan kurva f(x) hanya ditampilkan di slide metode
        if (currentMethod >= 0) {
            DrawLine(gX, yZero, gX + gW, yZero, COL_GREY);
            DrawLine(xZero, gY, xZero, gY + gH, COL_GREY);

            int fxW = MeasureTextMC(font, "f(x)", 16);
            DrawTextMC(font, "f(x)", xZero - fxW / 2, gY - 24, 16, COL_GREY);
            DrawTextMC(font, "x", gX + gW + 10, yZero - 8, 16, COL_GREY);

            // tick sumbu x: berjalan dari 0 ke arah positif dan negatif
            for (float xv = 0.0f; xv <= xMax + 0.001f; xv += xStep) {
                int tx = SX(xv);
                if (tx < gX || tx > gX + gW) continue;
                DrawLine(tx, yZero - tickLen/2, tx, yZero + tickLen/2, COL_GREY);
                char buf[16]; snprintf(buf, sizeof(buf), "%.1f", xv);
                int tw = MeasureTextMC(font, buf, 12);
                DrawTextMC(font, buf, tx - tw/2, yZero + tickLen/2 + 3, 12, COL_GREY);
            }
            for (float xv = xStep; xv <= -xMin + 0.001f; xv += xStep) {
                int tx = SX(-xv);
                if (tx < gX || tx > gX + gW) continue;
                DrawLine(tx, yZero - tickLen/2, tx, yZero + tickLen/2, COL_GREY);
                char buf[16]; snprintf(buf, sizeof(buf), "%.1f", -xv);
                int tw = MeasureTextMC(font, buf, 12);
                DrawTextMC(font, buf, tx - tw/2, yZero + tickLen/2 + 3, 12, COL_GREY);
            }

            // tick sumbu y: label 0, lalu berjalan ke atas dan ke bawah
            {
                int ty = SY(0.0f);
                if (ty >= gY && ty <= gY + gH) {
                    DrawLine(xZero - tickLen/2, ty, xZero + tickLen/2, ty, COL_GREY);
                    char buf[16]; snprintf(buf, sizeof(buf), "0");
                    int tw = MeasureTextMC(font, buf, 12);
                    DrawTextMC(font, buf, xZero - tickLen/2 - tw - 4, ty - 6, 12, COL_GREY);
                }
            }
            for (float yv = yStep; yv <= yMax + 0.001f; yv += yStep) {
                int ty = SY(yv);
                if (ty < gY || ty > gY + gH) continue;
                DrawLine(xZero - tickLen/2, ty, xZero + tickLen/2, ty, COL_GREY);
                char buf[16]; snprintf(buf, sizeof(buf), "%.1f", yv);
                int tw = MeasureTextMC(font, buf, 12);
                DrawTextMC(font, buf, xZero - tickLen/2 - tw - 4, ty - 6, 12, COL_GREY);
            }
            for (float yv = yStep; yv <= -yMin + 0.001f; yv += yStep) {
                int ty = SY(-yv);
                if (ty < gY || ty > gY + gH) continue;
                DrawLine(xZero - tickLen/2, ty, xZero + tickLen/2, ty, COL_GREY);
                char buf[16]; snprintf(buf, sizeof(buf), "%.1f", -yv);
                int tw = MeasureTextMC(font, buf, 12);
                DrawTextMC(font, buf, xZero - tickLen/2 - tw - 4, ty - 6, 12, COL_GREY);
            }

            // kurva f(x)
            int samples = 400;
            for (int i = 0; i < samples - 1; i++) {
                float x1 = xMin + (float)i       / samples * (xMax - xMin);
                float x2 = xMin + (float)(i + 1) / samples * (xMax - xMin);
                float fy1 = evalF(x1, sd.a, sd.b, sd.c, sd.d);
                float fy2 = evalF(x2, sd.a, sd.b, sd.c, sd.d);
                int sx1 = SX(x1), sx2 = SX(x2), sy1 = SY(fy1), sy2 = SY(fy2);
                if (sy1 >= gY && sy1 <= gY + gH && sy2 >= gY && sy2 <= gY + gH)
                    DrawLine(sx1, sy1, sx2, sy2, WHITE);
            }
        }

        /* KONSTRUKSI GEOMETRI PER METODE */

        // bisection: visualisasi penyempitan bracket xl-xu dan titik tengah xr tiap iterasi
        if (currentMethod == 0) {
            float *bisXl = sd.methods[0].xl;
            float *bisXu = sd.methods[0].xu;
            float *bisXr = sd.methods[0].xr;
            Color col = COL_BISECTION;
            for (int s = 0; s <= animStep; s++) {
                float xl_s = bisXl[s], xu_s = bisXu[s], xr_s = bisXr[s];
                float fxr = evalF(xr_s, sd.a, sd.b, sd.c, sd.d);
                int pxl = SX(xl_s), pxu = SX(xu_s), pxr = SX(xr_s), pyr = SY(fxr);
                unsigned char alpha = (s==animStep)?255:(s==animStep-1)?140:60;
                Color cFade = {col.r, col.g, col.b, alpha};
                DrawLineG(pxl, yZero, pxu, yZero, cFade);
                DrawLineG(pxl, yZero-6, pxl, yZero+6, cFade);
                DrawLineG(pxu, yZero-6, pxu, yZero+6, cFade);
                DrawDashedVLine(pxr, yZero, pyr, cFade);
                DrawCircle(pxr, pyr, 4, cFade);
            }
            {
                float xl_s = bisXl[animStep], xu_s = bisXu[animStep], xr_s = bisXr[animStep];
                float fxr = evalF(xr_s, sd.a, sd.b, sd.c, sd.d);
                int pxr = SX(xr_s), pyr = SY(fxr);
                DrawRectangle(SX(xl_s), yZero-4, SX(xu_s)-SX(xl_s), 8,
                    (Color){col.r, col.g, col.b, 50});
                DrawCircle(pxr, pyr, 7, col);
                char buf[48]; snprintf(buf, sizeof(buf), "iter %d  xr=%.5f", animStep+1, xr_s);
                DrawTextMC(font, buf, gX+4, gY+6, 13, col);
                char lbl[24];
                snprintf(lbl, sizeof(lbl), "xl=%.3f", xl_s);
                DrawTextMC(font, lbl, SX(xl_s)-16, yZero+12, 11, col);
                snprintf(lbl, sizeof(lbl), "xu=%.3f", xu_s);
                DrawTextMC(font, lbl, SX(xu_s)-16, yZero+12, 11, col);
            }
        }

        // false-position: visualisasi tali busur dari f(xl) ke f(xu) dan persimpangannya di xr
        if (currentMethod == 1) {
            float *fpXl = sd.methods[1].xl;
            float *fpXu = sd.methods[1].xu;
            float *fpXr = sd.methods[1].xr;
            Color col = COL_FALSEPOS;
            for (int s = 0; s <= animStep; s++) {
                float xl_s = fpXl[s], xu_s = fpXu[s], xr_s = fpXr[s];
                float fxl = evalF(xl_s, sd.a, sd.b, sd.c, sd.d);
                float fxu = evalF(xu_s, sd.a, sd.b, sd.c, sd.d);
                float fxr = evalF(xr_s, sd.a, sd.b, sd.c, sd.d);
                int pxl = SX(xl_s), pxu = SX(xu_s), pxr = SX(xr_s);
                int pyl = SY(fxl), pyu = SY(fxu), pyr = SY(fxr);
                unsigned char alpha = (s==animStep)?230:(s==animStep-1)?120:50;
                Color cFade = {col.r, col.g, col.b, alpha};
                DrawLineG(pxl, pyl, pxu, pyu, cFade);
                DrawCircle(pxl, pyl, 4, cFade);
                DrawCircle(pxu, pyu, 4, cFade);
                DrawDashedVLine(pxr, yZero, pyr, cFade);
                DrawCircle(pxr, pyr, 4, cFade);
            }
            {
                float xl_s = fpXl[animStep], xu_s = fpXu[animStep], xr_s = fpXr[animStep];
                float fxl = evalF(xl_s, sd.a, sd.b, sd.c, sd.d);
                float fxu = evalF(xu_s, sd.a, sd.b, sd.c, sd.d);
                float fxr = evalF(xr_s, sd.a, sd.b, sd.c, sd.d);
                int pxl = SX(xl_s), pxu = SX(xu_s), pxr = SX(xr_s);
                int pyl = SY(fxl), pyu = SY(fxu), pyr = SY(fxr);
                DrawTriangle((Vector2){pxl, pyl}, (Vector2){pxu, pyu},
                    (Vector2){pxr, yZero}, (Color){col.r, col.g, col.b, 30});
                DrawLineGEx(pxl, pyl, pxu, pyu, 2.0f, col);
                DrawCircle(pxl, pyl, 6, col);
                DrawCircle(pxu, pyu, 6, col);
                DrawCircle(pxr, pyr, 7, col);
                char buf[48]; snprintf(buf, sizeof(buf), "iter %d  xr=%.5f", animStep+1, xr_s);
                DrawTextMC(font, buf, gX+4, gY+6, 13, col);
                char lbl[24];
                snprintf(lbl, sizeof(lbl), "xl=%.3f", xl_s);
                DrawTextMC(font, lbl, pxl-16, yZero+12, 11, col);
                snprintf(lbl, sizeof(lbl), "xu=%.3f", xu_s);
                DrawTextMC(font, lbl, pxu-16, yZero+12, 11, col);
                snprintf(lbl, sizeof(lbl), "xr=%.5f", xr_s);
                DrawTextMC(font, lbl, pxr+8, yZero-18, 11, col);
                (void)pyr;
            }
        }

        // newton-raphson: visualisasi garis tangen di xi dan perpotongannya dengan sumbu x sebagai xr
        if (currentMethod == 2) {
            float *nrXi = sd.methods[2].xi;
            float *nrXr = sd.methods[2].xr_nr;
            Color col = COL_NEWTON;
            for (int s = 0; s <= animStep; s++) {
                float xi_s = nrXi[s], xr_s = nrXr[s];
                float fxi  = evalF(xi_s, sd.a, sd.b, sd.c, sd.d);
                float dfxi = evalDF(xi_s, sd.a, sd.b, sd.c);
                float tX0  = xi_s - 0.8f, tX1 = xi_s + 0.4f;
                float tY0  = fxi + dfxi * (tX0 - xi_s);
                float tY1  = fxi + dfxi * (tX1 - xi_s);
                int piX = SX(xi_s), piY = SY(fxi), prX = SX(xr_s);
                unsigned char alpha = (s==animStep)?220:(s==animStep-1)?120:50;
                Color cFade = {col.r, col.g, col.b, alpha};
                DrawLineG(SX(tX0), SY(tY0), SX(tX1), SY(tY1), cFade);
                DrawDashedVLine(piX, piY, yZero, cFade);
                DrawDashedHLine(xZero, piX, piY, cFade);
                DrawCircle(piX, piY, 4, cFade);
                DrawCircle(prX, yZero, 4, cFade);
            }
            {
                float xi_s = nrXi[animStep], xr_s = nrXr[animStep];
                float fxi  = evalF(xi_s, sd.a, sd.b, sd.c, sd.d);
                float dfxi = evalDF(xi_s, sd.a, sd.b, sd.c);
                float tX0  = xi_s - 1.2f, tX1 = xi_s + 0.5f;
                float tY0  = fxi + dfxi * (tX0 - xi_s);
                float tY1  = fxi + dfxi * (tX1 - xi_s);
                int piX = SX(xi_s), piY = SY(fxi), prX = SX(xr_s);
                DrawLineGEx(SX(tX0), SY(tY0), SX(tX1), SY(tY1), 2.0f, col);
                char slbl[40];
                snprintf(slbl, sizeof(slbl), "Slope = f'(xi)=%.2f", dfxi);
                int sw = MeasureTextMC(font, slbl, 12);
                DrawTextMC(font, slbl,
                    iclamp(SX(tX0)+10, gX+4, gX+gW-sw-4),
                    iclamp(SY(tY0)-20, gY+4, gY+gH-20), 12, col);
                DrawDashedVLine(piX, piY, yZero, col);
                DrawDashedHLine(xZero, piX, piY, col);
                DrawCircle(piX, piY, 7, col);
                DrawCircle(prX, yZero, 6, col);
                char buf[48]; snprintf(buf, sizeof(buf), "iter %d  xr=%.5f", animStep+1, xr_s);
                DrawTextMC(font, buf, gX+4, gY+6, 13, col);
                char lbl[24];
                snprintf(lbl, sizeof(lbl), "xi=%.3f", xi_s);
                DrawTextMC(font, lbl, piX-16, yZero+14, 11, col);
                snprintf(lbl, sizeof(lbl), "xi+1=%.5f", xr_s);
                DrawTextMC(font, lbl, prX-16, yZero-18, 11, col);
            }
        }

        // secant: visualisasi tali busur dari f(xi-1) ke f(xi) dan perpotongannya dengan sumbu x sebagai xr
        if (currentMethod == 3) {
            float *scXm1 = sd.methods[3].xm1;
            float *scXi  = sd.methods[3].xi_sc;
            float *scXr  = sd.methods[3].xr_sc;
            Color col = COL_SECANT;
            for (int s = 0; s <= animStep; s++) {
                float xm1  = scXm1[s], xi_s = scXi[s], xr_s = scXr[s];
                float fxm1 = evalF(xm1,  sd.a, sd.b, sd.c, sd.d);
                float fxi  = evalF(xi_s, sd.a, sd.b, sd.c, sd.d);
                float fxr  = evalF(xr_s, sd.a, sd.b, sd.c, sd.d);
                int pm1X = SX(xm1), pm1Y = SY(fxm1);
                int piX  = SX(xi_s), piY  = SY(fxi);
                int prX  = SX(xr_s);
                unsigned char alpha = (s==animStep)?220:(s==animStep-1)?120:50;
                Color cFade = {col.r, col.g, col.b, alpha};
                DrawLineG(pm1X, pm1Y, piX, piY, cFade);
                DrawDashedVLine(pm1X, pm1Y, yZero, cFade);
                DrawDashedVLine(piX,  piY,  yZero, cFade);
                DrawCircle(pm1X, pm1Y, 4, cFade);
                DrawCircle(piX,  piY,  4, cFade);
                DrawCircle(prX,  yZero, 4, cFade);
                (void)fxr;
            }
            {
                float xm1  = scXm1[animStep], xi_s = scXi[animStep], xr_s = scXr[animStep];
                float fxm1 = evalF(xm1,  sd.a, sd.b, sd.c, sd.d);
                float fxi  = evalF(xi_s, sd.a, sd.b, sd.c, sd.d);
                int pm1X = SX(xm1), pm1Y = SY(fxm1);
                int piX  = SX(xi_s), piY  = SY(fxi);
                int prX  = SX(xr_s);
                float slope = (fxi - fxm1) / (xi_s - xm1 + 1e-12f);
                float extX0 = xm1 - 0.5f, extX1 = xi_s + 0.5f;
                float extY0 = fxm1 + slope * (extX0 - xm1);
                float extY1 = fxm1 + slope * (extX1 - xm1);
                DrawLineGEx(SX(extX0), SY(extY0), SX(extX1), SY(extY1), 2.0f, col);
                DrawDashedVLine(pm1X, pm1Y, yZero, col);
                DrawDashedVLine(piX,  piY,  yZero, col);
                DrawDashedHLine(xZero, piX, piY,   col);
                DrawDashedHLine(xZero, pm1X, pm1Y, col);
                DrawCircle(pm1X, pm1Y, 6, col);
                DrawCircle(piX,  piY,  7, col);
                DrawCircle(prX,  yZero, 6, col);
                char buf[48]; snprintf(buf, sizeof(buf), "iter %d  xr=%.5f", animStep+1, xr_s);
                DrawTextMC(font, buf, gX+4, gY+6, 13, col);
                char lbl[24];
                snprintf(lbl, sizeof(lbl), "xi-1=%.3f", xm1);
                DrawTextMC(font, lbl, pm1X-20, yZero+14, 11, col);
                snprintf(lbl, sizeof(lbl), "xi=%.3f", xi_s);
                DrawTextMC(font, lbl, piX-16, yZero+14, 11, col);
                snprintf(lbl, sizeof(lbl), "xr=%.5f", xr_s);
                DrawTextMC(font, lbl, prX+8, yZero-16, 11, col);
            }
        }

        // slide rangkuman: grafik perbandingan konvergensi et% per iterasi (khusus POLY)
        if (currentMethod == -1) {
            const float *etData[4];
            int           etCount[4];
            for (int m = 0; m < 4; m++) {
                etData[m]  = sd.methods[m].et;
                etCount[m] = sd.methods[m].n;
            }

            // cari nilai maksimum et dan jumlah iterasi terpanjang untuk skala sumbu
            int iterMax = 1;
            float etMax = 1e-6f;
            for (int m = 0; m < 4; m++) {
                if (etCount[m] > iterMax) iterMax = etCount[m];
                for (int s = 0; s < etCount[m]; s++)
                    if (etData[m][s] > etMax) etMax = etData[m][s];
            }

            // hitung batas atas sumbu y yang rapi
            float rawY  = etMax * 1.15f;
            float ymag  = powf(10.0f, floorf(log10f(rawY)));
            float ynorm = rawY / ymag;
            float etYMax = (ynorm < 1.5f ? 1.0f : ynorm < 3.5f ? 2.0f :
                            ynorm < 7.5f ? 5.0f : 10.0f) * ymag;

            #define CX(iter) (gX + (int)(((iter) - 1.0f) / (iterMax <= 1 ? 1 : iterMax - 1) * gW))
            #define CY(v)    (gY + gH - (int)((v) / etYMax * gH))

            // hitung jarak tick sumbu y yang rapi
            float yTickStep = etYMax / 5.0f;
            float ymag2  = powf(10.0f, floorf(log10f(yTickStep)));
            float ynorm2 = yTickStep / ymag2;
            yTickStep = (ynorm2 < 1.5f ? 1.0f : ynorm2 < 3.5f ? 2.0f :
                         ynorm2 < 7.5f ? 5.0f : 10.0f) * ymag2;

            // garis grid horizontal
            for (float yv = 0; yv <= etYMax + yTickStep * 0.01f; yv += yTickStep) {
                int gy = CY(yv);
                if (gy < gY || gy > gY + gH) continue;
                DrawLine(gX, gy, gX + gW, gy, COL_DARKGREY);
                char lbl[20]; snprintf(lbl, sizeof(lbl), "%.4g", yv);
                int tw = MeasureTextMC(font, lbl, 11);
                DrawTextMC(font, lbl, gX - tw - 6, gy - 6, 11, COL_GREY);
            }

            // garis grid vertikal
            for (int it = 1; it <= iterMax; it++) {
                int cx = CX((float)it);
                DrawLine(cx, gY, cx, gY + gH, COL_DARKGREY);
                char lbl[8]; snprintf(lbl, sizeof(lbl), "%d", it);
                int tw = MeasureTextMC(font, lbl, 11);
                DrawTextMC(font, lbl, cx - tw/2, gY + gH + 4, 11, COL_GREY);
            }

            DrawTextMC(font, "et (%)", gX, gY - 24, 16, COL_GREY);
            {
                const char *xLbl = "Iteration";
                int tw = MeasureTextMC(font, xLbl, 16);
                DrawTextMC(font, xLbl, gX + gW - tw, gY + gH + 16, 16, COL_GREY);
            }
            DrawLine(gX, gY, gX, gY + gH, COL_GREY);
            DrawLine(gX, gY + gH, gX + gW, gY + gH, COL_GREY);

            // plot garis et% per metode
            for (int m = 0; m < 4; m++) {
                if (!sd.methodSelected[m]) continue;
                Color col = methodColors[m];
                int n = etCount[m];
                const float *et = etData[m];
                int prevX = -1, prevY = -1;
                for (int s = 0; s < n; s++) {
                    int px = CX((float)(s + 1));
                    int py = iclamp(CY(et[s]), gY, gY + gH);
                    DrawCircle(px, py, 4, col);
                    if (prevX >= 0) DrawLineGEx(prevX, prevY, px, py, 2.0f, col);
                    prevX = px; prevY = py;
                }
            }
            DrawTextMC(font, "CONVERGENCE COMPARISON", gX + 4, gY + 6, 14, WHITE);
            #undef CX
            #undef CY
        }

        // titik root pada kurva, ditampilkan di semua slide metode
        if (currentMethod >= 0) {
            Color rootColor = methodColors[currentMethod];
            float xr = sd.slideRoots[currentMethod];
            int rxs = SX(xr), rys = SY(evalF(xr, sd.a, sd.b, sd.c, sd.d));
            DrawCircle(rxs, rys, 5, rootColor);
            DrawTextMC(font, "root", rxs + 10, rys - 18, 14, rootColor);
        }

        // hint kontrol dan pesan konvergen di bawah grafik
        if (!animPlaying && !animDone && currentMethod >= 0)
            DrawTextMC(font, "[SPACE] play  [R] reset", gX, gY + gH + 6, 13, COL_DARKGREY);
        if (animDone && currentMethod >= 0) {
            DrawTextMC(font, "[R] reset", gX, gY + gH + 6, 13, COL_DARKGREY);
            char fin[64];
            snprintf(fin, sizeof(fin), "Convergence!  xr = %.8f", sd.slideRoots[currentMethod]);
            int fw = MeasureTextMC(font, fin, 13);
            DrawTextMC(font, fin, gX + gW - fw, gY + gH + 6, 13, methodColors[currentMethod]);
        }

        // tombol navigasi PREV dan NEXT
        Color prevCol = prevActive ? COL_PURPLE : COL_GREY;
        DrawRectangleRec(btnPrev, prevCol);
        {
            int pw = MeasureTextMC(font, "PREV", 16);
            DrawTextMC(font, "PREV",
                (int)(btnPrev.x + (btnPrev.width  - pw) / 2),
                (int)(btnPrev.y + (btnPrev.height - 16) / 2), 16, WHITE);
        }
        Color nextCol = nextActive ? COL_PINK : COL_GREY;
        DrawRectangleRec(btnNext, nextCol);
        {
            int nw = MeasureTextMC(font, "NEXT", 16);
            DrawTextMC(font, "NEXT",
                (int)(btnNext.x + (btnNext.width  - nw) / 2),
                (int)(btnNext.y + (btnNext.height - 16) / 2), 16, WHITE);
        }

        // indikator nomor slide di bawah tengah
        char slideLbl[32];
        snprintf(slideLbl, sizeof(slideLbl), "%d / %d", currentSlide + 1, totalSlides);
        int slw = MeasureTextMC(font, slideLbl, 14);
        DrawTextMC(font, slideLbl, (W - slw) / 2, 549, 14, COL_GREY);

        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
    freeSimData(&sd);
    #undef SX
    #undef SY
    return 0;
}
