#ifndef ENTIDADES_H
#define ENTIDADES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista_circular.h"
#include "cola.h"

#define MAX_NOMBRE 50
#define MAX_JUGADORES 100

typedef enum {
    TIPO_NORMAL = '.',
    TIPO_INICIO = 'I',
    TIPO_REFUGIO = 'S',
    TIPO_PREMIO = 'P',
    TIPO_VIDA = 'V',
    TIPO_OASIS = 'O',
    TIPO_TORMENTA = 'T',
    TIPO_BANDIDO = 'B',
    TIPO_JUGADOR = 'J'
} tTipoCasilla;

typedef struct {
    int posicion;
    char tipo;
    char descripcion[50];
} tCasilla;

typedef struct {
    int idJugador;
    char nombre[MAX_NOMBRE];
    int posicion;
    int vidas;
    int puntos;
    int protegidoOasis;
    int perdidoTurno;
} tJugador;

typedef struct {
    int id;
    int posicion;
    int activo;
} tBandido;

typedef struct {
    char direccion;
    int pasos;
    void* entidad;
} tMovimiento;

typedef struct {
    int totalCasillas;
    int vidasIniciales;
    int cantidadBandidos;
    int cantidadPremios;
    int cantidadVidas;
    int cantidadOasis;
    int cantidadTormentas;
} tConfiguracion;

typedef struct {
    tLista tablero;
    tJugador jugador;
    tLista bandidos;
    tCola colaMovimientos;
    tCola colaMovimientosJugador;
    tConfiguracion config;
    int turnoActual;
    int juegoActivo;
} tJuego;

typedef struct {
    int idPartida
    char nombre[MAX_NOMBRE];
    int puntuacion;
    int cantidadMovimientos;
} tRegistroPartida;

typedef struct{
    int idJugador;
    char nombre[MAX_NOMBRE];
} tRegistroJugador;

typedef struct{
  int idJugador;
  int cantidadPartidas;
  int puntuacionTotal;
} tRegistroRanking;

void crearJugador(tJugador *j, const char *nombre, int posicionInicial, int vidas);
void crearBandido(tBandido *b, int id, int posicion);
void inicializarConfiguracion(tConfiguracion *cfg);
int cargarConfiguracion(const char *archivo, tConfiguracion *cfg);
void mostrarConfiguracion(const tConfiguracion *cfg);
int generarTablero(tLista *tablero, const tConfiguracion *cfg);
void mostrarTablero(const tLista *tablero);
int buscarCasilla(const tLista *tablero, int posicion, tCasilla *casilla);
int moverJugador(tLista *tablero, tJugador *j, int pasos, char direccion, int totalCasillas);
void aplicarEfectoCasilla(tJugador *j, char tipoCasilla);
void mostrarEstadoJugador(const tJugador *j);
void inicializarJuego(tJuego *juego, const tConfiguracion *cfg, const char *nombreJugador);
void liberarJuego(tJuego *juego);
int encolarMovimiento(tCola *cola, char direccion, int pasos);
int desencolarMovimiento(tCola *cola, tMovimiento *mov);
void mostrarColaMovimientos(const tCola *cola);
int moverBandido(tLista *tablero, tBandido *b, const tJugador *j, int totalCasillas);
int verificarColision(const tJugador *j, const tBandido *b);
int verificarVictoria(const tJugador *j, const tLista *tablero);
int verificarDerrota(const tJugador *j);
void guardarCaravana(const char *archivo, const tJuego *juego);
int cargarCaravana(const char *archivo, tJuego *juego);

#endif
