#pragma once
#include "Tablero.h"

class Coordinador
{
private:
    // Definimos los estados principales del juego
    enum Estado {
        INICIO,
        TURNO_AZUL,      // Turno del Jugador 1 (ej. Rey Azul)
        TURNO_ROJO,      // Turno del Jugador 2 (ej. Rey Rojo)
        ARENA_COMBATE,   // Pantalla de lucha en tiempo real
        FIN_PARTIDA
    } estado;

    Tablero tablero;     // El objeto que contendrá la matriz 9x9 y las tropas

public:
    Coordinador();
    void tecla(unsigned char key);
    void raton(int boton, int estado, int x, int y); // Para interactuar con el ratón
    void mueve(double dt);
    void dibuja() const;
};