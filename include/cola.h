#ifndef COLA_H
#define COLA_H

/**
 * @file cola.h
 * @brief Implementación de la Cola.
 *
 * Capa de Estructuras de Datos.
 * Sirve para el manejo de los movimientos pendientes
 * del jugador y de los bandidos.
 */

#include <stdlib.h>
#include <string.h>

typedef struct sNodo
{
    void *info;
    unsigned tam;
    struct sNodo *sig;
}tNodo;

typedef struct
{
    tNodo *primero;
    tNodo *ultimo;
}tCola;

void CrearCola(tCola*);
int ColaLlena(const tCola*, unsigned);
int ColaVacia(const tCola*);
void VaciarCola(tCola*);
int PonerEnCola(tCola*,const void*, unsigned);
int SacarDeCola(tCola*, void*, unsigned);
int VerPrimero(const tCola*, void*, unsigned);

unsigned Min(unsigned, unsigned);

#endif // COLA_H
