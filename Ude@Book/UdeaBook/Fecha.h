#ifndef FECHA_H
#define FECHA_H

#include <iostream> // Necesario para ostream

class Fecha {
public:
    Fecha();
    Fecha(unsigned short int, unsigned short int, unsigned short int);
    void mostrar();

    bool operator == (const Fecha&);
    bool operator < (const Fecha&);
    bool operator <= (const Fecha&);

    // Al ser friend, lleva std:: antes de ostream si no usas 'using namespace std'
    friend std::ostream& operator << (std::ostream&, const Fecha&);

private:
    unsigned short int dia;
    unsigned short int mes;
    unsigned short int anio;
};

#endif
