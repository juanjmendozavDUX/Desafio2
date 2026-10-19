#include "UdeABook.h"
#include "Usuario.h"
#include "Album.h"
#include "Fotos.h"
#include "Mensaje.h"
#include "MedidorRecursos.h"
#include "Fecha.h"
#include <iostream>
#include <fstream>
#include <random>

using namespace std;
static const char* ARCH_USUARIOS    = "usuarios.txt";
static const char* ARCH_ALBUMES     = "albumes.txt";
static const char* ARCH_FOTOS       = "fotos.txt";
static const char* ARCH_AMISTADES   = "amistades.txt";
static const char* ARCH_SOLICITUDES = "solicitudes.txt";
static const char* ARCH_MENSAJES    = "mensajes.txt";
static const int DIA_SISTEMA = 17;
static const int MES_SISTEMA = 10;
static const int ANIO_SISTEMA = 2026;
static const int MAX_MENSAJE = 512;
static const char* carpetaDatos() {
    ifstream prueba(ARCH_USUARIOS);

    if (prueba.is_open()) {
        return "";
    }

    prueba.close();

    prueba.open("datos_prueba/usuarios.txt");

    if (prueba.is_open()) {
        return "datos_prueba/";
    }

    prueba.close();

    prueba.open("../datos_prueba/usuarios.txt");

    if (prueba.is_open()) {
        return "../datos_prueba/";
    }

    prueba.close();

    prueba.open("../../../datos_prueba/usuarios.txt");

    if (prueba.is_open()) {
        return "../../../datos_prueba/";
    }

    return "";
}

static void construirRuta(const char* carpeta, const char* nombre, char* ruta) {
    int posicion = 0;
    for (int i = 0; carpeta[i] != '\0'; i++) ruta[posicion++] = carpeta[i];
    for (int i = 0; nombre[i] != '\0'; i++) ruta[posicion++] = nombre[i];
    ruta[posicion] = '\0';
}

static bool abrirEntrada(ifstream& archivo, const char* nombre) {
    const char* carpeta = carpetaDatos();
    char ruta[256];
    construirRuta(carpeta, nombre, ruta);
    archivo.open(ruta);
    return archivo.is_open();
}

static bool abrirSalida(ofstream& archivo, const char* nombre) {
    const char* carpeta = carpetaDatos();
    char ruta[256];
    construirRuta(carpeta, nombre, ruta);
    archivo.open(ruta);
    return archivo.is_open();
}

static void quitarRetorno(Texto&) {
}

static int aEntero(const Texto& texto) {
    int valor = 0;
    for (int i = 0; i < texto.largo(); i++) {
        char c = texto.en(i);
        if (c >= '0' && c <= '9') valor = valor * 10 + (c - '0');
    }
    return valor;
}

static int aEnteroInicial(const Texto& texto) {
    int valor = 0;
    bool comenzo = false;
    for (int i = 0; i < texto.largo(); i++) {
        char c = texto.en(i);
        if (c >= '0' && c <= '9') {
            valor = valor * 10 + (c - '0');
            comenzo = true;
        } else if (comenzo) {
            break;
        }
    }
    return valor;
}

static Fecha aFecha(const Texto& texto) {
    int partes[3] = {0, 0, 0};
    int p = 0;
    bool hayDigito = false;
    for (int i = 0; i < texto.largo() && p < 3; i++) {
        char c = texto.en(i);
        if (c >= '0' && c <= '9') {
            partes[p] = partes[p] * 10 + (c - '0');
            hayDigito = true;
        } else if (hayDigito) {   // un separador despues de digitos: pasa a la siguiente parte
            p++;
            hayDigito = false;
        }
    }
    return Fecha(partes[0], partes[1], partes[2]);
}

static int leerEntero() {
    Texto linea;
    Texto::leerLinea(cin, linea);
    int valor = 0;
    bool hayDigito = false;
    for (int i = 0; i < linea.largo(); i++) {
        char c = linea.en(i);
        if (c >= '0' && c <= '9') {
            valor = valor * 10 + (c - '0');
            hayDigito = true;
            if (valor > 100000000) return -1;   // evita desbordar el int
        } else if (c != '\r' && c != ' ') {
            return -1;
        }
    }
    return hayDigito ? valor : -1;
}

