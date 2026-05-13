# 🔢 Kalkulator Root Finding — Polinomial & Euler
**Final Project Praktikum Pemrograman C — Kelompok 5**

> Program kalkulator pencarian akar (root finding) berbasis terminal yang dibangun menggunakan bahasa C, mendukung fungsi Polinomial (Linier, Kuadratik, Kubik) dan fungsi Euler.

---

## 👥 Anggota Kelompok

| Nama | NPM |
|------|-----|
| Raden Ayu Athifah Qurrota'aini | 2406408230 |
| Annabell Della Sumantri | 2406415040 |
| Keira Khairani Haqi | 2406419562 |
| Dwidra Audric Farras | 2406426265 |

---

## 📋 Fitur Program

### Jenis Fungsi
| Fungsi | Format |
|--------|--------|
| Polinomial Linier | `ax + b` |
| Polinomial Kuadratik | `ax² + bx + c` |
| Polinomial Kubik | `ax³ + bx² + cx + d` |
| Euler | `e^(ax+b) + cx + d` |

### Metode Komputasi Numerik
| Metode | Input yang Dibutuhkan |
|--------|----------------------|
| Bisection | xl (lower guess) dan xu (upper guess) |
| False-Position | xl (lower guess) dan xu (upper guess) |
| Newton-Raphson | xi (initial guess) |
| Secant | xi-1 dan xi (dua initial guesses) |

> Bisa memilih lebih dari satu metode sekaligus — program akan membandingkan hasilnya!

### Mode Berhenti Iterasi
- **Max Iteration** — berhenti saat iterasi ke-N
- **% Stopping Error** — berhenti saat `%ea < %es`
- **Keduanya** — berhenti saat salah satunya terpenuhi

### Output
- Tabel iterasi per metode (`xl`, `xu`, `xr`, `ea (%)`, `et (%)`)
- Analisis konvergensi otomatis per metode
- True Root analitik untuk fungsi Polinomial (formula Cardano untuk Kubik)
- Tabel rangkuman semua metode yang dipilih
- Tabel Skoring Borda Count (jika lebih dari 1 metode dipilih)
- Kesimpulan metode terbaik dengan disqualifikasi otomatis untuk metode divergen

---

## 🚀 Cara Kompilasi & Menjalankan

### Requirements
- GCC (atau compiler C apapun yang support C99+)
- Terminal dengan dukungan Unicode (untuk tampilan tabel box-drawing)

### Kompilasi
```bash
gcc -o finpro finpro.c -lm
```

### Jalankan
```bash
./finpro
```

### Windows
```bash
gcc -o finpro.exe finpro.c -lm
finpro.exe
```

> **Tips tampilan tabel:** Pastikan terminal kamu support UTF-8. Di Windows, jalankan `chcp 65001` di Command Prompt sebelum menjalankan program agar karakter box-drawing (`┌─┐│`) tampil dengan benar.

---

## 🖥️ Contoh Tampilan Terminal

### Header Program
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
```

### Contoh: f(x) = -0.5x² + 2.5x + 4.5 — Bisection, 3 Iterasi
```
Fungsi Kuadratik Anda       :  f(x) = -0.500000x^2 + 2.500000x + 4.500000
True Roots f(x)             :  xt_x1 = -1.405125 | xt_x2 = 6.405125
Root Pembanding True Error  :  xt = 6.405125

Bracket: [5.000000, 10.000000] dapat digunakan!

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
```

### Contoh: Tabel Skoring (4 Metode)
```
Tabel Skoring Metode Komputasi Numerik

※   Konvergen = 1 | Divergen = 0 | Terkecil = 4 | Terbesar = 1

┌───────────────────────┬──────────────────┬──────────────────┬──────────────────┬──────────────────┐
│        Kriteria       │    Bisection     │  False-Position  │  Newton-Raphson  │      Secant      │
├───────────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┤
│  Positif Konvergensi  │        1         │        1         │        1         │        1         │
├───────────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┤
│    Iterasi Terkecil   │        1         │        2         │        4         │        2         │
├───────────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┤
│      %ea Terkecil     │        2         │        4         │        1         │        3         │
├───────────────────────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┤
│           Total       │        4         │        7         │        6         │        6         │
└───────────────────────┴──────────────────┴──────────────────┴──────────────────┴──────────────────┘

※   Kesimpulan  :  Metode False-Position adalah metode yang memberikan hasil komputasi paling baik dan cocok untuk f(x).
```

---

## 🛡️ Validasi Input & Edge Case

Program menangani berbagai input tidak valid secara robust:

- **Input non-numerik** (`abc`, `1.5` pada field integer) → ditolak, diminta input ulang
- **Koefisien tidak valid** (`a = 0` pada fungsi yang tidak boleh) → ditolak
- **Diskriminan negatif** (Kuadratik tanpa akar real) → ditolak, diminta input ulang
- **Bracket tidak valid** (`f(xl) * f(xu) > 0`) → ditolak, diminta input ulang
- **Turunan nol di initial guess** (Newton-Raphson) → ditolak, diminta input ulang
- **Euler always-positive** (`c=0, d≥0`) → Bisection & False-Position dinonaktifkan otomatis
- **Division by zero** di semua metode → guard `< 1e-12`, iterasi dihentikan
- **Overflow / Inf / NaN** → guard `isfinite()` dan `DBL_MAX`
- **Divergen asimtotik** (NR/Secant pada Euler tanpa root) → terdeteksi dan dilaporkan

---

## 📊 Sistem Skoring (Borda Count)

Saat lebih dari satu metode dipilih, program menampilkan tabel skoring:

| Kriteria | Skor |
|----------|------|
| Konvergensi | Konvergen = 1, Divergen = 0 |
| Iterasi Terkecil | Terkecil = N, Terbesar = 1 |
| %ea Terkecil | Terkecil = N, Terbesar = 1 |
| %et Terkecil | Terkecil = N, Terbesar = 1 *(hanya Polinomial)* |

> Metode yang divergen secara otomatis didisqualifikasi dari kesimpulan, meskipun skor Borda-nya lebih tinggi.

---

## 📁 Struktur File

```
.
├── finpro.c                               → Source code utama
├── README.md                              → File ini
├── Alur_Program_...md                     → Dokumentasi teknis lengkap
└── Contoh_Tampilan_Terminal_...md         → Contoh output terminal lengkap
```

---

## 📖 Dokumentasi Teknis

Lihat **`Alur_Program_Kalkulator_Root_Finding_-_Polinomial___Euler.md`** untuk dokumentasi lengkap yang mencakup:
- Penjelasan semua struct, enum, union, dan function pointer
- Alur lengkap program utama
- Penjelasan setiap fungsi input, validasi, dan edge case
- Dokumentasi metode numerik dan formula True Root (termasuk Cardano)
- Penjelasan sistem skoring Borda Count dan logika disqualifikasi
- Tabel ringkasan semua constraint dan edge case yang ditangani

Lihat **`Contoh_Tampilan_Terminal_...md`** untuk melihat contoh output lengkap program untuk berbagai kasus uji.

---

*Dibuat untuk Final Project Praktikum Pemrograman C — Fakultas Teknik Universitas Indonesia*
