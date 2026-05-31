#include "../include/entidades.h"
#include <time.h>
#include <ctype.h>

//LISTO
void crearJugador(tJugador *j, const char *nombre, int posicionInicial, int vidas)
{
    strncpy(j->nombre, nombre, MAX_NOMBRE - 1);
    j->nombre[MAX_NOMBRE - 1] = '\0';
    j->posicion = posicionInicial;
    j->vidas = vidas;
    j->puntos = 0;
    j->protegidoOasis = 0;
    j->perdidoTurno = 0;
}

//LISTO
void crearBandido(tBandido *b, int id, int posicion)
{
    b->id = id;
    b->posicion = posicion;
    b->activo = 1;
}

//Listo
int cargarConfiguracion(const char *archivo, tConfiguracion *cfg)
{
    FILE* arch;
    abrirArchivo(&arch,archivo,"rt",0);

    leerArchivoTxt(arch, cfg, sizeof(tConfiguracion), sizeof(char)*51, trozarConfig);

    fclose(arch);
    return 1;
}
void trozarConfig(void *config, const void *dato)
{
    char *linea =(char*) dato;
    tConfiguracion *c = (tConfiguracion*) config;
    char parametro[50];
    int valor;
    sscanf(linea," %26[^: ] %*[: ] %d",parametro,&valor);

    if(strcmpi(parametro,"TOTAL_CASILLAS")==0)
        c->totalCasillas=valor;

    else if(strcmpi(parametro,"VIDAS_INICIALES")==0)
        c->vidasIniciales=valor;

    else if(strcmpi(parametro,"CANTIDAD_BANDIDOS")==0)
        c->cantidadBandidos=valor;

    else if(strcmpi(parametro,"CANTIDAD_PREMIOS")==0)
        c->cantidadPremios=valor;

    else if(strcmpi(parametro,"CANTIDAD_VIDAS")==0)
        c->cantidadVidas=valor;

    else if(strcmpi(parametro,"CANTIDAD_OASIS")==0)
        c->cantidadOasis=valor;

    else if(strcmpi(parametro,"CANTIDAD_TORMENTAS")==0)
        c->cantidadTormentas=valor;
}

//LISTO
void mostrarConfiguracion(const tConfiguracion *cfg){
  printf("Los parametros de configuracion son:\nCantidad de Casillas del Tablero:\t%d\nCantidad de Oasis:\t%d\nCantidad de Premios:\t%d\nCantidad de Tormentas:\t%d\nCantidad de Vidas Extra:\t%d\nCantidad de Vida Inicial:\t%d\nCantidad de Bandidos:\t%d\n",
         cfg->totalCasillas,cfg->cantidadOasis,cfg->cantidadPremios,cfg->cantidadTormentas,cfg->cantidadVidas,cfg->vidasIniciales,cfg->cantidadBandidos);
}

// Ah debatir, pero esta logica provicional funciona
int generarTablero(tLista *tablero, const tConfiguracion *cfg)
{
    int i;
    int sumaEspeciales = cfg->cantidadOasis + cfg->cantidadPremios + cfg->cantidadTormentas + cfg->cantidadVidas;

    if (sumaEspeciales > cfg->totalCasillas - 2) {
        return 0; // No se puede generar, demasiadas casillas especiales
    }

    tCasilla *casillas = (tCasilla*)malloc(sizeof(tCasilla) * cfg->totalCasillas);
    if (!casillas) return 0;

    // Inicializar todo como normal
    for(i = 0; i < cfg->totalCasillas; i++)
    {
        casillas[i].posicion = i;
        casillas[i].tipo = TIPO_NORMAL;
        strcpy(casillas[i].descripcion, "Casilla Normal");
    }

    casillas[0].tipo = TIPO_INICIO;
    strcpy(casillas[0].descripcion, "Inicio");

    // Asignar especiales aleatoriamente
    int c;

    c = 0;
    while(c < cfg->cantidadOasis) {
        int pos = (rand() % (cfg->totalCasillas - 2)) + 1;
        if(casillas[pos].tipo == TIPO_NORMAL) {
            casillas[pos].tipo = TIPO_OASIS;
            strcpy(casillas[pos].descripcion, "Oasis");
            c++;
        }
    }

    c = 0;
    while(c < cfg->cantidadPremios) {
        int pos = (rand() % (cfg->totalCasillas - 2)) + 1;
        if(casillas[pos].tipo == TIPO_NORMAL) {
            casillas[pos].tipo = TIPO_PREMIO;
            strcpy(casillas[pos].descripcion, "Premio");
            c++;
        }
    }

    c = 0;
    while(c < cfg->cantidadTormentas) {
        int pos = (rand() % (cfg->totalCasillas - 2)) + 1;
        if(casillas[pos].tipo == TIPO_NORMAL) {
            casillas[pos].tipo = TIPO_TORMENTA;
            strcpy(casillas[pos].descripcion, "Tormenta");
            c++;
        }
    }

    c = 0;
    while(c < cfg->cantidadVidas) {
        int pos = (rand() % (cfg->totalCasillas - 2)) + 1;
        if(casillas[pos].tipo == TIPO_NORMAL) {
            casillas[pos].tipo = TIPO_VIDA;
            strcpy(casillas[pos].descripcion, "Vida Extra");
            c++;
        }
    }

    // El ultimo nodo podría ser el refugio? El TP no lo especifica claro, asignemos al final.
    casillas[cfg->totalCasillas - 1].tipo = TIPO_REFUGIO;
    strcpy(casillas[cfg->totalCasillas - 1].descripcion, "Refugio / Fin");

    // Insertar en la lista
    for(i = 0; i < cfg->totalCasillas; i++)
    {
        insertarAlFinal(tablero, &casillas[i], sizeof(tCasilla));
    }

    free(casillas);
    return 1;
}

