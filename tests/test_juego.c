#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "../include/juego.h"
#include "../include/entidades.h"

// ==========================================
// Tests de Interacciones con Casillas
// ==========================================
void test_aplicar_efecto_premio() {
    tJugador j;
    crearJugador(&j, "TestUser", 1, 3);
    j.puntos = 0;
    
    aplicarEfectoCasilla(&j, TIPO_PREMIO); // Incrementa o da puntos
    
    // Asumiendo que el premio es positivo (puedes ajustar el valor esperado según tu lógica exacta)
    assert(j.puntos > 0);
    printf("[OK] test_aplicar_efecto_premio\n");
}

void test_aplicar_efecto_vida() {
    tJugador j;
    crearJugador(&j, "TestUser", 1, 3);
    
    aplicarEfectoCasilla(&j, TIPO_VIDA); 
    
    assert(j.vidas == 4);
    printf("[OK] test_aplicar_efecto_vida\n");
}

void test_aplicar_efecto_oasis() {
    tJugador j;
    crearJugador(&j, "TestUser", 1, 3);
    j.protegidoOasis = 0;
    j.perdidoTurno = 0;
    
    aplicarEfectoCasilla(&j, TIPO_OASIS); 
    
    assert(j.protegidoOasis == 1);
    printf("[OK] test_aplicar_efecto_oasis\n");
}

void test_aplicar_efecto_tormenta() {
    tJugador j;
    
    // CASO 1: desprotegido pierdes el turno
    crearJugador(&j, "TestUser", 1, 3);
    aplicarEfectoCasilla(&j, TIPO_TORMENTA);
    assert(j.perdidoTurno == 1);
    
    // CASO 2: protegido NO pierdes el turno pero consumes oasis
    crearJugador(&j, "TestUser", 1, 3);
    j.protegidoOasis = 1;
    aplicarEfectoCasilla(&j, TIPO_TORMENTA);
    assert(j.perdidoTurno == 0);
    assert(j.protegidoOasis == 0);
    
    printf("[OK] test_aplicar_efecto_tormenta\n");
}

// ==========================================
// Tests de Colisiones con Bandidos
// ==========================================
void test_verificar_colision_positiva() {
    tJugador j;
    tBandido b;
    
    crearJugador(&j, "TestUser", 5, 3);
    crearBandido(&b, 1, 5); // Misma posición
    
    tJuego juego;
    crearLista(&juego.bandidos);
    juego.jugador = j;
    int choque = verificarColision(&juego, &b);
    assert(choque == 1);
    printf("[OK] test_verificar_colision_positiva\n");
}

void test_verificar_colision_negativa() {
    tJugador j;
    tBandido b;
    
    crearJugador(&j, "TestUser", 5, 3);
    crearBandido(&b, 1, 6); // Posición distinta
    
    tJuego juego;
    crearLista(&juego.bandidos);
    juego.jugador = j;
    int choque = verificarColision(&juego, &b);
    assert(choque == 0);
    printf("[OK] test_verificar_colision_negativa\n");
}

// Se eliminaron tests de procesarColision porque la API fue delegada a otras capas.

// ==========================================
// Tests Lógica de Fin de Partida
// ==========================================
void test_verificar_derrota() {
    tJugador j;
    crearJugador(&j, "Muerto", 5, 0); // 0 vidas
    
    assert(verificarDerrota(&j) == 1);
    printf("[OK] test_verificar_derrota\n");
}

int main() {
    printf("Iniciando Tests...\n");
    printf("---------------------------\n");
    
    // Ejecutar pruebas
    test_aplicar_efecto_premio();
    test_aplicar_efecto_vida();
    test_aplicar_efecto_oasis();
    test_aplicar_efecto_tormenta();
    
    test_verificar_colision_positiva();
    test_verificar_colision_negativa();
    
    test_verificar_derrota();
    
    printf("---------------------------\n");
    printf("TODOS LOS TESTS PASARON EXITOSAMENTE!\n");
    
    return 0;
}
