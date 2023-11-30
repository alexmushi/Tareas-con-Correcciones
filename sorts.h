/*
 *  sorts.h
 *
 *  Created on: 7/09/2023
 *  Author: Alejandro Muñoz Shimano A01705550
 */

#include <iostream>
#include <vector>

using namespace std;

template <class T>
class Sorts {
private:
	void swap(std::vector<T>&, int, int);
	void copyArray(std::vector<T>&, std::vector<T>&, int, int);
	void mergeArray(std::vector<T>&, std::vector<T>&, int, int, int);
	void mergeSplit(std::vector<T>&, std::vector<T>&, int, int);


public:
    vector<T> ordenaSeleccion(vector<T> &);
    vector<T> ordenaBurbuja(vector<T>& arr);
    vector<T> ordenaMerge(vector<T>& arr);
    int busqSecuencial(const vector<T>& arr, T dato);
    int busqBinaria(const vector<T>& arr, T dato);
};

template <class T>
void Sorts<T>::swap(std::vector<T> &v, int i, int j) {
	T aux = v[i];
	v[i] = v[j];
	v[j] = aux;
}

template <class T>
void Sorts<T>::copyArray(std::vector<T> &A, std::vector<T> &B, int low, int high) {
	for (int i = low; i <= high; i++) {
		A[i] = B[i];
	}
}

template <class T>
void Sorts<T>::mergeArray(std::vector<T> &A, std::vector<T> &B, int low, int mid, int high) {
	int i, j, k;

	i = low;
	j = mid + 1;
	k = low;

	while (i <= mid &&j <= high) {
		if (A[i] < A[j]) {
			B[k] = A[i];
			i++;
		} else {
			B[k] = A[j];
			j++;
		}
		k++;
	}
	if (i > mid) {
		for (; j <= high; j++) {
			B[k++] = A[j];
		}
	} else {
		for (; i <= mid; i++) {
			B[k++] = A[i];
		}
	}
}

template <class T>
void Sorts<T>::mergeSplit(std::vector<T> &A, std::vector<T> &B, int low, int high) {
	int mid;

	if ( (high - low) < 1 ) {
		return;
	}
	mid = (high + low) / 2;
	mergeSplit(A, B, low, mid);
	mergeSplit(A, B, mid + 1, high);
	mergeArray(A, B, low, mid, high);
	copyArray(A, B, low, high);
}

template <class T>
vector<T> Sorts<T>::ordenaSeleccion(vector<T>&source){
	int pos;

	for (int i = source.size() - 1; i > 0; i--) {
		pos = 0;
		for (int j = 1; j <= i; j++) {
			if (source[j] > source[pos]) {
				pos = j;
			}
		}

		if (pos != i) {
			swap(source, i, pos);
		}
	}
	return source;
}

template <class T>
vector<T> Sorts<T>::ordenaBurbuja(vector<T>&source){

	for (int i = source.size() - 1; i > 0; i--) {
		for (int j = 0; j < i; j++) {
			if (source[j] > source[j + 1]) {
				swap(source, j, j + 1);
			}
		}
	}
	return source;
}

template <class T>
vector<T> Sorts<T>::ordenaMerge(vector<T>&source){
    std::vector<T> tmp(source.size());

	mergeSplit(source, tmp, 0, source.size() - 1);
	return source;
}

template <class T>
int Sorts<T>::busqSecuencial(const vector<T>& arr, T dato){
    int i = 0;

    while (i <= arr.size()) {
        if (dato == arr[i]) {
            return i;
        }
        i++;
    }

    return -1;
}

template <class T>
int Sorts<T>::busqBinaria(const vector<T>& arr, T dato){
    int mid;
	int low = 0;
	int high = arr.size() - 1;

	while (low <= high) {
		mid = (high + low) / 2;
		if (dato == arr[mid]) {
			return mid;
		} else if (dato < arr[mid]) {
			high = mid - 1;
		} else if (dato > arr[mid]) {
			low = mid + 1;
		}
	}
	return -1;
}
