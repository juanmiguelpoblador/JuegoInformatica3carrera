#pragma once
#include "personaje.h" 

class terrestre : public personaje {
public:
    // Constructor
    terrestre(int v, int f, int va, int tr, int rm, int b);

    // Movimiento en tablero
    void moverEnTablero() override;

    bool esmovimientovalido(int fori, int cori, int fdest, int cdest) override;
    bool puedeSaltar() const override { return false; }
};