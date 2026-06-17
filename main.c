#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "include/entidades.h"
#include "include/archivos.h"
#include "include/juego.h"
#include "include/menu.h"
#include "include/abb.h"

void jugar(tArchivos *archivosDelJuego, ArbolBin *indice, tLista *ranking);

int main()
{
    int opcion_elegida = 0;
    ArbolBin indice;
    tLista ranking;
    tArchivos archivosDelJuego;


    srand((unsigned)time(NULL));

    int arch = inicioAbrirArchivos(&archivosDelJuego);
    if (!arch) {
        puts("No se pudieron abrir los archivos de datos del juego.");
        return 1;
    }

    if(arch==1)
        IndexarArchivoOrdenado(archivosDelJuego.archIndice, ARCH_INDICE, &indice, sizeof(tIndice));
    else
        indexarArchivoJugadores(archivosDelJuego.archJug,&indice);

    crearRanking(&ranking, archivosDelJuego.archPart, &indice, archivosDelJuego.archJug);

    mostrarBienvenida();
    pausar_consola();
    limpiar_pantalla();

    do {
        mostrar_menu_principal();
        opcion_elegida = obtener_opcion(3);

        switch(opcion_elegida) {
            case 1:
                printf("Iniciando juego...\n");
                jugar(&archivosDelJuego, &indice, &ranking);
                break;

            case 2:
                printf("Mostrando ranking de jugadores...\n");
                mostrarListaRanking(&ranking);
                pausar_consola();
                break;

            case 3:
                printf("Saliendo del juego...\n");
                break;
        }
        limpiar_pantalla();
    } while(opcion_elegida != 3);

    vaciarLista(&ranking);
    liberarArbolBin(&indice);
    finCerrarArchivos(&archivosDelJuego);

    return 0;
}

void jugar(tArchivos *archivosDelJuego, ArbolBin *indice, tLista *ranking)
{
    tJuego juego;
    tConfiguracion cfg;
    tRegistroPartida reg;
    int resultado;

    if (!cargarConfiguracion("config.txt", &cfg)) {
        printf("No se pudo cargar config.txt. Revise que el archivo exista y sea valido.\n");
        pausar_consola();
        return;
    }

    if(cfg.mapaPregenerado!=1)
        mostrarConfiguracion(&cfg);
    else
        puts(">>Cargando Mapa Pregenerado...");
    inicializarJuego(&juego, &cfg);

    int opcion_elegida = 0;

    //Si la configuracion indica que el mapa esta pregenerado, lo carga
    if(cfg.mapaPregenerado == 1)
    {
        //Si no existe el mapa, pregunta si desea generar uno nuevo o volver
        if(cargarCaravana("caravana.txt", &juego)==0)
        {
            limpiar_pantalla();
            puts("No se pudo leer el tablero pregenerado.");
            puts("Desea:\n");
            puts("1: Generar un nuevo tablero.");
            puts("2: Volver al menu.");
            opcion_elegida = obtener_opcion(2);
            opcion_elegida++;
        }

        if(opcion_elegida == 0)
        {
            opcion_elegida=0;

            //Validar mapa pregenerado
            int tableroPre = verificarTablero(&juego.tablero, &juego.config);

            //Si el mapa no es complpetamente correcto, muestra este menu
            if(tableroPre != 1)
            {
                limpiar_pantalla();
                //Si el tablero es invalido:
                if(tableroPre == -1)
                {
                    puts("\nEl tablero pregenerado es invalido. Puede:");
                    puts("1: Generar un nuevo tablero.");
                    puts("2: Volver al menu.");
                    opcion_elegida = obtener_opcion(2);
                    opcion_elegida++;
                }

                //Si el tablero no esta balanceado
                if(tableroPre == 0)
                {
                    puts("\nEl tablero pregenerado no se encuentra balanceado, la experiencia puede que no sea lo mas satisfactoria.\nPuede\n");
                    puts("1: Continuar de todas formas.");
                    puts("2: Generar un nuevo tablero.");
                    puts("3: Volver al menu.");
                    opcion_elegida = obtener_opcion(3);
                }

            }
        }
    }

    if(opcion_elegida == 3)
    {
        puts("\nPor Favor, revisar el estado del mapa pregenerado, y asegurarse que respete el formato!");
        pausar_consola();
        return;
    }

    //Generar Tablero si lo indica la configuracion o se eligio generar un nuevo mapa
    if(cfg.mapaPregenerado == 0 || opcion_elegida == 2)
    {
        juego.config=cfg;
        do
        {
            generarTablero(&juego, &juego.config);
        }while(verificarTablero(&juego.tablero, &juego.config)!=1);
        guardarCaravana("caravana.txt", &juego);

    }

    iniciarPartida(&juego, archivosDelJuego->archJug, indice);
    limpiar_pantalla();

    mostrarReglas();
    printf("!Tablero listo!\nPosicion inicial: 1 (Inicio)\n");
    printf("Objetivo: Llegar a la posicion %d (Ciudad Refugio)\n\n", cfg.totalCasillas);
    pausar_consola();

    limpiar_pantalla();
    mostrarTablero(&juego.tablero);
    while (juego.juegoActivo) {
        resultado = ejecutarTurno(&juego);

        if (resultado != 1) {
            mostrarFinJuego(resultado, &juego);
            juego.juegoActivo = 0;
        }

        if (juego.juegoActivo) {
            pausar_consola();
            limpiar_pantalla();
            mostrarTablero(&juego.tablero);
        }
    }

    printf("\n  --- Resumen de Movimientos ---\n");
    mostrarColaMovimientos(&juego.colaMovimientosJugador);

    strncpy(reg.usuario, juego.jugador.usuario, MAX_NOMBRE - 1);
    reg.usuario[MAX_NOMBRE - 1] = '\0';
    reg.cantidadMovimientos = juego.totalMovimientos;
    reg.puntuacion = juego.jugador.puntos;

    fseek(archivosDelJuego->archPart, 0, SEEK_END);
    reg.idPartida = (int)(ftell(archivosDelJuego->archPart) / sizeof(tRegistroPartida)) + 1;
    actualizarRegistroPartidas(*archivosDelJuego, indice, ranking, &reg);

    archivarIndice(archivosDelJuego->archIndice, ARCH_INDICE, indice);

    liberarJuego(&juego);

    puts("\nPARTIDA FINALIZADA!");
    pausar_consola();
}
