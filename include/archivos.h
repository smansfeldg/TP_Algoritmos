#ifndef ARCHIVOS_H
#define ARCHIVOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_ARCH 0
#define ERROR_MEM -1
#define NO_ENCONTADO -1

#define TAM_LINEA 30
#define TAM 26

/**
 * @file archivos.h
 * @brief Manejo de persistencia y lectura de archivos.
 *
 * Capa de Acceso a Datos.
 * Gestión de lectura de config.txt, caravana.txt y la
 * persistencia binaria de los usuarios registrados.
 */


typedef struct{
    char nombre[TAM];
    int puntosTotales;
    int cantPartidas;
}sJugador;

typedef struct{
    char nombeJugador[TAM];
    int puntos;
    int movimientos;
}sPartida;

typedef struct{
    int cantPos;
    int vidasIniciales;
    int maxBandidos;
    int maxPremios;
    int maxVidasExtra;
    int maxOasis;
    int maxTormentas;
}sConfig;

typedef struct{
    int pos;
    char objetos[TAM];
}sCasilla;

typedef void (*accion) (void *b,const void *a);
typedef int (*cmp) (const void *a, const void *b);

////Funciones Genericas de Archivos
int abrirArchivo(FILE ** arch, const char *nombreArchivo, const char *modoApertura, int mostrarError);
int cerrarArchivo(FILE ** arch, const char *nombreArchivo, int mostrarError);


int mostrarArchivoTxt(FILE* arch, const char *nombreArchivo, accion Accion);
int leerArchivoTxt(FILE* arch, void *estDatos, unsigned tam, accion Accion, accion Cargar);

int mostrarArchivoBin(FILE* arch, unsigned tam, accion Accion);
int leerArchivoBin(FILE* arch, void *estDatos, unsigned tam, accion Accion, accion Cargar);

////Funciones para manejo de registros en Archivos Binarios
int escribirRegistro(FILE* arch,const void *dato, unsigned tam);
int leerRegistro(FILE* arch, void *dato, unsigned tam);

int buscarRegistro(FILE* arch,const void *dato, unsigned tam, cmp Cmp);//Si encuentra registro, posiciona el puntero a principio de este

int leerPos(FILE* arch, void *dato, unsigned tam, int pos);
int escribirPos(FILE* arch,const void *dato, unsigned tam, int pos);

int actualizarRegistro(FILE* arch, void *dato, unsigned tam, accion Accion);

int cmpNombre(const void *a, const void *b);
////Trozado
void trozarConfig(void *config, const void *dato);
void trozarMapa(void *mapa, const void *dato);
void trozarJugador(void *jugador, const void *dato);
void trozarPartida(void *partida, const void *dato);

////Cargar el registro en la estructure de datos que corresponda;
void cargarLista(void *lista, const void *dato);


void mostrarLinea(void *a,const void *dato);
void mostrarJugador(void *a,const void *dato);

#endif // ARCHIVOS_H
