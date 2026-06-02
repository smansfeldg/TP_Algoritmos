#include "../include/juego.h"
#include <time.h>
#include <ctype.h>
#include <stdio.h>

//LISTO
void mostrarBienvenida(){
  puts("\n\t\t\t° ° ° ° ° Caravana ° ° ° ° °");
  puts("Una caravana intenta atravesar una antigua ruta comercial para llegar a una ciudad refugio antes de que se agoten sus recursos.\n\nA lo largo del trayecto existen tesoros, provisiones, oasis, tormentas de arena y bandidos.\n\nEl desierto es hostil e impredecible. En cada turno el jugador arrojará un dado virtual que generará un valor aleatorio entre 1 y 6. El valor obtenido determinará exactamente la cantidad de posiciones que deberá desplazarse en ese turno.\n\nLos bandidos se desplazan automáticamente tratando de interceptar al jugador.\n\nCada decisión cuenta. La caravana debe avanzar. El desierto no perdona errores.\n");
}

//LISTO
void mostrarReglas(){
  puts("\n\t\t\t* * * * Reglas del juego * * * *\n");
  puts("\nEl jugador (J) debe desplazarse sobre una ruta desértica persiguiendo el objetivo de llegar desde el Campamento Inicial (I) hasta la Ciudad Refugio (S), evitando ser interceptado por los Bandidos (B) y aprovechando los recursos distribuidos a lo largo del recorrido. Para los bandidos es posible desplazarse hacia atrás y hacia adelante inclusive a través de la entrada y la salida, como si la ruta fuera un círculo. La partida finalizará cuando el jugador alcance la Ciudad Refugio o cuando pierda todas sus vidas antes de alcanzarla.\n\nSi, al efectuar un lanzamiento del dado, el jugador sobrepasara la Ciudad Refugio, deberá continuar su desplazamiento retrocediendo la cantidad de posiciones sobrantes, de modo tal que el movimiento total realizado sea exactamente igual al valor obtenido en el dado.\n\n");
}

//COMPLETAR
int iniciarPartida(tJuego *juego, FILE *archJug, ArbolBin *indice){
  char nombre[MAX_NOMBRE];
  char *aux;
  int existe;
  char buffer[100];

  do{
    printf("\nIngrese su nombre de jugador: ");
    fgets(nombre, MAX_NOMBRE, stdin);
    aux = strrchr(nombre,'\n');
    if(aux){
      (*aux) = '\0';
    }
    existe=buscarJugador(nombre,indice,cmpIndxNombre);
    if(existe==1)
    {
        printf("\nEse nombre ya existe. Eres tu? (S/N)\n");
        do{
            fgets(buffer,100,stdin);
            fflush(stdin);
        }while(buffer[0]!='S'&&buffer[0]!='N');
        if(buffer[0]=='S')
            existe=2;//Jugador ya existente juega una nueva partida
        else
            printf("\nEse nombre ya esta en uso, por favor elija otro.");
    }
  }while(existe==1);

  crearJugador(&juego->jugador,nombre,1,juego->config.vidasIniciales);
  if(existe!=2)
  {
      fseek(archJug,0,SEEK_END);
      juego->jugador.idJugador=(ftell(archJug)/sizeof(tRegistroJugador))+100;
      actualizarJugadores(archJug,indice,&juego->jugador);
  }

  return 1;
}

//LISTA
int lanzarDado(){
    return (rand() % 6) + 1; // analizar posibilidad de usar una función más robusta
}

