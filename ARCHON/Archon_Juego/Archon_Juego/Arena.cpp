#include "Arena.h"
#include <algorithm>
#include <cmath>

// =========================================================
// CONSTRUCTOR
// =========================================================
Arena::Arena(Pieza* piezaAzul, Pieza* piezaRoja, sf::RenderWindow& ventana)
    : piezaAzul_(piezaAzul), piezaRoja_(piezaRoja), ventana_(ventana)
{
    // Copiar vida actual de cada pieza
    vidaAzul_ = piezaAzul_->getVida();
    vidaRoja_ = piezaRoja_->getVida();

    // --- Sprites de las piezas ---
    spriteAzul_.setSize({ 48.f, 48.f });
    spriteAzul_.setFillColor(sf::Color(70, 140, 255));
    spriteAzul_.setOutlineColor(sf::Color::White);
    spriteAzul_.setOutlineThickness(2.f);
    spriteAzul_.setOrigin(24.f, 24.f);
    spriteAzul_.setPosition(ARENA_X + 120.f, ARENA_Y + ARENA_ALTO / 2.f);

    spriteRoja_.setSize({ 48.f, 48.f });
    spriteRoja_.setFillColor(sf::Color(220, 50, 50));
    spriteRoja_.setOutlineColor(sf::Color::White);
    spriteRoja_.setOutlineThickness(2.f);
    spriteRoja_.setOrigin(24.f, 24.f);
    spriteRoja_.setPosition(ARENA_X + ARENA_ANCHO - 120.f, ARENA_Y + ARENA_ALTO / 2.f);

    // --- Fondo de la arena ---
    fondo_.setSize({ ARENA_ANCHO, ARENA_ALTO });
    fondo_.setPosition(ARENA_X, ARENA_Y);
    fondo_.setFillColor(sf::Color(30, 30, 50));

    bordeArena_.setSize({ ARENA_ANCHO, ARENA_ALTO });
    bordeArena_.setPosition(ARENA_X, ARENA_Y);
    bordeArena_.setFillColor(sf::Color::Transparent);
    bordeArena_.setOutlineColor(sf::Color(200, 160, 50));
    bordeArena_.setOutlineThickness(4.f);

    // --- Barras de vida ---
    // Azul (izquierda)
    fondoBarraAzul_.setSize({ 300.f, 24.f });
    fondoBarraAzul_.setPosition(ARENA_X + 20.f, ARENA_Y - 40.f);
    fondoBarraAzul_.setFillColor(sf::Color(60, 60, 60));

    barraAzul_.setSize({ 300.f, 24.f });
    barraAzul_.setPosition(ARENA_X + 20.f, ARENA_Y - 40.f);
    barraAzul_.setFillColor(sf::Color(70, 200, 100));

    // Roja (derecha)
    fondoBarraRoja_.setSize({ 300.f, 24.f });
    fondoBarraRoja_.setPosition(ARENA_X + ARENA_ANCHO - 320.f, ARENA_Y - 40.f);
    fondoBarraRoja_.setFillColor(sf::Color(60, 60, 60));

    barraRoja_.setSize({ 300.f, 24.f });
    barraRoja_.setPosition(ARENA_X + ARENA_ANCHO - 320.f, ARENA_Y - 40.f);
    barraRoja_.setFillColor(sf::Color(220, 60, 60));

    // --- Textos (intentamos cargar una fuente del sistema) ---
    fuenteCargada_ = fuente_.loadFromFile("C:/Windows/Fonts/arial.ttf");
    if (fuenteCargada_) {
        textoNombreAzul_.setFont(fuente_);
        textoNombreAzul_.setString(piezaAzul_->getNombre());
        textoNombreAzul_.setCharacterSize(18);
        textoNombreAzul_.setFillColor(sf::Color(150, 200, 255));
        textoNombreAzul_.setPosition(ARENA_X + 20.f, ARENA_Y - 68.f);

        textoNombreRoja_.setFont(fuente_);
        textoNombreRoja_.setString(piezaRoja_->getNombre());
        textoNombreRoja_.setCharacterSize(18);
        textoNombreRoja_.setFillColor(sf::Color(255, 150, 150));
        textoNombreRoja_.setPosition(ARENA_X + ARENA_ANCHO - 320.f, ARENA_Y - 68.f);

        textoControles_.setFont(fuente_);
        textoControles_.setString("Azul: WASD + SPACE dispara    Rojo: Flechas + ENTER dispara");
        textoControles_.setCharacterSize(14);
        textoControles_.setFillColor(sf::Color(180, 180, 180));
        textoControles_.setPosition(ARENA_X + ARENA_ANCHO / 2.f - 280.f, ARENA_Y + ARENA_ALTO + 8.f);
    }
}

