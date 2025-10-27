#include "artista.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <memoria.h>
using namespace std;


// ---------- Constructor parametrizado ----------
artista::artista(const string& nombre_artista_,
                 const string& id_artista_,
                 const string& edad_,
                 const string& paisOrigen_,
                 const string& seguidores_,
                 const string& puestoRanking_,
                 const string& albumes_)
    :nombre_artista(nombre_artista_),
    id_artista(id_artista_),
    edad(edad_),
    paisOrigen(paisOrigen_),
    seguidores(seguidores_),
    puestoRanking(puestoRanking_),
    albumes(albumes_)
{}

//cargar artistas
void artista::cargarArtistas(artista**& artistas, int& totalArtistas) {
    ifstream archivo("artistas.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de artistas.txt\n";
        return;
    }

    string linea;
    int total = 0;

    // Contar líneas para conocer cuántas canciones hay
    while (getline(archivo, linea)) {
        total++;
        incrementarIteraciones();
    }
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
        getline(frase, albumnes);

        artista* nuevoArtista = new artista(nombre_artista, id_artista, edad, pais, cantidadSeguidores, puestoRanking, albumnes);
        registrarMemoria<artista>(1);
        artistas[i++] = nuevoArtista;
        incrementarIteraciones();
    }
    archivo.close();
}

// -------------------- Getters --------------------
const string& artista::getNombre_Artista() const {
    return nombre_artista;
}

const string& artista::getId_Artista() const {
    return id_artista;
}

const string& artista::getAlbumes() const {
    return albumes;
}
