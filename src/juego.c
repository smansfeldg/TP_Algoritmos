#include "../include/juego.h"
#include <time.h>
#include <ctype.h>
#include <stdio.h>

//HACER
void mostrarBienvenida()
{
  //IMPRIME EL MENSAJE BIENVENIDA
}

//COMPLETAR
int iniciarPartida(tJuego *juego){
  char nombre[MAX_NOMBRE];

  //REVISAR HACERLO MÁS SIMPLE Y PRÁCTICO
  printf("\nIngrese su nombre de jugador: ");
  while (getchar() != '\n');
  fgets(nombre, MAX_NOMBRE, stdin);
  nombre[strcspn(nombre, "\n")] = 0;

  //NO VA
  if (strlen(nombre) == 0) {
      strcpy(nombre, "Jugador1");
  }
  //DEBE BUSCAR EN EL ARCHIVO DE USUARIOS POR EL INDICE

  crearJugador(&juego->jugador,nombre,1,juego->config.vidasIniciales);

  return 1;
}

//LISTA
int lanzarDado(){
    return (rand() % 6) + 1; // analizar posibilidad de usar una función más robusta
}

// Helper para comprobar si hay bandido en la posicion
void checkBandidoPos(void *info, void *contexto)
{
    tBandido *b = (tBandido *)info;
    struct { int pos; int *encontrado; } *ctx = contexto;
    
    if (b->activo && b->posicion == ctx->pos)
    {
        *(ctx->encontrado) = 1;
    }
}

// Callback para imprimir una casilla puntual del tablero con posiciones
void imprimirCasillaJuego(void *info, void *contexto)
{
    tCasilla *casilla = (tCasilla *)info;
    const tJuego *juego = (const tJuego *)contexto;

    if (juego->jugador.posicion == casilla->posicion)
    {
        printf("[J]");
    }
    else
    {
        int bandidoEnemigo = 0;
        struct { int pos; int *encontrado; } ctxBandido = { casilla->posicion, &bandidoEnemigo };
        recorrerListaYAccionar(&juego->bandidos, &ctxBandido, checkBandidoPos);

        if (bandidoEnemigo)
        {
            printf("[B]");
        }
        else
        {
            printf("[%c]", casilla->tipo);
        }
    }
}

//EN DEBATE - HACER
void mostrarTableroConPosiciones(const tJuego *juego)
{
    if (!juego) return;
    
    printf("\n--- Mapeo del Desierto ---\n");
    recorrerListaYAccionar(&juego->tablero, (void *)juego, imprimirCasillaJuego);
    printf("\n--------------------------\n");
}

//LISTA/VERIFICAR
int procesarMovimientoJugador(tJuego *juego){
  tMovimiento nuevoMovimiento;

  puts("Presione cualquier tecla para lanzar el dado. . .");
  fflush(stdin);
  getc(stdin);

  nuevoMovimiento.pasos=lanzarDado();
  nuevoMovimiento.entidad= &(juego->jugador);
  //LE PIDE AL JUGADOR QUE INGRESE UNO DE LOS CARACTERES DE MOVIMIENTO
  //HASTA QUE NO INGRESE UNO VALIDO LE SIGUE PIDIENDO
  //B - RETROCEDER
  //F - AVANZAR
  do{
    printf("Ingrese como quiere avanzar:");
    if((juego->jugador.posicion - nuevoMovimiento.pasos) >= 0){
      puts("  B - Retroceder");
    }
    puts("  F - Avanzar:");

    fflush(stdin);
    nuevoMovimiento.direccion = getc(stdin);
    if(nuevoMovimiento.direccion == 'f') nuevoMovimiento.direccion = 'F';
    if(nuevoMovimiento.direccion == 'b') nuevoMovimiento.direccion = 'B';
  }while( !(((nuevoMovimiento.direccion == 'B') && ((juego->jugador.posicion - nuevoMovimiento.pasos) >= 0)) 
         || (nuevoMovimiento.direccion == 'F')) );

  encolarMovimiento(&juego->colaMovimientos,nuevoMovimiento);
  encolarMovimiento(&juego->colaMovimientosJugador,nuevoMovimiento);

  return 1;
}

