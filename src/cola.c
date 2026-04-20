/**
 * @file cola.c
 * @brief Implementación de las funciones de la estructura de Cola.
 *
 * Encolar (enqueue), desencolar (dequeue) y manejo de los turnos/movimientos
 * de los personajes en espera.
 */

#include "../include/cola.h"

void CrearCola(tCola *cola)
{
    cola->primero=NULL;
    cola->ultimo=NULL;
}
//////////////////////////////////////////////////////////////////////////////////////////
int ColaLlena(const tCola *cola, unsigned tam)
{
    tNodo *nuevo;
    void *dato;

    nuevo=malloc(sizeof(tNodo));
    dato=malloc(tam);

    free(nuevo);
    free(dato);

    return dato==NULL || nuevo==NULL;
}
//////////////////////////////////////////////////////////////////////////////////////////
int ColaVacia(const tCola *cola)
{
    return cola->primero==NULL;
}
//////////////////////////////////////////////////////////////////////////////////////////
void VaciarCola(tCola *cola)
{
    tNodo *elim;

    while(cola->primero!=NULL)
    {
        elim=cola->primero;
        cola->primero=elim->sig;
        free(elim->info);
        free(elim);
    }
}
//////////////////////////////////////////////////////////////////////////////////////////
