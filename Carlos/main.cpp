#pragma once
#include <array>
#include <vector>
#include <memory>
#include "Pieza.h" 

enum class ColorCasilla {
    Clara,     
    Oscura,    
    Neutral    
};

enum class Turno {
    LUZ,
    OSCURIDAD
};

struct Pos {
    int x = 0;
    int y = 0;
    bool operator==(const Pos& o) const { return x == o.x && y == o.y; }
    bool operator!=(const Pos& o) const { return !(*this == o); }
};

struct Casilla {
    ColorCasilla        color = ColorCasilla::Neutral;
    bool                esPuntoPoder = false;
    std::shared_ptr<Pieza> pieza = nullptr;  
};

enum class ResultadoVictoria {
    GanaLuz,
    GanaOscuridad,
    Ninguno
};

class Tablero {
public:
    static constexpr int TAMAÑO = 9;

    // Número de turnos entre cada paso del ciclo de oscilación
    static constexpr int PASOS_CICLO = 4;

    // Curación base por turno al estar en casilla propia (HP/turno)
    static constexpr int CURACION_BASE = 5;
    static constexpr int CURACION_PUNTO_PODER = 15;

    // Coordenadas fijas de los 5 puntos de poder:
    static constexpr std::array<Pos, 5> PUNTOS_PODER = { {
        {4, 4},  // centro
        {4, 0},  // borde superior
        {4, 8},  // borde inferior
        {0, 4},  // borde izquierdo
        {8, 4}   // borde derecho
    } };