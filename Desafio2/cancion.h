#ifndef CANCION_H
#define CANCION_H

#include <string>
using namespace std;

class cancion
{
private:

    string nombre_cancion;
    int id_cancion;
    int duracion;
    string ruta_128;
    string ruta_320;
    long reproducciones;
public:
    //constructor
    cancion();
    static void cargarCancion(cancion**& canciones, int& totalCanciones);
    void ruta_portada();
};

#endif // CANCION_H
