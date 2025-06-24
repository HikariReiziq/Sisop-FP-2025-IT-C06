#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

// 1. Variabel global yang di-hardcode
const int NUM_SIZE = 999999;
const int NUM_THREADS = 4;

// Variabel global untuk menyimpan hasil akhir
long long global_sum = 0;

// Mutex untuk melindungi akses ke global_sum dan std::cout
std::mutex sum_mutex;
std::mutex cout_mutex;

/**
 * @brief Fungsi yang akan dijalankan oleh setiap thread.
 * * @param thread_id ID dari thread yang sedang berjalan.
 * @param start_element Indeks awal elemen untuk thread ini.
 * @param end_element Indeks akhir (eksklusif) untuk thread ini.
 */
void worker_function(int thread_id, long long start_element, long long end_element) {
    // Setiap thread menghitung "hasil lokal" mereka.
    // Dalam kasus ini, hasil lokal adalah jumlah elemen yang mereka tangani.
    long long local_result = end_element - start_element;

    // --- Critical Section untuk Penjumlahan ---
    // Gunakan lock_guard untuk mengunci mutex secara otomatis.
    // Mutex akan terbuka saat 'lock' keluar dari scope blok ini.
    {
        std::lock_guard<std::mutex> lock(sum_mutex);
        global_sum += local_result;
    }
    // --- Akhir Critical Section ---


    // --- Critical Section untuk Printing ---
    // Mengunci output agar tidak tercampur dengan thread lain.
    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        // 4. Print setiap thread dan angka yang mereka miliki
        // (end_element - 1) karena end_element bersifat eksklusif (tidak termasuk)
        std::cout << "Thread " << thread_id << " (Elemen " << start_element << "-" << end_element - 1
                  << ") selesai. Hasil lokal: " << local_result << std::endl;
    }
    // --- Akhir Critical Section ---
}

int main() {
    std::cout << "Memulai penjumlahan paralel untuk " << NUM_SIZE
              << " elemen dengan " << NUM_THREADS << " thread...\n" << std::endl;

    // Vector untuk menyimpan semua thread yang dibuat
    std::vector<std::thread> threads;

    // 2. Bagi pekerjaan (NUM_SIZE) ke setiap thread
    long long base_chunk_per_thread = NUM_SIZE / NUM_THREADS;
    long long remainder = NUM_SIZE % NUM_THREADS;
    
    long long current_start = 0;

    // Membuat dan menjalankan thread
    for (int i = 0; i < NUM_THREADS; ++i) {
        long long chunk_size = base_chunk_per_thread;
        
        // 3. Tambahkan sisa pembagian ke thread terakhir
        if (i == NUM_THREADS - 1) {
            chunk_size += remainder;
        }

        long long end_element = current_start + chunk_size;

        // Membuat thread baru dan menjalankannya dengan worker_function
        threads.emplace_back(worker_function, i, current_start, end_element);

        // Update indeks awal untuk thread berikutnya
        current_start = end_element;
    }

    // Menunggu semua thread selesai (mirip konsep pthread_join)
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    
    std::cout << "\n--- Perhitungan Selesai ---\n";
    // 5. Print hasil akhir dari penjumlahan global
    std::cout << "Hasil akhir penjumlahan global: " << global_sum << std::endl;

    return 0;
}


// Penjelasan Kode
// Variabel Global & Mutex:

// NUM_SIZE dan NUM_THREADS didefinisikan sebagai konstanta sesuai permintaan soal.
// global_sum adalah variabel bersama yang akan diisi oleh semua thread. Karena ini adalah sumber daya bersama, aksesnya harus dilindungi.
// sum_mutex dibuat untuk melindungi global_sum.
// cout_mutex dibuat untuk melindungi std::cout agar hasil print tidak tumpang tindih.
// Pembagian Beban (main function):

// base_chunk_per_thread menghitung jumlah elemen dasar yang akan ditangani setiap thread.
// remainder menghitung sisa elemen setelah pembagian.
// Dalam for loop, kita menentukan chunk_size untuk setiap thread. Untuk thread terakhir (i == NUM_THREADS - 1), kita tambahkan remainder ke bebannya. Ini memenuhi syarat nomor 3 dari soal.
// Fungsi Pekerja (worker_function):

// Ini adalah fungsi yang dieksekusi oleh setiap thread.
// local_result adalah jumlah elemen yang ditangani oleh thread ini.
// std::lock_guard<std::mutex> lock(sum_mutex); adalah cara modern dan aman untuk menggunakan mutex di C++. Saat objek lock dibuat, ia otomatis mengunci sum_mutex. Ketika eksekusi keluar dari blok {...}, lock akan hancur dan otomatis membuka kuncian mutex. Ini mencegah lupa membuka kunci (unlock).
// Di dalam blok yang terkunci, thread dengan aman menambahkan local_result-nya ke global_sum.
// Proses yang sama diulang untuk cout_mutex saat akan mencetak ke layar, memastikan setiap baris output tercetak utuh.
// Join Threads:

// for (auto& t : threads) { t.join(); } adalah bagian di mana program utama (main) akan menunggu sampai semua thread selesai mengerjakan tugasnya sebelum melanjutkan eksekusi ke baris berikutnya. Ini memastikan global_sum sudah final sebelum dicetak.