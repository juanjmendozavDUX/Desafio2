#include "Usuario.h"
#include "Mensaje.h"
#include <iostream>

Usuario::Usuario(int indiceUsuario, Texto& nombre, Fecha& nacimiento, Texto& correoUsuario,
                 Texto& rutaFoto, Texto& clave, Texto& ciudadUsuario, Texto& paisUsuario,
                 Fecha& inscripcion)
    : indice(indiceUsuario), nombreUsuario(nombre), fechaNacimiento(nacimiento),
    correo(correoUsuario), rutaFotoPerfil(rutaFoto), contrasena(clave),
    ciudad(ciudadUsuario), pais(paisUsuario), fechaInscripcion(inscripcion),
    albumes(), mensajesEnviados(), mensajesRecibidos(), solicitudes(),
    duenoMensajesEnviados(true) {}

Usuario::Usuario(const Usuario& otro)
    : indice(otro.indice), nombreUsuario(otro.nombreUsuario),
    fechaNacimiento(otro.fechaNacimiento), correo(otro.correo),
    rutaFotoPerfil(otro.rutaFotoPerfil), contrasena(otro.contrasena),
    ciudad(otro.ciudad), pais(otro.pais), fechaInscripcion(otro.fechaInscripcion),
    albumes(otro.albumes), mensajesEnviados(otro.mensajesEnviados),
    mensajesRecibidos(otro.mensajesRecibidos), solicitudes(otro.solicitudes),
    duenoMensajesEnviados(false) {}

Usuario::~Usuario() { liberarMensajesEnviados(); }

void Usuario::liberarMensajesEnviados() {
    if (!duenoMensajesEnviados) return;
    for (int i = 0; i < mensajesEnviados.getCantidad(); i++) delete mensajesEnviados[i];
}

int Usuario::getIndice() const { return indice; }
const Texto& Usuario::getNombreUsuario() const { return nombreUsuario; }
const Texto& Usuario::getRutaFotoPerfil() const { return rutaFotoPerfil; }
void Usuario::setIndice(int nuevoIndice) { indice = nuevoIndice; }
void Usuario::setRutaFotoPerfil(Texto& nuevaRuta) { rutaFotoPerfil = nuevaRuta; }
bool Usuario::validarContrasena(Texto& clave) const { return contrasena == clave; }

void Usuario::agregarAlbum(const Album& album) { albumes.agregar(album); }

Album& Usuario::getAlbum(int posicion) { return albumes[posicion]; }
const Album& Usuario::getAlbum(int posicion) const { return albumes[posicion]; }
int Usuario::getNumAlbumes() const { return albumes.getCantidad(); }

void Usuario::listarAlbumesVisibles() const {
    std::cout << "--- Albumes visibles de " << nombreUsuario << " ---\n";
    int visibles = 0;
    for (int i = 0; i < albumes.getCantidad(); i++) {
        if (!albumes[i].visibleParaAmigas()) continue;
        std::cout << "  " << (i + 1) << ". " << albumes[i].getNombre()
                  << " (" << albumes[i].getNumFotos() << " fotos, privacidad "
                  << albumes[i].getPrivacidad() << ")\n";
        visibles++;
    }
    if (visibles == 0) std::cout << "  No hay albumes publicos o para amigos.\n";
}

void Usuario::insertarMensajeOrdenado(ArregloDinamico<Mensaje*>& arreglo, Mensaje* mensaje) {
    int posicion = 0;
    while (posicion < arreglo.getCantidad() && !(mensaje->operator<(*arreglo[posicion]))) posicion++;
    arreglo.agregar(mensaje);
    for (int i = arreglo.getCantidad() - 1; i > posicion; i--) arreglo[i] = arreglo[i - 1];
    arreglo[posicion] = mensaje;
}

void Usuario::agregarMensajeEnviado(Mensaje* mensaje) {
    if (mensaje != nullptr) insertarMensajeOrdenado(mensajesEnviados, mensaje);
}

void Usuario::agregarMensajeRecibido(Mensaje* mensaje) {
    if (mensaje != nullptr) insertarMensajeOrdenado(mensajesRecibidos, mensaje);
}

void Usuario::leerMensajesNuevos() {
    int nuevos = 0;
    for (int i = 0; i < mensajesRecibidos.getCantidad(); i++) {
        Mensaje* mensaje = mensajesRecibidos[i];
        if (mensaje->estaLeido()) continue;
        Usuario* emisor = mensaje->getEmisor();
        std::cout << "\nDe: " << (emisor == nullptr ? Texto("desconocido") : emisor->getNombreUsuario()) << "\n";
        mensaje->mostrar();
        mensaje->marcarLeido();
        nuevos++;
    }
    if (nuevos == 0) std::cout << "No tienes mensajes nuevos.\n";
}

void Usuario::buscarMensajes(const Fecha& inicio, const Fecha& fin) {
    int encontrados = 0;
    for (int i = 0; i < mensajesRecibidos.getCantidad(); i++) {
        Mensaje* mensaje = mensajesRecibidos[i];
        if (!mensaje->enRango(inicio, fin)) continue;
        Usuario* emisor = mensaje->getEmisor();
        std::cout << "\nDe: " << (emisor == nullptr ? Texto("desconocido") : emisor->getNombreUsuario()) << "\n";
        mensaje->mostrar();
        encontrados++;
    }
    if (encontrados == 0) std::cout << "No hay mensajes en ese rango.\n";
}

