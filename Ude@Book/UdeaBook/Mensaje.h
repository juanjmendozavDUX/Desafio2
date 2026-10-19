#ifndef MENSAJE_H
#define MENSAJE_H

#include <iostream>
#include "Fecha.h"
#include "Texto.h"

class Usuario;

class Mensaje {
private:
    Texto texto;
    Fecha fechaEnvio;
    bool leido;
    Usuario* emisor;
    Usuario* receptor;
public:
    Mensaje(Usuario*, Usuario*, Texto&, Fecha&);
    Mensaje(const Mensaje&);
    ~Mensaje();
    bool estaLeido() const;
    void marcarLeido();
    void mostrar() const;
    bool enRango(const Fecha&, const Fecha&) const;
    bool operator<(const Mensaje&) const;
    const Texto& getTexto() const;
    const Fecha& getFechaEnvio() const;
    Usuario* getEmisor() const;
    Usuario* getReceptor() const;
    unsigned long calcularMemoria() const;
};

#endif