#ifndef ALBUM_H
#define ALBUM_H

#include <string>
using namespace std;

class album
{
private:

    string id_album;
    string duracion;
    string puntuacion;
    string nombre_album;
    string fechaLanzamiento;
    string ruta_Imagen;
    string generos;
    string sello;

public:

    album(const string& nombre_album_,
          const string& id_album_,
          const string& duracion_,
          const string& fechaLanzamiento_,
          const string& rutaImagen_,
          const string& generos_,
          const string& puntuacion_,
          const string& sello_);

    static void cargarAlbumnes(album**& albumnes, int& totalAlbumnes);

    // Getters
    const string& getId_Album() const;
    const string& getRuta_Imagen() const;
    const string& getNombre_Album() const;
};

#endif // ALBUM_H
