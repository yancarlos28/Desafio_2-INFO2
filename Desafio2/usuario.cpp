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
                 const string& fechaInscripcion_)
    : nickname(nickname_),
    membresia(membresia_),
    ciudadUsuario(ciudadUsuario_),
    paisUsuario(paisUsuario_),
    fechaInscripcion(fechaInscripcion_),
    historialAleatorio(nullptr),
    historialFavoritos(nullptr),
    sigueA(nullptr)
{
    // Validar membresía (asumimos viene en minúsculas sin tildes)
    if (membresia != "premium" && membresia != "estandar")
        membresia = "estandar"; // valor por defecto

    // Configurar historiales vacíos
    configurarHistoriales(4, 6);
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
        getline(frase, fecha_Incripcion);

        usuario* nuevoUsuario = new usuario(nombre_usuario, membresia, ciudad, pais, fecha_Incripcion);
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

const string& usuario::getCiudad() const {
    return ciudadUsuario;
}

const string& usuario::getPais() const {
    return paisUsuario;
}

const string& usuario::getFecha() const {
    return fechaInscripcion;
}
