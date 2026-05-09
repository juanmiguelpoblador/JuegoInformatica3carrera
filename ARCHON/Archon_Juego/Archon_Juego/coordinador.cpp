#include "Coordinador.h"

Coordinador::Coordinador() {
    estado = INICIO; // Empezamos en la pantalla de título
}

void Coordinador::mueve(double dt) {
    switch (estado) {
    case INICIO:
        // Lógica de inicio (animaciones del menú, etc.)
        break;

    case TURNO_AZUL:
    case TURNO_ROJO:
        tablero.mueve(dt); // Actualiza animaciones de las piezas moviéndose

        // Comprobamos si las casillas deben cambiar de color (ciclo de oscilación)
        tablero.actualizarOscilacionCasillas();

        // 1. Detectar si hay victoria tras el último movimiento 
        if (tablero.comprobarVictoria()) {
            estado = FIN_PARTIDA;
        }
        // 2. Detectar si dos piezas han chocado en la misma casilla
        else if (tablero.hayCombatePendiente()) {
            estado = ARENA_COMBATE;
            // Aquí pasarías los datos de las tropas que van a pelear a la clase Arena
        }
        break;

    case ARENA_COMBATE:
        // Lógica de la arena de combate
        // Si el combate termina, evalúas quién ganó y devuelves el turno al jugador correspondiente
        break;

    case FIN_PARTIDA:
        // Detener el juego, mostrar rankings, etc.
        break;
    }
}

void Coordinador::dibuja() const {
    switch (estado) {
    case INICIO:
        // Dibujar menú inicial (ETSIDI::printxy, etc.) [cite: 1147, 1148, 1149]
        break;
    case TURNO_AZUL:
    case TURNO_ROJO:
        tablero.dibuja(); // Dibuja la matriz 9x9 y las tropas
        // Opcional: Dibujar un indicador visual de de quién es el turno
        break;
    case ARENA_COMBATE:
        // Dibujar la arena
        break;
    case FIN_PARTIDA:
        // Dibujar pantalla de Game Over / Victoria
        break;
    }
}