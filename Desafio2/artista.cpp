#include "artista.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <memoria.h>
using namespace std;

artista::artista() {}

void artista::cargarArtistas(artista**& artistas, int& totalArtistas) {
    ifstream archivo("artistas.txt");
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

    artistas = new artista*[total];
    registrarMemoria<artista*>(total);

    totalArtistas = total;
    int i = 0;

    // Leer y construir cada objeto cancion
    while (getline(archivo, linea)) {
        stringstream frase(linea);
        string nombre_artista, id_artista, edad, pais, cantidadSeguidores, puestoRanking, albumnes;
        getline(frase, nombre_artista, ',');
        getline(frase, id_artista, ',');
        getline(frase, edad, ',');
        getline(frase, pais, ',');
        getline(frase, cantidadSeguidores, ',');
        getline(frase, puestoRanking, ',');
        getline(frase, albumnes, ',');

        artista* nuevoArtista = new artista(nombre_artista, id_artista, edad, pais, cantidadSeguidores, puestoRanking, albumnes);
        registrarMemoria<artista>(1);
        artistas[i++] = nuevoArtista;
        incrementarIteraciones();
    }
    archivo.close();
}
