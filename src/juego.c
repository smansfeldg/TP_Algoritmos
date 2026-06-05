#define _CRT_SECURE_NO_WARNINGS

#include "../include/juego.h"
#include "../include/archivos.h"
#include <ctype.h>
#include <stdio.h>

static void leerLinea(char *destino, int tam)
{
  // Lee una linea segura desde stdin y elimina el salto final si existe.
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

static const char *descripcionCasilla(const tCasilla *casilla)
{
  if (!casilla) return "Normal";
  if (casilla->inicio) return "Inicio";
  if (casilla->refugio) return "Refugio";
  if (casilla->oasis) return "Oasis";
  if (casilla->tormenta) return "Tormenta";
  if (casilla->vidas) return "Vida extra";
  if (casilla->premios) return "Premio";
  return "Normal";
}

static int calcularDestinoJugador(const tJuego *juego, int pasos, char direccion)
{
  int destino = juego->jugador.posicion + (direccion == 'B' ? -pasos : pasos);

  while (destino > juego->config.totalCasillas || destino < 1) {
    if (destino > juego->config.totalCasillas) {
      destino = juego->config.totalCasillas - (destino - juego->config.totalCasillas);
    }
    if (destino < 1) {
      destino = 1 + (1 - destino);
    }
  }

  return destino;
}

static void mostrarPanelTurno(const tJuego *juego)
{
  tCasilla *casillaActual;

  if (!juego) return;

  // Se consulta la casilla actual para mostrar el contexto del turno.
  casillaActual = buscarCasilla(&juego->tablero, juego->jugador.posicion, cmpPosCasillas);

  printf("\n========================================\n");
  printf(" Turno %d | %s\n", juego->turnoActual, juego->jugador.nombre);
  printf("========================================\n");
  printf("Posicion: %d/%d", juego->jugador.posicion, juego->config.totalCasillas);
  if (casillaActual) {
    printf(" (%s)", descripcionCasilla(casillaActual));
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

static void agregarTextoCasilla(char *destino, size_t tam, const char *texto)
{
  size_t usados = strlen(destino);

  if (usados < tam) {
    snprintf(destino + usados, tam - usados, "%s", texto);
  }
}

static void agregarCantidadCasilla(char *destino, size_t tam, unsigned cantidad, char simbolo)
{
  size_t usados = strlen(destino);

  if (usados >= tam) {
    return;
  }

  if (cantidad > 1) {
    snprintf(destino + usados, tam - usados, "%u%c ", cantidad, simbolo);
  } else {
    snprintf(destino + usados, tam - usados, "%c ", simbolo);
  }
}

void mostrarBienvenida()
{
  puts("\n\t\t\tCaravana del Desierto");
  puts("Una caravana intenta atravesar una antigua ruta comercial para llegar a una ciudad refugio antes de que se agoten sus recursos.\n");
  puts("A lo largo del trayecto existen tesoros, provisiones, oasis, tormentas de arena y bandidos.");
  puts("Cada decision cuenta. La caravana debe avanzar. El desierto no perdona errores.\n");
}

void mostrarReglas()
{
  puts("\n\t\t\tReglas del juego\n");
  puts("El jugador (J) debe llegar desde el Campamento Inicial (I) hasta la Ciudad Refugio (S).");
  puts("En cada turno tira un dado de 1 a 6 y elige avanzar (F) o retroceder (B) exactamente esa cantidad.");
  puts("Si sobrepasa la Ciudad Refugio, rebota con los pasos sobrantes. Los bandidos se mueven en una ruta circular e intentan interceptarlo.\n");
}

int iniciarPartida(tJuego *juego, FILE *archJug, ArbolBin *indice)
{
  char usuario[MAX_NOMBRE];
  char nombre[MAX_NOMBRE];
  char respuesta[16];
  int existe;
  tJugador auxJug;

  do {
    printf("\nIngrese su usuario: ");
    leerLinea(usuario, MAX_NOMBRE);
    // Si el usuario no ingresa un nombre, se asigna uno por defecto.
    if (strlen(usuario) == 0) {
      strcpy(usuario, "Jugador1");
    }

    // Se busca si ya existe un jugador con ese nombre en el indice. -1 Es que no existe
    existe = buscarJugador(usuario, indice, cmpIndxApodo);
    if (existe != -1) {
      // Si ya existe, se confirma si quiere retomar ese usuario.
      printf("\nEse usuario ya existe. Es tu usuario? (S/N): ");
      leerLinea(respuesta, sizeof(respuesta));
      respuesta[0] = (char)toupper((unsigned char)respuesta[0]);
      if (respuesta[0] != 'S') {
        puts("Ese Apodo esta en uso, por favor elija otro.");
      }
    }
    // Si no existe, pregunta por el nombre del dueño del nuevo usuario
    else
    {
        do
        {
            printf("\nUsuario: %s - Disponible.", usuario);
            printf("\nPor favor, ingrese su nombre para saber a quien le pertenece: ");
            leerLinea(nombre, MAX_NOMBRE);
        }while(strlen(nombre) == 0);
    }
  } while (existe !=-1 && respuesta[0] != 'S');

  crearJugador(&juego->jugador, nombre, usuario, 1, juego->config.vidasIniciales);

  // Si el jugador ya existe, se carga el Nombre del individuo.
  if (existe!=-1) {
        leerPos(archJug,&auxJug,sizeof(tRegistroJugador),existe);
        strncpy(juego->jugador.nombre,auxJug.nombre,MAX_NOMBRE-1);
        juego->jugador.nombre[MAX_NOMBRE]='\0';

  } else {
    // Si es un usuario nuevo, se guarda en archivos.
    actualizarJugadores(archJug, indice, &juego->jugador);
  }

  return 1;
}

int lanzarDado()
{
  return (rand() % 6) + 1;
}

void imprimirCasillaJuego(const void *info, void *param)
{
  const tCasilla *casilla = (const tCasilla *)info;
  char casillaImprimir[80];
  char *aux;
  int *pos = (int *)param;

  snprintf(casillaImprimir, sizeof(casillaImprimir), "%02d:[", *pos);
  (*pos)++;

  if (casilla->normal) {
    agregarTextoCasilla(casillaImprimir, sizeof(casillaImprimir), ".");
  } else {
    if (casilla->inicio) agregarTextoCasilla(casillaImprimir, sizeof(casillaImprimir), "I ");
    if (casilla->refugio) agregarTextoCasilla(casillaImprimir, sizeof(casillaImprimir), "S ");
    if (casilla->oasis) agregarTextoCasilla(casillaImprimir, sizeof(casillaImprimir), "O ");
    if (casilla->tormenta) agregarTextoCasilla(casillaImprimir, sizeof(casillaImprimir), "T ");
    if (casilla->vidas) {
      agregarCantidadCasilla(casillaImprimir, sizeof(casillaImprimir), casilla->vidas, 'V');
    }
    if (casilla->premios) {
      agregarCantidadCasilla(casillaImprimir, sizeof(casillaImprimir), casilla->premios, 'P');
    }
    if (casilla->bandidos) {
      agregarCantidadCasilla(casillaImprimir, sizeof(casillaImprimir), casilla->bandidos, 'B');
    }
    if (casilla->jugador) agregarTextoCasilla(casillaImprimir, sizeof(casillaImprimir), "J ");
  }

  aux = strrchr(casillaImprimir, ' ');
  if (aux) {
    *aux = '\0';
  }

  agregarTextoCasilla(casillaImprimir, sizeof(casillaImprimir), "]");
  puts(casillaImprimir);
}

void mostrarTablero(const tLista *tablero)
{
  int posIni = 1;
  if (!tablero) return;
  printf("--- Mapeo del Desierto ---\n\n");
  mostrarDeIzqADer(tablero, &posIni, imprimirCasillaJuego);
  printf("\nJ: jugador | B: bandido | I: inicio | S: refugio | P: premio | V: vida | O: oasis | T: tormenta\n");
  printf("--------------------------\n\n");
}

int procesarMovimientoJugador(tJuego *juego)
{
  tMovimiento nuevoMovimiento;
  char linea[16];
  int puedeRetroceder;

  // El turno del jugador arranca esperando que confirme el lanzamiento.
  puts("\nPresione Enter para lanzar el dado.");
  leerLinea(linea, sizeof(linea));

  // El dado define cuantos pasos debera moverse el jugador.
  nuevoMovimiento.pasos = lanzarDado();
  printf("Dado: %d\n", nuevoMovimiento.pasos);
  nuevoMovimiento.entidad = &(juego->jugador);
  puedeRetroceder = (juego->jugador.posicion - nuevoMovimiento.pasos) > 0;

  do {
    // Se muestran las dos direcciones posibles con el destino calculado.
    printf("\nMovimiento disponible:\n");
    printf("  F - Avanzar hasta la posicion %d\n",
           calcularDestinoJugador(juego, nuevoMovimiento.pasos, 'F'));
    if (puedeRetroceder) {
      printf("  B - Retroceder hasta la posicion %d\n",
             calcularDestinoJugador(juego, nuevoMovimiento.pasos, 'B'));
    }
    printf("Elija direccion: ");

    leerLinea(linea, sizeof(linea));
    nuevoMovimiento.direccion = (char)toupper((unsigned char)linea[0]);

    // Solo se aceptan las opciones validas de avance o retroceso.
    if (nuevoMovimiento.direccion != 'F' && !(puedeRetroceder && nuevoMovimiento.direccion == 'B')) {
      printf("Opcion invalida. Use F");
      if (puedeRetroceder) {
        printf(" o B");
      }
      printf(".\n");
    }
  } while (nuevoMovimiento.direccion != 'F' && !(puedeRetroceder && nuevoMovimiento.direccion == 'B'));

  // El movimiento del jugador queda en cola para ejecutarse en el turno.
  encolarMovimiento(&juego->colaMovimientos, nuevoMovimiento);
  encolarMovimiento(&juego->colaMovimientosJugador, nuevoMovimiento);
  return 1;
}

void accionarBandido(void *bandido, void *contexto, void *extra)
{
  tJuego *juego = (tJuego *)contexto;
  tMovimiento nueMov;
  tBandido *bandidoActual = (tBandido *)bandido;
  int posJugador = juego->jugador.posicion;
  int haciaAdelante;
  int haciaAtras;

  if (!bandidoActual->activo) return;

  // Cada bandido tira un dado en su turno para definir cuantas casillas se mueve.
  nueMov.pasos = lanzarDado();
  nueMov.entidad = bandidoActual;

  /*
    La direccion se calcula comparando la distancia
    hacia adelante y hacia atras hasta la posicion actual del jugador.
    Como el tablero es circular, se toma el camino mas corto para intentar
    interceptarlo.
   */
  haciaAdelante = (posJugador - bandidoActual->posicion + juego->config.totalCasillas) % juego->config.totalCasillas;
  haciaAtras = (bandidoActual->posicion - posJugador + juego->config.totalCasillas) % juego->config.totalCasillas;
  nueMov.direccion = (haciaAdelante <= haciaAtras) ? 'F' : 'B';

  // El bandido no se mueve de inmediato, su movimiento queda en cola para ejecutarse en orden.
  encolarMovimiento(&juego->colaMovimientos, nueMov);
}

int procesarMovimientoBandidos(tJuego *juego)
{
  recorrerListaYAccionar(&juego->bandidos, juego, NULL, accionarBandido);
  return 1;
}

int jugarTurnoComputadora(tJuego *juego)
{
  tMovimiento nuevoMovimiento;
  tCasilla *casillaActualBandido;
  tBandido *bandido;

  while (!ColaVacia(&juego->colaMovimientos)) {
    desencolarMovimiento(&juego->colaMovimientos, &nuevoMovimiento);
    bandido = (tBandido *)nuevoMovimiento.entidad;

    if (bandido->activo) {
      casillaActualBandido = buscarCasilla(&juego->tablero, bandido->posicion, cmpPosCasillas);
      printf("[Movimiento de bandido %d: %d%c]\n", bandido->id, nuevoMovimiento.pasos, nuevoMovimiento.direccion);
      if (moverBandido(juego, bandido, nuevoMovimiento, casillaActualBandido) == -1) {
        return 0;
      }
    }
  }

  return 1;
}

int ejecutarTurno(tJuego *juego)
{
  tMovimiento nuevoMovimiento;
  tCasilla *casillaActualJugador;
  int protegidoAlInicio = juego->jugador.protegidoOasis;

  // Se muestra el estado actual antes de que el jugador tome una decision.
  mostrarPanelTurno(juego);

  if (!juego->jugador.perdidoTurno) {
    // Si no esta penalizado por una tormenta, se procesa su movimiento normal.
    procesarMovimientoJugador(juego);
  } else {
    // La tormenta hace saltar el turno de movimiento del jugador.
    puts("\nLa tormenta obliga al jugador a perder este turno.");
  }

  // Luego se preparan los movimientos de los bandidos para este mismo turno.
  procesarMovimientoBandidos(juego);

  if (!juego->jugador.perdidoTurno) {
    // Se ejecuta el movimiento elegido por el jugador.
    desencolarMovimiento(&juego->colaMovimientos, &nuevoMovimiento);
    casillaActualJugador = buscarCasilla(&juego->tablero, juego->jugador.posicion, cmpPosCasillas);
    if (!moverJugador(juego, nuevoMovimiento, casillaActualJugador)) {
      juego->juegoActivo = 0;
      return 0;
    }
    juego->totalMovimientos++;
  } else {
    // La penalizacion solo dura un turno.
    juego->jugador.perdidoTurno = 0;
  }

  // Una vez movido, se identifica la casilla para aplicar efectos del terreno.
  casillaActualJugador = buscarCasilla(&juego->tablero, juego->jugador.posicion, cmpPosCasillas);
  printf("\nLlegaste a la posicion %d: %s\n",
         juego->jugador.posicion,
         descripcionCasilla(casillaActualJugador));

  // Las casillas especiales pueden terminar la partida o modificar el estado del jugador.
  if (aplicarEfectoCasilla(&juego->jugador, casillaActualJugador)) {
    juego->juegoActivo = 0;
    return 2;
  }

  // Si los efectos dejaron al jugador sin vidas, la partida termina.
  if (verificarDerrota(&juego->jugador)) {
    juego->juegoActivo = 0;
    return 0;
  }

  // Ahora se ejecutan los movimientos de los bandidos contra el tablero actualizado.
  if (!jugarTurnoComputadora(juego)) {
    juego->juegoActivo = 0;
    return 0;
  }

  // Se vuelve a validar derrota porque un bandido puede quitar la ultima vida.
  if (verificarDerrota(&juego->jugador)) {
    juego->juegoActivo = 0;
    return 0;
  }

  // La proteccion del oasis dura hasta que termine este turno o hasta que se use.
  if (protegidoAlInicio && casillaActualJugador && !casillaActualJugador->oasis) {
    juego->jugador.protegidoOasis = 0;
  }

  juego->turnoActual++;
  printf("\n*****************************\n");
  return 1;
}

void mostrarFinJuego(int estado, const tJuego *j)
{
  if (estado == 2) {
    // Victoria: el jugador llego a la Ciudad Refugio.
    printf("\nFelicidades %s, llegaste a Ciudad Refugio con %d puntos.\n",
           j->jugador.nombre, j->jugador.puntos);
  } else if (estado == 0) {
    // Derrota: el jugador se quedo sin vidas antes de llegar al final.
    printf("\nGAME OVER\n%s fue derrotado antes de llegar a Ciudad Refugio.\n",
           j->jugador.nombre);
  }
}