// Helper para mostrarCasilla
void mostrarCasilla(const void *info)
{
    if (!info) return;
    tCasilla *casilla = (tCasilla *)info;
    printf("[%c]", casilla->tipo);
}

//EN DEBATE
void mostrarTablero(const tLista *tablero)
{
    printf("\nTablero: ");
    mostrarDeIzqADer(tablero, mostrarCasilla);
    printf("\n");
}

//HACER
//DEBERIA HABER UNA FUNCIÓN PARA BUSCAR EN LA IMPLEMENTACION DE LISTA CIRCULAR
//ESTA FUNCION DEBERIA RECIBIR UNA FUNCION DE COMPARACION DE PARA POSICION DE CASILLAS
tCasilla* buscarCasilla(const tLista *tablero, int posicion)
{
    //DEBERIA LLAMAR LA FUNCION DE BUSQUEDA CON LA FUNCION DE BUSQUEDA DE POSICION DE CASILLA
    return 0;
}

//LISTA
int moverJugador(tJuego* juego, tMovimiento movimiento){
    int nuevaPos, sentido;

    if(movimiento.direccion == 'F'){
      sentido = 1;
    }
    else if(movimiento.direccion == 'B'){
      sentido = -1;
    }

    nuevaPos = juego->jugador.posicion + movimiento.pasos*sentido;

    if(nuevaPos > juego->config.totalCasillas){
      nuevaPos = juego->config.totalCasillas - (nuevaPos%juego->config.totalCasillas);
    }

    posicionarJugador(juego, nuevaPos);

    return nuevaPos;
}

//LISTA PERO REVISAR SI SE PUEDE USAR PARA VERIFICAR VICTORIA
void aplicarEfectoCasilla(tJugador *j, char tipoCasilla)
{
    switch (tipoCasilla)
    {
    case TIPO_PREMIO:
        j->puntos += 10;
        break;
    case TIPO_VIDA:
        j->vidas += 1;
        break;
    case TIPO_OASIS:
        j->protegidoOasis = 1;
        break;
    case TIPO_TORMENTA:
        if (j->protegidoOasis) {
            j->protegidoOasis = 0;
        } else {
            j->perdidoTurno = 1;
        }
        break;
    }
}

//LISTO
void mostrarEstadoJugador(const tJugador *j){
  if(j->perdidoTurno){
    puts("EL JUGADOR PIERDE EL PROXIMO TURNO POR UNA TORMENTA DE ARENA");
  }
  if(j->protegidoOasis){
    puts("EL JUGADOR DESCANSA EN UN OASIS Y ESTARA PROTEGIDO HASTA EL PROXIMO TURNO");
  }
}

//HACER
void inicializarJuego(tJuego *juego, tConfiguracion *cfg){
  juego->config = *cfg;
  crearLista(&juego->tablero); // Asegurar que la lista se inicializa a NULL, de forma provisoria qeuda así
  CrearCola(&juego->colaMovimientos);
  CrearCola(&juego->colaMovimientosJugador);
  crearLista(&juego->bandidos);
  juego->totalMovimientos=0;
  juego->juegoActivo=1;
  juego->turnoActual=1;

  generarTablero(&juego->tablero,cfg);
}

