/**
 * @file menu.h
 * @brief Abstracción del menú y funciones de interacción genéricas.
 *
 * Capa de Presentación. Contiene funciones genéricas para mostrar
 * opciones, pedir datos al usuario y manejar el flujo de menú.
 */

#ifndef MENU_H
#define MENU_H

/**
 * @brief Muestra el menú principal de la aplicación.
 */
void mostrar_menu_principal();

/**
 * @brief Lee la opción seleccionada por el usuario.
 * 
 * @param max_opcion La opción máxima válida.
 * @return La opción elegida (entero válido).
 */
int obtener_opcion(int max_opcion);

/**
 * @brief Limpia la pantalla de la consola (genérico).
 */
void limpiar_pantalla();

/**
 * @brief Pausa la ejecución hasta que el usuario presione una tecla.
 */
void pausar_consola();

#endif // MENU_H
