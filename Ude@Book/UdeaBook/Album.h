#ifndef ALBUM_H
#define ALBUM_H

#include "Fotos.h"
#include "ArregloDinamicoT.h"

class Album {
public:
    Album();
    Album(Texto&, Texto&, char);
    Album(const Album&);
    Album& operator=(const Album&);
    ~Album();
    Texto getNombre() const;
    Texto getRuta() const;
    char getPrivacidad() const;
    bool visibleParaAmigas() const;
    bool visibleParaAmigos() const;
    int getNumFotos() const;
    void agregarFoto(const Foto&);
    Foto* getFoto(int);
    const Foto* getFoto(int) const;
    Foto* siguienteFoto();
    Foto* fotoAnterior();
    void reiniciarCarrusel();
    unsigned long calcularMemoria() const;
private:
    Texto nombre;
    Texto ruta;
    char privacidad;
    ArregloDinamico<Foto> fotos;
    int posActual;
};

#endif