//HACER
void liberarJuego(tJuego *juego)
{
  //LO QUE DICE EL NOMBRE
}

//HACER
int encolarMovimiento(tCola *cola, tMovimiento movimiento){
  return PonerEnCola(cola,&movimiento,sizeof(tMovimiento));
}

//LISTO
int desencolarMovimiento(tCola *cola, tMovimiento *mov){
    return SacarDeCola(cola,mov,sizeof(tMovimiento)); //SacarDeCola
}

//LISTO
void mostrarColaMovimientos(tCola *cola){
  tMovimiento mov;

  puts("Movimientos realizados por el jugador:");
  while(!ColaVacia(cola)){
    desencolarMovimiento(cola,&mov);
    printf("%c%d\n",mov.direccion,mov.pasos);
  }
}

//LISTO
int moverBandido(tJuego *juego, tBandido *b, tMovimiento movimiento){
    int nuevaPos, sentido;

    if(movimiento.direccion == 'F'){
      sentido = 1;
    }
    else if(movimiento.direccion == 'B'){
      sentido = -1;
    }

    nuevaPos = b->posicion + movimiento.pasos*sentido;

    if(nuevaPos > juego->config.totalCasillas){
      nuevaPos = (nuevaPos%juego->config.totalCasillas);
    }

    if(nuevaPos < 1){
      nuevaPos *= -1;
    }

    b->posicion = nuevaPos;

    verificarColision(juego, b);
    return 1;
}

//LISTO
int verificarColision(tJuego *juego, tBandido *b){
    int colision;

    if( (colision = ((juego->jugador.posicion == b->posicion)) && b->activo)){
      b->activo = 0;
      if(!juego->jugador.protegidoOasis){
        juego->jugador.vidas -= 1;
        posicionarJugador(juego, 1);
      }
    }

    return colision;
}

//REVISAR (EN ESPECIAL LOS PARAMETROS)
int verificarVictoria(const tJugador *j, const tLista *tablero)
{
    //LO QUE DICE EL NOMBRE
    return 0;
}

//LISTO
int verificarDerrota(const tJugador *j)
{
    return (j->vidas <= 0);
}

//HACER
void guardarCaravana(const char *archivo, const tJuego *juego)
{

}

//HACER
int cargarCaravana(const char *archivo, tJuego *juego)
{
    return 1;
}

//LISTO
int posicionarJugador(tJuego *juego, int posicion){
  tNodoListaC *bandidoActual, *bandidoIni;

  juego->jugador.posicion=posicion;

  //GUARDAR EL PRIMER BANDIDO
  bandidoIni = juego->bandidos;
  bandidoActual = bandidoIni;
  do{//RECORRER LISTA BANDIDOS
    if(verificarColision(juego, bandidoActual->info)){
      break;
    }
    bandidoActual = juego->bandidos->sig;
  }while( ((tBandido*)(bandidoActual->info))->id != ((tBandido*)(bandidoIni->info))->id);

  return 1;
}

//ADEMAS DEBERIA HABER UN PAR DE FUNCIONES PARA GUARDAR Y CARGAR JUGADORES, OTRO PAR PARA LAS PARTIDAS Y OTRO PAR PARA EL INDICE
int IndexarArchivo(FILE *archJ, char *nombreArch, ArbolBin *arbolIndx, unsigned tam)
{
    FILE *archIndice;
    if(abrirArchivo(&archIndice,nombreArch,"rb",0)==0)
       return 0;

    crearArbolBin(arbolIndx);
    tRegistroJugador jugador;
    tIndice auxIndx;

    fseek(archIndice,0,SEEK_END);
    int nroRegistro=0;

    while(fread(&jugador,sizeof(tRegistroJugador),1,archJ)==1)
    {
        auxIndx.registro=nroRegistro;
        strcpy(auxIndx.nombre,jugador.nombre);
        insertarNodoArbolBin(arbolIndx,&auxIndx,sizeof(tIndice),cmpIndxNombre);
        nroRegistro++;
    }

    cerrarArchivo(&archIndice,nombreArch,0);

    return 1;
}

