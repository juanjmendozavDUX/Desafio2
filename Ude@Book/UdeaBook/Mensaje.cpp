#include "Mensaje.h"
#include <iostream>

Mensaje::Mensaje(Usuario* e, Usuario* r, Texto& t, Fecha& f)
    : texto(t), fechaEnvio(f), leido(false), emisor(e), receptor(r) {}
Mensaje::Mensaje(const Mensaje& otro)
    : texto(otro.texto), fechaEnvio(otro.fechaEnvio), leido(otro.leido),
    emisor(otro.emisor), receptor(otro.receptor) {}
Mensaje::~Mensaje() {}

bool Mensaje::estaLeido() const { return leido; }
void Mensaje::marcarLeido() { leido = true; }
void Mensaje::mostrar() const {
    std::cout << "Fecha: " << fechaEnvio << "\n";
    std::cout << "Mensaje: " << texto << "\n";
}
bool Mensaje::enRango(const Fecha& inicio, const Fecha& fin) const {
    return !(fechaEnvio < inicio) && !(fin < fechaEnvio);
}
bool Mensaje::operator<(const Mensaje& otro) const { return fechaEnvio < otro.fechaEnvio; }
const Texto& Mensaje::getTexto() const { return texto; }
const Fecha& Mensaje::getFechaEnvio() const { return fechaEnvio; }
Usuario* Mensaje::getEmisor() const { return emisor; }
Usuario* Mensaje::getReceptor() const { return receptor; }
unsigned long Mensaje::calcularMemoria() const { return sizeof(*this) + texto.calcularMemoria(); }