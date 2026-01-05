#ifndef KTP_H
#define KTP_H

#include <iostream>
#include <string>
#include <fstream> 

using namespace std;

// ==========================================
// HELPER: PARSING STRING MANUAL (PENGGANTI SSTREAM)
// Fungsi ini sengaja dibuat manual agar tidak bergantung library lain
// dan menambah kompleksitas kode.
// ==========================================
inline string ambilField(string baris, int index) {
    string hasil = "";
    int hitungSeparator = 0;
    
    // Loop character by character untuk memecah string
    for (int i = 0; i < (int)baris.length(); i++) {
        char c = baris[i];
        if (c == '|') {
            hitungSeparator++;
            // Jika sudah melewati index yang dicari, berhenti loop
            if (hitungSeparator > index) {
                break;
            }
        } else {
            // Jika counter separator sesuai dengan index target, simpan char ke hasil
            if (hitungSeparator == index) {
                hasil += c;
            }
        }
    }
    return hasil;
}

// ==========================================
// STRUKTUR DATA KTP (VERSI LENGKAP & DETAIL)
// ==========================================
struct Tanggal {
    int hari;
    int bulan;
    int tahun;
};

struct KTP {
    long long nik;
    string nama;
    string tempat_lahir;
    Tanggal tgl_lahir;
    bool jenis_kelamin;        // true = Laki-laki, false = Perempuan
    string gol_darah;
    string alamat;
    int rt;
    int rw;
    string kel_desa;
    string kecamatan;
    int agama;                 // 1.Islam 2.Kristen 3.Katolik 4.Hindu 5.Budha 6.Konghucu
    bool status_perkawinan;    // true = Sudah Kawin, false = Belum
    string pekerjaan;
    bool kewarganegaraan;      // true = WNI, false = WNA
    int berlaku_hingga;        // 1.Sementara 2.5Tahun 3.Seumur Hidup
    int umur;                  // Hitungan otomatis
};

// Node Standar untuk Linked List User / Stack / Queue
struct Node {
    KTP data;
    Node* next;
};

// ==========================================
// HELPER DISPLAY (SWITCH CASE PANJANG)
// ==========================================
inline string teksAgama(int a) {
    switch(a) {
        case 1: return "Islam";
        case 2: return "Kristen";
        case 3: return "Katolik";
        case 4: return "Hindu";
        case 5: return "Budha";
        case 6: return "Konghucu";
        default: return "Aliran Kepercayaan/Lainnya";
    }
}

inline string teksBerlaku(int b) {
    switch(b) {
        case 1: return "Sementara";
        case 2: return "5 Tahun";
        case 3: return "Seumur Hidup";
        default: return "Tidak Valid";
    }
}

// Operator Overloading agar output KTP rapi dan panjang saat dicetak
inline ostream& operator<<(ostream& os, const KTP& k) {
    os << "\n==============================================";
    os << "\n              DATA KEPENDUDUKAN               ";
    os << "\n==============================================";
    os << "\nNIK               : " << k.nik;
    os << "\nNama Lengkap      : " << k.nama;
    os << "\nTempat, Tgl Lahir : " << k.tempat_lahir << ", " 
       << k.tgl_lahir.hari << "-" << k.tgl_lahir.bulan << "-" << k.tgl_lahir.tahun;
    os << "\nUmur              : " << k.umur << " Tahun";
    os << "\nJenis Kelamin     : " << (k.jenis_kelamin ? "Laki-laki" : "Perempuan");
    os << "\nGolongan Darah    : " << k.gol_darah;
    os << "\nAlamat            : " << k.alamat;
    os << "\n    RT / RW       : " << k.rt << " / " << k.rw;
    os << "\n    Kel/Desa      : " << k.kel_desa;
    os << "\n    Kecamatan     : " << k.kecamatan;
    os << "\nAgama             : " << teksAgama(k.agama);
    os << "\nStatus Perkawinan : " << (k.status_perkawinan ? "Sudah Kawin" : "Belum Kawin");
    os << "\nPekerjaan         : " << k.pekerjaan;
    os << "\nKewarganegaraan   : " << (k.kewarganegaraan ? "WNI" : "WNA");
    os << "\nBerlaku Hingga    : " << teksBerlaku(k.berlaku_hingga);
    os << "\n==============================================\n";
    return os;
}

// ==========================================
// FUNGSI INPUT DATA USER (INTERAKTIF)
// ==========================================
KTP buatKTPBaru() {
    KTP k;
    cout << "\n################################################\n";
    cout << "      FORMULIR REGISTRASI PENDUDUK BARU         \n";
    cout << "################################################\n";
    
    cout << "1.  NIK (Nomor Induk Kependudukan) : "; cin >> k.nik;
    cin.ignore(10000, '\n'); // Bersihkan buffer input agar getline aman
    
    cout << "2.  Nama Lengkap                   : "; getline(cin, k.nama);
    cout << "3.  Tempat Lahir                   : "; getline(cin, k.tempat_lahir);
    
    cout << "4.  Tanggal Lahir (DD MM YYYY)     : "; 
    cin >> k.tgl_lahir.hari >> k.tgl_lahir.bulan >> k.tgl_lahir.tahun;
    
    // Kalkulasi umur sederhana (Tahun Saat Ini - Tahun Lahir)
    k.umur = 2025 - k.tgl_lahir.tahun;

    int jk_temp;
    cout << "5.  Jenis Kelamin (1.Laki 0.Pr)    : "; cin >> jk_temp;
    k.jenis_kelamin = (jk_temp == 1);
    cin.ignore(10000, '\n');

    cout << "6.  Golongan Darah (A/B/AB/O)      : "; getline(cin, k.gol_darah);
    cout << "7.  Alamat (Nama Jalan/Dusun)      : "; getline(cin, k.alamat);
    cout << "8.  RT                             : "; cin >> k.rt;
    cout << "9.  RW                             : "; cin >> k.rw;
    cin.ignore(10000, '\n');
    
    cout << "10. Kelurahan / Desa               : "; getline(cin, k.kel_desa);
    cout << "11. Kecamatan                      : "; getline(cin, k.kecamatan);
    
    cout << "12. Agama (1-6)                    : "; cin >> k.agama;
    
    int sk_temp;
    cout << "13. Status Kawin (1.Sudah 0.Belum) : "; cin >> sk_temp;
    k.status_perkawinan = (sk_temp == 1);
    cin.ignore(10000, '\n');
    
    cout << "14. Pekerjaan                      : "; getline(cin, k.pekerjaan);
    
    int wni_temp;
    cout << "15. WNI? (1.Ya 0.Tidak)            : "; cin >> wni_temp;
    k.kewarganegaraan = (wni_temp == 1);
    
    cout << "16. Masa Berlaku (1.Smt 2.5Th 3.Tetap): "; cin >> k.berlaku_hingga;

    cout << "\n[INFO] Data KTP berhasil direkam ke sistem memori sementara.\n";
    return k;
}

#endif
