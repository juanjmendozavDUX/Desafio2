#include "Fotos.h"
#include <iostream>

Foto::Foto() : fechaCaptura(), tamano(0), ruta(), likes(0) {}
Foto::Foto(Fecha& fecha, int tamanoFoto, Texto& rutaFoto)
    : fechaCaptura(fecha), tamano(tamanoFoto), ruta(rutaFoto), likes(0) {}
Foto::Foto(const Foto& otra)
    : fechaCaptura(otra.fechaCaptura), tamano(otra.tamano), ruta(otra.ruta), likes(otra.likes) {}
Foto::~Foto() {}

Foto& Foto::operator=(const Foto& otra) {
    if (this == &otra) return *this;
    fechaCaptura = otra.fechaCaptura;
    tamano = otra.tamano;
    ruta = otra.ruta;
    likes = otra.likes;
    return *this;
}

Texto Foto::getRuta() const { return ruta; }
const Fecha& Foto::getFechaCaptura() const { return fechaCaptura; }
int Foto::getTamano() const { return tamano; }
int Foto::getLikes() const { return likes; }
int Foto::getlikes() const { return getLikes(); }
void Foto::setLikes(int nuevosLikes) { likes = nuevosLikes < 0 ? 0 : nuevosLikes; }

void Foto::mostrarRuta() const {
    std::cout << "Ruta de la foto: " << ruta << std::endl;
    std::cout << "Fecha de captura: " << fechaCaptura << std::endl;
}

void Foto::darLike() { likes++; }
void Foto::quitarLike() { if (likes > 0) likes--; }
void Foto::darlike() { darLike(); }
void Foto::quitarlike() { quitarLike(); }
unsigned long Foto::calcularMemoria() const { return sizeof(*this) + ruta.calcularMemoria(); }
