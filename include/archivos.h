#ifndef ARCHIVOS_H
#define ARCHIVOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_ARCH 0
#define ERROR_MEM -1
#define NO_ENCONTADO -1

#define TAM_LINEA 30
#define TAM 20

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

typedef void (*accion) (const void *a);
typedef int (*cmp) (const void *a, const void *b);
//
int cmpNombre(const void *a, const void *b);

////Funciones Genericas de Archivos
int abrirArchivo(FILE ** arch, const char *nombreArchivo, const char *modoApertura, int mostrarError);
int cerrarArchivo(FILE ** arch, const char *nombreArchivo, int mostrarError);

int recorrerArchivoTxt(FILE* arch, const char *nombreArchivo, accion Accion);
int recorrerArchivoBin(FILE* arch, const char *nombreArchivo, unsigned tam, accion Accion);

////Funciones para manejo de registros en Archivos Binarios
int escribirRegistro(FILE* arch,const void *dato, unsigned tam);
int buscarRegistro(FILE* arch,const void *dato, unsigned tam, cmp Cmp);//Si encuentra registro, posiciona el puntero a principio de este
int leerRegistro(FILE* arch, void *dato, unsigned tam);
int actualizarRegistro(FILE* arch, void *dato, unsigned tam);

////Funciones para Indice
int leerPos(FILE* arch, void *dato, unsigned tam, int pos);
int escribirPos(FILE* arch, void *dato, unsigned tam, int pos);



//Funciones de prueba
void mostrarTxt(const void *dato);
void mostrarBin(const void *dato);

void crearPrueba(const char *nombreArchivo);

#endif // ARCHIVOS_H
