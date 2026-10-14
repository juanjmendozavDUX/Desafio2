#ifndef MENSAJE_H
#define MENSAJE_H
#include <string>
#include "Fecha.h"

using namespace std;

class Usuario;

class Mensaje {
private:
    string texto;
    Fecha fechaEnvio;
    bool leido;
    Usuario *emisor;
    Usuario *receptor;

public:
    Mensaje(Usuario*, Usuario*, string&, Fecha&);
    Mensaje(const Mensaje&);
    ~Mensaje();
    bool estaLeido();
    void marcarLeido();
    void mostrar();
    bool enRango(Fecha&, Fecha&);
    bool operator<(const Mensaje&);
    unsigned long calcularMemoria();
};

#endif // MENSAJE_H