//LISTA
void imprimirCasillaJuego(const void *info, void* param){
    tCasilla *casilla = (tCasilla*)info;
    char casillaImprimir[50];
    char buff[60];
    char *aux;
    int *pos = (int*)param;

    if(!casilla->bandidos && !casilla->inicio && !casilla->jugador && !casilla->normal && !casilla->oasis && !casilla->premios && !casilla->refugio && !casilla->tormenta && !casilla->vidas){
      casilla->normal = 1;
    }

    sprintf(casillaImprimir, "%02d:[", *pos);
    (*pos)++;

    if(casilla->normal){
      strcat(casillaImprimir, ".");
    }
    else{
      if(casilla->inicio){
        strcat(casillaImprimir, "I ");
      }
      else if(casilla->refugio){
        strcat(casillaImprimir, "S ");
      }
      else{
        if(casilla->oasis){
          strcat(casillaImprimir, "O ");
        }
        if(casilla->tormenta){
          strcat(casillaImprimir, "T ");
        }
        if(casilla->vidas){
          if(casilla->vidas>1){
            sprintf(buff ,"%s%dV ",casillaImprimir,casilla->vidas);
            strcpy(casillaImprimir,buff);
          }
          else
            strcat(casillaImprimir, "V ");
        }
        if(casilla->premios){
          if(casilla->premios>1){
            sprintf(buff ,"%s%dP ",casillaImprimir,casilla->premios);
            strcpy(casillaImprimir,buff);
          }
          else
            strcat(casillaImprimir, "P ");
        }
      }

      if(casilla->bandidos){
        if(casilla->bandidos>1){
          sprintf(buff ,"%s%dB ",casillaImprimir,casilla->bandidos);
          strcpy(casillaImprimir,buff);
        }
        else
          strcat(casillaImprimir, "B ");
      }
      if(casilla->jugador){
        strcat(casillaImprimir, "J ");
      }
    }

    aux = strrchr(casillaImprimir,' ');
    if(aux)
      (*aux) = '\0';

    if(strlen(casillaImprimir)<7){
      aux = strrchr(casillaImprimir,'[');
      memmove(aux, aux+1,20);
    }
    else{
      strcat(casillaImprimir, "]");
    }

    puts(casillaImprimir);
}

//LISTA
void mostrarTablero(const tLista* tablero){
    int posIni = 1;
    if (!tablero) return;
    printf("--- Mapeo del Desierto ---\n\n");
    mostrarDeIzqADer(tablero, &posIni, imprimirCasillaJuego);
    printf("\n--------------------------\n\n");
}

//LISTA
int procesarMovimientoJugador(tJuego *juego){
  tMovimiento nuevoMovimiento;
  char buffer[100];

  printf("Presione cualquier tecla para lanzar el dado. . .");
  getc(stdin);

  nuevoMovimiento.pasos=lanzarDado();
  printf("Obtuviste %d en el dado\n", nuevoMovimiento.pasos);
  nuevoMovimiento.entidad= &(juego->jugador);
  //LE PIDE AL JUGADOR QUE INGRESE UNO DE LOS CARACTERES DE MOVIMIENTO
  //HASTA QUE NO INGRESE UNO VALIDO LE SIGUE PIDIENDO
  //B - RETROCEDER
  //F - AVANZAR
  puts("Ingrese como quiere moverse:");
  if((juego->jugador.posicion - nuevoMovimiento.pasos) > 0){
    puts("(B - Retroceder)");
  }
  puts("(F - Avanzar)");

  do{
    do{
      fgets(buffer,100,stdin);
      fflush(stdin);
    }while(buffer[0]=='\n');

    nuevoMovimiento.direccion = buffer[0];

  }while( !(nuevoMovimiento.direccion == 'B') && !(nuevoMovimiento.direccion == 'F') );

  encolarMovimiento(&juego->colaMovimientos,nuevoMovimiento);
  encolarMovimiento(&juego->colaMovimientosJugador,nuevoMovimiento);
  return 1;
}

//LISTA
void accionarBandido(void* bandido, void* contexto){
  tJuego *juego = (tJuego*)contexto;
  tMovimiento nueMov;
  tBandido* bandidoActual = (tBandido*)bandido;
  if(bandidoActual->activo){
    nueMov.pasos=lanzarDado();
    nueMov.entidad=bandidoActual;
    int posJugador = juego->jugador.posicion;
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
  }

}

//LISTA
int procesarMovimientoBandidos(tJuego *juego){

  recorrerListaYAccionar(&juego->bandidos,juego,accionarBandido);

  return 1;
}

