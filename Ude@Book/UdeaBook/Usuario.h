#ifndef USUARIO_H
#define USUARIO_H

#include <iostream>
#include "Texto.h"
#include "Fecha.h"
#include "Album.h"
#include "ArregloDinamicoT.h"

class Mensaje;

class Usuario {
private:
    int indice;
    Texto nombreUsuario;
    Fecha fechaNacimiento;
    Texto correo;
    Texto rutaFotoPerfil;
    Texto contrasena;
    Texto ciudad;
    Texto pais;
    Fecha fechaInscripcion;
    ArregloDinamico<Album> albumes;
    ArregloDinamico<Mensaje*> mensajesEnviados;
    ArregloDinamico<Mensaje*> mensajesRecibidos;
    ArregloDinamico<Usuario*> solicitudes;
    bool duenoMensajesEnviados;

    void liberarMensajesEnviados();
    void insertarMensajeOrdenado(ArregloDinamico<Mensaje*>& arreglo, Mensaje* mensaje);

public:
    Usuario(int, Texto&, Fecha&, Texto&, Texto&, Texto&, Texto&, Texto&, Fecha&);
    Usuario(const Usuario&);
    ~Usuario();

    int getIndice() const;
    const Texto& getNombreUsuario() const;
    const Texto& getRutaFotoPerfil() const;
    void setIndice(int);
    void setRutaFotoPerfil(Texto&);
    bool validarContrasena(Texto&) const;

    void agregarAlbum(const Album&);
    Album& getAlbum(int);
    const Album& getAlbum(int) const;
    int getNumAlbumes() const;
    void listarAlbumesVisibles() const;

    void agregarMensajeEnviado(Mensaje*);
    void agregarMensajeRecibido(Mensaje*);
    void leerMensajesNuevos();
    void buscarMensajes(const Fecha&, const Fecha&);

    void agregarSolicitud(Usuario*);
    bool tieneSolicitudDe(Usuario*) const;
    void quitarSolicitud(Usuario*);
    int mostrarSolicitudes() const;

    void guardarSolicitudes(std::ostream&) const;
    void guardarMensajes(std::ostream&) const;
    void guardarAlbumes(std::ostream&, std::ostream&) const;

    unsigned long calcularMemoria() const;
    bool operator==(const Usuario&) const;
    Usuario& operator=(const Usuario&);
    friend std::ostream& operator<<(std::ostream&, const Usuario&);
};

#endif