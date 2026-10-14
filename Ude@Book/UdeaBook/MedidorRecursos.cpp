#include "MedidorRecursos.h"
#include <iostream>

using namespace std;

MedidorRecursos::MedidorRecursos() {
    reiniciar(); //aprovechamos la función para inicializar todo en 0
}

void MedidorRecursos::reiniciar() {
    iteraciones = 0;
    memoriaLocal = 0;
}

void MedidorRecursos::sumarIteraciones(unsigned long cantidad) {
    iteraciones += cantidad;
}

void MedidorRecursos::sumarMemoriaLocal(unsigned long cantidad) {
    memoriaLocal += cantidad;
}

void MedidorRecursos::mostrarMetricas(unsigned long idProceso) {
    cout << "--- Indicador del Proceso " << idProceso << " ---\n";
    cout << "Iteraciones: " << iteraciones << "\n";
    cout << "Memoria Local consumida: " << memoriaLocal << " bytes\n";
    cout << "---------------------------------\n";
}