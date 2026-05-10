#pragma once
#include <array>
#include <vector>
#include <memory>
#include "Pieza.h" 

enum class ResultadoVictoria { GanaAzul, GanaRojo, Ninguno }; //Decidimos el estado de la partida

struct Casilla { //Que hay en cada cuadro de la casilla 
    bool esPuntoPoder = false; //true si es uno de los 5 puntos de poder, se aplican los bonificadores de curación y protección a la pieza encima
    Pieza* pieza = nullptr; 
}; //Usamos std::shared_ptr para no utilizar new al crear una pieza y tener que borrarla despues con delete

class Tablero {
public: //CHEKEAR CAMBIOS (VISUAL)
    static constexpr int TAM = 9; //Tamaño de tablero

    Tablero(); //constructor
    virtual ~Tablero();
    void inicializa();
    void dibuja() const;
    void mueve(double dt);
    void tecla(unsigned char key);
    // Lógica de juego
    bool moverPieza(int xOrig, int yOrig, int xDest, int yDest); //movimiento en el tablero. Recibe COORDENADAS de origen y de destino
    void finalizarTurno(); //Limpiezatras movimiento. Cambia turno
    ResultadoVictoria comprobarVictoria() const; //Comprueba estado

    // Hechizos (Cerebro del juego)
    bool ejecutarHechizo(int idHechizo, int x, int y, int x2 = -1, int y2 = -1);
    void resolverCombate(Pieza* perdedor);
    Pieza* getAtacante() const;
    Pieza* getDefensor() const;
    bool hayCombatePendiente() const;

private:
    Casilla matriz[TAM][TAM];//9X9
    Equipo turnoActual; //almacena  a quien le toca mover 
    int contadorTurnos; //Cuenta los turnos para el ciclo de oscilacion (color de las casillas)

    // Métodos auxiliares
    bool esMovimientoLegal(Pieza* p, int xO, int yO, int xD, int yD) const; //comprueba si pieza puede llegar a su destino segun rango y tipo (Tierra,Vuelo,Tp)
    void aplicarCuracion(); //Recorre tablero buscando piezas en punto de poder para aplicar bonificador
    int atacanteX = -1, atacanteY = -1;
    int defensorX = -1, defensorY = -1;
};