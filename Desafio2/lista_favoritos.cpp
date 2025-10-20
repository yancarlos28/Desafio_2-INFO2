#include "lista_favoritos.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <memoria.h>
using namespace std;

lista_favoritos::lista_favoritos() {}

void lista_favoritos::cargarListasFavoritos(lista_favoritos**& listaFavoritos, int& totalListas) {
    ifstream archivo("lista_favoritos.txt");
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

    listaFavoritos = new lista_favoritos*[total];
    registrarMemoria<lista_favoritos*>(total);

    totalListas = total;
    int i = 0;

    // Leer y construir cada objeto cancion
    while (getline(archivo, linea)) {
        stringstream frase(linea);
        string id_usuario, id_canciones;
        getline(frase, id_usuario, ',');
        getline(frase, id_canciones, ',');


        lista_favoritos* nuevaLista = new lista_favoritos(id_usuario, id_canciones);
        registrarMemoria<lista_favoritos>(1);
        listaFavoritos[i++] = nuevaLista;
        incrementarIteraciones();
    }
    archivo.close();
}
