#include "usuario.h"
#include "lista_favoritos.h"
#include "cancion.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <memoria.h>
using namespace std;



// Constructor parametrizado (membresía en minúsculas, sin tildes)
// -----------------------------------------------------------
usuario::usuario(const string& nickname_,
                 const string& membresia_,
                 const string& ciudadUsuario_,
                 const string& paisUsuario_,
                 const string& fechaInscripcion_,
                 const string &nicknameAseguir_)
    : nickname(nickname_),
    membresia(membresia_),
    ciudadUsuario(ciudadUsuario_),
    paisUsuario(paisUsuario_),
    fechaInscripcion(fechaInscripcion_),
    nicknameAseguir(nicknameAseguir_),
    //historialAleatorio(nullptr),
    //historialFavoritos(nullptr),
    sigueA(nullptr),
    favoritos(nullptr)
{
    // Validar membresía (asumimos viene en minúsculas sin tildes)
    if (membresia != "premium" && membresia != "estandar")
        membresia = "estandar"; // valor por defecto

    // Configurar historiales vacíos
    //configurarHistoriales(4, 6);
}

void usuario::cargarUsuarios(usuario**& usuarios, int& totalUsuarios) {

    ifstream archivo("usuarios.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de usuarios.txt\n";
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

    usuarios = new usuario*[total];
    registrarMemoria<usuario*>(total);

    totalUsuarios = total;
    int i = 0;

    // Leer y construir cada objeto cancion
    while (getline(archivo, linea)) {
        stringstream frase(linea);
        string nombre_usuario, membresia, ciudad, pais, fecha_Incripcion, nicknameAseguir;
        getline(frase, nombre_usuario, ',');
        getline(frase, membresia, ',');
        getline(frase, ciudad, ',');
        getline(frase, pais, ',');
        getline(frase, fecha_Incripcion,',');
        getline(frase, nicknameAseguir);

        usuario* nuevoUsuario = new usuario(nombre_usuario, membresia, ciudad, pais, fecha_Incripcion,nicknameAseguir);
        registrarMemoria<usuario>(1);
        usuarios[i++] = nuevoUsuario;
        incrementarIteraciones();
    }
    archivo.close();
}

// -------------------- Getters --------------------
const string& usuario::getNickname() const {
    return nickname;
}

const string& usuario::getMembresia() const {
    return membresia;
}
const string& usuario::getNicknameAseguir() const {
    return nicknameAseguir;
}

const string& usuario::getCiudad() const {
    return ciudadUsuario;
}

const string& usuario::getPais() const {
    return paisUsuario;
}

const string& usuario::getFecha() const {
    return fechaInscripcion;
}
void usuario::setNicknameAseguir(const string& nick) {
    nicknameAseguir = nick;
}

//Implementaciones

bool usuario::seguir_listafavorita(lista_favoritos* lista_aSeguir)
{
    // Validaciones simples
    if (!lista_aSeguir) return false;

    // Si ya la sigo, no hago nada
    if (sigueA == lista_aSeguir) return false;

    // Enlazo por puntero (no se copia nada)
    sigueA = lista_aSeguir;
    return true;
}

void usuario::dejar_seguir()
{
    // Desenlazar
    sigueA = nullptr;
}
void usuario::recorrerFavoritosFusion(void (*visitor)(cancion*)) const
{
    if (!visitor) return;
    if (!favoritos) return;

    // Preparar arrays fuente
    int tamPropio   = favoritos->getTamLista();
    cancion** arrP  = favoritos->getLista();

    int tamSeguido  = (sigueA ? sigueA->getTamLista() : 0);
    cancion** arrS  = (sigueA ? sigueA->getLista()     : nullptr);

    // Reservar un arreglo temporal para “vistos” (máximo = propio + seguido)
    int cap = tamPropio + tamSeguido;
    if (cap <= 0) cap = 1;

    cancion** vistos = new cancion*[cap];
    registrarMemoria<cancion*>(cap);
    int nVistos = 0;

    // Helper local: ¿ya vimos este ID?
    auto yaVisto = [&](const string& id)->bool {
        for (int j = 0; j < nVistos; ++j) {
            // Comparamos por ID para evitar duplicados aunque los punteros difieran
            if (vistos[j] && vistos[j]->getId_Cancion() == id) return true;
        }
        return false;
    };

    // 1) Primero, tus canciones
    for (int i = 0; i < tamPropio; ++i) {
        cancion* s = arrP ? arrP[i] : nullptr;
        if (!s) continue;
        const string& id = s->getId_Cancion();
        if (yaVisto(id)) continue;

        vistos[nVistos++] = s;
        visitor(s);
        incrementarIteraciones();
    }

    // 2) Luego, canciones del seguido (si hay)
    for (int i = 0; i < tamSeguido; ++i) {
        cancion* s = arrS ? arrS[i] : nullptr;
        if (!s) continue;
        const string& id = s->getId_Cancion();
        if (yaVisto(id)) continue;

        vistos[nVistos++] = s;
        visitor(s);
        incrementarIteraciones();
    }

    delete[] vistos;
    liberarMemoria<cancion*>(cap);
}
