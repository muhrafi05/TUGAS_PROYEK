#ifndef STACKKTP_H
#define STACKKTP_H

#include "ktp.h"

class StackKTP {
private:
    KTP data[100];
    int top;

public:
    StackKTP() { top = -1; }

    bool isFull() { return top >= 99; }
    bool isEmpty() { return top == -1; }

    void push(KTP x) {
        if (isFull()) { 
            cout << "Stack Penuh (Max 100)!\n"; 
            return; 
        }
        data[++top] = x;
    }

    void pop(KTP &x) {
        if (isEmpty()) { 
            cout << "Stack Kosong!\n"; 
            return; 
        }
        x = data[top--];
    }

    void display() {
        if (isEmpty()) { 
            cout << "Stack KTP Kosong.\n"; 
            return; 
        }
        cout << "\n==== TUMPUKAN DATA KTP (LIFO: LAST IN FIRST OUT) ====\n";
        cout << "Data paling atas (terakhir masuk) akan ditampilkan pertama:\n";
        for (int i = top; i >= 0; i--) {
            // Memanfaatkan operator<< dari ktp.h yang sudah lengkap
            cout << data[i]; 
        }
        cout << "\n======================================================\n";
    }
};

#endif
