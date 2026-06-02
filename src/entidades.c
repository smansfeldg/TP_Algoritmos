#define _CRT_SECURE_NO_WARNINGS

#include "../include/entidades.h"

#ifndef _WIN32
#include <strings.h>
#define strcmpi strcasecmp
#endif

static int claveIgual(const char *clave, const char *esperada)
{
#ifdef _WIN32
    return _stricmp(clave, esperada) == 0;
#else
    return strcmpi(clave, esperada) == 0;
#endif
}

static void actualizarNormalCasilla(tCasilla *casilla)
{
    if (!casilla) return;
    // Si no tiene ninguna caracteristica especial, es normal.
    casilla->normal = !(casilla->inicio || casilla->refugio || casilla->premios ||
                        casilla->vidas || casilla->oasis || casilla->tormenta ||
                        casilla->jugador || casilla->bandidos);
}

void crearJugador(tJugador *j, const char *nombre, int posicionInicial, int vidas)
{
    // Se inicializa el estado base del jugador al comenzar o retomar una partida.
    strncpy(j->nombre, nombre, MAX_NOMBRE - 1);
    j->nombre[MAX_NOMBRE - 1] = '\0';
    j->posicion = posicionInicial;
    j->vidas = vidas;
    j->puntos = 0;
    j->protegidoOasis = 0;
    j->perdidoTurno = 0;
    j->idJugador = 0;
}

void crearBandido(tBandido *b, int id, int posicion)
{
    b->id = id;
    b->posicion = posicion;
    b->activo = 1;
}

void inicializarConfiguracion(tConfiguracion *cfg)
{
    cfg->totalCasillas = 30;
    cfg->vidasIniciales = 3;
    cfg->cantidadBandidos = 3;
    cfg->cantidadPremios = 4;
    cfg->cantidadVidas = 2;
    cfg->cantidadOasis = 2;
    cfg->cantidadTormentas = 3;
}

int cargarConfiguracion(const char *archivo, tConfiguracion *cfg)
{
    FILE *arch;

    inicializarConfiguracion(cfg);
    if (abrirArchivo(&arch, archivo, "rt", 0) != OK) {
        return 0;
    }

    leerArchivoTxt(arch, cfg, sizeof(tConfiguracion), sizeof(char) * 80, trozarConfig);
    fclose(arch);

    return cfg->totalCasillas >= 3 && cfg->vidasIniciales > 0;
}

void trozarConfig(void *config, const void *dato)
{
    char *linea = (char *)dato;
    tConfiguracion *c = (tConfiguracion *)config;
    char parametro[50];
    int valor;

    if (sscanf(linea, " %49[^: ] %*[: ] %d", parametro, &valor) != 2) {
        return;
    }

    if (claveIgual(parametro, "TOTAL_CASILLAS") || claveIgual(parametro, "cantidad_posiciones")) {
        c->totalCasillas = valor;
    } else if (claveIgual(parametro, "VIDAS_INICIALES") || claveIgual(parametro, "vidas_inicio")) {
        c->vidasIniciales = valor;
    } else if (claveIgual(parametro, "CANTIDAD_BANDIDOS") || claveIgual(parametro, "maximo_bandidos")) {
        c->cantidadBandidos = valor;
    } else if (claveIgual(parametro, "CANTIDAD_PREMIOS") || claveIgual(parametro, "maximo_premios")) {
        c->cantidadPremios = valor;
    } else if (claveIgual(parametro, "CANTIDAD_VIDAS") || claveIgual(parametro, "maximo_vidas_extra")) {
        c->cantidadVidas = valor;
    } else if (claveIgual(parametro, "CANTIDAD_OASIS") || claveIgual(parametro, "maximo_oasis")) {
        c->cantidadOasis = valor;
    } else if (claveIgual(parametro, "CANTIDAD_TORMENTAS") || claveIgual(parametro, "maximo_tormentas")) {
        c->cantidadTormentas = valor;
    }
}