void Usuario::agregarSolicitud(Usuario* usuario) {
    if (usuario != nullptr && !tieneSolicitudDe(usuario)) solicitudes.agregar(usuario);
}

bool Usuario::tieneSolicitudDe(Usuario* usuario) const {
    if (usuario == nullptr) return false;
    for (int i = 0; i < solicitudes.getCantidad(); i++) {
        if (solicitudes[i] == usuario || *solicitudes[i] == *usuario) return true;
    }
    return false;
}

void Usuario::quitarSolicitud(Usuario* usuario) {
    if (usuario == nullptr) return;
    for (int i = 0; i < solicitudes.getCantidad(); i++) {
        if (solicitudes[i] == usuario || *solicitudes[i] == *usuario) {
            solicitudes.eliminar(i);
            return;
        }
    }
}

int Usuario::mostrarSolicitudes() const {
    std::cout << "--- Solicitudes recibidas ---\n";
    for (int i = 0; i < solicitudes.getCantidad(); i++) {
        std::cout << "  " << (i + 1) << ". " << solicitudes[i]->getNombreUsuario() << "\n";
    }
    return solicitudes.getCantidad();
}

void Usuario::guardarSolicitudes(std::ostream& salida) const {
    for (int i = 0; i < solicitudes.getCantidad(); i++) {
        salida << solicitudes[i]->getNombreUsuario() << '|' << nombreUsuario << '\n';
    }
}

void Usuario::guardarMensajes(std::ostream& salida) const {
    for (int i = 0; i < mensajesEnviados.getCantidad(); i++) {
        Mensaje* mensaje = mensajesEnviados[i];
        if (mensaje->getEmisor() == nullptr || mensaje->getReceptor() == nullptr) continue;
        salida << mensaje->getEmisor()->getNombreUsuario() << '|'
               << mensaje->getReceptor()->getNombreUsuario() << '|'
               << mensaje->getTexto() << '|'
               << mensaje->getFechaEnvio().getDia() << '|'
               << mensaje->getFechaEnvio().getMes() << '|'
               << mensaje->getFechaEnvio().getAnio() << '|'
               << (mensaje->estaLeido() ? 1 : 0) << '\n';
    }
}

void Usuario::guardarAlbumes(std::ostream& salidaAlbumes, std::ostream& salidaFotos) const {
    for (int i = 0; i < albumes.getCantidad(); i++) {
        const Album& album = albumes[i];
        salidaAlbumes << nombreUsuario << '|' << album.getNombre() << '|'
                      << album.getRuta() << '|' << album.getPrivacidad() << '\n';
        for (int j = 0; j < album.getNumFotos(); j++) {
            const Foto* foto = album.getFoto(j);
            salidaFotos << nombreUsuario << '|' << album.getNombre() << '|'
                        << foto->getRuta() << '|' << foto->getTamano() << '|'
                        << foto->getFechaCaptura().getDia() << '|'
                        << foto->getFechaCaptura().getMes() << '|'
                        << foto->getFechaCaptura().getAnio() << '|'
                        << foto->getLikes() << '\n';
        }
    }
}

unsigned long Usuario::calcularMemoria() const {
    unsigned long total = sizeof(*this);
    total += nombreUsuario.calcularMemoria() + correo.calcularMemoria();
    total += rutaFotoPerfil.calcularMemoria() + contrasena.calcularMemoria();
    total += ciudad.calcularMemoria() + pais.calcularMemoria();
    total += albumes.calcularMemoria();
    for (int i = 0; i < albumes.getCantidad(); i++) total += albumes[i].calcularMemoria();
    total += (static_cast<unsigned long>(mensajesEnviados.getCapacidad()) +
              mensajesRecibidos.getCapacidad()) * sizeof(Mensaje*);
    total += solicitudes.calcularMemoria();
    for (int i = 0; i < mensajesEnviados.getCantidad(); i++) total += mensajesEnviados[i]->calcularMemoria();
    return total;
}

bool Usuario::operator==(const Usuario& otro) const {
    return nombreUsuario == otro.nombreUsuario;
}

Usuario& Usuario::operator=(const Usuario& otro) {
    if (this == &otro) return *this;
    liberarMensajesEnviados();
    indice = otro.indice;
    nombreUsuario = otro.nombreUsuario;
    fechaNacimiento = otro.fechaNacimiento;
    correo = otro.correo;
    rutaFotoPerfil = otro.rutaFotoPerfil;
    contrasena = otro.contrasena;
    ciudad = otro.ciudad;
    pais = otro.pais;
    fechaInscripcion = otro.fechaInscripcion;
    albumes = otro.albumes;
    mensajesEnviados = otro.mensajesEnviados;
    mensajesRecibidos = otro.mensajesRecibidos;
    solicitudes = otro.solicitudes;
    duenoMensajesEnviados = false;
    return *this;
}

std::ostream& operator<<(std::ostream& salida, const Usuario& usuario) {
    salida << usuario.nombreUsuario << " (" << usuario.correo << ")";
    return salida;
}