// =========================================================
// UPDATE - Lógica principal por frame
// =========================================================
Pieza* Arena::update(float deltaTime) {
    // Descontar recargas
    if (recargaAzul_  > 0.f) recargaAzul_  -= deltaTime;
    if (recargaRoja_ > 0.f) recargaRoja_ -= deltaTime;

    // Actualizar habilidades especiales (ej. forma de fuego)
    piezaAzul_->actualizarArena(deltaTime * 1000.f); // la clase espera milisegundos
    piezaRoja_->actualizarArena(deltaTime * 1000.f);

    procesarInput(deltaTime);
    actualizarProyectiles(deltaTime);
    comprobarColisiones();
    actualizarBarrasVida();

    // Comprobar fin del combate
    if (vidaAzul_ <= 0) {
        piezaAzul_->setVida(0);
        return piezaRoja_; // gana el rojo
    }
    if (vidaRoja_ <= 0) {
        piezaRoja_->setVida(0);
        return piezaAzul_; // gana el azul
    }
    return nullptr; // sigue el combate
}

// =========================================================
// DRAW
// =========================================================
void Arena::draw() {
    ventana_.draw(fondo_);
    ventana_.draw(bordeArena_);

    // Proyectiles
    for (auto& p : proyectiles_)
        if (p.activo) ventana_.draw(p.forma);

    // Piezas
    ventana_.draw(spriteAzul_);
    ventana_.draw(spriteRoja_);

    dibujarBarrasVida();
    dibujarTextos();
}

// =========================================================
// INPUT - Movimiento y disparo
// =========================================================
void Arena::procesarInput(float deltaTime) {
    float velAzul = calcularVelPixeles(piezaAzul_->getVelocidad());
    float velRoja = calcularVelPixeles(piezaRoja_->getVelocidad());
    float dt = deltaTime;

    // --- Jugador Azul: WASD ---
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) moverSprite(spriteAzul_,  0.f, -velAzul * dt);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) moverSprite(spriteAzul_,  0.f,  velAzul * dt);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) moverSprite(spriteAzul_, -velAzul * dt, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) moverSprite(spriteAzul_,  velAzul * dt, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && recargaAzul_ <= 0.f) {
        disparar(piezaAzul_, spriteAzul_, spriteRoja_, 0);
        recargaAzul_ = calcularIntervaloRecarga(piezaAzul_->getVelocidadAtaque());
    }

    // --- Jugador Rojo: Flechas ---
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    moverSprite(spriteRoja_,  0.f, -velRoja * dt);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  moverSprite(spriteRoja_,  0.f,  velRoja * dt);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  moverSprite(spriteRoja_, -velRoja * dt, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) moverSprite(spriteRoja_,  velRoja * dt, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && recargaRoja_ <= 0.f) {
        disparar(piezaRoja_, spriteRoja_, spriteAzul_, 1);
        recargaRoja_ = calcularIntervaloRecarga(piezaRoja_->getVelocidadAtaque());
    }
}

// Mueve un sprite sin salirse de los límites de la arena
void Arena::moverSprite(sf::RectangleShape& sprite, float dx, float dy) {
    sf::Vector2f pos = sprite.getPosition();
    float nx = pos.x + dx;
    float ny = pos.y + dy;

    // Clamp dentro del rectángulo de la arena
    nx = std::max(ARENA_X + 24.f, std::min(nx, ARENA_X + ARENA_ANCHO - 24.f));
    ny = std::max(ARENA_Y + 24.f, std::min(ny, ARENA_Y + ARENA_ALTO  - 24.f));

    sprite.setPosition(nx, ny);
}

