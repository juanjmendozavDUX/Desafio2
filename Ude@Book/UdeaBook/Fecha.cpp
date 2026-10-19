#include "Fecha.h"
#include <iostream>

Fecha::Fecha() : dia(1), mes(1), anio(2026) {}

Fecha::Fecha(unsigned short int d, unsigned short int m, unsigned short int a) {
    unsigned short int diasMes[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((a % 400 == 0) || (a % 4 == 0 && a % 100 != 0)) diasMes[1] = 29;
    if (a < 1 || m < 1 || m > 12 || d < 1 || d > diasMes[m - 1]) {
        dia = 1;
        mes = 1;
        anio = 2000;
    } else {
        dia = d;
        mes = m;
        anio = a;
    }
}

void Fecha::mostrar() const { std::cout << *this << std::endl; }
void Fecha::guardar(std::ostream& salida) const { salida << dia << '-' << mes << '-' << anio; }
unsigned short int Fecha::getDia() const { return dia; }
unsigned short int Fecha::getMes() const { return mes; }
unsigned short int Fecha::getAnio() const { return anio; }

bool Fecha::operator==(const Fecha& otra) const {
    return anio == otra.anio && mes == otra.mes && dia == otra.dia;
}

bool Fecha::operator<(const Fecha& otra) const {
    if (anio != otra.anio) return anio < otra.anio;
    if (mes != otra.mes) return mes < otra.mes;
    return dia < otra.dia;
}

bool Fecha::operator<=(const Fecha& otra) const { return *this < otra || *this == otra; }

std::ostream& operator<<(std::ostream& salida, const Fecha& fecha) {
    salida << fecha.dia << '/' << fecha.mes << '/' << fecha.anio;
    return salida;
}