void mostrarConfiguracion(const tConfiguracion *cfg)
{
    printf("Los parametros de configuracion son:\n");
    printf("Cantidad de Casillas del Tablero: %d\n", cfg->totalCasillas);
    printf("Cantidad de Oasis:              %d\n", cfg->cantidadOasis);
    printf("Cantidad de Premios:            %d\n", cfg->cantidadPremios);
    printf("Cantidad de Tormentas:          %d\n", cfg->cantidadTormentas);
    printf("Cantidad de Vidas Extra:        %d\n", cfg->cantidadVidas);
    printf("Cantidad de Vida Inicial:       %d\n", cfg->vidasIniciales);
    printf("Cantidad de Bandidos:           %d\n", cfg->cantidadBandidos);
}

int generarTablero(tJuego *juego, const tConfiguracion *cfg)
{
    tBandido bandidoAux;
    tCasilla *casillas;
    int i;
    int c;

    casillas = (tCasilla *)calloc(cfg->totalCasillas, sizeof(tCasilla));
    if (!casillas) return 0;

    for (i = 0; i < cfg->totalCasillas; i++) {
        casillas[i].posicion = i + 1;
        casillas[i].normal = 1;
    }

    casillas[0].inicio = 1;
    casillas[0].jugador = 1;
    casillas[0].normal = 0;
    casillas[cfg->totalCasillas - 1].refugio = 1;
    casillas[cfg->totalCasillas - 1].normal = 0;

    for (c = 0; c < cfg->cantidadOasis; c++) {
        int pos = (rand() % (cfg->totalCasillas - 2)) + 1;
        casillas[pos].oasis = 1;
        actualizarNormalCasilla(&casillas[pos]);
    }

    for (c = 0; c < cfg->cantidadPremios; c++) {
        int pos = (rand() % (cfg->totalCasillas - 2)) + 1;
        casillas[pos].premios++;
        actualizarNormalCasilla(&casillas[pos]);
    }

    for (c = 0; c < cfg->cantidadTormentas; c++) {
        int pos = (rand() % (cfg->totalCasillas - 2)) + 1;
        casillas[pos].tormenta = 1;
        actualizarNormalCasilla(&casillas[pos]);
    }

    for (c = 0; c < cfg->cantidadVidas; c++) {
        int pos = (rand() % (cfg->totalCasillas - 2)) + 1;
        casillas[pos].vidas++;
        actualizarNormalCasilla(&casillas[pos]);
    }

    for (c = 0; c < cfg->cantidadBandidos; c++) {
        int pos = (rand() % (cfg->totalCasillas - 2)) + 1;
        casillas[pos].bandidos++;
        actualizarNormalCasilla(&casillas[pos]);
        crearBandido(&bandidoAux, c + 1, pos + 1);
        insertarAlFinal(&juego->bandidos, &bandidoAux, sizeof(tBandido));
    }

    for (i = 0; i < cfg->totalCasillas; i++) {
        insertarAlFinal(&juego->tablero, &casillas[i], sizeof(tCasilla));
    }

    free(casillas);
    return 1;
}

tCasilla *buscarCasilla(const tLista *tablero, int posicion, int (*cmp)(const void *, const void *))
{
    tCasilla *ret = NULL;
    tCasilla aBuscar;

    aBuscar.posicion = posicion;
    buscarElementoLista(tablero, (void **)&ret, &aBuscar, cmp);
    return ret;
}

int moverJugador(tJuego *juego, tMovimiento movimiento, tCasilla *casillaActual)
{
    int nuevaPos;

    // Se limpia la casilla anterior antes de calcular el nuevo destino.
    if (casillaActual) {
        casillaActual->jugador = 0;
        actualizarNormalCasilla(casillaActual);
    }

    // El jugador avanza o retrocede la cantidad exacta que marco el dado.
    nuevaPos = juego->jugador.posicion;
    if (movimiento.direccion == 'F') {
        nuevaPos += movimiento.pasos;
        printf("El jugador se movio %d casillas hacia adelante\n", movimiento.pasos);
    } else {
        nuevaPos -= movimiento.pasos;
        printf("El jugador se movio %d casillas hacia atras\n", movimiento.pasos);
    }

    /* Si el destino se pasa de los limites, se aplica rebote para mantenerlo dentro del tablero. */
    while (nuevaPos > juego->config.totalCasillas || nuevaPos < 1) {
        if (nuevaPos > juego->config.totalCasillas) {
            nuevaPos = juego->config.totalCasillas - (nuevaPos - juego->config.totalCasillas);
        }
        if (nuevaPos < 1) {
            nuevaPos = 1 + (1 - nuevaPos);
        }
    }

    return posicionarJugador(juego, nuevaPos);
}

