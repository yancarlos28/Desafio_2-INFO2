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
    string generos;
    //lista de canciones

public:
    album();
    void cargarAlbumnes(album**& albumnes, int& totalAlbumnes);
};

#endif // ALBUM_H