static bool pedirFecha(Fecha& resultado) {
    cout << "  Dia: ";  int d = leerEntero();
    cout << "  Mes: ";  int m = leerEntero();
    cout << "  Año: "; int a = leerEntero();
    if (a < 1 || a > 9999 || m < 1 || m > 12 || d < 1) return false;
    int diasMes[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((a % 400 == 0) || (a % 4 == 0 && a % 100 != 0)) diasMes[1] = 29;
    if (d > diasMes[m - 1]) return false;
    resultado = Fecha(d, m, a);
    return true;
}

// Constructor y destructor

// 'usuarios' se construye solo (es un ArregloDinamico con su propio constructor).
// La matriz no se crea aqui: su tamano depende de cuantos usuarios se carguen.
UdeABook::UdeABook() {
    usuarioActivo = nullptr;
    matrizAmistad = nullptr;
    medidor = new MedidorRecursos();
    fotosAmigosActualizadas = false;
    numMostrados = 0;
    for (int i = 0; i < 3; i++) amigosMostrados[i] = -1;
}

// Regla: todo 'new' tiene su 'delete'. Se libera en orden inverso al de creacion.
UdeABook::~UdeABook() {
    int n = usuarios.getCantidad();

    if (matrizAmistad != nullptr) {
        for (int i = 0; i < n; i++) {
            delete[] matrizAmistad[i];   // cada fila
        }
        delete[] matrizAmistad;          // el arreglo de filas
    }

    for (int i = 0; i < n; i++) {
        delete usuarios[i];              // el arreglo solo guarda punteros: hay que borrar cada Usuario
    }

    delete medidor;
}

void UdeABook::quickSort(int inicio, int fin) {
    // intercambia las posiciones a y b del arreglo
    auto intercambiar = [this](int a, int b) {
        Usuario* temp = usuarios[a];
        usuarios[a] = usuarios[b];
        usuarios[b] = temp;
    };

    while (inicio < fin) {
        int medio = inicio + (fin - inicio) / 2;
        if (usuarios[medio]->getNombreUsuario() < usuarios[inicio]->getNombreUsuario()) intercambiar(inicio, medio);
        if (usuarios[fin]->getNombreUsuario()   < usuarios[inicio]->getNombreUsuario()) intercambiar(inicio, fin);
        if (usuarios[fin]->getNombreUsuario()   < usuarios[medio]->getNombreUsuario())  intercambiar(medio, fin);
        intercambiar(inicio, medio);

        Texto nombrePivote = usuarios[inicio]->getNombreUsuario();
        int i = inicio + 1;
        int j = fin;
        while (i <= j) {
            while (i <= fin && usuarios[i]->getNombreUsuario() < nombrePivote) i++;
            while (j > inicio && !(usuarios[j]->getNombreUsuario() < nombrePivote)) j--;
            if (i < j) intercambiar(i, j);
        }
        intercambiar(inicio, j);

        if (j - inicio < fin - j) {
            quickSort(inicio, j - 1);
            inicio = j + 1;
        } else {
            quickSort(j + 1, fin);
            fin = j - 1;
        }
    }
}

void UdeABook::cargarDatos() {
    ifstream archUsuarios;
    if (!abrirEntrada(archUsuarios, ARCH_USUARIOS)) {
        cout << "No se pudo abrir " << ARCH_USUARIOS << " (debe estar en la carpeta de ejecucion).\n";
        return;
    }
    Texto nombre, nacimiento, correo, rutaFoto, contrasena, ciudad, pais, inscripcion;
    while (Texto::leerHasta(archUsuarios, '|', nombre)) {
        Texto::leerHasta(archUsuarios, '|', nacimiento);
        Texto::leerHasta(archUsuarios, '|', correo);
        Texto::leerHasta(archUsuarios, '|', rutaFoto);
        Texto::leerHasta(archUsuarios, '|', contrasena);
        Texto::leerHasta(archUsuarios, '|', ciudad);
        Texto::leerHasta(archUsuarios, '|', pais);
        Texto::leerLinea(archUsuarios, inscripcion);
        if (nombre.vacio()) continue;
        Fecha fNac = aFecha(nacimiento);
        Fecha fIns = aFecha(inscripcion);
        usuarios.agregar(new Usuario(aEntero(nombre), nombre, fNac, correo,
                                     rutaFoto, contrasena, ciudad, pais, fIns));
    }
    archUsuarios.close();

    int n = usuarios.getCantidad();
    matrizAmistad = new bool*[n];
    for (int i = 0; i < n; i++) matrizAmistad[i] = new bool[n]();

    ifstream archAlbumes;
    if (abrirEntrada(archAlbumes, ARCH_ALBUMES)) {
        Texto dueno, nomAlbum, rutaAlbum, priv;
        while (Texto::leerHasta(archAlbumes, '|', dueno)) {
            if (!Texto::leerHasta(archAlbumes, '|', nomAlbum)) break;
            if (!Texto::leerHasta(archAlbumes, '|', rutaAlbum)) break;
            if (!Texto::leerLinea(archAlbumes, priv)) break;
            Usuario* u = buscarUsuario(aEntero(dueno));
            if (u == nullptr) continue;
            char privacidad = (priv.largo() > 0) ? priv.en(0) : 'V';
            Album album(nomAlbum, rutaAlbum, privacidad);
            u->agregarAlbum(album);
        }
        archAlbumes.close();
    }

    ifstream archFotos;
    if (abrirEntrada(archFotos, ARCH_FOTOS)) {
        Texto dueno, nomAlbum, rutaFoto, tamano, dia, mes, anio, likes;
        while (Texto::leerHasta(archFotos, '|', dueno)) {
            if (!Texto::leerHasta(archFotos, '|', nomAlbum)) break;
            if (!Texto::leerHasta(archFotos, '|', rutaFoto)) break;
            if (!Texto::leerHasta(archFotos, '|', tamano)) break;
            if (!Texto::leerHasta(archFotos, '|', dia)) break;
            if (!Texto::leerHasta(archFotos, '|', mes)) break;
            if (!Texto::leerHasta(archFotos, '|', anio)) break;
            if (!Texto::leerLinea(archFotos, likes)) break;
            Usuario* u = buscarUsuario(aEntero(dueno));
            if (u == nullptr) continue;
            for (int i = 0; i < u->getNumAlbumes(); i++) {
                Album& album = u->getAlbum(i);
                if (album.getNombre() == nomAlbum) {
                    Fecha fecha(aEntero(dia), aEntero(mes), aEntero(anio));
                    Foto foto(fecha, aEntero(tamano), rutaFoto);
                    foto.setLikes(aEntero(likes));
                    album.agregarFoto(foto);
                    break;
                }
            }
        }
        archFotos.close();
    }

    ifstream archAmistades;
    if (abrirEntrada(archAmistades, ARCH_AMISTADES)) {
        Texto a, b;
        while (Texto::leerHasta(archAmistades, '|', a)) {
            if (!Texto::leerLinea(archAmistades, b)) break;
            Usuario* ua = buscarUsuario(aEntero(a));
            Usuario* ub = buscarUsuario(aEntero(b));
            if (ua != nullptr && ub != nullptr && ua != ub &&
                ua->getIndice() < n && ub->getIndice() < n) {
                matrizAmistad[ua->getIndice()][ub->getIndice()] = true;
                matrizAmistad[ub->getIndice()][ua->getIndice()] = true;
            }
        }
        archAmistades.close();
    }

    ifstream archSolicitudes;
    if (abrirEntrada(archSolicitudes, ARCH_SOLICITUDES)) {
        Texto origen, destino;
        while (Texto::leerHasta(archSolicitudes, '|', origen)) {
            if (!Texto::leerLinea(archSolicitudes, destino)) break;
            Usuario* uo = buscarUsuario(aEntero(origen));
            Usuario* ud = buscarUsuario(aEntero(destino));
            if (uo != nullptr && ud != nullptr && uo != ud) ud->agregarSolicitud(uo);
        }
        archSolicitudes.close();
    }

    ifstream archMensajes;
    if (abrirEntrada(archMensajes, ARCH_MENSAJES)) {
        Texto emi, rec, texto, dia, mes, anio;
        while (Texto::leerHasta(archMensajes, '|', emi)) {
            if (!Texto::leerHasta(archMensajes, '|', rec)) break;
            if (!Texto::leerHasta(archMensajes, '|', texto)) break;
            if (!Texto::leerHasta(archMensajes, '|', dia)) break;
            if (!Texto::leerHasta(archMensajes, '|', mes)) break;
            if (!Texto::leerLinea(archMensajes, anio)) break;
            Usuario* e = buscarUsuario(aEntero(emi));
            Usuario* r = buscarUsuario(aEntero(rec));
            if (e == nullptr || r == nullptr) continue;
            Fecha f(aEntero(dia), aEntero(mes), aEnteroInicial(anio));
            Mensaje* m = new Mensaje(e, r, texto, f);
            for (int i = 0; i + 1 < anio.largo(); i++) {
                if (anio.en(i) == '|' && anio.en(i + 1) == '1') {
                    m->marcarLeido();
                    break;
                }
            }
            e->agregarMensajeEnviado(m);
            r->agregarMensajeRecibido(m);
        }
        archMensajes.close();
    }
    medidor->reiniciar();
}

void UdeABook::actualizarDatos() {
    if (matrizAmistad == nullptr) return;
    int n = usuarios.getCantidad();
    ofstream archAmistades;
    if (abrirSalida(archAmistades, ARCH_AMISTADES)) {
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (matrizAmistad[i][j]) {
                    archAmistades << usuarios[i]->getNombreUsuario() << '|'
                                  << usuarios[j]->getNombreUsuario() << '\n';
                }
            }
        }
    }
    ofstream archSolicitudes;
    ofstream archMensajes;
    ofstream archAlbumes;
    ofstream archFotos;
    abrirSalida(archSolicitudes, ARCH_SOLICITUDES);
    abrirSalida(archMensajes, ARCH_MENSAJES);
    abrirSalida(archAlbumes, ARCH_ALBUMES);
    abrirSalida(archFotos, ARCH_FOTOS);
    for (int i = 0; i < n; i++) {
        if (archSolicitudes.is_open()) usuarios[i]->guardarSolicitudes(archSolicitudes);
        if (archMensajes.is_open()) usuarios[i]->guardarMensajes(archMensajes);
        if (archAlbumes.is_open() && archFotos.is_open()) {
            usuarios[i]->guardarAlbumes(archAlbumes, archFotos);
        }
    }
}

