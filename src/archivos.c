/**
 * @file archivos.c
 * @brief Implementación de las operaciones de entrada/salida de archivos.
 *
 * Funciones para cargar configuraciones, mapas y guardar
 * el estado y usuarios de forma persistente.
 */

#include "../include/archivos.h"

int abrirArchivo(FILE ** arch, const char *nombreArchivo, const char *modoApertura, int mostrarError)
{
    *arch = fopen(nombreArchivo,modoApertura);

    if(*arch==NULL)
    {
        if(mostrarError==1)
            fprintf(stderr,"Error al abrir el archivo %s en modo %s.\n", nombreArchivo,modoApertura);
        return 0;
    }
    return 1;
}
int cerrarArchivo(FILE ** arch, const char *nombreArchivo, int mostrarError)
{
    if(fclose(*arch)==0)
        return 1; //Cerrado Correctamente

    else if(mostrarError==1)
        fprintf(stderr,"Error al cerrar el archivo %s.\n", nombreArchivo);

    return 0;

}
