#include "Menu.h"
#include <iostream>

Menu::Menu(float width, float height) {
    // Definimos las zonas de clic 
    // Formato: (X, Y, Ancho, Alto)
    rectJugar = sf::FloatRect(400, 275, 480, 80);
    rectRanking = sf::FloatRect(400, 360, 480, 80);
    rectSalir = sf::FloatRect(400, 440, 480, 80);
}

bool Menu::cargarRecursos() {
    if (!texFondoNormal.loadFromFile("assets/menu_normal.png") ||
        !texFondoJugar.loadFromFile("assets/menu_jugar_on.png") ||
        !texFondoRanking.loadFromFile("assets/menu_ranking_on.png")) {
        return false;
    }
    sprFondo.setTexture(texFondoNormal);
    //Ajustar el tamaño
    sf::Vector2u textureSize = texFondoNormal.getSize();
    float scaleX = 1280.0f / textureSize.x;
    float scaleY = 720.0f / textureSize.y;

    sprFondo.setScale(scaleX, scaleY);
    return true;
}

void Menu::actualizar(sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mouseCoords = window.mapPixelToCoords(mousePos);

    // Efecto visual: Cambiar fondo según la posición del ratón
    if (rectJugar.contains(mouseCoords)) {
        sprFondo.setTexture(texFondoJugar);
    }
    else if (rectRanking.contains(mouseCoords)) {
        sprFondo.setTexture(texFondoRanking);
    }
    else {
        sprFondo.setTexture(texFondoNormal);
    }
}

EstadoMenu Menu::procesarEventos(sf::RenderWindow& window, sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

        if (rectJugar.contains(mousePos))   return EstadoMenu::JUGANDO;
        if (rectRanking.contains(mousePos)) return EstadoMenu::RANKING;
        if (rectSalir.contains(mousePos))   return EstadoMenu::SALIR;
    }
    return EstadoMenu::MENU_PRINCIPAL;
}

void Menu::dibujar(sf::RenderWindow& window) {
    window.draw(sprFondo);
}