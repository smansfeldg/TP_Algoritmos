#include "../include/entidades.h"
#include <time.h>
#include <ctype.h>

#ifndef _WIN32
      int strcmpi(const char* c1, const char* c2){
        return strcasecmp(c1, c2);
      }
#endif

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

//Listo
void trozarConfig(void *config, const void *dato)
{
    char *linea =(char*) dato;
    tConfiguracion *c = (tConfiguracion*) config;
    char parametro[50];
    int valor;
    sscanf(linea," %49[^: ] %*[: ] %d",parametro,&valor);



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
  printf("Los parametros de configuracion son:\nCantidad de Casillas del Tablero: %d\nCantidad de Oasis:\t\t  %d\nCantidad de Premios:\t\t  %d\nCantidad de Tormentas:\t\t  %d\nCantidad de Vidas Extra:\t  %d\nCantidad de Vida Inicial:\t  %d\nCantidad de Bandidos:\t\t  %d\n",
         cfg->totalCasillas,cfg->cantidadOasis,cfg->cantidadPremios,cfg->cantidadTormentas,cfg->cantidadVidas,cfg->vidasIniciales,cfg->cantidadBandidos);
}

//Listo
int generarTablero(tJuego *juego, const tConfiguracion *cfg){
    tBandido bandidoAux;
    int i;
    int sumaEspeciales = cfg->cantidadOasis + cfg->cantidadTormentas;

    if (sumaEspeciales > cfg->totalCasillas - 2) {
        return 0; // No se puede generar, demasiadas casillas especiales
    }

    tCasilla *casillas = (tCasilla*)malloc(sizeof(tCasilla) * cfg->totalCasillas);
    if (!casillas) return 0;

    // Inicializar todo como normal
    for(i = 0; i < cfg->totalCasillas; i++){
        casillas[i].posicion = i+1;
        casillas[i].normal = 1;
        casillas[i].bandidos = 0;
        casillas[i].inicio = 0;
        casillas[i].jugador = 0;
        casillas[i].oasis = 0;
        casillas[i].premios = 0;
        casillas[i].refugio = 0;
        casillas[i].tormenta = 0;
        casillas[i].vidas = 0;
    }

    casillas[0].inicio = 1;
    casillas[0].normal = 0;
    casillas[0].jugador = 1;

    // Asignar especiales aleatoriamente
    int c;

    c = 0;
    while(c < cfg->cantidadOasis) {
        int pos = (rand() % (cfg->totalCasillas - 2)) + 1;
        if(casillas[pos].normal){
            casillas[pos].normal = 0;
            casillas[pos].oasis  = 1;
            c++;
        }
    }

    c = 0;
    while(c < cfg->cantidadPremios) {
        int pos = (rand() % (cfg->totalCasillas - 2)) + 1;
        casillas[pos].normal = 0;
        casillas[pos].premios++;
        c++;
    }

    c = 0;
    while(c < cfg->cantidadTormentas) {
        int pos = (rand() % (cfg->totalCasillas - 2)) + 1;
        if(casillas[pos].normal) {
            casillas[pos].normal = 0;
            casillas[pos].tormenta = 1;
            c++;
        }
    }

    c = 0;
    while(c < cfg->cantidadVidas) {
        int pos = (rand() % (cfg->totalCasillas - 2)) + 1;
        casillas[pos].normal = 0;
        casillas[pos].vidas++;
        c++;
    }

    c = 0;
    while(c < cfg->cantidadBandidos) {
        int pos = (rand() % (cfg->totalCasillas - 2)) + 1;
        casillas[pos].normal = 0;
        casillas[pos].bandidos++;
        crearBandido(&bandidoAux,c,pos+1);

        insertarAlComienzo(&juego->bandidos,&bandidoAux,sizeof(tBandido));
        c++;
    }


    // El ultimo nodo debe ser el refugio
    casillas[cfg->totalCasillas - 1].refugio = 1;
    casillas[cfg->totalCasillas - 1].normal = 0;

    // Insertar en la lista
    for(i = 0; i < cfg->totalCasillas; i++){
        insertarAlFinal(&juego->tablero, &casillas[i], sizeof(tCasilla));
    }

    free(casillas);
    return 1;
}

//LISTO
tCasilla* buscarCasilla(const tLista *tablero, int posicion, int (*cmp)(const void*, const void*)){
    tCasilla *ret=NULL, aBuscar;
    aBuscar.posicion = posicion;

    buscarElementoLista(tablero, (void**)(&ret), &aBuscar, cmp);
    return ret;
}

//LISTA
//DEVUELVE 1 SI MOVIO SIN PROBLEMAS Y 0 SI EL JUGADOR PERDIO
int moverJugador(tJuego* juego, tMovimiento movimiento, tCasilla* casillaActual){
    int nuevaPos, sentido;

    if(movimiento.direccion == 'F'){
      sentido = 1;
      printf("El jugador se movio %d casillas hacia adelante\n", movimiento.pasos);
    }
    else if(movimiento.direccion == 'B'){
      sentido = -1;
      printf("El jugador se movio %d casillas hacia atrás\n", movimiento.pasos);
    }

    nuevaPos = juego->jugador.posicion + movimiento.pasos*sentido;

    //ANTES DE ABANDONAR LA CASILLA CASILLA->JUGADOR = 0;
    casillaActual->jugador = 0;

    if(nuevaPos > juego->config.totalCasillas){
      nuevaPos = juego->config.totalCasillas - (nuevaPos%juego->config.totalCasillas);
    }

    return posicionarJugador(juego, nuevaPos);
}

