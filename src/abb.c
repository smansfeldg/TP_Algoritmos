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

  if(*arbol){
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

  return cargarElementosOrdenadosArbolBin(&((*arbol)->nodoDer),datos,medio+1,lim_der,params,leer);
}

void recorrerEnOrdenArbolBin(ArbolBin *arbol, unsigned nivel, void* params, void (*accion)(void* dato, size_t tam, unsigned nivel, void* params)){
  if(!(*arbol)){
    return;
  }

  recorrerEnOrdenArbolBin(&(*arbol)->nodoIzq,nivel+1,params,accion);
  accion((*arbol)->dato,(*arbol)->tam,nivel,params);
  recorrerEnOrdenArbolBin(&(*arbol)->nodoDer,nivel+1,params,accion);
}

int eliminarRaizArbolBin(ArbolBin *arbol){
  NodoRaiz *aux;

  //Si el arbol es NULL retorna 0
  if( !(*arbol) ){
    return 0;
  }

  //Libera el dato
  free((*arbol)->dato);

  //Si sus hijos son NULL simplemente elimina el nodo
  if(!(*arbol)->nodoIzq && !(*arbol)->nodoDer){
    free((*arbol));
    (*arbol) = NULL;
    return 1;
  }

  //Guardo temporalmente el nodo a eliminar
  aux = (*arbol);

  //Guardo en arbol el subarbol con el que voy a reemplazar el nodo eliminado
  //El subarbol es el camino de mayor nivel entre el más grande de los menores
  //y el más pequeño de los mayores
  arbol = alturaArbolBin(&(*arbol)->nodoIzq) > alturaArbolBin(&(*arbol)->nodoDer)?mayorSubArbolBin(&(*arbol)->nodoIzq):menorSubArbolBin(&(*arbol)->nodoDer);

  //Copio los datos del reemplazo en el nodo que va a ser eliminado
  aux->dato = (*arbol)->dato;
  aux->tam = (*arbol)->tam;

  //Guardo el nodo con el que reemplacé el nodo eliminado para eliminarlo
  aux = (*arbol);

  //Asigno el nodo izquierdo, si existe, al nodo con el que reemplacé al
  //nodo eliminado o lo asigno al derecho si no existe
  (*arbol) = (*arbol)->nodoIzq?(*arbol)->nodoIzq:(*arbol)->nodoDer;

  //Libero el nodo para reemplazo que quedó suelto
  free(aux);

  return 1;
}

int eliminarElementoArbolBin(ArbolBin *arbol, void* elemento, size_t tam, int (*cmp)(const void* a, const void* b)){
  if( !(arbol = buscarSubArbolBin(arbol,elemento,cmp)) ){
    return 0;
  }

  if((*arbol)->tam < tam){
    tam = (*arbol)->tam;
  }
  memcpy(elemento,(*arbol)->dato,tam);

  return eliminarRaizArbolBin(arbol);
}

NodoRaiz* buscarNodoArbolBin(const ArbolBin *arbol, void* elemento, int (*cmp)(const void* a, const void* b)){
  int res;

  if( !(*arbol) ){
    return NULL;
  }

  res = cmp(elemento,(*arbol)->dato);
  if(res){
    if(res<0){
      return buscarNodoArbolBin(&(*arbol)->nodoIzq,elemento,cmp);
    }
    return buscarNodoArbolBin(&(*arbol)->nodoDer,elemento,cmp);
  }

  return (*arbol);
}

NodoRaiz** buscarSubArbolBin(const ArbolBin *arbol, void* elemento, int (*cmp)(const void* a, const void* b)){
  int res;

  if( !(*arbol) ){
    return NULL;
  }

  res = cmp(elemento,(*arbol)->dato);
  if(res){
    if(res<0){
      return buscarSubArbolBin(&(*arbol)->nodoIzq,elemento,cmp);
    }
    return buscarSubArbolBin(&(*arbol)->nodoDer,elemento,cmp);
  }

  return (NodoRaiz**)arbol;
}

NodoRaiz* mayorNodoArbolBin(const ArbolBin* arbol){
  if( !(*arbol) ){
    return NULL;
  }

  while((*arbol)->nodoDer){
    arbol = &(*arbol)->nodoDer;
  }

  return (NodoRaiz*)(*arbol);
}

NodoRaiz* menorNodoArbolBin(const ArbolBin* arbol){
  if( !(*arbol) ){
    return NULL;
  }

  while((*arbol)->nodoIzq){
    arbol = &(*arbol)->nodoIzq;
  }

  return (NodoRaiz*)(*arbol);
}

NodoRaiz** mayorSubArbolBin(const ArbolBin* arbol){
  if( !(*arbol) ){
    return NULL;
  }

  while((*arbol)->nodoDer){
    arbol = &(*arbol)->nodoDer;
  }

  return (NodoRaiz**)arbol;
}

NodoRaiz** menorSubArbolBin(const ArbolBin* arbol){
  if( !(*arbol) ){
    return NULL;
  }

  while((*arbol)->nodoIzq){
    arbol = &(*arbol)->nodoIzq;
  }

  return (NodoRaiz**)arbol;
}

unsigned alturaArbolBin(const ArbolBin* arbol){
  unsigned alturaDer, alturaIzq;
  if(!(*arbol)){
    return 0;
  }

  alturaDer = alturaArbolBin(&(*arbol)->nodoDer);
  alturaIzq = alturaArbolBin(&(*arbol)->nodoIzq);

  return 1 + (alturaDer>alturaIzq?alturaDer:alturaIzq);
}

unsigned cantidadNodosArbolBin(const ArbolBin* arbol){
  if(!(*arbol)){
    return 0;
  }
  return 1 + cantidadNodosArbolBin(&(*arbol)->nodoIzq) + cantidadNodosArbolBin(&(*arbol)->nodoDer);
}

void liberarArbolBin(ArbolBin *arbol){
  if(arbol && *arbol){
    liberarArbolBin(&((*arbol)->nodoIzq));
    liberarArbolBin(&((*arbol)->nodoDer));
    free((*arbol)->dato);
    free((*arbol));
    *arbol=NULL;
  }
}
