#ifndef RENTAL_MOBIL_H
#define RENTAL_MOBIL_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <string>
#include "ktp.h" // Wajib include ktp.h

using namespace std;

// ==========================================
// KONSTANTA BIAYA DAN DENDA
// ==========================================
const long BIAYA_SOPIR_PER_JAM = 25000;
const long DENDA_TERLAMBAT_PER_JAM = 50000;
const long DISKON_HARIAN_FLAT = 200000;
const long DISKON_MINGGUAN_FLAT = 400000;

// ==========================================
// FORWARD DECLARATIONS (Agar Class Saling Kenal)
// ==========================================
struct ModelMobil;
class DaftarModelMobil;
class RentalMobil;
struct NodeRental;
class SenaraiRental;

// ==========================================
// FUNGSI UTILITY (HELPER)
// ==========================================
void bersihkanBufferInput() { 
    cin.ignore(10000, '\n'); 
}

string ambilWaktuSekarang() {
    time_t now = time(0);
    char* dt = ctime(&now);
    string tgl = (dt) ? dt : "-";
    if(!tgl.empty() && tgl.back()=='\n') tgl.pop_back();
    return tgl;
}

// ==========================================
// 1. DATA MOBIL (DOUBLY LINKED LIST)
// ==========================================
struct ModelMobil {
    int id; 
    string nama;
    string merk; 
    long hargaPerJam;
};

struct NodeModel {
    ModelMobil data; 
    NodeModel *next; 
    NodeModel *prev;
    
    NodeModel(int id, string n, string m, long h) : next(NULL), prev(NULL) {
        data.id = id; 
        data.nama = n; 
        data.merk = m; 
        data.hargaPerJam = h;
    }
};

class DaftarModelMobil {
private:
    NodeModel *head;
    NodeModel *tail;

    void init() {
        // Inisialisasi 20 Data Mobil (Hardcode agar data selalu ada)
        tambahModel(1, "Civic Turbo", "Honda", 500000); 
        tambahModel(2, "Jazz RS", "Honda", 300000);
        tambahModel(3, "Brio Satya", "Honda", 250000); 
        tambahModel(4, "CR-V Prestige", "Honda", 600000);
        tambahModel(5, "City Hatchback", "Honda", 450000); 
        tambahModel(6, "Yaris GR", "Toyota", 700000);
        tambahModel(7, "Innova Reborn", "Toyota", 800000); 
        tambahModel(8, "Avanza Veloz", "Toyota", 350000);
        tambahModel(9, "Fortuner VRZ", "Toyota", 950000); 
        tambahModel(10, "Rush TRD", "Toyota", 550000);
        tambahModel(11, "Ayla R", "Daihatsu", 200000); 
        tambahModel(12, "Sigra R", "Daihatsu", 250000);
        tambahModel(13, "Xenia Xi", "Daihatsu", 300000); 
        tambahModel(14, "Terios TX", "Daihatsu", 450000);
        tambahModel(15, "Rocky Turbo", "Daihatsu", 400000); 
        tambahModel(16, "Xpander Ult.", "Mitsubishi", 450000);
        tambahModel(17, "Xpander Cross", "Mitsubishi", 500000); 
        tambahModel(18, "Pajero Sport", "Mitsubishi", 1000000);
        tambahModel(19, "Xforce Exceed", "Mitsubishi", 550000); 
        tambahModel(20, "Triton 4x4", "Mitsubishi", 800000);
    }

public:
    DaftarModelMobil() : head(NULL), tail(NULL) { 
        init(); 
    }
    
    void tambahModel(int id, string n, string m, long h) {
        NodeModel* b = new NodeModel(id, n, m, h);
        if (!head) {
            head = tail = b;
        } else { 
            tail->next = b; 
            b->prev = tail; 
            tail = b; 
        }
    }
    
    const ModelMobil* cari(int id) const {
        NodeModel* t = head; 
        while (t) { 
            if (t->data.id == id) return &t->data; 
            t = t->next; 
        } 
        return NULL;
    }
    
    void showMerk(string m) const {
        NodeModel* t = head; 
        bool f = false;
        cout << "\n--- LIST MOBIL MERK: " << m << " ---\n";
        while (t) { 
            if (t->data.merk == m) { 
                cout << "ID: " << setw(2) << t->data.id 
                     << " | " << setw(15) << t->data.nama 
                     << " | Rp " << t->data.hargaPerJam << "/jam\n"; 
                f = true; 
            } 
            t = t->next; 
        }
        if (!f) cout << "Merk tidak ditemukan atau stok kosong.\n";
    }
};

