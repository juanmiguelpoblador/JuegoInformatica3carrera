#include "tablero.h"
#include <cmath>
#include <vector>
Tablero::Tablero() {    
    turnoActual = (Equipo::Luz);
    contadorTurnos = 0; 
    inicializa();
}
Tablero::~Tablero() {
    for (int i = 0; i < TAM; ++i) {
        for (int j = 0; j < TAM; ++j) {
            if (matriz[i][j].pieza != nullptr) {
                delete matriz[i][j].pieza;
                matriz[i][j].pieza = nullptr;
            }
        }
    }
}
void Tablero::inicializa() {
    // Limpiamos por si venimos de reiniciar la partida
    for (int i = 0; i < TAM; ++i) {
        for (int j = 0; j < TAM; ++j) {
            if (matriz[i][j].pieza != nullptr) {
                delete matriz[i][j].pieza;
                matriz[i][j].pieza = nullptr;
            }
            matriz[i][j].esPuntoPoder = false;
        }
    }

    // Marcar Puntos de Poder (Centro y bordes)
    std::vector<std::pair<int, int>> pPoder = { {4,4}, {0,4}, {8,4}, {4,0}, {4,8} };
    for (auto& p : pPoder) {
        matriz[p.first][p.second].esPuntoPoder = true;
    }
    // Colocar piezas usando "new" 
    // (Asegúrate de incluir la cabecera correcta para Mago y Bruja)
    matriz[0][4].pieza = new Mago(Equipo::Luz);
    matriz[8][4].pieza = new Bruja(Equipo::Oscuridad);
    //completar con el resto de las 18 piezas por bando
}

bool Tablero::esMovimientoLegal(Pieza* p, int xO, int yO, int xD, int yD) const {
    // Si la casilla está ocupada POR UN ALIADO, no puedes mover ahí
    Pieza* destino = matriz[xD][yD].pieza;
    if (destino != nullptr && destino->getEquipo() == p->getEquipo()) {
        return false;
    }

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
    Pieza* p = matriz[xO][yO].pieza;
    if (!p || p->getEquipo() != turnoActual /* || p->estaEncarcelada() */) return false;

    if (esMovimientoLegal(p, xO, yO, xD, yD)) {
        Pieza* destino = matriz[xD][yD].pieza;
        if (destino != nullptr && destino->getEquipo() != p->getEquipo()) {
            // AQUÍ NO MOVEMOS LA PIEZA AÚN NI FINALIZAMOS EL TURNO.
            // Le indicamos al Coordinador que hay que pasar al estado ARENA_COMBATE
            return true; // Podemos devolver true para indicar que la acción fue válida
        }

        // Si la casilla está vacía, movimiento normal
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
            Pieza* p = matriz[i][j].pieza;
            if (p != nullptr) {
                // Las piezas en los puntos de poder se curan más rápido [cite: 137]
                if (matriz[i][j].esPuntoPoder) 
                    p->curar(15);
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