//LISTA/VERIFICAR
int procesarMovimientoBandidos(tJuego *juego){
  tLista* listaBandidos; //SI SE UTILIZA UNA FUNCION RECORRER LISTA ESTO NO VA
  tBandido* bandidoActual; //SI SE UTILIZA UNA FUNCION RECORRER LISTA ESTO NO VA
  listaBandidos = &juego->bandidos; //SI SE UTILIZA UNA FUNCION RECORRER LISTA ESTO NO VA
  tMovimiento nueMov;
  int posJugador;
  posJugador = juego->jugador.posicion;

//REEMPLAZAR POR UNA FUNCION DE RECORRER LISTA
//  while(obtenerYAvanzarLista(listaBandidos, bandidoActual)){
    nueMov.pasos=lanzarDado();
    nueMov.entidad=bandidoActual;
    //HACER FUNCION
    //SI POSJUGADOR < POSBANDIDO
    if(posJugador < bandidoActual->posicion) {
      // SI (TOTAL CASILLAS + POSJUGADOR) - POSBANDIDO < POSBANDIDO - POSJUGADOR
      //  AVANZAR
      // SINO
      //  RETROCEDER
      if( ((juego->config.totalCasillas+posJugador)-bandidoActual->posicion) < (bandidoActual->posicion-posJugador) ){
        nueMov.direccion = 'F';
      }
      else{
        nueMov.direccion = 'B';
      }
    }
    else{
      //SINO
      // SI (TOTAL CASILLAS + POSBANDIDO) - POSJUGADOR < POSJUGADOR - POSBANDIDO
      //  RETROCEDER
      // SINO
      //  AVANZAR
      if( ((juego->config.totalCasillas+bandidoActual->posicion)-posJugador) < (posJugador-bandidoActual->posicion) ){
        nueMov.direccion = 'B';
      }
      else{
        nueMov.direccion = 'F';
      }
    }

    encolarMovimiento(&juego->colaMovimientos,nueMov);
//  }

  return 1;
}

//LISTA/VERIFICAR
void jugarTurnoComputadora(tJuego *juego){
  tMovimiento nuevoMovimiento;
  tBandido* bandido;

  while(!ColaVacia(&juego->colaMovimientos)){ //(Y SI JUEGO ACTIVO) <---------------------------------------------
    desencolarMovimiento(&juego->colaMovimientos, &nuevoMovimiento);
    bandido = (tBandido*)nuevoMovimiento.entidad;
    if(bandido->activo){
      moverBandido(juego, bandido, nuevoMovimiento);
    }
  }
}

//COMPLETAR/DISCUTIR
int ejecutarTurno(tJuego *juego){
    int estadoJuego=1;//0-DERROTA 1-JUGANDO 2-VICTORIA
    tMovimiento nuevoMovimiento;

    //HACER FUNCION
    if(juego->jugador.protegidoOasis){
      juego->jugador.protegidoOasis = 0;
    }
    if(juego->jugador.perdidoTurno){
      juego->jugador.perdidoTurno = 0;
    }

    //FASE DE TIRAR DADOS
    //JUGADOR
    procesarMovimientoJugador(juego);
    //BANDIDOS
    procesarMovimientoBandidos(juego);

    //FASE DE MOVER
    //JUGADOR
    desencolarMovimiento(&(juego->colaMovimientos), &nuevoMovimiento);
    moverJugador(juego, nuevoMovimiento);
    //AGREGAR: SUMAR UNO A LOS MOVIMIENTOS TOTALES DE LA PARTIDA
    mostrarEstadoJugador(&(juego->jugador));
    aplicarEfectoCasilla(&(juego->jugador), buscarCasilla(&juego->tablero,(juego->jugador).posicion)->tipo);
    //BANDIDOS
    jugarTurnoComputadora(juego);

    //VERIFICAR CONDICION DE VICTORIA <------------------------------------------------------------ estadoJuego = verificarDerrota(&juego->jugador);
    //VERIFICAR CONDICION DE DERROTA <------------------------------------------------------------- estadoJuego = verificarVictoria(&juego->jugador, &juego->tablero);
    //EL ESTADO DEL JUEGO DEBERIA CAMBIAR EN EL MOVIMIENTO DEL JUGADOR Y DE LOS BANDIDOS
    //PORQUE NO TIENE SENTIDO SEGUIR PROCESANDO SI YA FINALIZÓ EL JUEGO ADEMÁS DE OTRAS CUESTIONES

    juego->turnoActual++;

    return estadoJuego;
}

//HACER
void mostrarFinJuego(int victoria, const tJugador *j)
{
  //IMPRIME POR PANTALLA EL MENSAJE FINAL SEGUN EL RESULTADO DE LA PARTIDA
}

//HACER UNA FUNCION PARA PROCESAR LAS PARTIDAS DE JUGADORES EN UNA LISTA ORDENADA POR PUNTOS TOTALES
//HACER UNA FUNCION PARA MOSTRAR LA LISTA DE PUNTAJES
