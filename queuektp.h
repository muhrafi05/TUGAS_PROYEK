#ifndef QUEUEKTP_H
#define QUEUEKTP_H

#include "ktp.h"

class QueueKTP {
private:
    Node* front;
    Node* rear;

public:
    QueueKTP() { 
        front = rear = NULL; 
    }

    bool isEmpty() { return front == NULL; }

    void enqueue(KTP x) {
        Node* baru = new Node;
        baru->data = x;
        baru->next = NULL;
        
        if (isEmpty()) {
            front = rear = baru;
        } else {
            rear->next = baru;
            rear = baru;
        }
    }

    bool dequeue() {
        if (isEmpty()) { 
            cout << "Antrian Kosong!\n"; 
            return false; 
        }
        Node* temp = front;
        front = front->next;
        if (front == NULL) rear = NULL;
        delete temp;
        return true;
    }

    void display() {
        if (isEmpty()) { 
            cout << "Antrian KTP Kosong.\n"; 
            return; 
        }
        cout << "\n==== ANTRIAN DATA KTP (FIFO: FIRST IN FIRST OUT) ====\n";
        cout << "Data paling depan (pertama masuk) akan ditampilkan pertama:\n";
        
        Node* temp = front;
        int i = 1;
        while (temp != NULL) {
            cout << "\n[Antrian No. " << i++ << "]";
            cout << temp->data; // Menggunakan format lengkap dari ktp.h
            temp = temp->next;
        }
        cout << "\n=====================================================\n";
    }
};

#endif
