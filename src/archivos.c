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
            printf("Error al abrir el archivo %s en modo %s.", nombreArchivo,modoApertura);
        return 0;
    }
    return 1;
}
int cerrarArchivo(FILE* arch)
{
    if(fclose(arch)==0)
        return 1; //Cerrado Correctamente
    else
        return 0;

}
