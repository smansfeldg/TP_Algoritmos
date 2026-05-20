#ifndef JUEGO_H
#define JUEGO_H

#include "entidades.h"
#include "cola.h"

int iniciarPartida(tJuego *juego);
int ejecutarTurno(tJuego *juego);
int procesarMovimientoJugador(tJuego *juego);
int procesarMovimientoBandidos(tJuego *juego);
void mostrarTableroConPosiciones(const tJuego *juego);
int lanzarDado();
int obtenerPasosMovimiento();
void mostrarBienvenida();
void mostrarFinJuego(int victoria, const tJugador *j);
void jugarTurnoComputadora(tJuego *juego);

#endif
