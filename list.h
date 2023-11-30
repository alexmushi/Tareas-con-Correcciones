#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <sstream>

template <class T>
class List {
private:
    template <class S>
    class Link {
    public:
        S value;
        Link* next;

        Link(S val, Link* nxt = 0) : value(val), next(nxt) {}
    };

    Link<T>* head;

public:
    List() : head(0) {}

    void insertion(const T& element) {
    Link<T>* newNode = new Link<T>(element, 0);

    if (head == 0) {
        head = newNode;
    } else {
        Link<T>* current = head;
        while (current->next != 0) {
            current = current->next;
        }
        current->next = newNode;
    }
}


    int search(const T& element) {
        Link<T>* current = head;
        int position = 0;

        while (current != 0) {
            if (current->value == element) {
                return position;
            }
            current = current->next;
            position++;
        }

        return -1;
    }

    void update(int position, const T& newValue) {
        Link<T>* current = head;
        int currentPosition = 0;

        while (current != 0 && currentPosition < position) {
            current = current->next;
            currentPosition++;
        }

        if (current != 0) {
            current->value = newValue;
        }
    }

    T deleteAt(int position) {

        if (position == 0) {
            Link<T>* temp = head;
            head = head->next;
            T deletedValue = temp->value;
            delete temp;
            return deletedValue;
        }

        Link<T>* current = head;
        int currentPosition = 0;

        while (current->next != 0 && currentPosition < position - 1) {
            current = current->next;
            currentPosition++;
        }


        Link<T>* temp = current->next;
        current->next = temp->next;
        T deletedValue = temp->value;
        delete temp;
        return deletedValue;
    }

    std::string toString() const {
        std::stringstream aux;
        Link<T>* p;
        p = head;

        aux << "[";

        while (p != 0) {
            aux << p->value;
            if (p->next != 0) {
                aux << ", ";
            }
            p = p->next;
        }

        aux << "]";

        return aux.str();
    }

    ~List() {
        while (head != 0) {
            Link<T>* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

#endif

