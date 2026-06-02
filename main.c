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

int main(){
    srand((unsigned)time(NULL));

    int opcion_elegida = 0;

    //Abir Archivos e inicialisar indice y ranking.
    ArbolBin indice;
    tLista ranking;
    tArchivos archivosDelJuego;
    inicioAbrirArchivos(&archivosDelJuego);
    IndexarArchivo(archivosDelJuego.archIndice, ARCH_INDICE,&indice,sizeof(tIndice));

    crearRanking(&ranking,archivosDelJuego.archPart,&indice,archivosDelJuego.archJug);
    leerArchivoBin(archivosDelJuego.archPart,NULL,sizeof(tRegistroPartida),mostrarPartidas);
    ////

    mostrarBienvenida();
    pausar_consola();
    limpiar_pantalla();

    do {
        mostrar_menu_principal();
        opcion_elegida = obtener_opcion(3); // Actualizar al agregar futuras opciones

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


    //Vaciar listas, guardar cambias y cerrar archivos
    vaciarLista(&ranking);
    archivarIndice(archivosDelJuego.archIndice, ARCH_INDICE,&indice);
    finCerrarArchivos(&archivosDelJuego);
    //

    return 0;
}

void jugar(tArchivos *archivosDelJuego, ArbolBin *indice, tLista *ranking){
    tJuego juego;
    tConfiguracion cfg;
    tRegistroPartida reg;
    int resultado;

    cargarConfiguracion("config.txt", &cfg);
    mostrarConfiguracion(&cfg);

    inicializarJuego(&juego, &cfg);
    //Corregir implementacon de indices
    iniciarPartida(&juego, archivosDelJuego->archJug, indice);
    limpiar_pantalla();

    guardarCaravana("caravana.txt", &juego);
    mostrarReglas();
    printf("!Tablero listo!\nPosicion inicial: 1 (Inicio)\n");
    printf("Objetivo: Llegar a la posicion %d (Ciudad Refugio)\n\n", cfg.totalCasillas);

    pausar_consola();

    limpiar_pantalla();
    mostrarTablero(&juego.tablero);
    while (juego.juegoActivo) {
        resultado = ejecutarTurno(&juego);

        //MUESTRA EL FINAL DEL JUEGO SI SE DA EL MISMO
        mostrarFinJuego(resultado,&juego);

        pausar_consola();

        limpiar_pantalla();
        mostrarTablero(&juego.tablero);
    }

    //HACER UNA FUNCION PARA GUARDAR INDICE
    //GUARDAR INDICE

    printf("\n  --- Resumen de Movimientos ---\n");
    mostrarColaMovimientos(&juego.colaMovimientosJugador);

    strncpy(reg.nombre, juego.jugador.nombre, MAX_NOMBRE - 1);
    reg.nombre[MAX_NOMBRE - 1] = '\0';
    reg.cantidadMovimientos=juego.totalMovimientos;
    reg.puntuacion = juego.jugador.puntos;
    fseek(archivosDelJuego->archPart,0,SEEK_END);
    reg.idPartida=ftell(archivosDelJuego->archPart)/sizeof(tRegistroPartida)+1;

    //Guardar Partida
    //Corregir indice y como se impplementa con el ranking
    actualizarRegistroPartidas(archivosDelJuego->archPart,ranking,&reg);

    liberarJuego(&juego);


    puts("\n¡PARTIDA FINALIZADA!");
    pausar_consola();
}
