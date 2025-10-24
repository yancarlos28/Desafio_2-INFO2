#ifndef USUARIO_H
#define USUARIO_H

#include <cancion.h>
#include <lista_favoritos.h>
#include <string>

using namespace std;

class usuario
{
private:

    string nickname;
    string membresia;
    string ciudadUsuario;
    string paisUsuario;
    string fechaInscripcion;
    //cancion **historialAleatorio;
    //cancion **historialFavoritos;
    lista_favoritos *sigueA;
    lista_favoritos *favoritos;

public:

    //constructor parametrizado
    usuario(const string& nickname_,
            const string& membresia_,
            const string& ciudadUsuario_,
            const string& paisUsuario_,
            const string& fechaInscripcion_);

    //cargar datos suarios
    static void cargarUsuarios(usuario**& usuarios, int& totalUsuarios);


    // Getters
    const string& getNickname() const;
    const string& getMembresia() const;
    lista_favoritos* getListaFavoritos() const { return favoritos; }
    lista_favoritos* getListaSeguido() const { return sigueA; }
    void setListaFavoritos(lista_favoritos* lf) { favoritos = lf; }

    //Busqueda y seguimiento
    bool seguir_listafavorita(lista_favoritos* lista_aSeguir);
    void dejar_seguir();
    void recorrerFavoritosFusion(void (*visitor)(cancion*)) const;


    // Configurar capacidades de historiales (por defecto 4 y 6)
    //void configurarHistoriales(int capAleatorio = 4, int capFav = 6);


};

#endif // USUARIO_H
