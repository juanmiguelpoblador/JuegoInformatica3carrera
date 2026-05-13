#include "Coordinador.h"

Coordinador::Coordinador() : menu(1280, 720), estado(EstadoJuego::INICIO) {}

Coordinador::~Coordinador() {
    // Limpieza de memoria para evitar fugas
    for (auto& p : piezas) delete p;
    piezas.clear();
}

void Coordinador::inicializar() {
    menu.cargarRecursos();
}

void Coordinador::gestionarEventos(sf::RenderWindow& window, sf::Event& event) {
    if (estado == EstadoJuego::INICIO) {
        EstadoMenu res = menu.procesarEventos(window, event);
        if (res == EstadoMenu::JUGANDO) {
            estado = EstadoJuego::ARENA_COMBATE;

            // --- NUEVO: SPAWN DEL CABALLERO ---
            // Usamos el Grid System: Fila 4 (centro vertical), Columna 0 (izquierda)
            sf::Vector2f pos = arena.getCentroCasilla(4, 0);

            // Creamos la pieza y la guardamos en el vector gestor
            piezas.push_back(new Caballero(pos.x, pos.y, 1));
        }
    }
}

void Coordinador::actualizar(sf::RenderWindow& window) {
    if (estado == EstadoJuego::INICIO) {
        menu.actualizar(window);
    }
    else if (estado == EstadoJuego::ARENA_COMBATE) {
        // Actualizamos todas las piezas que existan en el vector
        for (auto& p : piezas) {
            p->actualizar();
        }
    }
}

void Coordinador::dibujar(sf::RenderWindow& window) {
    if (estado == EstadoJuego::INICIO) {
        menu.dibujar(window);
    }
    else if (estado == EstadoJuego::ARENA_COMBATE) {
        // 1. Dibujamos el fondo y el tablero
        arena.dibujar(window);

        // 2. Dibujamos todas las piezas (Caballeros, etc.) encima del tablero
        for (auto& p : piezas) {
            p->dibujar(window);
        }
    }
}

bool Coordinador::salir() {
    return false;
}