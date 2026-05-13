#pragma once
#include <SFML/Graphics.hpp>

class CasillaArena {
private:
    sf::Sprite sprite;
    bool esTierra;

public:
    CasillaArena(float x, float y, float tam, sf::Texture& texCsped, sf::Texture& texTierra, bool _esTierra) {
        esTierra = _esTierra;

        if (esTierra) sprite.setTexture(texTierra);
        else sprite.setTexture(texCsped);

        sprite.setPosition(x, y);

        float scaleX = tam / sprite.getTexture()->getSize().x;
        float scaleY = tam / sprite.getTexture()->getSize().y;
        sprite.setScale(scaleX, scaleY);
    }

    void dibujar(sf::RenderWindow& window) {
        window.draw(sprite);
    }
};