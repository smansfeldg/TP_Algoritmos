/**
 * @file lista_circular.c
 * @brief Implementación de las operaciones de la Lista Circular Doblemente Enlazada.
 *
 * Lógica para la manipulación genérica de los nodos de la ruta.
 */

#include "../include/lista_circular.h"

void crearLista(tLista *p)
{
    *p = NULL;
}

int vaciarLista(tLista *p)
{
    int cant = 0;
    if(!*p)
        return 0;
        
    tNodo *act = *p, *aux;
    /* Rompemos el enlace circular para poder iterar como una lista simple */
    (*p)->ant->sig = NULL; 

    while(act)
    {
        aux = act->sig;
        free(act->info);
        free(act);
        act = aux;
        cant++;
    }
    *p = NULL;
    return cant;
}

int listaVacia(const tLista *p)
{
    return *p == NULL;
}

int listaLlena(const tLista *p, unsigned cantBytes)
{
    tNodo *nue = (tNodo *)malloc(sizeof(tNodo));
    void *aux = malloc(cantBytes);

    free(nue);
    free(aux);
    return aux == NULL || nue == NULL;
}

int insertarAlFinal(tLista *p, const void *d, unsigned cantBytes)
{
    tNodo *nue = (tNodo *)malloc(sizeof(tNodo));
    if(!nue)
        return SIN_MEM;
        
    nue->info = malloc(cantBytes);
    if(!nue->info)
    {
        free(nue);
        return SIN_MEM;
    }
    
    memcpy(nue->info, d, cantBytes);
    nue->tamInfo = cantBytes;

    if(!*p)
    {
        nue->sig = nue;
        nue->ant = nue;
        *p = nue;
    }
    else
    {
        tNodo *ultimo = (*p)->ant;
        nue->sig = *p;
        nue->ant = ultimo;
        (*p)->ant = nue;
        ultimo->sig = nue;
    }
    return TODO_BIEN;
}

int insertarAlComienzo(tLista *p, const void *d, unsigned cantBytes)
{
    tNodo *nue = (tNodo *)malloc(sizeof(tNodo));
    if(!nue)
        return SIN_MEM;
        
    nue->info = malloc(cantBytes);
    if(!nue->info)
    {
        free(nue);
        return SIN_MEM;
    }
    
    memcpy(nue->info, d, cantBytes);
    nue->tamInfo = cantBytes;

    if(!*p)
    {
        nue->sig = nue;
        nue->ant = nue;
    }
    else
    {
        tNodo *ultimo = (*p)->ant;
        nue->sig = *p;
        nue->ant = ultimo;
        (*p)->ant = nue;
        ultimo->sig = nue;
    }
    *p = nue;
    return TODO_BIEN;
}

int mostrarDeIzqADer(const tLista *p,
                      void (*mostrar)(const void *))
{
    int cant = 0;
    if(!*p)
        return 0;

    mostrar(NULL);
    tNodo *act = *p;
    do
    {
        mostrar(act->info);
        act = act->sig;
        cant++;
    } while(act != *p);
    
    return cant;
}

int mostrarDeDerAIzq(const tLista *p,
                      void (*mostrar)(const void *))
{
    int cant = 0;
    if(!*p)
        return 0;

    mostrar(NULL);
    tNodo *act = (*p)->ant; /* Comenzamos desde el último */
    do
    {
        mostrar(act->info);
        act = act->ant;
        cant++;
    } while(act != (*p)->ant);
    
    return cant;
}

int insertarEnOrden(tLista *p, const void *d, unsigned cantBytes,
                    int (*comparar)(const void *, const void *),
                    int (*acumular)(void **, unsigned *,
                                     const void *, unsigned))
{
    tNodo *act = *p;
    tNodo *nue;
    int cmp;

    if(!act)
    {
        if((nue = (tNodo *)malloc(sizeof(tNodo))) == NULL) return SIN_MEM;
        if((nue->info = malloc(cantBytes)) == NULL) { free(nue); return SIN_MEM; }
        memcpy(nue->info, d, cantBytes);
        nue->tamInfo = cantBytes;
        nue->sig = nue->ant = nue;
        *p = nue;
        return TODO_BIEN;
    }

    tNodo *primero = *p;
    do {
        cmp = comparar(act->info, d);
        if (cmp == 0)
        {
            if(acumular)
                if(acumular(&act->info, &act->tamInfo, d, cantBytes) == 0)
                    return SIN_MEM;
            return CLA_DUP;
        }
        if (cmp > 0)
            break;
        act = act->sig;
    } while(act != primero);

    if((nue = (tNodo *)malloc(sizeof(tNodo))) == NULL) return SIN_MEM;
    if((nue->info = malloc(cantBytes)) == NULL) { free(nue); return SIN_MEM; }
    
    memcpy(nue->info, d, cantBytes);
    nue->tamInfo = cantBytes;

    tNodo *ant = act->ant;
    nue->sig = act;
    nue->ant = ant;
    ant->sig = nue;
    act->ant = nue;

    /* Si se insertó antes del primer elemento y es menor que este, actualizamos el puntero */
    if(act == *p && cmp > 0)
        *p = nue;

    return TODO_BIEN;
}

void ordenarLista(tLista *p, int (*comparar)(const void *, const void *))
{
    if(!*p || (*p)->sig == *p)
        return;
        
    int huboCambio;
    tNodo *act;
    do {
        huboCambio = 0;
        act = *p;
        do {
            if(comparar(act->info, act->sig->info) > 0)
            {
                void *auxInfo = act->info;
                unsigned auxTam = act->tamInfo;
                
                act->info = act->sig->info;
                act->tamInfo = act->sig->tamInfo;
                
                act->sig->info = auxInfo;
                act->sig->tamInfo = auxTam;
                huboCambio = 1;
            }
            act = act->sig;
        } while(act->sig != *p);
    } while(huboCambio);
}

int eliminarPorClave(tLista *p, void *d, unsigned cantBytes,
                     int (*comparar)(const void *, const void *))
{
    if(!*p)
        return 0;
        
    tNodo *act = *p;
    do {
        if(comparar(act->info, d) == 0)
        {
            memcpy(d, act->info, cantBytes);
            if(act->sig == act) /* Es el único nodo */
            {
                *p = NULL;
            }
            else
            {
                act->ant->sig = act->sig;
                act->sig->ant = act->ant;
                if(act == *p) /* Eliminamos el primero */
                    *p = act->sig;
            }
            free(act->info);
            free(act);
            return 1;
        }
        act = act->sig;
    } while(act != *p);

    return 0;
}
