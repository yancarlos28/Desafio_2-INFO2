#include "album.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <memoria.h>
using namespace std;

album::album(const string& nombre_album_,
             const string& id_album_,
             const string& duracion_,
             const string& fechaLanzamiento_,
             const string& rutaImagen_,
             const string& generos_,
             const string& puntuacion_,
             const string& sello_)
    :
    id_album(id_album_),
    duracion(duracion_),
    puntuacion(puntuacion_),
    nombre_album(nombre_album_),
    fechaLanzamiento(fechaLanzamiento_),
    ruta_Imagen(rutaImagen_),
    generos(generos_),
    sello(sello_){}

//~album()

void album::cargarAlbumnes(album**& albumnes, int& totalAlbumnes) {
    ifstream archivo("albumn.txt");
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

    albumnes = new album*[total];
    registrarMemoria<album*>(total);

    totalAlbumnes = total;
    int i = 0;

    // Leer y construir cada objeto cancion
    while (getline(archivo, linea)) {
        stringstream frase(linea);
        string nombre_album, id_album, duracion, fechaLanzamiento, rutaImagen, generos, puntuacion,sello;
        getline(frase, nombre_album, ',');
        getline(frase, id_album, ',');
        getline(frase, duracion, ',');
        getline(frase, fechaLanzamiento, ',');
        getline(frase, rutaImagen, ',');
        getline(frase, generos, ',');
        getline(frase, puntuacion, ',');
        getline(frase, sello);


        album* nuevoAlbum = new album(nombre_album, id_album, duracion, fechaLanzamiento, rutaImagen, generos,puntuacion,sello);
        registrarMemoria<album>(1);
        albumnes[i++] = nuevoAlbum;
        incrementarIteraciones();
    }
    archivo.close();
}
// -------------------- Getters --------------------
const string& album::getId_Album() const {
    return id_album;
}

const string& album::getRuta_Imagen() const {
    return ruta_Imagen;
}

