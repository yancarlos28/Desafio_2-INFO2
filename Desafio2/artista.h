#ifndef ARTISTA_H
#define ARTISTA_H

#include <string>

using namespace std;

class artista
{

private:

    string nombre_artista;
    string id_artista;
    string edad;
    string paisOrigen;
    string seguidores;
    string puestoRanking;
    string albumes;

public:

    artista(const string& nombre_artista_,
            const string& id_artista_,
            const string& edad_,
            const string& paisOrigen_,
            const string& seguidores_,
            const string& puestoRanking_,
            const string& albumes_);

    static void cargarArtistas(artista**& artistas, int& totalArtistas);

    // Getters
    const string& getNombre_Artista() const;
    const string& getId_Artista() const;
    const string& getAlbumes() const;

};

#endif // ARTISTA_H
