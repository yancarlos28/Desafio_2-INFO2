#ifndef LISTA_FAVORITOS_H
#define LISTA_FAVORITOS_H

#include <cancion.h>


class lista_favoritos
{
private:
    string nickname_seguidor;
    cancion **lista_favorito;

public:
    bool agregar(int &id_cancion);
    bool eliminar(int &id_cancion);
    void fusionarDe(string &ListaFavoritos);
    void cargarListasFavoritos(lista_favoritos**& listaFavoritos, int& totalListas);

};

#endif // LISTA_FAVORITOS_H