void actualizarJugadores(FILE *archJug, ArbolBin *indice, const tRegistroJugador *nuevo)
{
    escribirNuevoReg(archJug,nuevo,sizeof(tRegistroJugador));
    fflush(archJug);

    tIndice aux;
    strcpy(aux.nombre,nuevo->nombre);
    aux.registro=(ftell(archJug)/sizeof(tRegistroJugador))-1;

    insertarNodoArbolBin(indice,&aux,sizeof(tIndice), cmpIndxNombre);
}
int cmpIndxNombre(const void *a, const void *b)
{
    tIndice *indx1=(tIndice*)a;
    tIndice *indx2=(tIndice*)b;

    return strcmp(indx1->nombre,indx2->nombre);
}

int archivarIndice(char *nombreArch, ArbolBin *arbolIndx)
{
    FILE *archIndice;
    if(abrirArchivo(&archIndice,nombreArch,"wb",0)==0)
       return 0;

    recorrerEnOrdenArbolBin(arbolIndx,0,archIndice, guardNodoIndxEnArchivo);

    return cerrarArchivo(&archIndice,nombreArch,0);
}

void guardNodoIndxEnArchivo(void *dato, size_t tam, unsigned nivel, void *params)
{
    FILE *arch=(FILE*)params;
    fwrite(dato,tam,1,arch);
}

int buscarJugador(char *nombre, ArbolBin *indice, cmp Cmp)
{
    return(buscarNodoArbolBin(indice,nombre,Cmp)!=NULL);
}

/////
////
int crearRanking(tLista *ranking, FILE* arch, ArbolBin *indice, FILE *archJug)
{
    crearLista(ranking);
    leerArchivoBin(arch,ranking,sizeof(tRegistroRanking),cargarRanking);

    return 1;
}

void cargarRanking(void *ranking, const void *dato)
{
    tRegistroPartida *partida=(tRegistroPartida*)dato;
    tRegistroRanking reg;

    strcpy(reg.nombre,partida->nombre);
    reg.idJugador=0;
    reg.cantidadPartidas=1;
    reg.puntuacionTotal=partida->puntuacion;
    insertarEnOrden(ranking,&reg,sizeof(tRegistroRanking),cmpRankNombres,sumarRankPuntos);

}
int cmpRankNombres(const void*a, const void *b)
{
    tRegistroRanking *reg1=(tRegistroRanking*)a;
    tRegistroRanking *reg2=(tRegistroRanking*)b;

    return strcmp(reg1->nombre,reg2->nombre);
}

int sumarRankPuntos(void **dato1, unsigned *tam1, const void* dato2, unsigned tam2)
{
    tRegistroRanking **reg1=(tRegistroRanking**)dato1;
    tRegistroRanking *reg2=(tRegistroRanking*)dato2;

    (*reg1)->puntuacionTotal+=reg2->puntuacionTotal;
    (*reg1)->cantidadPartidas++;

    return 1;
}

int cmpRankPuntos(const void *a, const void *b)
{
    tRegistroRanking *reg1=(tRegistroRanking*)a;
    tRegistroRanking *reg2=(tRegistroRanking*)b;

    return reg2->puntuacionTotal-reg1->puntuacionTotal;
}

int actualizarRegistroPartidas(FILE* arch, tLista *ranking, const tRegistroPartida *partida)
{
    escribirNuevoReg(arch,partida,sizeof(tRegistroPartida));

    tRegistroRanking nuevo;

    nuevo.idJugador=0;
    strcpy(nuevo.nombre,partida->nombre);
    nuevo.cantidadPartidas=1;
    nuevo.puntuacionTotal=partida->puntuacion;
    insertarEnOrden(ranking,&nuevo,sizeof(tRegistroRanking),cmpRankNombres,sumarRankPuntos);

    return 1;
}
void mostrarListaRanking(tLista *ranking)
{
    ordenarLista(ranking, cmpRankPuntos);
    printf("\nRANKING DE JUGADORES:\n");
    printf("---\n");
    mostrarDeIzqADer(ranking, mostrarRanking);
    printf("\n---\n");
    ordenarLista(ranking, cmpRankNombres);
}
void mostrarRanking(const void *reg)
{
    tRegistroRanking *ranking=(tRegistroRanking*)reg;
    printf("%s\tCodigo: %d |  Puntos: %d  | Cantidad de Partidas: %d\n",ranking->nombre,ranking->idJugador,ranking->puntuacionTotal,ranking->cantidadPartidas);
}
