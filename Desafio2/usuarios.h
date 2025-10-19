#ifndef USUARIOS_H
#define USUARIOS_H

#include <string>
#include <cancion.h>
#include <lista_favoritos.h>

using namespace std;
class usuarios
{
private:
    string nickname;
    string membresia;
    string ciudad;
    string pais;
    string fecha;
    cancion **historial_aleatorio;
    cancion **historial_favoritos;
    lista_favoritos *sigueA;

public:
    usuarios();
    //control de reproduccion
    void iniciarReproduccion();
    void detenerReproduccion();
    void pasarSiguiente();
    void volverAnterior();
    void repetirCancion();

    //Busqueda y seguimiento
    cancion* buscar_cancion(int id9) const;
    bool seguir_listafavorita(lista_favoritos* lista_aSeguir);
    void dejar_seguir();

    //salida requeridas
    void mostrar_cancionUbicacion() const;

    //getter y setter
    bool esPremium() const;

    const string& getNickname() const { return nickname; }
    const string& getCiudad()   const { return ciudad; }
    const string& getPais()     const { return pais;   }
    const string& getFecha()    const { return fecha;  }

    void setNickname(const string& s);
    void setMembresia(const string& s);     // normaliza a "premium"/"estandar"
    void setCiudad(const string& s);
    void setPais(const string& s);
    void setFecha(const string& s);

    // Configurar capacidades de historiales (por defecto 4 y 6)
    void configurarHistoriales(int capAleatorio = 4, int capFav = 6);

    //seguir favoritos de &usuario

};

#endif // USUARIOS_H
