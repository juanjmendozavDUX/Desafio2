#ifndef MEDIDORRECURSOS_H
#define MEDIDORRECURSOS_H

class MedidorRecursos {
private:
    unsigned long iteraciones;
    unsigned long memoriaLocal;

public:
    MedidorRecursos();
    void reiniciar();
    void sumarIteraciones(unsigned long);
    void sumarMemoriaLocal(unsigned long);
    void mostrarMetricas(unsigned long);
};

#endif
