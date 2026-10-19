#include "Album.h"

Album::Album() : nombre(), ruta(), privacidad('R'), fotos(), posActual(-1) {}
Album::Album(Texto& nombreAlbum, Texto& rutaAlbum, char privacidadAlbum)
    : nombre(nombreAlbum), ruta(rutaAlbum), privacidad(privacidadAlbum), fotos(), posActual(-1) {}
Album::Album(const Album& otro)
    : nombre(otro.nombre), ruta(otro.ruta), privacidad(otro.privacidad), fotos(otro.fotos), posActual(otro.posActual) {}
Album::~Album() {}

Album& Album::operator=(const Album& otro) {
    if (this == &otro) return *this;
    nombre = otro.nombre;
    ruta = otro.ruta;
    privacidad = otro.privacidad;
    fotos = otro.fotos;
    posActual = otro.posActual;
    return *this;
}

Texto Album::getNombre() const { return nombre; }
Texto Album::getRuta() const { return ruta; }
char Album::getPrivacidad() const { return privacidad; }
bool Album::visibleParaAmigas() const { return privacidad == 'A' || privacidad == 'P'; }
bool Album::visibleParaAmigos() const { return visibleParaAmigas(); }
int Album::getNumFotos() const { return fotos.getCantidad(); }
void Album::agregarFoto(const Foto& foto) { fotos.agregar(foto); }

Foto* Album::getFoto(int indice) {
    if (indice < 0 || indice >= getNumFotos()) return nullptr;
    return &fotos[indice];
}
const Foto* Album::getFoto(int indice) const {
    if (indice < 0 || indice >= getNumFotos()) return nullptr;
    return &fotos[indice];
}

void Album::reiniciarCarrusel() { posActual = -1; }

Foto* Album::siguienteFoto() {
    int cantidad = getNumFotos();
    if (cantidad == 0) return nullptr;
    posActual = (posActual + 1) % cantidad;
    return &fotos[posActual];
}

Foto* Album::fotoAnterior() {
    int cantidad = getNumFotos();
    if (cantidad == 0) return nullptr;
    posActual = (posActual - 1 + cantidad) % cantidad;
    return &fotos[posActual];
}

unsigned long Album::calcularMemoria() const {
    unsigned long memoria = sizeof(*this) + nombre.calcularMemoria() + ruta.calcularMemoria();
    memoria += fotos.calcularMemoria();
    for (int i = 0; i < fotos.getCantidad(); i++) memoria += fotos[i].calcularMemoria();
    return memoria;
}