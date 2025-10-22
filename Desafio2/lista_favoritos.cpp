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
    lista_favorito(nullptr),
    tam_lista(0) // ← inicializamos
{
    if (id_canciones.empty()) {
        lista_favorito = nullptr;
        tam_lista = 0;
        return;
    }

    // contar IDs separados por ';'
    int cantidad = 1;
    for (size_t i = 0; i < id_canciones.size(); i++)
        if (id_canciones[i] == ';') ++cantidad;

    tam_lista = cantidad;                // ← GUARDAMOS el tamaño
    lista_favorito = new cancion*[tam_lista];
    for (int i = 0; i < tam_lista; ++i)
        lista_favorito[i] = nullptr;     // aún sin enlazar a canciones reales
}

//cargar lista de favoritos
void lista_favoritos::cargarListasFavoritos(lista_favoritos**& listaFavoritos, int& totalListas, cancion** canciones, int totalCanciones) {
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
        getline(frase, id_canciones);


        lista_favoritos* nuevaLista = new lista_favoritos(id_usuario, id_canciones);
        registrarMemoria<lista_favoritos>(1);
        nuevaLista->enlazarDesdeCadena(id_canciones, canciones, totalCanciones); // <--- ENLACE AQUÍ
        listaFavoritos[i++] = nuevaLista;
        incrementarIteraciones();
    }
    archivo.close();
}

// Busca linealmente una canción por id (texto) en el arreglo cargado
static cancion* findSongById(cancion** canciones, int total, const string& idTxt){
    for (int i=0;i<total;++i){
        if (canciones[i] && canciones[i]->getId_Cancion() == idTxt){
            return canciones[i];
        }
    }
    return 0;
}

// id_canciones: "100010101;100020202;100030303"
void lista_favoritos::enlazarDesdeCadena(const string& id_canciones,
                                         cancion** canciones, int totalCanciones)
{
    if (!lista_favorito) return;

    // Recorremos la cadena y vamos asignando secuencialmente
    int pos = 0;                // índice en lista_favorito
    string cur;
    for (size_t i=0;i<id_canciones.size();++i){
        char c = id_canciones[i];
        if (c==';'){
            if (!cur.empty()){
                cancion* ptr = findSongById(canciones, totalCanciones, cur);
                if (ptr) lista_favorito[pos] = ptr;
                ++pos;
                cur.clear();
            }
        } else if (c!=' ' && c!='\t' && c!='\r' && c!='\n'){
            cur.push_back(c);
        }
    }
    if (!cur.empty() && lista_favorito){ // último id (sin ';' final)
        cancion* ptr = findSongById(canciones, totalCanciones, cur);
        if (ptr && pos>=0) lista_favorito[pos] = ptr;
    }
}
