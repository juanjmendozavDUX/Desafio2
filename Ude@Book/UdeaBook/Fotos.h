#ifndef FOTOS_H
#define FOTOS_H

#include "Fecha.h"
#include "Texto.h"

class Foto {
public:
    Foto();
    Foto(Fecha&, int, Texto&);
    Foto(const Foto&);
    Foto& operator=(const Foto&);
    ~Foto();
    Texto getRuta() const;
    const Fecha& getFechaCaptura() const;
    int getTamano() const;
    int getLikes() const;
    int getlikes() const;
    void setLikes(int);
    void mostrarRuta() const;
    void darLike();
    void quitarLike();
    void darlike();
    void quitarlike();
    unsigned long calcularMemoria() const;
private:
    Fecha fechaCaptura;
    int tamano;
    Texto ruta;
    int likes;
};

#endif