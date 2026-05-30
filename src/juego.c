#define _CRT_SECURE_NO_WARNINGS

#include "../include/juego.h"
#include "../include/archivos.h"
#include <time.h>
#include <ctype.h>
#include <stdio.h>

static void leerLinea(char *destino, int tam)
{
  if (!destino || tam <= 0) return;

  if (fgets(destino, tam, stdin) == NULL) {
    destino[0] = '\0';
    return;
  }

  destino[strcspn(destino, "\n")] = '\0';
}

static int contarBandidosActivos(const tJuego *juego)
{
  int activos = 0;
  tNodoListaC *act;

  if (!juego || !juego->bandidos) return 0;

  act = juego->bandidos;
  do {
    tBandido *b = (tBandido *)act->info;
    if (b->activo) activos++;
    act = act->sig;
  } while (act != juego->bandidos);

  return activos;
}

static const char *descripcionCasilla(char tipo)
{
  switch (tipo) {
    case TIPO_INICIO: return "Inicio";
    case TIPO_REFUGIO: return "Refugio";
    case TIPO_PREMIO: return "Premio";
    case TIPO_VIDA: return "Vida extra";
    case TIPO_OASIS: return "Oasis";
    case TIPO_TORMENTA: return "Tormenta";
    default: return "Normal";
  }
}

static int calcularDestinoJugador(const tJuego *juego, int pasos, char direccion)
{
  int destino = juego->jugador.posicion + (direccion == 'B' ? -pasos : pasos);

  if (destino >= juego->config.totalCasillas) {
    while (destino >= juego->config.totalCasillas || destino < 0) {
      if (destino >= juego->config.totalCasillas) {
        destino = (juego->config.totalCasillas - 1) - (destino - (juego->config.totalCasillas - 1));
      }
      if (destino < 0) {
        destino *= -1;
      }
    }
  }
  if (destino < 0) destino = 0;

  return destino;
}

static void mostrarPanelTurno(const tJuego *juego)
{
  tCasilla *casillaActual;

  if (!juego) return;

  casillaActual = buscarCasilla(&juego->tablero, juego->jugador.posicion);

  printf("\n========================================\n");
  printf(" Turno %d | %s\n", juego->turnoActual, juego->jugador.nombre);
  printf("========================================\n");
  printf("Posicion: %d/%d", juego->jugador.posicion + 1, juego->config.totalCasillas);
  if (casillaActual) {
    printf(" (%s)", descripcionCasilla(casillaActual->tipo));
  }
  printf("\nVidas: %d | Puntos: %d | Bandidos activos: %d\n",
         juego->jugador.vidas,
         juego->jugador.puntos,
         contarBandidosActivos(juego));

  if (juego->jugador.protegidoOasis) {
    printf("Estado: protegido por oasis\n");
  } else if (juego->jugador.perdidoTurno) {
    printf("Estado: pierde este turno por tormenta\n");
  } else {
    printf("Estado: listo para avanzar\n");
  }
}

// Muestra una bienvenida breve antes de empezar la partida.
void mostrarBienvenida()
{
  printf("========================================\n");
  printf("        CARAVANA DEL DESIERTO\n");
  printf("========================================\n");
  printf("Avanza por el tablero, esquiva bandidos y llega al refugio.\n");
  printf("Las casillas especiales pueden darte puntos, vidas o afectar tu turno.\n");
}

