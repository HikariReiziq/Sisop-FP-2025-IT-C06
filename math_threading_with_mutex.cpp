#include <iostream>
#include <thread>
#include <vector>

using namespace std;

// Hardcoded global variables
const int NUM_SIZE = 999999;
const int NUM_THREADS = 4;

vector<int> hasil_lokal(NUM_THREADS, 0);

void worker(int thread_id, int start, int end) {
    // Semua elemen bernilai 1, sehingga jumlah = (end - start + 1)
    int sum = end - start + 1;
    hasil_lokal[thread_id] = sum;
    printf("Thread %d (Elemen %d-%d) selesai. Hasil lokal: %d\n", thread_id, start, end, sum);
}

int main() {
    printf("Memulai penjumlahan paralel untuk %d elemen dengan %d thread...\n\n", NUM_SIZE, NUM_THREADS);
    vector<thread> threads;
    int part_size = NUM_SIZE / NUM_THREADS;
    int sisa = NUM_SIZE % NUM_THREADS;

    int current = 0;
    for (int i = 0; i < NUM_THREADS; ++i) {
        int start = current;
        int end = start + part_size - 1;
        if (i == NUM_THREADS - 1) end += sisa; // thread terakhir dapat sisa
        threads.emplace_back(worker, i, start, end);
        current = end + 1;
    }

    // Join all threads
    for (auto& t : threads) t.join();

    printf("\n--- Perhitungan Selesai ---\n");
    int total = 0;
    for (int i = 0; i < NUM_THREADS; ++i)
        total += hasil_lokal[i];

    printf("Hasil akhir penjumlahan global: %d\n", total);
    return 0;
}
