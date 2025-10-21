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
    string ruta;
    string creditos;
    string reproducciones;

public:

    //constructor
    cancion(const string& nombreCancion_,
            const string& id_cancion_,
            const string& duracion_,
            const string& ruta_,
            const string& creditos_,
            const string& reproducciones_);

    static void cargarCancion(cancion**& canciones, int& totalCanciones);
    void ruta_portada();
    // Getters
    const string& getNombreCancion() const;
    const string& getId_Cancion() const;
    const string& getRuta() const;
    const string& getCreditos() const;

};

#endif // CANCION_H
