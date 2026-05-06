#include "../include/entidades.h"
#include <time.h>
#include <ctype.h>

void crearJugador(tJugador *j, const char *nombre, int posicionInicial, int vidas)
{

}

void crearBandido(tBandido *b, int id, int posicion)
{

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
    return 1; //(j->posicion == b->posicion && b->activo);
}

void procesarColision(tJuego *juego, int indiceBandido)
{

}

int verificarVictoria(const tJugador *j, const tLista *tablero)
{

    return 0;
}

int verificarDerrota(const tJugador *j)
{
    return 1; //(j->vidas <= 0);
}

void guardarCaravana(const char *archivo, const tJuego *juego)
{

}

int cargarCaravana(const char *archivo, tJuego *juego)
{
    return 1;
}