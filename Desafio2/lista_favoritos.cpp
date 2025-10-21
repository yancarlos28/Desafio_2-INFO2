#include "lista_favoritos.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <memoria.h>
using namespace std;



// Constructor parametrizado
lista_favoritos::lista_favoritos(const string& id_usuario, const string& id_canciones)
    : nickname_seguidor(id_usuario),
    lista_favorito(nullptr)
{
    // ----------------------------------------------------------
    // id_canciones es una cadena como: "100010101;100020202;100030303"
    // Aquí solo almacenaremos el texto o cantidad, no las canciones reales aún.
    // ----------------------------------------------------------
    if (id_canciones.empty()) {
        lista_favorito = nullptr;
        return;
    }

    // Contar cuántos IDs hay (separados por ';')
    int cantidad = 1;
    for (size_t i = 0; i < id_canciones.size(); i++)
        if (id_canciones[i] == ';') ++cantidad;

    // Crear arreglo dinámico de punteros
    lista_favorito = new cancion*[cantidad];
    for (int i = 0; i < cantidad; ++i)
        lista_favorito[i] = nullptr; // aún sin enlazar a canciones reales

    // ⚠️ En esta etapa no podemos crear objetos cancion porque solo tenemos IDs.
    // Cuando cargues las canciones en memoria, deberás vincularlas:
    // buscar cada id y asignar el puntero real en lista_favorito[i].
}

void lista_favoritos::cargarListasFavoritos(lista_favoritos**& listaFavoritos, int& totalListas) {
    ifstream archivo("lista_favoritos.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de lista_favoritos\n";
        return;
    }

    string linea;
    int total = 0;

    // Contar líneas para conocer cuántas lista de favoritos hay
    while (getline(archivo, linea)) total++;
    archivo.clear();
    archivo.seekg(0);

    listaFavoritos = new lista_favoritos*[total];
    registrarMemoria<lista_favoritos*>(total);

    totalListas = total;
    int i = 0;

    // Leer y construir cada objeto lista_favoritos
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
