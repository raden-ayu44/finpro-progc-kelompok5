---
title: Alur Program Kalkulator Root Finding - Polinomial & Euler

---

---
title: Alur Program Kalkulator Root Finding - Polinomial & Euler

---

# Alur Program Kalkulator Root Finding - Polinomial & Euler
**Final Project Praktikum Pemrograman C - Kelompok 5**

---

## 1. STRUKTUR DATA

### Enum
| Enum | Nilai | Fungsi |
|------|-------|--------|
| `FunctionType` | `POLYNOMIAL=1`, `EULER=2` | Jenis fungsi yang dipilih |
| `PolynomialDegree` | `LINEAR=1`, `QUADRATIC=2`, `CUBIC=3` | Derajat polinomial |
| `DecimalNumbers` | `FOUR=4`, `SIX=6`, `EIGHT=8` | Jumlah angka desimal tampilan |
| `RootMethods` | `BISECTION=1`, `FALSE_POSITION=2`, `NEWTON_RAPHSON=3`, `SECANT=4` | Metode numerik |
| `StopMode` | `ITER=1`, `EA=2`, `BOTH=3` | Mode berhenti iterasi |
| `ExitChoice` | `EXIT=0`, `CALCULATE=1` | Pilihan lanjut/keluar |

> **Kenapa enum mulai dari 1 (bukan 0)?**
> Karena input user adalah angka 1/2/3/4, sehingga cast langsung `(FunctionType)funcType` valid tanpa offset.
> `ExitChoice` pengecualian — mulai dari 0 karena `[0] Keluar` dan `[1] Lanjut` lebih intuitif.

### Union `FunctionParameter`
```
linear[2]    → {a, b}           untuk ax + b
quadratic[3] → {a, b, c}        untuk ax² + bx + c
cubic[4]     → {a, b, c, d}     untuk ax³ + bx² + cx + d
euler[4]     → {a, b, c, d}     untuk e^(ax+b) + cx + d
```
> **Kenapa union?** Hanya satu jenis fungsi yang aktif dalam satu sesi. Union menghemat memori — hanya mengalokasikan ukuran member terbesar (4 double = 32 byte), bukan semua sekaligus.

### Struct `FunctionConfig`
Menyimpan semua konfigurasi sesi kalkulasi:
- `funcType`, `degree`, `decimal` — pilihan fungsi & tampilan
- `param` — koefisien fungsi (union)
- `methodSelected[4]` — flag array, `1` = dipilih, `0` = tidak
- `xl`, `xu` — bracket untuk Bisection & False-Position
- `xi_nr` — initial guess untuk Newton-Raphson
- `xi_1`, `xi_sec` — initial guesses untuk Secant
- `xt_lin`, `xt_quad`, `xt_cube` — true root masing-masing derajat polinomial untuk perhitungan `%et`
- `exitChoice` — pilihan lanjut/keluar

> **Kenapa semua disimpan dalam satu struct?** Agar satu pointer `*cfg` bisa dioper ke semua fungsi tanpa parameter berlebihan. Reset mudah dengan `cfg = (FunctionConfig){0}`.

### Struct `StopCriteria`
```
mode      → ITER / EA / BOTH
maxIter   → batas iterasi (1-100)
stopError → batas %ea (0.01%-100.00%)
```
> Dipisah dari `FunctionConfig` karena stopping criteria bersifat independen — tidak terikat ke jenis fungsi tertentu, dan perlu dioper langsung ke fungsi metode.

### Struct `IterationResult`
Satu baris tabel iterasi:
```
iter          → nomor iterasi
xl, xu        → bracket (Bisection & False-Position)
xi_1, xi      → guesses (Secant & Newton-Raphson)
xr            → hasil estimasi akar
ea            → approximate error (%)
et            → true error relative (%) — hanya untuk polinomial
```

### Struct `MethodResult`
Hasil lengkap satu metode:
```
rows[]    → array dinamis IterationResult
count     → jumlah baris terisi
capacity  → kapasitas alokasi saat ini
root      → nilai akar final
finalEa   → nilai ea final
finalEt   → nilai et final — hanya untuk polinomial, 0 untuk Euler
```
> **Kenapa alokasi dinamis?** Jumlah iterasi tidak diketahui sebelumnya, terutama untuk mode `EA`. Dimulai dari `capacity=10`, digandakan saat penuh (`realloc`).

