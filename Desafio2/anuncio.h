#ifndef ANUNCIO_H
#define ANUNCIO_H

#include <string>
using namespace std;

class anuncio
{
private:
    string mensaje;
    int peso;
public:
    anuncio();
    void cargarAnuncios(anuncio**& anuncios, int& totalAnuncios);
};

#endif // ANUNCIO_H
