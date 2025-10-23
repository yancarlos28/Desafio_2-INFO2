#ifndef USUARIO_H
#define USUARIO_H

#include <string>
#include <cancion.h>
#include <lista_favoritos.h>

using namespace std;
class usuario
{
private:
    string nickname;
    string membresia;
    string ciudadUsuario;
    string paisUsuario;
    string fechaInscripcion;
    cancion **historialAleatorio;
    cancion **historialFavoritos;
    lista_favoritos *sigueA;

public:
    //constructor
    usuario();
    usuario(const string& nickname_,
            const string& membresia_,
            const string& ciudadUsuario_,
            const string& paisUsuario_,
            const string& fechaInscripcion_);

    //cargar datos
    void cargarUsuarios(usuario**& usuarios, int& totalUsuarios);
    //control de reproduccion
    void iniciarReproduccion();
    void detenerReproduccion();
    void pasarSiguiente();
    void volverAnterior();
    void repetirCancion();

    //Busqueda y seguimiento
    cancion* buscar_cancion(string id_cancion) const;
    bool seguir_listafavorita(lista_favoritos* lista_aSeguir);
    void dejar_seguir();

    //salida requeridas
    void mostrar_cancionUbicacion() const;

    //getter y setter

    // Getters
    const string& getNickname() const;
    const string& getMembresia() const;
    const string& getCiudad() const;
    const string& getPais() const;
    const string& getFecha() const;


    // Configurar capacidades de historiales (por defecto 4 y 6)
    void configurarHistoriales(int capAleatorio = 4, int capFav = 6);

    //seguir favoritos de &usuario

};

#endif // USUARIO_H
