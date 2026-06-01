#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "include/entidades.h"
#include "include/archivos.h"
#include "include/juego.h"
#include "include/menu.h"
#include "include/abb.h"

void jugar();

int main(){
    srand((unsigned)time(NULL));

    int opcion_elegida = 0;

    mostrarBienvenida();
    pausar_consola();
    limpiar_pantalla();

    do {
        mostrar_menu_principal();
        opcion_elegida = obtener_opcion(3); // Actualizar al agregar futuras opciones

        switch(opcion_elegida) {
            case 1:
                printf("Iniciando juego...\n");
                jugar();
                break;
            case 2:
                printf("Mostrando ranking de jugadores...\n");
                // TODO: Implementar mostrar_ranking()
                pausar_consola();
                break;
            case 3:
                printf("Saliendo del juego...\n");
                break;
        }
        limpiar_pantalla();
    } while(opcion_elegida != 3);

    return 0;
}

void jugar(){
    tJuego juego;
    tConfiguracion cfg;
    tRegistroPartida reg;
    int resultado;

    //HACER FUNCION PARA CARGAR INDICE
    //CARGAR INDICE

    cargarConfiguracion("config.txt", &cfg);
    mostrarConfiguracion(&cfg);

    inicializarJuego(&juego, &cfg);
    iniciarPartida(&juego);
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
    reg.puntuacion = juego.jugador.puntos;

    /*HACER FUNCION - GUARDAR EN EL ARHIVO DE PARTIDAS
    guardarPuntaje("puntajes.dat", &reg);
    */

    liberarJuego(&juego);


    puts("\n¡PARTIDA FINALIZADA!");
    pausar_consola();
}
