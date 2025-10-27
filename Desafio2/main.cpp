#include <cancion.h>
#include <usuario.h>
#include <lista_favoritos.h>
#include <anuncio.h>
#include <album.h>
#include <utilidades.h>
#include <artista.h>
#include "memoria.h"
#include "liberar_memoria.h"

using namespace std;

int main()
{
    // 1) Carga de archivos

    cancion** canciones=0; int totalCanciones=0;
    cancion::cargarCancion(canciones, totalCanciones);

    usuario** usuarios = 0;   int totalUsuarios = 0;
    usuario::cargarUsuarios(usuarios, totalUsuarios);

    lista_favoritos** listas=0; int totalListas=0;
    lista_favoritos::cargarListasFavoritos(listas, totalListas, canciones, totalCanciones);

    anuncio** anuncios = 0; int totalAnuncios = 0;
    anuncio::cargarAnuncios(anuncios, totalAnuncios);

    album** albumes = 0; int totalAlbumnes = 0;
    album::cargarAlbumnes(albumes, totalAlbumnes);

    artista** artistas = 0; int totalArtistas = 0;
    artista::cargarArtistas(artistas, totalArtistas);


    // 2) LOGIN + MENU

    flujoLoginYMenu(canciones, totalCanciones, usuarios, totalUsuarios, listas, totalListas, anuncios, totalAnuncios, albumes, totalAlbumnes, artistas, totalArtistas);
    //cout << "\n=== Métricas antes de liberar ===\n";
    mostrarUsoMemoria();

    liberarArregloDePunteros(listas, totalListas);
    //cout << "[Tras liberar listas] "; mostrarUsoMemoria();

    liberarArregloDePunteros(usuarios, totalUsuarios);
    //cout << "[Tras liberar usuarios] "; mostrarUsoMemoria();

    liberarArregloDePunteros(canciones, totalCanciones);
    //cout << "[Tras liberar canciones] "; mostrarUsoMemoria();

    liberarArregloDePunteros(albumes, totalAlbumnes);
    //cout << "[Tras liberar albumes] "; mostrarUsoMemoria();

    liberarArregloDePunteros(artistas, totalArtistas);
    //cout << "[Tras liberar artistas] "; mostrarUsoMemoria();

    liberarArregloDePunteros(anuncios, totalAnuncios);
    //cout << "[Tras liberar anuncios] "; mostrarUsoMemoria();

    //cout << "\n=== Métricas tras liberar todo ===\n";
    mostrarUsoMemoria();

    return 0;
}
