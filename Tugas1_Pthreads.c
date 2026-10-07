#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*
 * Mutex digunakan untuk mengatur akses ke console.
 * Tujuannya agar output dari beberapa thread tidak saling bertabrakan.
 */
pthread_mutex_t output_mutex = PTHREAD_MUTEX_INITIALIZER;

void *hitung_faktorial(void *arg) {
    int n = 5;
    long long hasil = 1;

    for (int i = 1; i <= n; i++) {
        hasil *= i;
    }

    pthread_mutex_lock(&output_mutex);
    printf("[Thread-1-Faktorial] %d! = %lld\n", n, hasil);
    pthread_mutex_unlock(&output_mutex);

    return NULL;
}

void *tampilkan_fibonacci(void *arg) {
    int jumlah = 10;
    long long a = 0;
    long long b = 1;

    pthread_mutex_lock(&output_mutex);
    printf("[Thread-2-Fibonacci] Deret Fibonacci: ");
    pthread_mutex_unlock(&output_mutex);

    for (int i = 0; i < jumlah; i++) {
        pthread_mutex_lock(&output_mutex);
        printf("%lld", a);

        if (i < jumlah - 1) {
            printf(" ");
        }
        pthread_mutex_unlock(&output_mutex);

        long long berikutnya = a + b;
        a = b;
        b = berikutnya;
    }

    pthread_mutex_lock(&output_mutex);
    printf("\n");
    pthread_mutex_unlock(&output_mutex);

    return NULL;
}

void *baca_file(void *arg) {
    FILE *file = fopen("data.txt", "r");

    pthread_mutex_lock(&output_mutex);

    if (file == NULL) {
        printf("[Thread-3-BacaFile] Gagal membuka data.txt\n");
        pthread_mutex_unlock(&output_mutex);
        return NULL;
    }

    printf("[Thread-3-BacaFile] Isi file:\n");
    pthread_mutex_unlock(&output_mutex);

    char baris[256];

    while (fgets(baris, sizeof(baris), file) != NULL) {
        pthread_mutex_lock(&output_mutex);
        printf("[Thread-3-BacaFile] %s", baris);
        pthread_mutex_unlock(&output_mutex);
    }

    fclose(file);

    return NULL;
}

int main(void) {
    pthread_t thread1, thread2, thread3;

    printf("=== TUGAS 1 - PTHREADS / MULTITHREADING ===\n");
    printf("Bahasa: C\n\n");

    /*
     * pthread_create() membuat tiga thread.
     * Masing-masing thread menjalankan fungsi yang berbeda.
     */
    pthread_create(&thread1, NULL, hitung_faktorial, NULL);
    pthread_create(&thread2, NULL, tampilkan_fibonacci, NULL);
    pthread_create(&thread3, NULL, baca_file, NULL);

    /*
     * pthread_join() membuat main thread menunggu
     * sampai ketiga thread selesai.
     */
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    pthread_mutex_destroy(&output_mutex);

    printf("\nSemua thread telah selesai.\n");

    return 0;
}
