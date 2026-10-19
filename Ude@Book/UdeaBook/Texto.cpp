#include "Texto.h"
#include <iostream>

Texto::Texto() : datos(nullptr), longitud(0), capacidad(8) {
    datos = new char[capacidad];
}

Texto::Texto(const char* texto) : datos(nullptr), longitud(0), capacidad(8) {
    datos = new char[capacidad];
    if (texto != nullptr) {
        int i = 0;
        while (texto[i] != '\0') {
            agregar(texto[i]);
            i++;
        }
    }
}

Texto::Texto(const Texto& otro)
    : datos(nullptr), longitud(otro.longitud), capacidad(otro.capacidad) {
    datos = new char[capacidad];
    for (int i = 0; i < longitud; i++) datos[i] = otro.datos[i];
}

Texto::~Texto() {
    delete[] datos;
}

void Texto::reservar(int nuevaCapacidad) {
    if (nuevaCapacidad <= capacidad) return;
    char* nuevos = new char[nuevaCapacidad];
    for (int i = 0; i < longitud; i++) nuevos[i] = datos[i];
    delete[] datos;
    datos = nuevos;
    capacidad = nuevaCapacidad;
}

Texto& Texto::operator=(const Texto& otro) {
    if (this == &otro) return *this;
    char* nuevos = new char[otro.capacidad];
    for (int i = 0; i < otro.longitud; i++) nuevos[i] = otro.datos[i];
    delete[] datos;
    datos = nuevos;
    longitud = otro.longitud;
    capacidad = otro.capacidad;
    return *this;
}

Texto& Texto::operator=(const char* texto) {
    limpiar();
    if (texto != nullptr) {
        int i = 0;
        while (texto[i] != '\0') {
            agregar(texto[i]);
            i++;
        }
    }
    return *this;
}

void Texto::limpiar() {
    longitud = 0;
}

void Texto::agregar(char caracter) {
    if (longitud == capacidad) reservar(capacidad * 2);
    datos[longitud] = caracter;
    longitud++;
}

int Texto::largo() const {
    return longitud;
}

bool Texto::vacio() const {
    return longitud == 0;
}

char Texto::en(int posicion) const {
    if (posicion < 0 || posicion >= longitud) return '\0';
    return datos[posicion];
}

unsigned long Texto::calcularMemoria() const {
    return static_cast<unsigned long>(capacidad * sizeof(char));
}

bool Texto::operator==(const Texto& otro) const {
    if (longitud != otro.longitud) return false;
    for (int i = 0; i < longitud; i++) {
        if (datos[i] != otro.datos[i]) return false;
    }
    return true;
}

bool Texto::operator!=(const Texto& otro) const {
    return !(*this == otro);
}

bool Texto::operator<(const Texto& otro) const {
    int limite = longitud < otro.longitud ? longitud : otro.longitud;
    for (int i = 0; i < limite; i++) {
        if (datos[i] < otro.datos[i]) return true;
        if (datos[i] > otro.datos[i]) return false;
    }
    return longitud < otro.longitud;
}

bool Texto::leerHasta(std::istream& entrada, char separador, Texto& resultado) {
    resultado.limpiar();
    char caracter;
    bool leyo = false;
    while (entrada.get(caracter)) {
        if (caracter == '\n' && !leyo) continue;
        leyo = true;
        if (caracter == separador) return true;
        if (caracter != '\r') resultado.agregar(caracter);
    }
    return leyo;
}

bool Texto::leerLinea(std::istream& entrada, Texto& resultado) {
    resultado.limpiar();
    char caracter;
    bool leyo = false;
    while (entrada.get(caracter)) {
        leyo = true;
        if (caracter == '\n') return true;
        if (caracter != '\r') resultado.agregar(caracter);
    }
    return leyo;
}

std::ostream& operator<<(std::ostream& salida, const Texto& texto) {
    for (int i = 0; i < texto.longitud; i++) salida << texto.datos[i];
    return salida;
}