/**
 * @file archivos.c
 * @brief Implementación de las operaciones de entrada/salida de archivos.
 *
 * Funciones para cargar configuraciones, mapas y guardar
 * el estado y usuarios de forma persistente.
 */

#include "../include/archivos.h"

////
int abrirArchivo(FILE ** arch, const char *nombreArchivo, const char *modoApertura, int mostrarError)
{
    *arch = fopen(nombreArchivo,modoApertura);

    if(*arch==NULL)
    {
        if(mostrarError==1)
            fprintf(stderr,"Error al abrir el archivo %s en modo %s.\n", nombreArchivo,modoApertura);
        return ERROR_ARCH;
    }
    return 1;
}
int cerrarArchivo(FILE ** arch, const char *nombreArchivo, int mostrarError)
{
    if(fclose(*arch)==0)
        return 1; //Cerrado Correctamente

    else if(mostrarError==1)
        fprintf(stderr,"Error al cerrar el archivo %s.\n", nombreArchivo);

    return ERROR_ARCH;

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
        return ERROR_MEM;

    rewind(arch);

    fread(aux,tam,1,arch);
    while(!feof(arch))

    {
        Accion(aux);
        fread(aux,tam,1,arch);
    }

    free(aux);

    return 1;
}

////
int escribirRegistro(FILE* arch,const void *dato, unsigned tam)
{
    return fwrite(dato, tam,1,arch)==1;
}


//Busca el registro, Si lo encuentra, se posiciona al principio
int buscarRegistro(FILE* arch,const void *dato, unsigned tam, cmp Cmp)
{
    rewind(arch);
    void *aux=malloc(tam);
    if(aux==NULL)
        return ERROR_MEM;
    int pos=0;

    fread(aux, tam, 1, arch);
    while (!feof(arch))
    {
        if (Cmp(aux, dato) == 0)
        {
            fseek(arch,-tam,SEEK_CUR);
            free(aux);
            return pos;
        }
        pos++;
        fread(aux, tam, 1, arch);
    }

    free(aux);
    return NO_ENCONTADO;
}

//Si encontre el registro, lo puedo leer o actualizar
int leerRegistro(FILE* arch, void *dato, unsigned tam)
{
    return fread(dato,tam,1,arch)==1;
}
int actualizarRegistro(FILE* arch, void *dato, unsigned tam)
{
    return fwrite(dato, tam,1,arch)==1;
}

////
int leerPos(FILE* arch, void *dato, unsigned tam, int pos)
{
    fseek(arch,tam*pos,SEEK_SET);

    return fread(dato,tam,1,arch)==1;
}
int escribirPos(FILE* arch, void *dato, unsigned tam, int pos)
{
    fseek(arch,tam*pos,SEEK_SET);

    return fwrite(dato,tam,1,arch)==1;
}

int cmpNombre(const void *a, const void *b)
{
    sJugador *j1 = (sJugador*)a;
    sJugador *j2 = (sJugador*)b;

    return strcmpi(j1->nombre,j2->nombre);
}


//////
//////
//////
void crearPrueba(const char *nombreArchivo)
{
    FILE* arch;
    abrirArchivo(&arch,"prueba.bin","wb",1);
    sJugador jugadores[]={{"Juan Perez",4,0},
                      {"Florencia Re",4,0},
                      {"Maria Benitez",7,5},
                      {"Victor Gomez",7,5},
                      {"Flavio Gomez",9,2},
                      {"Julieta Beltran",3,2},
                      {"Fernanda Gauna",9,1},
                      {"Lucas Grance",10,2},
                      {"Pablo Soligo",10,3}};
    unsigned tamanio = sizeof(sJugador);
    int ce = sizeof(jugadores)/tamanio;

    fwrite(jugadores, sizeof(sJugador), ce, arch);
    cerrarArchivo(&arch,"prueba.bin",1);
}

void mostrarLinea(const void *dato)
{
    char *d = (char*)dato;
    printf("%s\n", d);
}

void mostrarBin(const void *dato)
{
    sJugador *d = (sJugador*)dato;
    printf("%s|%d|%d\n", d->nombre,d->puntosTotales,d->cantPartidas);
}
