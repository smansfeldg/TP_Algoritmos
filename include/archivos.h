#ifndef ARCHIVOS_H
#define ARCHIVOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_ARCH 0
#define ERROR_MEM -1
#define NO_ENCONTRADO -1
#define OK 1

/**
 * @file archivos.h
 * @brief Manejo de persistencia y lectura de archivos.
 *
 * Capa de Acceso a Datos.
 * Gestión de lectura de config.txt, caravana.txt y la
 * persistencia binaria de los usuarios registrados.
 */

typedef void (*accion) (void *b,const void *a);
typedef int (*cmp) (const void *a, const void *b);

int abrirArchivo(FILE ** arch, const char *nombreArchivo, const char *modoApertura, int mostrarError);
int cerrarArchivo(FILE ** arch, const char *nombreArchivo, int mostrarError);

int leerArchivoTxt(FILE* arch, void *estDatos, unsigned tam, unsigned tamLinea, accion Accion);
int leerArchivoBin(FILE* arch, void *estDatos, unsigned tam, accion Accion);

int buscarRegistro(FILE* arch, void *reg,const void *dato, unsigned tam, cmp Cmp);
int leerPos(FILE* arch, void *dato, unsigned tam, int pos);
int escribirPos(FILE* arch,const void *dato, unsigned tam, int pos);

int actualizarRegistro(FILE* arch, void *reg, const void *dato, unsigned tam, accion Accion);

int escribirTxt(FILE *arch, void *dato, unsigned tamLinea, accion Accion);
int escribirNuevoReg(FILE *arch, void *dato, unsigned tam);


#endif // ARCHIVOS_H
