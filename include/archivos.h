#ifndef ARCHIVOS_H
#define ARCHIVOS_H

#include <stdio.h>
#include <stdlib.h>

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


//Utilizado como Prueba
typedef struct{
    char nombre[TAM];
    int puntos;
    int cantPartidas;
}sPrueba;

typedef void (*accion) (void *a);

//Funciones para mostrar archivos creadas como prueba
void mostrarTxt(void *dato);
void mostrarBin(void *dato);

int abrirArchivo(FILE ** arch, const char *nombreArchivo, const char *modoApertura, int mostrarError);
int cerrarArchivo(FILE ** arch, const char *nombreArchivo, int mostrarError);

int recorrerArchivoTxt(FILE* arch, const char *nombreArchivo, accion Accion);
int recorrerArchivoBin(FILE* arch, const char *nombreArchivo, unsigned tam, accion Accion);

//Crea un archivo de prueba para el binario
void crearPrueba(const char *nombreArchivo);

#endif // ARCHIVOS_H