// Objek Global Mobil
DaftarModelMobil dbMobil;
const ModelMobil* getMobil(int id) { return dbMobil.cari(id); }

// ==========================================
// 2. CLASS RENTAL (INTI LOGIKA BISNIS)
// ==========================================
class Penyewa {
public: 
    string nama;
    string alamat;
    string noHpKontak; 
    long long nik;
    
    // Fungsi Input Manual (Fallback jika user belum login)
    void manual() {
        cout << "Nama Lengkap   : "; getline(cin, nama); 
        cout << "NIK            : "; cin >> nik;
        cout << "HP (Wajib Isi) : "; cin >> noHpKontak; 
        bersihkanBufferInput(); 
        cout << "Alamat         : "; getline(cin, alamat);
    }
};

class RentalMobil {
public:
    Penyewa penyewa; 
    int idMobil; 
    int paket; 
    long durasi; 
    bool supir; 
    long long total; 
    string tgl;
    
    void hitung() {
        const ModelMobil* m = getMobil(idMobil); 
        if (!m) { total = 0; return; }
        
        long jam = 0;
        long disc = 0;

        if (paket == 1) { // Per Jam
            jam = durasi;
            disc = 0;
        } else if (paket == 2) { // Per Hari
            jam = durasi * 24;
            disc = DISKON_HARIAN_FLAT * durasi;
        } else if (paket == 3) { // Per Minggu
            jam = durasi * 24 * 7;
            disc = DISKON_MINGGUAN_FLAT * durasi;
        }
        
        long biayaDasar = jam * m->hargaPerJam;
        long biayaSupir = supir ? (jam * BIAYA_SOPIR_PER_JAM) : 0;
        
        total = biayaDasar + biayaSupir - disc;
        if (total < 0) total = 0;
    }
    
    // INTEGRASI DATA KTP KE TRANSAKSI
    void input(KTP* u = NULL) {
        if (u) {
            // Autofill dari Pointer KTP yang sedang login
            penyewa.nama = u->nama; 
            penyewa.nik = u->nik; 
            penyewa.alamat = u->alamat;
            
            cout << "\n[SYSTEM AUTO-FILL] Data dasar diambil dari E-KTP User: " << u->nama << ".\n";
            cout << "Karena struktur KTP baru tidak memiliki data No HP, mohon lengkapi:\n";
            cout << "Nomor HP / WhatsApp (Aktif) : "; 
            cin >> penyewa.noHpKontak;
        } else {
            // Jika Guest / Admin
            penyewa.manual();
        }

        cout << "\nPilih Kategori Merk Mobil:\n";
        cout << "1. Honda\n2. Toyota\n3. Daihatsu\n4. Mitsubishi\n";
        cout << "Pilihan: "; 
        int mk; cin >> mk;
        string smk = (mk == 1) ? "Honda" : (mk == 2) ? "Toyota" : (mk == 3) ? "Daihatsu" : "Mitsubishi";
        
        dbMobil.showMerk(smk);
        
        cout << "\nMasukkan ID Mobil pilihan Anda : "; cin >> idMobil;
        cout << "Pilih Paket (1.Jam 2.Hari 3.Minggu) : "; cin >> paket;
        cout << "Durasi Sewa (Angka)                 : "; cin >> durasi;
        cout << "Gunakan Jasa Sopir? (1.Ya 0.Tidak)  : "; cin >> supir;
        
        tgl = ambilWaktuSekarang(); 
        hitung();
    }
    
    void struk() {
        const ModelMobil* m = getMobil(idMobil);
        cout << "\n============================================\n";
        cout << "          STRUKTI BUKTI PENYEWAAN           \n";
        cout << "============================================\n";
        cout << "Nama Penyewa   : " << penyewa.nama << endl;
        cout << "NIK            : " << penyewa.nik << endl;
        cout << "Kontak         : " << penyewa.noHpKontak << endl;
        cout << "Unit Mobil     : " << (m ? m->nama : "Invalid ID") << endl;
        cout << "Merk           : " << (m ? m->merk : "-") << endl;
        cout << "Durasi         : " << durasi << ((paket==1)?" Jam": (paket==2)?" Hari":" Minggu") << endl;
        cout << "Supir          : " << (supir ? "Ya" : "Tidak") << endl;
        cout << "Waktu Transaksi: " << tgl << endl;
        cout << "--------------------------------------------\n";
        cout << "TOTAL BIAYA    : Rp " << total << endl;
        cout << "============================================\n";
    }
};