Usuario* UdeABook::buscarUsuario(Texto& nombreUsuario) {
    for (int i = 0; i < usuarios.getCantidad(); i++) {
        medidor->sumarIteraciones(1);
        if (usuarios[i]->getNombreUsuario() == nombreUsuario) return usuarios[i];
    }
    return nullptr;
}

Usuario* UdeABook::buscarUsuario(int indice) {
    if (indice < 0) return nullptr;
    if (indice < usuarios.getCantidad() && usuarios[indice]->getIndice() == indice) {
        return usuarios[indice];
    }
    for (int i = 0; i < usuarios.getCantidad(); i++) {
        if (usuarios[i]->getIndice() == indice) return usuarios[i];
    }
    return nullptr;
}

bool UdeABook::iniciarSesion(Texto& nombreUsuario, Texto& contrasena) {
    Usuario* u = buscarUsuario(nombreUsuario);
    if (u == nullptr || !u->validarContrasena(contrasena)) {
        return false;
    }
    usuarioActivo = u;
    fotosAmigosActualizadas = false;
    numMostrados = 0;
    return true;
}

int UdeABook::seleccionarAmigos(bool excluirPrevios) {
    if (usuarioActivo == nullptr || matrizAmistad == nullptr) return 0;
    int n = usuarios.getCantidad();
    int yo = usuarioActivo->getIndice();
    bool* miFila = matrizAmistad[yo];

    ArregloDinamico<int> candidatos;
    ArregloDinamico<double> pesos;
    for (int k = 0; k < n; k++) {
        if (!miFila[k]) continue;
        if (excluirPrevios) {
            bool yaMostrado = false;
            for (int m = 0; m < numMostrados; m++) {
                if (amigosMostrados[m] == k) yaMostrado = true;
            }
            if (yaMostrado) continue;
        }
        int comunes = amigosEnComun(yo, k);
        double peso = 1.0;
        if (comunes > 20) peso = 3.5;
        else if (comunes > 10) peso = 2.5;
        candidatos.agregar(k);
        pesos.agregar(peso);
    }
    medidor->sumarIteraciones(n);

    int cantidadCand = candidatos.getCantidad();
    if (cantidadCand == 0) {
        if (!excluirPrevios) numMostrados = 0;
        return 0;
    }

    static mt19937 generador((random_device())());
    double total = 0;
    for (int i = 0; i < cantidadCand; i++) total += pesos[i];

    int nuevos[3];
    int elegidos = 0;
    for (int ronda = 0; ronda < 3 && ronda < cantidadCand; ronda++) {
        uniform_real_distribution<double> dado(0.0, total);
        double r = dado(generador);
        double acumulado = 0;
        int elegido = -1;
        for (int i = 0; i < cantidadCand; i++) {
            if (pesos[i] <= 0) continue;
            acumulado += pesos[i];
            elegido = i;
            if (r < acumulado) break;
        }
        nuevos[elegidos++] = candidatos[elegido];
        total -= pesos[elegido];
        pesos[elegido] = 0;
        medidor->sumarIteraciones(cantidadCand);
    }

    for (int i = 0; i < elegidos; i++) amigosMostrados[i] = nuevos[i];
    numMostrados = elegidos;
    return elegidos;
}

