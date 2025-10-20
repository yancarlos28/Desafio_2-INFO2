#ifndef ARTISTA_H
#define ARTISTA_H

#include <string>

using namespace std;

class artista
{

private:
    int id_artista;
    int edad;
    string paisOrigen;
    long seguidores;
    int rankingTendencia;

public:
    artista();
    int totalAlbums();
    void cargarArtistas(artista**& artistas, int& totalArtistas);
};

#endif // ARTISTA_H
