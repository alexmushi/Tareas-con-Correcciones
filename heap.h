#ifndef HEAP_H_
#define HEAP_H_

#include <string>
#include <sstream>
#include "exception.h"
#include <iostream>

template <class T>
class Heap {

public:
	T *data;
	unsigned int tam;
	unsigned int cuenta;
	unsigned int count();
	unsigned int parent(unsigned int) const;
	unsigned int left(unsigned int) const;
	unsigned int right(unsigned int) const;
	void heapify(unsigned int);
	void swap(unsigned int, unsigned int);
	Heap(unsigned int) ;
	~Heap();
	bool empty() const;
	bool full() const;
	void push(T) ;
	void pop() ;
	int size();
	T top() ;
	void clear();
	std::string toString() const;
};

template <class T>
Heap<T>::Heap(unsigned int sze)  {
	tam = sze;
	data = new T[tam];
	if (data == 0) {
		throw OutOfMemory();
	}
	cuenta = 0;
}

template <class T>
unsigned int Heap<T>::count() {
    return cuenta;
}
template <class T>
Heap<T>::~Heap() {
	delete [] data;
	data = 0;
	tam = 0;
	cuenta = 0;
}


template <class T>
bool Heap<T>::empty() const {
	return (cuenta == 0);
}
template <class T>
bool Heap<T>::full() const {
	return (cuenta == tam);
}
template <class T>
unsigned int Heap<T>::parent(unsigned int pos) const{
	return (pos - 1) / 2;
}

template <class T>
unsigned int Heap<T>::left(unsigned int pos) const {
	return ((2 * pos) + 1);
}

template <class T>
unsigned int Heap<T>::right(unsigned int pos) const {
	return ((2 * pos) + 2);
}

template <class T>
void Heap<T>::swap(unsigned int i, unsigned int j) {
	T aux = data[i];
	data[i] = data[j];
	data[j] = aux;
}
template <class T>
void Heap<T>::heapify(unsigned int pos) {
	unsigned int le = left(pos);
	unsigned int ri = right(pos);
	unsigned int min = pos;
	if (le <= cuenta && data[le] < data[min]) {
		min = le;
	}
	if (ri <= cuenta && data[ri] < data[min]) {
		min = ri;
	}
	if (min != pos) {
		swap(pos, min);
		heapify(min);
	}
}

template <class T>
void Heap<T>::push(T val)  {
	unsigned int pos;
	if (full()) {
		throw Overflow();
	}

	pos = cuenta;
	cuenta++;
	while (pos > 0 && val < data[parent(pos)]) {
		data[pos] = data[parent(pos)];
		pos = parent(pos);
	}
	data[pos] = val;
}

template <class T>
void Heap<T>::pop()  {
	if (empty()) {
		throw NoSuchElement();
	}
	T aux = data[0];

	data[0] = data[--cuenta];
	heapify(0);
}

template <class T>
T Heap<T>::top()  {
	if (empty()) {
		throw NoSuchElement();
	}
	T aux = data[0];
	return aux;
}


template <class T>
void Heap<T>::clear() {
	cuenta = 0;
}

template <class T>
std::string Heap<T>::toString() const {
	std::stringstream aux;
	aux << "[";	for (unsigned int i = 0; i < cuenta; i++) {
		if (i != 0) {
			aux << " ";
		} aux << data[i];
	} aux << "]";
	return aux.str();
}

template <class T>
int Heap<T>::size() {
    return cuenta;
}


#endif /* HASH_H_ */