### Struct `SummaryEntry`
Satu baris tabel rangkuman hasil semua metode:
```
name        → nama metode ("Bisection", "False-Position", dst)
iterations  → jumlah iterasi yang dijalankan
convStatus  → 1 = Konvergen, 0 = Divergen / Tidak Dapat Disimpulkan
xr          → nilai akar final
ea          → nilai ea final
et          → nilai et final
```

### Struct `FunctionMeta`
Template fungsi yang dipakai di `inputFunctionParameter`:
```
funcName, funcFormat, funcCondition → string display
varNames[]  → nama koefisien (a, b, c, d)
numCoef     → jumlah koefisien
coef*       → pointer ke array koefisien di FunctionConfig
printFunction, printTrueRoot → function pointer
```
> **Kenapa function pointer?** Agar `inputFunctionParameter` bisa handle semua jenis fungsi dengan satu alur kode yang sama, tanpa switch-case berulang untuk print. `coef*` adalah pointer langsung ke array di dalam union `cfg->param`, sehingga nilai yang diinput langsung tersimpan ke `FunctionConfig` tanpa penyalinan.

---

## 2. KONSTANTA & THRESHOLD GLOBAL

| Konstanta / Threshold | Nilai | Fungsi |
|-----------------------|-------|--------|
| `MAX_ITER` | 100 | Batas iterasi default & hard limit untuk mode EA |
| `DEFAULT_ES` | 0.0001 | Batas minimum stopError (= 0.01%) |
| Hard limit xr | 10000.0 | Jika `\|xr\| > 10000`, paksa berhenti (divergen ekstrem) |
| Threshold turunan NR | 1e-10 (input) / 1e-12 (iterasi) | `\|f'(x)\| < threshold` dianggap nol → berhenti |
| Threshold penyebut | 1e-12 | `\|f(x0)-f(x1)\|` atau `\|f(xl)-f(xu)\| < 1e-12` dianggap nol |
| Threshold ea | 1e-12 | `\|xr\| < 1e-12` → ea tidak dihitung (cegah pembagian nilai sangat kecil) |
| Threshold asimtotik | 20.0 | `\|xr\| > 20` untuk deteksi drift asimtotik Euler |

> `DEFAULT_ES` dipakai sebagai batas bawah validasi input `%es`. User tidak boleh input `es < 0.01%` karena dengan tampilan 4 desimal minimum, nilai lebih kecil tidak akan terbaca.

---

## 3. FUNGSI INPUT PELINDUNG

### `clearInputBuffer()`
Mengosongkan sisa karakter di buffer `stdin` setelah `scanf("%lf")` berhasil membaca, mencegah karakter sisa (seperti `\n`) masuk ke `scanf` berikutnya.

**Digunakan oleh**: semua input `scanf("%lf", ...)` — input koefisien, bracket, dan initial guess.

### `scanInt(int *out)`
Membaca satu baris input menggunakan `fgets`, lalu parsing dengan `strtol`:
- Menolak input desimal (`0.1` → setelah `0` ada `.`, dianggap karakter sisa)
- Menolak huruf (`abc`), karakter campuran (`1abc`)
- Return `1` jika berhasil, `0` jika gagal
- Tidak memerlukan `clearInputBuffer()` karena `fgets` sudah membaca seluruh baris

> **Kenapa `0.1` lolos `scanf("%d")` tapi tidak lolos `scanInt`?** `scanf("%d")` berhenti parsing di `.` dan menganggap `0` sebagai hasil valid, meninggalkan `.1\n` di buffer. `strtol` melakukan hal yang sama, tapi pointer `end` akan menunjuk ke `.` — `*end != '\n' && *end != '\0'` terpenuhi → fungsi mengembalikan 0 (gagal).

**Digunakan oleh**: semua input pilihan integer — `funcType`, `degree`, `decimal`, `method`, `stop`, `maxIter`, `exitChoice`.

### `scanDouble(double *out)`
Membaca satu baris input menggunakan `fgets`, lalu parsing dengan `strtod`:
- Menolak huruf dan karakter tidak valid (`abc`, `1abc`)
- Menerima desimal valid (`1.5`, `-3.14`, `0.01`)
- Return `1` jika berhasil, `0` jika gagal