struct NodeRental { 
    RentalMobil data; 
    NodeRental *next; 
    NodeRental *prev; 
    NodeRental(RentalMobil d) : data(d), next(NULL), prev(NULL) {} 
};

// ==========================================
// 3. STRUKTUR DATA LANJUTAN (STACK, QUEUE, DLL)
// ==========================================

// A. STACK SAMPAH (UNDO DELETE)
class StackSampah {
    NodeRental* top;
public:
    StackSampah() : top(NULL) {}
    
    void push(RentalMobil d) {
        NodeRental* x = new NodeRental(d); 
        x->next = top; 
        top = x;
    }
    
    bool pop(RentalMobil& d) {
        if (!top) return false; 
        NodeRental* t = top; 
        d = t->data; 
        top = top->next; 
        delete t; 
        return true;
    }
};
StackSampah tongSampah;

// B. SYSTEM LOG (LINKED LIST)
struct NodeLog { string a, w; NodeLog* n; };
class SysLog {
    NodeLog* h;
public:
    SysLog() : h(NULL) {}
    void log(string a) {
        NodeLog* x = new NodeLog; 
        x->a = a; 
        x->w = ambilWaktuSekarang(); 
        x->n = h; 
        h = x;
    }
    void show() {
        NodeLog* t = h; 
        cout << "\n=== SYSTEM ACTIVITY LOG ===\n"; 
        while (t) {
            cout << "[" << t->w << "] " << t->a << endl; 
            t = t->n;
        }
    }
};
SysLog appLog;

// C. QUEUE REFUND (ANTRIAN KELUHAN)
struct NodeRef { string n, a; NodeRef* next; };
class QueueRefund {
    NodeRef *f, *r;
public:
    QueueRefund() : f(NULL), r(NULL) {}
    
    void ajukan(string n, string a) {
        NodeRef* x = new NodeRef; 
        x->n = n; 
        x->a = a; 
        x->next = NULL; 
        if (!f) f = r = x; 
        else { r->next = x; r = x; }
        cout << ">> Pengajuan Refund berhasil masuk antrian.\n";
    }
    
    void proses() {
        if (!f) {
            cout << ">> Antrian Refund Kosong.\n";
            return;
        } 
        NodeRef* t = f; 
        cout << "\n[PROSES REFUND] \nNama: " << t->n << "\nAlasan: " << t->a << endl; 
        f = f->next; 
        if (!f) r = NULL; 
        delete t;
        cout << ">> Status: DISETUJUI & SELESAI.\n";
    }
};
QueueRefund antrianRefund;

// D. SENARAI RENTAL (DATABASE UTAMA - DOUBLY LINKED LIST)
class SenaraiRental {
public:
    NodeRental *head, *tail;
    
    SenaraiRental() : head(NULL), tail(NULL) {}
    
    // Tambah Data di Belakang
    void add(RentalMobil d) {
        NodeRental* x = new NodeRental(d); 
        if (!head) head = tail = x; 
        else { tail->next = x; x->prev = tail; tail = x; }
    }
    
    // Tampilkan Semua Data Transaksi
    void show() {
        NodeRental* t = head; 
        int i = 1; 
        cout << "\n=== REKAP DATA TRANSAKSI ===\n";
        while (t) {
            const ModelMobil* m = getMobil(t->data.idMobil);
            cout << i++ << ". " << t->data.penyewa.nama 
                 << " | Unit: " << (m ? m->nama : "?")
                 << " | Total: Rp " << t->data.total << endl; 
            t = t->next;
        }
    }
    
    // Hapus Data Terakhir (Soft Delete ke Sampah)
    void delLast() {
        if (!tail) return; 
        NodeRental* t = tail; 
        tongSampah.push(t->data); // Backup ke Stack
        
        if (head == tail) head = tail = NULL; 
        else { tail = tail->prev; tail->next = NULL; } 
        
        delete t; 
        cout << ">> Transaksi terakhir dihapus dan dipindah ke Tong Sampah.\n";
    }
    
