#pragma once
#include <SFML/Graphics.hpp>

class Casilla {
private:
    sf::Sprite sprite;
    bool esTierra;

public:
    // Pasamos las texturas por referencia para no recargarlas 81 veces (optimización)
    Casilla(float x, float y, float tam, sf::Texture& texCsped, sf::Texture& texTierra, bool _esTierra) {
        esTierra = _esTierra;

        if (esTierra) sprite.setTexture(texTierra);
        else sprite.setTexture(texCsped);

        sprite.setPosition(x, y);

        // Escalamos la imagen al tamaño de la casilla (ej: 70x70)
        float scaleX = tam / sprite.getTexture()->getSize().x;
        float scaleY = tam / sprite.getTexture()->getSize().y;
        sprite.setScale(scaleX, scaleY);
    }

    void dibujar(sf::RenderWindow& window) {
        window.draw(sprite);
    }
};