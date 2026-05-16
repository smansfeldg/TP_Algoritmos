#include "../include/entidades.h"
#include <time.h>
#include <ctype.h>

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

void crearBandido(tBandido *b, int id, int posicion)
{
    b->id = id;
    b->posicion = posicion;
    b->activo = 1;
}

//NO VA
void inicializarConfiguracion(tConfiguracion *cfg)
{

}

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

void mostrarConfiguracion(const tConfiguracion *cfg)
{
  //IMPRIME UN MENSAJE MOSTRANDO CADA CAMPO CON SU VALOR
}

int generarTablero(tLista *tablero, const tConfiguracion *cfg)
{
    //CREA CASILLA POR CASILLA EN BUCLE
    //COMPROBAR QUE LA SUMA DE CASILLAS ESPECIALES SEA MENOR AL TAMAÑO DEL TABLERO - 2 (POR EL INICIO Y EL FINAL)
    //UTILIZA UN ALGORITMO PARA ASIGNAR ALEATORIAMENTE LAS CASILLAS ESPECIALES Y QUE NO SE REPITAN
    //ASIGNA ALEATORIAMENTE A LOS BANDIDOS
    //UBICA AL JUGADOR EN LA PRIMER CASILLA
    return 1;
}

void mostrarTablero(const tLista *tablero)
{
    //A DEBATIR
}

//DEBERIA HABER UNA FUNCIÓN PARA BUSCAR EN LA IMPLEMENTACION DE LISTA CIRCULAR
//ESTA FUNCION DEBERIA RECIBIR UNA FUNCION DE COMPARACION DE PARA POSICION DE CASILLAS
int buscarCasilla(const tLista *tablero, int posicion, tCasilla *casilla)
{
    //DEBERIA LLAMAR LA FUNCION DE BUSQUEDA CON LA FUNCION DE BUSQUEDA DE POSICION DE CASILLA
    return 0;
}

//VA EN "procesarMovimientoJugador"
int moverJugador(tLista *tablero, tJugador *j, int pasos, char direccion, int totalCasillas)
{
    int nuevaPos;
    return nuevaPos;
}

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

void mostrarEstadoJugador(const tJugador *j)
{
  //UTILIZA UN IF POR CADA CAMPO DEL JUGADOR Y MUESTRA UN MENSAJE SEGUN CORRESPONDA
}

void inicializarJuego(tJuego *juego, const tConfiguracion *cfg, const char *nombreJugador)
{
  //INICIALIZA/INSTANCIA/CREA LOS ELEMENTOS DEL JUEGO CON LAS FUNCIONES CORRESPONDIENTES
}

void liberarJuego(tJuego *juego)
{
  //LO QUE DICE EL NOMBRE
}

int encolarMovimiento(tCola *cola, char direccion, int pasos)
{
  //LO QUE DICE EL NOMBRE
}

int desencolarMovimiento(tCola *cola, tMovimiento *mov)
{
    //LO QUE DICE EL NOMBRE
    return 1; //SacarDeCola
}

//NO PODÉS MOSTRAR UNA COLA DE MOVIMIENTOS CONST PORQUE LA TENÉS QUE VACIAR SI O SI
void mostrarColaMovimientos(const tCola *cola)
{
  //MIENTRAS COLA NO SEA VACIA
  // DESAPILAR MOVIMIENTO
  // IMPRIMIR MOVIMIENTO
}

//VA EN procesarMovimientosBandido
int moverBandido(tLista *tablero, tBandido *b, const tJugador *j, int totalCasillas)
{
    return 1; //b->posicion;
}

int verificarColision(const tJugador *j, const tBandido *b)
{
    return (j->posicion == b->posicion && b->activo);
}

int verificarVictoria(const tJugador *j, const tLista *tablero)
{
    //LO QUE DICE EL NOMBRE
    return 0;
}

int verificarDerrota(const tJugador *j)
{
    return (j->vidas <= 0);
}

//ESTAS FUNCIONES NO VAN / DEBERIAN SE CON RESPECTO A LAS PARTIDAS
//ADEMAS DEBERIA HABER UN PAR PARA GUARDAR Y CARGAR JUGADORES
void guardarCaravana(const char *archivo, const tJuego *juego)
{

}

int cargarCaravana(const char *archivo, tJuego *juego)
{
    return 1;
}
