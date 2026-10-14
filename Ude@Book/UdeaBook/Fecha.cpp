#include "Fecha.h"
#include <iostream>

// Constructor por defecto
Fecha::Fecha() : dia(1), mes(1), anio(2026) {}

// Constructor parametrizado
Fecha::Fecha(unsigned short int d, unsigned short int m, unsigned short int a) {
    // 1. Definir los días máximos de cada mes
    unsigned short int diasMes[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // 2. Verificar si es año bisiesto para cambiar febrero a 29 días
    if ((a % 400 == 0) || (a % 4 == 0 && a % 100 != 0)) {
        diasMes[1] = 29;
    }

    // 3. Validar que los datos tengan sentido matemático
    if (a < 1 || m < 1 || m > 12 || d < 1 || d > diasMes[m - 1]) {
        std::cout << "Error: Fecha invalida (" << d << "/" << m << "/" << a
                  << "). Asignando 1/1/2000 por defecto.\n";
        dia = 1;
        mes = 1;
        anio = 2000;
    } else {
        // Si todo es correcto, asignamos los valores reales
        dia = d;
        mes = m;
        anio = a;
    }
}

// Método para mostrar la fecha en consola
void Fecha::mostrar() {
    std::cout << dia << "/" << mes << "/" << anio << std::endl;
}

// 1. Operador de Igualdad (==)
// Dos fechas son iguales si coinciden en año, mes y día.
bool Fecha::operator == (const Fecha& otra) {
    return (this->anio == otra.anio &&
            this->mes == otra.mes &&
            this->dia == otra.dia);
}

// 2. Operador Menor Que (<)
// Compara cronológicamente de mayor a menor jerarquía: año, luego mes, luego día.
bool Fecha::operator < (const Fecha& otra) {
    if (this->anio != otra.anio) {
        return this->anio < otra.anio;
    }
    if (this->mes != otra.mes) {
        return this->mes < otra.mes;
    }
    return this->dia < otra.dia;
}

// 3. Operador Menor o Igual Que (<=)
// Reutiliza los operadores '<' y '==' que ya programamos arriba. ¡Es más eficiente!
bool Fecha::operator <= (const Fecha& otra) {
    return (*this < otra || *this == otra);
}

// 4. Operador de Flujo de Salida (<<) - Función Amiga (friend)
// Permite hacer cosas como: std::cout << miFecha;
std::ostream& operator << (std::ostream& os, const Fecha& f) {
    os << f.dia << "/" << f.mes << "/" << f.anio;
    return os;
}