#include <iostream>
#include <cancion.h>
#include <usuario.h>
#include <lista_favoritos.h>
#include <anuncio.h>

using namespace std;

int main()
{
    // 1) Carga de archivos

    cancion** canciones=0; int totalCanciones=0;
    cancion::cargarCancion(canciones, totalCanciones);

    usuario** usuarios=0; int totalUsuarios=0;
    usuario uLoader; uLoader.cargarUsuarios(usuarios, totalUsuarios);

    lista_favoritos** listas=0; int totalListas=0;
    lista_favoritos::cargarListasFavoritos(listas, totalListas, canciones, totalCanciones);

    anuncio** anuncios = 0; int totalAnuncios = 0;
    anuncio::cargarAnuncios(anuncios, totalAnuncios);

    if (totalUsuarios==0 || totalCanciones==0){
        cout << "Faltan datos para reproducir.\n";
        return 0;
    }

    // 2) Login de usuario



    return 0;
}
