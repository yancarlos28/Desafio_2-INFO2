#include "memoria.h"
#include "usuario.h"
#include "anuncio.h"
#include "album.h"
#include "artista.h"
#include "utilidades.h"
#include "lista_favoritos.h"
#include <chrono>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <thread>
#include <fstream>
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

static int randEnRango(int minimo, int maximo) {
    static bool seeded = false;
    if (!seeded) { srand((unsigned)time(0)); seeded = true; }
    if (maximo < minimo) return minimo;                 // guardia
    return minimo + rand() % (maximo - minimo + 1);
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

struct MetaDeCancion {
    string nombreAlbum;
    string rutaPortada;
    string nombreArtista;
    bool ok() const {
        return !nombreAlbum.empty() && !rutaPortada.empty() && !nombreArtista.empty(); }
};

MetaDeCancion resolverMetaDesdeIdCancion(
    const string& idCancion,
    album** albumnes, int totalAlbumnes,
    artista** artistas, int totalArtistas)
{
    MetaDeCancion meta;

    const string id5 = idArtistaDeCancion(idCancion);
    const string id7 = idAlbum7DeCancion(idCancion);

    const album* a = buscarAlbumPorId7(albumnes, totalAlbumnes, id7);
    const artista* ar = buscarArtistaPorId5(artistas, totalArtistas, id5);

    if(a){
        meta.nombreAlbum = a->getNombre_Album();
        meta.rutaPortada = a->getRuta_Imagen();
    }
    if(ar){
        meta.nombreArtista = ar->getNombre_Artista();
    }
    return meta;
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

// ------------- REPRODUCCION -----------
void reproducirCancion(usuario* user,
                       cancion** canciones,
                       int totalCanciones,
                       artista** artistas,
                       int totalArtistas,
                       album** albumes,
                       int totalAlbumes){

    if (totalCanciones <= 0) return;
    int var = randEnRango(0, totalCanciones - 1);


    cancion* ptrCancion = canciones[var];
    string nombreCancion = ptrCancion->getNombreCancion();
    string ruta_128_Cancion = ptrCancion->getRuta128();
    string ruta_320_Cancion = ptrCancion->getRuta320();
    string duracion_Cancion = ptrCancion->getDuracion();
    string id_Cancion = ptrCancion->getId_Cancion();
    string id_Artista = copiarPrimeros5(id_Cancion);
    string id_Album = copiarPrimeros7(id_Cancion);

    string artistaNombre = "";
    string nombreAlbum = "";
    string rutaAlbum="";

    for (int i = 0; i < totalArtistas; ++i) {
        artista* ptrArtista = artistas[i];
        if(ptrArtista->getId_Artista()==id_Artista){
            artistaNombre = ptrArtista->getNombre_Artista();
        }
    }

    for (int i = 0; i < totalAlbumes; ++i) {
        album* ptrAlbum = albumes[i];
        if(ptrAlbum->getId_Album()==id_Album){
            nombreAlbum = ptrAlbum->getNombre_Album();
            rutaAlbum = ptrAlbum->getRuta_Imagen();
        }
    }

    //reproduccion aleatorio para standard
    if(user->getMembresia() == "estandar"){

        cout << "========Reproduciendo========\n";
        cout << "\n";
        cout << "Cantante: "<< artistaNombre << "\n";
        cout << "Album: "<< nombreAlbum << "\n";
        cout << "Ruta a la portada: "<< rutaAlbum << "\n";
        cout << "Titulo de la cancion: "<< nombreCancion << "\n";
        cout << "Ruta al archivo del audio: "<< ruta_128_Cancion << "\n";
        cout << "Duracion: "<< duracion_Cancion << "\n";
    }

    //reproduccion aleatorio para premium
    if(user->getMembresia() == "premium"){

        cout << "========Reproduciendo========\n";

        cout << "Cantante: "<< artistaNombre << ""<< endl;
        cout << "Album: "<< nombreAlbum << ""<< endl;
        cout << "Ruta a la portada: "<< rutaAlbum << ""<< endl;
        cout << "Titulo de la cancion: "<< nombreCancion << ""<< endl;
        cout << "Ruta al archivo del audio: "<< ruta_320_Cancion << ""<< endl;
        cout << "Duracion: "<< duracion_Cancion << ""<< endl;
    }
};


void mostrarAnuncio(anuncio**& anuncios, int& totalAnuncios)
{
    if (!anuncios || totalAnuncios <= 0) return;

    // No repetir el mismo consecutivamente
    static anuncio* anuncioAnterior = nullptr;
    anuncio* anuncioActual = nullptr;

    // Contar cuántos anuncios hay por categoría
    int totalTipoC = 0;
    int totalTipoB = 0;
    int totalTipoAAA = 0;

    for (int i = 0; i < totalAnuncios; ++i) {
        anuncio* a = anuncios[i];
        if (!a) continue;

        const string& tipo = a->getTipo_Anuncio();
        if (tipo == "C") ++totalTipoC;
        else if (tipo == "B") ++totalTipoB;
        else ++totalTipoAAA;
    }

    // Elegir categoría según prioridad 1:2:3 (C:B:AAA)
    auto elegirCategoria = [&]() -> char {
        int dado = randEnRango(1, 6);  // 1–6 para proporción 1:2:3
        if (dado == 1) return 'C';         // 1/6 ≈ 16.7 %
        else if (dado <= 3) return 'B';    // 2/6 ≈ 33.3 %
        else return 'A';                   // 3/6 ≈ 50 %
    };

    char categoriaSeleccionada = elegirCategoria();

    // Seleccionar un anuncio aleatorio de la categoría elegida
    for (int intentos = 0; intentos < 10 && !anuncioActual; ++intentos) {
        int indiceAleatorio = randEnRango(0, totalAnuncios - 1);
        anuncio* candidato = anuncios[indiceAleatorio];
        if (!candidato) continue;

        const std::string& tipo = candidato->getTipo_Anuncio();
        bool coincideCategoria =
            (categoriaSeleccionada == 'C' && tipo == "C") ||
            (categoriaSeleccionada == 'B' && tipo == "B") ||
            (categoriaSeleccionada == 'A' && tipo == "AAA");

        // Evitar repetir el mismo anuncio consecutivamente
        if (coincideCategoria && candidato != anuncioAnterior) {
            anuncioActual = candidato;
        }
    }

    // Si no se encontró con los intentos aleatorios, buscar secuencialmente
    if (!anuncioActual) {
        for (int i = 0; i < totalAnuncios; ++i) {
            anuncio* candidato = anuncios[i];
            if (!candidato) continue;

            const std::string& tipo = candidato->getTipo_Anuncio();
            bool coincideCategoria =
                (categoriaSeleccionada == 'C' && tipo == "C") ||
                (categoriaSeleccionada == 'B' && tipo == "B") ||
                (categoriaSeleccionada == 'A' && tipo == "AAA");

            if (coincideCategoria && candidato != anuncioAnterior) {
                anuncioActual = candidato;
                break;
            }
        }
    }

    // Mostrar anuncio elegido
    if (anuncioActual) {
        cout << "\n--- ANUNCIO ---\n";
        cout << "[" << anuncioActual->getTipo_Anuncio() << "] "
                  << anuncioActual->getMensaje() << "\n";
        anuncioAnterior = anuncioActual;
    }
}



void reproduccionAleatoria(usuario* user,
                           int totalCanciones,
                           cancion** canciones,
                           artista** artistas,
                           int totalArtistas,
                           album** albumes,
                           int totalAlbumes,
                           anuncio**& anuncios,
                           int& totalAnuncios)
{
    if (!user || !canciones || totalCanciones <= 0) {
        std::cout << "(No hay canciones para reproducir)\n";
        return;
    }

    // ===== Modo ESTANDAR pedido =====
    if (user->getMembresia() == "estandar") {
        int desdeUltAnuncio = 0;

        // 1) Mostrar las primeras 5
        const int K = 5;
        for (int i = 0; i < K; ++i) {
            reproducirCancion(user, canciones, totalCanciones, artistas, totalArtistas, albumes, totalAlbumes);
            ++desdeUltAnuncio;
            std::this_thread::sleep_for(std::chrono::seconds(3));

            if (desdeUltAnuncio == 2 && totalAnuncios > 0) {
                mostrarAnuncio(anuncios, totalAnuncios);
                std::this_thread::sleep_for(std::chrono::seconds(3));
                desdeUltAnuncio = 0;
            }
            incrementarIteraciones();
        }

        // 2) Luego: ir pidiendo siguiente o detener
        while (true) {
            std::cout << "[N] siguiente   [S] detener\n";
            if (std::cin.peek()=='\n')
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            char op = 0;
            std::cin >> op;

            if (op=='S' || op=='s') {
                std::cout << "Se detuvo la reproduccion.\n";
                break;
            }

            // N o cualquier otra tecla => siguiente
            reproducirCancion(user, canciones, totalCanciones, artistas, totalArtistas, albumes, totalAlbumes);
            ++desdeUltAnuncio;
            std::this_thread::sleep_for(std::chrono::seconds(3));

            if (desdeUltAnuncio == 2 && totalAnuncios > 0) {
                mostrarAnuncio(anuncios, totalAnuncios);
                std::this_thread::sleep_for(std::chrono::seconds(3));
                desdeUltAnuncio = 0;
            }
        }
        return;
    }

    // ===== Premium (con controles N/P/R/S y SIN anuncios) =====
    // Historial “atrás” con límite M_PREV, sin “adelante”. No guardamos posiciones: solo cancion*.
    {

        cancion** src = canciones;
        const int tam = totalCanciones;

        // Contar válidos
        int validos = 0;
        for (int i = 0; i < tam; ++i) { if (src[i]) ++validos; incrementarIteraciones(); }
        if (validos <= 0) { cout << "(No hay canciones válidas para reproducir)\n"; return; }

        // Historial atrás (tamaño exacto en cada cambio)
        const int M_PREV = 6;
        cancion** pilaAtras = nullptr;
        int nAtras = 0;

        auto pushAtras = [&](cancion* c){
            if (!c) return;
            if (nAtras < M_PREV) {
                cancion** nuevo = new cancion*[nAtras + 1];
                registrarMemoria<cancion*>(nAtras + 1);
                for (int i = 0; i < nAtras; ++i) { nuevo[i] = pilaAtras[i]; incrementarIteraciones(); }
                nuevo[nAtras] = c;
                if (pilaAtras) { delete[] pilaAtras; liberarMemoria<cancion*>(nAtras); }
                pilaAtras = nuevo; nAtras += 1;
            } else {
                cancion** nuevo = new cancion*[M_PREV];
                registrarMemoria<cancion*>(M_PREV);
                for (int i = 1; i < M_PREV; ++i) { nuevo[i-1] = pilaAtras[i]; incrementarIteraciones(); }
                nuevo[M_PREV-1] = c;
                if (pilaAtras) { delete[] pilaAtras; liberarMemoria<cancion*>(M_PREV); }
                pilaAtras = nuevo; nAtras = M_PREV;
            }
        };

        auto popAtras = [&]()->cancion*{
            if (nAtras == 0) return nullptr;
            cancion* top = pilaAtras[nAtras-1];
            if (nAtras-1 > 0) {
                cancion** nuevo = new cancion*[nAtras - 1];
                registrarMemoria<cancion*>(nAtras - 1);
                for (int i = 0; i < nAtras-1; ++i) { nuevo[i] = pilaAtras[i]; incrementarIteraciones(); }
                delete[] pilaAtras; liberarMemoria<cancion*>(nAtras);
                pilaAtras = nuevo; nAtras -= 1;
            } else {
                delete[] pilaAtras; liberarMemoria<cancion*>(nAtras);
                pilaAtras = nullptr; nAtras = 0;
            }
            return top;
        };

        auto estaEnHist = [&](cancion* x)->bool {
            for (int i = 0; i < nAtras; ++i) { if (pilaAtras[i] == x) return true; incrementarIteraciones(); }
            return false;
        };

        // Escoger siguiente aleatoria evitando la actual y el historial atrás
        auto escogerSiguiente = [&](cancion* actual)->int {
            if (validos == 1) {
                for (int i = 0; i < tam; ++i) if (src[i]) return i;
                return -1;
            }
            int elegido = -1;
            for (int intentos = 0; intentos < 64; ++intentos) {
                int r = randEnRango(0, tam - 1);
                cancion* c = src[r];
                if (!c) continue;
                if (c == actual) continue;
                if (!estaEnHist(c)) { elegido = r; break; }
                incrementarIteraciones();
            }
            if (elegido < 0) {
                for (int i = 0; i < tam; ++i) { if (src[i] && src[i] != actual && !estaEnHist(src[i])) { elegido = i; break; } incrementarIteraciones(); }
            }
            if (elegido < 0) {
                for (int i = 0; i < tam; ++i) { if (src[i]) { elegido = i; break; } incrementarIteraciones(); }
            }
            return elegido;
        };

        // Primera canción
        int idxActual = escogerSiguiente(nullptr);
        if (idxActual < 0) { cout << "(No hay canción para reproducir)\n"; return; }

        bool repetir = false;
        bool salir   = false;

        cout << "\n[Premium] Reproducción aleatoria con controles (N/P/R/S)\n";
        while (!salir) {
            cancion* c = src[idxActual];

            // ---- Mostrar meta (artista, álbum, portada) en 320 kbps ----
            {
                string id = c->getId_Cancion();
                string id5 = copiarPrimeros5(id);
                string id7 = copiarPrimeros7(id);

                string artistaNombre = "";
                string nombreAlbum   = "";
                string rutaAlbum     = "";

                for (int i = 0; i < totalArtistas; ++i) {
                    artista* ptrArtista = artistas[i];
                    if (ptrArtista && ptrArtista->getId_Artista() == id5) { artistaNombre = ptrArtista->getNombre_Artista(); break; }
                    incrementarIteraciones();
                }
                for (int i = 0; i < totalAlbumes; ++i) {
                    album* ptrAlbum = albumes[i];
                    if (ptrAlbum && ptrAlbum->getId_Album() == id7) { nombreAlbum = ptrAlbum->getNombre_Album(); rutaAlbum = ptrAlbum->getRuta_Imagen(); break; }
                    incrementarIteraciones();
                }

                cout << "======== Reproduciendo ========\n";
                cout << "Cantante: " << artistaNombre << "\n";
                cout << "Album:   " << nombreAlbum   << "\n";
                cout << "Portada: " << rutaAlbum     << "\n";
                cout << "Titulo:  " << c->getNombreCancion() << "\n";
                cout << "Ruta:    " << c->getRuta320()       << "\n";
                cout << "Duracion:" << c->getDuracion()      << "\n\n";
            }
            // -------------------------------------------------------------

            cout << "[N] siguiente   [P] previa   [R] repetir:" << (repetir ? "ON" : "OFF") << "   [S] salir\n";
            this_thread::sleep_for(chrono::seconds(3));

            char op = 0;
            if (cin.peek()!='\n') cin >> op;
            if (cin.peek()=='\n') cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (op=='S' || op=='s') {
                salir = true;
                break;
            }
            else if (op=='R' || op=='r') {
                // Repetir solo alterna flag. No tocamos historial ni índice.
                repetir = !repetir;
                continue;
            }
            else if (op=='P' || op=='p') {
                // Retroceder: tomar última reproducida del historial (si hay).
                cancion* anterior = popAtras();
                if (anterior) {
                    int found = -1;
                    for (int i = 0; i < tam; ++i) { if (src[i] == anterior) { found = i; break; } incrementarIteraciones(); }
                    if (found >= 0) { idxActual = found; repetir = false; continue; }
                    else { cout << "(No se pudo ubicar la canción anterior)\n"; }
                } else {
                    cout << "(No hay previa)\n";
                }
                // Si no había previa o falló, dejaremos que caiga a “siguiente” abajo.
            }

            // Siguiente (N u otra tecla que no sea R/S/P)
            if (!repetir) {
                // ⚠️ Empujar al historial SOLO cuando realmente avanzamos
                pushAtras(c);
            }
            int nextIdx = escogerSiguiente(c);
            if (nextIdx >= 0) idxActual = nextIdx;
            incrementarIteraciones();
        }

        if (pilaAtras) { delete[] pilaAtras; liberarMemoria<cancion*>(nAtras); }
        cout << "[INFO] Se detuvo la reproducción (premium).\n";
        return;
    }

}



void ejecutarMiListaFavoritosSecuencial(usuario* user,
                                        lista_favoritos** listas, int totalListas,
                                        album** albumnes, int totalAlbumnes,
                                        artista** artistas, int totalArtistas)
{
    if (!user || !listas || totalListas <= 0) {
        std::cout << "No hay listas de favoritos cargadas.\n";
        return;
    }

    // 1) Buscar la lista del usuario actual por nickname
    lista_favoritos* miLista = 0;
    const string nick = user->getNickname();
    for (int i = 0; i < totalListas; ++i) {
        if (listas[i] && listas[i]->getNicknameSeguidor() == nick) {
            miLista = listas[i];
            break;
        }
        incrementarIteraciones();
    }
    if (!miLista) { cout << "No se encontró una lista de favoritos para este usuario.\n"; return; }
    // ==== SIEMPRE MODO FUSIONADO ====
    // Mi lista
    cancion** arrP = miLista->getLista();
    int       tamP = miLista->getTamLista();

    // Lista del seguido (si existe)
    cancion** arrS = nullptr;
    int       tamS = 0;
    {
        const string nickSeguido = user->getNicknameAseguir(); // getter existente
        if (!nickSeguido.empty()) {
            // Buscar DIRECTO en 'listas' por nickname (no dependemos de usuarios[])
            for (int i = 0; i < totalListas; ++i) {
                if (listas[i] && listas[i]->getNicknameSeguidor() == nickSeguido) {
                    arrS = listas[i]->getLista();
                    tamS = listas[i]->getTamLista();
                    break;
                }
                incrementarIteraciones();
            }
        }
    }

    // Si no hay seguida válida, usamos solo la propia
    cancion** fav = nullptr;
    int       tam = 0;
    bool      esFusion = false;

    if (!arrS || tamS <= 0) {
        fav = arrP; tam = tamP;
    } else {
        // Fusión temporal sin duplicados (por ID). Solo punteros; no se replican canciones.
        int cap = tamP + tamS; if (cap <= 0) cap = 1;
        cancion** fusion = new cancion*[cap];
        registrarMemoria<cancion*>(cap);
        for (int i = 0; i < cap; ++i) fusion[i] = nullptr;

        auto existeId = [&](const string& id)->bool {
            for (int k = 0; k < cap; ++k) { if (fusion[k] && fusion[k]->getId_Cancion() == id) return true; }
            return false;
        };

        int nFusion = 0;
        // 1) Primero, mis favoritas
        for (int i = 0; i < tamP; ++i) {
            cancion* c = (arrP ? arrP[i] : nullptr);
            if (!c) { incrementarIteraciones(); continue; }
            const string& id = c->getId_Cancion();
            if (id.empty() || existeId(id)) { incrementarIteraciones(); continue; }
            fusion[nFusion++] = c; incrementarIteraciones();
        }
        // 2) Luego, las del seguido
        for (int i = 0; i < tamS; ++i) {
            cancion* c = (arrS ? arrS[i] : nullptr);
            if (!c) { incrementarIteraciones(); continue; }
            const string& id = c->getId_Cancion();
            if (id.empty() || existeId(id)) { incrementarIteraciones(); continue; }
            fusion[nFusion++] = c; incrementarIteraciones();
        }

        if (nFusion == 0) {
            delete[] fusion; liberarMemoria<cancion*>(cap);
            cout << "No hay canciones para reproducir (ni propias ni seguidas).\n"; return;
        }
        fav = fusion; tam = nFusion; esFusion = true;
    }
    // 2) Tomar solo punteros válidos
    if (!fav || tam <= 0) { cout << "Tu lista de favoritos está vacía.\n"; return; }

    int validos = 0;
    for (int i = 0; i < tam; ++i) {
        if (fav[i]) ++validos;
        incrementarIteraciones();
    }
    if (validos == 0) { cout << "Tu lista no contiene canciones válidas.\n"; return; }

    // 3) Compactar índices válidos
    int* idxs = new int[validos];
    registrarMemoria<int>(validos);
    int p = 0;
    for (int i = 0; i < tam; ++i) {
        if (fav[i]) idxs[p++] = i;
        incrementarIteraciones();
    }

    // === Historial atrás (solo retroceso), sin "adelante" ===
    const int M_PREV = 6;     // máximo de pasos hacia atrás que guardamos

    cancion** pilaAtras = nullptr;
    int nAtras = 0;

    auto pushAtras = [&](cancion* c){
        if (c == nullptr) return;
        if (nAtras < M_PREV) {
            cancion** nuevo = new cancion*[nAtras + 1];
            registrarMemoria<cancion*>(nAtras + 1);
            for (int i = 0; i < nAtras; ++i) { nuevo[i] = pilaAtras[i]; incrementarIteraciones(); }
            nuevo[nAtras] = c;
            if (pilaAtras) { delete[] pilaAtras; liberarMemoria<cancion*>(nAtras); }
            pilaAtras = nuevo; nAtras += 1;
        } else {
            // lleno: descartamos el más viejo y metemos c al final
            cancion** nuevo = new cancion*[M_PREV];
            registrarMemoria<cancion*>(M_PREV);
            for (int i = 1; i < M_PREV; ++i) { nuevo[i-1] = pilaAtras[i]; incrementarIteraciones(); }
            nuevo[M_PREV-1] = c;
            if (pilaAtras) { delete[] pilaAtras; liberarMemoria<cancion*>(M_PREV); }
            pilaAtras = nuevo; nAtras = M_PREV;
        }
    };

    auto popAtras = [&]()->cancion*{
        if (nAtras == 0) return nullptr;
        cancion* top = pilaAtras[nAtras-1];
        if (nAtras-1 > 0) {
            cancion** nuevo = new cancion*[nAtras - 1];
            registrarMemoria<cancion*>(nAtras - 1);
            for (int i = 0; i < nAtras-1; ++i) { nuevo[i] = pilaAtras[i]; incrementarIteraciones(); }
            delete[] pilaAtras; liberarMemoria<cancion*>(nAtras);
            pilaAtras = nuevo; nAtras -= 1;
        } else {
            delete[] pilaAtras; liberarMemoria<cancion*>(nAtras);
            pilaAtras = nullptr; nAtras = 0;
        }
        return top;
    };
    // === fin historial ===

    int cursor = 0;           // índice en idxs[]
    bool salir = false;

    cout << "\n=== Mi lista de favoritos (SECUENCIAL – PREMIUM) ===\n";
    while (!salir) {
        if (cursor < 0) cursor = 0;
        if (cursor >= validos) cursor = validos - 1;

        cancion* c = fav[ idxs[cursor] ];
        if (c) {
            auto meta = resolverMetaDesdeIdCancion(c->getId_Cancion(),
                                                   albumnes, totalAlbumnes,
                                                   artistas, totalArtistas);
            cout << "======== Reproduciendo ========\n";
            if (meta.ok()) {
                cout << "Cantante: " << meta.nombreArtista << "\n";
                cout << "Album:   " << meta.nombreAlbum   << "\n";
                cout << "Portada: " << meta.rutaPortada   << "\n";
            }
            cout << "Titulo:  " << c->getNombreCancion() << "\n";
            const string& rutaAudio = (user->getMembresia() == "premium") ? c->getRuta320() : c->getRuta128();
            cout << "Ruta:    " << rutaAudio << "\n";
            cout << "Duracion:" << c->getDuracion() << "\n\n";
        }

        cout << "[N] siguiente   [P] previa   [R] repetir   [S] salir\n";
        if (cin.peek()=='\n') cin.ignore(numeric_limits<streamsize>::max(), '\n');
        char op = 0; cin >> op;

        if (op=='S' || op=='s') {
            salir = true;

        } else if (op=='R' || op=='r') {
            // repetir: no movemos el cursor ni tocamos historial

        } else if (op=='N' || op=='n') {
            if (cursor + 1 < validos) {
                // el actual queda en historial hacia atrás
                pushAtras( fav[idxs[cursor]] );
                ++cursor;
            } else {
                cout << "(Fin de la lista)\n";
            }

        } else if (op=='P' || op=='p') {
            // ir a la canción anterior del historial (si existe)
            cancion* anterior = popAtras();
            if (anterior) {
                // ubicar la canción anterior en fav (por puntero)
                int nuevoIdx = -1;
                for (int i = 0; i < validos; ++i) {
                    if (fav[idxs[i]] == anterior) { nuevoIdx = i; break; }
                    incrementarIteraciones();
                }
                if (nuevoIdx >= 0) cursor = nuevoIdx;
                else cout << "(No se pudo ubicar la canción anterior)\n";
            } else {
                cout << "(No hay previa)\n";
            }

        } else {
            // cualquier otra tecla => no hace nada especial
        }
    }

    // liberar arreglos locales
    delete [] idxs;
    liberarMemoria<int>(validos);

    if (pilaAtras){ delete[] pilaAtras; liberarMemoria<cancion*>(nAtras); }
    if (esFusion) { delete[] fav; liberarMemoria<cancion*>(tam); }


    cout << "[INFO] Fin del recorrido de tu lista de favoritos.\n";
}



void ejecutarMiListaFavoritosAleatorioPremium(usuario* user,
                                              lista_favoritos** listas, int totalListas,
                                              album** albumnes, int totalAlbumnes,
                                              artista** artistas, int totalArtistas)
{
    // 0) Validaciones y restricción de premium
    if (!user || !listas || totalListas <= 0) { cout << "No hay listas de favoritos cargadas.\n"; return; }
    if (user->getMembresia() != "premium") { cout << "(Solo disponible para usuarios premium)\n"; return; }

    // 1) Localizar mi lista por nickname
    lista_favoritos* miLista = 0;
    const string nick = user->getNickname();
    for (int i = 0; i < totalListas; ++i) {
        if (listas[i] && listas[i]->getNicknameSeguidor() == nick) { miLista = listas[i]; break; }
    }
    if (!miLista || miLista->getTamLista() <= 0) { cout << "Tu lista de favoritos está vacía.\n"; return; }
    // ==== SIEMPRE MODO FUSIONADO ====
    // Mi lista
    cancion** arrP = miLista->getLista();
    int       tamP = miLista->getTamLista();

    // Posible lista del seguido (si existe en 'listas' por nicknameAseguir)
    cancion** arrS = nullptr;
    int       tamS = 0;
    {
        const string nickSeguido = user->getNicknameAseguir();
        if (!nickSeguido.empty()) {
            for (int i = 0; i < totalListas; ++i) {
                if (listas[i] && listas[i]->getNicknameSeguidor() == nickSeguido) {
                    arrS = listas[i]->getLista();
                    tamS = listas[i]->getTamLista();
                    break;
                }
            }
        }
    }

    // Si no hay seguida válida, usamos solo la propia; si sí hay, construimos fusión sin duplicados
    cancion** fav = nullptr;
    int       tam = 0;
    bool      esFusion = false;

    if (!arrS || tamS <= 0) {
        cout << "\n=== Mis favoritos (ALEATORIO – SOLO MI LISTA) ===\n";
        fav = arrP; tam = tamP;
    } else {
        // Fusión temporal por punteros (sin duplicar IDs)
        int cap = tamP + tamS; if (cap <= 0) cap = 1;
        cancion** fusion = new cancion*[cap];
        registrarMemoria<cancion*>(cap);
        for (int i = 0; i < cap; ++i) fusion[i] = nullptr;

        auto existeId = [&](const string& id)->bool {
            for (int k = 0; k < cap; ++k)
                if (fusion[k] && fusion[k]->getId_Cancion() == id) return true;
            return false;
        };

        int nFusion = 0;
        // Primero: mis canciones
        for (int i = 0; i < tamP; ++i) {
            cancion* c = (arrP ? arrP[i] : nullptr);
            if (!c) continue;
            const string& id = c->getId_Cancion();
            if (id.empty() || existeId(id)) continue;
            fusion[nFusion++] = c;
        }
        // Luego: las del seguido
        for (int i = 0; i < tamS; ++i) {
            cancion* c = (arrS ? arrS[i] : nullptr);
            if (!c) continue;
            const string& id = c->getId_Cancion();
            if (id.empty() || existeId(id)) continue;
            fusion[nFusion++] = c;
        }

        if (nFusion == 0) {
            delete[] fusion; liberarMemoria<cancion*>(cap);
            cout << "Tu lista fusionada quedó vacía.\n"; return;
        }

        cout << "\n=== Mis favoritos (ALEATORIO – FUSIONADA) ===\n";
        fav = fusion; tam = nFusion; esFusion = true;
    }

    // 2) Fuente y conteo de válidos (sin STL)
    int validos = 0; for (int i=0;i<tam;++i) if (fav[i]) ++validos;
    if (validos <= 0) { cout << "Tu lista no contiene canciones válidas.\n"; return; }

    // 3) Buffer circular de ÚLTIMAS 6 canciones (punteros)
    const int MAX_HIST = 6;                    // M=6 (según enunciado)
    cancion* hist[MAX_HIST];                   // almacena PUNTEROS a canciones
    int histSize = 0;                          // tamaño lógico del historial
    int histPos  = -1;                         // posición del elemento "actual" en el anillo
    for (int i=0;i<MAX_HIST;++i) hist[i] = 0;  // limpia

    auto estaEnHist = [&](cancion* x)->bool {
        for (int i=0;i<histSize;++i) { if (hist[i] && hist[i]==x) return true; }
        return false;
    };

    // 4) Elegir SIGUIENTE índice aleatorio evitando las 6 más recientes
    auto escogerSiguiente = [&](cancion* actual)->int {
        if (validos == 0) return -1;
        if (validos == 1) {            // solo hay una canción válida
            for (int i=0;i<tam;++i) if (fav[i]) return i;
            return -1;
        }

        // Intentar varias veces no chocar con el historial reciente
        int elegido = -1;
        for (int intentos=0; intentos<64; ++intentos) {
            int r = randEnRango(0, tam-1);                 // helper ya definido en utilidades.cpp
            cancion* c = fav[r];
            if (!c) continue;
            if (c == actual) continue;                     // evita repetición inmediata
            if (!estaEnHist(c)) { elegido = r; break; }    // evita cualquiera de las últimas 6
        }

        // Fallback 1: si todos los intentos chocaron, elegir cualquiera distinta a la actual
        if (elegido < 0) {
            for (int i=0;i<tam;++i) { if (fav[i] && fav[i]!=actual) { elegido = i; break; } }
        }
        // Fallback 2: si no hay distinta (tam==1), dar la única válida
        if (elegido < 0) {
            for (int i=0;i<tam;++i) if (fav[i]) { elegido = i; break; }
        }
        return elegido;
    };

    // 5) Arranque: escoge primera
    int idxActual = escogerSiguiente(nullptr);
    if (idxActual < 0) { cout << "No hay canción válida para reproducir.\n"; return; }

    bool repetir   = false;   // repetir indefinidamente la canción actual
    bool salir     = false;   // detener reproducción
    int pasosPrev  = 0;       // cuántas "previas" seguidas llevamos (máx 6)

    cout << "\n=== Mis favoritos (modo ALEATORIO - PREMIUM) ===\n";
    while (!salir) {
        cancion* c = fav[idxActual];
        if (c) {
            // 5.a) Mostrar meta + 320 kbps (premium)
            auto meta = resolverMetaDesdeIdCancion(c->getId_Cancion(), albumnes, totalAlbumnes, artistas, totalArtistas);
            cout << "======== Reproduciendo ========\n";
            if (meta.ok()){
                cout << "Cantante: " << meta.nombreArtista << "\n";
                cout << "Album:   " << meta.nombreAlbum   << "\n";
                cout << "Portada: " << meta.rutaPortada   << "\n";
            }
            cout << "Titulo:  " << c->getNombreCancion() << "\n";
            cout << "Ruta:    " << c->getRuta320()       << "\n";
            cout << "Duracion:" << c->getDuracion()      << "\n\n";
        }

        // 5.b) Registrar en historial SOLO si no estamos en modo repetir
        if (!repetir) {
            histPos = (histPos + 1) % MAX_HIST;
            hist[histPos] = c;                          // guarda puntero
            if (histSize < MAX_HIST) ++histSize;        // crece hasta 6 y luego recicla
            pasosPrev = 0;                              // al avanzar, resetea retrocesos
        }

        // 5.c) Controles (auto-siguiente tras 3s)
        cout << "[N] siguiente  [P] previa  [R] repetir:" << (repetir?"ON":"OFF") << "  [S] salir\n";
        std::this_thread::sleep_for(std::chrono::seconds(3));
        char op = 0;
        if (cin.peek()!='\n') cin >> op;
        if (cin.peek()=='\n') cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (op=='S' || op=='s') { salir = true; break; }
        else if (op=='R' || op=='r') { repetir = !repetir; continue; }
        else if (op=='P' || op=='p') {
            // 6) PREVIA: retroceder hasta M=6 (o hasta agotar historial real)
            if (histSize > 1 && pasosPrev < (histSize - 1) && pasosPrev < (MAX_HIST - 1)) {
                // movernos una posición "hacia atrás" en el anillo
                histPos = (histPos - 1 + MAX_HIST) % MAX_HIST;
                cancion* previa = hist[histPos];
                // ubicar el índice de esa canción en 'fav' por igualdad de puntero
                int found = -1;
                for (int i=0;i<tam;++i) { if (fav[i] == previa) { found = i; break; } }
                if (found >= 0) idxActual = found;
                ++pasosPrev;
                repetir = false;           // salir de repetir si estaba activado
                continue;                  // volver a imprimir esa "previa"
            } else {
                cout << "(No hay previa o ya alcanzaste el limite de 6)\n";
                // no cambiamos nada, seguimos con la actual / o pasamos a N abajo
            }
        }

        // 7) Siguiente (N o Enter): elegir aleatoria evitando historial reciente
        repetir = false;                    // al pasar, repetir queda OFF
        {
            cancion* actual = (histSize > 0 ? hist[histPos] : nullptr);
            int nextIdx = escogerSiguiente(actual);
            if (nextIdx >= 0) idxActual = nextIdx;
            // Si por alguna razón no se encontró, se quita el continue y se reintenta en la próxima iteración
        }
        incrementarIteraciones();
    }
    if (esFusion) { delete[] fav; liberarMemoria<cancion*>(tam); }
    cout << "[INFO] Fin de favoritos (aleatorio premium).\n";
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


bool seguirListaFavoritosPorNickname(usuario** usuarios, int totalUsuarios,
                                     lista_favoritos** listas, int totalListas,
                                     usuario* seguidor, const string& nickObjetivo)
{
    if (!seguidor) {
        cout << "[ERROR] No hay usuario seguidor.\n"; return false; }

    usuario* objetivo = buscarUsuarioPorNickname(usuarios, totalUsuarios, nickObjetivo);
    if (!objetivo) {
        cout << "[ERROR] No existe el usuario: " << nickObjetivo << "\n"; return false; }
    if (objetivo == seguidor) {
        cout << "[WARN] No puedes seguir tu propia lista.\n"; return false; }

    //  CLAVE: buscar en el ARREGLO DE LISTAS por nickname
    lista_favoritos* listaObjetivo = buscarListaDeFavoritos(objetivo, listas, totalListas);
    if (!listaObjetivo) {
        cout << "[ERROR] El usuario objetivo no tiene lista de favoritos.\n";
        return false;
    }
    if (seguidor->getMembresia() != "premium") {
        cout << "[ERROR] Solo usuarios premium pueden seguir listas.\n";
        return false;
    }
    if (objetivo->getMembresia() != "premium") {
        cout << "[ERROR] Solo puedes seguir listas de usuarios premium.\n";
        return false;
    }

    if (seguidor->seguir_listafavorita(listaObjetivo)) {
        cout << "[OK] Ahora sigues la lista de " << nickObjetivo << ".\n";
        // Actualiza el campo para persistir:
        seguidor->setNicknameAseguir(nickObjetivo);

        // Guarda usuarios.txt con el nuevo "a quien sigo"
        if (!guardarUsuarios(usuarios, totalUsuarios)) {
            cout << "[WARN] No se pudo guardar usuarios.txt con el seguimiento.\n";
        } else {
            cout << "[OK] Seguimiento guardado en usuarios.txt\n";
        }
        return true;
    } else {
        cout << "[INFO] Ya seguías esa lista.\n";
        return false;
    }
}


void dejarDeSeguirLista(usuario* seguidor,
                        usuario** usuarios, int totalUsuarios)
{
    if (!seguidor) return;
    seguidor->dejar_seguir();

    // Limpia el campo y persiste
    seguidor->setNicknameAseguir("");

    if (!guardarUsuarios(usuarios, totalUsuarios)) {
        std::cout << "[WARN] No se pudo guardar usuarios.txt al dejar de seguir.\n";
    } else {
        std::cout << "[OK] Dejar de seguir guardado en usuarios.txt\n";
    }

    std::cout << "[OK] Dejaste de seguir la lista.\n";
}


void imprimirFavoritosFusion(usuario* u)
{
    if (!u) {
        cout << "[ERROR] Usuario inválido.\n";
        return;
    }

    cout << "\nFavoritos fusionados de " << u->getNickname() << ":\n";

    auto visitor = [](cancion* s){
        if (!s) return;
        cout << "  ♪ " << s->getNombreCancion() << " [id=" << s->getId_Cancion() << "]\n";
        // Si quieres, aquí mismo puedes llamar a resolverMetaDesdeIdCancion(...)
        // para mostrar álbum/portada/artista.
    };

    u->recorrerFavoritosFusion(visitor);
}

bool guardarUsuarios(usuario** usuarios, int totalUsuarios, const std::string& ruta)
{
    if (!usuarios || totalUsuarios <= 0) return false;

    std::ofstream out(ruta.c_str(), std::ios::out | std::ios::trunc);
    if (!out.is_open()) {
        std::cout << "No se pudo abrir " << ruta << " para escritura.\n";
        return false;
    }

    for (int i = 0; i < totalUsuarios; ++i) {
        usuario* u = usuarios[i];
        if (!u) continue;

        // FORMATO: nick,membresia,ciudad,pais,fecha,nicknameAseguir
        out << u->getNickname()        << ","
            << u->getMembresia()       << ","
            << u->getCiudad()          << ","
            << u->getPais()            << ","
            << u->getFecha()           << ","
            << u->getNicknameAseguir() << "\n";
    }

    if (!out.good()) {
        std::cout << "Error al escribir " << ruta << ".\n";
        return false;
    }
    return true;
}
