/**
 * @file lista_circular.c
 * @brief Implementación de las operaciones de la Lista Circular Doblemente Enlazada.
 *
 * Lógica para la manipulación genérica de los nodos de la ruta.
 */

#include "../include/lista_circular.h"

void crearLista(tLista *p)
{
    
}

int vaciarLista(tLista *p)
{
    
    return 0;
}

int listaVacia(const tLista *p)
{
    
    return 0;
}

int listaLlena(const tLista *p, unsigned cantBytes)
{
    
    return 0;
}

int insertarAlFinal(tLista *p, const void *d, unsigned cantBytes)
{
    
    return 0;
}

int insertarAlComienzo(tLista *p, const void *d, unsigned cantBytes)
{
    
    return 0;
}

void mostrarDeIzqADer(const tLista *p,
                      void (*mostrar)(const void *))
{
    
}

void mostrarDeDerAIzq(const tLista *p,
                      void (*mostrar)(const void *))
{
    
}

int insertarEnOrden(tLista *p, const void *d, unsigned cantBytes,
                    int (*comparar)(const void *, const void *),
                    int (*acumular)(void **, unsigned *,
                                     const void *, unsigned))
{
    
    return 0;
}

void ordenarLista(tLista *p, int (*comparar)(const void *, const void *))
{
    
}

int eliminarPorClave(tLista *p, void *d, unsigned cantBytes,
                     int (*comparar)(const void *, const void *))
{
    
    return 0;
}