//LISTA
//DEVUELVE 1 SI TERMINO EL TURNO CORRECTAMENTE, O 0 SI EL JUGADOR FUE DERROTADO
int jugarTurnoComputadora(tJuego *juego){
  tMovimiento nuevoMovimiento;
  tCasilla* casillaActualBandido;
  tBandido* bandido;

  while(!ColaVacia(&juego->colaMovimientos)){
    desencolarMovimiento(&juego->colaMovimientos, &nuevoMovimiento);
    bandido = (tBandido*)nuevoMovimiento.entidad;

    casillaActualBandido = buscarCasilla(&juego->tablero, bandido->posicion,cmpPosCasillas);
    if(bandido->activo){
      printf("[Movimiento de bandido %d (casilla %d): %d%c]\n",bandido->id,casillaActualBandido->posicion,nuevoMovimiento.pasos,nuevoMovimiento.direccion);
      if(moverBandido(juego, bandido, nuevoMovimiento, casillaActualBandido) == -1){
        return 0;
      }
    }
  }

  return 1;
}

//LISTA
//ESTADOS DEL JUEGO
//0-DERROTA 1-JUGANDO 2-VICTORIA
int ejecutarTurno(tJuego *juego){
    tMovimiento nuevoMovimiento;
    tCasilla* casillaActualJugador;

    printf("********** %s **********\n\n", juego->jugador.nombre);
    printf(" Puntaje: %d\n",juego->jugador.puntos);
    printf(" Vida: %d\n",juego->jugador.vidas);
    printf("\n********** Turno %d **********\n", juego->turnoActual);

    //FASE DE TIRAR DADOS
    //JUGADOR
    if(!(juego->jugador.perdidoTurno)){
      procesarMovimientoJugador(juego);
    }else{
      puts("TURNO PERDIDO");
    }

    //BANDIDOS
    procesarMovimientoBandidos(juego);
    puts("[Movimientos procesados]");
    //FASE DE MOVER
    //JUGADOR
    if(!(juego->jugador.perdidoTurno)){
      desencolarMovimiento(&(juego->colaMovimientos), &nuevoMovimiento);
      puts("[Movimiento jugador encolado]");
      casillaActualJugador = buscarCasilla(&juego->tablero,(juego->jugador).posicion,cmpPosCasillas);
      puts("[Casilla jugador obtenida]");
      if(!moverJugador(juego, nuevoMovimiento, casillaActualJugador)){
        return 0;
      }
      puts("[Movimiento jugador realizado]");
    }else{
      juego->jugador.perdidoTurno = 0;
    }

    if(juego->jugador.protegidoOasis){
      juego->jugador.protegidoOasis = 0;
    }

    //AGREGAR: SUMAR UNO A LOS MOVIMIENTOS TOTALES DE LA PARTIDA
    juego->totalMovimientos++;
    casillaActualJugador = buscarCasilla(&juego->tablero,(juego->jugador).posicion,cmpPosCasillas);
    //DEVUELVE 1 SI EL USUARIO LLEGO A LA CASILLA DE REFUGIO
    if(aplicarEfectoCasilla(&(juego->jugador), casillaActualJugador)){
      juego->juegoActivo=0;
      return 2;
    }
    puts("[Efectos de la casilla aplicados al jugador]");

    //BANDIDOS
    if(!jugarTurnoComputadora(juego)){
      juego->juegoActivo=0;
      return 0;
    }

    //PORQUE NO TIENE SENTIDO SEGUIR PROCESANDO SI YA FINALIZÓ EL JUEGO ADEMÁS DE OTRAS CUESTIONES

    juego->turnoActual++;

    printf("\n*****************************\n");
    return 1;
}

//LISTO
void mostrarFinJuego(int estado, const tJuego *j){
  //IMPRIME POR PANTALLA EL MENSAJE FINAL SEGUN EL RESULTADO DE LA PARTIDA
  if(estado==2){
    printf("\nFelicidades %s llegaste a Ciudad Refugio con %d puntos\n", j->jugador.nombre, j->jugador.puntos);
  }
  else if(estado==0){
    puts("\nGAME OVER\nFuiste derrotado por los Bandidos antes de llegar a Ciudad Refugio\n");
  }
}

//HACER UNA FUNCION PARA PROCESAR LAS PARTIDAS DE JUGADORES EN UNA LISTA ORDENADA POR PUNTOS TOTALES
//HACER UNA FUNCION PARA MOSTRAR LA LISTA DE PUNTAJES
