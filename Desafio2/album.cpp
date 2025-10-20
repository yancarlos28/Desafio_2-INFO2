#include "album.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <memoria.h>
using namespace std;

album::album() {}

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
        string nombre_album, id_album, duracion, fechaLanzamiento, rutaImagen, generos;
        getline(frase, nombre_album, ',');
        getline(frase, id_album, ',');
        getline(frase, duracion, ',');
        getline(frase, fechaLanzamiento, ',');
        getline(frase, rutaImagen, ',');
        getline(frase, generos, ',');

        album* nuevoAlbum = new album(nombre_album, id_album, duracion, fechaLanzamiento, rutaImagen, generos);
        registrarMemoria<album>(1);
        albumnes[i++] = nuevoAlbum;
        incrementarIteraciones();
    }
    archivo.close();
}
