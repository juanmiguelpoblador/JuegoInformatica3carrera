
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

enum class EstadoMenu {
    MENU_PRINCIPAL,
    JUGANDO,
    RANKING,
    SALIR
};

class Menu {
private:
    // Texturas para el efecto visual de "iluminar" botones
    sf::Texture texFondoNormal;
    sf::Texture texFondoJugar;
    sf::Texture texFondoRanking;
    sf::Sprite sprFondo;

    // Hitboxes: Áreas invisibles donde el usuario hace clic
    sf::FloatRect rectJugar;
    sf::FloatRect rectRanking;
    sf::FloatRect rectSalir;

public:
    Menu(float width, float height);

    // Carga las imágenes necesarias
    bool cargarRecursos();

    // Cambia la textura del fondo si el ratón está encima de un botón
    void actualizar(sf::RenderWindow& window);

    // Dibuja el menú
    void dibujar(sf::RenderWindow& window);

    // Procesa los clics y devuelve el nuevo estado del juego
    EstadoMenu procesarEventos(sf::RenderWindow& window, sf::Event& event);
};