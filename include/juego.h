#ifndef JUEGO_H
#define JUEGO_H

#include "entidades.h"
#include "cola.h"

int iniciarPartida(tJuego *juego, const char *nombreJugador);
int ejecutarTurno(tJuego *juego);
int procesarMovimientoJugador(tJuego *juego);
int procesarMovimientosBandidos(tJuego *juego);
int resolverColisiones(tJuego *juego);
void mostrarTableroConPosiciones(const tJuego *juego);
int lanzarDado();
char obtenerDireccionMovimiento();
int obtenerPasosMovimiento();
void mostrarBienvenida();
void mostrarFinJuego(int victoria, const tJugador *j);
void jugarTurnoComputadora(tJuego *juego);

#endif