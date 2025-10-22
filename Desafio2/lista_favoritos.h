#ifndef LISTA_FAVORITOS_H
#define LISTA_FAVORITOS_H

#include <cancion.h>


class lista_favoritos
{
private:
    string nickname_seguidor;
    cancion **lista_favorito;
    int tam_lista;

public:
    bool agregar(int &id_cancion);
    bool eliminar(int &id_cancion);
    void fusionarDe(string &ListaFavoritos);
    static void cargarListasFavoritos(lista_favoritos**& listaFavoritos, int& totalListas, cancion** canciones, int totalCanciones);
    lista_favoritos(const string& id_usuario, const string& id_canciones);
    // NUEVO: enlaza ids de una cadena "id;id;id" con punteros reales
    void enlazarDesdeCadena(const string& id_canciones,
                            cancion** canciones, int totalCanciones);
    // === GETTERS NUEVOS ===
    const string& getNicknameSeguidor() const { return nickname_seguidor; }
    cancion** getLista() const { return lista_favorito; }
    int getTamLista() const { return tam_lista; }
};

#endif // LISTA_FAVORITOS_H
