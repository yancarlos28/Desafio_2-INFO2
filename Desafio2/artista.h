#ifndef ARTISTA_H
#define ARTISTA_H

#include <string>
#include <iostream>
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

};

#endif // ARTISTA_H