    // Restore Data dari Sampah
    void restore() {
        RentalMobil d; 
        if (tongSampah.pop(d)) {
            add(d); 
            cout << ">> Data a.n " << d.penyewa.nama << " berhasil dikembalikan (Restore).\n";
        } else {
            cout << ">> Tong Sampah kosong.\n";
        }
    }
    
    // ==========================================
    // SAVE & LOAD TRANSAKSI (FORMAT LENGKAP)
    // ==========================================
    void save() {
        ofstream f("transaksi.txt");
        NodeRental* t = head;
        while (t) {
            // Format: ID|Paket|Durasi|Supir|Total|Tgl|NIK|Nama|Alamat|HPKontak
            f << t->data.idMobil << "|" 
              << t->data.paket << "|" 
              << t->data.durasi << "|" 
              << t->data.supir << "|"
              << t->data.total << "|" 
              << t->data.tgl << "|" 
              << t->data.penyewa.nik << "|" 
              << t->data.penyewa.nama << "|"
              << t->data.penyewa.alamat << "|" 
              << t->data.penyewa.noHpKontak << endl;
            t = t->next;
        } 
        f.close();
    }
    
    void load() {
        ifstream f("transaksi.txt"); 
        string l;
        if (!f.is_open()) return;
        
        while (getline(f, l)) {
            if (l.empty()) continue; 
            RentalMobil r;
            try {
                r.idMobil = stoi(ambilField(l, 0)); 
                r.paket = stoi(ambilField(l, 1));
                r.durasi = stol(ambilField(l, 2)); 
                r.supir = stoi(ambilField(l, 3));
                r.total = stoll(ambilField(l, 4)); 
                r.tgl = ambilField(l, 5);
                r.penyewa.nik = stoll(ambilField(l, 6)); 
                r.penyewa.nama = ambilField(l, 7);
                r.penyewa.alamat = ambilField(l, 8); 
                r.penyewa.noHpKontak = ambilField(l, 9);
            } catch (...) { continue; } 
            add(r);
        } 
        f.close();
        cout << "[SYSTEM] Riwayat Transaksi berhasil dimuat.\n";
    }
};

// ==========================================
// 4. FITUR BARU: PRIORITY QUEUE (ANTRIAN BERPRIORITAS)
// ==========================================
struct NodePriority {
    string nama;
    int tingkatPrioritas; // 1 = VVIP, 2 = VIP, 3 = Biasa
    NodePriority *next;
};

class PriorityQueue {
private:
    NodePriority *head;

public:
    PriorityQueue() { head = NULL; }

    // Enqueue berdasarkan prioritas (semakin kecil angkanya, semakin tinggi prioritasnya)
    void enqueue(string n, int p) {
        NodePriority *temp = new NodePriority;
        temp->nama = n;
        temp->tingkatPrioritas = p;
        temp->next = NULL;

        // Jika antrian kosong atau prioritas lebih tinggi dari head
        if (head == NULL || p < head->tingkatPrioritas) {
            temp->next = head;
            head = temp;
        } else {
            // Cari posisi yang tepat
            NodePriority *curr = head;
            while (curr->next != NULL && curr->next->tingkatPrioritas <= p) {
                curr = curr->next;
            }
            temp->next = curr->next;
            curr->next = temp;
        }
        cout << "[PRIORITY] " << n << " ditambahkan ke antrian dengan prioritas " << p << ".\n";
    }

    void dequeue() {
        if (head == NULL) {
            cout << "[PRIORITY] Antrian kosong.\n";
        } else {
            NodePriority *temp = head;
            cout << "[PRIORITY] Memanggil: " << temp->nama << " (Prio: " << temp->tingkatPrioritas << ")\n";
            head = head->next;
            delete temp;
        }
    }

    void display() {
        if (head == NULL) {
            cout << "[PRIORITY] Antrian kosong.\n";
        } else {
            NodePriority *ptr = head;
            cout << "\n=== DAFTAR ANTRIAN PRIORITAS ===\n";
            while (ptr != NULL) {
                cout << "Nama: " << ptr->nama << " | Level: " << ptr->tingkatPrioritas << "\n";
                ptr = ptr->next;
            }
        }
    }
};
PriorityQueue antrianPrioritas;

// ==========================================
// 5. FITUR BARU: LIST PROMO (DOUBLY LINKED LIST)
// ==========================================
struct PromoData {
    string namaPromo;
    int potongan;
    string kodePromo;
};

struct NodePromo {
    PromoData data;
    NodePromo *next;
    NodePromo *prev;
};

