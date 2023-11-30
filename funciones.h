/*
 *  funciones.h
 *
 *  Created on: 23/08/2023
 *  Author: Alejandro Muñoz Shimano A01705550
 */

#include <iostream>
using namespace std;

class Funciones {
public:
    // Función para calcular la sumatoria de 1 hasta n de forma iterativa
    int sumaIterativa(int n) {
        int suma = 0;
        if (n == 0) {
            return 0;
        }
        else {
        for (int i = 1; i <= n; i++) {
            suma += i;
        }
        return suma;
        }
    }

    // Función para calcular la sumatoria de 1 hasta n de forma recursiva
    int sumaRecursiva(int n) {
        if (n == 0) {
            return 0;
        }
        else if (n == 1) {
            return 1;
        }
        else {
            return n + sumaRecursiva(n - 1);
        }
    }

    // Función para calcular la sumatoria de 1 hasta n de forma directa
    int sumaDirecta(int n) {
        return (n * (n + 1)) / 2;
    }
};
