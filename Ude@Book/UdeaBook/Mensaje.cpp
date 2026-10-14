#include "Mensaje.h"
#include <iostream>

Mensaje::Mensaje(Usuario* e, Usuario* r, string& t, Fecha& f) {
    emisor = e;
    receptor = r;
    texto = t;
    fechaEnvio = f;
    leido = false;
}

Mensaje::Mensaje(const Mensaje& otro) {
    emisor = otro.emisor;
    receptor = otro.receptor;
    texto = otro.texto;
    fechaEnvio = otro.fechaEnvio;
    leido = otro.leido;
}

Mensaje::~Mensaje() {
}

bool Mensaje::estaLeido(){
    return leido;
}

void Mensaje::marcarLeido(){
    leido = true;
}

void Mensaje::mostrar(){
        cout << "Fecha: " << fechaEnvio << "\n";
        cout << "Mensaje: " << texto << "\n";
}

bool Mensaje::enRango(Fecha& inicio, Fecha& fin) {
    return !(fechaEnvio < inicio) && !(fin < fechaEnvio);
}

bool Mensaje::operator<(const Mensaje& otro) {
    return fechaEnvio < otro.fechaEnvio;
}

unsigned long Mensaje::calcularMemoria() {
    return sizeof(*this) + texto.capacity();
}