**Digunakan oleh**: input `%es` (`stopError`) saja. Input koefisien, bracket, dan initial guess tetap menggunakan `scanf("%lf")` + `clearInputBuffer()` karena input desimal di sana memang harus diterima.

> **Pola pemakaian di seluruh program:**
> - Input integer (pilihan menu, max_iter): `while (!scanInt(&var))`
> - Input double (koefisien, bracket, guess): `while (scanf("%lf", &var) != 1)` lalu `clearInputBuffer()`
> - Khusus `%es`: `while (!scanDouble(&var))` untuk konsistensi penolakan karakter tidak valid

---

## 4. ALUR PROGRAM UTAMA (`main`)

```
MULAI
│
├─ Tampilkan header program
│
└─ DO-WHILE (exitChoice == CALCULATE)
   │
   ├─ Reset: cfg = {0}, sc = {BOTH, MAX_ITER, DEFAULT_ES}, memset(summaryList)
   │   → Setiap iterasi loop mulai fresh, tidak ada data sisa dari sesi sebelumnya
   │
   ├─ [1] inputFunctionType → cfg.funcType
   │
   ├─ [2] IF POLYNOMIAL → inputPolynomialDegree → cfg.degree
   │       (Euler tidak perlu derajat)
   │
   ├─ [3] inputDecimalNumbers → cfg.decimal
   │
   ├─ [4] inputFunctionParameter → cfg.param + print fungsi + print true root
   │
   ├─ [5] inputRootMethods → cfg.methodSelected[4]
   │       (Bisection & False-Position dinonaktifkan otomatis jika Euler always-positive)
   │
   ├─ [6] inputStopMode → sc.mode, sc.maxIter, sc.stopError
   │
   ├─ [7] IF Bisection || False-Position → inputBracket → cfg.xl, cfg.xu
   │
   ├─ [8] IF Newton-Raphson → inputGuessNewtonRaphson → cfg.xi_nr
   │       (validasi f'(xi) ≠ 0 langsung di sini)
   │
   ├─ [9] IF Secant → inputGuessSecant → cfg.xi_1, cfg.xi_sec
   │
   ├─ [10] Jalankan metode yang dipilih, cetak tabel iterasi + analisis, simpan ke summaryList, free memory
   │   ├─ IF Bisection    → methodBisection     → printBisectionFalsePosition → printAnalysis
   │   ├─ IF False-Pos    → methodFalsePosition → printBisectionFalsePosition → printAnalysis
   │   ├─ IF Newton-Raph  → methodNewtonRaphson → printNewtonRaphson          → printAnalysis
   │   └─ IF Secant       → methodSecant        → printSecant                 → printAnalysis
   │
   ├─ [11] printSummaryTable → tabel rangkuman semua metode
   │        IF index > 1 → printScoringTable → tabel skoring Borda Count
   │
   └─ [12] inputExitChoice → cfg.exitChoice
```

---

## 5. INPUT & VALIDASI

### inputFunctionType
- Input: `1` (Polinomial) atau `2` (Euler)
- Validasi dua lapis: `scanInt` menolak non-integer, `do-while` menolak di luar range 1/2

### inputPolynomialDegree
- Input: `1` (Linier), `2` (Kuadratik), `3` (Kubik)
- Hanya muncul jika `funcType == POLYNOMIAL`

### inputDecimalNumbers
- Input: `4`, `6`, atau `8`
- Menentukan presisi tampilan seluruh output numerik program

### inputFunctionParameter
Alur:
1. Set `FunctionMeta` sesuai jenis & derajat fungsi menggunakan compound literal
2. Input koefisien satu per satu dengan `scanf("%lf")` + `clearInputBuffer()`
3. **Constraint koefisien:**
   - Linier: `a ≠ 0` dan `b ≠ 0` (keduanya, karena jika salah satu 0 fungsi bukan linier sejati)
   - Kuadratik: `a ≠ 0` (leading coefficient) — kondisi tampilan: `"a Tidak Boleh 0 dan Diskriminan Tidak Boleh Negatif"`
   - Kubik: `a ≠ 0` (leading coefficient)
   - Euler: `a ≠ 0` (koefisien eksponen, jika 0 fungsi bukan Euler sejati)
4. **Constraint diskriminan Kuadratik:** `b² - 4ac ≥ 0`
   - Jika `disc < 0` → 2 akar imajiner → tidak ada akar real → minta input ulang semua koefisien
   - Nilai diskriminan ditampilkan dengan presisi `decimal` yang dipilih user
