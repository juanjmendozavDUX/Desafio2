#ifndef FECHA_H
#define FECHA_H

#include <iostream>

class Fecha {
public:
    Fecha();
    Fecha(unsigned short int, unsigned short int, unsigned short int);
    void mostrar() const;
    void guardar(std::ostream&) const;
    unsigned short int getDia() const;
    unsigned short int getMes() const;
    unsigned short int getAnio() const;
    bool operator==(const Fecha&) const;
    bool operator<(const Fecha&) const;
    bool operator<=(const Fecha&) const;
    friend std::ostream& operator<<(std::ostream&, const Fecha&);
private:
    unsigned short int dia;
    unsigned short int mes;
    unsigned short int anio;
};

#endif