class PromoList {
private:
    NodePromo *head;
    NodePromo *tail;
    NodePromo *current; // Untuk navigasi saat view

public:
    PromoList() {
        head = NULL;
        tail = NULL;
        current = NULL;
        // Inisialisasi Promo Awal
        tambahPromo("DISKON MERDEKA", 17000, "MERDEKA17");
        tambahPromo("SUPER HEMAT", 50000, "HEMAT50");
        tambahPromo("AKHIR TAHUN", 100000, "YEAR END");
        current = head;
    }

    void tambahPromo(string nama, int pot, string kode) {
        NodePromo *baru = new NodePromo;
        baru->data.namaPromo = nama;
        baru->data.potongan = pot;
        baru->data.kodePromo = kode;
        baru->next = NULL;
        baru->prev = NULL;

        if (head == NULL) {
            head = tail = baru;
        } else {
            tail->next = baru;
            baru->prev = tail;
            tail = baru;
        }
        // cout << "[PROMO] Promo " << nama << " ditambahkan.\n";
    }

    // Fitur Navigasi Doubly Linked List (Next/Prev)
    void lihatPromo() {
        if (head == NULL) {
            cout << "Tidak ada promo aktif.\n";
            return;
        }

        if (current == NULL) current = head;

        char pilihan;
        do {
            cout << "\n--------------------------------\n";
            cout << "        PROMO SAAT INI          \n";
            cout << "--------------------------------\n";
            cout << "Nama     : " << current->data.namaPromo << endl;
            cout << "Potongan : Rp " << current->data.potongan << endl;
            cout << "Kode     : " << current->data.kodePromo << endl;
            cout << "--------------------------------\n";
            cout << "[N] Next Promo  |  [P] Previous Promo  |  [X] Keluar\n";
            cout << "Pilihan: "; cin >> pilihan;

            if (pilihan == 'n' || pilihan == 'N') {
                if (current->next != NULL) {
                    current = current->next;
                } else {
                    cout << ">> Ini adalah promo terakhir.\n";
                }
            } else if (pilihan == 'p' || pilihan == 'P') {
                if (current->prev != NULL) {
                    current = current->prev;
                } else {
                    cout << ">> Ini adalah promo pertama.\n";
                }
            }
        } while (pilihan != 'x' && pilihan != 'X');
    }
};
PromoList daftarPromo;

// ==========================================
// 6. ALGORITMA: BST & HUFFMAN
// ==========================================

// BST (Binary Search Tree) - Analisis Harga
struct NodeBST { 
    RentalMobil d; 
    NodeBST *l, *r; 
    NodeBST(RentalMobil x) : d(x), l(NULL), r(NULL) {}
};

class BST {
    NodeBST* root; 
    
    void ins(NodeBST*& n, RentalMobil d) {
        if (!n) n = new NodeBST(d); 
        else if (d.total < n->d.total) ins(n->l, d); 
        else ins(n->r, d);
    }
    
    void sh(NodeBST* n) {
        if (n) {
            sh(n->l); 
            cout << "Rp " << setw(10) << n->d.total << " - " << n->d.penyewa.nama << endl; 
            sh(n->r);
        }
    }
    
    void del(NodeBST* n) {
        if (n) {
            del(n->l); 
            del(n->r); 
            delete n;
        }
    }

public:
    BST() : root(NULL) {} 
    ~BST() { del(root); } 
    
    void build(SenaraiRental& l) {
        NodeRental* t = l.head; 
        while (t) {
            ins(root, t->data); 
            t = t->next;
        }
    } 
    
    void show() {
        cout << "\n--- ANALISIS HARGA (SORTED BY PRICE) ---\n";
        sh(root);
    }
};

// HUFFMAN (Analisis Frekuensi Karakter Nama)
void huffman(SenaraiRental& l) {
    int f[256] = {0}; 
    NodeRental* t = l.head; 
    
    while (t) {
        for (int i = 0; i < (int)t->data.penyewa.nama.length(); i++) {
            char c = t->data.penyewa.nama[i];
            f[(int)c]++;
        }
        t = t->next;
    }
    
    cout << "\n--- FREKUENSI KARAKTER NAMA PENYEWA (HUFFMAN BASE) ---\n"; 
    for (int i = 0; i < 256; i++) {
        if (f[i] > 0) cout << "'" << (char)i << "': " << f[i] << " | "; 
    }
    cout << endl;
}

#endif