void UdeABook::imprimirMostrados() {
    for (int i = 0; i < numMostrados; i++) {
        Usuario* amigo = usuarios[amigosMostrados[i]];
        cout << "  " << amigo->getNombreUsuario() << ": " << amigo->getRutaFotoPerfil() << "\n";
    }
}

void UdeABook::mostrarMejoresAmigos() {
    if (seleccionarAmigos(false) == 0) {
        cout << "Aun no tienes amigos para mostrar.\n";
        return;
    }
    cout << "--- Fotos de perfil de tus amigos ---\n";
    imprimirMostrados();
}

bool UdeABook::actualizarMejoresAmigos() {
    if (fotosAmigosActualizadas) {
        cout << "Ya usaste tu unica actualizacion de fotos.\n";
        return false;
    }
    if (seleccionarAmigos(true) == 0) {
        cout << "No tienes mas amigos distintos para mostrar.\n";
        return false;
    }
    fotosAmigosActualizadas = true;
    cout << "--- Nuevas fotos de perfil de tus amigos ---\n";
    imprimirMostrados();
    return true;
}

bool UdeABook::sonAmigos(int indice1, int indice2) {
    int n = usuarios.getCantidad();
    if (matrizAmistad == nullptr || indice1 < 0 || indice2 < 0 || indice1 >= n || indice2 >= n) {
        return false;
    }
    return matrizAmistad[indice1][indice2];
}

