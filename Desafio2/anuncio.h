#ifndef ANUNCIO_H
#define ANUNCIO_H

#include <string>
using namespace std;

class anuncio
{
private:

    string tipo_anuncio;
    string mensaje;

public:

    anuncio(const string& tipo_anuncio_, const string& mensaje_);
    static void cargarAnuncios(anuncio**& mensaje_, int& totalAnuncios);

    // Getters
    const string& getTipo_Anuncio() const;
    const string& getMensaje() const;
};

#endif // ANUNCIO_H
