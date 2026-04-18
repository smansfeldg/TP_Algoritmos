#ifndef LISTA_CIRCULAR_H
#define LISTA_CIRCULAR_H

/**
 * @file lista_circular.h
 * @brief Implementación de la Lista Circular Doblemente Enlazada.
 *
 * Capa de Estructuras de Datos.
 * Debe ser genérica para manejar los nodos de la ruta.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIN_MEM    0
#define TODO_BIEN  1
#define CLA_DUP    2

typedef struct sNodo
{
    void        *info;
    unsigned    tamInfo;
    struct sNodo *sig,
                 *ant;
} tNodo;

typedef tNodo *tLista;

void crearLista(tLista *p);
int vaciarLista(tLista *p);
int listaVacia(const tLista *p);
int listaLlena(const tLista *p, unsigned cantBytes);
int insertarAlFinal(tLista *p, const void *d, unsigned cantBytes);
int insertarAlComienzo(tLista *p, const void *d, unsigned cantBytes);
void mostrarDeIzqADer(const tLista *p,
                      void (*mostrar)(const void *));
void mostrarDeDerAIzq(const tLista *p,
                      void (*mostrar)(const void *));
int insertarEnOrden(tLista *p, const void *d, unsigned cantBytes,
                    int (*comparar)(const void *, const void *),
                    int (*acumular)(void **, unsigned *,
                                     const void *, unsigned));
void ordenarLista(tLista *p, int (*comparar)(const void *, const void *));
int eliminarPorClave(tLista *p, void *d, unsigned cantBytes,
                     int (*comparar)(const void *, const void *));

#endif // LISTA_CIRCULAR_H