int aplicarEfectoCasilla(tJugador *j, tCasilla *casilla)
{
    int obtuvoOasis = 0;

    // La Ciudad Refugio termina la partida de forma inmediata.
    if (casilla->refugio) {
        return 1;
    }

    // El oasis da proteccion temporal contra la proxima tormenta o intercepcion.
    if (casilla->oasis) {
        j->protegidoOasis = 1;
        obtuvoOasis = 1;
        puts("El jugador descansa en un oasis y estara protegido hasta su proximo turno.");
        casilla->oasis = 0;
    }

    // La tormenta puede quitar la proteccion o forzar a perder un turno.
    if (casilla->tormenta) {
        if (j->protegidoOasis && !obtuvoOasis) {
            j->protegidoOasis = 0;
            puts("El jugador fue protegido de la tormenta por el oasis.");
        } else if (obtuvoOasis) {
            puts("El oasis protege al jugador de la tormenta y conserva la proteccion para el proximo turno.");
        } else {
            puts("El jugador pierde el proximo turno debido a una tormenta de arena.");
            j->perdidoTurno = 1;
        }
        casilla->tormenta = 0;
    }

    // Las casillas de vida suman vidas al contador del jugador.
    if (casilla->vidas) {
        printf("El jugador gano %u vidas\n", casilla->vidas);
        j->vidas += (int)casilla->vidas;
        casilla->vidas = 0;
    }

    // Los premios suman puntos para el resultado final.
    if (casilla->premios) {
        printf("El jugador encontro %u premios\n", casilla->premios);
        j->puntos += (int)casilla->premios;
        casilla->premios = 0;
    }

    actualizarNormalCasilla(casilla);
    return 0;
}

void inicializarJuego(tJuego *juego, tConfiguracion *cfg)
{
    // Se copia la configuracion y se construyen las estructuras necesarias para jugar.
    juego->config = *cfg;
    crearLista(&juego->tablero);
    CrearCola(&juego->colaMovimientos);
    CrearCola(&juego->colaMovimientosJugador);
    crearLista(&juego->bandidos);
    juego->totalMovimientos = 0;
    juego->juegoActivo = 1;
    juego->turnoActual = 1;

    generarTablero(juego, cfg);
}

void liberarJuego(tJuego *juego)
{
    vaciarLista(&juego->tablero);
    vaciarLista(&juego->bandidos);
    VaciarCola(&juego->colaMovimientos);
    VaciarCola(&juego->colaMovimientosJugador);
}

int encolarMovimiento(tCola *cola, tMovimiento movimiento)
{
    return PonerEnCola(cola, &movimiento, sizeof(tMovimiento));
}

int desencolarMovimiento(tCola *cola, tMovimiento *mov)
{
    return SacarDeCola(cola, mov, sizeof(tMovimiento));
}

void mostrarColaMovimientos(tCola *cola)
{
    tMovimiento mov;

    puts("Movimientos realizados por el jugador:");
    while (!ColaVacia(cola)) {
        desencolarMovimiento(cola, &mov);
        printf("%c%d\n", mov.direccion, mov.pasos);
    }
}

int moverBandido(tJuego *juego, tBandido *b, tMovimiento movimiento, tCasilla *casillaActual)
{
    int nuevaPos = b->posicion;

    /*
      Primero se "saca" al bandido de la casilla actual para que el tablero
      no quede con un contador desactualizado mientras se calcula el destino.
     */
    if (casillaActual && casillaActual->bandidos > 0) {
        casillaActual->bandidos--;
        actualizarNormalCasilla(casillaActual);
    }

    // La direccion viene decidida por accionarBandido: F avanza, B retrocede. 
    if (movimiento.direccion == 'F') {
        nuevaPos += movimiento.pasos;
    } else {
        nuevaPos -= movimiento.pasos;
    }

    /*
      El tablero es circular, asi que si el bandido se pasa de los extremos
      se lo hace reingresar por el otro lado hasta quedar dentro de rango.
     */
    while (nuevaPos < 1) {
        nuevaPos += juego->config.totalCasillas;
    }
    while (nuevaPos > juego->config.totalCasillas) {
        nuevaPos -= juego->config.totalCasillas;
    }

    // Se actualiza la posicion y se marca la nueva casilla ocupada por el bandido.
    b->posicion = nuevaPos;
    casillaActual = buscarCasilla(&juego->tablero, b->posicion, cmpPosCasillas);
    if (casillaActual) {
        casillaActual->bandidos++;
        actualizarNormalCasilla(casillaActual);
    }

    // Si termina en la misma casilla que el jugador, se resuelve la intercepcion.
    return verificarColision(juego, b, casillaActual);
}

