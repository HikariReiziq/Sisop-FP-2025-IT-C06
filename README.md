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

### Konsep Dasar Pembagian Kerja (Partitioning)

Salah satu langkah pertama dalam merancang program paralel adalah memecah masalah menjadi "potongan" pekerjaan diskrit yang dapat didistribusikan ke beberapa tugas. Ini dikenal sebagai dekomposisi atau partisi. Ada dua cara dasar untuk membagi pekerjaan komputasi di antara tugas paralel: **dekomposisi domain** dan **dekomposisi fungsional**.

- **Dekomposisi Domain (Domain Decomposition)**: Dalam jenis partisi ini, data yang terkait dengan masalah terurai. Setiap tugas paralel kemudian bekerja pada sebagian data. Contohnya adalah memproses array besar, di mana setiap thread diberi sebagian dari array tersebut untuk dihitung.

![img1](./assets/domain_decomp.jpg)

- **Dekomposisi Fungsional (Functional Decomposition)**: Dalam pendekatan ini, fokusnya adalah pada komputasi yang akan dilakukan daripada pada data yang dimanipulasi oleh komputasi. Masalahnya terurai sesuai dengan pekerjaan yang harus dilakukan. Setiap tugas kemudian melakukan sebagian dari keseluruhan pekerjaan. Misalnya, satu thread menangani I/O, sementara thread lain melakukan komputasi.

![img2](./assets/functional_decomp.jpg)

Kode yang kami berikan sebelumnya adalah contoh jelas dari **Dekomposisi Domain**, di mana domain angka dari `0` hingga `NUM_SIZE - 1` dibagi menjadi beberapa sub-rentang untuk setiap thread.

### Alokasi Tugas (Load Balancing)

*Load Balancing* mengacu pada praktik mendistribusikan jumlah pekerjaan yang kira-kira sama di antara tugas-tugas sehingga semua tugas tetap sibuk sepanjang waktu. Ini dapat dianggap sebagai minimalisasi waktu idle tugas. *Load Balancing* penting untuk program paralel karena alasan kinerja. Misalnya, jika semua tugas tunduk pada titik sinkronisasi penghalang, tugas paling lambat akan menentukan performa secara keseluruhan.

**Cara Mencapai *Load Balancing***

Untuk operasi array/matriks di mana setiap tugas melakukan pekerjaan yang sama, distribusikan kumpulan data secara merata di antara tugas. Untuk iterasi perulangan di mana pekerjaan yang dilakukan dalam setiap iterasi serupa, distribusikan iterasi secara merata di seluruh tugas.

- **Alokasi Statis (Static Load Balancing)**: Beban kerja didistribusikan di antara thread **sebelum** eksekusi dimulai. Beban untuk setiap thread bersifat tetap. Metode ini efisien jika tugas-tugasnya seragam dan dapat diprediksi, seperti dalam kasus Anda.C++
    
    **Solusi Kode (Contoh Alokasi Statis dari Kode ):**
    
    ```cpp
    // Bagian Kunci dari Alokasi Statis
    // Beban kerja dihitung di awal sebelum thread melakukan tugas utama.
    long long base_chunk_per_thread = NUM_SIZE / NUM_THREADS;
    long long remainder = NUM_SIZE % NUM_THREADS;
    long long current_start = 0;
    
    for (int i = 0; i < NUM_THREADS; ++i) {
        long long chunk_size = base_chunk_per_thread;
        if (i == NUM_THREADS - 1) {
            chunk_size += remainder;
        }
        long long end_element = current_start + chunk_size;
    
        // Jatah (start_element, end_element) sudah final saat thread dibuat.
        threads.emplace_back(worker_function, i, current_start, end_element);
        current_start = end_element;
    }
    ```
    
- **Alokasi Dinamis (Dynamic Load Balancing)**: Beban kerja didistribusikan saat program berjalan. Biasanya, ada antrean tugas (*task pool*) di mana thread yang menganggur dapat mengambil pekerjaan baru. Ini lebih baik untuk tugas yang tidak dapat diprediksi tetapi memiliki overhead manajemen yang lebih tinggi.

Ketika jumlah pekerjaan yang akan dilakukan setiap tugas sengaja bervariasi, atau tidak dapat diprediksi, mungkin akan membantu untuk menggunakan pendekatan ***kumpulan tugas penjadwal***. Saat setiap tugas menyelesaikan pekerjaannya, ia menerima bagian baru dari antrean kerja.

![img3](./assets/schedulerTaskPool.jpg)

Pada akhirnya, mungkin perlu untuk merancang algoritme yang mendeteksi dan menangani ketidakseimbangan beban saat terjadi secara dinamis dalam kode.

[1] B. Barney, "Introduction to Parallel Computing Tutorial," Lawrence Livermore National Laboratory.

## Video Demonstrasi

[Akses Video dalam Assets](./assets/Video_Demo.mkv)

https://github.com/user-attachments/assets/89488b9f-5809-4926-957f-9f11f9c99896


## Daftar Pustaka

1. [https://en.cppreference.com/w/cpp/thread/thread](https://en.cppreference.com/w/cpp/thread/thread)
2. [https://cplusplus.com/reference/mutex/mutex/](https://cplusplus.com/reference/mutex/mutex/)
3. [https://www.geeksforgeeks.org/multithreading-in-cpp/](https://www.geeksforgeeks.org/multithreading-in-cpp/)
