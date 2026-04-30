/**
 * @file archivos.c
 * @brief Implementación de las operaciones de entrada/salida de archivos.
 *
 * Funciones para cargar configuraciones, mapas y guardar
 * el estado y usuarios de forma persistente.
 */

#include "../include/archivos.h"

/////////////////////
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

int mostrarArchivoTxt(FILE* arch, const char *nombreArchivo, accion Accion)
{
    char linea[TAM_LINEA];

    rewind(arch);

    fgets(linea,TAM_LINEA-1,arch);
    while(!feof(arch))
    {
        Accion(NULL, linea);
        fgets(linea,TAM_LINEA-1,arch);
    }

    return 1;
}

int leerArchivoTxt(FILE* arch, void *estDatos, unsigned tam, accion Accion, accion Cargar)
{
    void *aux = malloc(tam);
    if(!aux)
        return ERROR_MEM;

    char linea[TAM_LINEA];

    rewind(arch);

    fgets(linea,TAM_LINEA-1,arch);
    while(!feof(arch))
    {
        Accion(aux, linea);
        Cargar(estDatos, aux);
        fgets(linea,TAM_LINEA-1,arch);
    }

    free(aux);
    return 1;
}

int mostrarArchivoBin(FILE* arch, unsigned tam, accion Accion)
{
    void *aux = malloc(tam);
    if(!aux)
        return ERROR_MEM;

    rewind(arch);

    fread(aux,tam,1,arch);
    while(!feof(arch))
    {
        Accion(NULL, aux);
        fread(aux,tam,1,arch);
    }

    free(aux);
    return 1;
}

int leerArchivoBin(FILE* arch, void *estDatos, unsigned tam, accion Accion, accion Cargar)
{
    void *reg=malloc(tam);
    if(!reg)
        return ERROR_MEM;

    void *dato = malloc(tam);
    if(!dato)
    {
        free(reg);
        return ERROR_MEM;
    }

    rewind(arch);

    fread(reg,tam,1,arch);
    while(!feof(arch))

    {
        Accion(dato, reg);
        Cargar(estDatos, dato);
        fread(reg,tam,1,arch);
    }

    free(reg);
    free(dato);
    return 1;
}

////
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

int leerRegistro(FILE* arch, void *dato, unsigned tam)
{
    return fread(dato,tam,1,arch)==1;
}

int actualizarRegistro(FILE* arch, void *dato, unsigned tam, accion Accion)
{
    void *aux=malloc(tam);
    if(aux==NULL)
        return ERROR_MEM;

    leerRegistro(arch,dato,tam);

    fseek(arch,-tam,SEEK_CUR);

    Accion(aux,dato);

    escribirRegistro(arch,aux,tam);

    free(aux);
    return 1;
}

int leerPos(FILE* arch, void *dato, unsigned tam, int pos)
{
    fseek(arch,tam*pos,SEEK_SET);

    return fread(dato,tam,1,arch)==1;
}

int escribirPos(FILE* arch,const void *dato, unsigned tam, int pos)
{
    fseek(arch,tam*pos,SEEK_SET);

    return fwrite(dato,tam,1,arch)==1;
}

//////
//////

int cmpNombre(const void *a, const void *b)
{
    sJugador *j1 = (sJugador*)a;
    sJugador *j2 = (sJugador*)b;

    return strcmpi(j1->nombre,j2->nombre);
}

void trozarConfig(void *config, const void *dato)
{
    char *linea =(char*) dato;
    sConfig *c = (sConfig*) config;
    char parametro[TAM];
    int valor;
    sscanf(linea," %26[^: ] %*[: ] %d",parametro,&valor);

    if(strcmpi(parametro,"cantidad_posiciones")==0)
        c->cantPos=valor;

    else if(strcmpi(parametro,"vidas_inicio")==0)
        c->vidasIniciales=valor;

    else if(strcmpi(parametro,"maximo_bandidos")==0)
        c->maxBandidos=valor;

    else if(strcmpi(parametro,"maximo_premios")==0)
        c->maxPremios=valor;

    else if(strcmpi(parametro,"maximo_vidas_extra")==0)
        c->maxVidasExtra=valor;

    else if(strcmpi(parametro,"maximo_oasis")==0)
        c->maxOasis=valor;

    else if(strcmpi(parametro,"maximo_tormentas")==0)
        c->maxTormentas=valor;
}

void trozarMapa(void *mapa, const void *dato)
{
    char *linea =(char*) dato;
    sCasilla *c = (sCasilla*) mapa;
    sscanf(linea,"%d:%s",&c->pos,c->objetos);
}

void trozarJugador(void *jugador,const void *dato)
{
    sJugador *d = (sJugador*)dato;
    sJugador *j = (sJugador*)jugador;

    j->cantPartidas=d->cantPartidas;
    j->puntosTotales=d->puntosTotales;
    strcpy(j->nombre,d->nombre);
}


void trozarPartida(void *partida,const void *dato)
{
    sPartida *d = (sPartida*)dato;
    sPartida *p =(sPartida*)partida;

    p->movimientos=d->movimientos;
    strcpy(p->nombeJugador,d->nombeJugador);
    p->puntos=d->puntos;
}


void cargarLista(void *lista, const void *dato)
{

}

void mostrarLinea(void *a,const void *dato)
{
    char *d = (char*)dato;
    printf("%s\n", d);
}

void mostrarJugador(void *a ,const void *dato)
{
    sJugador *d = (sJugador*)dato;
    printf("%s|%d|%d\n", d->nombre,d->puntosTotales,d->cantPartidas);
}