// Cuenta las columnas donde AMBAS filas tienen 'true'.
int UdeABook::amigosEnComun(int indice1, int indice2) {
    int n = usuarios.getCantidad();
    if (matrizAmistad == nullptr || indice1 < 0 || indice2 < 0 || indice1 >= n || indice2 >= n) {
        return 0;
    }
    bool* fila1 = matrizAmistad[indice1];
    bool* fila2 = matrizAmistad[indice2];
    int comunes = 0;
    for (int k = 0; k < n; k++) {
        if (fila1[k] && fila2[k]) comunes++;
    }
    medidor->sumarIteraciones(n);
    return comunes;
}

bool UdeABook::enviarSolicitud(Texto& nombreDestino) {
    if (usuarioActivo == nullptr) return false;

    Usuario* destino = buscarUsuario(nombreDestino);
    if (destino == nullptr) {
        cout << "No existe ese usuario.\n";
        return false;
    }
    if (destino == usuarioActivo) {
        cout << "No puedes enviarte una solicitud a ti mismo.\n";
        return false;
    }
    if (sonAmigos(usuarioActivo->getIndice(), destino->getIndice())) {
        cout << "Ya son amigos.\n";
        return false;
    }
    if (usuarioActivo->tieneSolicitudDe(destino)) {
        cout << "Ese usuario ya te envio una solicitud: aceptala desde tus solicitudes.\n";
        return false;
    }
    if (destino->tieneSolicitudDe(usuarioActivo)) {
        cout << "Ya enviaste una solicitud a ese usuario.\n";
        return false;
    }
    destino->agregarSolicitud(usuarioActivo);
    return true;
}

