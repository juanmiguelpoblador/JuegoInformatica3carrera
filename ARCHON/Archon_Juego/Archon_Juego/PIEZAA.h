#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Pieza {
protected:
    float x, y;
    int bando;
    int vida; // Añadimos vida para que 'curar' tenga sentido
    sf::Sprite sprite;
    sf::Texture textura;

public:
    Pieza(float _x, float _y, int _bando) : x(_x), y(_y), bando(_bando), vida(100) {}
    virtual ~Pieza() {}

    virtual void actualizar() = 0;

    // --- EL FIX PARA EL ERROR LNK2019 ---
    // Añadimos el cuerpo de la función curar aquí mismo
    void curar(int puntos) {
        vida += puntos;
        if (vida > 100) vida = 100; // Límite de vida opcional
    }

    void cargarImagen(std::string ruta) {
        if (textura.loadFromFile(ruta)) {
            sprite.setTexture(textura);
            sf::FloatRect bounds = sprite.getLocalBounds();
            sprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
            sprite.setPosition(x, y);
        }
    }

    void dibujar(sf::RenderWindow& window) {
        sprite.setPosition(x, y);
        window.draw(sprite);
    }
};