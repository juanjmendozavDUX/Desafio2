#ifndef ARREGLODINAMICOT_H
#define ARREGLODINAMICOT_H

#include <iostream>

template <class T>
class ArregloDinamico {
private:
    T* elementos;
    int cantidad;
    int capacidad;

    void ampliar() {
        // 1.crear nueva caja
        T* temporal = new T[capacidad * 2]();

        // 2.mudar los elementos
        for (int i = 0; i < cantidad; i++) {
            temporal[i] = elementos[i];
        }

        // 3.destruir la caja vieja
        delete[] elementos;

        // 4.renombrar la caja nueva y actualizar el limite
        elementos = temporal;
        capacidad = capacidad * 2;
    }

public:
    ArregloDinamico(int cap = 10) {
        capacidad = cap;
        cantidad = 0;
        elementos = new T[capacidad];
    }

    ~ArregloDinamico() {
        delete[] elementos;
    }

    int getCantidad() {
        return cantidad;
    }

    void agregar(T nuevoElemento) {
        if (cantidad == capacidad) {
            ampliar();
        }
        elementos[cantidad] = nuevoElemento;
        cantidad++;
    }

    // metodo para obtener un elemento en una posicion especifica
    T& obtener(int indice) {
        // aqui se podría agregar validacion para no salir del limite
        return elementos[indice];
    }
};

#endif // ARREGLODINAMICOT_H