//LISTA PERO REVISAR SI SE PUEDE USAR PARA VERIFICAR VICTORIA
int aplicarEfectoCasilla(tJugador *j, tCasilla *casilla){
    //CONDICION DE VICTORIA
    if(casilla->refugio){
      return 1;
    }

    if(!casilla->normal){
      if(casilla->oasis){
        j->protegidoOasis = 1;
        puts("El jugador descansa en un oasis y estara protegido hasta su proximo turno");
        casilla->oasis = 0;
      }
      if(casilla->tormenta){
        if(j->protegidoOasis){
            j->protegidoOasis = 0;
            puts("¡El jugador fue protegido de la tormenta por el poder del Oasis!");
        }
        else{
            puts("El jugador pierde el proximo turno debido a una tormenta de arena que no lo deja avanzar");
            j->perdidoTurno = 1;
        }
        casilla->tormenta = 0;
      }
      if(casilla->vidas){
        printf("El jugador gano %d vidas\n",casilla->vidas);
        j->vidas += casilla->vidas;
        casilla->vidas = 0;
      }
      if(casilla->premios){
        printf("El jugador encontró %d premios\n", casilla->premios);
        j->puntos += 10 * casilla->premios;
        casilla->premios = 0;
      }
    }
    return 0;
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

  generarTablero(juego,cfg);
}

//HACER
void liberarJuego(tJuego *juego)
{
  //LO QUE DICE EL NOMBRE
}

//LISTO
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
int moverBandido(tJuego *juego, tBandido *b, tMovimiento movimiento, tCasilla *casillaActual){
    int nuevaPos, sentido;

    if(movimiento.direccion == 'F'){
      sentido = 1;
    }
    else if(movimiento.direccion == 'B'){
      sentido = -1;
    }

    nuevaPos = b->posicion + movimiento.pasos*sentido;

    //ANTES DE ABANDONAR LA CASILLA CASILLA.BANDIDO--;
    casillaActual->bandidos--;

    if(nuevaPos > juego->config.totalCasillas){
      nuevaPos = (nuevaPos%juego->config.totalCasillas);
    }

    if(nuevaPos < 1){
      nuevaPos = juego->config.totalCasillas+nuevaPos;
    }

    b->posicion = nuevaPos;

    casillaActual = buscarCasilla(&juego->tablero,b->posicion,cmpPosCasillas);
    //DESPUES DE LLEGAR A LA NUEVA CASILLA CASILLA->BANDIDO++;
    casillaActual->bandidos++;
    casillaActual->normal = 0;

    return verificarColision(juego, b, casillaActual);
}

//LISTO
//DEVUELVE 1 EN COLISION, 0 SI NO PASO NADA Y -1 Y EL JUGADOR PERDIO
int verificarColision(tJuego *juego, tBandido *b, tCasilla *casillaActual){
    int colision;

    if( (colision = ((juego->jugador.posicion == b->posicion)) && b->activo)){
      b->activo = 0;
      //DESPUES DE COLISIONAR CON JUGADOR CASILLA->BANDIDO--;
      casillaActual->bandidos--;
      puts("¡El jugador fue golpeado por un bandido!");
      if(!juego->jugador.protegidoOasis){
        juego->jugador.vidas -= 1;
        if(juego->jugador.vidas==0){
          return -1;
        }
        //DESPUES DE COLISIONAR CON JUGADOR CASILLA->JUGADOR = 0;
        casillaActual->jugador = 0;
        posicionarJugador(juego, 1);
      }
      else{
        puts("¡Pero fue protegido por el poder del Oasis!");
        juego->jugador.protegidoOasis=0;
      }
    }

    return colision;
}

//LISTO
int verificarDerrota(const tJugador *j){
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

void comprobarColisionesJB(void* bandido, void* contexto){
  tBandido* bandidoActual = (tBandido*)bandido;
  tJuego* juego = (tJuego*)contexto;
  tCasilla *casillaActual = buscarCasilla(&juego->tablero,bandidoActual->posicion,cmpPosCasillas);

  if(verificarColision(juego, bandidoActual, casillaActual)==-1){
    juego->juegoActivo=0;
  }
}

//LISTO
//DEVUELVE 1 SI PUDO POSICIONAR Y 0 SI EL JUGADOR PERDIO
int posicionarJugador(tJuego *juego, int posicion){
  tCasilla *casillaActual;

  juego->jugador.posicion=posicion;
  casillaActual = buscarCasilla(&juego->tablero,(juego->jugador).posicion,cmpPosCasillas);
  //DESPUES DE POSICIONAR CASILLA->JUGADOR=1;
  casillaActual->jugador = 1;
  casillaActual->normal = 0;

  recorrerListaYAccionar(&juego->bandidos, juego, comprobarColisionesJB);

  if(!juego->juegoActivo){
    return 0;
  }

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
void mostrarRanking(const void *reg, void* param)
{
    tRegistroRanking *ranking=(tRegistroRanking*)reg;
    printf("%s\tCodigo: %d |  Puntos: %d  | Cantidad de Partidas: %d\n",ranking->nombre,ranking->idJugador,ranking->puntuacionTotal,ranking->cantidadPartidas);
}
