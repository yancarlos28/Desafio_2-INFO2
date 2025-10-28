#ifndef MEMORIA_H
#define MEMORIA_H
#include <iostream>

// Variables globales (se declaran aqui como extern, pero se definen en utilidades.cpp)
extern int contadorIteracionesGlobal;
extern size_t memoriaReservadaGlobal;


// Plantilla para registrar memoria
template <typename T>
void registrarMemoria(size_t cantidad) {
    memoriaReservadaGlobal += sizeof(T) * cantidad;
}

template <typename T>
void liberarMemoria(size_t cantidad) {
    if (memoriaReservadaGlobal >= sizeof(T) * cantidad) {
        memoriaReservadaGlobal -= sizeof(T) * cantidad;
    }
}

template <typename T = void>
void incrementarIteraciones() {
    contadorIteracionesGlobal++;
}

template <typename T = void>
void mostrarUsoMemoria() {
    std::cout << "Iteraciones hasta el momento: " << contadorIteracionesGlobal << "\n";
    std::cout << "Memoria reservada hasta el momento: " << memoriaReservadaGlobal << " bytes" << "\n";
}

#endif // MEMORIA_H

