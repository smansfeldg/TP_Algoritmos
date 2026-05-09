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

void inicializarConfiguracion(tConfiguracion *cfg)
{

}

int cargarConfiguracion(const char *archivo, tConfiguracion *cfg)
{
    return 1;
}

void mostrarConfiguracion(const tConfiguracion *cfg)
{

}

int generarTablero(tLista *tablero, const tConfiguracion *cfg)
{
    return 1;
}

void mostrarTablero(const tLista *tablero)
{

}

int buscarCasilla(const tLista *tablero, int posicion, tCasilla *casilla)
{
    return 0;
}

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

}

void inicializarJuego(tJuego *juego, const tConfiguracion *cfg, const char *nombreJugador)
{

}

void liberarJuego(tJuego *juego)
{

}

int encolarMovimiento(tCola *cola, char direccion, int pasos)
{

}

int desencolarMovimiento(tCola *cola, tMovimiento *mov)
{
    return 1; //SacarDeCola
}

void mostrarColaMovimientos(const tCola *cola)
{

}

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

    return 0;
}

int verificarDerrota(const tJugador *j)
{
    return (j->vidas <= 0);
}

void guardarCaravana(const char *archivo, const tJuego *juego)
{

}

int cargarCaravana(const char *archivo, tJuego *juego)
{
    return 1;
}