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

//HACER
int cargarConfiguracion(const char *archivo, tConfiguracion *cfg)
{
    //ABRE EL ARCHIVO DE CONFIGURACION
    //MIENTRAS NO SEA EL FIN DEL ARCHIVO
    // LEE UN REGISTRO
    // ASGINA CADA REGISTRO LEIDO A UN CAMPO DE LA CONFIGURACION
    // PUEDE ASIGNAR POR ORDEN U OBTENER EL CAMPO Y CARGARLO EN LA CONFIGURACION CORRESPONDIENTE
    //FIN MIENTRAS NO FIN ARCHIVO
    return 1;
}

//LISTO
void mostrarConfiguracion(const tConfiguracion *cfg){
  printf("Los parametros de configuracion son:\nCantidad de Casillas del Tablero:\t%d\nCantidad de Oasis:\t%d\nCantidad de Premios:\t%d\nCantidad de Tormentas:\t%d\nCantidad de Vidas Extra:\t%d\nCantidad de Vida Inicial:\t%d\nCantidad de Bandidos:\t%d\n",
         cfg->totalCasillas,cfg->cantidadOasis,cfg->cantidadPremios,cfg->cantidadTormentas,cfg->cantidadVidas,cfg->vidasIniciales,cfg->cantidadBandidos);
}

//HACER
int generarTablero(tLista *tablero, const tConfiguracion *cfg)
{
    //CREA CASILLA POR CASILLA EN BUCLE
    //COMPROBAR QUE LA SUMA DE CASILLAS ESPECIALES SEA MENOR AL TAMAÑO DEL TABLERO - 2 (POR EL INICIO Y EL FINAL)
    //UTILIZA UN ALGORITMO PARA ASIGNAR ALEATORIAMENTE LAS CASILLAS ESPECIALES Y QUE NO SE REPITAN
    //ASIGNA ALEATORIAMENTE A LOS BANDIDOS
    //UBICA AL JUGADOR EN LA PRIMER CASILLA
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
