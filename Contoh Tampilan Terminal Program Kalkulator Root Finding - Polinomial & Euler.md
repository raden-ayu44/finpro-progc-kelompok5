# Contoh Tampilan Terminal Program Kalkulator Root Finding - Polinomial & Euler
**Final Project Praktikum Pemrograman C - Kelompok 5**

## HEADER
```
========================================================================================================================

                                  PROGRAM KALKULATOR ROOT FINDING - POLINOMIAL & EULER
                                         Final Project Praktikum Pemrograman C

Oleh: Kelompok 5
- Raden Ayu Athifah Qurrota'aini (2406408230)
- Annabell Della Sumantri        (2406415040)
- Keira Khairani Haqi            (2406419562)
- Dwidra Audric Farras           (2406426265)

========================================================================================================================

Selamat datang di Kalkulator Root Finding!

========================================================================================================================
```
## POLINOMIAL
```
========================================================================================================================

Pilihan Jenis Fungsi [ Masukkan Angka 1/2 ]

[1] Fungsi Polinomial  :  ax^n + bx^(n-1) + ... + d
[2] Fungsi Euler       :  e^(ax+b) + cx + d

Pilihan Anda           :  1

Anda memilih Fungsi Polinomial!

========================================================================================================================
```
### SOAL #1
```
========================================================================================================================

Pilihan Derajat Polinomial [ Masukkan Angka 1/2/3 ]

[1] Linier     :  ax + b
[2] Kuadratik  :  ax^2 + bx + c
[3] Kubik      :  ax^3 + bx^2 + cx + d

Pilihan Anda   :  2

Anda memilih Kuadrat!

========================================================================================================================

Pilihan Pengaturan Tampilan Desimal [ Masukkan Angka 4/6/8 ]

[4] 4 Angka Desimal  :  X.XXXX
[6] 6 Angka Desimal  :  X.XXXXXX
[8] 8 Angka Desimal  :  X.XXXXXXXX

Pilihan Anda         :  6

Anda memilih Tampilan 6 Angka Desimal!

========================================================================================================================

Masukkan Koefisien Fungsi Kuadratik [ ax^2 + bx + c | a dan b Tidak Boleh 0 ]

a = -0.5
b = 2.5
c = 4.5

Fungsi Kuadratik Anda  :  f(x) = -0.500000x^2 + 2.500000x + 4.500000
True Roots f(x)        :  xt_1 = -1.405125 | xt_2 = 6.405125

※   xt digunakan untuk mencari True Relative Error (%et) dari setiap Metode.
    xt yang digunakan akan otomatis disesuaikan dengan akar terdekat dari xr.

========================================================================================================================

Pilihan Metode Komputasi Numerik [ Masukkan Angka 0/1/2/3/4 ]

Anda boleh memilih lebih dari satu Metode!
※   Anda dapat membatalkan pilihan Metode dengan memasukkan angka yang sama. Semua pilihan Anda akan secara otomatis
    tersimpan setelah pemilihan ke-9 kali.

[1] Bisection        :  Membutuhkan xl (lower guess) dan xu (upper guess)
[2] False-Position   :  Membutuhkan xl (lower guess) dan xu (upper guess)
[3] Newton-Raphson   :  Membutuhkan xi (initial guess)
[4] Secant           :  Membutuhkan xi-1 (initial guess 0) dan xi (initial guess 1)
[0] Selesai

Pilihan ke-1 Anda    :  1

Anda memilih Metode Bisection!

Pilihan ke-2 Anda    :  0

Anda memilih Metode  :
1. Bisection

========================================================================================================================

Pilihan Mode Berhenti Iterasi [ Masukkan Angka 1/2/3 ]

[1] Input Max Iteration     :  Pencarian akar berhenti saat iter = max_iter
[2] Input % Stopping Error  :  Pencarian akar berhenti saat %ea < %es
[3] Input Keduanya          :  Pencarian akar berhenti saat salah satunya terpenuhi

Pilihan Anda                :  1

Anda memilih Input Max Iteration!

max_iter [ 1 - 100 ] = 3

※   Pencarian akar berhenti saat iterasi mencapai ke-3 kali.

========================================================================================================================

Masukkan Lower Guess (xl) dan Upper Guess (xu) [ Bisection & False-Position ]

※   Program akan melakukan verifikasi terlebih dahulu untuk memastikan terdapat root di interval yang diberikan!

xl = 5
f(xl) = 4.500000

xu = 10
f(xu) = -20.500000

f(5.000000) * f(10.000000) < 0 memenuhi kondisi!
Bracket: [5.000000, 10.000000] dapat digunakan!

========================================================================================================================

Tabel Iterasi Metode Bisection

┌───────────┬──────────────┬──────────────┬──────────────┬──────────────┬──────────────┐
│  Iterasi  │      xl      │      xu      │      xr      │    ea (%)    │    et (%)    │
├───────────┼──────────────┼──────────────┼──────────────┼──────────────┼──────────────┤
│     1     │   5.000000   │  10.000000   │   7.500000   │  100.000000  │  17.093736   │
├───────────┼──────────────┼──────────────┼──────────────┼──────────────┼──────────────┤
│     2     │   5.000000   │   7.500000   │   6.250000   │  20.000000   │   2.421886   │
├───────────┼──────────────┼──────────────┼──────────────┼──────────────┼──────────────┤
│     3     │   6.250000   │   7.500000   │   6.875000   │   9.090909   │   7.335925   │
└───────────┴──────────────┴──────────────┴──────────────┴──────────────┴──────────────┘

※   Analisis    :  ea terus menurun secara konsisten.
    Kesimpulan  :  xr konvergen untuk metode Bisection pada xr = 6.875000 dengan ea = 9.090909%.

========================================================================================================================

Tabel Kesimpulan Hasil Komputasi Numerik

Iterasi Maksimum : 3

┌──────────────────┬───────────────┬───────────────┬───────────────┬───────────────┬───────────────┐
│      Metode      │    Iterasi    │      xr       │    ea (%)     │    et (%)     │  Konvergensi  │
├──────────────────┼───────────────┼───────────────┼───────────────┼───────────────┼───────────────┤
│    Bisection     │       3       │   6.875000    │   9.090909    │   7.335925    │   Konvergen   │
└──────────────────┴───────────────┴───────────────┴───────────────┴───────────────┴───────────────┘

========================================================================================================================
```
### SOAL #2
```
========================================================================================================================

Pilihan Derajat Polinomial [ Masukkan Angka 1/2/3 ]

[1] Linier     :  ax + b
[2] Kuadratik  :  ax^2 + bx + c
[3] Kubik      :  ax^3 + bx^2 + cx + d

Pilihan Anda   :  3

Anda memilih Kubik!

========================================================================================================================

Pilihan Pengaturan Tampilan Desimal [ Masukkan Angka 4/6/8 ]

[4] 4 Angka Desimal  :  X.XXXX
[6] 6 Angka Desimal  :  X.XXXXXX
[8] 8 Angka Desimal  :  X.XXXXXXXX

Pilihan Anda         :  6

Anda memilih Tampilan 6 Angka Desimal!

========================================================================================================================

Masukkan Koefisien Fungsi Kubik [ ax^3 + bx^2 + cx + d | a Tidak Boleh 0 ]

a = 5
b = -5
c = 6
d = -2

Fungsi Kubik Anda            :  f(x) = 5.000000x^3 + -5.000000x^2 + 6.000000x + -2.000000
Highest Real True Root f(x)  :  xt = 1.163254
Root Pembanding True Error   :  xt = 1.163254

※   xt digunakan untuk mencari True Relative Error (%et) dari setiap Metode.

========================================================================================================================

Pilihan Metode Komputasi Numerik [ Masukkan Angka 0/1/2/3/4 ]

Anda boleh memilih lebih dari satu Metode!
※   Anda dapat membatalkan pilihan Metode dengan memasukkan angka yang sama. Semua pilihan Anda akan secara otomatis
    tersimpan setelah pemilihan ke-9 kali.

[1] Bisection        :  Membutuhkan xl (lower guess) dan xu (upper guess)
[2] False-Position   :  Membutuhkan xl (lower guess) dan xu (upper guess)
[3] Newton-Raphson   :  Membutuhkan xi (initial guess)
[4] Secant           :  Membutuhkan xi-1 (initial guess 0) dan xi (initial guess 1)
[0] Selesai

Pilihan ke-1 Anda    :  1

Anda memilih Metode Bisection!

Pilihan ke-2 Anda    :  0

Anda memilih Metode  :
1. Bisection

========================================================================================================================

Pilihan Mode Berhenti Iterasi [ Masukkan Angka 1/2/3 ]

[1] Input Max Iteration     :  Pencarian akar berhenti saat iter = max_iter
[2] Input % Stopping Error  :  Pencarian akar berhenti saat %ea < %es
[3] Input Keduanya          :  Pencarian akar berhenti saat salah satunya terpenuhi

Pilihan Anda                :  2

Anda memilih Input % Stopping Error!

%es [ 0.01% - 100.00% ] = 10

※   Pencarian akar berhenti saat approximate error (ea) jatuh di bawah 10% (0.100000).

========================================================================================================================

Masukkan Lower Guess (xl) dan Upper Guess (xu) [ Bisection & False-Position ]

※   Program akan melakukan verifikasi terlebih dahulu untuk memastikan terdapat root di interval yang diberikan!

xl = 0
f(xl) = -2.000000

xu = 1
f(xu) = 4.000000

f(0.000000) * f(1.000000) < 0 memenuhi kondisi!
Bracket: [0.000000, 1.000000] dapat digunakan!

========================================================================================================================

Tabel Iterasi Metode Bisection

┌───────────┬──────────────┬──────────────┬──────────────┬──────────────┬──────────────┐
│  Iterasi  │      xl      │      xu      │      xr      │    ea (%)    │    et (%)    │
├───────────┼──────────────┼──────────────┼──────────────┼──────────────┼──────────────┤
│     1     │   0.000000   │   1.000000   │   0.500000   │  100.000000  │  57.017115   │
├───────────┼──────────────┼──────────────┼──────────────┼──────────────┼──────────────┤
│     2     │   0.000000   │   0.500000   │   0.250000   │  100.000000  │  78.508557   │
├───────────┼──────────────┼──────────────┼──────────────┼──────────────┼──────────────┤
│     3     │   0.250000   │   0.500000   │   0.375000   │  33.333333   │  67.762836   │
├───────────┼──────────────┼──────────────┼──────────────┼──────────────┼──────────────┤
│     4     │   0.375000   │   0.500000   │   0.437500   │  14.285714   │  62.389975   │
├───────────┼──────────────┼──────────────┼──────────────┼──────────────┼──────────────┤
│     5     │   0.375000   │   0.437500   │   0.406250   │   7.692308   │  65.076406   │
└───────────┴──────────────┴──────────────┴──────────────┴──────────────┴──────────────┘

※   Analisis    :  ea terus menurun secara konsisten.
    Kesimpulan  :  xr konvergen untuk metode Bisection pada xr = 0.406250 dengan ea = 7.692308%.

========================================================================================================================

Tabel Kesimpulan Hasil Komputasi Numerik

% Stopping Error : 10%

┌──────────────────┬───────────────┬───────────────┬───────────────┬───────────────┬───────────────┐
│      Metode      │    Iterasi    │      xr       │    ea (%)     │    et (%)     │  Konvergensi  │
├──────────────────┼───────────────┼───────────────┼───────────────┼───────────────┼───────────────┤
│    Bisection     │       5       │   0.406250    │   7.692308    │   65.076406   │   Konvergen   │
└──────────────────┴───────────────┴───────────────┴───────────────┴───────────────┴───────────────┘

========================================================================================================================
```
### SOAL #3
```
========================================================================================================================

Pilihan Derajat Polinomial [ Masukkan Angka 1/2/3 ]

[1] Linier     :  ax + b
[2] Kuadratik  :  ax^2 + bx + c
[3] Kubik      :  ax^3 + bx^2 + cx + d

Pilihan Anda   :  3

Anda memilih Kubik!

========================================================================================================================

Pilihan Pengaturan Tampilan Desimal [ Masukkan Angka 4/6/8 ]

[4] 4 Angka Desimal  :  X.XXXX
[6] 6 Angka Desimal  :  X.XXXXXX
[8] 8 Angka Desimal  :  X.XXXXXXXX

Pilihan Anda         :  8

Anda memilih Tampilan 8 Angka Desimal!

========================================================================================================================

Masukkan Koefisien Fungsi Kubik [ ax^3 + bx^2 + cx + d | a Tidak Boleh 0 ]

a = -2.75
b = 18
c = -21
d = -12

Fungsi Kubik Anda  :  f(x) = -2.75000000x^3 + 18.00000000x^2 + -21.00000000x + -12.00000000
True Roots f(x)    :  xt_1 = 4.74032566 | xt_2 = -0.41468941 | xt_3 = 2.21981830

※   xt digunakan untuk mencari True Relative Error (%et) dari setiap Metode.
    xt yang digunakan akan otomatis disesuaikan dengan akar terdekat dari xr.

========================================================================================================================

Pilihan Metode Komputasi Numerik [ Masukkan Angka 0/1/2/3/4 ]

Anda boleh memilih lebih dari satu Metode!
※   Anda dapat membatalkan pilihan Metode dengan memasukkan angka yang sama. Semua pilihan Anda akan secara otomatis
    tersimpan setelah pemilihan ke-9 kali.

[1] Bisection        :  Membutuhkan xl (lower guess) dan xu (upper guess)
[2] False-Position   :  Membutuhkan xl (lower guess) dan xu (upper guess)
[3] Newton-Raphson   :  Membutuhkan xi (initial guess)
[4] Secant           :  Membutuhkan xi-1 (initial guess 0) dan xi (initial guess 1)
[0] Selesai

Pilihan ke-1 Anda    :  1

Anda memilih Metode Bisection!

Pilihan ke-2 Anda    :  2

Anda memilih Metode False-Position!

Pilihan ke-3 Anda    :  0

Anda memilih Metode  :
1. Bisection
2. False-Position

========================================================================================================================

Pilihan Mode Berhenti Iterasi [ Masukkan Angka 1/2/3 ]

[1] Input Max Iteration     :  Pencarian akar berhenti saat iter = max_iter
[2] Input % Stopping Error  :  Pencarian akar berhenti saat %ea < %es
[3] Input Keduanya          :  Pencarian akar berhenti saat salah satunya terpenuhi

Pilihan Anda                :  2

Anda memilih Input % Stopping Error!

%es [ 0.01% - 100.00% ] = 1

※   Pencarian akar berhenti saat approximate error (ea) jatuh di bawah 1% (0.01000000).

========================================================================================================================

Masukkan Lower Guess (xl) dan Upper Guess (xu) [ Bisection & False-Position ]

※   Program akan melakukan verifikasi terlebih dahulu untuk memastikan terdapat root di interval yang diberikan!

xl = -1
f(xl) = 29.75000000

xu = 0
f(xu) = -12.00000000

f(-1.00000000) * f(0.00000000) < 0 memenuhi kondisi!
Bracket: [-1.00000000, 0.00000000] dapat digunakan!

========================================================================================================================

Tabel Iterasi Metode Bisection

┌───────────┬────────────────┬────────────────┬────────────────┬────────────────┬────────────────┐
│  Iterasi  │       xl       │       xu       │       xr       │     ea (%)     │     et (%)     │
├───────────┼────────────────┼────────────────┼────────────────┼────────────────┼────────────────┤
│     1     │  -1.00000000   │   0.00000000   │  -0.50000000   │  100.00000000  │  20.57216461   │
├───────────┼────────────────┼────────────────┼────────────────┼────────────────┼────────────────┤
│     2     │  -0.50000000   │   0.00000000   │  -0.25000000   │  100.00000000  │  39.71391769   │
├───────────┼────────────────┼────────────────┼────────────────┼────────────────┼────────────────┤
│     3     │  -0.50000000   │  -0.25000000   │  -0.37500000   │  33.33333333   │   9.57087654   │
├───────────┼────────────────┼────────────────┼────────────────┼────────────────┼────────────────┤
│     4     │  -0.50000000   │  -0.37500000   │  -0.43750000   │  14.28571429   │   5.50064403   │
├───────────┼────────────────┼────────────────┼────────────────┼────────────────┼────────────────┤
│     5     │  -0.43750000   │  -0.37500000   │  -0.40625000   │   7.69230769   │   2.03511625   │
├───────────┼────────────────┼────────────────┼────────────────┼────────────────┼────────────────┤
│     6     │  -0.43750000   │  -0.40625000   │  -0.42187500   │   3.70370370   │   1.73276389   │
├───────────┼────────────────┼────────────────┼────────────────┼────────────────┼────────────────┤
│     7     │  -0.42187500   │  -0.40625000   │  -0.41406250   │   1.88679245   │   0.15117618   │
├───────────┼────────────────┼────────────────┼────────────────┼────────────────┼────────────────┤
│     8     │  -0.42187500   │  -0.41406250   │  -0.41796875   │   0.93457944   │   0.79079385   │
└───────────┴────────────────┴────────────────┴────────────────┴────────────────┴────────────────┘

※   Analisis    :  ea terus menurun secara konsisten.
    Kesimpulan  :  xr konvergen untuk metode Bisection pada xr = -0.41796875 dengan ea = 0.93457944%.

========================================================================================================================

Tabel Iterasi Metode False Position

┌───────────┬────────────────┬────────────────┬────────────────┬────────────────┬────────────────┐
│  Iterasi  │       xl       │       xu       │       xr       │     ea (%)     │     et (%)     │
├───────────┼────────────────┼────────────────┼────────────────┼────────────────┼────────────────┤
│     1     │  -1.00000000   │   0.00000000   │  -0.28742515   │  100.00000000  │  30.68905507   │
├───────────┼────────────────┼────────────────┼────────────────┼────────────────┼────────────────┤
│     2     │  -1.00000000   │  -0.28742515   │  -0.37944891   │  24.25195024   │   8.49804696   │
├───────────┼────────────────┼────────────────┼────────────────┼────────────────┼────────────────┤
│     3     │  -1.00000000   │  -0.37944891   │  -0.40523213   │   6.36257916   │   2.28057092   │
├───────────┼────────────────┼────────────────┼────────────────┼────────────────┼────────────────┤
│     4     │  -1.00000000   │  -0.40523213   │  -0.41217328   │   1.68403787   │   0.60675097   │
├───────────┼────────────────┼────────────────┼────────────────┼────────────────┼────────────────┤
│     5     │  -1.00000000   │  -0.41217328   │  -0.41402154   │   0.44641654   │   0.16105341   │
└───────────┴────────────────┴────────────────┴────────────────┴────────────────┴────────────────┘

※   Analisis    :  ea terus menurun secara konsisten.
    Kesimpulan  :  xr konvergen untuk metode False-Position pada xr = -0.41402154 dengan ea = 0.44641654%.

========================================================================================================================

Tabel Kesimpulan Hasil Komputasi Numerik

% Stopping Error : 1%

┌──────────────────┬───────────────┬───────────────┬───────────────┬───────────────┬───────────────┐
│      Metode      │    Iterasi    │      xr       │    ea (%)     │    et (%)     │  Konvergensi  │
├──────────────────┼───────────────┼───────────────┼───────────────┼───────────────┼───────────────┤
│    Bisection     │       8       │  -0.41796875  │  0.93457944   │  0.79079385   │   Konvergen   │
├──────────────────┼───────────────┼───────────────┼───────────────┼───────────────┼───────────────┤
│  False-Position  │       5       │  -0.41402154  │  0.44641654   │  0.16105341   │   Konvergen   │
└──────────────────┴───────────────┴───────────────┴───────────────┴───────────────┴───────────────┘

Tabel Skoring Metode Komputasi Numerik

※   Konvergen = 1 | Divergen = 0 | Terkecil = 2 | Terbesar = 1

┌───────────────────────┬──────────────────┬──────────────────┐
│       Kriteria        │    Bisection     │  False-Position  │
├───────────────────────┼──────────────────┼──────────────────┤
│  Positif Konvergensi  │        1         │        1         │
├───────────────────────┼──────────────────┼──────────────────┤
│   Iterasi Terkecil    │        1         │        2         │
├───────────────────────┼──────────────────┼──────────────────┤
│     %ea Terkecil      │        1         │        2         │
├───────────────────────┼──────────────────┼──────────────────┤
│     %et Terkecil      │        1         │        2         │
├───────────────────────┼──────────────────┼──────────────────┤
│         Total         │        4         │        7         │
└───────────────────────┴──────────────────┴──────────────────┘

※   Kesimpulan  :  Metode False-Position adalah metode yang memberikan hasil komputasi paling baik dan cocok untuk f(x).

========================================================================================================================
```
### SOAL #4
```
========================================================================================================================

Pilihan Derajat Polinomial [ Masukkan Angka 1/2/3 ]

[1] Linier     :  ax + b
[2] Kuadratik  :  ax^2 + bx + c
[3] Kubik      :  ax^3 + bx^2 + cx + d

Pilihan Anda   :  3

Anda memilih Kubik!

========================================================================================================================

Pilihan Pengaturan Tampilan Desimal [ Masukkan Angka 4/6/8 ]

[4] 4 Angka Desimal  :  X.XXXX
[6] 6 Angka Desimal  :  X.XXXXXX
[8] 8 Angka Desimal  :  X.XXXXXXXX

Pilihan Anda         :  8

Anda memilih Tampilan 8 Angka Desimal!

========================================================================================================================

Masukkan Koefisien Fungsi Kubik [ ax^3 + bx^2 + cx + d | a Tidak Boleh 0 ]

a = 2
b = -11.7
c = 17.7
d = -5

Fungsi Kubik Anda  :  f(x) = 2.00000000x^3 + -11.70000000x^2 + 17.70000000x + -5.00000000
True Roots f(x)    :  xt_1 = 3.56316082 | xt_2 = 0.36509824 | xt_3 = 1.92174093

※   xt digunakan untuk mencari True Relative Error (%et) dari setiap Metode.
    xt yang digunakan akan otomatis disesuaikan dengan akar terdekat dari xr.

========================================================================================================================

Pilihan Metode Komputasi Numerik [ Masukkan Angka 0/1/2/3/4 ]

Anda boleh memilih lebih dari satu Metode!
※   Anda dapat membatalkan pilihan Metode dengan memasukkan angka yang sama. Semua pilihan Anda akan secara otomatis
    tersimpan setelah pemilihan ke-9 kali.

[1] Bisection        :  Membutuhkan xl (lower guess) dan xu (upper guess)
[2] False-Position   :  Membutuhkan xl (lower guess) dan xu (upper guess)
[3] Newton-Raphson   :  Membutuhkan xi (initial guess)
[4] Secant           :  Membutuhkan xi-1 (initial guess 0) dan xi (initial guess 1)
[0] Selesai

Pilihan ke-1 Anda    :  3

Anda memilih Metode Newton-Raphson!

Pilihan ke-2 Anda    :  4

Anda memilih Metode Secant!

Pilihan ke-3 Anda    :  0

Anda memilih Metode  :
1. Newton-Raphson
2. Secant

========================================================================================================================

Pilihan Mode Berhenti Iterasi [ Masukkan Angka 1/2/3 ]

[1] Input Max Iteration     :  Pencarian akar berhenti saat iter = max_iter
[2] Input % Stopping Error  :  Pencarian akar berhenti saat %ea < %es
[3] Input Keduanya          :  Pencarian akar berhenti saat salah satunya terpenuhi

Pilihan Anda                :  1

Anda memilih Input Max Iteration!

max_iter [ 1 - 100 ] = 3

※   Pencarian akar berhenti saat iterasi mencapai ke-3 kali.

========================================================================================================================

Masukkan Initial Guess (xi) [ Newton-Raphson ]

xi = 3

========================================================================================================================

Masukkan Initial Guesses (xi-1) dan (xi) [ Secant | xi-1 dan xi Tidak Boleh Sama ]

xi-1 = 3
xi = 4

========================================================================================================================

Tabel Iterasi Newton Raphson

┌───────────┬───────────────┬───────────────┬───────────────┬───────────────┐
│  Iterasi  │      xi       │      xr       │    ea (%)     │    et (%)     │
├───────────┼───────────────┼───────────────┼───────────────┼───────────────┤
│     1     │  3.00000000   │  5.13333333   │  41.55844156  │  44.06684361  │
├───────────┼───────────────┼───────────────┼───────────────┼───────────────┤
│     2     │  5.13333333   │  4.26975006   │  20.22561661  │  19.83040526  │
├───────────┼───────────────┼───────────────┼───────────────┼───────────────┤
│     3     │  4.26975006   │  3.79293448   │  12.57115245  │  6.44859065   │
└───────────┴───────────────┴───────────────┴───────────────┴───────────────┘

※   Analisis    :  ea terus menurun secara konsisten.
    Kesimpulan  :  xr konvergen untuk metode Newton-Raphson pada xr = 3.79293448 dengan ea = 12.57115245%.

========================================================================================================================

Tabel Iterasi Metode Secant

┌───────────┬───────────────┬───────────────┬───────────────┬───────────────┬───────────────┐
│  Iterasi  │     xi-1      │      xi       │      xr       │    ea (%)     │    et (%)     │
├───────────┼───────────────┼───────────────┼───────────────┼───────────────┼───────────────┤
│     1     │  3.00000000   │  4.00000000   │  3.32653061   │  20.24539877  │  6.64101971   │
├───────────┼───────────────┼───────────────┼───────────────┼───────────────┼───────────────┤
│     2     │  4.00000000   │  3.32653061   │  3.48127271   │  4.44498636   │  2.29818747   │
├───────────┼───────────────┼───────────────┼───────────────┼───────────────┼───────────────┤
│     3     │  3.32653061   │  3.48127271   │  3.58627538   │  2.92790330   │  0.64870941   │
└───────────┴───────────────┴───────────────┴───────────────┴───────────────┴───────────────┘

※   Analisis    :  ea terus menurun secara konsisten.
    Kesimpulan  :  xr konvergen untuk metode Secant pada xr = 3.58627538 dengan ea = 2.92790330%.

========================================================================================================================

Tabel Kesimpulan Hasil Komputasi Numerik

Iterasi Maksimum : 3

┌──────────────────┬───────────────┬───────────────┬───────────────┬───────────────┬───────────────┐
│      Metode      │    Iterasi    │      xr       │    ea (%)     │    et (%)     │  Konvergensi  │
├──────────────────┼───────────────┼───────────────┼───────────────┼───────────────┼───────────────┤
│  Newton-Raphson  │       3       │  3.79293448   │  12.57115245  │  6.44859065   │   Konvergen   │
├──────────────────┼───────────────┼───────────────┼───────────────┼───────────────┼───────────────┤
│      Secant      │       3       │  3.58627538   │  2.92790330   │  0.64870941   │   Konvergen   │
└──────────────────┴───────────────┴───────────────┴───────────────┴───────────────┴───────────────┘

Tabel Skoring Metode Komputasi Numerik

※   Konvergen = 1 | Divergen = 0 | Terkecil = 2 | Terbesar = 1

┌───────────────────────┬──────────────────┬──────────────────┐
│       Kriteria        │  Newton-Raphson  │      Secant      │
├───────────────────────┼──────────────────┼──────────────────┤
│  Positif Konvergensi  │        1         │        1         │
├───────────────────────┼──────────────────┼──────────────────┤
│   Iterasi Terkecil    │        1         │        1         │
├───────────────────────┼──────────────────┼──────────────────┤
│     %ea Terkecil      │        1         │        2         │
├───────────────────────┼──────────────────┼──────────────────┤
│     %et Terkecil      │        1         │        2         │
├───────────────────────┼──────────────────┼──────────────────┤
│         Total         │        4         │        6         │
└───────────────────────┴──────────────────┴──────────────────┘

※   Kesimpulan  :  Metode Secant adalah metode yang memberikan hasil komputasi paling baik dan cocok untuk f(x).

========================================================================================================================
```
### SOAL #5
```
========================================================================================================================

Pilihan Derajat Polinomial [ Masukkan Angka 1/2/3 ]

[1] Linier     :  ax + b
[2] Kuadratik  :  ax^2 + bx + c
[3] Kubik      :  ax^3 + bx^2 + cx + d

Pilihan Anda   :  2

Anda memilih Kuadrat!

========================================================================================================================

Pilihan Pengaturan Tampilan Desimal [ Masukkan Angka 4/6/8 ]

[4] 4 Angka Desimal  :  X.XXXX
[6] 6 Angka Desimal  :  X.XXXXXX
[8] 8 Angka Desimal  :  X.XXXXXXXX

Pilihan Anda         :  8

Anda memilih Tampilan 8 Angka Desimal!

========================================================================================================================

Masukkan Koefisien Fungsi Kuadratik [ ax^2 + bx + c | a dan b Tidak Boleh 0 ]

a = -0.9
b = 1.7
c = 2.5

Fungsi Kuadratik Anda  :  f(x) = -0.90000000x^2 + 1.70000000x + 2.50000000
True Roots f(x)        :  xt_1 = -0.97121552 | xt_2 = 2.86010441

※   xt digunakan untuk mencari True Relative Error (%et) dari setiap Metode.
    xt yang digunakan akan otomatis disesuaikan dengan akar terdekat dari xr.

========================================================================================================================

Pilihan Metode Komputasi Numerik [ Masukkan Angka 0/1/2/3/4 ]

Anda boleh memilih lebih dari satu Metode!
※   Anda dapat membatalkan pilihan Metode dengan memasukkan angka yang sama. Semua pilihan Anda akan secara otomatis
    tersimpan setelah pemilihan ke-9 kali.

[1] Bisection        :  Membutuhkan xl (lower guess) dan xu (upper guess)
[2] False-Position   :  Membutuhkan xl (lower guess) dan xu (upper guess)
[3] Newton-Raphson   :  Membutuhkan xi (initial guess)
[4] Secant           :  Membutuhkan xi-1 (initial guess 0) dan xi (initial guess 1)
[0] Selesai

Pilihan ke-1 Anda    :  3

Anda memilih Metode Newton-Raphson!

Pilihan ke-2 Anda    :  0

Anda memilih Metode  :
1. Newton-Raphson

========================================================================================================================

Pilihan Mode Berhenti Iterasi [ Masukkan Angka 1/2/3 ]

[1] Input Max Iteration     :  Pencarian akar berhenti saat iter = max_iter
[2] Input % Stopping Error  :  Pencarian akar berhenti saat %ea < %es
[3] Input Keduanya          :  Pencarian akar berhenti saat salah satunya terpenuhi

Pilihan Anda                :  2

Anda memilih Input % Stopping Error!

%es [ 0.01% - 100.00% ] = 0.01

※   Pencarian akar berhenti saat approximate error (ea) jatuh di bawah 0.01% (0.00010000).

========================================================================================================================

Masukkan Initial Guess (xi) [ Newton-Raphson ]

xi = 5

========================================================================================================================

Tabel Iterasi Newton Raphson

┌───────────┬───────────────┬───────────────┬───────────────┬───────────────┐
│  Iterasi  │      xi       │      xr       │    ea (%)     │    et (%)     │
├───────────┼───────────────┼───────────────┼───────────────┼───────────────┤
│     1     │  5.00000000   │  3.42465753   │  46.00000000  │  19.73889931  │
├───────────┼───────────────┼───────────────┼───────────────┼───────────────┤
│     2     │  3.42465753   │  2.92435700   │  17.10805275  │  2.24651209   │
├───────────┼───────────────┼───────────────┼───────────────┼───────────────┤
│     3     │  2.92435700   │  2.86114698   │  2.20925459   │  0.03645217   │
├───────────┼───────────────┼───────────────┼───────────────┼───────────────┤
│     4     │  2.86114698   │  2.86010469   │  0.03644225   │  0.00000991   │
├───────────┼───────────────┼───────────────┼───────────────┼───────────────┤
│     5     │  2.86010469   │  2.86010441   │  0.00000991   │  0.00000000   │
└───────────┴───────────────┴───────────────┴───────────────┴───────────────┘

※   Analisis    :  ea terus menurun secara konsisten.
    Kesimpulan  :  xr konvergen untuk metode Newton-Raphson pada xr = 2.86010441 dengan ea = 0.00000991%.

========================================================================================================================

Tabel Kesimpulan Hasil Komputasi Numerik

% Stopping Error : 0.01%

┌──────────────────┬───────────────┬───────────────┬───────────────┬───────────────┬───────────────┐
│      Metode      │    Iterasi    │      xr       │    ea (%)     │    et (%)     │  Konvergensi  │
├──────────────────┼───────────────┼───────────────┼───────────────┼───────────────┼───────────────┤
│  Newton-Raphson  │       5       │  2.86010441   │  0.00000991   │  0.00000000   │   Konvergen   │
└──────────────────┴───────────────┴───────────────┴───────────────┴───────────────┴───────────────┘

========================================================================================================================

Apakah Anda ingin melanjutkan? [ Masukkan Angka 1/0 ]

[1] Lanjut
[0] Keluar Program

Pilihan Anda  :  0

Anda memilih Keluar Program!

Terima kasih sudah menggunakan Kalkulator Root Finding kami!

========================================================================================================================
```
### SOAL #6
```
========================================================================================================================

Pilihan Derajat Polinomial [ Masukkan Angka 1/2/3 ]

[1] Linier     :  ax + b
[2] Kuadratik  :  ax^2 + bx + c
[3] Kubik      :  ax^3 + bx^2 + cx + d

Pilihan Anda   :  3

Anda memilih Kubik!

========================================================================================================================

Pilihan Pengaturan Tampilan Desimal [ Masukkan Angka 4/6/8 ]

[4] 4 Angka Desimal  :  X.XXXX
[6] 6 Angka Desimal  :  X.XXXXXX
[8] 8 Angka Desimal  :  X.XXXXXXXX

Pilihan Anda         :  8

Anda memilih Tampilan 8 Angka Desimal!

========================================================================================================================

Masukkan Koefisien Fungsi Kubik [ ax^3 + bx^2 + cx + d | a Tidak Boleh 0 ]

a = 1
b = -6
c = 11
d = -6.1

Fungsi Kubik Anda  :  f(x) = 1.00000000x^3 + -6.00000000x^2 + 11.00000000x + -6.10000000
True Roots f(x)    :  xt_1 = 3.04668053 | xt_2 = 1.05435073 | xt_3 = 1.89896874

※   xt digunakan untuk mencari True Relative Error (%et) dari setiap Metode.
    xt yang digunakan akan otomatis disesuaikan dengan akar terdekat dari xr.

========================================================================================================================

Pilihan Metode Komputasi Numerik [ Masukkan Angka 0/1/2/3/4 ]

Anda boleh memilih lebih dari satu Metode!
※   Anda dapat membatalkan pilihan Metode dengan memasukkan angka yang sama. Semua pilihan Anda akan secara otomatis
    tersimpan setelah pemilihan ke-9 kali.

[1] Bisection        :  Membutuhkan xl (lower guess) dan xu (upper guess)
[2] False-Position   :  Membutuhkan xl (lower guess) dan xu (upper guess)
[3] Newton-Raphson   :  Membutuhkan xi (initial guess)
[4] Secant           :  Membutuhkan xi-1 (initial guess 0) dan xi (initial guess 1)
[0] Selesai

Pilihan ke-1 Anda    :  3

Anda memilih Metode Newton-Raphson!

Pilihan ke-2 Anda    :  4

Anda memilih Metode Secant!

Pilihan ke-3 Anda    :  0

Anda memilih Metode  :
1. Newton-Raphson
2. Secant

========================================================================================================================

Pilihan Mode Berhenti Iterasi [ Masukkan Angka 1/2/3 ]

[1] Input Max Iteration     :  Pencarian akar berhenti saat iter = max_iter
[2] Input % Stopping Error  :  Pencarian akar berhenti saat %ea < %es
[3] Input Keduanya          :  Pencarian akar berhenti saat salah satunya terpenuhi

Pilihan Anda                :  1

Anda memilih Input Max Iteration!

max_iter [ 1 - 100 ] = 3

※   Pencarian akar berhenti saat iterasi mencapai ke-3 kali.

========================================================================================================================

Masukkan Initial Guess (xi) [ Newton-Raphson ]

xi = 3.5

========================================================================================================================

Masukkan Initial Guesses (xi-1) dan (xi) [ Secant | xi-1 dan xi Tidak Boleh Sama ]

xi-1 = 2.5
xi = 3.5

========================================================================================================================

Tabel Iterasi Newton Raphson

┌───────────┬───────────────┬───────────────┬───────────────┬───────────────┐
│  Iterasi  │      xi       │      xr       │    ea (%)     │    et (%)     │
├───────────┼───────────────┼───────────────┼───────────────┼───────────────┤
│     1     │  3.50000000   │  3.19130435   │  9.67302452   │  4.74693078   │
├───────────┼───────────────┼───────────────┼───────────────┼───────────────┤
│     2     │  3.19130435   │  3.06869882   │  3.99535875   │  0.72269766   │
├───────────┼───────────────┼───────────────┼───────────────┼───────────────┤
│     3     │  3.06869882   │  3.04731674   │  0.70166924   │  0.02088190   │
└───────────┴───────────────┴───────────────┴───────────────┴───────────────┘

※   Analisis    :  ea terus menurun secara konsisten.
    Kesimpulan  :  xr konvergen untuk metode Newton-Raphson pada xr = 3.04731674 dengan ea = 0.70166924%.

========================================================================================================================

Tabel Iterasi Metode Secant

┌───────────┬───────────────┬───────────────┬───────────────┬───────────────┬───────────────┐
│  Iterasi  │     xi-1      │      xi       │      xr       │    ea (%)     │    et (%)     │
├───────────┼───────────────┼───────────────┼───────────────┼───────────────┼───────────────┤
│     1     │  2.50000000   │  3.50000000   │  2.71111111   │  29.09836066  │  11.01426347  │
├───────────┼───────────────┼───────────────┼───────────────┼───────────────┼───────────────┤
│     2     │  3.50000000   │  2.71111111   │  2.87109050   │  5.57207765   │  5.76332263   │
├───────────┼───────────────┼───────────────┼───────────────┼───────────────┼───────────────┤
│     3     │  2.71111111   │  2.87109050   │  3.22192345   │  10.88892866  │  5.75192954   │
└───────────┴───────────────┴───────────────┴───────────────┴───────────────┴───────────────┘

※   Analisis    :  ea tidak konsisten menurun atau menaik.
    Kesimpulan  :  Tidak dapat menyimpulkan apabila xr konvergen sangat lambat atau xr divergen untuk metode Secant.

========================================================================================================================

Tabel Kesimpulan Hasil Komputasi Numerik

Iterasi Maksimum : 3

┌──────────────────┬───────────────┬───────────────┬───────────────┬───────────────┬───────────────┐
│      Metode      │    Iterasi    │      xr       │    ea (%)     │    et (%)     │  Konvergensi  │
├──────────────────┼───────────────┼───────────────┼───────────────┼───────────────┼───────────────┤
│  Newton-Raphson  │       3       │  3.04731674   │  0.70166924   │  0.02088190   │   Konvergen   │
├──────────────────┼───────────────┼───────────────┼───────────────┼───────────────┼───────────────┤
│      Secant      │       3       │  3.22192345   │  10.88892866  │  5.75192954   │   Divergen    │
└──────────────────┴───────────────┴───────────────┴───────────────┴───────────────┴───────────────┘

Tabel Skoring Metode Komputasi Numerik

※   Konvergen = 1 | Divergen = 0 | Terkecil = 2 | Terbesar = 1

┌───────────────────────┬──────────────────┬──────────────────┐
│       Kriteria        │  Newton-Raphson  │      Secant      │
├───────────────────────┼──────────────────┼──────────────────┤
│  Positif Konvergensi  │        1         │        0         │
├───────────────────────┼──────────────────┼──────────────────┤
│   Iterasi Terkecil    │        1         │        1         │
├───────────────────────┼──────────────────┼──────────────────┤
│     %ea Terkecil      │        2         │        1         │
├───────────────────────┼──────────────────┼──────────────────┤
│     %et Terkecil      │        2         │        1         │
├───────────────────────┼──────────────────┼──────────────────┤
│         Total         │        6         │        3         │
└───────────────────────┴──────────────────┴──────────────────┘

※   Kesimpulan  :  Metode Newton-Raphson adalah metode yang memberikan hasil komputasi paling baik dan cocok untuk f(x).

========================================================================================================================
```
## EULER
```
========================================================================================================================

Pilihan Jenis Fungsi [ Masukkan Angka 1/2 ]

[1] Fungsi Polinomial  :  ax^n + bx^(n-1) + ... + d
[2] Fungsi Euler       :  e^(ax+b) + cx + d

Pilihan Anda           :  2

Anda memilih Fungsi Euler!

========================================================================================================================
```
### SOAL #1
```
========================================================================================================================

Pilihan Pengaturan Tampilan Desimal [ Masukkan Angka 4/6/8 ]

[4] 4 Angka Desimal  :  X.XXXX
[6] 6 Angka Desimal  :  X.XXXXXX
[8] 8 Angka Desimal  :  X.XXXXXXXX

Pilihan Anda         :  8

Anda memilih Tampilan 8 Angka Desimal!

========================================================================================================================

Masukkan Koefisien Fungsi Euler [ e^(ax+b) + cx + d | a Tidak Boleh 0 ]

a = -1
b = 0
c = -1
d = 0

Fungsi Euler Anda  :  f(x) = e^(-1.00000000x + 0.00000000) + -1.00000000x + 0.00000000
※   Fungsi Euler tidak memiliki True Root yang dapat dihitung secara analitik untuk mencari True Relative Error (%et).

========================================================================================================================

Pilihan Metode Komputasi Numerik [ Masukkan Angka 0/1/2/3/4 ]

Anda boleh memilih lebih dari satu Metode!
※   Anda dapat membatalkan pilihan Metode dengan memasukkan angka yang sama. Semua pilihan Anda akan secara otomatis
    tersimpan setelah pemilihan ke-9 kali.

[1] Bisection        :  Membutuhkan xl (lower guess) dan xu (upper guess)
[2] False-Position   :  Membutuhkan xl (lower guess) dan xu (upper guess)
[3] Newton-Raphson   :  Membutuhkan xi (initial guess)
[4] Secant           :  Membutuhkan xi-1 (initial guess 0) dan xi (initial guess 1)
[0] Selesai

Pilihan ke-1 Anda    :  3

Anda memilih Metode Newton-Raphson!

Pilihan ke-2 Anda    :  0

Anda memilih Metode  :
1. Newton-Raphson

========================================================================================================================

Pilihan Mode Berhenti Iterasi [ Masukkan Angka 1/2/3 ]

[1] Input Max Iteration     :  Pencarian akar berhenti saat iter = max_iter
[2] Input % Stopping Error  :  Pencarian akar berhenti saat %ea < %es
[3] Input Keduanya          :  Pencarian akar berhenti saat salah satunya terpenuhi

Pilihan Anda                :  1

Anda memilih Input Max Iteration!

max_iter [ 1 - 100 ] = 4

※   Pencarian akar berhenti saat iterasi mencapai ke-4 kali.

========================================================================================================================

Masukkan Initial Guess (xi) [ Newton-Raphson ]

xi = 0

========================================================================================================================

Tabel Iterasi Newton Raphson

┌───────────┬────────────────┬────────────────┬────────────────┐
│  Iterasi  │       xi       │       xr       │     ea (%)     │
├───────────┼────────────────┼────────────────┼────────────────┤
│     1     │   0.00000000   │   0.50000000   │  100.00000000  │
├───────────┼────────────────┼────────────────┼────────────────┤
│     2     │   0.50000000   │   0.56631100   │  11.70929098   │
├───────────┼────────────────┼────────────────┼────────────────┤
│     3     │   0.56631100   │   0.56714317   │   0.14672871   │
├───────────┼────────────────┼────────────────┼────────────────┤
│     4     │   0.56714317   │   0.56714329   │   0.00002211   │
└───────────┴────────────────┴────────────────┴────────────────┘

※   Analisis    :  ea terus menurun secara konsisten.
    Kesimpulan  :  xr konvergen untuk metode Newton-Raphson pada xr = 0.56714329 dengan ea = 0.00002211%.

========================================================================================================================

Tabel Kesimpulan Hasil Komputasi Numerik

Iterasi Maksimum : 4

┌──────────────────┬───────────────┬───────────────┬───────────────┬───────────────┐
│      Metode      │    Iterasi    │      xr       │    ea (%)     │  Konvergensi  │
├──────────────────┼───────────────┼───────────────┼───────────────┼───────────────┤
│  Newton-Raphson  │       4       │  0.56714329   │  0.00002211   │   Konvergen   │
└──────────────────┴───────────────┴───────────────┴───────────────┴───────────────┘

========================================================================================================================
```
### SOAL #2
```
========================================================================================================================

Pilihan Pengaturan Tampilan Desimal [ Masukkan Angka 4/6/8 ]

[4] 4 Angka Desimal  :  X.XXXX
[6] 6 Angka Desimal  :  X.XXXXXX
[8] 8 Angka Desimal  :  X.XXXXXXXX

Pilihan Anda         :  6

Anda memilih Tampilan 6 Angka Desimal!

========================================================================================================================

Masukkan Koefisien Fungsi Euler [ e^(ax+b) + cx + d | a Tidak Boleh 0 ]

a = -1
b = 0
c = -1
d = 0

Fungsi Euler Anda  :  f(x) = e^(-1.000000x + 0.000000) + -1.000000x + 0.000000
※   Fungsi Euler tidak memiliki True Root yang dapat dihitung secara analitik untuk mencari True Relative Error (%et).

========================================================================================================================

Pilihan Metode Komputasi Numerik [ Masukkan Angka 0/1/2/3/4 ]

Anda boleh memilih lebih dari satu Metode!
※   Anda dapat membatalkan pilihan Metode dengan memasukkan angka yang sama. Semua pilihan Anda akan secara otomatis
    tersimpan setelah pemilihan ke-9 kali.

[1] Bisection        :  Membutuhkan xl (lower guess) dan xu (upper guess)
[2] False-Position   :  Membutuhkan xl (lower guess) dan xu (upper guess)
[3] Newton-Raphson   :  Membutuhkan xi (initial guess)
[4] Secant           :  Membutuhkan xi-1 (initial guess 0) dan xi (initial guess 1)
[0] Selesai

Pilihan ke-1 Anda    :  4

Anda memilih Metode Secant!

Pilihan ke-2 Anda    :  0

Anda memilih Metode  :
1. Secant

========================================================================================================================

Pilihan Mode Berhenti Iterasi [ Masukkan Angka 1/2/3 ]

[1] Input Max Iteration     :  Pencarian akar berhenti saat iter = max_iter
[2] Input % Stopping Error  :  Pencarian akar berhenti saat %ea < %es
[3] Input Keduanya          :  Pencarian akar berhenti saat salah satunya terpenuhi

Pilihan Anda                :  1

Anda memilih Input Max Iteration!

max_iter [ 1 - 100 ] = 3

※   Pencarian akar berhenti saat iterasi mencapai ke-3 kali.

========================================================================================================================

Masukkan Initial Guesses (xi-1) dan (xi) [ Secant | xi-1 dan xi Tidak Boleh Sama ]

xi-1 = 0
xi = 1

========================================================================================================================

Tabel Iterasi Metode Secant

┌───────────┬─────────────┬─────────────┬─────────────┬─────────────┐
│  Iterasi  │    xi-1     │     xi      │     xr      │   ea (%)    │
├───────────┼─────────────┼─────────────┼─────────────┼─────────────┤
│     1     │  0.000000   │  1.000000   │  0.612700   │  63.212056  │
├───────────┼─────────────┼─────────────┼─────────────┼─────────────┤
│     2     │  1.000000   │  0.612700   │  0.563838   │  8.665860   │
├───────────┼─────────────┼─────────────┼─────────────┼─────────────┤
│     3     │  0.612700   │  0.563838   │  0.567170   │  0.587472   │
└───────────┴─────────────┴─────────────┴─────────────┴─────────────┘

※   Analisis    :  ea terus menurun secara konsisten.
    Kesimpulan  :  xr konvergen untuk metode Secant pada xr = 0.567170 dengan ea = 0.587472%.

========================================================================================================================

Tabel Kesimpulan Hasil Komputasi Numerik

Iterasi Maksimum : 3

┌──────────────────┬───────────────┬───────────────┬───────────────┬───────────────┐
│      Metode      │    Iterasi    │      xr       │    ea (%)     │  Konvergensi  │
├──────────────────┼───────────────┼───────────────┼───────────────┼───────────────┤
│      Secant      │       3       │   0.567170    │   0.587472    │   Konvergen   │
└──────────────────┴───────────────┴───────────────┴───────────────┴───────────────┘

========================================================================================================================
```
### SOAL #3
```
========================================================================================================================

Pilihan Pengaturan Tampilan Desimal [ Masukkan Angka 4/6/8 ]

[4] 4 Angka Desimal  :  X.XXXX
[6] 6 Angka Desimal  :  X.XXXXXX
[8] 8 Angka Desimal  :  X.XXXXXXXX

Pilihan Anda         :  8

Anda memilih Tampilan 8 Angka Desimal!

========================================================================================================================

Masukkan Koefisien Fungsi Euler [ e^(ax+b) + cx + d | a Tidak Boleh 0 ]

a = 0.5
b = 0
c = 5
d = -5

Fungsi Euler Anda  :  f(x) = e^(0.50000000x + 0.00000000) + 5.00000000x + -5.00000000
※   Fungsi Euler tidak memiliki True Root yang dapat dihitung secara analitik untuk mencari True Relative Error (%et).

========================================================================================================================

Pilihan Metode Komputasi Numerik [ Masukkan Angka 0/1/2/3/4 ]

Anda boleh memilih lebih dari satu Metode!
※   Anda dapat membatalkan pilihan Metode dengan memasukkan angka yang sama. Semua pilihan Anda akan secara otomatis
    tersimpan setelah pemilihan ke-9 kali.

[1] Bisection        :  Membutuhkan xl (lower guess) dan xu (upper guess)
[2] False-Position   :  Membutuhkan xl (lower guess) dan xu (upper guess)
[3] Newton-Raphson   :  Membutuhkan xi (initial guess)
[4] Secant           :  Membutuhkan xi-1 (initial guess 0) dan xi (initial guess 1)
[0] Selesai

Pilihan ke-1 Anda    :  1

Anda memilih Metode Bisection!

Pilihan ke-2 Anda    :  2

Anda memilih Metode False-Position!

Pilihan ke-3 Anda    :  3

Anda memilih Metode Newton-Raphson!

Pilihan ke-4 Anda    :  4

Anda memilih Metode Secant!

Pilihan ke-5 Anda    :  0

Anda memilih Metode  :
1. Bisection
2. False-Position
3. Newton-Raphson
4. Secant

========================================================================================================================

Pilihan Mode Berhenti Iterasi [ Masukkan Angka 1/2/3 ]

[1] Input Max Iteration     :  Pencarian akar berhenti saat iter = max_iter
[2] Input % Stopping Error  :  Pencarian akar berhenti saat %ea < %es
[3] Input Keduanya          :  Pencarian akar berhenti saat salah satunya terpenuhi

Pilihan Anda                :  2

Anda memilih Input % Stopping Error!

%es [ 0.01% - 100.00% ] = 2

※   Pencarian akar berhenti saat approximate error (ea) jatuh di bawah 2% (0.02000000).

========================================================================================================================

Masukkan Lower Guess (xl) dan Upper Guess (xu) [ Bisection & False-Position ]

※   Program akan melakukan verifikasi terlebih dahulu untuk memastikan terdapat root di interval yang diberikan!

xl = 0
f(xl) = -4.00000000

xu = 2
f(xu) = 7.71828183

f(0.00000000) * f(2.00000000) < 0 memenuhi kondisi!
Bracket: [0.00000000, 2.00000000] dapat digunakan!

========================================================================================================================

Masukkan Initial Guess (xi) [ Newton-Raphson ]

xi = 0.7

========================================================================================================================

Masukkan Initial Guesses (xi-1) dan (xi) [ Secant | xi-1 dan xi Tidak Boleh Sama ]

xi-1 = 0
xi = 2

========================================================================================================================

Tabel Iterasi Metode Bisection

┌───────────┬────────────────┬────────────────┬────────────────┬────────────────┐
│  Iterasi  │       xl       │       xu       │       xr       │     ea (%)     │
├───────────┼────────────────┼────────────────┼────────────────┼────────────────┤
│     1     │   0.00000000   │   2.00000000   │   1.00000000   │  100.00000000  │
├───────────┼────────────────┼────────────────┼────────────────┼────────────────┤
│     2     │   0.00000000   │   1.00000000   │   0.50000000   │  100.00000000  │
├───────────┼────────────────┼────────────────┼────────────────┼────────────────┤
│     3     │   0.50000000   │   1.00000000   │   0.75000000   │  33.33333333   │
├───────────┼────────────────┼────────────────┼────────────────┼────────────────┤
│     4     │   0.50000000   │   0.75000000   │   0.62500000   │  20.00000000   │
├───────────┼────────────────┼────────────────┼────────────────┼────────────────┤
│     5     │   0.62500000   │   0.75000000   │   0.68750000   │   9.09090909   │
├───────────┼────────────────┼────────────────┼────────────────┼────────────────┤
│     6     │   0.68750000   │   0.75000000   │   0.71875000   │   4.34782609   │
├───────────┼────────────────┼────────────────┼────────────────┼────────────────┤
│     7     │   0.68750000   │   0.71875000   │   0.70312500   │   2.22222222   │
├───────────┼────────────────┼────────────────┼────────────────┼────────────────┤
│     8     │   0.70312500   │   0.71875000   │   0.71093750   │   1.09890110   │
└───────────┴────────────────┴────────────────┴────────────────┴────────────────┘

※   Analisis    :  ea terus menurun secara konsisten.
    Kesimpulan  :  xr konvergen untuk metode Bisection pada xr = 0.71093750 dengan ea = 1.09890110%.

========================================================================================================================

Tabel Iterasi Metode False Position

┌───────────┬────────────────┬────────────────┬────────────────┬────────────────┐
│  Iterasi  │       xl       │       xu       │       xr       │     ea (%)     │
├───────────┼────────────────┼────────────────┼────────────────┼────────────────┤
│     1     │   0.00000000   │   2.00000000   │   0.68269394   │  100.00000000  │
├───────────┼────────────────┼────────────────┼────────────────┼────────────────┤
│     2     │   0.68269394   │   2.00000000   │   0.71266435   │   4.20540319   │
├───────────┼────────────────┼────────────────┼────────────────┼────────────────┤
│     3     │   0.71266435   │   2.00000000   │   0.71409655   │   0.20056159   │
└───────────┴────────────────┴────────────────┴────────────────┴────────────────┘

※   Analisis    :  ea terus menurun secara konsisten.
    Kesimpulan  :  xr konvergen untuk metode False-Position pada xr = 0.71409655 dengan ea = 0.20056159%.

========================================================================================================================

Tabel Iterasi Newton Raphson

┌───────────┬───────────────┬───────────────┬───────────────┐
│  Iterasi  │      xi       │      xr       │    ea (%)     │
├───────────┼───────────────┼───────────────┼───────────────┤
│     1     │  0.70000000   │  0.71417497   │  1.98480308   │
└───────────┴───────────────┴───────────────┴───────────────┘

※   Analisis    :  ea langsung memenuhi kriteria berhenti pada iterasi pertama.
    Kesimpulan  :  xr konvergen sangat cepat untuk metode Newton-Raphson pada xr = 0.71417497 dengan ea = 1.98480308%.

========================================================================================================================

Tabel Iterasi Metode Secant

┌───────────┬────────────────┬────────────────┬────────────────┬────────────────┐
│  Iterasi  │      xi-1      │       xi       │       xr       │     ea (%)     │
├───────────┼────────────────┼────────────────┼────────────────┼────────────────┤
│     1     │   0.00000000   │   2.00000000   │   0.68269394   │  192.95704571  │
├───────────┼────────────────┼────────────────┼────────────────┼────────────────┤
│     2     │   2.00000000   │   0.68269394   │   0.71266435   │   4.20540319   │
├───────────┼────────────────┼────────────────┼────────────────┼────────────────┤
│     3     │   0.68269394   │   0.71266435   │   0.71417019   │   0.21085150   │
└───────────┴────────────────┴────────────────┴────────────────┴────────────────┘

※   Analisis    :  ea terus menurun secara konsisten.
    Kesimpulan  :  xr konvergen untuk metode Secant pada xr = 0.71417019 dengan ea = 0.21085150%.

========================================================================================================================

Tabel Kesimpulan Hasil Komputasi Numerik

% Stopping Error : 2%

┌──────────────────┬───────────────┬───────────────┬───────────────┬───────────────┐
│      Metode      │    Iterasi    │      xr       │    ea (%)     │  Konvergensi  │
├──────────────────┼───────────────┼───────────────┼───────────────┼───────────────┤
│    Bisection     │       8       │  0.71093750   │  1.09890110   │   Konvergen   │
├──────────────────┼───────────────┼───────────────┼───────────────┼───────────────┤
│  False-Position  │       3       │  0.71409655   │  0.20056159   │   Konvergen   │
├──────────────────┼───────────────┼───────────────┼───────────────┼───────────────┤
│  Newton-Raphson  │       1       │  0.71417497   │  1.98480308   │   Konvergen   │
├──────────────────┼───────────────┼───────────────┼───────────────┼───────────────┤
│      Secant      │       3       │  0.71417019   │  0.21085150   │   Konvergen   │
└──────────────────┴───────────────┴───────────────┴───────────────┴───────────────┘

Tabel Skoring Metode Komputasi Numerik

※   Konvergen = 1 | Divergen = 0 | Terkecil = 4 | Terbesar = 1

┌───────────────────────┬──────────────────┬──────────────────┬──────────────────┬──────────────────┐
│       Kriteria        │    Bisection     │  False-Position  │  Newton-Raphson  │      Secant      │
├───────────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┤
│  Positif Konvergensi  │        1         │        1         │        1         │        1         │
├───────────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┤
│   Iterasi Terkecil    │        1         │        2         │        4         │        2         │
├───────────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┤
│     %ea Terkecil      │        2         │        4         │        1         │        3         │
├───────────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┤
│         Total         │        4         │        7         │        6         │        6         │
└───────────────────────┴──────────────────┴──────────────────┴──────────────────┴──────────────────┘

※   Kesimpulan  :  Metode False-Position adalah metode yang memberikan hasil komputasi paling baik dan cocok untuk f(x).

========================================================================================================================
```
## LANJUT/KELUAR
```
========================================================================================================================

Apakah Anda ingin melanjutkan? [ Masukkan Angka 1/0 ]

[1] Lanjut
[0] Keluar Program

Pilihan Anda  :  1

Hore! Anda memilih Lanjut!

========================================================================================================================
```
```
========================================================================================================================

Apakah Anda ingin melanjutkan? [ Masukkan Angka 1/0 ]

[1] Lanjut
[0] Keluar Program

Pilihan Anda  :  0

Anda memilih Keluar Program!

Terima kasih sudah menggunakan Kalkulator Root Finding kami!

========================================================================================================================
```
## ERROR INPUT HANDLING
```
========================================================================================================================

Pilihan Jenis Fungsi [ Masukkan Angka 1/2 ]

[1] Fungsi Polinomial  :  ax^n + bx^(n-1) + ... + d
[2] Fungsi Euler       :  e^(ax+b) + cx + d

Pilihan Anda           :  -1

Pilihan Anda tidak valid! Tolong masukkan angka 1/2.

Pilihan Anda           :  0

Pilihan Anda tidak valid! Tolong masukkan angka 1/2.

Pilihan Anda           :  0.1

Pilihan Anda tidak valid! Tolong masukkan angka 1/2.

Pilihan Anda           :  l

Pilihan Anda tidak valid! Tolong masukkan angka 1/2.

Pilihan Anda           :  1

Anda memilih Fungsi Polinomial!

========================================================================================================================
```
```
========================================================================================================================

Pilihan Derajat Polinomial [ Masukkan Angka 1/2/3 ]

[1] Linier     :  ax + b
[2] Kuadratik  :  ax^2 + bx + c
[3] Kubik      :  ax^3 + bx^2 + cx + d

Pilihan Anda   :  -1

Pilihan Anda tidak valid! Tolong masukkan angka 1/2/3.

Pilihan Anda   :  0

Pilihan Anda tidak valid! Tolong masukkan angka 1/2/3.

Pilihan Anda   :  0.1

Pilihan Anda tidak valid! Tolong masukkan angka 1/2/3.

Pilihan Anda   :  l

Pilihan Anda tidak valid! Tolong masukkan angka 1/2/3.

Pilihan Anda   :  1

Anda memilih Linier!

========================================================================================================================
```
```
========================================================================================================================

Pilihan Pengaturan Tampilan Desimal [ Masukkan Angka 4/6/8 ]

[4] 4 Angka Desimal  :  X.XXXX
[6] 6 Angka Desimal  :  X.XXXXXX
[8] 8 Angka Desimal  :  X.XXXXXXXX

Pilihan Anda         :  -1

Pilihan Anda tidak valid! Tolong masukkan angka 4/6/8.

Pilihan Anda         :  0

Pilihan Anda tidak valid! Tolong masukkan angka 4/6/8.

Pilihan Anda         :  0.1

Pilihan Anda tidak valid! Tolong masukkan angka 4/6/8.

Pilihan Anda         :  l

Pilihan Anda tidak valid! Tolong masukkan angka 4/6/8.

Pilihan Anda         :  4

Anda memilih Tampilan 4 Angka Desimal!

========================================================================================================================
```
```
========================================================================================================================

Masukkan Koefisien Fungsi Linier [ ax + b | a dan b Tidak Boleh 0 ]

a = 0

Input tidak valid! a tidak boleh 0.

a = l

Input tidak valid! Tolong masukkan bilangan.

a = 1
b = 0

Input tidak valid! b tidak boleh 0.

b = 1

Fungsi Linier Anda          :  f(x) = 1.0000x + 1.0000
True Root f(x)              :  xt = -1.0000
Root Pembanding True Error  :  xt = -1.0000

※   xt digunakan untuk mencari True Relative Error (%et) dari setiap Metode.

========================================================================================================================
```
```
========================================================================================================================

Masukkan Koefisien Fungsi Kuadratik [ ax^2 + bx + c | a Tidak Boleh 0 dan Diskriminan Tidak Boleh Negatif ]

a = 0

Input tidak valid! a tidak boleh 0.

a = l

Input tidak valid! Tolong masukkan bilangan.

a = 1
b = 0
c = 1

Input tidak valid! Diskriminan negatif (b^2 - 4ac = -4.0000).
Fungsi tidak memiliki akar real. Tolong input ulang koefisien.

a = 1
b = 2
c = 0

Fungsi Kuadratik Anda       :  f(x) = 1.0000x^2 + 2.0000x + 0.0000
True Roots f(x)             :  xt_x1 = 0.0000 | xt_x2 = -2.0000
Root Pembanding True Error  :  xt = 0.0000

※   xt digunakan untuk mencari True Relative Error (%et) dari setiap Metode.

========================================================================================================================
```
```
========================================================================================================================

Masukkan Koefisien Fungsi Kubik [ ax^3 + bx^2 + cx + d | a Tidak Boleh 0 ]

a = 0

Input tidak valid! a tidak boleh 0.

a = l

Input tidak valid! Tolong masukkan bilangan.

a = 1
b = 0
c = 1
d = 0

Fungsi Kubik Anda            :  f(x) = 1.0000x^3 + 0.0000x^2 + 1.0000x + 0.0000
Highest Real True Root f(x)  :  xt = 0.8660
Root Pembanding True Error   :  xt = 0.8660

※   xt digunakan untuk mencari True Relative Error (%et) dari setiap Metode.

========================================================================================================================
```
```
========================================================================================================================

Masukkan Koefisien Fungsi Euler [ e^(ax+b) + cx + d | a Tidak Boleh 0 ]

a = 0

Input tidak valid! a tidak boleh 0.

a = l

Input tidak valid! Tolong masukkan bilangan.

a = 1
b = 0
c = 0
d = 0

Fungsi Euler Anda  :  f(x) = e^(1.0000x + 0.0000) + 0.0000x + 0.0000
※   Fungsi Euler tidak memiliki True Root yang dapat dihitung secara analitik untuk mencari True Relative Error (%et).

========================================================================================================================
```
```
========================================================================================================================

Pilihan Metode Komputasi Numerik [ Masukkan Angka 0/1/2/3/4 ]

Anda boleh memilih lebih dari satu Metode!
※   Anda dapat membatalkan pilihan Metode dengan memasukkan angka yang sama. Semua pilihan Anda akan secara otomatis
    tersimpan setelah pemilihan ke-9 kali.

Peringatan: c = 0.0000 dan d = 0.0000 menyebabkan f(x) = e^(1.0000x) selalu positif.
※   Metode Bisection dan False-Position tidak dapat digunakan!

[1] Bisection        :  Membutuhkan xl (lower guess) dan xu (upper guess) [TIDAK TERSEDIA]
[2] False-Position   :  Membutuhkan xl (lower guess) dan xu (upper guess) [TIDAK TERSEDIA]
[3] Newton-Raphson   :  Membutuhkan xi (initial guess)
[4] Secant           :  Membutuhkan xi-1 (initial guess 0) dan xi (initial guess 1)
[0] Selesai

Pilihan ke-1 Anda    :  1

Metode Bisection tidak dapat digunakan! Tolong pilih Metode lain.

Pilihan ke-1 Anda    :  1

Metode Bisection tidak dapat digunakan! Tolong pilih Metode lain.

Pilihan ke-1 Anda    :  2

Metode False-Position tidak dapat digunakan! Tolong pilih Metode lain.

Pilihan ke-1 Anda    :  2

Metode False-Position tidak dapat digunakan! Tolong pilih Metode lain.

Pilihan ke-1 Anda    :  0

Anda belum membuat pilihan! Tolong masukkan angka 1/2/3/4.

Pilihan ke-1 Anda    :  3

Anda memilih Metode Newton-Raphson!

Pilihan ke-2 Anda    :  0

Anda memilih Metode  :
1. Newton-Raphson

========================================================================================================================
```
```
========================================================================================================================

Pilihan Metode Komputasi Numerik [ Masukkan Angka 0/1/2/3/4 ]

Anda boleh memilih lebih dari satu Metode!
※   Anda dapat membatalkan pilihan Metode dengan memasukkan angka yang sama. Semua pilihan Anda akan secara otomatis
    tersimpan setelah pemilihan ke-9 kali.

[1] Bisection        :  Membutuhkan xl (lower guess) dan xu (upper guess)
[2] False-Position   :  Membutuhkan xl (lower guess) dan xu (upper guess)
[3] Newton-Raphson   :  Membutuhkan xi (initial guess)
[4] Secant           :  Membutuhkan xi-1 (initial guess 0) dan xi (initial guess 1)
[0] Selesai

Pilihan ke-1 Anda    :  0

Anda belum membuat pilihan! Tolong masukkan angka 1/2/3/4.

Pilihan ke-1 Anda    :  -1

Pilihan tidak valid! Tolong masukkan angka 0/1/2/3/4.

Pilihan ke-1 Anda    :  0.1

Pilihan Anda tidak valid! Tolong masukkan angka 0/1/2/3/4.

Pilihan ke-1 Anda    :  l

Pilihan Anda tidak valid! Tolong masukkan angka 0/1/2/3/4.

Pilihan ke-1 Anda    :  1

Anda memilih Metode Bisection!

Pilihan ke-2 Anda    :  0

Anda memilih Metode  :
1. Bisection

========================================================================================================================
```
```
========================================================================================================================

Pilihan Mode Berhenti Iterasi [ Masukkan Angka 1/2/3 ]

[1] Input Max Iteration     :  Pencarian akar berhenti saat iter = max_iter
[2] Input % Stopping Error  :  Pencarian akar berhenti saat %ea < %es
[3] Input Keduanya          :  Pencarian akar berhenti saat salah satunya terpenuhi

Pilihan Anda                :  -1

Pilihan Anda tidak valid! Tolong masukkan angka 1/2/3.

Pilihan Anda                :  0

Pilihan Anda tidak valid! Tolong masukkan angka 1/2/3.

Pilihan Anda                :  0.1

Pilihan Anda tidak valid! Tolong masukkan angka 1/2/3.

Pilihan Anda                :  l

Pilihan Anda tidak valid! Tolong masukkan angka 1/2/3.

Pilihan Anda                :  3

Anda memilih Input Keduanya!

max_iter [ 1 - 100 ] = -1

Input tidak valid! max_iter harus di antara 1 - 100.

max_iter [ 1 - 100 ] = 0

Input tidak valid! max_iter harus di antara 1 - 100.

max_iter [ 1 - 100 ] = 0.1

Input tidak valid! max_iter harus di antara 1 - 100.

max_iter [ 1 - 100 ] = 101

Input tidak valid! max_iter harus di antara 1 - 100.

max_iter [ 1 - 100 ] = l

Input tidak valid! max_iter harus di antara 1 - 100.

max_iter [ 1 - 100 ] = 1

%es [ 0.01% - 100.00% ] = -1

Input tidak valid! %es harus di antara 0.01% (0.0001) - 100.00%.

%es [ 0.01% - 100.00% ] = 0

Input tidak valid! %es harus di antara 0.01% (0.0001) - 100.00%.

%es [ 0.01% - 100.00% ] = 0.0001

Input tidak valid! %es harus di antara 0.01% (0.0001) - 100.00%.

%es [ 0.01% - 100.00% ] = 101

Input tidak valid! %es harus di antara 0.01% (0.0001) - 100.00%.

%es [ 0.01% - 100.00% ] = l

Input tidak valid! %es harus di antara 0.01% (0.0001) - 100.00%.

%es [ 0.01% - 100.00% ] = 1  

※   Pencarian akar berhenti saat iterasi mencapai ke-1 kali atau saat approximate error (ea) jatuh di bawah
    1% (0.0100).

========================================================================================================================
```
```
========================================================================================================================

Masukkan Lower Guess (xl) dan Upper Guess (xu) [ Bisection & False-Position ]

※   Program akan melakukan verifikasi terlebih dahulu untuk memastikan terdapat root di interval yang diberikan!

xl = 0
f(xl) = 1.0000

xu = 1
f(xu) = 2.0000

f(0.0000) * f(1.0000) > 0 tidak memenuhi kondisi!
Bracket: [0.0000, 1.0000] tidak dapat digunakan!
Tolong masukkan kembali xl dan xu.

xl = -2
f(xl) = -1.0000

xu = 0
f(xu) = 1.0000

f(-2.0000) * f(0.0000) < 0 memenuhi kondisi!
Bracket: [-2.0000, 0.0000] dapat digunakan!

========================================================================================================================
```
```
========================================================================================================================

Apakah Anda ingin melanjutkan? [ Masukkan Angka 1/0 ]

[1] Lanjut
[0] Keluar Program

Pilihan Anda  :  -1

Pilihan Anda tidak valid! Tolong masukkan angka 1/0.

Pilihan Anda  :  l

Pilihan Anda tidak valid! Tolong masukkan angka 1/0.

Pilihan Anda  :  0.1

Pilihan Anda tidak valid! Tolong masukkan angka 1/0.

Pilihan Anda  :  0

Anda memilih Keluar Program!

Terima kasih sudah menggunakan Kalkulator Root Finding kami!

========================================================================================================================
```
## MAX TOGGLE ROOT METHOD
```
========================================================================================================================

Pilihan Metode Komputasi Numerik [ Masukkan Angka 0/1/2/3/4 ]

Anda boleh memilih lebih dari satu Metode!
※   Anda dapat membatalkan pilihan Metode dengan memasukkan angka yang sama. Semua pilihan Anda akan secara otomatis
    tersimpan setelah pemilihan ke-9 kali.

[1] Bisection        :  Membutuhkan xl (lower guess) dan xu (upper guess)
[2] False-Position   :  Membutuhkan xl (lower guess) dan xu (upper guess)
[3] Newton-Raphson   :  Membutuhkan xi (initial guess)
[4] Secant           :  Membutuhkan xi-1 (initial guess 0) dan xi (initial guess 1)
[0] Selesai

Pilihan ke-1 Anda    :  1

Anda memilih Metode Bisection!

Pilihan ke-2 Anda    :  2

Anda memilih Metode False-Position!

Pilihan ke-3 Anda    :  3

Anda memilih Metode Newton-Raphson!

Pilihan ke-4 Anda    :  4

Anda memilih Metode Secant!

Pilihan ke-5 Anda    :  1

Anda membatalkan Metode Bisection!

Pilihan ke-5 Anda    :  2

Anda membatalkan Metode False-Position!

Pilihan ke-5 Anda    :  3

Anda membatalkan Metode Newton-Raphson!

Pilihan ke-5 Anda    :  4

Anda membatalkan Metode Secant!

Pilihan ke-5 Anda    :  1

Anda memilih Metode Bisection!

Pilihan ke-6 Anda    :  2

Anda memilih Metode False-Position!

Pilihan ke-7 Anda    :  3

Anda memilih Metode Newton-Raphson!

Pilihan ke-8 Anda    :  4

Anda memilih Metode Secant!

Pilihan ke-9 Anda    :  1

Anda membatalkan Metode Bisection!

Pilihan ke-9 Anda    :  2

Anda membatalkan Metode False-Position!

Pilihan ke-9 Anda    :  3

Anda membatalkan Metode Newton-Raphson!

Pilihan ke-9 Anda    :  4

Anda membatalkan Metode Secant!

Pilihan ke-9 Anda    :  1

Anda memilih Metode Bisection!

Anda memilih Metode :
1. Bisection

========================================================================================================================
```
