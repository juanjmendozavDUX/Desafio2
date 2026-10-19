#ifndef UDEABOOK_H
#define UDEABOOK_H

#include "Texto.h"
#include "ArregloDinamicoT.h"

class Usuario;
class MedidorRecursos;
class Fecha;

class UdeABook {
private:
    ArregloDinamico<Usuario*> usuarios;
    bool** matrizAmistad;
    Usuario* usuarioActivo;
    bool fotosAmigosActualizadas;
    MedidorRecursos* medidor;
    int amigosMostrados[3];
    int numMostrados;

    void quickSort(int, int);
    int seleccionarAmigos(bool);
    void imprimirMostrados();
    void terminarFuncionalidad(int);

public:
    UdeABook();
    ~UdeABook();
    void cargarDatos();
    void actualizarDatos();
    bool iniciarSesion(Texto&, Texto&);
    Usuario* buscarUsuario(Texto&);
    Usuario* buscarUsuario(int);
    void mostrarMejoresAmigos();
    bool actualizarMejoresAmigos();
    bool enviarSolicitud(Texto&);
    bool aceptarSolicitud(Texto&);
    void visitarPerfilAmigo(Texto&);
    void sugerirAmigos(int);
    int amigosEnComun(int, int);
    bool sonAmigos(int, int);
    bool enviarMensaje(Texto&, Texto&);
    void leerMensajesNuevos();
    void buscarMensajes(Fecha&, Fecha&);
    unsigned long calcularMemoria();
    void mostrarMenu();
};

#endif