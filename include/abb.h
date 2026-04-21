#ifndef ABB_H
#define ABB_H

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
void liberarArbolBin(ArbolBin *arbol);

#endif // ABB_H
