#ifndef ARCHIVOS_H
#define ARCHIVOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARCH_JUGADORES "jugadores.bin"
#define ARCH_PARTIDAS "partidas.bin"
#define ARCH_INDICE "indice.bin"

#define ERROR_ARCH 0
#define ERROR_MEM -1
#define NO_ENCONTRADO -1
#define OK 1

typedef void (*accion) (void *b, const void *a);
typedef int (*cmp) (const void *a, const void *b);

int abrirArchivo(FILE **arch, const char *nombreArchivo, const char *modoApertura, int mostrarError);
int cerrarArchivo(FILE **arch, const char *nombreArchivo, int mostrarError);

int leerArchivoTxt(FILE *arch, void *estDatos, unsigned tam, unsigned tamLinea, accion Accion);
int leerArchivoBin(FILE *arch, void *estDatos, unsigned tam, accion Accion);

int buscarRegistro(FILE *arch, void *reg, const void *dato, unsigned tam, cmp Cmp);
int actualizarRegistro(FILE *arch, void *reg, const void *dato, unsigned tam, accion Accion);
int leerPos(FILE *arch, void *dato, unsigned tam, int pos);
int escribirPos(FILE *arch, const void *dato, unsigned tam, int pos);

int escribirTxt(FILE *arch, void *dato, unsigned tamLinea, accion Accion);
int escribirNuevoReg(FILE *arch, const void *dato, unsigned tam);

#endif
