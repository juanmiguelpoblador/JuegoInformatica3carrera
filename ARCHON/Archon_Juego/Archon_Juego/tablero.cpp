#include "tablero.h"
#include <cmath>

Tablero::Tablero() : turnoActual(Equipo::Luz), contadorTurnos(0) { //lista de inicializacion donde ponemos que el 
    inicializarTablero();
}

void Tablero::inicializarTablero() {
    //Marcar Puntos de Poder 
    std::vector<std::pair<int, int>> pPoder = { {4,4}, {0,4}, {8,4}, {4,0}, {4,8} };
    for (auto& p : pPoder) matriz[p.first][p.second].esPuntoPoder = true;

    //Colocar piezas (Ejemplo: Mago en el centro de la fila 0 para Luz)
    matriz[0][4].pieza = std::make_shared<Mago>(Equipo::Luz);
    matriz[8][4].pieza = std::make_shared<Bruja>(Equipo::Oscuridad);
    // completar con el resto de las 18 piezas por bando
}

bool Tablero::esMovimientoLegal(std::shared_ptr<Pieza> p, int xO, int yO, int xD, int yD) const {
    if (matriz[xD][yD].pieza != nullptr) return false; // Casilla ocupada

    int distTotal = std::abs(xD - xO) + std::abs(yD - yO);
    if (distTotal > p->getRangoTablero()) return false;

    // Diferenciación por tipo de movimiento 
    if (p->getTipoMovimiento() == TipoMovimiento::Tierra) {
        // No permite diagonales puras si ambas coordenadas cambian
        if (xO != xD && yO != yD) return false;
    }
    // Vuelo y Teletransporte ignoran obstáculos, por lo que no necesitan más validación aquí
    return true;
}

bool Tablero::moverPieza(int xO, int yO, int xD, int yD) {
    auto p = matriz[xO][yO].pieza;
    if (!p || p->getEquipo() != turnoActual || p->estaEncarcelada()) return false;

    if (esMovimientoLegal(p, xO, yO, xD, yD)) {
        matriz[xD][yD].pieza = p;
        matriz[xO][yO].pieza = nullptr;
        finalizarTurno();
        return true;
    }
    return false;
}

void Tablero::aplicarCuracion() {
    for (int i = 0; i < TAM; ++i) {
        for (int j = 0; j < TAM; ++j) {
            if (auto p = matriz[i][j].pieza) {
                if (matriz[i][j].esPuntoPoder) p->curar(15); // Curación punto poder
                // Aquí podrías añadir la curación por color de casilla 
            }
        }
    }
}

void Tablero::finalizarTurno() {
    aplicarCuracion();
    turnoActual = (turnoActual == Equipo::Luz) ? Equipo::Oscuridad : Equipo::Luz;
    contadorTurnos++;
}

ResultadoVictoria Tablero::comprobarVictoria() const {
    int controlLuz = 0, controlOsc = 0;
    // Lógica para contar cuántos puntos de poder tiene cada bandoç
    // Si uno llega a 5, devuelve la victoria correspondiente.
    return ResultadoVictoria::Ninguno;
}