#ifndef LISTA_FAVORITOS_H
#define LISTA_FAVORITOS_H

#include <cancion.h>
#include <string>

using namespace std;

class lista_favoritos
{
private:

    string nickname_seguidor;
    cancion **lista_favorito;
    int tam_lista;

public:

    lista_favoritos(const string& id_usuario, const string& id_canciones);
    static void cargarListasFavoritos(lista_favoritos**& listaFavoritos, int& totalListas, cancion** canciones, int totalCanciones);
    void enlazarDesdeCadena(const string& id_canciones,
                            cancion** canciones, int totalCanciones);

    bool agregarPorIdTexto(const string& idTxt,
                           cancion** canciones, int totalCanciones);
    bool eliminarPorIdTexto(const string& idTxt);

    // ---------Getters--------
    const string& getNicknameSeguidor() const { return nickname_seguidor; }
    cancion** getLista() const { return lista_favorito; }
    int getTamLista() const { return tam_lista; }


};

bool guardarListasFavoritos(lista_favoritos** listas, int totalListas);

#endif // LISTA_FAVORITOS_H
