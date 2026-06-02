#define _CRT_SECURE_NO_WARNINGS

#include "../include/juego.h"
#include "../include/archivos.h"
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
  char nombre[MAX_NOMBRE];
  char respuesta[16];
  int existe;
  tIndice buscado;
  NodoRaiz *nodo;

  do {
    printf("\nIngrese su nombre de jugador: ");
    leerLinea(nombre, MAX_NOMBRE);
    if (strlen(nombre) == 0) {
      strcpy(nombre, "Jugador1");
    }

    existe = buscarJugador(nombre, indice, cmpIndxNombre);
    if (existe) {
      printf("\nEse nombre ya existe. Es tu usuario? (S/N): ");
      leerLinea(respuesta, sizeof(respuesta));
      respuesta[0] = (char)toupper((unsigned char)respuesta[0]);
      if (respuesta[0] != 'S') {
        puts("Ese nombre esta en uso, por favor elija otro.");
      }
    }
  } while (existe && respuesta[0] != 'S');

  crearJugador(&juego->jugador, nombre, 1, juego->config.vidasIniciales);

  if (existe) {
    strcpy(buscado.nombre, nombre);
    nodo = buscarNodoArbolBin(indice, &buscado, cmpIndxNombre);
    if (nodo) {
      juego->jugador.idJugador = (int)((tIndice *)nodo->dato)->registro;
    }
  } else {
    fseek(archJug, 0, SEEK_END);
    juego->jugador.idJugador = (int)(ftell(archJug) / sizeof(tRegistroJugador)) + 1;
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
  char buff[80];
  char *aux;
  int *pos = (int *)param;

  sprintf(casillaImprimir, "%02d:[", *pos);
  (*pos)++;

  if (casilla->normal) {
    strcat(casillaImprimir, ".");
  } else {
    if (casilla->inicio) strcat(casillaImprimir, "I ");
    if (casilla->refugio) strcat(casillaImprimir, "S ");
    if (casilla->oasis) strcat(casillaImprimir, "O ");
    if (casilla->tormenta) strcat(casillaImprimir, "T ");
    if (casilla->vidas) {
      if (casilla->vidas > 1) {
        sprintf(buff, "%s%dV ", casillaImprimir, casilla->vidas);
        strcpy(casillaImprimir, buff);
      } else {
        strcat(casillaImprimir, "V ");
      }
    }
    if (casilla->premios) {
      if (casilla->premios > 1) {
        sprintf(buff, "%s%dP ", casillaImprimir, casilla->premios);
        strcpy(casillaImprimir, buff);
      } else {
        strcat(casillaImprimir, "P ");
      }
    }
    if (casilla->bandidos) {
      if (casilla->bandidos > 1) {
        sprintf(buff, "%s%dB ", casillaImprimir, casilla->bandidos);
        strcpy(casillaImprimir, buff);
      } else {
        strcat(casillaImprimir, "B ");
      }
    }
    if (casilla->jugador) strcat(casillaImprimir, "J ");
  }

  aux = strrchr(casillaImprimir, ' ');
  if (aux) {
    *aux = '\0';
  }

  strcat(casillaImprimir, "]");
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

  puts("\nPresione Enter para lanzar el dado.");
  leerLinea(linea, sizeof(linea));

  nuevoMovimiento.pasos = lanzarDado();
  printf("Dado: %d\n", nuevoMovimiento.pasos);
  nuevoMovimiento.entidad = &(juego->jugador);

  do {
    printf("\nMovimiento disponible:\n");
    printf("  F - Avanzar hasta la posicion %d\n",
           calcularDestinoJugador(juego, nuevoMovimiento.pasos, 'F'));
    printf("  B - Retroceder hasta la posicion %d\n",
           calcularDestinoJugador(juego, nuevoMovimiento.pasos, 'B'));
    printf("Elija direccion: ");

    leerLinea(linea, sizeof(linea));
    nuevoMovimiento.direccion = (char)toupper((unsigned char)linea[0]);

    if (nuevoMovimiento.direccion != 'B' && nuevoMovimiento.direccion != 'F') {
      printf("Opcion invalida. Use F o B.\n");
    }
  } while (nuevoMovimiento.direccion != 'B' && nuevoMovimiento.direccion != 'F');

  encolarMovimiento(&juego->colaMovimientos, nuevoMovimiento);
  encolarMovimiento(&juego->colaMovimientosJugador, nuevoMovimiento);
  return 1;
}

void accionarBandido(void *bandido, void *contexto)
{
  tJuego *juego = (tJuego *)contexto;
  tMovimiento nueMov;
  tBandido *bandidoActual = (tBandido *)bandido;
  int posJugador = juego->jugador.posicion;
  int haciaAdelante;
  int haciaAtras;

  if (!bandidoActual->activo) return;

  nueMov.pasos = lanzarDado();
  nueMov.entidad = bandidoActual;

  haciaAdelante = (posJugador - bandidoActual->posicion + juego->config.totalCasillas) % juego->config.totalCasillas;
  haciaAtras = (bandidoActual->posicion - posJugador + juego->config.totalCasillas) % juego->config.totalCasillas;
  nueMov.direccion = (haciaAdelante <= haciaAtras) ? 'F' : 'B';

  encolarMovimiento(&juego->colaMovimientos, nueMov);
}

int procesarMovimientoBandidos(tJuego *juego)
{
  recorrerListaYAccionar(&juego->bandidos, juego, accionarBandido);
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

  mostrarPanelTurno(juego);

  if (!juego->jugador.perdidoTurno) {
    procesarMovimientoJugador(juego);
  } else {
    puts("\nLa tormenta obliga al jugador a perder este turno.");
  }

  procesarMovimientoBandidos(juego);

  if (!juego->jugador.perdidoTurno) {
    desencolarMovimiento(&juego->colaMovimientos, &nuevoMovimiento);
    casillaActualJugador = buscarCasilla(&juego->tablero, juego->jugador.posicion, cmpPosCasillas);
    if (!moverJugador(juego, nuevoMovimiento, casillaActualJugador)) {
      juego->juegoActivo = 0;
      return 0;
    }
    juego->totalMovimientos++;
  } else {
    juego->jugador.perdidoTurno = 0;
  }

  casillaActualJugador = buscarCasilla(&juego->tablero, juego->jugador.posicion, cmpPosCasillas);
  printf("\nLlegaste a la posicion %d: %s\n",
         juego->jugador.posicion,
         descripcionCasilla(casillaActualJugador));

  if (aplicarEfectoCasilla(&juego->jugador, casillaActualJugador)) {
    juego->juegoActivo = 0;
    return 2;
  }

  if (verificarDerrota(&juego->jugador)) {
    juego->juegoActivo = 0;
    return 0;
  }

  if (!jugarTurnoComputadora(juego)) {
    juego->juegoActivo = 0;
    return 0;
  }

  if (verificarDerrota(&juego->jugador)) {
    juego->juegoActivo = 0;
    return 0;
  }

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
    printf("\nFelicidades %s, llegaste a Ciudad Refugio con %d puntos.\n",
           j->jugador.nombre, j->jugador.puntos);
  } else if (estado == 0) {
    printf("\nGAME OVER\n%s fue derrotado antes de llegar a Ciudad Refugio.\n",
           j->jugador.nombre);
  }
}
