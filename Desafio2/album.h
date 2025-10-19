#ifndef ALBUM_H
#define ALBUM_H

#include <string>
using namespace std;
class album
{
private:
    int id_album;
    string nombre_album;
    string sello;
    string fechaLanzamiento;
    string ruta_portada;
    int puntuacion;
    //lista de canciones

public:
    album();
};

#endif // ALBUM_H