5. Print representasi fungsi via `printFunction` (function pointer)
6. Print true root via `printTrueRoot` (function pointer)

### inputRootMethods
- Multi-pilih dengan toggle (`XOR ^= 1`)
- **Constraint:** minimal 1 metode dipilih (validasi saat `[0] Selesai`)
- Auto-stop setelah pilihan ke-9 (`pick > 9`) untuk mencegah input tak terbatas
- Pembatalan: input angka yang sama → toggle kembali ke 0, `pick--`
- **Edge case Euler always-positive:** jika `c == 0 && d >= 0`, fungsi `e^(ax+b) + d` selalu positif, tidak punya root
  - Bisection & False-Position ditandai `[TIDAK TERSEDIA]` di menu
  - Jika user mencoba memilih metode tersebut → ditolak dengan pesan error
  - Deteksi dilakukan di awal `inputRootMethods` sehingga berlaku di setiap pemanggilan

### inputStopMode
- **Mode ITER:** input `maxIter` (1-100) menggunakan `scanInt`
  - Constraint: `1 ≤ maxIter ≤ MAX_ITER`
- **Mode EA:** input `%es` (0.01%-100.00%) menggunakan `scanDouble`
  - Constraint bawah: `DEFAULT_ES * 100 = 0.01%`
  - Constraint atas: `100.00%`
  - Mode EA tetap punya hard limit `MAX_ITER` di `shouldContinue` sebagai safety net
- **Mode BOTH:** keduanya diinput, iterasi berhenti jika **salah satunya** terpenuhi

### inputBracket
- Input `xl` dan `xu` menggunakan `scanf("%lf")` + `clearInputBuffer()`
- Setelah tiap input, langsung evaluasi dan tampilkan `f(xl)` / `f(xu)` ke user
- **Constraint:** `f(xl) * f(xu) < 0` (Intermediate Value Theorem)
  - Jika `> 0`: kedua ujung satu sisi, tidak ada sign change, bracket tidak valid
  - Jika `= 0`: salah satu ujung adalah akar tepat (diterima, iterasi akan menemukan akar)
- Loop sampai bracket valid

### inputGuessNewtonRaphson
- Input `xi` menggunakan `scanf("%lf")` + `clearInputBuffer()`
- **Constraint:** `|f'(xi)| > 1e-10`
  - Jika turunan nol atau sangat kecil → Newton-Raphson tidak dapat menghitung langkah berikutnya (division by zero)
  - Validasi dilakukan langsung di `inputGuessNewtonRaphson` dengan `evaluateDerivative`
  - Ditampilkan nilai `f'(xi)` yang menyebabkan penolakan

### inputGuessSecant
- Input `xi-1` dan `xi` menggunakan `scanf("%lf")` + `clearInputBuffer()`
- **Constraint:** `xi-1 ≠ xi`
  - Jika sama → pembagi `f(x0) - f(x1)` berpotensi nol → division by zero

---

## 6. EVALUASI FUNGSI

### evaluateFunction
Menghitung `f(x)` berdasarkan `funcType` dan `degree`:
```
LINEAR    : a*x + b
QUADRATIC : a*x² + b*x + c
CUBIC     : a*x³ + b*x² + c*x + d
EULER     : e^(a*x + b) + c*x + d
```
**Guard overflow:** Untuk Euler, eksponen dihitung terpisah (`double exponent = a*x + b`) untuk mencegah overflow di exp. Hasil diperiksa dengan `isfinite()`. Jika overflow (misal `exp` dari nilai x sangat besar):
```c
if (!isfinite(result)) return (result > 0) ? DBL_MAX : -DBL_MAX;
```
Mengembalikan nilai finite terbesar/terkecil dari `<float.h>` agar perhitungan tidak menghasilkan `Inf` atau `NaN`.

**Digunakan oleh**: `inputBracket`, semua metode (dalam loop iterasi), dan `printAnalysis` (untuk deteksi asimtotik).

### evaluateDerivative
Menghitung `f'(x)` analitik untuk Newton-Raphson:
```
LINEAR    : a
QUADRATIC : 2a*x + b
CUBIC     : 3a*x² + 2b*x + c
EULER     : a * e^(a*x + b) + c
```

---

