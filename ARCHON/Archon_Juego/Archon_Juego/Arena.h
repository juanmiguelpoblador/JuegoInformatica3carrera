#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "Pieza.h"

// =========================================================
// PROYECTIL - Bala/disparo en la arena de combate
// =========================================================
struct Proyectil {
    sf::CircleShape forma;
    sf::Vector2f    velocidad;
    int             danio;
    bool            activo = true;
    int             equipoOwner; // 0 = Azul, 1 = Rojo

    Proyectil(float x, float y, sf::Vector2f vel, int dmg, int equipo)
        : velocidad(vel), danio(dmg), activo(true), equipoOwner(equipo)
    {
        forma.setRadius(6.f);
        forma.setFillColor(equipo == 0 ? sf::Color(100, 180, 255) : sf::Color(255, 80, 80));
        forma.setOrigin(6.f, 6.f);
        forma.setPosition(x, y);
    }

    void update() { forma.move(velocidad); }
};

// =========================================================
// ARENA - Escenario de combate en tiempo real
// =========================================================
class Arena {
public:
    // Constructor: recibe las dos piezas que van a combatir y la ventana
    Arena(Pieza* piezaAzul, Pieza* piezaRoja, sf::RenderWindow& ventana);

    // Actualiza toda la lógica. Devuelve:
    //   nullptr  -> combate en curso
    //   piezaAzul -> ganó el azul
    //   piezaRoja -> ganó el rojo
    Pieza* update(float deltaTime);

    // Dibuja la arena sobre la ventana
    void draw();

private:
    // --- Referencia a la ventana ---
    sf::RenderWindow& ventana_;

    // --- Piezas que combaten (punteros a los objetos del tablero) ---
    Pieza* piezaAzul_;
    Pieza* piezaRoja_;

    // --- Vida actual en la arena (copia, para no destruir la del tablero hasta que termine) ---
    int vidaAzul_;
    int vidaRoja_;

    // --- Representación visual de cada pieza en la arena ---
    sf::RectangleShape spriteAzul_;
    sf::RectangleShape spriteRoja_;

    // --- Proyectiles en vuelo ---
    std::vector<Proyectil> proyectiles_;

    // --- Temporizadores de recarga (en segundos) ---
    float recargaAzul_  = 0.f;
    float recargaRoja_  = 0.f;

    // --- Barras de vida ---
    sf::RectangleShape fondoBarraAzul_;
    sf::RectangleShape barraAzul_;
    sf::RectangleShape fondoBarraRoja_;
    sf::RectangleShape barraRoja_;

    // --- Fondo de la arena ---
    sf::RectangleShape fondo_;
    sf::RectangleShape bordeArena_;

    // --- Texto de nombres ---
    sf::Font            fuente_;
    sf::Text            textoNombreAzul_;
    sf::Text            textoNombreRoja_;
    sf::Text            textoControles_;
    bool                fuenteCargada_ = false;

    // --- Métodos privados ---
    void procesarInput(float deltaTime);
    void moverSprite(sf::RectangleShape& sprite, float dx, float dy);
    void disparar(Pieza* atacante, sf::RectangleShape& spriteOrigen,
                  sf::RectangleShape& spriteDestino, int equipoOwner);
    void actualizarProyectiles(float deltaTime);
    void comprobarColisiones();
    void actualizarBarrasVida();
    void dibujarBarrasVida();
    void dibujarTextos();

    // Convierte velocidad de Pieza (1-10) a píxeles/segundo en la arena
    float calcularVelPixeles(int velPieza) const;

    // Intervalo de recarga en segundos según velocidadAtaque de la pieza
    float calcularIntervaloRecarga(int velAtaque) const;

    // Límites de la arena en pantalla
    static constexpr float ARENA_X      = 100.f;
    static constexpr float ARENA_Y      = 80.f;
    static constexpr float ARENA_ANCHO  = 1000.f;
    static constexpr float ARENA_ALTO   = 620.f;
};
