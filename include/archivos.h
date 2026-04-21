#ifndef ARCHIVOS_H
#define ARCHIVOS_H

#include <stdio.h>
#include <stdlib.h>

/**
 * @file archivos.h
 * @brief Manejo de persistencia y lectura de archivos.
 *
 * Capa de Acceso a Datos.
 * Gestión de lectura de config.txt, caravana.txt y la
 * persistencia binaria de los usuarios registrados.
 */


int abrirArchivo(FILE ** arch, const char *nombreArchivo, const char *modoApertura, int mostrarError);
int cerrarArchivo(FILE* arch);



#endif // ARCHIVOS_H
