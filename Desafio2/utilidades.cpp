#include "memoria.h"
#include "usuario.h"
#include "anuncio.h"
#include "album.h"
#include "artista.h"
#include "utilidades.h"
#include "lista_favoritos.h"
#include <limits>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

/**Definicion de variables globales para almacenar memoria
*/

int contadorIteracionesGlobal = 0;
size_t memoriaReservadaGlobal = 0;

/*BUSQUEDA Y VALIDACION-----------
 */

usuario* buscarUsuarioPorNickname(usuario** usuarios, int totalUsuarios, const string& nick) {
    if (!usuarios) return nullptr;
    for (int i = 0; i < totalUsuarios; ++i) {
        usuario* u = usuarios[i];
        if (u && u->getNickname() == nick) return u;
    }
    return nullptr;
}

lista_favoritos* buscarListaDeFavoritos(const usuario* u,
                                        lista_favoritos* const* listas, int totalListas)
{
    if (!u || !listas || totalListas <= 0) return nullptr;
    const string& nick = u->getNickname();
    for (int i = 0; i < totalListas; ++i) {
        lista_favoritos* L = listas[i];
        if (L && L->getNicknameSeguidor() == nick) return L; // usa tu getter real
    }
    return nullptr;
}

static bool existeCancionId(cancion** canciones, int total, const string& idTxt) {
    if (!canciones || total <= 0) return false;
    for (int i = 0; i < total; ++i) {
        if (canciones[i] && canciones[i]->getId_Cancion() == idTxt) return true;
    }
    return false;
}


const album* buscarAlbumPorId7(album** albumnes, int totalAlb, const string& id7){
    if(!albumnes) return nullptr;
    for(int i=0;i<totalAlb;++i){
        const album* a = albumnes[i];
        if(a && a->getId_Album()==id7) return a;
    }
    return nullptr;
}

const artista* buscarArtistaPorId5(artista** artistas, int totalArt, const string& id5){
    if(!artistas) return nullptr;
    for(int i=0;i<totalArt;++i){
        const artista* ar = artistas[i];
        if(ar && ar->getId_Artista()==id5) return ar;
    }
    return nullptr;
}


int validad_entero(int min, int max) {
    int opcion;
    bool valido = false;

    while (!valido) {
        cout << "Ingrese una opcion (" << min << "-" << max << "): ";
        cin >> opcion;

        if (cin.fail() || opcion < min || opcion > max) {
            cout << "Entrada invalida. Por favor, ingrese un numero entre "
                 << min << " y " << max << ".\n";

            // limpiar el estado del flujo
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            valido = true;
        }
    }

    return opcion;
}

//para sacar id del artista en base al id de la cancion
string copiarPrimeros5(const string& original) {
    return original.substr(0, 5);
}

//para sacar id del album en base al id de la cancion
string copiarPrimeros7(const string& original) {
    return original.substr(0, 7);
}

string idArtistaDeCancion(const std::string& id9){
    return (id9.size() >= 5) ? id9.substr(0,5) : "";
}
string idAlbum7DeCancion(const std::string& id9){
    return (id9.size() >= 7) ? id9.substr(0,7) : "";
}

// Flujo de login y menu
void flujoLoginYMenu(
    cancion**& canciones, int& totalCanciones,
    usuario**& usuarios, int& totalUsuarios,
    lista_favoritos**& listasFavoritos, int& totalListasFavoritos,
    anuncio**& anuncios, int& totalAnuncios,album **albumnes, int totalAlbumnes, artista **artistas, int totalArtistas
    ) {
    cout << "Ingresa tu nickname: ";
    string nick;
    getline(cin, nick);
    if (nick.empty()) {
        cout << "Nickname vacío. Saliendo.\n";
        return;
    }

    usuario* u = buscarUsuarioPorNickname(usuarios, totalUsuarios, nick);
    if (!u) {
        cout << "No existe el usuario '" << nick << "'.\n";
        return;
    }

    // Mostrar membresía y ciudad-país (si tienes getters)
    cout << "Hola, " << u->getNickname() << "  [Membresia: " << u->getMembresia() << "]\n";

    // Obtener su lista de favoritos
    lista_favoritos* miLista = buscarListaDeFavoritos(u, listasFavoritos, totalListasFavoritos);
    u->setListaFavoritos(miLista);
    if (!miLista) {
        cout << "(Advertencia) No tienes lista de favoritos cargada.\n";
    }

    // Despachar a menú por membresía
    const string mem = u->getMembresia();
    if (mem == "premium") {
        mostrarMenuPremium(u, usuarios, totalUsuarios, canciones, totalCanciones, listasFavoritos, totalListasFavoritos, albumnes, totalAlbumnes, artistas, totalArtistas, anuncios, totalAnuncios);
    } else {
        mostrarMenuEstandar(u, totalUsuarios, canciones, totalCanciones, albumnes, totalAlbumnes, artistas, totalArtistas, anuncios, totalAnuncios);
    }

    // Mostrar métricas al salir del menú
    cout << "\n=== METRICAS ===\n";
    mostrarUsoMemoria();
    cout << "================\n";
}


// =====================
// MENU USUARIO ESTÁNDAR
// =====================

void mostrarMenuEstandar(usuario* usuarioActual, int totalUsuarios,
                         cancion**& canciones, int& totalCanciones,
                         album** albumnes, int totalAlbumnes,
                         artista** artistas, int totalArtistas,
                         anuncio**& anuncios,int & totalAnuncios)
{
    bool exit = false;
    while (!exit) {
        incrementarIteraciones();
        cout << "------------------------------\n";
        cout << "MENU USUARIO ESTANDAR\n";
        cout << "------------------------------\n";
        cout << "1. Reproduccion aleatoria \n";
        cout << "2. Volver al menu principal\n";

        int opc = validad_entero(1, 2);

        if (opc == 1) {
            reproduccionAleatoria(usuarioActual, totalCanciones, canciones, artistas, totalArtistas, albumnes,totalAlbumnes, anuncios, totalAnuncios);

            mostrarUsoMemoria();
        }
        else {
            exit = true;
        }
    }
}

