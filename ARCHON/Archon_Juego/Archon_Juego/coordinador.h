#pragma once
#include <SFML/Graphics.hpp>
#include "Tablero.h"
#include "Arena.h"

class Coordinador {
private:
    enum Estado {
        INICIO,
        TURNO_AZUL,
        TURNO_ROJO,
        ARENA_COMBATE,
        FIN_PARTIDA
    } estado;

    Tablero tablero;

    // --- Arena de combate ---
    Arena*        arenaActual  = nullptr;   // arena activa (nullptr si no hay combate)
    Pieza*        ganadorArena = nullptr;   // resultado del combate
    sf::Clock     relojArena;              // delta time propio de la arena

public:
    Coordinador();
    ~Coordinador();

    void tecla(unsigned char key);
    void raton(int boton, int estado, int x, int y);
    void mueve(double dt);
    void dibuja() const;
};