int verificarColision(tJuego *juego, tBandido *b, tCasilla *casillaActual)
{
    // Solo hay choque real si el bandido sigue activo y coincide con la posicion del jugador.
    if (!(b && b->activo && juego->jugador.posicion == b->posicion)) {
        return 0;
    }

    puts("El jugador fue interceptado por un bandido.");
    // El oasis puede anular la primera intercepcion sin perder una vida.
    if (juego->jugador.protegidoOasis) {
        puts("Pero fue protegido por el oasis.");
        juego->jugador.protegidoOasis = 0;
        return 1;
    }

    // Si no hay proteccion, el bandido queda desactivado y el jugador pierde una vida.
    b->activo = 0;
    if (casillaActual && casillaActual->bandidos > 0) {
        casillaActual->bandidos--;
        actualizarNormalCasilla(casillaActual);
    }

    if (casillaActual) {
        casillaActual->jugador = 0;
        actualizarNormalCasilla(casillaActual);
    }

    juego->jugador.vidas -= 1;
    if (juego->jugador.vidas <= 0) {
        return -1;
    }

    // El jugador vuelve al campamento inicial y se revisan colisiones en esa casilla.
    if (!posicionarJugador(juego, 1)) {
        return -1;
    }

    puts("El jugador pierde una vida y vuelve al Campamento Inicial.");
    return 1;
}

int verificarDerrota(const tJugador *j)
{
    return j->vidas <= 0;
}

void guardarCaravana(const char *archivo, const tJuego *juego)
{
    FILE *f = fopen(archivo, "wt");
    tNodoListaC *act;

    if (!f || !juego || !juego->tablero) {
        if (f) fclose(f);
        return;
    }

    fprintf(f, "CARAVANA DEL DESIERTO\n");
    fprintf(f, "Posiciones: %d | Vidas: %d | Bandidos: %d\n\n",
            juego->config.totalCasillas,
            juego->config.vidasIniciales,
            juego->config.cantidadBandidos);

    act = juego->tablero;
    do {
        tCasilla *c = (tCasilla *)act->info;
        fprintf(f, "%02d:[", c->posicion);
        if (c->inicio) fprintf(f, "I ");
        if (c->refugio) fprintf(f, "S ");
        if (c->oasis) fprintf(f, "O ");
        if (c->tormenta) fprintf(f, "T ");
        if (c->vidas) fprintf(f, "%uV ", c->vidas);
        if (c->premios) fprintf(f, "%uP ", c->premios);
        if (c->bandidos) fprintf(f, "%uB ", c->bandidos);
        if (c->jugador) fprintf(f, "J ");
        if (c->normal) fprintf(f, ".");
        fprintf(f, "]\n");
        act = act->sig;
    } while (act != juego->tablero);

    fprintf(f, "\nReferencias: J jugador, B bandido, I inicio, S refugio, P premio, V vida, O oasis, T tormenta, . ruta.\n");
    fclose(f);
}

int cargarCaravana(const char *archivo, tJuego *juego)
{
    (void)archivo;
    (void)juego;
    return 1;
}

void comprobarColisionesJB(void *bandido, void *contexto)
{
    tBandido *bandidoActual = (tBandido *)bandido;
    tJuego *juego = (tJuego *)contexto;
    tCasilla *casillaActual = buscarCasilla(&juego->tablero, bandidoActual->posicion, cmpPosCasillas);

    if (verificarColision(juego, bandidoActual, casillaActual) == -1) {
        juego->juegoActivo = 0;
    }
}

