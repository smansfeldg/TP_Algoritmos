/**
 * @file abb.c
 * @brief Operaciones del Árbol de Búsqueda Binaria.
 *
 * Inserción, eliminación, búsqueda y recorridos para administrar
 * a los jugadores indexados.
 */
#include <string.h>
#include <stdlib.h>
#include "../include/abb.h"

void crearArbolBin(ArbolBin *arbol){
  *arbol = NULL;
}

int insertarNodoRecArbolBin(ArbolBin *arbol, const void* dato, const size_t tam, int (*cmp)(const void* a, const void* b)){
  int res_comp, res_inser;
  NodoRaiz *nuevo_nodo;

  if(arbol){
    res_comp = cmp((*arbol)->dato,dato);
    if(res_comp>0){
      return (res_inser = insertarNodoRecArbolBin(&(*arbol)->nodoIzq,dato,tam,cmp));
    }
    else if(res_comp<0){
      return (res_inser = insertarNodoRecArbolBin(&(*arbol)->nodoDer,dato,tam,cmp));
    }

    return 0;
  }

  nuevo_nodo = malloc(sizeof(NodoRaiz));

  if(!nuevo_nodo){
    return -1;
  }

  nuevo_nodo->dato = malloc(tam);

  if(!nuevo_nodo->dato){
    free(nuevo_nodo);
    return -1;
  }

  nuevo_nodo->tam=tam;
  nuevo_nodo->nodoDer=NULL;
  nuevo_nodo->nodoIzq=NULL;
  (*arbol) = nuevo_nodo;
  memcpy((*arbol)->dato, dato, tam);
  return 1;
}

int insertarNodoArbolBin(ArbolBin *arbol, const void* dato, const size_t tam, int (*cmp)(const void* a, const void* b)){
  int res_comp;
  NodoRaiz *nuevo_nodo;

  while(*arbol){
    res_comp = cmp((*arbol)->dato,dato);
    if(res_comp>0){
      arbol = &((*arbol)->nodoIzq);
    }
    else if(res_comp<0){
      arbol = &((*arbol)->nodoDer);
    }
    else{
      return 0;
    }
  }

  nuevo_nodo = malloc(sizeof(NodoRaiz));
  if(!nuevo_nodo){
    return -1;
  }

  nuevo_nodo->dato = malloc(tam);
  if(!nuevo_nodo->dato){
    free(nuevo_nodo);
    return -1;
  }

  nuevo_nodo->nodoDer=NULL;
  nuevo_nodo->nodoIzq=NULL;
  nuevo_nodo->tam=tam;

  (*arbol) = nuevo_nodo;
  memcpy((*arbol)->dato, dato, tam);
  return 1;
}

int cargarElementosOrdenadosArbolBin(ArbolBin *arbol, void *datos, int lim_izq, int lim_der, void* params, size_t (*leer)(void **elemento, void *datos, unsigned posicion, void *params)){
  int medio, resultado;

  medio = (lim_izq + lim_der)/2;

  if(lim_izq>lim_der){
    return 1;
  }

  (*arbol) = malloc(sizeof(NodoRaiz));
  if(!(*arbol)){
    return 0;
  }

  if(!((*arbol)->tam = leer(&((*arbol)->dato),datos,medio,params))){
    free((*arbol));
    return 0;
  }

  (*arbol)->nodoDer=NULL;
  (*arbol)->nodoIzq=NULL;

  if((resultado = cargarElementosOrdenadosArbolBin(&((*arbol)->nodoIzq),datos,lim_izq,medio-1,params,leer)) != 1){
    return resultado;
  }

  return cargarElementosOrdenadosArbolBin(&((*arbol)->nodoIzq),datos,medio+1,lim_der,params,leer);
}

void liberarArbolBin(ArbolBin *arbol){
  if((*arbol)->nodoIzq != NULL){
    liberarArbolBin(&((*arbol)->nodoIzq));
  }
  if((*arbol)->nodoDer != NULL){
    liberarArbolBin(&((*arbol)->nodoDer));
  }

  free((*arbol)->dato);

  if((*arbol) != NULL){
    free((*arbol));
  }

}
