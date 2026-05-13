#pragma once
#include <vector>
#include "Menu.h"
#include "Arena.h"
#include "Caballero.h"

enum class EstadoJuego { INICIO, ARENA_COMBATE, SALIR };

class Coordinador {
private:
    EstadoJuego estado;
    Menu menu;
    Arena arena;
    std::vector<Pieza*> piezas; // Gestor de entidades

public:
    Coordinador();
    ~Coordinador();
    void inicializar();
    void gestionarEventos(sf::RenderWindow& window, sf::Event& event);
    void actualizar(sf::RenderWindow& window);
    void dibujar(sf::RenderWindow& window);
    bool salir();
};