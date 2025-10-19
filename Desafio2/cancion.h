#ifndef CANCION_H
#define CANCION_H

#include <string>
using namespace std;
class cancion
{
private:
    string nombre;
    int id_cancion;
    int duracion;
    string ruta_128;
    string ruta_320;
    long reproducciones;
public:
    cancion();
    void ruta_portada();
};

#endif // CANCION_H
