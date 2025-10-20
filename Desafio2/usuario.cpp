#include "usuario.h"
#include "lista_favoritos.h"
#include "cancion.h"
#include <cstdio>   // printf
#include <cctype>   // std::tolower
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <memoria.h>
using namespace std;

// ---------------------- Helpers internos ----------------------

// Normaliza string a minúsculas in-place (sin usar <algorithm>)
static void to_lower_ascii(std::string& s){
    for (size_t i=0;i<s.size();++i){
        unsigned char c = static_cast<unsigned char>(s[i]);
        s[i] = static_cast<char>(std::tolower(c));
    }
}

// Inserta en buffer circular: escribe en pos (len % cap), luego len++
static void push_hist(cancion**& buf, int& len, const int cap, cancion* c){
    if (!buf || cap<=0 || !c) return;
    int pos = (len % cap);
    buf[pos] = c;
    ++len;
}

// Devuelve puntero a la “entrada idxLogico” desde el final en un buffer circular.
// idxLogico=0 -> última insertada; 1 -> anterior; etc.
// Retorna nullptr si no existe.
static cancion* get_hist_at_logical(cancion* const* buf, int len, int cap, int idxLogico){
    if (!buf || cap<=0 || len<=0) return nullptr;
    int usados = (len < cap ? len : cap);
    if (idxLogico < 0 || idxLogico >= usados) return nullptr;

    int lastIdx = (len - 1) % cap;
    int idxFis  = lastIdx - idxLogico;
    if (idxFis < 0) idxFis += cap;
    return buf[idxFis];
}

// ---------------------- Implementación ------------------------

usuarios::usuarios()
    : historial_aleatorio(nullptr), histA_len(0), histA_cap(0),
    historial_favoritos(nullptr), histF_len(0), histF_cap(0),
    sigueA(nullptr), repetirFlag(0)
{
    // Por defecto
    membresia = "estandar";
}

usuarios::~usuarios(){
    // NO destruimos canciones ni la lista seguida (no ownership)
    if (historial_aleatorio) delete[] historial_aleatorio;
    if (historial_favoritos) delete[] historial_favoritos;
}

// ---------------------- Setters ----------------------

void usuario::setNickname(const std::string& s){ nickname = s; }
void usuario::setCiudad(const std::string& s){ ciudad = s; }
void usuario::setPais(const std::string& s){ pais = s; }
void usuario::setFecha(const std::string& s){ fecha = s; }

void usuario::setMembresia(const std::string& s){
    membresia = s;
    to_lower_ascii(membresia);
    // aceptar variantes comunes
    if (membresia == "premium" || membresia == "premiun") {
        membresia = "premium";
    } else {
        membresia = "estandar";
    }
}

// ---------------------- Configuración de historiales ----------------------

void usuario::configurarHistoriales(int capAleatorio, int capFav){
    if (historial_aleatorio) { delete[] historial_aleatorio; historial_aleatorio = nullptr; }
    if (historial_favoritos) { delete[] historial_favoritos; historial_favoritos = nullptr; }
    histA_len = histF_len = 0;

    if (capAleatorio > 0){
        historial_aleatorio = new cancion*[capAleatorio];
        for (int i=0;i<capAleatorio;++i) historial_aleatorio[i] = nullptr;
        histA_cap = capAleatorio;
    } else histA_cap = 0;

    if (capFav > 0){
        historial_favoritos = new cancion*[capFav];
        for (int i=0;i<capFav;++i) historial_favoritos[i] = nullptr;
        histF_cap = capFav;
    } else histF_cap = 0;
}

// ---------------------- Lógica de reproducción (stubs) ----------------------

void usuario::iniciarReproduccion(){
    // Aquí resetearías estados, prepararías cola, etc.
}

void usuario::detenerReproduccion(){
    // Aquí detienes y limpias estado actual de reproducción si lo manejas.
}

void usuario::pasarSiguiente(){
    // En tu implementación real: selecciona la siguiente canción y haz play.
    // Ejemplo de registrar en historial aleatorio (simulado):
    cancion* actual = nullptr; // TODO: apunta a la canción realmente reproducida
    if (actual && histA_cap > 0){
        push_hist(historial_aleatorio, histA_len, histA_cap, actual);
    }
}

void usuario::volverAnterior(){
    // Recupera la inmediatamente anterior a la última reproducida en aleatorio
    cancion* anterior = get_hist_at_logical(historial_aleatorio, histA_len, histA_cap, 1);
    if (anterior){
        // TODO: posiciona reproducción en 'anterior'
        // Opcional: empujarla otra vez al historial si tu UX lo requiere
    } else {
        // printf("No hay anterior disponible.\n");
    }
}

void usuario::repetirCancion(){
    repetirFlag = !repetirFlag; // toggle
}

// ---------------------- Búsqueda / seguimiento ----------------------

cancion* usuario::buscar_cancion(int id9) const {
    (void)id9;
    // Debería consultar a tu repositorio/catálogo. Stub:
    return nullptr;
}

bool usuario::seguir_listafavorita(lista_favoritos* listaAjena){
    sigueA = listaAjena; // solo referencia
    return (sigueA != nullptr);
}

void usuario::dejar_seguir(){
    sigueA = nullptr;
}

// ---------------------- Salida requerida ----------------------

void usuario::mostrar_cancionUbicacion() const {
    // Debe imprimir ruta de audio (128/320 según membresía) y portada.
    // Aquí usamos un stub de canción actual:
    const cancion* actual = nullptr; // TODO: apunta a la canción en reproducción
    if (!actual) {
        // printf("No hay canción en reproducción.\n");
        return;
    }

    // Asumiendo estas firmas en cancion:
    // const char* getRuta128() const; const char* getRuta320() const;
    // const char* getPortadaPNG() const; const char* getNombre() const;

    const char* ruta   = esPremium() ? actual->getRuta320() : actual->getRuta128();
    const char* portada= actual->getPortadaPNG();
    const char* nombre = actual->getNombre();

    std::printf("Reproduciendo: %s\nAudio: %s\nPortada: %s\n", nombre, ruta, portada);
}

// ---------------------- Utilidades ----------------------

bool usuario::esPremium() const {
    // miembros normalizados a "premium" o "estandar"
    return membresia == "premium";
}

void usuario::cargarUsuarios(usuario**& usuarios, int& totalUsuarios) {

    ifstream archivo("usuario.txt");
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

    usuarios = new usuario*[total];
    registrarMemoria<usuario*>(total);

    totalUsuarios = total;
    int i = 0;

    // Leer y construir cada objeto cancion
    while (getline(archivo, linea)) {
        stringstream frase(linea);
        string nombre_usuario, membresia, ciudad, pais, fecha_Incripcion;
        getline(frase, nombre_usuario, ',');
        getline(frase, membresia, ',');
        getline(frase, ciudad, ',');
        getline(frase, pais, ',');
        getline(frase, fecha_Incripcion, ',');

        usuario* nuevoUsuario = new usuario(nombre_usuario, membresia, ciudad, pais, fecha_Incripcion);
        registrarMemoria<usuario>(1);
        usuarios[i++] = nuevoUsuario;
        incrementarIteraciones();
    }
    archivo.close();
}
