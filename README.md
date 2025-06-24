# Sisop-FP-2025-IT-C06

# Final Project Sistem Operasi IT

## Peraturan

1. Waktu pengerjaan dimulai hari Kamis (19 Juni 2025) setelah soal dibagikan hingga hari Rabu (25 Juni 2025) pukul 23.59 WIB.
2. Praktikan diharapkan membuat laporan penjelasan dan penyelesaian soal dalam bentuk Readme(github).
3. Format nama repository github “Sisop-FP-2025-IT-\[Kelas]\[Kelompok]” (contoh\:Sisop-FP-2025-IT-A01).
4. Setelah pengerjaan selesai, seluruh source code dan semua script bash, awk, dan file yang berisi cron job ditaruh di github masing - masing kelompok, dan link github dikumpulkan pada form yang disediakan. Pastikan github di setting ke publik.
5. Commit terakhir maksimal 10 menit setelah waktu pengerjaan berakhir. Jika melewati maka akan dinilai berdasarkan commit terakhir.
6. Jika tidak ada pengumuman perubahan soal oleh asisten, maka soal dianggap dapat diselesaikan.
7. Jika ditemukan soal yang tidak dapat diselesaikan, harap menuliskannya pada Readme beserta permasalahan yang ditemukan.
8. Praktikan tidak diperbolehkan menanyakan jawaban dari soal yang diberikan kepada asisten maupun praktikan dari kelompok lainnya.
9. Jika ditemukan indikasi kecurangan dalam bentuk apapun di pengerjaan soal final project, maka nilai dianggap 0.
10. Pengerjaan soal final project sesuai dengan modul yang telah diajarkan.

## Kelompok C06

## Anggota Kelompok

| Nama                         | NRP        |
| ---------------------------- | ---------- |
| Syifa Nurul Alfiah           | 5027241019 |
| Khumaidi Kharis Az-zacky     | 5027241049 |
| M. Hikari Reiziq Rakhmadinta | 5027241079 |
| Muhammad Farrel Shaputra     | 5027241110 |

## Deskripsi Soal 18 - Math Threading

### a. Dua variabel global hardcoded

```cpp
const int NUM_SIZE = 999999;
const int NUM_THREADS = 4;
```

Variabel global `NUM_SIZE` dan `NUM_THREADS` di-hardcode:

* `NUM_SIZE`: jumlah elemen total.
* `NUM_THREADS`: jumlah thread.

### b. NUM\_SIZE dibagi ke setiap thread

```cpp
long long base_chunk_per_thread = NUM_SIZE / NUM_THREADS;
long long remainder = NUM_SIZE % NUM_THREADS;
```

* `base_chunk_per_thread` digunakan untuk membagi elemen ke tiap thread.
* `remainder` digunakan untuk menyimpan sisa elemen jika pembagian tidak rata.

### c. Penanganan sisa pembagian

```cpp
if (i == NUM_THREADS - 1) {
    chunk_size += remainder;
}
```

* Jika ada sisa (`remainder`), elemen sisa ditambahkan ke thread terakhir.
* Ini memastikan jumlah total tetap sama dengan `NUM_SIZE`.

### d. Print setiap thread dan angka yang ditanganinya

```cpp
std::cout << "Thread " << thread_id << " (Elemen " << start_element << "-" << end_element - 1
          << ") selesai. Hasil lokal: " << local_result << std::endl;
```

* Print ID thread, range elemen, dan jumlah yang dihitung oleh thread tersebut.
* Diletakkan di dalam `cout_mutex` agar hasil print tidak bercampur.

### e. Print hasil akhir

```cpp
std::cout << "Hasil akhir penjumlahan global: " << global_sum << std::endl;
```

* Menampilkan hasil akhir dari semua penjumlahan `local_result` setiap thread.
* Hasil ini harus sama dengan `NUM_SIZE`.

## Cara Kompilasi dan Menjalankan

```bash
g++ -std=c++11 -o math_threading math_threading.cpp -pthread
./math_threading
```

## Struktur Repository

```
.
├── math_threading.cpp
├── README.md
├── assets/
│   └── Video_demo.mkv
```

## Video Demonstrasi

[Akses Video dalam Assets](./assets/Video_demo.mkv)
https://github.com/user-attachments/assets/89488b9f-5809-4926-957f-9f11f9c99896


## Daftar Pustaka

1. [https://en.cppreference.com/w/cpp/thread/thread](https://en.cppreference.com/w/cpp/thread/thread)
2. [https://cplusplus.com/reference/mutex/mutex/](https://cplusplus.com/reference/mutex/mutex/)
3. [https://www.geeksforgeeks.org/multithreading-in-cpp/](https://www.geeksforgeeks.org/multithreading-in-cpp/)
