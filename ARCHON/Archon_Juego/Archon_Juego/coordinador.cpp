#include "Coordinador.h"
#include "Arena.h"

// =========================================================
// Necesitamos una ventana SFML para la arena.
// Asumimos que en Main.cpp existe una ventana global o
// se pasa de alguna forma. Ajusta según vuestro Main.cpp.
// =========================================================
extern sf::RenderWindow ventana; // <-- ajusta si en vuestro main se llama diferente

Coordinador::Coordinador() {
    estado = INICIO;
    arenaActual = nullptr;
    ganadorArena = nullptr;
    relojArena.restart();
}

Coordinador::~Coordinador() {
    delete arenaActual;
}

// =========================================================
// MUEVE - Lógica principal por frame
// =========================================================
void Coordinador::mueve(double dt) {
    switch (estado) {

    case INICIO:
        // Lógica de inicio (animaciones del menú, etc.)
        // Por ahora pasamos directo al turno azul con ENTER
        break;

    case TURNO_AZUL:
    case TURNO_ROJO:
        tablero.mueve(dt);

        // 1. ¿Hay victoria?
        if (tablero.comprobarVictoria() != ResultadoVictoria::Ninguno) {
            estado = FIN_PARTIDA;
        }
        // 2. ¿Dos piezas han chocado? → activar arena
        else if (tablero.hayCombatePendiente()) {
            Pieza* atacante = tablero.getAtacante();
            Pieza* defensor = tablero.getDefensor();

            if (atacante && defensor) {
                // Crear la arena con las dos piezas que van a combatir
                delete arenaActual; // por si había una anterior sin limpiar
                arenaActual = new Arena(
                    atacante->getEquipo() == Equipo::Azul ? atacante : defensor,
                    atacante->getEquipo() == Equipo::Rojo ? atacante : defensor,
                    ventana
                );
                ganadorArena = nullptr;
                relojArena.restart();
                estado = ARENA_COMBATE;
            }
        }
        break;

    case ARENA_COMBATE:
        if (arenaActual) {
            float deltaArena = relojArena.restart().asSeconds();
            // Limitar delta para evitar saltos si la ventana se congela
            if (deltaArena > 0.05f) deltaArena = 0.05f;

            ganadorArena = arenaActual->update(deltaArena);

            // ¿Terminó el combate?
            if (ganadorArena != nullptr) {
                // La pieza perdedora es la que no ganó
                Pieza* perdedor = (ganadorArena == tablero.getAtacante())
                    ? tablero.getDefensor()
                    : tablero.getAtacante();

                // Informar al tablero del resultado
                tablero.resolverCombate(perdedor);

                // Limpiar arena
                delete arenaActual;
                arenaActual = nullptr;
                ganadorArena = nullptr;

                // Volver al tablero y cambiar turno
                tablero.finalizarTurno();
                estado = (tablero.comprobarVictoria() != ResultadoVictoria::Ninguno)
                    ? FIN_PARTIDA
                    : TURNO_AZUL; // finalizarTurno ya cambia el turno interno
            }
        }
        break;

    case FIN_PARTIDA:
        // Detener el juego, mostrar rankings, etc.
        break;
    }
}

// =========================================================
// DIBUJA
// =========================================================
void Coordinador::dibuja() const {
    switch (estado) {

    case INICIO:
        // Dibujar menú inicial
        break;

    case TURNO_AZUL:
    case TURNO_ROJO:
        tablero.dibuja();
        // Opcional: indicador de turno
        break;

    case ARENA_COMBATE:
        // El tablero de fondo (opcional, puedes quitarlo)
        tablero.dibuja();
        // Encima dibujamos la arena a pantalla completa
        if (arenaActual) {
            arenaActual->draw();
        }
        break;

    case FIN_PARTIDA:
        // Pantalla de Game Over / Victoria
        break;
    }
}

// =========================================================
// TECLADO
// =========================================================
void Coordinador::tecla(unsigned char key) {
    switch (estado) {

    case INICIO:
        // ENTER para empezar
        if (key == 13) estado = TURNO_AZUL;
        break;

    case TURNO_AZUL:
    case TURNO_ROJO:
        tablero.tecla(key);
        break;

    case ARENA_COMBATE:
        // El input de la arena se gestiona en Arena::procesarInput()
        // con sf::Keyboard::isKeyPressed, no hace falta nada aquí
        break;

    case FIN_PARTIDA:
        break;
    }
}

// =========================================================
// RATÓN
// =========================================================
void Coordinador::raton(int boton, int estadoBoton, int x, int y) {
    switch (estado) {
    case TURNO_AZUL:
    case TURNO_ROJO:
        // Pasar click al tablero si lo necesita
        break;
    default:
        break;
    }
}