int posicionarJugador(tJuego *juego, int posicion)
{
    tCasilla *casillaActual;

    // La nueva posicion se guarda en el estado del jugador.
    juego->jugador.posicion = posicion;
    casillaActual = buscarCasilla(&juego->tablero, juego->jugador.posicion, cmpPosCasillas);
    if (!casillaActual) {
        return 0;
    }

    // Se marca la casilla ocupada por el jugador y se recalcula su tipo.
    casillaActual->jugador = 1;
    actualizarNormalCasilla(casillaActual);

    // Tras mover al jugador, se revisa si algun bandido quedo en la misma casilla.
    recorrerListaYAccionar(&juego->bandidos, juego, comprobarColisionesJB);

    return juego->juegoActivo;
}

int cmpPosCasillas(const void *a, const void *b)
{
    const tCasilla *cas1 = (const tCasilla *)a;
    const tCasilla *cas2 = (const tCasilla *)b;
    return cas1->posicion - cas2->posicion;
}

int IndexarArchivo(FILE *archIndx, char *nombreArch, ArbolBin *arbolIndx, unsigned tam)
{
    tIndice auxIndx;
    (void)archIndx;

    crearArbolBin(arbolIndx);
    if (abrirArchivo(&archIndx, nombreArch, "rb", 0) == 0) {
        return 0;
    }

    while (fread(&auxIndx, tam, 1, archIndx) == 1) {
        insertarNodoArbolBin(arbolIndx, &auxIndx, sizeof(tIndice), cmpIndxNombre);
    }

    return cerrarArchivo(&archIndx, nombreArch, 0);
}

void actualizarJugadores(FILE *archJug, ArbolBin *indice, const tJugador *nuevo)
{
    tRegistroJugador auxReg;
    tIndice aux;

    auxReg.idJugador = nuevo->idJugador;
    strcpy(auxReg.nombre, nuevo->nombre);

    escribirNuevoReg(archJug, &auxReg, sizeof(tRegistroJugador));
    fflush(archJug);

    strcpy(aux.nombre, nuevo->nombre);
    aux.registro = (unsigned)nuevo->idJugador;

    insertarNodoArbolBin(indice, &aux, sizeof(tIndice), cmpIndxNombre);
}

int cmpIndxNombre(const void *a, const void *b)
{
    const tIndice *indx1 = (const tIndice *)a;
    const tIndice *indx2 = (const tIndice *)b;

    return strcmp(indx1->nombre, indx2->nombre);
}

int archivarIndice(FILE *archIndx, char *nombreArch, ArbolBin *arbolIndx)
{
    (void)archIndx;

    if (abrirArchivo(&archIndx, nombreArch, "wb", 0) == 0) {
       return 0;
    }

    recorrerEnOrdenArbolBin(arbolIndx, 0, archIndx, guardNodoIndxEnArchivo);
    liberarArbolBin(arbolIndx);

    return cerrarArchivo(&archIndx, nombreArch, 0);
}

void guardNodoIndxEnArchivo(void *dato, size_t tam, unsigned nivel, void *params)
{
    FILE *arch = (FILE *)params;
    (void)nivel;
    fwrite(dato, tam, 1, arch);
}

int buscarJugador(char *nombre, ArbolBin *indice, cmp Cmp)
{
    tIndice buscado;

    strcpy(buscado.nombre, nombre);
    buscado.registro = 0;
    return buscarNodoArbolBin(indice, &buscado, Cmp) != NULL;
}

int crearRanking(tLista *ranking, FILE *archPart, ArbolBin *indice, FILE *archJug)
{
    (void)indice;
    (void)archJug;
    crearLista(ranking);
    leerArchivoBin(archPart, ranking, sizeof(tRegistroPartida), cargarRanking);
    return 1;
}

void cargarRanking(void *ranking, const void *dato)
{
    const tRegistroPartida *partida = (const tRegistroPartida *)dato;
    tRegistroRanking reg;

    strcpy(reg.nombre, partida->nombre);
    reg.idJugador = 0;
    reg.cantidadPartidas = 1;
    reg.puntuacionTotal = partida->puntuacion;
    insertarEnOrden(ranking, &reg, sizeof(tRegistroRanking), cmpRankNombres, sumarRankPuntos);
}

