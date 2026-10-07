import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class Tugas1_Pthreads {

    // Digunakan agar output dari beberapa thread tidak saling bertabrakan.
    private static final Object OUTPUT_LOCK = new Object();

    static class FactorialThread extends Thread {
        public FactorialThread() {
            super("Thread-1-Faktorial");
        }

        @Override
        public void run() {
            int n = 5;
            long hasil = 1;

            for (int i = 1; i <= n; i++) {
                hasil *= i;
            }

            synchronized (OUTPUT_LOCK) {
                System.out.println("[" + getName() + "] " + n + "! = " + hasil);
            }
        }
    }

    static class FibonacciThread extends Thread {
        public FibonacciThread() {
            super("Thread-2-Fibonacci");
        }

        @Override
        public void run() {
            int jumlah = 10;
            long a = 0;
            long b = 1;

            StringBuilder deret = new StringBuilder();

            for (int i = 0; i < jumlah; i++) {
                deret.append(a);
                if (i < jumlah - 1) {
                    deret.append(" ");
                }

                long berikutnya = a + b;
                a = b;
                b = berikutnya;
            }

            synchronized (OUTPUT_LOCK) {
                System.out.println("[" + getName() + "] Deret Fibonacci: " + deret);
            }
        }
    }

    static class FileThread extends Thread {
        public FileThread() {
            super("Thread-3-BacaFile");
        }

        @Override
        public void run() {
            synchronized (OUTPUT_LOCK) {
                System.out.println("[" + getName() + "] Isi file:");
            }

            try (BufferedReader reader = new BufferedReader(new FileReader("data.txt"))) {
                String baris;

                while ((baris = reader.readLine()) != null) {
                    synchronized (OUTPUT_LOCK) {
                        System.out.println("[" + getName() + "] " + baris);
                    }
                }
            } catch (IOException e) {
                synchronized (OUTPUT_LOCK) {
                    System.out.println("[" + getName() + "] Gagal membaca file: " + e.getMessage());
                }
            }
        }
    }

    public static void main(String[] args) {
        System.out.println("=== TUGAS 1 - PTHREADS / MULTITHREADING ===");
        System.out.println("Bahasa: Java");
        System.out.println();

        Thread thread1 = new FactorialThread();
        Thread thread2 = new FibonacciThread();
        Thread thread3 = new FileThread();

        // start() menjalankan ketiga thread secara concurrent.
        thread1.start();
        thread2.start();
        thread3.start();

        try {
            // join() membuat main thread menunggu ketiga thread selesai.
            thread1.join();
            thread2.join();
            thread3.join();
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            System.out.println("Main thread dihentikan.");
        }

        System.out.println();
        System.out.println("Semua thread telah selesai.");
    }
}
