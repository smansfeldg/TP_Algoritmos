#define _CRT_SECURE_NO_WARNINGS

/**
 * @file archivos.c
 * @brief Implementación de las operaciones de entrada/salida de archivos.
 *
 * Funciones para cargar configuraciones, mapas y guardar
 * el estado y usuarios de forma persistente.
 */

#include "../include/archivos.h"
#include "../include/abb.h"

typedef struct {
    char nombre[MAX_NOMBRE];
    int puntuacionTotal;
    int cantidadPartidas;
} tRankingNombre;

static int compararJugadorPorNombre(const void *a, const void *b)
{
    const tRegistroJugador *jugA = (const tRegistroJugador *)a;
    const tRegistroJugador *jugB = (const tRegistroJugador *)b;
    return strcmp(jugA->nombre, jugB->nombre);
}

static int compararRankingDesc(const void *a, const void *b)
{
    const tRankingNombre *rA = (const tRankingNombre *)a;
    const tRankingNombre *rB = (const tRankingNombre *)b;

    if (rA->puntuacionTotal != rB->puntuacionTotal) {
        return rB->puntuacionTotal - rA->puntuacionTotal;
    }
    return strcmp(rA->nombre, rB->nombre);
}

static void persistirIndiceJugadores(const char *archivoIndice, tRegistroJugador *jugadores, int cantidad)
{
    FILE *indice;

    if (cantidad > 1) {
        qsort(jugadores, cantidad, sizeof(tRegistroJugador), compararJugadorPorNombre);
    }

    indice = fopen(archivoIndice, "wb");
    if (!indice) return;

    /*
     * El indice persistido se guarda ordenado por nombre. Al iniciar una partida
     * se reconstruye un ABB con estos registros para mantener la busqueda separada
     * del archivo principal de jugadores.
     */
    fwrite(jugadores, sizeof(tRegistroJugador), cantidad, indice);
    fclose(indice);
}

static void cargarIndiceEnArbol(const char *archivoIndice, ArbolBin *indice)
{
    FILE *arch = fopen(archivoIndice, "rb");
    tRegistroJugador reg;

    crearArbolBin(indice);
    if (!arch) return;

    while (fread(&reg, sizeof(tRegistroJugador), 1, arch) == 1) {
        insertarNodoArbolBin(indice, &reg, sizeof(tRegistroJugador), compararJugadorPorNombre);
    }

    fclose(arch);
}

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

    return fwrite(reg,tam,1,arch)==1;
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
    int r = fwrite(registro, sizeof(tRegistroPartida), 1, arch);
    fclose(arch);
    return r == 1;
}

int leerPuntajes(const char *nombreArchivo, void *registros, int maxRegistros)
{
    FILE *arch = fopen(nombreArchivo, "rb");
    if(!arch) return 0;
    int leidos = fread(registros, sizeof(tRegistroPartida), maxRegistros, arch);
    fclose(arch);
    return leidos;
}

