#include "cancion.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <memoria.h>
using namespace std;

//constructor parametrizado
cancion::cancion(const string& nombreCancion_,
                 const string& id_cancion_,
                 const string& duracion_,
                 const string& ruta128_,
                 const string& ruta320_,
                 const string& creditos_,
                 const string &reproducciones_)
    : nombreCancion(nombreCancion_),
    id_cancion(id_cancion_),
    duracion(duracion_),
    ruta128(ruta128_),
    ruta320(ruta320_),
    creditos(creditos_),
    reproducciones(reproducciones_)
{}

//cargar canciones
void cancion::cargarCancion(cancion**& canciones, int& totalCanciones) {
    ifstream archivo("canciones.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de canciones.txt\n";
        return;
    }

    string linea;
    int total = 0;

    // Contar líneas para conocer cuántas canciones hay
    while (getline(archivo, linea)) total++;
    archivo.clear();
    archivo.seekg(0);

    canciones = new cancion*[total];
    registrarMemoria<cancion*>(total);

    totalCanciones = total;
    int i = 0;

    // Leer y construir cada objeto cancion
    while (getline(archivo, linea)) {
        stringstream frase(linea);
        string nombre_cancion, id_cancion, duracion, ruta128, ruta320, creditos, vecesRepetidas;
        getline(frase, nombre_cancion, ',');
        getline(frase, id_cancion, ',');
        getline(frase, duracion, ',');
        getline(frase, ruta128, ',');
        getline(frase, ruta320,',');
        getline(frase, creditos, ',');
        getline(frase, vecesRepetidas);

        cancion* nuevaCancion = new cancion(nombre_cancion, id_cancion, duracion, ruta128, ruta320, creditos, vecesRepetidas);
        registrarMemoria<cancion>(1);
        canciones[i++] = nuevaCancion;
        incrementarIteraciones();
    }
    archivo.close();
}
// -------------------- Getters --------------------
const string& cancion::getNombreCancion() const {
    return nombreCancion;
}

const string& cancion::getId_Cancion() const {
    return id_cancion;
}

const string& cancion::getRuta128() const {
    return ruta128;
}
const string& cancion::getRuta320() const {
    return ruta320;
}

const string& cancion::getCreditos() const {
    return creditos;
}
const string& cancion::getDuracion() const{
    return duracion;
}
