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
    bool agregarPorIdTexto(const string& idTxt,
                           cancion** canciones, int totalCanciones);

    bool eliminarPorIdTexto(const string& idTxt);   // ya lo llamabas

    static bool guardarListasFavoritos(lista_favoritos** listas, int totalListas); // persistencia

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
// --- Persistencia: guarda todas las listas al archivo fuente ---
bool guardarListasFavoritos(lista_favoritos** listas, int totalListas);

#endif // LISTA_FAVORITOS_H
