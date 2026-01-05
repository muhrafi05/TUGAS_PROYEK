#ifndef ARRAY1D_H
#define ARRAY1D_H

#include "ktp.h"

class ArrayKTP {
private:
    KTP data[100];
    int size;

public:
    ArrayKTP() {
        size = 0;
    }

    bool isFull() { return size >= 100; }
    bool isEmpty() { return size == 0; }

    void insert(KTP x) {
        if (isFull()) {
            cout << "Array Penuh!\n";
            return;
        }
        data[size] = x;
        size++;
    }

    void display() {
        if (isEmpty()) {
            cout << "Array Kosong.\n";
            return;
        }

        cout << "\n==== DATA KTP (Array Linear) ====\n";
        for (int i = 0; i < size; i++) {
            cout << "\n[Data Index ke-" << i << "]";
            cout << data[i]; 
        }
    }
};

#endif
