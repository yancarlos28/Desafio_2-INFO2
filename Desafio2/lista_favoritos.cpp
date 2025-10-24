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
    ifstream archivo("listafavoritos.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de listafavoritos.txt\n";
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

// Busca linealmente una canción por id en el arreglo cargado
static cancion* encontrarCancionporID(cancion** canciones, int total, const string& idTxt){
    for (int i=0;i<total;++i){
        if (canciones[i] && canciones[i]->getId_Cancion() == idTxt){
            return canciones[i];
        }
    }
    return 0;
}
// Compactar: deja solo punteros válidos y ajusta tam_lista
static void compactarLista(cancion**& arr, int& tam_lista) {
    int cnt = 0;
    for (int i = 0; i < tam_lista; ++i) if (arr[i]) ++cnt;

    cancion** nuevo = (cnt > 0) ? new cancion*[cnt] : 0;
    int j = 0;
    for (int i = 0; i < tam_lista; ++i) if (arr[i]) nuevo[j++] = arr[i];

    if (arr) delete [] arr;
    arr = nuevo;
    tam_lista = cnt;
}

// Enlazar desde id_canciones: "100010101;100020202;100030303"
void lista_favoritos::enlazarDesdeCadena(const string& id_canciones,
                                         cancion** canciones, int totalCanciones)
{
    if (!lista_favorito || tam_lista <= 0) return;

    int pos = 0;
    string cur;

    for (size_t i = 0; i < id_canciones.size(); ++i) {
        char c = id_canciones[i];
        if (c == ';') {
            if (!cur.empty()) {
                cancion* ptr = encontrarCancionporID(canciones, totalCanciones, cur);
                if (ptr && pos < tam_lista) {
                    lista_favorito[pos++] = ptr;   // solo si hay espacio
                }
                cur.clear();
            }
        } else if (c!=' ' && c!='\t' && c!='\r' && c!='\n') {
            cur.push_back(c);
        }
    }

    // último id (sin ';' final)
    if (!cur.empty() && pos < tam_lista) {
        cancion* ptr = encontrarCancionporID(canciones, totalCanciones, cur);
        if (ptr) lista_favorito[pos++] = ptr;
    }

    // 🧹 Compacta: quita nulos y ajusta tam_lista
    compactarLista(lista_favorito, tam_lista);
}

// Agregar por ID SIN huecos (realloc exacto +1)
bool lista_favoritos::agregarPorIdTexto(const string &idTxt,
                                        cancion** canciones, int totalCanciones)
{
    if (idTxt.empty() || !canciones || totalCanciones <= 0) return false;

    // 1) Resolver puntero de canción
    cancion* ptr = encontrarCancionporID(canciones, totalCanciones, idTxt);
    if (!ptr) return false;  // ID no existe en catálogo

    // 2) Evitar duplicado
    for (int i = 0; i < tam_lista; ++i) {
        if (lista_favorito[i] && lista_favorito[i] == ptr) return false;
    }

    // 3) Compactar antes de crecer (no huecos)
    compactarLista(lista_favorito, tam_lista);

    // 4) Redimensionar exacto +1 y appendea
    cancion** nuevo = new cancion*[tam_lista + 1];
    for (int i = 0; i < tam_lista; ++i) nuevo[i] = lista_favorito[i];
    nuevo[tam_lista] = ptr;

    if (lista_favorito) delete [] lista_favorito;
    lista_favorito = nuevo;
    tam_lista += 1;

    return true;
}

// Eliminar por ID SIN huecos (realloc exacto -1)
bool lista_favoritos::eliminarPorIdTexto(const string& idTxt)
{
    if (!lista_favorito || tam_lista <= 0) return false;

    // 1) localizar índice a eliminar por id de texto
    int pos = -1;
    for (int i = 0; i < tam_lista; ++i) {
        if (lista_favorito[i] && lista_favorito[i]->getId_Cancion() == idTxt) {
            pos = i; break;
        }
    }
    if (pos < 0) return false; // no estaba

    // 2) crear nuevo arreglo exacto (tam_lista-1) copiando todo excepto pos
    cancion** nuevo = (tam_lista - 1 > 0) ? new cancion*[tam_lista - 1] : 0;
    for (int i = 0, j = 0; i < tam_lista; ++i) {
        if (i == pos) continue;
        nuevo[j++] = lista_favorito[i];
    }

    // 3) reemplazar arreglo y actualizar tamaño lógico
    delete [] lista_favorito;
    lista_favorito = nuevo;
    tam_lista -= 1;

    return true;
}

bool guardarListasFavoritos(lista_favoritos** listas, int totalListas)
{
    // Validaciones básicas
    if (!listas || totalListas <= 0) {
        // Si no hay listas, aun así truncamos el archivo a vacío para ser consistentes
        ofstream vacio("listafavoritos.txt", ios::out | ios::trunc);
        return vacio.good();
    }

    ofstream out("listafavoritos.txt", ios::out | ios::trunc);
    if (!out.is_open()) {
        std::cout << "No se pudo abrir listafavoritos.txt para escritura.\n";
        return false;
    }

    // Recorremos todas las listas y serializamos: nickname,id;id;id
    for (int i = 0; i < totalListas; ++i) {
        incrementarIteraciones();

        lista_favoritos* lf = listas[i];
        if (!lf) continue;

        const string& nick = lf->getNicknameSeguidor();
        cancion** arr = lf->getLista();
        int n = lf->getTamLista();

        out << nick << ",";  // siempre escribimos el nickname

        // Escribir ids separados por ';' (solo punteros válidos)
        bool primero = true;
        for (int j = 0; j < n; ++j) {
            incrementarIteraciones();

            if (!arr || !arr[j]) continue;
            const string& id = arr[j]->getId_Cancion();
            if (id.empty()) continue;

            if (!primero) out << ';';
            out << id;
            primero = false;
        }
        out << "\n";
    }

    out.flush();
    return out.good();
}



