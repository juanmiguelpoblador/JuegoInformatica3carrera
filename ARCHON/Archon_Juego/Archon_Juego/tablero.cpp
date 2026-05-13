#include "tablero.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

// La ventana se pasa desde el coordinador
// Para dibujar usamos una referencia externa
// IMPORTANTE: el coordinador llama a tablero.dibuja() y luego dibuja la selección encima

// Constantes visuales (deben coincidir con coordinador.cpp)
static constexpr float TAM_CASILLA = 70.f;
static constexpr float OFFSET_X = 100.f;
static constexpr float OFFSET_Y = 50.f;

// Referencia a la ventana SFML (definida en Main.cpp)
extern sf::RenderWindow* gVentana;

// =========================================================
// CONSTRUCTOR / DESTRUCTOR
// =========================================================
Tablero::Tablero() {
    turnoActual = Equipo::Azul;
    contadorTurnos = 0;
    inicializa();
}

Tablero::~Tablero() {
    for (int i = 0; i < TAM; ++i)
        for (int j = 0; j < TAM; ++j)
            if (matriz[i][j].pieza) { delete matriz[i][j].pieza; matriz[i][j].pieza = nullptr; }
}

// =========================================================
// INICIALIZA - coloca todas las piezas
// =========================================================
void Tablero::inicializa() {
    for (int i = 0; i < TAM; ++i)
        for (int j = 0; j < TAM; ++j) {
            if (matriz[i][j].pieza) { delete matriz[i][j].pieza; matriz[i][j].pieza = nullptr; }
            matriz[i][j].esPuntoPoder = false;
        }

    // Puntos de poder
    for (auto& p : std::vector<std::pair<int, int>>{ {4,4},{0,4},{8,4},{4,0},{4,8} })
        matriz[p.first][p.second].esPuntoPoder = true;

    // --- Bando Azul (columna 0) ---
    matriz[0][0].pieza = new Caballero(Equipo::Azul);
    matriz[1][0].pieza = new Arqueras(Equipo::Azul);
    matriz[2][0].pieza = new Dragon(Equipo::Azul);
    matriz[3][0].pieza = new Golem(Equipo::Azul);
    matriz[4][0].pieza = new Mago(Equipo::Azul);
    matriz[5][0].pieza = new Golem(Equipo::Azul);
    matriz[6][0].pieza = new Valkiria(Equipo::Azul);
    matriz[7][0].pieza = new Arqueras(Equipo::Azul);
    matriz[8][0].pieza = new Caballero(Equipo::Azul);

    // --- Bando Rojo (columna 8) ---
    matriz[0][8].pieza = new Caballero_oscuro(Equipo::Rojo);
    matriz[1][8].pieza = new Reina_arquera(Equipo::Rojo);
    matriz[2][8].pieza = new Dragon_infernal(Equipo::Rojo);
    matriz[3][8].pieza = new PEKKA(Equipo::Rojo);
    matriz[4][8].pieza = new Bruja(Equipo::Rojo);
    matriz[5][8].pieza = new PEKKA(Equipo::Rojo);
    matriz[6][8].pieza = new Bandida(Equipo::Rojo);
    matriz[7][8].pieza = new Reina_arquera(Equipo::Rojo);
    matriz[8][8].pieza = new Caballero_oscuro(Equipo::Rojo);
}

// =========================================================
// DIBUJA
// =========================================================
void Tablero::dibuja() const {
    if (!gVentana) return;
    // Fondo del tablero
    static sf::Texture texFondo;
    static sf::Sprite sprFondo;
    static bool cargado = false;
    if (!cargado) {
        texFondo.loadFromFile("assets/arena_fondo.png");
        sprFondo.setTexture(texFondo);
        sprFondo.setScale(
            1280.f / texFondo.getSize().x,
            720.f / texFondo.getSize().y
        );
        cargado = true;
    }
    gVentana->draw(sprFondo);
    sf::RenderWindow& window = *gVentana;

    for (int i = 0; i < TAM; ++i) {
        for (int j = 0; j < TAM; ++j) {
            // Casilla
            sf::RectangleShape cas({ TAM_CASILLA - 2.f, TAM_CASILLA - 2.f });
            cas.setPosition(OFFSET_X + j * TAM_CASILLA, OFFSET_Y + i * TAM_CASILLA);

            if ((i + j) % 2 == 0)
                cas.setFillColor(sf::Color(210, 180, 140));
            else
                cas.setFillColor(sf::Color(100, 70, 40));

            if (matriz[i][j].esPuntoPoder) {
                cas.setOutlineColor(sf::Color(255, 215, 0));
                cas.setOutlineThickness(3.f);
            }
            else {
                cas.setOutlineThickness(0.f);
            }
            window.draw(cas);

            // Pieza
            if (matriz[i][j].pieza) {
                Pieza* p = matriz[i][j].pieza;

                sf::CircleShape circ(TAM_CASILLA * 0.33f);
                circ.setOrigin(TAM_CASILLA * 0.33f, TAM_CASILLA * 0.33f);
                circ.setPosition(
                    OFFSET_X + j * TAM_CASILLA + TAM_CASILLA / 2.f,
                    OFFSET_Y + i * TAM_CASILLA + TAM_CASILLA / 2.f
                );
                circ.setFillColor(p->getEquipo() == Equipo::Azul
                    ? sf::Color(70, 130, 220)
                    : sf::Color(220, 60, 60));
                circ.setOutlineColor(sf::Color::White);
                circ.setOutlineThickness(2.f);
                window.draw(circ);

                // Barra de vida
                float ratio = static_cast<float>(p->getVida()) / p->getVidaMax();
                sf::RectangleShape fv({ TAM_CASILLA - 10.f, 5.f });
                fv.setFillColor(sf::Color(40, 40, 40));
                fv.setPosition(OFFSET_X + j * TAM_CASILLA + 5.f, OFFSET_Y + i * TAM_CASILLA + TAM_CASILLA - 9.f);
                window.draw(fv);

                sf::RectangleShape bv({ (TAM_CASILLA - 10.f) * ratio, 5.f });
                bv.setFillColor(ratio > 0.5f ? sf::Color::Green : sf::Color::Red);
                bv.setPosition(fv.getPosition());
                window.draw(bv);
            }
        }
    }

    // Indicador de turno lateral
    sf::RectangleShape ind({ 18.f, TAM_CASILLA * TAM });
    ind.setPosition(OFFSET_X - 26.f, OFFSET_Y);
    ind.setFillColor(turnoActual == Equipo::Azul
        ? sf::Color(70, 130, 220, 200)
        : sf::Color(220, 60, 60, 200));
    window.draw(ind);
}