## 7. METODE KOMPUTASI NUMERIK

### Kondisi Berhenti (`shouldContinue`)
```
Guard paksa berhenti: !isfinite(xr) || !isfinite(ea) || |xr| > 10000

ITER : iter < maxIter
EA   : iter < MAX_ITER && ea > stopError  ← hard limit MAX_ITER sebagai safety
BOTH : iter < maxIter && ea > stopError
```
> DO-WHILE looping selama kondisi TRUE. Guard `isfinite` dan `|xr| > 10000` mencegah infinite loop akibat divergensi ekstrem atau overflow.

### Alokasi Memori (`shouldRealloc`)
- Kapasitas awal: `maxIter` (mode ITER/BOTH) atau `10` (mode EA)
- Jika `count >= capacity`: `capacity *= 2`, `realloc`
- Mode EA mulai dari 10 karena jumlah iterasi tidak diketahui di awal
- Setelah data disalin ke `summaryList`: `free(res.rows)` — mencegah memory leak dalam loop

### Penghentian Dini (ea Stabil)
Semua metode memiliki mekanisme penghentian dini jika `ea` tidak berubah selama 3 iterasi berturut-turut (dibandingkan sebagai string dengan presisi `decimal`):
```c
if (strcmp(eaStr, eaOldStr) == 0 && iter > 2) {
    eaSameCount++;
    if (eaSameCount > 2) break;
}
```
> Menangani kasus konvergensi ekstrem di mana ea sudah di bawah resolusi tampilan, sehingga tidak perlu terus iterasi tanpa perubahan yang terlihat.

### methodBisection
```
1. xr = (xl + xu) / 2
2. IF f(xl) * f(xr) < 0 → xu = xr  (akar di kiri)
   IF f(xl) * f(xr) > 0 → xl = xr  (akar di kanan)
   IF = 0 → xr adalah akar tepat
3. IF |xr| > 1e-12: ea = |xr - xrold| / |xr| * 100
4. Simpan iterasi, lanjut jika shouldContinue
```
> `xr` diinisialisasi `0` — iterasi pertama `ea = 100%` (standar numerik). `xl_save` dan `xu_save` disimpan sebelum update bracket agar nilai yang ditampilkan di tabel adalah bracket yang digunakan, bukan hasil update.

### methodFalsePosition
```
1. Guard: IF |f(xl) - f(xu)| < 1e-12 → break (cegah division by zero)
2. xr = xu - f(xu)*(xl - xu) / (f(xl) - f(xu))
3. Update bracket sama seperti Bisection
4. IF |xr| > 1e-12: ea = |xr - xrold| / |xr| * 100
5. Simpan iterasi, lanjut jika shouldContinue
```
> Lebih cepat konvergen dari Bisection karena menggunakan slope fungsi (interpolasi linear).

### methodNewtonRaphson
```
1. fx = f(x), dfx = f'(x)
2. Guard: IF |dfx| < 1e-12 → break (cegah division by zero)
3. xr = x - fx / dfx
4. IF |xr| > 1e-12: ea = |xr - xrold| / |xr| * 100
5. x = xr
6. Simpan iterasi, lanjut jika shouldContinue
```
> `fx` dan `dfx` dihitung sekali per iterasi dan digunakan langsung — menghindari double evaluation. `xi` yang disimpan ke tabel adalah nilai `x` sebelum update (= `xrold`).

### methodSecant
```
1. Guard: IF |f(x0) - f(x1)| < 1e-12 → break (cegah division by zero)
2. xr = x1 - f(x1)*(x0 - x1) / (f(x0) - f(x1))
3. Simpan xi-1 = x0, xi = x1 sebelum update
4. x0 = x1; x1 = xr
5. IF |xr| > 1e-12: ea = |xr - xrold| / |xr| * 100
6. Simpan iterasi, lanjut jika shouldContinue
```
> Perlu dua titik awal (xi-1 dan xi) sebagai pengganti turunan. Tidak memerlukan `evaluateDerivative`.

---

## 8. TRUE ROOT & TRUE ERROR

### calcTrueError
Menghitung `|xt - xr|` untuk polinomial. Mengambil `xt` sesuai derajat dari `cfg`.

### calcTrueErrorRelative
Menghitung `|xt - xr| / |xt| * 100` untuk polinomial. Hanya dipanggil jika `funcType == POLYNOMIAL`.

