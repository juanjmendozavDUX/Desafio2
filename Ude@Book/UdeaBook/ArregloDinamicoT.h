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
        int nuevaCapacidad = capacidad > 0 ? capacidad * 2 : 1;
        T* temporal = new T[nuevaCapacidad];
        for (int i = 0; i < cantidad; i++) temporal[i] = elementos[i];
        delete[] elementos;
        elementos = temporal;
        capacidad = nuevaCapacidad;
    }

public:
    ArregloDinamico(int cap = 4)
        : elementos(nullptr), cantidad(0), capacidad(cap > 0 ? cap : 1) {
        elementos = new T[capacidad];
    }

    ArregloDinamico(const ArregloDinamico& otro)
        : elementos(nullptr), cantidad(otro.cantidad), capacidad(otro.capacidad) {
        elementos = new T[capacidad];
        for (int i = 0; i < cantidad; i++) elementos[i] = otro.elementos[i];
    }

    ~ArregloDinamico() { delete[] elementos; }

    void agregar(const T& nuevoElemento) {
        if (cantidad == capacidad) ampliar();
        elementos[cantidad] = nuevoElemento;
        cantidad++;
    }

    void eliminar(int indice) {
        if (indice < 0 || indice >= cantidad) return;
        for (int i = indice; i < cantidad - 1; i++) elementos[i] = elementos[i + 1];
        cantidad--;
    }

    T& obtener(int indice) { return elementos[indice]; }
    const T& obtener(int indice) const { return elementos[indice]; }
    int getCantidad() const { return cantidad; }
    int getCapacidad() const { return capacidad; }
    unsigned long calcularMemoria() const {
        return static_cast<unsigned long>(capacidad) * sizeof(T);
    }

    T& operator[](int i) { return elementos[i]; }
    const T& operator[](int i) const { return elementos[i]; }

    ArregloDinamico& operator=(const ArregloDinamico& otro) {
        if (this == &otro) return *this;
        T* nuevo = new T[otro.capacidad];
        for (int i = 0; i < otro.cantidad; i++) nuevo[i] = otro.elementos[i];
        delete[] elementos;
        elementos = nuevo;
        cantidad = otro.cantidad;
        capacidad = otro.capacidad;
        return *this;
    }
};

#endif