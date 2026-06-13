#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "include/entidades.h"
#include "include/archivos.h"
#include "include/juego.h"
#include "include/menu.h"
#include "include/abb.h"

void jugar(tArchivos *archivosDelJuego, ArbolBin *indice, tLista *ranking);

int main()
{
    int opcion_elegida = 0;
    ArbolBin indice;
    tLista ranking;
    tArchivos archivosDelJuego;


    srand((unsigned)time(NULL));

    int arch = inicioAbrirArchivos(&archivosDelJuego);
    if (!arch) {
        puts("No se pudieron abrir los archivos de datos del juego.");
        return 1;
    }

    if(arch==1)
        IndexarArchivoOrdenado(archivosDelJuego.archIndice, ARCH_INDICE, &indice, sizeof(tIndice));
    else
        indexarArchivoJugadores(archivosDelJuego.archJug,&indice);

    crearRanking(&ranking, archivosDelJuego.archPart, &indice, archivosDelJuego.archJug);

    mostrarBienvenida();
    pausar_consola();
    limpiar_pantalla();

    do {
        mostrar_menu_principal();
        opcion_elegida = obtener_opcion(3);

        switch(opcion_elegida) {
            case 1:
                printf("Iniciando juego...\n");
                jugar(&archivosDelJuego, &indice, &ranking);
                break;

            case 2:
                printf("Mostrando ranking de jugadores...\n");
                mostrarListaRanking(&ranking);
                pausar_consola();
                break;

            case 3:
                printf("Saliendo del juego...\n");
                break;
        }
        limpiar_pantalla();
    } while(opcion_elegida != 3);

    vaciarLista(&ranking);
    archivarIndice(archivosDelJuego.archIndice, ARCH_INDICE, &indice);
    finCerrarArchivos(&archivosDelJuego);

    return 0;
}

void jugar(tArchivos *archivosDelJuego, ArbolBin *indice, tLista *ranking)
{
    tJuego juego;
    tConfiguracion cfg;
    tRegistroPartida reg;
    int resultado;

    if (!cargarConfiguracion("config.txt", &cfg)) {
        printf("No se pudo cargar config.txt. Revise que el archivo exista y sea valido.\n");
        pausar_consola();
        return;
    }

    mostrarConfiguracion(&cfg);
    inicializarJuego(&juego, &cfg);
    iniciarPartida(&juego, archivosDelJuego->archJug, indice);
    limpiar_pantalla();

    guardarCaravana("caravana.txt", &juego);

    ///Para probar si carga bien caravana.
    //vaciarLista(&juego.tablero);
    //vaciarLista(&juego.bandidos);
    //cargarCaravana("caravana.txt", &juego);

    mostrarReglas();
    printf("!Tablero listo!\nPosicion inicial: 1 (Inicio)\n");
    printf("Objetivo: Llegar a la posicion %d (Ciudad Refugio)\n\n", cfg.totalCasillas);
    pausar_consola();

    limpiar_pantalla();
    mostrarTablero(&juego.tablero);
    while (juego.juegoActivo) {
        resultado = ejecutarTurno(&juego);

        if (resultado != 1) {
            mostrarFinJuego(resultado, &juego);
            juego.juegoActivo = 0;
        }

        if (juego.juegoActivo) {
            pausar_consola();
            limpiar_pantalla();
            mostrarTablero(&juego.tablero);
        }
    }

    printf("\n  --- Resumen de Movimientos ---\n");
    mostrarColaMovimientos(&juego.colaMovimientosJugador);

    strncpy(reg.usuario, juego.jugador.usuario, MAX_NOMBRE - 1);
    reg.usuario[MAX_NOMBRE - 1] = '\0';
    reg.cantidadMovimientos = juego.totalMovimientos;
    reg.puntuacion = juego.jugador.puntos;

    fseek(archivosDelJuego->archPart, 0, SEEK_END);
    reg.idPartida = (int)(ftell(archivosDelJuego->archPart) / sizeof(tRegistroPartida)) + 1;
    actualizarRegistroPartidas(*archivosDelJuego, indice, ranking, &reg);

    liberarJuego(&juego);

    puts("\nPARTIDA FINALIZADA!");
    pausar_consola();
}