// =====================
// MENU USUARIO PREMIUM
// =====================
void mostrarMenuPremium(usuario* usuarioActual,usuario** usuarios, int totalUsuarios,
                        cancion**& canciones, int& totalCanciones,
                        lista_favoritos**& listadefavoritos, int& totalListas,
                        album** albumnes, int totalAlbumnes,
                        artista** artistas, int totalArtistas,
                        anuncio**& anuncios,int & totalAnuncios)
{
    bool salir = false;

    while (!salir) {
        incrementarIteraciones();

        cout << "------------------------------\n";
        cout << "MENU USUARIO PREMIUM\n";
        cout << "------------------------------\n";
        cout << "1. Reproduccion aleatoria\n";
        cout << "2. Menu lista de favoritos\n";
        cout << "3. Volver al menu principal\n";

        int opc = validad_entero(1, 3);

        if (opc == 1) {
            cout << "\n[Premium] Reproducción aleatoria de todas las canciones...\n";
            reproduccionAleatoria(usuarioActual, totalCanciones, canciones, artistas, totalArtistas, albumnes,totalAlbumnes, anuncios, totalAnuncios);
        }
        else if (opc == 2) {
            bool volverFavs = false;
            while (!volverFavs) {
                cout << "\n--- LISTA DE FAVORITOS (PREMIUM) ---\n";
                cout << "1. Editar mi lista de favoritos\n";
                cout << "2. Seguir otra lista de favoritos\n";
                cout << "3. Ejecutar mi lista de favoritos\n";
                cout << "4. Volver al menu anterior\n";

                int opc_2 = validad_entero(1, 4);

                if (opc_2 == 1) {
                    // ==== EDITAR MI LISTA: agregar/eliminar por ID ====
                    lista_favoritos* mi = buscarListaDeFavoritos(usuarioActual, listadefavoritos, totalListas);
                    if (!mi) {
                        cout << "No se encontró una lista de favoritos para este usuario.\n";
                        continue;
                    }

                    bool volverEd = false;
                    while (!volverEd) {
                        cout << "\n[Editar mis favoritos]\n";
                        cout << "1) Agregar por ID\n";
                        cout << "2) Eliminar por ID\n";
                        cout << "3) Volver\n";
                        int opEd = validad_entero(1, 3);

                        if (opEd == 1) {
                            cout << "ID de cancion a AGREGAR: ";
                            string id;
                            if (cin.peek()=='\n') cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            std::getline(cin, id);

                            if (!existeCancionId(canciones, totalCanciones, id)) {
                                cout << "ID no existe en el catalogo.\n";
                                continue;
                            }
                            if (mi->agregarPorIdTexto(id, canciones, totalCanciones)) {
                                cout << "Agregada.\n";
                                if (!guardarListasFavoritos(listadefavoritos, totalListas)) {
                                    cout << "Error al guardar cambios\n";
                                } else {
                                    cout <<"Cambios guardados. \n";
                                }
                            } else {
                                cout << "No se pudo agregar (duplicado o error).\n";
                            }
                        }
                        else if (opEd == 2) {
                            cout << "ID de cancion a ELIMINAR: ";
                            string id;
                            if (cin.peek()=='\n') cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            std::getline(cin, id);

                            if (mi->eliminarPorIdTexto(id)) {
                                cout << " Eliminada.\n";
                                if (!guardarListasFavoritos(listadefavoritos, totalListas)) {
                                    cout << " No se pudo guardar cambios.txt\n";
                                } else{
                                    cout <<"Cambios guardados \n";
                                }
                            } else {
                                cout << "No se pudo eliminar (ID no está en tu lista).\n";
                            }
                        }
                        else { // opEd == 3
                            volverEd = true;
                        }
                    }
                }
                else if (opc_2 == 2) {
                    int opc_3;
                    cout <<"1. Seguir usuario nuevo: "<< endl;
                    cout <<"2. Eliminar usuario existente: "<< endl;
                    cin >>opc_3;
                    if (opc_3==1){
                            cout << "Nickname a seguir: ";
                            string nick;
                            cin >> nick;
                            seguirListaFavoritosPorNickname(usuarios, totalUsuarios,
                                                            listadefavoritos, totalListas,
                                                            usuarioActual, nick);
                    }
                    else if (opc_3==2){
                        dejarDeSeguirLista(usuarioActual, usuarios, totalUsuarios);

                    }

                }
                else if (opc_2 == 3) {
                    cout << "\n--- Ejecutar mis favoritos ---\n";
                    cout << "1. Secuencial\n";
                    cout << "2. Aleatoria\n";
                    int modo = validad_entero(1, 2);

                    if (modo == 1) {
                        ejecutarMiListaFavoritosSecuencial(usuarioActual, listadefavoritos, totalListas,
                                                           albumnes, totalAlbumnes, artistas, totalArtistas);
                    } else {
                        ejecutarMiListaFavoritosAleatorioPremium(usuarioActual, listadefavoritos, totalListas,
                                                                 albumnes, totalAlbumnes, artistas, totalArtistas);
                    }
                }
                else { // opc_2 == 4
                    volverFavs = true;
                }
            }
        }
        else { // opc == 3
            salir = true;
        }

        cout << "\n";
    }
}