// =========================================================
// DISPARO
// =========================================================
void Arena::disparar(Pieza* atacante, sf::RectangleShape& origen,
                     sf::RectangleShape& destino, int equipoOwner)
{
    sf::Vector2f posOrigen  = origen.getPosition();
    sf::Vector2f posDestino = destino.getPosition();

    sf::Vector2f dir = posDestino - posOrigen;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len < 1.f) return;
    dir /= len;

    // Velocidad del proyectil basada en alcanceAtaque (más alcance = proyectil más rápido)
    float velProyectil = 250.f + atacante->getAlcanceAtaque() * 20.f;
    sf::Vector2f velVec = dir * velProyectil; // en píxeles/segundo

    proyectiles_.emplace_back(posOrigen.x, posOrigen.y,
                               velVec / 60.f,  // se llama ~60fps, así que dividimos
                               atacante->getFuerzaAtaque(),
                               equipoOwner);
}

// =========================================================
// PROYECTILES
// =========================================================
void Arena::actualizarProyectiles(float deltaTime) {
    for (auto& p : proyectiles_) {
        if (!p.activo) continue;
        p.update();

        // Desactivar si sale de la arena
        auto pos = p.forma.getPosition();
        if (pos.x < ARENA_X || pos.x > ARENA_X + ARENA_ANCHO ||
            pos.y < ARENA_Y || pos.y > ARENA_Y + ARENA_ALTO)
        {
            p.activo = false;
        }
    }

    // Eliminar proyectiles inactivos
    proyectiles_.erase(
        std::remove_if(proyectiles_.begin(), proyectiles_.end(),
            [](const Proyectil& p) { return !p.activo; }),
        proyectiles_.end()
    );
}

// =========================================================
// COLISIONES proyectil <-> pieza
// =========================================================
void Arena::comprobarColisiones() {
    sf::FloatRect boundsAzul = spriteAzul_.getGlobalBounds();
    sf::FloatRect boundsRoja = spriteRoja_.getGlobalBounds();

    for (auto& p : proyectiles_) {
        if (!p.activo) continue;

        // Proyectil del azul impacta al rojo
        if (p.equipoOwner == 0 && boundsRoja.contains(p.forma.getPosition())) {
            vidaRoja_ = std::max(0, vidaRoja_ - p.danio);
            p.activo = false;
        }
        // Proyectil del rojo impacta al azul
        else if (p.equipoOwner == 1 && boundsAzul.contains(p.forma.getPosition())) {
            vidaAzul_ = std::max(0, vidaAzul_ - p.danio);
            p.activo = false;
        }
    }
}

// =========================================================
// BARRAS DE VIDA
// =========================================================
void Arena::actualizarBarrasVida() {
    float ratioAzul = static_cast<float>(vidaAzul_) / piezaAzul_->getVidaMax();
    float ratioRoja = static_cast<float>(vidaRoja_) / piezaRoja_->getVidaMax();

    ratioAzul = std::max(0.f, std::min(1.f, ratioAzul));
    ratioRoja = std::max(0.f, std::min(1.f, ratioRoja));

    barraAzul_.setSize({ 300.f * ratioAzul, 24.f });
    barraRoja_.setSize({ 300.f * ratioRoja, 24.f });

    // Color que cambia de verde a rojo según la vida
    auto colorBarra = [](float ratio) -> sf::Color {
        return sf::Color(
            static_cast<sf::Uint8>((1.f - ratio) * 220),
            static_cast<sf::Uint8>(ratio * 200),
            50
        );
    };
    barraAzul_.setFillColor(colorBarra(ratioAzul));
    barraRoja_.setFillColor(colorBarra(ratioRoja));
}

void Arena::dibujarBarrasVida() {
    ventana_.draw(fondoBarraAzul_);
    ventana_.draw(barraAzul_);
    ventana_.draw(fondoBarraRoja_);
    ventana_.draw(barraRoja_);
}

void Arena::dibujarTextos() {
    if (!fuenteCargada_) return;
    ventana_.draw(textoNombreAzul_);
    ventana_.draw(textoNombreRoja_);
    ventana_.draw(textoControles_);
}

// =========================================================
// CONVERSIÓN DE ATRIBUTOS
// =========================================================

// La velocidad de pieza va de 1 a 10; en arena la mapeamos a 100-400 px/s
float Arena::calcularVelPixeles(int velPieza) const {
    return 100.f + velPieza * 30.f;
}

// velocidadAtaque: 1=muy lento(1.5s), 10=muy rápido(0.2s)
float Arena::calcularIntervaloRecarga(int velAtaque) const {
    return std::max(0.2f, 1.6f - velAtaque * 0.14f);
}