// Inicia la partida solicitando el jugador y enlazandolo con los registros.
int iniciarPartida(tJuego *juego){
  char nombre[MAX_NOMBRE];

  //REVISAR HACERLO MÁS SIMPLE Y PRÁCTICO
  printf("\nIngrese su nombre de jugador: ");
  leerLinea(nombre, MAX_NOMBRE);

  if (strlen(nombre) == 0) {
      strcpy(nombre, "Jugador1");
  }
  crearJugador(&juego->jugador,nombre,0,juego->config.vidasIniciales);
  /*
   * El alta/busqueda queda en archivos.c: la capa de juego solo pide el nombre
   * y recibe un id persistente para vincular las partidas.
   */
  obtenerORegistrarJugador("jugadores.dat", "indice_jugadores.dat", &juego->jugador);

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
        if (casilla->tipo == TIPO_NORMAL)
        {
            printf("[J]");
        }
        else
        {
            printf("[%cJ]", casilla->tipo);
        }
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

// Muestra el tablero con una convencion uniforme para casilleros compuestos.
void mostrarTableroConPosiciones(const tJuego *juego)
{
    if (!juego) return;
    
    printf("\n--- Mapa del desierto ---\n");
    recorrerListaYAccionar(&juego->tablero, (void *)juego, imprimirCasillaJuego);
    printf("\nJ: jugador | B: bandido | I: inicio | S: refugio | P: premio | V: vida | O: oasis | T: tormenta\n");
    printf("-------------------------\n");
}

//LISTA/VERIFICAR
int procesarMovimientoJugador(tJuego *juego){
  tMovimiento nuevoMovimiento;
  char linea[10];
  int puedeRetroceder;

  puts("\nPresione Enter para lanzar el dado.");
  leerLinea(linea, sizeof(linea));

  nuevoMovimiento.pasos=lanzarDado();
  printf("Dado: %d\n", nuevoMovimiento.pasos);
  nuevoMovimiento.entidad= &(juego->jugador);
  puedeRetroceder = (juego->jugador.posicion - nuevoMovimiento.pasos) >= 0;

  // Se valida la direccion con una linea completa para evitar saltos por Enter pendiente.
  do{
    printf("\nMovimiento disponible:\n");
    printf("  F - Avanzar hasta la posicion %d\n",
           calcularDestinoJugador(juego, nuevoMovimiento.pasos, 'F') + 1);
    if(puedeRetroceder){
      printf("  B - Retroceder hasta la posicion %d\n",
             calcularDestinoJugador(juego, nuevoMovimiento.pasos, 'B') + 1);
    }
    printf("Elija direccion: ");

    leerLinea(linea, sizeof(linea));
    nuevoMovimiento.direccion = toupper((unsigned char)linea[0]);

    if( !(((nuevoMovimiento.direccion == 'B') && puedeRetroceder) || (nuevoMovimiento.direccion == 'F')) ){
      printf("Opcion invalida. Use F");
      if (puedeRetroceder) printf(" o B");
      printf(".\n");
    }
  }while( !(((nuevoMovimiento.direccion == 'B') && puedeRetroceder) 
         || (nuevoMovimiento.direccion == 'F')) );

  encolarMovimiento(&juego->colaMovimientos,nuevoMovimiento);
  encolarMovimiento(&juego->colaMovimientosJugador,nuevoMovimiento);

  return 1;
}

void accionarBandido(void *info, void *contexto) {
  tBandido* bandidoActual = (tBandido*)info;
  tJuego* juego = (tJuego*)contexto;
  tMovimiento nueMov;
  
  if(!bandidoActual->activo) return;

  int posJugador = juego->jugador.posicion;
  nueMov.pasos = lanzarDado();
  nueMov.entidad = bandidoActual;

  if(posJugador < bandidoActual->posicion) {
    if( ((juego->config.totalCasillas + posJugador) - bandidoActual->posicion) < (bandidoActual->posicion - posJugador) ){
      nueMov.direccion = 'F';
    }
    else{
      nueMov.direccion = 'B';
    }
  }
  else{
    if( ((juego->config.totalCasillas + bandidoActual->posicion) - posJugador) < (posJugador - bandidoActual->posicion) ){
      nueMov.direccion = 'B';
    }
    else{
      nueMov.direccion = 'F';
    }
  }

  encolarMovimiento(&juego->colaMovimientos, nueMov);
}

//LISTA/VERIFICAR
int procesarMovimientoBandidos(tJuego *juego){
  recorrerListaYAccionar(&juego->bandidos, juego, accionarBandido);
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

// Ejecuta un turno completo: encola movimientos, resuelve efectos y verifica fin.
int ejecutarTurno(tJuego *juego){
    int estadoJuego=1;//0-DERROTA 1-JUGANDO 2-VICTORIA
    tMovimiento nuevoMovimiento;
    tCasilla *casillaActual;
    int protegidoAlInicio = juego->jugador.protegidoOasis;

    mostrarPanelTurno(juego);

    if(juego->jugador.perdidoTurno){
      printf("\nLa tormenta obliga al jugador a perder este turno.\n");
      juego->jugador.perdidoTurno = 0;
      procesarMovimientoBandidos(juego);
      jugarTurnoComputadora(juego);
      juego->turnoActual++;
      return verificarDerrota(&juego->jugador) ? 0 : 1;
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
    juego->totalMovimientos++;

    casillaActual = buscarCasilla(&juego->tablero,(juego->jugador).posicion);
    if (casillaActual) {
      printf("\nLlegaste a la posicion %d: %s\n",
             juego->jugador.posicion + 1,
             descripcionCasilla(casillaActual->tipo));
      aplicarEfectoCasilla(&(juego->jugador), casillaActual->tipo);
      mostrarEstadoJugador(&(juego->jugador));
    }

    if (verificarDerrota(&juego->jugador)) {
      return 0;
    }
    if (verificarVictoria(&juego->jugador, &juego->tablero)) {
      return 2;
    }

    //BANDIDOS
    jugarTurnoComputadora(juego);

    //VERIFICAR CONDICION DE VICTORIA <------------------------------------------------------------ estadoJuego = verificarDerrota(&juego->jugador);
    //VERIFICAR CONDICION DE DERROTA <------------------------------------------------------------- estadoJuego = verificarVictoria(&juego->jugador, &juego->tablero);
    //EL ESTADO DEL JUEGO DEBERIA CAMBIAR EN EL MOVIMIENTO DEL JUGADOR Y DE LOS BANDIDOS
    //PORQUE NO TIENE SENTIDO SEGUIR PROCESANDO SI YA FINALIZÓ EL JUEGO ADEMÁS DE OTRAS CUESTIONES

    if (verificarDerrota(&juego->jugador)) {
      estadoJuego = 0;
    }
    if (protegidoAlInicio && casillaActual && casillaActual->tipo != TIPO_OASIS) {
      juego->jugador.protegidoOasis = 0;
    }

    juego->turnoActual++;

    return estadoJuego;
}

// Imprime el resultado final con los datos principales del jugador.
void mostrarFinJuego(int victoria, const tJugador *j)
{
  printf("\n========================================\n");
  if (victoria) {
    printf("              VICTORIA\n");
    printf("Llegaste al refugio.\n");
  } else {
    printf("              DERROTA\n");
    printf("La caravana se quedo sin vidas.\n");
  }
  printf("========================================\n");
  if (j) {
    printf("Jugador: %s\n", j->nombre);
    printf("Vidas restantes: %d\n", j->vidas);
    printf("Puntos: %d\n", j->puntos);
  }
}

