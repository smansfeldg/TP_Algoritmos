#ifndef ABB_H
#define ABB_H
#include <stdio.h>

/**
 * @file abb.h
 * @brief Implementación del Árbol de Búsqueda Binaria (ABB).
 *
 * Capa de Estructuras de Datos.
 * Utilizado para el manejo y búsqueda rápida en el índice
 * de jugadores.
 */

typedef struct NodoArbol{
  void* dato;
  size_t tam;
  struct NodoArbol *nodoIzq, *nodoDer;
} NodoRaiz;

typedef NodoRaiz* ArbolBin;

void crearArbolBin(ArbolBin *arbol);
int insertarNodoRecArbolBin(ArbolBin *arbol, const void* dato, const size_t tam, int (*cmp)(const void* a, const void* b));
int insertarNodoArbolBin(ArbolBin *arbol, const void* dato, const size_t tam, int (*cmp)(const void* a, const void* b));
int cargarElementosOrdenadosArbolBin(ArbolBin *arbol, void *datos, int lim_izq, int lim_der, void* params, size_t (*leer)(void **elementos, void *datos, unsigned posicion, void *params));
void recorrerEnOrdenArbolBin(ArbolBin *arbol, unsigned nivel, void* params, void (*accion)(void* dato, size_t tam, unsigned nivel, void* params));
int eliminarElementoArbolBin(ArbolBin *arbol, void* elemento, size_t tam, int (*cmp)(const void* a, const void* b));
int eliminarRaizArbolBin(ArbolBin *arbol);
NodoRaiz* buscarNodoArbolBin(const ArbolBin *arbol, void* elemento, int (*cmp)(const void* a, const void* b));
NodoRaiz** buscarSubArbolBin(const ArbolBin *arbol, void* elemento, int (*cmp)(const void* a, const void* b));
NodoRaiz* mayorNodoArbolBin(const ArbolBin* arbol);
NodoRaiz* menorNodoArbolBin(const ArbolBin* arbol);
NodoRaiz** mayorSubArbolBin(const ArbolBin* arbol);
NodoRaiz** menorSubArbolBin(const ArbolBin* arbol);
unsigned alturaArbolBin(const ArbolBin* arbol);
unsigned cantidadNodosArbolBin(const ArbolBin* arbol);
void liberarArbolBin(ArbolBin *arbol);

#endif // ABB_H
