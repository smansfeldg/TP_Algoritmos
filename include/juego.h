#ifndef JUEGO_H
#define JUEGO_H

#include "entidades.h"
#include "cola.h"

int iniciarPartida(tJuego *juego);
int ejecutarTurno(tJuego *juego);
int procesarMovimientoJugador(tJuego *juego);
int procesarMovimientoBandidos(tJuego *juego);
void mostrarTablero(const tLista *tablero);
int lanzarDado();
int obtenerPasosMovimiento();
void mostrarBienvenida();
void mostrarFinJuego(int victoria, const tJuego *j);
void mostrarReglas();
int jugarTurnoComputadora(tJuego *juego);

#endif
