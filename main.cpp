#include <iostream>
#include "ktp.h"
#include "linkedlist.h"
#include "rental_header.h"

using namespace std;

// INSTANCE GLOBAL
LinkedListKTP dbUser;
SenaraiRental dbTrans;

// ==========================================
// MENU USER (SETELAH LOGIN)
// ==========================================
void menuUser(KTP* user) {
    int p;
    while(true) {
        cout << "\n============================================\n";
        cout << "       SELAMAT DATANG, " << user->nama << " \n";
        cout << "============================================\n";
        cout << "1. Lihat Profil Lengkap Saya (E-KTP)\n";
        cout << "2. Sewa Mobil Baru\n";
        cout << "3. Lihat Promo Mobil (Double Linked List)\n";
        cout << "4. Ajukan Refund / Keluhan\n";
        cout << "0. Logout / Keluar Akun\n";
        cout << "============================================\n";
        cout << "Masukkan Pilihan Anda: "; 
        cin >> p; 
        bersihkanBufferInput();

        if (p == 1) {
            // Menampilkan data KTP menggunakan operator<< yang ada di ktp.h
            cout << *user; 
        } 
        else if (p == 2) {
            RentalMobil r;
            // Pass pointer user ke fungsi rental agar auto-fill NIK/Nama/Alamat
            r.input(user); 
            
            // Tampilkan struk bukti transaksi
            r.struk();
            
            // Simpan transaksi ke database (RAM)
            dbTrans.add(r);
            
            // Simpan transaksi ke File (Harddisk) - Auto Save
            dbTrans.save();
            
            // Catat log aktivitas sistem
            appLog.log("Transaksi Sewa Mobil oleh User: " + user->nama);
        } 
        else if (p == 3) {
            // Memanggil fitur Promo Double Linked List
            daftarPromo.lihatPromo();
            appLog.log("User melihat daftar promo.");
        }
        else if (p == 4) {
            string a; 
            cout << "Silakan tulis alasan Refund/Keluhan secara detail: "; 
            getline(cin, a);
            
            // Masukkan ke Queue (Antrian)
            antrianRefund.ajukan(user->nama, a); 
            appLog.log("User " + user->nama + " mengajukan refund.");
        } 
        else if (p == 0) {
            cout << "Logout berhasil. Sampai jumpa " << user->nama << "!\n";
            break;
        } 
        else {
            cout << "Pilihan tidak valid. Silakan coba lagi.\n";
        }
    }
}

// ==========================================
// MENU ADMIN (PANEL KONTROL)
// ==========================================
void menuAdmin() {
    int p;
    while(true) {
        cout << "\n############################################\n";
        cout << "           ADMIN CONTROL PANEL              \n";
        cout << "############################################\n";
        cout << "1. Lihat Semua Riwayat Transaksi\n";
        cout << "2. Lihat Log Aktivitas Sistem\n";
        cout << "3. Analisis Harga Sewa (BST Sorting)\n";
        cout << "4. Analisis Nama Pelanggan (Huffman Freq)\n";
        cout << "5. Proses Antrian Refund (FIFO)\n";
        cout << "6. Kelola Antrian Prioritas (Priority Queue)\n";
        cout << "7. Hapus Transaksi Terakhir (Soft Delete)\n";
        cout << "8. Restore Transaksi dari Sampah (Undo)\n";
        cout << "0. Kembali ke Menu Utama\n";
        cout << "############################################\n";
        cout << "Pilih Menu Admin: "; 
        cin >> p; 
        bersihkanBufferInput();

        if(p == 1) {
            dbTrans.show();
        }
        else if(p == 2) {
            appLog.show();
        }
        else if(p == 3) { 
            BST b; 
            b.build(dbTrans); 
            b.show(); 
        }
        else if(p == 4) {
            huffman(dbTrans);
        }
        else if(p == 5) {
            antrianRefund.proses();
        }
        else if(p == 6) {
            int sub;
            cout << "\n--- KELOLA ANTRIAN PRIORITAS ---\n";
            cout << "1. Tambah Pelanggan Prioritas\n";
            cout << "2. Panggil Pelanggan (Dequeue)\n";
            cout << "3. Lihat Daftar Antrian\n";
            cout << "Pilih: "; cin >> sub;
            
            if (sub == 1) {
                string nm; int lvl;
                cout << "Nama Pelanggan: "; cin >> nm;
                cout << "Level Prioritas (1.VVIP 2.VIP 3.Biasa): "; cin >> lvl;
                antrianPrioritas.enqueue(nm, lvl);
            } else if (sub == 2) {
                antrianPrioritas.dequeue();
            } else if (sub == 3) {
                antrianPrioritas.display();
            }
        }
        else if(p == 7) { 
            dbTrans.delLast(); 
            dbTrans.save(); 
        }
        else if(p == 8) { 
            dbTrans.restore(); 
            dbTrans.save(); 
        }
        else if(p == 0) {
            break;
        }
        else {
            cout << "Input salah.\n";
        }
    }
}

// ==========================================
// MAIN PROGRAM
// ==========================================
int main() {
    cout << "\n[BOOT] Sedang memuat sistem dan database...\n";
    
    // Muat semua data dari file txt
    dbUser.muatDataDariFile();
    dbTrans.load();
    
    // Inisialisasi Akun Admin (Hardcoded)
    KTP admin; 
    admin.nik = 12345; 
    admin.nama = "SUPER ADMIN";
    dbUser.insert(admin); // Hanya insert ke RAM

    int p;
    while(true) {
        cout << "\n============================================\n";
        cout << "    SISTEM RENTAL MOBIL TERINTEGRASI E-KTP  \n";
        cout << "============================================\n";
        cout << "1. Registrasi Penduduk Baru (Buat E-KTP)\n";
        cout << "2. Login User (Menggunakan NIK)\n";
        cout << "3. Login Admin (Password: admin)\n";
        cout << "0. Keluar & Simpan Data\n";
        cout << "============================================\n";
        cout << "Masukkan Pilihan Anda: "; 
        cin >> p; 
        bersihkanBufferInput();

        if (p == 1) {
            KTP k = buatKTPBaru();
            dbUser.insert(k);
            dbUser.simpanDataKeFile();
        } 
        else if (p == 2) {
            long long nik; 
            cout << "\nMasukkan NIK Terdaftar Anda: "; 
            cin >> nik;
            
            KTP* u = dbUser.loginByNIK(nik);
            
            if (u != NULL) {
                cout << "Login Berhasil! Mengalihkan...";
                menuUser(u);
            } else {
                cout << "[ERROR] NIK tidak ditemukan dalam database penduduk.\n";
                cout << "Silakan Registrasi (Menu 1) terlebih dahulu.\n";
            }
        } 
        else if (p == 3) {
            string pw; 
            cout << "\nMasukkan Password Admin: "; 
            cin >> pw;
            
            if (pw == "admin") {
                menuAdmin();
            } else {
                cout << "[ACCESS DENIED] Password Admin Salah!\n";
            }
        } 
        else if (p == 0) {
            cout << "\n[SHUTDOWN] Menyimpan seluruh data ke harddisk...\n";
            dbUser.simpanDataKeFile();
            dbTrans.save();
            cout << "Data tersimpan aman. Terima kasih telah menggunakan aplikasi ini.\n";
            break;
        } 
        else {
            cout << "Pilihan tidak valid.\n";
        }
    }
    return 0;
}
