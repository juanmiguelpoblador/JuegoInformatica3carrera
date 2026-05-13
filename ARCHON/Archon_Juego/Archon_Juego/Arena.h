#pragma once
#include "Casilla.h"
#include <vector>

class Arena {
private:
    std::vector<CasillaArena*> tablero;
    sf::Texture texAzul, texRojo, texFondo;
    sf::Sprite sprFondo;

    const int DIM = 9;
    const float TAM_CASILLA = 50.0f;
    float inicioX, inicioY;

public:
    Arena() {
        // 1. Carga de recursos
        texAzul.loadFromFile("assets/tile_azul.png");
        texRojo.loadFromFile("assets/tile_rojo.png");
        texFondo.loadFromFile("assets/arena_fondo.png");

        sprFondo.setTexture(texFondo);
        sprFondo.setScale(1280.0f / texFondo.getSize().x, 720.0f / texFondo.getSize().y);

        // 2. CÁLCULO DE CENTRADO
        // El tablero mide 450px (9 * 50). 
        // (1280 - 450) / 2 = 415px.
        inicioX = 415.0f;

        // El inicioY depende de dónde empiece el "piso" en tu dibujo.
        // Si 320px queda muy arriba, súbelo a 350px.
        inicioY = 120.0f;

        // 3. Generación del tablero
        // Limpiamos por si acaso se llama dos veces
        for (auto& cas : tablero) delete cas;
        tablero.clear();

        for (int f = 0; f < DIM; f++) {
            for (int c = 0; c < DIM; c++) {
                float px = inicioX + (c * TAM_CASILLA);
                float py = inicioY + (f * TAM_CASILLA);

                // Lógica de colores tipo ajedrez
                bool esRojo = ((f + c) % 2 == 0);
                tablero.push_back(new CasillaArena(px, py, TAM_CASILLA, texAzul, texRojo, esRojo));
            }
        }
    }

    // Traduce coordenadas de matriz a coordenadas de píxeles
    sf::Vector2f getCentroCasilla(int fila, int col) {
        float px = inicioX + (col * TAM_CASILLA) + (TAM_CASILLA / 2.0f);
        float py = inicioY + (fila * TAM_CASILLA) + (TAM_CASILLA / 2.0f);
        return sf::Vector2f(px, py);
    }

    void dibujar(sf::RenderWindow& window) {
        window.draw(sprFondo);
        for (auto& cas : tablero) {
            cas->dibujar(window);
        }
    }

    ~Arena() {
        for (auto& cas : tablero) delete cas;
        tablero.clear();
    }
};