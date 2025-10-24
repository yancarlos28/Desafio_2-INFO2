#ifndef CANCION_H
#define CANCION_H

#include <string>
using namespace std;

class cancion
{
private:

    string nombreCancion;
    string id_cancion;
    string duracion;
    string ruta128;
    string ruta320;
    string creditos;
    string reproducciones;

public:

    //constructor
    cancion(const string& nombreCancion_,
            const string& id_cancion_,
            const string& duracion_,
            const string& ruta128_,
            const string& ruta320_,
            const string& creditos_,
            const string& reproducciones_);

    static void cargarCancion(cancion**& canciones, int& totalCanciones);

    // Getters
    const string& getNombreCancion() const;
    const string& getId_Cancion() const;
    const string& getCreditos() const;
    const string& getRuta128() const;
    const string& getRuta320() const;
    const string& getDuracion() const;
};

#endif // CANCION_H
