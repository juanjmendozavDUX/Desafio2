#ifndef TEXTO_H
#define TEXTO_H

#include <iosfwd>

class Texto {
private:
    char* datos;
    int longitud;
    int capacidad;

    void reservar(int nuevaCapacidad);

public:
    Texto();
    Texto(const char* texto);
    Texto(const Texto& otro);
    ~Texto();

    Texto& operator=(const Texto& otro);
    Texto& operator=(const char* texto);

    void limpiar();
    void agregar(char caracter);
    int largo() const;
    bool vacio() const;
    char en(int posicion) const;
    unsigned long calcularMemoria() const;

    bool operator==(const Texto& otro) const;
    bool operator!=(const Texto& otro) const;
    bool operator<(const Texto& otro) const;

    static bool leerHasta(std::istream& entrada, char separador, Texto& resultado);
    static bool leerLinea(std::istream& entrada, Texto& resultado);

    friend std::ostream& operator<<(std::ostream& salida, const Texto& texto);
};

#endif