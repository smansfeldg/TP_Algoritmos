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

int recorrerArchivoTxt(FILE* arch, const char *nombreArchivo, accion Accion)
{
    char linea[TAM_LINEA];
    while(fgets(linea,TAM_LINEA-1,arch))
    {
        Accion(linea);
    }

    return 1;
}

int recorrerArchivoBin(FILE* arch, const char *nombreArchivo, unsigned tam, accion Accion)
{
    void *aux;
    aux=malloc(tam);
    if(!aux)
        return 0;
    fread(aux,tam,1,arch);
    while(!feof(arch))

    {
        Accion(aux);
        fread(aux,tam,1,arch);
    }

    free(aux);

    return 1;
}

void mostrarLinea(void *dato)
{
    char *d = (char*)dato;
    printf("%s\n", d);
}

void mostrarBin(void *dato)
{
    sPrueba *d = (sPrueba*)dato;
    printf("%s|%d|%d\n", d->nombre,d->puntos,d->cantPartidas);
}

void crearPrueba(const char *nombreArchivo)
{
    FILE* arch;
    abrirArchivo(&arch,"prueba.bin","wb",1);
    sPrueba jugadores[]={{"Juan Perez",4,0},
                      {"Florencia Re",4,0},
                      {"Maria Benitez",7,5},
                      {"Victor Gomez",7,5},
                      {"Flavio Gomez",9,2},
                      {"Julieta Beltran",3,2},
                      {"Fernanda Gauna",9,1},
                      {"Lucas Grance",10,2},
                      {"Pablo Soligo",10,3}};
    unsigned tamanio = sizeof(sPrueba);
    int ce = sizeof(jugadores)/tamanio;

    fwrite(jugadores, sizeof(sPrueba), ce, arch);
    cerrarArchivo(&arch,"prueba.bin",1);
}
