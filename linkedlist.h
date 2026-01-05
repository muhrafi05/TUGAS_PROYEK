#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "ktp.h"

class LinkedListKTP {
private:
    Node* head;

public:
    LinkedListKTP() {
        head = NULL;
    }

    bool isEmpty() {
        return head == NULL;
    }

    // Insert data user baru ke linked list
    void insert(KTP x) {
        Node* baru = new Node;
        baru->data = x;
        baru->next = NULL;

        if (head == NULL) {
            head = baru;
        } else {
            Node* temp = head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = baru;
        }
    }

    // ==========================================
    // SIMPAN SEMUA FIELD KTP KE FILE (users.txt)
    // ==========================================
    void simpanDataKeFile() {
        ofstream file("users.txt");
        Node* t = head;
        while (t != NULL) {
            // Menyimpan dengan format Delimiter Pipe '|' agar mudah diparsing
            file << t->data.nik << "|"
                 << t->data.nama << "|"
                 << t->data.tempat_lahir << "|"
                 << t->data.tgl_lahir.hari << "|"
                 << t->data.tgl_lahir.bulan << "|"
                 << t->data.tgl_lahir.tahun << "|"
                 << t->data.jenis_kelamin << "|"
                 << t->data.gol_darah << "|"
                 << t->data.alamat << "|"
                 << t->data.rt << "|"
                 << t->data.rw << "|"
                 << t->data.kel_desa << "|"
                 << t->data.kecamatan << "|"
                 << t->data.agama << "|"
                 << t->data.status_perkawinan << "|"
                 << t->data.pekerjaan << "|"
                 << t->data.kewarganegaraan << "|"
                 << t->data.berlaku_hingga << "|"
                 << t->data.umur << endl;
            
            t = t->next;
        }
        file.close();
        // cout << "[SYSTEM] Data User berhasil disimpan.\n";
    }

    // ==========================================
    // MUAT SEMUA FIELD KTP DARI FILE
    // ==========================================
    void muatDataDariFile() {
        ifstream file("users.txt");
        if (!file.is_open()) return;

        string baris;
        while (getline(file, baris)) {
            if (baris.empty()) continue;
            
            KTP k;
            // Blok try-catch untuk mencegah crash jika data di file korup/tidak sesuai format
            try {
                k.nik = stoll(ambilField(baris, 0));
                k.nama = ambilField(baris, 1);
                k.tempat_lahir = ambilField(baris, 2);
                k.tgl_lahir.hari = stoi(ambilField(baris, 3));
                k.tgl_lahir.bulan = stoi(ambilField(baris, 4));
                k.tgl_lahir.tahun = stoi(ambilField(baris, 5));
                k.jenis_kelamin = (stoi(ambilField(baris, 6)) == 1);
                k.gol_darah = ambilField(baris, 7);
                k.alamat = ambilField(baris, 8);
                k.rt = stoi(ambilField(baris, 9));
                k.rw = stoi(ambilField(baris, 10));
                k.kel_desa = ambilField(baris, 11);
                k.kecamatan = ambilField(baris, 12);
                k.agama = stoi(ambilField(baris, 13));
                k.status_perkawinan = (stoi(ambilField(baris, 14)) == 1);
                k.pekerjaan = ambilField(baris, 15);
                k.kewarganegaraan = (stoi(ambilField(baris, 16)) == 1);
                k.berlaku_hingga = stoi(ambilField(baris, 17));
                k.umur = stoi(ambilField(baris, 18));
            } catch (...) {
                // Jika error parsing, lewati baris ini dan lanjut ke baris berikutnya
                continue; 
            }

            insert(k);
        }
        file.close();
        cout << "[SYSTEM] Database Penduduk berhasil dimuat ke dalam sistem.\n";
    }

    // Fungsi Login: Mencari user berdasarkan NIK
    KTP* loginByNIK(long long nikTarget) {
        Node* temp = head;
        while (temp != NULL) {
            if (temp->data.nik == nikTarget) {
                return &(temp->data); 
            }
            temp = temp->next;
        }
        return NULL;
    }
};

#endif
