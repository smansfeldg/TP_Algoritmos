#define _CRT_SECURE_NO_WARNINGS

#include "../include/entidades.h"
#include <time.h>

static int claveIgual(const char *clave, const char *esperada)
{
    return strcmp(clave, esperada) == 0;
}

static int posicionOcupadaPorBandido(const tLista *bandidos, int posicion)
{
    tNodoListaC *act;

    if (!bandidos || !*bandidos) return 0;

    act = *bandidos;
    do {
        tBandido *b = (tBandido *)act->info;
        if (b->activo && b->posicion == posicion) return 1;
        act = act->sig;
    } while (act != *bandidos);

    return 0;
}

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

void inicializarConfiguracion(tConfiguracion *cfg)
{
    cfg->totalCasillas = 30;
    cfg->vidasIniciales = 3;
    cfg->cantidadBandidos = 3;
    cfg->cantidadPremios = 4;
    cfg->cantidadVidas = 2;
    cfg->cantidadOasis = 2;
    cfg->cantidadTormentas = 3;
}

int cargarConfiguracion(const char *archivo, tConfiguracion *cfg)
{
    FILE *f = fopen(archivo, "rt");
    if (!f) return 0;

    char clave[50];
    int valor;
    inicializarConfiguracion(cfg);

    while (fscanf(f, "%s %d", clave, &valor) == 2)
    {
        char *dosPuntos = strchr(clave, ':');
        if (dosPuntos) *dosPuntos = '\0';

        if (claveIgual(clave, "TOTAL_CASILLAS") || claveIgual(clave, "cantidad_posiciones")) cfg->totalCasillas = valor;
        else if (claveIgual(clave, "VIDAS_INICIALES") || claveIgual(clave, "vidas_inicio")) cfg->vidasIniciales = valor;
        else if (claveIgual(clave, "CANTIDAD_BANDIDOS") || claveIgual(clave, "maximo_bandidos")) cfg->cantidadBandidos = valor;
        else if (claveIgual(clave, "CANTIDAD_PREMIOS") || claveIgual(clave, "maximo_premios")) cfg->cantidadPremios = valor;
        else if (claveIgual(clave, "CANTIDAD_VIDAS") || claveIgual(clave, "maximo_vidas_extra")) cfg->cantidadVidas = valor;
        else if (claveIgual(clave, "CANTIDAD_OASIS") || claveIgual(clave, "maximo_oasis")) cfg->cantidadOasis = valor;
        else if (claveIgual(clave, "CANTIDAD_TORMENTAS") || claveIgual(clave, "maximo_tormentas")) cfg->cantidadTormentas = valor;
    }

    fclose(f);
    return cfg->totalCasillas >= 3 && cfg->vidasIniciales > 0;
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

// Busca una casilla por posicion dentro de la lista circular del tablero.
//DEBERIA HABER UNA FUNCIÓN PARA BUSCAR EN LA IMPLEMENTACION DE LISTA CIRCULAR
//ESTA FUNCION DEBERIA RECIBIR UNA FUNCION DE COMPARACION DE PARA POSICION DE CASILLAS
tCasilla* buscarCasilla(const tLista *tablero, int posicion)
{
    // Implementar esta funcion dentro de las primitivas de lista circular, con una funcion de comparacion, pero por ahora esto para probar
    if(!*tablero) return NULL;

    tNodoListaC *act = *tablero;
    do {
        tCasilla *c = (tCasilla *)act->info;
        if(c->posicion == posicion)
            return c;
        act = act->sig;
    } while(act != *tablero);

    return NULL;
}

//LISTA
int moverJugador(tJuego* juego, tMovimiento movimiento){
    int nuevaPos, sentido = 1;

    if(movimiento.direccion == 'F'){
      sentido = 1;
    }
    else if(movimiento.direccion == 'B'){
      sentido = -1;
    }

    nuevaPos = juego->jugador.posicion + movimiento.pasos*sentido;

    /*
     * El jugador no recorre la ruta como circulo: si sobrepasa el refugio,
     * rebota con los pasos sobrantes para cumplir exactamente el valor del dado.
     */
    while(nuevaPos >= juego->config.totalCasillas || nuevaPos < 0){
      if(nuevaPos >= juego->config.totalCasillas){
        nuevaPos = (juego->config.totalCasillas - 1) - (nuevaPos - (juego->config.totalCasillas - 1));
      }
      if(nuevaPos < 0){
        nuevaPos *= -1;
      }
    }

    posicionarJugador(juego, nuevaPos);

    return nuevaPos;
}

// Aplica solo el efecto de la casilla destino, no las intermedias.
void aplicarEfectoCasilla(tJugador *j, char tipoCasilla)
{
    switch (tipoCasilla)
    {
    case TIPO_PREMIO:
        j->puntos += 1;
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

// Inicializa tablero, colas y bandidos de una nueva partida.
void inicializarJuego(tJuego *juego, tConfiguracion *cfg){
  int i;
  int maxBandidos;
  tBandido bandido;

  juego->config = *cfg;
  crearLista(&juego->tablero); // Asegurar que la lista se inicializa a NULL, de forma provisoria qeuda así
  CrearCola(&juego->colaMovimientos);
  CrearCola(&juego->colaMovimientosJugador);
  crearLista(&juego->bandidos);
  juego->totalMovimientos=0;
  juego->juegoActivo=1;
  juego->turnoActual=1;

  generarTablero(&juego->tablero,cfg);

  /*
   * Los bandidos viven en una lista separada para que puedan compartir casilla
   * con premios, oasis u otros eventos sin modificar la ruta base.
   */
  maxBandidos = cfg->cantidadBandidos;
  if (maxBandidos > cfg->totalCasillas - 2) {
    maxBandidos = cfg->totalCasillas - 2;
  }

  for(i = 0; i < maxBandidos; i++){
    int posicion;
    do {
      posicion = (rand() % (cfg->totalCasillas - 2)) + 1;
    } while (posicionOcupadaPorBandido(&juego->bandidos, posicion));

    crearBandido(&bandido, i + 1, posicion);
    insertarAlFinal(&juego->bandidos, &bandido, sizeof(tBandido));
  }
}

// Libera las estructuras dinamicas creadas durante la partida.
void liberarJuego(tJuego *juego)
{
  vaciarLista(&juego->tablero);
  vaciarLista(&juego->bandidos);
  VaciarCola(&juego->colaMovimientos);
  VaciarCola(&juego->colaMovimientosJugador);
}

// Encola un movimiento de jugador o bandido para resolverlo en orden.
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
    int nuevaPos, sentido = 1;

    if(movimiento.direccion == 'F'){
      sentido = 1;
    }
    else if(movimiento.direccion == 'B'){
      sentido = -1;
    }

    nuevaPos = b->posicion + movimiento.pasos*sentido;

    /*
     * Los bandidos si usan la ruta como circulo, tal como pide el enunciado.
     * Se normaliza con modulo para soportar avances y retrocesos.
     */
    while(nuevaPos < 0){
      nuevaPos += juego->config.totalCasillas;
    }
    while(nuevaPos >= juego->config.totalCasillas){
      nuevaPos -= juego->config.totalCasillas;
    }

    b->posicion = nuevaPos;

    verificarColision(juego, b);
    return 1;
}

//LISTO
int verificarColision(tJuego *juego, tBandido *b){
    int colision;

    if( (colision = ((juego->jugador.posicion == b->posicion)) && b->activo)){
      if(!juego->jugador.protegidoOasis){
        b->activo = 0;
        juego->jugador.vidas -= 1;
        posicionarJugador(juego, 0);
      } else {
        puts("El oasis protege al jugador del bandido.");
      }
    }

    return colision;
}

// La victoria se produce cuando el jugador queda sobre la Ciudad Refugio.
int verificarVictoria(const tJugador *j, const tLista *tablero)
{
    tCasilla *casilla = buscarCasilla(tablero, j->posicion);
    return casilla && casilla->tipo == TIPO_REFUGIO;
}

//LISTO
int verificarDerrota(const tJugador *j)
{
    return (j->vidas <= 0);
}

// Guarda una foto simple del escenario generado para cumplir con caravana.txt.
void guardarCaravana(const char *archivo, const tJuego *juego)
{
    FILE *f = fopen(archivo, "wt");
    tNodoListaC *act;

    if (!f || !juego || !juego->tablero) {
        if (f) fclose(f);
        return;
    }

    fprintf(f, "CARAVANA DEL DESIERTO\n");
    fprintf(f, "Posiciones: %d | Vidas: %d | Bandidos: %d\n\n",
            juego->config.totalCasillas,
            juego->config.vidasIniciales,
            juego->config.cantidadBandidos);

    act = juego->tablero;
    do {
        tCasilla *casilla = (tCasilla *)act->info;
        int hayJugador = juego->jugador.posicion == casilla->posicion;
        int hayBandido = posicionOcupadaPorBandido(&juego->bandidos, casilla->posicion);

        fprintf(f, "%02d:", casilla->posicion + 1);
        if (hayJugador || hayBandido) {
            fprintf(f, "[");
            if (casilla->tipo != TIPO_NORMAL) fprintf(f, "%c", casilla->tipo);
            if (hayJugador) fprintf(f, "J");
            if (hayBandido) fprintf(f, "B");
            fprintf(f, "]");
        } else {
            fprintf(f, "[%c]", casilla->tipo);
        }
        fprintf(f, " %s\n", casilla->descripcion);
        act = act->sig;
    } while (act != juego->tablero);

    fprintf(f, "\nReferencias: J jugador, B bandido, I inicio, S refugio, P premio, V vida, O oasis, T tormenta, . ruta.\n");
    fclose(f);
}

// La carga de caravana queda reservada para una futura opcion de continuar partida.
int cargarCaravana(const char *archivo, tJuego *juego)
{
    (void)archivo;
    (void)juego;
    return 1;
}

//LISTO
int posicionarJugador(tJuego *juego, int posicion){
  tNodoListaC *bandidoActual, *bandidoIni;

  juego->jugador.posicion=posicion;

  if(!juego->bandidos) return 1;

  //GUARDAR EL PRIMER BANDIDO
  bandidoIni = juego->bandidos;
  bandidoActual = bandidoIni;
  do{//RECORRER LISTA BANDIDOS
    if(verificarColision(juego, bandidoActual->info)){
      break;
    }
    bandidoActual = bandidoActual->sig;
  }while(bandidoActual != bandidoIni);

  return 1;
}

//ADEMAS DEBERIA HABER UN PAR DE FUNCIONES PARA GUARDAR Y CARGAR JUGADORES, OTRO PAR PARA LAS PARTIDAS Y OTRO PAR PARA EL INDICE
