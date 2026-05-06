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
        return ERROR_ARCH;
    }
    return OK;
}
int cerrarArchivo(FILE ** arch, const char *nombreArchivo, int mostrarError)
{
    if(fclose(*arch)==0)
        return OK;

    else if(mostrarError==1)
        fprintf(stderr,"Error al cerrar el archivo %s.\n", nombreArchivo);

    return ERROR_ARCH;

}

/////////////////////
/////////////////////
int leerArchivoTxt(FILE* arch, void *estDatos, unsigned tam, unsigned tamLinea, accion Accion, accion Cargar)
{
    void *aux = malloc(tam);
    if(aux==NULL)
        return ERROR_MEM;

    char *linea=malloc(tamLinea);
    if(linea==NULL)
    {
        free(aux);
        return ERROR_MEM;
    }

    rewind(arch);

    fgets(linea,tamLinea,arch);
    while(!feof(arch))
    {
        //Trozar linea y cargarla en aux.
        Accion(aux, linea);
        //Cargar aux en la estructura de datos correspondiente.
        Cargar(estDatos, aux);
        fgets(linea,tamLinea,arch);
    }

    free(linea);
    free(aux);
    return OK;
}

/////////////////////
/////////////////////
int leerArchivoBin(FILE* arch, void *estDatos, unsigned tam, accion Accion, accion Cargar)
{
    void *reg=malloc(tam);
    if(reg==NULL)
        return ERROR_MEM;

    void *dato=malloc(tam);
    if(dato==NULL)
    {
        free(reg);
        return ERROR_MEM;
    }

    rewind(arch);

    fread(reg,tam,1,arch);
    while(!feof(arch))

    {
        //Trozar registro y cargarlo en dato.
        Accion(dato, reg);
        //Cargar dato en la estructura de datos correspondiente.
        Cargar(estDatos, dato);
        fread(reg,tam,1,arch);
    }

    free(reg);
    free(dato);
    return OK;
}

/////////////////////
/////////////////////
int buscarRegistro(FILE* arch, void *reg, const void *dato, unsigned tam, cmp Cmp)
{
    void *aux=malloc(tam);
    if(aux==NULL)
        return ERROR_MEM;

    int pos=0;

    rewind(arch);

    //Busca el registro con un contador para la posicion.
    fread(aux, tam, 1, arch);
    while (!feof(arch))
    {
        if (Cmp(aux, dato) == 0)
        {
            //Carga el registro en *reg y devuelve su posicion.
            fseek(arch,-tam,SEEK_CUR);
            memcpy(reg,aux,tam);
            free(aux);
            return pos;
        }
        pos++;
        fread(aux, tam, 1, arch);
    }

    free(aux);
    //Si no se encuentra, devuelve -1 y ep puntero queda al final.
    return NO_ENCONTRADO;
}

int actualizarRegistro(FILE* arch, void *reg, const void *dato, unsigned tam, accion Accion)
{
    //Reg es obtenido del archivo y dato es conseguido durante la partida
    //Actualiza el registro a partir del dato.
    Accion(reg,dato);

    return fwrite(dato,tam,1,arch)==1;
}

int leerPos(FILE* arch, void *dato, unsigned tam, int pos)
{
    if(fseek(arch,tam*pos,SEEK_SET)!=0)
        return ERROR_ARCH;

    return fread(dato,tam,1,arch)==1;
}

int escribirPos(FILE* arch,const void *dato, unsigned tam, int pos)
{
    if(fseek(arch,tam*pos,SEEK_SET)!=0)
        return ERROR_ARCH;

    return fwrite(dato,tam,1,arch)==1;
}

/////////////////////
/////////////////////
int escribirTxt(FILE *arch, void *dato, unsigned tamLinea, accion Accion)
{
    char *linea=malloc(tamLinea);
    if(linea==NULL)
        return ERROR_MEM;

    //Da formato al dato y lo carga en un string.
    Accion(linea,dato);

    //Escribe en el archivo lo que este escrito en linea.
    if(fprintf(arch,"%s\n",linea)<0)
    {
        free(linea);
        return ERROR_ARCH;
    }

    free(linea);
    return OK;
}

// Funciones de puntaje
int guardarPuntaje(const char *nombreArchivo, const void *registro)
{
    FILE *arch = fopen(nombreArchivo, "ab");
    if(!arch) return 0;
    int r = fwrite(registro, sizeof(tRegistroPuntaje), 1, arch);
    fclose(arch);
    return r == 1;
}

int leerPuntajes(const char *nombreArchivo, void *registros, int maxRegistros)
{
    FILE *arch = fopen(nombreArchivo, "rb");
    if(!arch) return 0;
    int leidos = fread(registros, sizeof(tRegistroPuntaje), maxRegistros, arch);
    fclose(arch);
    return leidos;
}
