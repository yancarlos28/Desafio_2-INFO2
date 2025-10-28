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
    tam_lista(0)
{
    if (id_canciones.empty()) {
        lista_favorito = nullptr;
        tam_lista = 0;
        return;
    }

    // contar IDs separados por ';'
    int cantidad = 1;
    for (size_t i = 0; i < id_canciones.size(); i++) {
        char caracter = id_canciones[i];
        if (caracter == ';') {
            ++cantidad;
        }
        incrementarIteraciones();
    }

    tam_lista = cantidad;
    lista_favorito = new cancion*[tam_lista];
    registrarMemoria<cancion*>(tam_lista);
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
    while (getline(archivo, linea)) {
        total++;
        incrementarIteraciones();
    }
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
        nuevaLista->enlazarDesdeCadena(id_canciones, canciones, totalCanciones); // --- ENLACE AQUÍ
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
        incrementarIteraciones();
    }
    return 0;
}


// Compactar: deja solo punteros válidos y ajusta tam_lista
static void compactarLista(cancion**& arr, int& tam_lista) {
    int cnt = 0;
    for (int i = 0; i < tam_lista; ++i){

        if (arr[i]) ++cnt;
        incrementarIteraciones();
    }

    cancion** nuevo = (cnt > 0) ? new cancion*[cnt] : 0;
    registrarMemoria<cancion>(cnt);
    // ADDED: neutraliza registro erróneo y registra correctamente como punteros
    if (cnt > 0) {
        liberarMemoria<cancion>(cnt);         // neutraliza el conteo incorrecto
        registrarMemoria<cancion*>(cnt);      // cuenta el arreglo de punteros
    }
    int j = 0;
    for (int i = 0; i < tam_lista; ++i){

        if (arr[i]) nuevo[j++] = arr[i];
        incrementarIteraciones(); }

    if (arr != nullptr) {
        // liberar contabilidad correcta del arreglo anterior (punteros)
        // Nota: usamos tam_lista ANTES de reasignarlo a cnt
        delete[] arr;
        // neutraliza la liberación incorrecta que viene abajo (si existiera)
        registrarMemoria<cancion>(cnt);
        liberarMemoria<cancion>(cnt);         // neutraliza el registro previo errado
        liberarMemoria<cancion*>(tam_lista);  // libera correctamente el arreglo previo de punteros
        arr = nullptr;
    }
    arr = nuevo;
    tam_lista = cnt;
}


// Enlazar desde id_canciones: "100010101;100020202;100030303"
void lista_favoritos::enlazarDesdeCadena(const string& id_canciones,
                                         cancion** canciones, int totalCanciones){

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
        incrementarIteraciones();
    }

    // último id (sin ';' final)
    if (!cur.empty() && pos < tam_lista) {
        cancion* ptr = encontrarCancionporID(canciones, totalCanciones, cur);
        if (ptr) lista_favorito[pos++] = ptr;
    }

    // Compacta: quita nulos y ajusta tam_lista
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
        incrementarIteraciones();
    }

    // 3) Compactar antes de crecer (no huecos)
    compactarLista(lista_favorito, tam_lista);

    // 4) Redimensionar exacto +1 y appendea
    cancion** nuevo = new cancion*[tam_lista + 1];
    registrarMemoria<cancion>(1);
    // ADDED: neutraliza y registra correctamente
    liberarMemoria<cancion>(1);                         // neutraliza registro incorrecto
    registrarMemoria<cancion*>(tam_lista + 1);          // registra el arreglo de punteros completo

    for (int i = 0; i < tam_lista; ++i){

        nuevo[i] = lista_favorito[i];
        incrementarIteraciones(); }
    nuevo[tam_lista] = ptr;

    if (lista_favorito != nullptr) {
        delete [] lista_favorito;
        // ADDED: neutralizar y liberar correctamente el arreglo previo
        registrarMemoria<cancion>(tam_lista);           // añadimos para neutralizar la línea siguiente
        liberarMemoria<cancion>(tam_lista);             // neutraliza el conteo errado existente
        liberarMemoria<cancion*>(tam_lista);            // libera correctamente el arreglo previo de punteros
    }
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
        incrementarIteraciones();
    }
    if (pos < 0) return false; // no estaba

    // 2) crear nuevo arreglo exacto (tam_lista-1) copiando todo excepto pos
    cancion** nuevo = (tam_lista - 1 > 0) ? new cancion*[tam_lista - 1] : 0;
    registrarMemoria<cancion>(tam_lista - 1);
    // ADDED: neutraliza y registra correctamente
    if (tam_lista - 1 > 0) {
        liberarMemoria<cancion>(tam_lista - 1);         // neutraliza el conteo errado
        registrarMemoria<cancion*>(tam_lista - 1);      // registra arreglo de punteros
    }
    for (int i = 0, j = 0; i < tam_lista; ++i) {
        if (i == pos) continue;
        nuevo[j++] = lista_favorito[i];
        incrementarIteraciones();
    }

    // 3) reemplazar arreglo y actualizar tamaño lógico
    delete [] lista_favorito;
    //neutralizar y liberar correctamente el arreglo previo
    registrarMemoria<cancion>(tam_lista);               // añadimos para neutralizar la línea siguiente
    liberarMemoria<cancion>(tam_lista);                 // neutraliza
    liberarMemoria<cancion*>(tam_lista);                // libera correctamente el arreglo previo de punteros

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

        // Escribir ids separados por ';'
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

bool lista_favoritos::contienePtr(cancion** arr, int usados, cancion* c) const {
    if (!c) return false;
    for (int i = 0; i < usados; ++i) {
        if (arr[i] && (*arr[i] == *c)) return true; // ← usa operator==
    }
    return false;
}

// destructor — libera solo el arreglo de punteros interno
lista_favoritos::~lista_favoritos() {
    if (lista_favorito != nullptr) {
        delete[] lista_favorito;                    // libera el arreglo
        liberarMemoria<cancion*>(tam_lista);        // descuenta punteros, NO objetos
        lista_favorito = nullptr;
        tam_lista = 0;
    }
}