bool UdeABook::aceptarSolicitud(Texto& nombreOrigen) {
    if (usuarioActivo == nullptr || matrizAmistad == nullptr) return false;

    Usuario* origen = buscarUsuario(nombreOrigen);
    if (origen == nullptr || !usuarioActivo->tieneSolicitudDe(origen)) {
        return false;
    }
    int a = usuarioActivo->getIndice();
    int b = origen->getIndice();
    matrizAmistad[a][b] = true;
    matrizAmistad[b][a] = true;
    usuarioActivo->quitarSolicitud(origen);
    return true;
}

void UdeABook::visitarPerfilAmigo(Texto& nombreAmigo) {
    if (usuarioActivo == nullptr) return;

    Usuario* amigo = buscarUsuario(nombreAmigo);
    if (amigo == nullptr || !sonAmigos(usuarioActivo->getIndice(), amigo->getIndice())) {
        cout << "Ese usuario no existe o no es tu amigo.\n";
        return;
    }

    amigo->listarAlbumesVisibles();
    cout << "Numero del album a abrir (0 para volver): ";
    int eleccion = leerEntero();
    if (eleccion < 1) return;
    int contadorVisible = 0;
    int indiceReal = -1;
    for (int i = 0; i < amigo->getNumAlbumes(); i++) {
        if (!amigo->getAlbum(i).visibleParaAmigos()) continue;
        contadorVisible++;
        if (contadorVisible == eleccion) {
            indiceReal = i;
            break;
        }
    }
    if (indiceReal < 0) return;
    Album& album = amigo->getAlbum(indiceReal);
    int numFotos = album.getNumFotos();
    if (numFotos == 0) {
        cout << "El album no tiene fotos.\n";
        return;
    }

    bool* conLike = new bool[numFotos]();
    medidor->sumarMemoriaLocal(sizeof(bool) * numFotos + sizeof(int) * 3 + sizeof(Foto*) + sizeof(Album*));

    album.reiniciarCarrusel();
    Foto* foto = album.siguienteFoto();               // la primera foto
    int actual = 0;
    int opcion;
    do {
        cout << "\nFoto " << (actual + 1) << " de " << numFotos << ": ";
        foto->mostrarRuta();
        cout << "Likes: " << foto->getLikes() << (conLike[actual] ? "  (tu like esta dado)" : "") << "\n";
        cout << "1. Siguiente   2. Previa   3. Dar/quitar like   0. Salir del album\nOpcion: ";
        opcion = leerEntero();
        if (opcion == 1) {
            foto = album.siguienteFoto();
            actual = (actual + 1) % numFotos;
        } else if (opcion == 2) {
            foto = album.fotoAnterior();
            actual = (actual - 1 + numFotos) % numFotos;
        } else if (opcion == 3) {
            if (conLike[actual]) { foto->quitarLike(); conLike[actual] = false; }
            else                 { foto->darLike();    conLike[actual] = true; }
        } else if (opcion != 0) {
            cout << "Opcion invalida.\n";
        }
        medidor->sumarIteraciones(1);
    } while (opcion != 0);

    delete[] conLike;
}