int obtenerORegistrarJugador(const char *archivoJugadores, const char *archivoIndice, tJugador *jugador)
{
    FILE *arch;
    tRegistroJugador jugadores[MAX_JUGADORES];
    tRegistroJugador buscado;
    ArbolBin indice;
    NodoRaiz *nodo;
    int cantidad = 0;
    int i;

    if (!jugador) return 0;

    cargarIndiceEnArbol(archivoIndice, &indice);
    buscado.idJugador = 0;
    strncpy(buscado.nombre, jugador->nombre, MAX_NOMBRE - 1);
    buscado.nombre[MAX_NOMBRE - 1] = '\0';

    nodo = buscarNodoArbolBin(&indice, &buscado, compararJugadorPorNombre);
    if (nodo) {
        tRegistroJugador *reg = (tRegistroJugador *)nodo->dato;
        jugador->idJugador = reg->idJugador;
        liberarArbolBin(&indice);
        return 1;
    }
    if (indice) liberarArbolBin(&indice);

    arch = fopen(archivoJugadores, "rb");
    if (arch) {
        while (cantidad < MAX_JUGADORES &&
               fread(&jugadores[cantidad], sizeof(tRegistroJugador), 1, arch) == 1) {
            cantidad++;
        }
        fclose(arch);
    }

    for (i = 0; i < cantidad; i++) {
        if (strcmp(jugadores[i].nombre, jugador->nombre) == 0) {
            jugador->idJugador = jugadores[i].idJugador;
            persistirIndiceJugadores(archivoIndice, jugadores, cantidad);
            return 1;
        }
    }

    if (cantidad >= MAX_JUGADORES) return 0;

    jugadores[cantidad].idJugador = cantidad + 1;
    strncpy(jugadores[cantidad].nombre, jugador->nombre, MAX_NOMBRE - 1);
    jugadores[cantidad].nombre[MAX_NOMBRE - 1] = '\0';
    jugador->idJugador = jugadores[cantidad].idJugador;
    cantidad++;

    arch = fopen(archivoJugadores, "wb");
    if (!arch) return 0;
    for (i = 0; i < cantidad; i++) {
        fwrite(&jugadores[i], sizeof(tRegistroJugador), 1, arch);
    }
    fclose(arch);

    persistirIndiceJugadores(archivoIndice, jugadores, cantidad);
    return 1;
}

int guardarRegistroPartida(const char *nombreArchivo, const tRegistroPartida *registro)
{
    FILE *arch;
    tRegistroPartida reg;
    int cantidad = 0;

    if (!registro) return 0;

    arch = fopen(nombreArchivo, "rb");
    if (arch) {
        while (fread(&reg, sizeof(tRegistroPartida), 1, arch) == 1) {
            cantidad++;
        }
        fclose(arch);
    }

    reg = *registro;
    reg.idPartida = cantidad + 1;

    arch = fopen(nombreArchivo, "ab");
    if (!arch) return 0;
    cantidad = fwrite(&reg, sizeof(tRegistroPartida), 1, arch) == 1;
    fclose(arch);
    return cantidad;
}

void mostrarRankingJugadores(const char *nombreArchivo)
{
    FILE *arch = fopen(nombreArchivo, "rb");
    tRankingNombre ranking[MAX_JUGADORES];
    tRegistroPartida partida;
    int cantidad = 0;
    int i;

    if (!arch) {
        printf("\nTodavia no hay partidas registradas.\n");
        return;
    }

    while (fread(&partida, sizeof(tRegistroPartida), 1, arch) == 1) {
        int pos = -1;
        for (i = 0; i < cantidad; i++) {
            if (strcmp(ranking[i].nombre, partida.nombre) == 0) {
                pos = i;
                break;
            }
        }
        if (pos == -1 && cantidad < MAX_JUGADORES) {
            pos = cantidad++;
            strncpy(ranking[pos].nombre, partida.nombre, MAX_NOMBRE - 1);
            ranking[pos].nombre[MAX_NOMBRE - 1] = '\0';
            ranking[pos].puntuacionTotal = 0;
            ranking[pos].cantidadPartidas = 0;
        }
        if (pos != -1) {
            ranking[pos].puntuacionTotal += partida.puntuacion;
            ranking[pos].cantidadPartidas++;
        }
    }
    fclose(arch);

    if (cantidad == 0) {
        printf("\nTodavia no hay partidas registradas.\n");
        return;
    }

    qsort(ranking, cantidad, sizeof(tRankingNombre), compararRankingDesc);

    printf("\n========== Ranking ==========\n");
    printf("%-4s %-20s %-8s %-8s\n", "Nro", "Jugador", "Puntos", "Partidas");
    for (i = 0; i < cantidad; i++) {
        printf("%-4d %-20s %-8d %-8d\n",
               i + 1,
               ranking[i].nombre,
               ranking[i].puntuacionTotal,
               ranking[i].cantidadPartidas);
    }
}
