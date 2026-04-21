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
int VerPrimero(const tCola *cola, void *dato, unsigned tam)
{
    if(cola->primero==NULL)
        return 0;
    memcpy(dato,cola->primero->info,Min(tam,cola->primero->tam));
    return 1;
}
//////////////////////////////////////////////////////////////////////////////////////////
int PonerEnCola(tCola *cola, const void *dato, unsigned tam)
{
    tNodo *nuevo;

    nuevo=malloc(sizeof(tNodo));
    if(nuevo==NULL)
        return 0;

    nuevo->info=malloc(tam);
    if(nuevo->info==NULL)
    {
        free(nuevo);
        return 0;
    }
    memcpy(nuevo->info,dato,tam);
    nuevo->tam=tam;
    nuevo->sig=NULL;
    if(cola->ultimo!=NULL)
        cola->ultimo->sig=nuevo;
    else
        cola->primero=nuevo;
    cola->ultimo=nuevo;

    return 1;
}
//////////////////////////////////////////////////////////////////////////////////////////
int SacarDeCola(tCola *cola, void *dato, unsigned tam)
{
    tNodo *elim=cola->primero;
    if(elim==NULL)
        return 0;

    cola->primero=elim->sig;
    memcpy(dato,elim->info,Min(tam,elim->tam));
    free(elim->info);
    free(elim);
    if(cola->primero==NULL)
        cola->ultimo=NULL;
    return 1;
}
//////////////////////////////////////////////////////////////////////////////////////////
unsigned Min(unsigned a, unsigned b)
{
    if(a<=b)
        return a;
    else
        return b;
}
