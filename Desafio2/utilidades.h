#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <usuario.h>
#include <anuncio.h>
#include <cancion.h>
#include <lista_favoritos.h>
#include <album.h>
#include <artista.h>
#include <string>


//Login, busqueda y validacion

void flujoLoginYMenu(cancion**& canciones, int& totalCanciones, usuario**& usuarios, int& totalUsuarios,
                     lista_favoritos**& listasFavoritos, int& totalListasFavoritos,
                     anuncio**& anuncios, int& totalAnuncios, album **albumnes, int totalAlbumnes, artista **artistas, int totalArtistas);


usuario* buscarUsuarioPorNickname(usuario** usuarios, int totalUsuarios, const string& nick);


lista_favoritos* buscarListaDeFavoritos(const usuario* u,
                                        lista_favoritos* const* listas, int totalListas);


bool seguirListaFavoritosPorNickname(usuario** usuarios, int totalUsuarios,
                                     lista_favoritos** listas, int totalListas,
                                     usuario* seguidor, const string& nickObjetivo);


void dejarDeSeguirLista(usuario* seguidor, usuario **usuarios, int totalUsuarios);

// Imprime (o reproduce) la "vista fusionada" (propia + seguida) sin duplicados.
void imprimirFavoritosFusion(usuario* u);

int validad_entero(int min, int max);

//menu estandar
void reproduccionAleatoria(usuario* user,
                           int totalCanciones,
                           cancion** canciones,
                           artista** artistas,
                           int totalArtistas,
                           album** albumes,
                           int totalAlbumes,
                           anuncio**& anuncios,
                           int& totalAnuncios);
void mostrarMenuEstandar(usuario* usuarioActual, int totalUsuarios,
                         cancion**& canciones, int& totalCanciones,
                         album** albumnes, int totalAlbumnes,
                         artista** artistas, int totalArtistas,
                         anuncio**& anuncios,int & totalAnuncios);

//menu premium
void mostrarMenuPremium(usuario* usuarioActual, usuario **usuarios, int totalUsuarios, cancion**& canciones, int& totalCanciones,
                        lista_favoritos**& listadefavoritos, int& totalListas, album **albumnes, int totalAlbumnes, artista **artistas, int totalArtistas, anuncio **&anuncios, int &totalAnuncios);

void ejecutarMiListaFavoritosSecuencial(usuario* user,
                                        lista_favoritos** listas, int totalListas,
                                        album** albumnes, int totalAlbumnes,
                                        artista** artistas, int totalArtistas);

void ejecutarMiListaFavoritosAleatorioPremium(usuario* user,
                                              lista_favoritos** listas, int totalListas,
                                              album** albumnes, int totalAlbumnes,
                                              artista** artistas, int totalArtistas);

// utilidades.h
bool guardarUsuarios(usuario** usuarios, int totalUsuarios, const std::string& ruta = "usuarios.txt");


#endif // UTILIDADES_H
