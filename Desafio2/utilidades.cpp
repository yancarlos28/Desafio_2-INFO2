#include "memoria.h"
#include "usuario.h"
#include "anuncio.h"
#include "album.h"
#include "artista.h"
#include "utilidades.h"
#include <chrono>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <thread>
using namespace std;

/**Definicion de variables globales para almacenar memoria
*/

int contadorIteracionesGlobal = 0;
size_t memoriaReservadaGlobal = 0;

/* LOGIN, BUSQUEDA Y VALIDACION-----------
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

bool seguirListaFavoritosPorNickname(usuario** usuarios, int totalUsuarios,
                                     lista_favoritos** listas, int totalListas,
                                     usuario* seguidor, const string& nickObjetivo)
{
    if (!seguidor) { cout << "[ERROR] No hay usuario seguidor.\n"; return false; }

    usuario* objetivo = buscarUsuarioPorNickname(usuarios, totalUsuarios, nickObjetivo);
    if (!objetivo) { cout << "[ERROR] No existe el usuario: " << nickObjetivo << "\n"; return false; }
    if (objetivo == seguidor) { cout << "[WARN] No puedes seguir tu propia lista.\n"; return false; }

    // 🔑 CLAVE: buscar en el ARREGLO DE LISTAS por nickname
    lista_favoritos* listaObjetivo = buscarListaDeFavoritos(objetivo, listas, totalListas);
    if (!listaObjetivo) {
        cout << "[ERROR] El usuario objetivo no tiene lista de favoritos.\n";
        return false;
    }

    if (seguidor->seguir_listafavorita(listaObjetivo)) {
        cout << "[OK] Ahora sigues la lista de " << nickObjetivo << ".\n";
        return true;
    } else {
        cout << "[INFO] Ya seguías esa lista.\n";
        return false;
    }
}



// verificar si un ID de canción existe en el catálogo
static bool existeCancionId(cancion** canciones, int total, const string& idTxt) {
    if (!canciones || total <= 0) return false;
    for (int i = 0; i < total; ++i) {
        if (canciones[i] && canciones[i]->getId_Cancion() == idTxt) return true;
    }
    return false;
}
string idArtistaDeCancion(const std::string& id9){
    return (id9.size() >= 5) ? id9.substr(0,5) : "";
}
string idAlbum7DeCancion(const std::string& id9){
    return (id9.size() >= 7) ? id9.substr(0,7) : "";
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


// Flujo de login + enrutamiento a menú
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

//generar numero aleatorio

static int randEnRango(int minimo, int maximo) {
    static bool seeded = false;
    if (!seeded) { srand((unsigned)time(0)); seeded = true; }
    if (maximo < minimo) return minimo;                 // guardia
    return minimo + rand() % (maximo - minimo + 1);
}

int generarNumeroAleatorioUnico(int minimo, int maximo) {
    static bool inicializado = false;
    static int usados[100000];   // almacena los números ya usados
    static int cantidadUsados = 0;

    if (!inicializado) {
        srand(time(0));
        for (int i = 0; i < 100000; i++) usados[i] = -1;
        inicializado = true;
    }

    if (cantidadUsados >= (maximo - minimo + 1))
        return -1; // ya se usaron todos los posibles

    int num;
    bool repetido;
    do {
        repetido = false;
        num = minimo + rand() % (maximo - minimo + 1);
        for (int i = 0; i < cantidadUsados; i++) {
            if (usados[i] == num) {
                repetido = true;
                break;
            }
        }
    } while (repetido);

    usados[cantidadUsados++] = num;
    return num;
}

//para sacar id del artista en base al id de la cancion
string copiarPrimeros5(const string& original) {
    return original.substr(0, 5);
}

//para sacar id del album en base al id de la cancion
string copiarPrimeros7(const string& original) {
    return original.substr(0, 7);
}
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
    // Evitar crashes si no hay anuncios
    if (!anuncios || totalAnuncios <= 0) return;

    // No repetir el mismo consecutivamente
    static anuncio* ptrAnuncioAnterior = nullptr;
    anuncio* ptrAnuncioActual = nullptr;

    // Contar cuántos hay por categoría
    int cantidadC = 0;
    int cantidadB = 0;
    int cantidadAAA = 0;

    for (int i = 0; i < totalAnuncios; ++i) {
        anuncio* ptrAnuncio = anuncios[i];
        if (!ptrAnuncio) continue;
        const string& tipo = ptrAnuncio->getTipo_Anuncio();
        if (tipo == "C") ++cantidadC;
        else if (tipo == "B") ++cantidadB;
        else /* "AAA" u otro */ ++cantidadAAA;
    }

    // Random simple (no-único) local y seguro
    auto randEnRango = [](int minimo, int maximo)->int {
        static bool seeded = false;
        if (!seeded) { srand((unsigned)time(0)); seeded = true; }
        if (maximo < minimo) return minimo;
        return minimo + rand() % (maximo - minimo + 1);
    };

    // Elegir categoría ponderada: C(10), B(20), AAA(30) => total 60
    auto elegirCategoria = [&]()->char {
        int r = randEnRango(1, 60);
        if (r <= 10) return 'C';
        if (r <= 30) return 'B';   // (11..30)
        return 'A';                // (31..60) = AAA
    };

    // Elegir 1 anuncio de la categoría dada SIN crear arreglos auxiliares
    auto pickDeCategoria = [&](char cat)->anuncio* {
        // 1) contar cuántos de esa categoría hay
        int count = 0;
        for (int i = 0; i < totalAnuncios; ++i) {
            anuncio* a = anuncios[i];
            if (!a) continue;
            const string& t = a->getTipo_Anuncio();
            bool ok = (cat=='C' && t=="C") ||
                      (cat=='B' && t=="B") ||
                      (cat=='A' && (t=="AAA" || (t!="B" && t!="C")));
            if (ok) ++count;
        }
        if (count == 0) return nullptr;

        // 2) elegir el k-ésimo válido al azar
        int k = randEnRango(0, count - 1);
        for (int i = 0; i < totalAnuncios; ++i) {
            anuncio* a = anuncios[i];
            if (!a) continue;
            const string& t = a->getTipo_Anuncio();
            bool ok = (cat=='C' && t=="C") ||
                      (cat=='B' && t=="B") ||
                      (cat=='A' && (t=="AAA" || (t!="B" && t!="C")));
            if (ok) {
                if (k == 0) return a;
                --k;
            }
        }
        return nullptr;
    };

    // ¿Cuántos anuncios válidos hay en total? (para el caso límite de 1)
    int totalValidos = 0;
    for (int i = 0; i < totalAnuncios; ++i) if (anuncios[i]) ++totalValidos;

    // Intentar elegir uno que NO sea igual al anterior (si hay más de 1)
    for (int intentos = 0; intentos < 20; ++intentos) {
        char cat = elegirCategoria();
        ptrAnuncioActual = pickDeCategoria(cat);

        // Fallback si la categoría quedó vacía
        if (!ptrAnuncioActual) {
            if (cantidadAAA) ptrAnuncioActual = pickDeCategoria('A');
            if (!ptrAnuncioActual && cantidadB)  ptrAnuncioActual = pickDeCategoria('B');
            if (!ptrAnuncioActual && cantidadC)  ptrAnuncioActual = pickDeCategoria('C');
        }

        // Si no hay ninguno, salir
        if (!ptrAnuncioActual) break;

        // Si sólo hay 1 anuncio total, no hay forma de evitar repetición
        if (totalValidos <= 1) break;

        // Evitar repetición consecutiva
        if (ptrAnuncioActual != ptrAnuncioAnterior) break;
    }

    if (!ptrAnuncioActual) return; // no hay nada que mostrar

    // Mostrar y recordar
    cout << ptrAnuncioActual->getMensaje();
    ptrAnuncioAnterior = ptrAnuncioActual;
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

    // K = 5: límite de canciones a reproducir, como exige el enunciado
    const int limiteK = 5;
    const int limite = (totalCanciones < limiteK) ? totalCanciones : limiteK;

    if (user->getMembresia() == "estandar") {
        for (int reproduccion = 0; reproduccion < limite; ++reproduccion) {
            reproducirCancion(user, canciones, totalCanciones, artistas, totalArtistas, albumes, totalAlbumes);
            std::this_thread::sleep_for(std::chrono::seconds(3)); // temporizador de 3s

            // Anuncio cada 2 canciones (2, 4,...). Con limite=5 saldrán a lo sumo 2 anuncios (tras la 2 y la 4).
            if ((reproduccion + 1) % 2 == 0 && totalAnuncios > 0) {
                mostrarAnuncio(anuncios, totalAnuncios);
                std::this_thread::sleep_for(std::chrono::seconds(3)); // pausa corta tras anuncio
            }
        }
    } else { // premium
        for (int reproduccion = 0; reproduccion < limite; ++reproduccion) {
            reproducirCancion(user, canciones, totalCanciones, artistas, totalArtistas, albumes, totalAlbumes);
            std::this_thread::sleep_for(std::chrono::seconds(3)); // temporizador de 3s
        }
    }

    std::cout << "[INFO] Reproduccion finalizada automaticamente tras "
              << limite << " canciones.\n";
}


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