### printTrueRootLinear
```
xt = -b / a
```

### printTrueRootQuadratic
```
disc = b² - 4ac  (sudah dijamin ≥ 0 dari inputFunctionParameter)
x1 = (-b + √disc) / 2a
x2 = (-b - √disc) / 2a
xt = max(x1, x2)  ← akar terbesar sebagai pembanding
```

### printTrueRootCubic (Cardano's Formula)
Depressed cubic: substitusi `x = t - b/(3a)` menghasilkan `t³ + pt + q = 0`
```
p = (3ac - b²) / 3a²
q = (2b³ - 9abc + 27a²d) / 27a³
delta = (q/2)² + (p/3)³
```

**Kasus delta > 0** (1 akar real + 2 akar kompleks):
```
u = cbrt(-q/2 + √delta)   [complex]
v = cbrt(-q/2 - √delta)   [complex]
xt = Re(u + v) - b/(3a)
```
> Pakai `complex.h` — `cpow` dan `csqrt` untuk handle akar kompleks, ambil bagian real dengan `creal`.

**Kasus delta ≤ 0** (3 akar real — metode trigonometri):
```
r = 2√(-p/3)
theta = acos((3q/2p) * √(-3/p))
x_k = r * cos((theta + 2πk) / 3) - b/(3a)  untuk k = 0, 1, 2
xt = max(x1, x2, x3)
```
> Rounding manual setelah perhitungan untuk mengatasi floating point error:
> `xt = round(xt * rounding) / rounding` dimana `rounding = 10^(decimal+1)`

### printTrueRootEuler
Fungsi Euler tidak memiliki true root analitik, sehingga `%et` tidak dihitung:
```
"Fungsi Euler tidak memiliki True Root yang dapat dihitung secara analitik
 untuk mencari True Relative Error (%et)."
```
> Kolom `et (%)` tidak ditampilkan di tabel iterasi dan tabel rangkuman untuk Euler.

---

## 9. ANALISIS KONVERGENSI (`printAnalysis`)

Dipanggil setelah setiap tabel iterasi. Membandingkan nilai `ea` dari 3 iterasi terakhir:

```
count == 1 → Konvergen sangat cepat (ea memenuhi kriteria di iterasi pertama)

count >= 2:
  last   = rows[count-1]
  prev   = rows[count-2]
  prev2  = rows[count-3]  (jika count < 3: prev2 = prev)
  final_f = evaluateFunction(cfg, last.xr)

  isDrifting = (|last.xr| > 20.0 && |final_f| < 1e-4 && final_f > 0 && funcType == EULER)
  converge   = (last.ea < prev.ea) && (prev.ea < prev2.ea)
  diverge    = (last.ea > prev.ea) && (prev.ea > prev2.ea)

  isDrifting → convStatus = 0, cetak kesimpulan asimtotik (Euler always-positive)
  converge   → convStatus = 1, cetak kesimpulan konvergen
  diverge    → convStatus = 0, cetak kesimpulan divergen
  lainnya    → convStatus = 0, cetak "tidak dapat disimpulkan"
```

**isDrifting** mendeteksi kasus khusus Euler yang asimtotik: metode (Newton-Raphson/Secant) berjalan menjauhi root karena `f(x) = e^(ax+b)` selalu positif — xr membesar ke ±∞ sementara f(xr) mendekati 0 tapi tidak pernah menyentuhnya. Kondisi `|xr| > 20` dan `|f(xr)| < 1e-4` dengan `f(xr) > 0` mengidentifikasi perilaku ini.

> Menggunakan 3 titik (bukan 2) untuk menghindari false positive — satu kenaikan/penurunan ea saja tidak cukup untuk menyimpulkan tren. Jika hanya ada 2 iterasi (`prev2 = prev`), kondisi `converge` selalu false karena `prev.ea < prev2.ea` tidak terpenuhi → jatuh ke "tidak dapat disimpulkan".

`convStatus` disimpan ke `summaryList[index].convStatus` untuk digunakan di tabel rangkuman dan skoring.

---

## 10. PRINT TABEL

### columnWidthIter
Scan semua nilai `xr`, `ea`, `et`, `xl`, `xu`, `xi`, `xi_1` dari semua baris, cari string terpanjang saat diformat dengan presisi `decimal`, tambah 4 (2 padding kiri + 2 padding kanan).
> Field yang tidak terpakai oleh suatu metode bernilai `0.0` — tetap menghasilkan string valid. Lebih simpel daripada scan per-metode.