// =========================================================
// LÓGICA
// =========================================================
bool Tablero::esMovimientoLegal(Pieza* p, int xO, int yO, int xD, int yD) const {
    Pieza* dest = matriz[xD][yD].pieza;
    if (dest && dest->getEquipo() == p->getEquipo()) return false;

    int dist = std::abs(xD - xO) + std::abs(yD - yO);
    if (dist > p->getRangoTablero()) return false;

    if (p->getTipoMovimiento() == TipoMovimiento::Tierra) {
        if (xO != xD && yO != yD) return false;
        if (yO == yD) {
            int paso = (xD > xO) ? 1 : -1;
            for (int i = xO + paso; i != xD; i += paso)
                if (matriz[i][yO].pieza) return false;
        }
        else {
            int paso = (yD > yO) ? 1 : -1;
            for (int j = yO + paso; j != yD; j += paso)
                if (matriz[xO][j].pieza) return false;
        }
    }
    return true;
}

bool Tablero::moverPieza(int xO, int yO, int xD, int yD) {
    if (xO < 0 || xO >= TAM || yO < 0 || yO >= TAM || xD < 0 || xD >= TAM || yD < 0 || yD >= TAM) return false;
    Pieza* p = matriz[xO][yO].pieza;
    if (!p || p->getEquipo() != turnoActual) return false;

    if (esMovimientoLegal(p, xO, yO, xD, yD)) {
        Pieza* dest = matriz[xD][yD].pieza;
        if (dest && dest->getEquipo() != p->getEquipo()) {
            atacanteX = xO; atacanteY = yO;
            defensorX = xD; defensorY = yD;
            return true;
        }
        matriz[xD][yD].pieza = p;
        matriz[xO][yO].pieza = nullptr;
        finalizarTurno();
        return true;
    }
    return false;
}

void Tablero::aplicarCuracion() {
    for (int i = 0; i < TAM; ++i)
        for (int j = 0; j < TAM; ++j)
            if (matriz[i][j].pieza && matriz[i][j].esPuntoPoder)
                matriz[i][j].pieza->curar(15);
}

void Tablero::finalizarTurno() {
    aplicarCuracion();
    turnoActual = (turnoActual == Equipo::Azul) ? Equipo::Rojo : Equipo::Azul;
    contadorTurnos++;
}

ResultadoVictoria Tablero::comprobarVictoria() const {
    return ResultadoVictoria::Ninguno;
}

Pieza* Tablero::getAtacante() const {
    return (atacanteX != -1) ? matriz[atacanteX][atacanteY].pieza : nullptr;
}

Pieza* Tablero::getDefensor() const {
    return (defensorX != -1) ? matriz[defensorX][defensorY].pieza : nullptr;
}

void Tablero::resolverCombate(Pieza* perdedor) {
    if (!perdedor) return;

    // Encontrar dónde está el perdedor
    int px = -1, py = -1;
    for (int i = 0; i < TAM && px == -1; ++i)
        for (int j = 0; j < TAM && px == -1; ++j)
            if (matriz[i][j].pieza == perdedor) { px = i; py = j; }

    if (px == -1) return;

    // El ganador ocupa la casilla del perdedor
    bool perdedorEsDefensor = (px == defensorX && py == defensorY);
    if (perdedorEsDefensor && atacanteX != -1) {
        delete matriz[defensorX][defensorY].pieza;
        matriz[defensorX][defensorY].pieza = matriz[atacanteX][atacanteY].pieza;
        matriz[atacanteX][atacanteY].pieza = nullptr;
    }
    else {
        delete matriz[px][py].pieza;
        matriz[px][py].pieza = nullptr;
    }

    atacanteX = atacanteY = defensorX = defensorY = -1;
}

bool Tablero::hayCombatePendiente() const {
    return (atacanteX != -1 && atacanteY != -1);
}

void Tablero::mueve(double dt) {}
void Tablero::tecla(unsigned char key) {}