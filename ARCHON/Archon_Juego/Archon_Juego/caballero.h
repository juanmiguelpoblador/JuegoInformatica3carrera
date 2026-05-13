#pragma once
#include "PIEZAA.h"

class Caballero : public Pieza {
public:
    Caballero(float x, float y, int bando) : Pieza(x, y, bando) {
        // Ajusta la ruta a tu imagen real
        cargarImagen("assets/caballero_azul.png");
    }

    void actualizar() override {
        // Lógica de movimiento simple para probar
        x += 0.2f;
    }
};
