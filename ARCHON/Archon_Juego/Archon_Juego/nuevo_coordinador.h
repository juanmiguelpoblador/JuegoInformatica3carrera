#pragma once
#include "Menu.h"
#include "Arena.h"

class Coordinador {
private:
    EstadoMenu estado;
    Menu menu;
    Arena arena;

public:
    Coordinador() : estado(EstadoMenu::MENU_PRINCIPAL), menu(1280, 720) {}

    void inicializar() {
        menu.cargarRecursos();
    }

    void gestionarEventos(sf::RenderWindow& window, sf::Event& event) {
        if (estado == EstadoMenu::MENU_PRINCIPAL) {
            estado = menu.procesarEventos(window, event);
        }
        else if (estado == EstadoMenu::JUGANDO) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                estado = EstadoMenu::MENU_PRINCIPAL;
            }
        }
    }

    void actualizar(sf::RenderWindow& window) {
        if (estado == EstadoMenu::MENU_PRINCIPAL) {
            menu.actualizar(window);
        }
    }

    void dibujar(sf::RenderWindow& window) {
        if (estado == EstadoMenu::MENU_PRINCIPAL) {
            menu.dibujar(window);
        }
        else if (estado == EstadoMenu::JUGANDO) {
            arena.dibujar(window);
        }
    }

    bool salir() { return estado == EstadoMenu::SALIR; }
};