### columnWidthSumm
Serupa dengan `columnWidthIter`, tapi untuk tabel rangkuman — scan `xr`, `ea`, `et` dari semua `SummaryEntry`, dengan minimum lebar 11 (untuk kolom "Konvergen"/"Divergen").

### printCenteredStr / printCenteredVal
Centering dengan menghitung padding kiri dan kanan:
```
totalPad = colWidth - len
padLeft  = totalPad / 2
padRight = totalPad - padLeft  ← padRight ≥ padLeft jika totalPad ganjil
```
Setiap cell diakhiri karakter `│` (U+2502).

### printHLine
Border horizontal dengan Unicode box-drawing characters:
```
┌─────────────┬────────┬────────┐   (top)
├─────────────┼────────┼────────┤   (mid)
└─────────────┴────────┴────────┘   (bottom)
```
- Kolom Iterasi: fixed 11 karakter (`"  Iterasi  "`)
- Kolom nilai: dynamic `col` karakter dari `columnWidthIter`

### printBisectionFalsePosition / printNewtonRaphson / printSecant
- Kolom `et (%)` hanya ditampilkan jika `funcType == POLYNOMIAL` (`hasTrue` flag)
- Jumlah kolom (`nCols`) disesuaikan otomatis:

| Metode | Polinomial | Euler |
|--------|-----------|-------|
| Bisection / False-Position | 5 kolom (ada et) | 4 kolom |
| Newton-Raphson | 4 kolom | 3 kolom |
| Secant | 5 kolom | 4 kolom |

---

## 11. TABEL RANGKUMAN & SKORING

### printSummaryTable
Menampilkan satu baris per metode yang dipilih:
- Kolom: Metode, Iterasi, xr, ea (%), et (%) [hanya polinomial], Konvergensi
- Info stopping criteria ditampilkan di atas tabel (maxIter dan/atau %es sesuai mode)

### printScoringTable
Hanya ditampilkan jika lebih dari satu metode dipilih (`index > 1`).

Menggunakan **metode Borda Count** — sistem skoring berbasis peringkat:

| Kriteria | Skor |
|----------|------|
| Positif Konvergensi | Konvergen = 1, Divergen = 0 |
| Iterasi Terkecil | Terkecil = `count`, Terbesar = 1 |
| %ea Terkecil | Terkecil = `count`, Terbesar = 1 |
| %et Terkecil | Terkecil = `count`, Terbesar = 1 (hanya polinomial) |

> `count` adalah jumlah metode yang dipilih (2-4), sehingga skor "Terkecil" mengikuti jumlah metode yang dibandingkan, bukan hardcoded 4.

Ranking dihitung dengan: `rank = 1 + jumlah metode lain yang lebih baik dari metode ini`.

**Logika kesimpulan dengan disqualifikasi Divergen:**
```
IF metode skor tertinggi == Konvergen
  → cetak metode tersebut sebagai pemenang

IF metode skor tertinggi == Divergen
  → cari metode Konvergen dengan skor tertinggi berikutnya
  → IF ditemukan: cetak metode Konvergen tersebut, tambahkan catatan disqualifikasi
  → IF tidak ada metode Konvergen: cetak "Tidak ada metode yang berhasil konvergen"
```

> Metode Divergen tidak layak dinyatakan "terbaik" meskipun skornya lebih tinggi, karena ia gagal menemukan akar. Disqualifikasi ini menjaga kelogisan kesimpulan.

---

## 12. UNICODE BOX-DRAWING CHARACTERS

| Karakter | Unicode | Posisi |
|----------|---------|--------|
| `┌` | U+250C | Sudut kiri atas |
| `┐` | U+2510 | Sudut kanan atas |
| `└` | U+2514 | Sudut kiri bawah |
| `┘` | U+2518 | Sudut kanan bawah |
| `─` | U+2500 | Garis horizontal |
| `│` | U+2502 | Garis vertikal |
| `┬` | U+252C | T atas |
| `┴` | U+2534 | T bawah |
| `├` | U+251C | T kiri |
| `┤` | U+2524 | T kanan |
| `┼` | U+253C | Persilangan |

---

## 13. MEMORY MANAGEMENT