void ejecutarMiListaFavoritosSecuencial(usuario* user,
                                        lista_favoritos** listas, int totalListas,
                                        album** albumnes, int totalAlbumnes,
                                        artista** artistas, int totalArtistas)
{
    if (!user || !listas || totalListas <= 0) {
        std::cout << "No hay listas de favoritos cargadas.\n";
        return;
    }

    // Buscar la lista del usuario actual
    lista_favoritos* miLista = 0;
    const string nick = user->getNickname();
    for (int i = 0; i < totalListas; ++i) {
        if (listas[i] && listas[i]->getNicknameSeguidor() == nick) {
            miLista = listas[i];
            break;
        }
    }

    if (!miLista) {
        cout << "No se encontró una lista de favoritos para este usuario.\n";
        return;
    }

    cancion** fav = miLista->getLista();
    int tam = miLista->getTamLista();
    if (!fav || tam <= 0) {
        cout << "Tu lista de favoritos está vacía.\n";
        return;
    }

    // Contar cuántas canciones reales hay
    int validos = 0;
    for (int i = 0; i < tam; ++i)
        if (fav[i]) ++validos;

    if (validos == 0) {
        cout << "Tu lista no contiene canciones válidas.\n";
        return;
    }
    int* idxs = new int[validos];
    int p = 0;
    for (int i = 0; i < tam; ++i) {
        if (fav[i]) idxs[p++] = i;
    }

    // 4) Iterar sobre mi lista de favoritos (secuencial)
    cout << "\n=== Mi lista de favoritos (" << validos << " canciones) ===\n";
    for (int k = 0; k < validos; ++k) {
        cancion* c = fav[idxs[k]];
        auto meta = resolverMetaDesdeIdCancion(c->getId_Cancion(), albumnes, totalAlbumnes, artistas, totalArtistas);
        if (meta.ok()){
            cout << "Cantante: " << meta.nombreArtista << "\n";
            cout << "Album: "   << meta.nombreAlbum  << "\n";
            cout << "Ruta a la portada del album: " << meta.rutaPortada   << "\n\n";
        }
        cout << "Titulo de la cancion reproducida: " << c->getNombreCancion() << endl;
        cout << "Ruta al archivo de audio: " << c->getRuta320() << "\n\n";
        cout << "Duracion:"<< c->getDuracion()<< "\n\n";
        this_thread::sleep_for(chrono::seconds(5));
        cout<< "1. Reproducir \n";
        cout << "2. Detener \n";
    }

    delete [] idxs;

    cout << "Fin del recorrido de tu lista de favoritos.\n";

}


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
                    cout << "Nickname a seguir: ";
                    string nick;
                    cin >> nick;
                    seguirListaFavoritosPorNickname(usuarios, totalUsuarios,
                                                    listadefavoritos, totalListas,
                                                    usuarioActual, nick);

                }
                else if (opc_2 == 3) {

                    // Ejecutar mi lista de favoritos secuencial
                    ejecutarMiListaFavoritosSecuencial(usuarioActual, listadefavoritos, totalListas, albumnes, totalAlbumnes, artistas,totalArtistas);
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



// ---------------------------------------------------
// Dejar de seguir (desenlaza puntero).
// ---------------------------------------------------
void dejarDeSeguirLista(usuario* seguidor)
{
    if (!seguidor) return;
    seguidor->dejar_seguir();
    cout << "[OK] Dejaste de seguir la lista.\n";
}

// ---------------------------------------------------
// Imprimir la vista fusionada (propia + seguida) sin duplicados.
// Usa tu método: usuario::recorrerFavoritosFusion(visitor).
// ---------------------------------------------------
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

