#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/entidades.h"

// Función de comparación para ordenar el índice alfabéticamente
int comparadorNombres(const void *a, const void *b) {
    const tRegistroJugador *rA = (const tRegistroJugador *)a;
    const tRegistroJugador *rB = (const tRegistroJugador *)b;
    return strcmp(rA->nombre, rB->nombre);
}

int main() {
    printf("--- Generador de datos de prueba ---\n");

    // 1. Datos significativos para jugadores
    tRegistroJugador jugadores[] = {
        {1, "Alice"},
        {2, "Bob Junior"},
        {3, "CARLOS"},
        {4, "diana"},
        {5, "JugadorConNombreMuyLargo"},
        {6, "Zoe"}
    };
    int cant_jugadores = sizeof(jugadores) / sizeof(jugadores[0]);

    // Archivo de jugadores original
    FILE *f_jugadores = fopen("jugadores.dat", "wb");
    if (!f_jugadores) {
        printf("Error creando jugadores.dat\n");
        return 1;
    }
    fwrite(jugadores, sizeof(tRegistroJugador), cant_jugadores, f_jugadores);
    fclose(f_jugadores);
    printf("[OK] jugadores.dat guardado con %d registros.\n", cant_jugadores);

    // Archivo de índice de jugadores
    tRegistroJugador indice[10];
    memcpy(indice, jugadores, sizeof(jugadores));
    qsort(indice, cant_jugadores, sizeof(tRegistroJugador), comparadorNombres);

    FILE *f_indice = fopen("indice_jugadores.dat", "wb");
    if (!f_indice) {
        printf("Error creando indice_jugadores.dat\n");
        return 1;
    }
    fwrite(indice, sizeof(tRegistroJugador), cant_jugadores, f_indice);
    fclose(f_indice);
    printf("[OK] indice_jugadores.dat guardado y ordenado alfabeticamente.\n");

    // 2. Datos significativos para partidas
    tRegistroPartida partidas[] = {
        // idPartida, nombre, puntuacion, cantidadMovimientos
        {1, "Alice", 1500, 45},                 // Partida normal
        {2, "Alice", 2100, 30},                 // Partida muy buena (misma jugadora)
        {3, "Bob Junior", 0, 10},               // Perdió rápido
        {4, "CARLOS", -150, 20},                // Puntuación negativa por penalizaciones
        {5, "diana", 10000, 100},               // Valores muy altos
        {6, "Zoe", 120, 5},                     // Ganó en muy pocos movimientos
        {7, "JugadorInexistente", 500, 25}      // Registro de un jugador que ya no está en la BD
    };
    int cant_partidas = sizeof(partidas) / sizeof(partidas[0]);

    FILE *f_partidas = fopen("partidas.dat", "wb");
    if (!f_partidas) {
        printf("Error creando partidas.dat\n");
        return 1;
    }
    fwrite(partidas, sizeof(tRegistroPartida), cant_partidas, f_partidas);
    fclose(f_partidas);
    printf("[OK] partidas.dat guardado con %d registros.\n", cant_partidas);

    printf("Generacion completada correctamente.\n");
    return 0;
}
