#include "tablero.h"
#include <cmath>
#include <vector>
Tablero::Tablero() {    
    turnoActual = (Equipo::Azul);
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
    matriz[0][4].pieza = new Mago(Equipo::Azul);
    matriz[8][4].pieza = new Bruja(Equipo::Rojo);
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
        if (xO != xD && yO != yD) return false; //cero diagonales puras
        if (yO == yD) {
            int paso = (xD > xO) ? 1 : -1;
            for (int i = xO + paso; i != xD; i += paso) {
                if (matriz[i][yO].pieza != nullptr) return false; // Hay alguien en medio
            }
        }
        else if (xO == xD) {
            int paso = (yD > yO) ? 1 : -1;
            for (int j = yO + paso; j != yD; j += paso) {
                if (matriz[xO][j].pieza != nullptr) return false; // Hay alguien en medio
            }
        }
    }
    // Vuelo y Teletransporte ignoran obstáculos, por lo que no necesitan más validación aquí
    return true;
}

bool Tablero::moverPieza(int xO, int yO, int xD, int yD) {
    // ESCUDO: Si hacen clic fuera del tablero de 9x9, ignoramos la acción
    if (xO < 0 || xO >= TAM || yO < 0 || yO >= TAM ||
        xD < 0 || xD >= TAM || yD < 0 || yD >= TAM) {
        return false;
    }
    Pieza* p = matriz[xO][yO].pieza;
    if (!p || p->getEquipo() != turnoActual /* || p->estaEncarcelada() */) return false;

    if (esMovimientoLegal(p, xO, yO, xD, yD)) {
        Pieza* destino = matriz[xD][yD].pieza;
        if (destino != nullptr && destino->getEquipo() != p->getEquipo()) {
            // atacanteX = xO; atacanteY = yO;
            defensorX = xD; defensorY = yD;
            // Le indicamos al Coordinador que hay que pasar al estado ARENA_COMBATE
            return true; // Le dice al Coordinador: "¡Pausa el tablero, nos vamos a la arena!"
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
}

void Tablero::finalizarTurno() {
    aplicarCuracion();
    turnoActual = (turnoActual == Equipo::Azul) ? Equipo::Rojo : Equipo::Azul;
    contadorTurnos++;
}

ResultadoVictoria Tablero::comprobarVictoria() const {
    int controlLuz = 0, controlOsc = 0;
    // Lógica para contar cuántos puntos de poder tiene cada bandoç
    // Si uno llega a 5, devuelve la victoria correspondiente.
    return ResultadoVictoria::Ninguno;
}
Pieza* Tablero::getAtacante() const {
    if (atacanteX != -1 && atacanteY != -1) return matriz[atacanteX][atacanteY].pieza;
    return nullptr;
}

Pieza* Tablero::getDefensor() const {
    if (defensorX != -1 && defensorY != -1) return matriz[defensorX][defensorY].pieza;
    return nullptr;
}

void Tablero::resolverCombate(Pieza* perdedor) {
    // Aquí programaremos más adelante qué pasa cuando alguien muere en la arena
    // (Borrar la pieza, mover al ganador a la casilla, limpiar las coordenadas, etc.)
}
bool Tablero::hayCombatePendiente() const {
    // Si atacanteX no es -1, significa que hemos guardado un combate que aún no se ha resuelto
    return (atacanteX != -1 && atacanteY != -1);
}
void Tablero::mueve(double dt) {
    // Aquí irá la lógica de las animaciones y la oscilación de colores
}

void Tablero::tecla(unsigned char key) {
    // Aquí gestionarás las teclas si el coordinador te las pasa
}
void Tablero::dibuja() const {
    // Por ahora lo dejamos vacío para que el error desaparezca
    // Aquí es donde luego usarás SFML o ETSIDI para pintar la cuadrícula
}