void UdeABook::sugerirAmigos(int k) {
    if (usuarioActivo == nullptr || matrizAmistad == nullptr) return;
    if (k < 1 || k > 20) {
        cout << "K debe estar entre 1 y 20.\n";
        return;
    }
    int n = usuarios.getCantidad();
    int yo = usuarioActivo->getIndice();
    bool* miFila = matrizAmistad[yo];

    int* conteo = new int[n]();
    for (int f = 0; f < n; f++) {
        if (!miFila[f]) continue;
        bool* filaF = matrizAmistad[f];
        for (int g = 0; g < n; g++) {
            if (filaF[g] && g != yo && !miFila[g]) conteo[g]++;
        }
        medidor->sumarIteraciones(n);
    }
    medidor->sumarIteraciones(n);

    int* mejores = new int[k];
    int* cuentas = new int[k];
    int cantidadMejores = 0;
    for (int g = 0; g < n; g++) {
        int c = conteo[g];
        if (c == 0) continue;
        if (cantidadMejores == k && c <= cuentas[k - 1]) continue;
        int pos = (cantidadMejores < k) ? cantidadMejores : k - 1;
        while (pos > 0 && cuentas[pos - 1] < c) {
            cuentas[pos] = cuentas[pos - 1];
            mejores[pos] = mejores[pos - 1];
            pos--;
        }
        cuentas[pos] = c;
        mejores[pos] = g;
        if (cantidadMejores < k) cantidadMejores++;
    }
    medidor->sumarIteraciones(n);
    medidor->sumarMemoriaLocal(sizeof(int) * (n + 2 * k + 8) + sizeof(void*) * 4);

    if (cantidadMejores == 0) {
        cout << "No hay sugerencias de amistad por ahora.\n";
    } else {
        cout << "--- Sugerencias de amistad ---\n";
        for (int i = 0; i < cantidadMejores; i++) {
            cout << (i + 1) << ". " << usuarios[mejores[i]]->getNombreUsuario()
            << " (" << cuentas[i] << " amigos en comun)\n";
        }
        cout << "Nombre de usuario al que quieres enviar solicitud (Enter para omitir): ";
        Texto elegido;
        Texto::leerLinea(cin, elegido);
        quitarRetorno(elegido);
        if (!elegido.vacio()) {
            bool estaEnLista = false;
            for (int i = 0; i < cantidadMejores; i++) {
                if (usuarios[mejores[i]]->getNombreUsuario() == elegido) estaEnLista = true;
            }
            if (!estaEnLista) cout << "Ese usuario no esta en la lista de sugerencias.\n";
            else if (enviarSolicitud(elegido)) cout << "Solicitud enviada.\n";
        }
    }

    delete[] conteo;
    delete[] mejores;
    delete[] cuentas;
}

// VI. Mensajes

bool UdeABook::enviarMensaje(Texto& nombreDestino, Texto& textoMensaje) {
    if (usuarioActivo == nullptr) return false;

    int largo = (int)textoMensaje.largo();
    if (largo == 0 || largo > MAX_MENSAJE) {
        cout << "El mensaje debe tener entre 1 y " << MAX_MENSAJE << " caracteres.\n";
        return false;
    }

    Usuario* destino = buscarUsuario(nombreDestino);
    if (destino == nullptr) {
        cout << "No existe ese usuario.\n";
        return false;
    }
    if (!sonAmigos(usuarioActivo->getIndice(), destino->getIndice())) {
        cout << "Solo puedes enviar mensajes a tus amigos.\n";
        return false;
    }

    Fecha hoy(DIA_SISTEMA, MES_SISTEMA, ANIO_SISTEMA);
    Mensaje* nuevo = new Mensaje(usuarioActivo, destino, textoMensaje, hoy);
    usuarioActivo->agregarMensajeEnviado(nuevo);
    destino->agregarMensajeRecibido(nuevo);
    return true;
}

void UdeABook::leerMensajesNuevos() {
    if (usuarioActivo == nullptr) return;
    usuarioActivo->leerMensajesNuevos();
}

void UdeABook::buscarMensajes(Fecha& inicio, Fecha& fin) {
    if (usuarioActivo == nullptr) return;
    if (!(inicio <= fin)) {
        cout << "La fecha de inicio debe ser menor o igual a la fecha final.\n";
        return;
    }
    usuarioActivo->buscarMensajes(inicio, fin);
}

unsigned long UdeABook::calcularMemoria() {
    unsigned long total = sizeof(*this);
    int n = usuarios.getCantidad();

    total += (unsigned long)usuarios.getCapacidad() * sizeof(Usuario*);

    for (int i = 0; i < n; i++) {
        total += usuarios[i]->calcularMemoria();
    }

    if (matrizAmistad != nullptr) {
        total += (unsigned long)n * sizeof(bool*);
        total += (unsigned long)n * n * sizeof(bool);
    }

    total += sizeof(MedidorRecursos);
    return total;
}

void UdeABook::terminarFuncionalidad(int id) {
    medidor->sumarMemoriaLocal(calcularMemoria());
    medidor->mostrarMetricas(id);
    medidor->reiniciar();
}

