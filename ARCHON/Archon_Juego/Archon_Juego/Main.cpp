#include <SFML/Graphics.hpp>
#include "Coordinador.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Archon Clash Royale");
    window.setFramerateLimit(60);

    Coordinador coordinador;
    coordinador.inicializar();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            coordinador.gestionarEventos(window, event);
        }

        if (coordinador.salir()) window.close();

        coordinador.actualizar(window);

        window.clear();
        coordinador.dibujar(window);
        window.display();
    }

    return 0;
}