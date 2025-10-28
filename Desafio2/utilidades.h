#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <string>
#include <usuario.h>
#include <anuncio.h>
#include <cancion.h>
#include <lista_favoritos.h>
#include <album.h>
#include <artista.h>

// ===== Tipos auxiliares compartidos =====
struct MetaDeCancion {
    std::string nombreAlbum;
    std::string rutaPortada;
    std::string nombreArtista;
    bool ok() const {
        return !nombreAlbum.empty() && !rutaPortada.empty() && !nombreArtista.empty();
    }
};

// ===== Login, búsqueda y validación =====
void flujoLoginYMenu(cancion**& canciones, int& totalCanciones,
                     usuario**& usuarios, int& totalUsuarios,
                     lista_favoritos**& listasFavoritos, int& totalListasFavoritos,
                     anuncio**& anuncios, int& totalAnuncios,
                     album** albumnes, int totalAlbumnes,
                     artista** artistas, int totalArtistas);

const album*   buscarAlbumPorId7  (album** albumnes,   int totalAlb, const std::string& id7);
const artista* buscarArtistaPorId5(artista** artistas, int totalArt, const std::string& id5);
usuario*       buscarUsuarioPorNickname(usuario** usuarios, int totalUsuarios, const std::string& nick);

lista_favoritos* buscarListaDeFavoritos(const usuario* u,
                                        lista_favoritos* const* listas, int totalListas);

// ===== Seguir/Dejar de seguir listas =====
bool seguirListaFavoritosPorNickname(usuario** usuarios, int totalUsuarios,
                                     lista_favoritos** listas, int totalListas,
                                     usuario* seguidor, const std::string& nickObjetivo);

void dejarDeSeguirLista(usuario* seguidor, usuario** usuarios, int totalUsuarios);

// ===== Utilidades de ID =====
std::string idArtistaDeCancion (const std::string& id9);
std::string idAlbum7DeCancion  (const std::string& id9);
std::string copiarPrimeros5    (const std::string& original);
std::string copiarPrimeros7    (const std::string& original);

// ===== Entrada validada =====
int validad_entero(int min, int max);

// ===== Reproducir una (elige 128/320 según membresía) =====
void reproducirCancion(usuario* user,
                       cancion** canciones, int totalCanciones,
                       artista** artistas, int totalArtistas,
                       album** albumes, int totalAlbumes);

// ===== Reproducción aleatoria (estándar/premium) =====
void mostrarAnuncio(anuncio**& anuncios, int& totalAnuncios);

void reproduccionAleatoria(usuario* user, int totalCanciones,
                           cancion** canciones,
                           artista** artistas, int totalArtistas,
                           album** albumes, int totalAlbumes,
                           anuncio**& anuncios, int& totalAnuncios);

void mostrarMenuEstandar(usuario* usuarioActual, int totalUsuarios,
                         cancion**& canciones, int& totalCanciones,
                         album** albumnes, int totalAlbumnes,
                         artista** artistas, int totalArtistas,
                         anuncio**& anuncios, int& totalAnuncios);

// ===== Menú premium y favoritos =====
void mostrarMenuPremium(usuario* usuarioActual, usuario** usuarios, int totalUsuarios,
                        cancion**& canciones, int& totalCanciones,
                        lista_favoritos**& listadefavoritos, int& totalListas,
                        album** albumnes, int totalAlbumnes,
                        artista** artistas, int totalArtistas,
                        anuncio**& anuncios, int& totalAnuncios);

void ejecutarMiListaFavoritosSecuencial(usuario* user,
                                        lista_favoritos** listas, int totalListas,
                                        album** albumnes, int totalAlbumnes,
                                        artista** artistas, int totalArtistas);

void ejecutarMiListaFavoritosAleatorioPremium(usuario* user,
                                              lista_favoritos** listas, int totalListas,
                                              album** albumnes, int totalAlbumnes,
                                              artista** artistas, int totalArtistas);

// ===== Persistencia =====
bool guardarUsuarios(usuario** usuarios, int totalUsuarios, const std::string& ruta = "usuarios.txt");

// ===== Meta desde ID de canción (prototipo compartido) =====
MetaDeCancion resolverMetaDesdeIdCancion(const std::string& idCancion,
                                         album** albumnes, int totalAlbumnes,
                                         artista** artistas, int totalArtistas);

#endif // UTILIDADES_H
