#include "anuncio.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <memoria.h>
using namespace std;

// Constructor parametrizado
anuncio::anuncio(const string& tipo_anuncio_, const string& mensaje_)
    : tipo_anuncio(tipo_anuncio_),
    mensaje(mensaje_)
{
    // Si quisieras validar categoría, podrías hacerlo aquí:
    // if (tipo_anuncio != "C" && tipo_anuncio != "B" && tipo_anuncio != "AAA")
    //     this->tipo_anuncio = "C"; // default
}

//cargar anuncios
void anuncio::cargarAnuncios(anuncio**& anuncios, int& totalAnuncios) {

    ifstream archivo("anuncio.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de anuncio.txt\n";
        return;
    }

    string linea;
    int total = 0;

    // Contar líneas para conocer cuántas anuncios hay
    while (getline(archivo, linea)) {
        total++;
        incrementarIteraciones();
    }
    archivo.clear();
    archivo.seekg(0);

    anuncios = new anuncio*[total];
    registrarMemoria<anuncio*>(total);

    totalAnuncios = total;
    int i = 0;

    // Leer y construir cada objeto cancion
    while (getline(archivo, linea)) {
        stringstream frase(linea);
        string categoria, mensaje;
        getline(frase, categoria, ',');
        getline(frase, mensaje);


        anuncio* nuevoAnuncio = new anuncio(categoria, mensaje);
        registrarMemoria<anuncio>(1);
        anuncios[i++] = nuevoAnuncio;
        incrementarIteraciones();
    }
    archivo.close();
}

// -------------------- Getters --------------------
const string& anuncio::getTipo_Anuncio() const  {
    return tipo_anuncio;
}

const string& anuncio::getMensaje() const {
    return mensaje;
}