Setiap `MethodResult` menggunakan `malloc` untuk `rows[]`. Setelah tabel dicetak dan data disalin ke `summaryList`, memory dibebaskan:
```c
free(resBisection.rows);
free(resFalsePosition.rows);
free(resNewtonRaphson.rows);
free(resSecant.rows);
```
> Penting karena program berjalan dalam `do-while` loop — tanpa `free`, setiap sesi kalkulasi akan leak memory.

`summaryList[4]` dideklarasikan di `main` (stack, bukan heap) dan di-reset dengan `memset` di awal setiap sesi:
```c
memset(summaryList, 0, sizeof(summaryList));
```

---

## 14. RINGKASAN CONSTRAINT & EDGE CASE

| Constraint / Edge Case | Lokasi | Alasan |
|------------------------|--------|--------|
| `a ≠ 0` untuk semua fungsi | `inputFunctionParameter` | Leading coefficient 0 = bukan polinomial derajat tersebut |
| `b ≠ 0` untuk Linier | `inputFunctionParameter` | Jika b=0, fungsi jadi `ax`, bukan `ax+b` sejati |
| `disc ≥ 0` untuk Kuadratik | `inputFunctionParameter` | Akar imajiner tidak bisa dicari dengan metode bracket/iterasi |
| `f(xl)*f(xu) < 0` | `inputBracket` | IVT: sign change diperlukan untuk garansi akar di interval |
| `\|f'(xi)\| > 1e-10` untuk Newton-Raphson | `inputGuessNewtonRaphson` | Turunan nol → division by zero di iterasi pertama |
| `xi-1 ≠ xi` untuk Secant | `inputGuessSecant` | Pembagi `f(x0) - f(x1)` tidak boleh nol |
| `1 ≤ maxIter ≤ 100` | `inputStopMode` | Batas bawah: minimal 1 iterasi. Batas atas: default 100 |
| `0.01% ≤ %es ≤ 100%` | `inputStopMode` | Batas bawah: resolusi tampilan 4 desimal. Batas atas: ea tidak mungkin > 100% |
| Hard limit `MAX_ITER` di mode EA | `shouldContinue` | Safety net mencegah infinite loop jika divergen |
| `isfinite(xr) && isfinite(ea)` | `shouldContinue` | Guard terhadap `Inf`/`NaN` akibat overflow |
| `\|xr\| > 10000` | `shouldContinue` | Paksa berhenti jika divergen ke nilai ekstrem |
| `\|f(xl)-f(xu)\| < 1e-12` | `methodFalsePosition` | Guard division by zero saat fungsi hampir flat di bracket |
| `\|dfx\| < 1e-12` | `methodNewtonRaphson` | Guard division by zero saat turunan nol di tengah iterasi |
| `\|f(x0)-f(x1)\| < 1e-12` | `methodSecant` | Guard division by zero saat dua titik menghasilkan nilai fungsi sama |
| `evaluateFunction` overflow guard | `evaluateFunction` | `exp(x)` bisa overflow untuk x besar → kembalikan `DBL_MAX`/`-DBL_MAX` |
| Euler always-positive (`c=0, d≥0`) | `inputRootMethods` | `e^(ax+b) + d` selalu positif → tidak punya root → Bisection & False-Position dinonaktifkan |
| `isDrifting` asimtotik Euler | `printAnalysis` | NR/Secant menuju ±∞ karena `f(x)` selalu positif → deteksi via `\|xr\|>20 && f(xr)>0 && f(xr)<1e-4` |
| ea stabil 3 iterasi | Semua metode | Konvergensi di bawah resolusi tampilan → hentikan iterasi lebih awal |
| `pick ≤ 9` di pilihan metode | `inputRootMethods` | Maksimum 9 kali toggle (4 metode × 2 + 1 ekstra) |
| `count == 1` di analisis | `printAnalysis` | Konvergen sangat cepat di iterasi pertama → tidak bisa bandingkan tren ea |
| Disqualifikasi Divergen di skoring | `printScoringTable` | Metode Divergen tidak layak dinyatakan terbaik meskipun skor Borda lebih tinggi |

---

*Dokumen ini dibuat berdasarkan kode sumber `finpro.c` — Program Kalkulator Root Finding Polinomial & Euler, Final Project Praktikum Pemrograman C, Kelompok 5.*
