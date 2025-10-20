#include "cancion.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <memoria.h>
using namespace std;

void cancion::cargarCancion(cancion**& canciones, int& totalCanciones) {
    ifstream archivo("cancion.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de Alojamientos\n";
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
        string nombre_cancion, id_cancion, ruta_cancion, creditos, vecesRepetidas;
        getline(frase, nombre_cancion, ',');
        getline(frase, id_cancion, ',');
        getline(frase, ruta_cancion, ',');
        getline(frase, creditos, ',');
        getline(frase, vecesRepetidas, ',');

        cancion* nuevaCancion = new cancion(nombre_cancion, id_cancion, ruta_cancion, creditos, vecesRepetidas);
        registrarMemoria<cancion>(1);
        canciones[i++] = nuevaCancion;
        incrementarIteraciones();
    }
    archivo.close();
}