int cmpRankNombres(const void *a, const void *b)
{
    const tRegistroRanking *reg1 = (const tRegistroRanking *)a;
    const tRegistroRanking *reg2 = (const tRegistroRanking *)b;

    return strcmp(reg1->nombre, reg2->nombre);
}

int sumarRankPuntos(void **dato1, unsigned *tam1, const void *dato2, unsigned tam2)
{
    tRegistroRanking **reg1 = (tRegistroRanking **)dato1;
    const tRegistroRanking *reg2 = (const tRegistroRanking *)dato2;
    (void)tam1;
    (void)tam2;

    (*reg1)->puntuacionTotal += reg2->puntuacionTotal;
    (*reg1)->cantidadPartidas++;

    return 1;
}

int cmpRankPuntos(const void *a, const void *b)
{
    const tRegistroRanking *reg1 = (const tRegistroRanking *)a;
    const tRegistroRanking *reg2 = (const tRegistroRanking *)b;

    return reg2->puntuacionTotal - reg1->puntuacionTotal;
}

int actualizarRegistroPartidas(FILE *arch, tLista *ranking, const tRegistroPartida *partida)
{
    tRegistroRanking nuevo;

    escribirNuevoReg(arch, partida, sizeof(tRegistroPartida));
    fflush(arch);

    nuevo.idJugador = 0;
    strcpy(nuevo.nombre, partida->nombre);
    nuevo.cantidadPartidas = 1;
    nuevo.puntuacionTotal = partida->puntuacion;
    insertarEnOrden(ranking, &nuevo, sizeof(tRegistroRanking), cmpRankNombres, sumarRankPuntos);

    return 1;
}

void mostrarListaRanking(tLista *ranking)
{
    if (listaVacia(ranking)) {
        puts("\nTodavia no hay partidas registradas.");
        return;
    }

    ordenarLista(ranking, cmpRankPuntos);
    printf("\nRANKING DE JUGADORES:\n");
    printf("---\n");
    mostrarDeIzqADer(ranking, NULL, mostrarRanking);
    printf("\n---\n");
    ordenarLista(ranking, cmpRankNombres);
}

void mostrarRanking(const void *reg, void *param)
{
    const tRegistroRanking *ranking = (const tRegistroRanking *)reg;
    (void)param;
    printf("%s | Puntos: %d | Cantidad de Partidas: %d\n",
           ranking->nombre,
           ranking->puntuacionTotal,
           ranking->cantidadPartidas);
}

int inicioAbrirArchivos(tArchivos *archivos)
{
    if (abrirArchivo(&archivos->archJug, ARCH_JUGADORES, "r+b", 0) == 0 &&
        abrirArchivo(&archivos->archJug, ARCH_JUGADORES, "w+b", 0) == 0) {
        return 0;
    }

    if (abrirArchivo(&archivos->archPart, ARCH_PARTIDAS, "r+b", 0) == 0 &&
        abrirArchivo(&archivos->archPart, ARCH_PARTIDAS, "w+b", 0) == 0) {
        cerrarArchivo(&archivos->archJug, ARCH_JUGADORES, 0);
        return 0;
    }

    archivos->archIndice = NULL;
    return 1;
}

int finCerrarArchivos(tArchivos *archivos)
{
    if (cerrarArchivo(&archivos->archJug, ARCH_JUGADORES, 0) == 0) {
        return 0;
    }
    if (cerrarArchivo(&archivos->archPart, ARCH_PARTIDAS, 0) == 0) {
        return 0;
    }

    return 1;
}

void mostrarIndx(void *dato, size_t tam, unsigned nivel, void *params)
{
    tIndice *indx = (tIndice *)dato;
    (void)tam;
    (void)nivel;
    (void)params;
    printf("Nombre: %s | Registro: %u\n", indx->nombre, indx->registro);
}

void mostrarPartidas(void *a, const void *b)
{
    const tRegistroPartida *p = (const tRegistroPartida *)b;
    (void)a;
    printf("Id:%d | Nombre: %s | Puntos: %d | Mov: %d\n",
           p->idPartida, p->nombre, p->puntuacion, p->cantidadMovimientos);
}

void mostrarJugadores(void *a, const void *b)
{
    const tRegistroJugador *j = (const tRegistroJugador *)b;
    (void)a;
    printf("Codigo: %d | Nombre: %s\n", j->idJugador, j->nombre);
}
