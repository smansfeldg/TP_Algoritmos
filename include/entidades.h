#ifndef ENTIDADES_H
#define ENTIDADES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lista_circular.h"
#include "cola.h"
#include "archivos.h"
#include "abb.h"

#define MAX_NOMBRE 50
#define MAX_JUGADORES 100

/*
PRIORITARIO:
AGREGAR LA ESTRUCTURA DE LISTA O LISTA DOBLEMENTE ENLAZADA
CAMBIAR EL NOMBRE DE LA LISTA CIRCULAR
CAMBIAR LAS LISTAS (EXCEPTO EL TABLERO) POR LISTAS COMUNES(QUE TENGAN FIN)
LA LISTA NUEVA DEBE TENER UNA FUNCION DE OBTENER Y AVANZAR QUE DEVUELVA 0 SI LLEGA AL FINAL Y 1 SI DEVOLVIÓ ALGO
SINO HACER UNA FUNCION PARA RECORRER LA LISTA CIRCULAR Y QUE RECIBA UN PARAMETRO Y UNA FUNCION POR PARÁMETRO
DE SER ASI EL CONTENIDO DEL WHILE DE "PROCESARMOVIMIENTOBANDIDOS" DEBE SER LA FUNCIÓN QUE SE PASE COMO ARGUMENTO Y
SE DEBERA AJUSTAR PARA ESO MISMO
*/

#define TIPO_NORMAL   '.'
#define TIPO_INICIO   'I'
#define TIPO_REFUGIO  'S'
#define TIPO_PREMIO   'P'
#define TIPO_VIDA     'V'
#define TIPO_OASIS    'O'
#define TIPO_TORMENTA 'T'
#define TIPO_BANDIDO  'B'
#define TIPO_JUGADOR  'J'

typedef struct {
    int posicion;
    unsigned normal;
    unsigned inicio;
    unsigned refugio;
    unsigned premios;
    unsigned vidas;
    unsigned oasis;
    unsigned tormenta;
    unsigned jugador;
    unsigned bandidos;
} tCasilla;

typedef struct {
    char usuario[MAX_NOMBRE];
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
    int mapaPregenerado;
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
    int totalMovimientos;
} tJuego;

typedef struct {
    int idPartida;
    char usuario[MAX_NOMBRE];
    int puntuacion;
    int cantidadMovimientos;
} tRegistroPartida;

typedef struct{
    char usuario[MAX_NOMBRE];
    char nombre[MAX_NOMBRE];
} tRegistroJugador;

typedef struct{
  char nombre[MAX_NOMBRE];
  char usuario[MAX_NOMBRE];
  int cantidadPartidas;
  int puntuacionTotal;
} tRegistroRanking;

typedef struct{
    char usuario[MAX_NOMBRE];
    unsigned registro;
} tIndice;

typedef struct{
    FILE *archJug;
    FILE *archPart;
    FILE *archIndice;
}tArchivos;

void crearJugador(tJugador *j, const char *nombre, const char *usuario,int posicionInicial, int vidas);
void crearBandido(tBandido *b, int id, int posicion);

int cargarConfiguracion(const char *archivo, tConfiguracion *cfg);
void trozarConfig(void *config, const void *dato);
void mostrarConfiguracion(const tConfiguracion *cfg);

int generarTablero(tJuego *juego, const tConfiguracion *cfg);

tCasilla* buscarCasilla(const tLista *tablero, int posicion, int (*cmp)(const void*, const void*));
int moverJugador(tJuego* juego, tMovimiento movimiento, tCasilla* Casilla);
int aplicarEfectoCasilla(tJugador *j, tCasilla* Casilla);
void inicializarJuego(tJuego *juego, tConfiguracion *cfg);
void liberarJuego(tJuego *juego);
int encolarMovimiento(tCola *cola, tMovimiento mov);
int desencolarMovimiento(tCola *cola, tMovimiento *mov);
void mostrarColaMovimientos(tCola *cola);
int moverBandido(tJuego* juego,tBandido *b,tMovimiento movimiento, tCasilla* Casilla);
int verificarColision(tJuego *juego, tBandido *b, tCasilla* Casilla);
int verificarDerrota(const tJugador *j);
void guardarCaravana(const char *archivo, const tJuego *juego);
int cargarCaravana(const char *archivo, tJuego *juego);
int posicionarJugador(tJuego *juego, int posicion);
int cmpPosCasillas(const void *a, const void *b);


/////////Funciones de Indice///////////
void indexarArchivoJugadores(FILE *archJug, ArbolBin *indice);
int IndexarArchivoOrdenado(FILE *archIndx, char *nombreArch, ArbolBin *arbolIndx, unsigned tam);
size_t guardarIndiceEnNodo(void **arbo, void*arch, unsigned pos, void* param);

void actualizarJugadores(FILE *archJug, ArbolBin *indice, const tJugador *nuevo);
int cmpIndxApodo(const void *a, const void *b);

int archivarIndice(FILE *archIndx, char *nombreArch, ArbolBin *arbolIndx);
void guardNodoIndxEnArchivo(void *dato, size_t tam, unsigned nivel, void *params);
void mostrarIndxArch (void *a, const void *b);

int buscarJugador(char *nombre, ArbolBin *indice, cmp Cmp);

/////////Funciones de Ranking///////////
int crearRanking(tLista *ranking, FILE* archPart, ArbolBin *indice, FILE *archJug);
void cargarRanking(void *ranking, const void *dato);
void cargarNombres(void *dato, void *contexto, void *param);

int cmpRankUsuarios(const void*a, const void *b);
int sumarRankPuntos(void **dato1, unsigned *tam1, const void* dato2, unsigned tam2);
int cmpRankPuntos(const void *a, const void *b);

int actualizarRegistroPartidas(tArchivos archivos, ArbolBin *indice, tLista *ranking, const tRegistroPartida *partida);
void mostrarListaRanking(tLista *ranking);
void mostrarRanking(const void *ranking, void* param);

int inicioAbrirArchivos(tArchivos *archivos);
int finCerrarArchivos(tArchivos *archivos);

/////////Funciones de Prueba///////////
void mostrarIndx(void *dato, size_t tam, unsigned nivel, void *params);
void mostrarPartidas(void *a, const void *b);
void mostrarJugadores(void *a, const void *b);
void crearPruebas();

int verificarTablero(tLista *tablero, tConfiguracion *cfg);
void incrementarCont(tConfiguracion *contador, tCasilla *actual);
int verificarCasillaInicio (tCasilla *inicio);
int verificarCasillaFin (tCasilla *fin);

#endif
