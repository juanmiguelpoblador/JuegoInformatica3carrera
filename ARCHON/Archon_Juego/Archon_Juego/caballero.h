#pragma once
#include "terrestre.h"

class caballero : public terrestre
{
public:
    caballero(int bando);
    void atacarEnArena() override;
    std::string getsimbolo() const override { return "caballero"; }
};