void UdeABook::mostrarMenu() {
    medidor->reiniciar();
    bool dentro = false;
    Texto nombre, clave;
    for (int intento = 1; intento <= 3 && !dentro; intento++) {
        cout << "=== Ude@Book ===\nUsuario: ";
        Texto::leerLinea(cin, nombre);
        quitarRetorno(nombre);
        cout << "Contrasena: ";
        Texto::leerLinea(cin, clave);
        quitarRetorno(clave);
        dentro = iniciarSesion(nombre, clave);
        if (!dentro) cout << "Usuario o contrasena incorrectos.\n\n";
    }
    if (!dentro) {
        cout << "Demasiados intentos fallidos.\n";
        return;
    }

    cout << "\nBienvenido, " << nombre << "\n";
    mostrarMejoresAmigos();
    if (numMostrados > 0) {
        cout << "1. Actualizar estas fotos (solo una vez)   0. Continuar\nOpcion: ";
        if (leerEntero() == 1) actualizarMejoresAmigos();
    }
    medidor->sumarMemoriaLocal(sizeof(int) * 4 + sizeof(bool) + sizeof(Texto) * 2);
    terminarFuncionalidad(0);

    int opcion;
    do {
        cout << "\n===== MENU PRINCIPAL =====\n"
             << "1. Enviar solicitud de amistad\n"
             << "2. Aceptar solicitudes recibidas\n"
             << "3. Visitar perfil de un amigo\n"
             << "4. Ver sugerencias de amistad\n"
             << "5. Enviar mensaje\n"
             << "6. Leer mensajes nuevos\n"
             << "7. Buscar mensajes por rango de fechas\n"
             << "8. Salir\n"
             << "Opcion: ";
        opcion = leerEntero();
        Texto otro, texto;

        switch (opcion) {
        case 1:
            cout << "Nombre de usuario destino: ";
            Texto::leerLinea(cin, otro);
            quitarRetorno(otro);
            if (enviarSolicitud(otro)) cout << "Solicitud enviada.\n";
            medidor->sumarMemoriaLocal(sizeof(Texto));
            terminarFuncionalidad(1);
            break;

        case 2: {
            int pendientes = usuarioActivo->mostrarSolicitudes();
            if (pendientes == 0) {
                cout << "No tienes solicitudes pendientes.\n";
            } else {
                cout << "Nombre del usuario a aceptar (Enter para volver): ";
                Texto::leerLinea(cin, otro);
                quitarRetorno(otro);
                if (!otro.vacio()) {
                    if (aceptarSolicitud(otro)) cout << "Ahora son amigos.\n";
                    else cout << "No tienes una solicitud pendiente de ese usuario.\n";
                }
            }
            medidor->sumarMemoriaLocal(sizeof(int) + sizeof(Texto));
            terminarFuncionalidad(2);
            break;
        }

        case 3:
            cout << "Nombre de usuario de tu amigo: ";
            Texto::leerLinea(cin, otro);
            quitarRetorno(otro);
            visitarPerfilAmigo(otro);
            medidor->sumarMemoriaLocal(sizeof(Texto));
            terminarFuncionalidad(3);
            break;

        case 4: {
            cout << "Cuantas sugerencias quieres ver (1 a 20): ";
            int k = leerEntero();
            sugerirAmigos(k);
            medidor->sumarMemoriaLocal(sizeof(int));
            terminarFuncionalidad(4);
            break;
        }

        case 5:
            cout << "Nombre de usuario de tu amigo: ";
            Texto::leerLinea(cin, otro);
            quitarRetorno(otro);
            cout << "Mensaje (maximo " << MAX_MENSAJE << " caracteres): ";
            Texto::leerLinea(cin, texto);
            quitarRetorno(texto);
            if (enviarMensaje(otro, texto)) cout << "Mensaje enviado.\n";
            medidor->sumarMemoriaLocal(sizeof(Texto) * 2);
            terminarFuncionalidad(5);
            break;

        case 6:
            leerMensajesNuevos();
            terminarFuncionalidad(6);
            break;

        case 7: {
            Fecha desde, hasta;
            cout << "Fecha de inicio:\n";
            bool ok1 = pedirFecha(desde);
            cout << "Fecha de fin:\n";
            bool ok2 = pedirFecha(hasta);
            if (!ok1 || !ok2) cout << "Fecha invalida.\n";
            else buscarMensajes(desde, hasta);
            medidor->sumarMemoriaLocal(sizeof(Fecha) * 2 + sizeof(bool) * 2);
            terminarFuncionalidad(7);
            break;
        }

        case 8:
            break;

        default:
            cout << "Opcion invalida.\n";
        }
    } while (opcion != 8);

    actualizarDatos();
    cout << "Datos guardados. Hasta pronto.\n";
    usuarioActivo = nullptr;
}