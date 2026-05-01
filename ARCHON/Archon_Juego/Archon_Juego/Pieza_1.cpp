#include "Pieza.h"
#include <algorithm>


Pieza::Pieza(Equipo         equipo,
             const std::string& nombre,
             TipoMovimiento tipoMov,
             TipoArma       tipoArma,
             int            vidaMax,
             int            velocidad,
             int            fuerzaAtaque,
             int            velocidadAtaque,
             int            alcanceAtaque,
             int            rangoTablero)
    : equipo_(equipo)
    , nombre_(nombre)
    , tipoMov_(tipoMov)
    , tipoArma_(tipoArma)
    , vida_(vidaMax)
    , vidaMax_(vidaMax)
    , velocidad_(velocidad)
    , fuerzaAtaque_(fuerzaAtaque)
    , velocidadAtaque_(velocidadAtaque)
    , alcanceAtaque_(alcanceAtaque)
    , rangoTablero_(rangoTablero)
{}

void Pieza::recibirDanio(int cantidad) {
    vida_ = std::max(0, vida_ - cantidad);
}

void Pieza::curar(int cantidad) {
    vida_ = std::min(vidaMax_, vida_ + cantidad);
}


// ---- Piezas de tierra ----

Caballero::Caballero(Equipo equipo)
    : Pieza(equipo, "Caballero",
            TipoMovimiento::Tierra, TipoArma::CuerpoACuerpo,
            /*vida*/ 60,  /*vel*/ 130,
            /*atk*/  15,  /*atkMs*/ 400,
            /*rng*/   1,  /*rngTab*/ 3)
{}

Goblin::Caballero_oscuro(Equipo equipo)
    : Pieza(equipo, "Caballero_oscuro",
            TipoMovimiento::Tierra, TipoArma::CuerpoACuerpo,
            60, 130, 15, 400, 1, 3)
{}


Golem::Golem(Equipo equipo)
    : Pieza(equipo, "Golem",
            TipoMovimiento::Tierra, TipoArma::CuerpoACuerpo,
            /*vida*/ 200,  /*vel*/  55,
            /*atk*/   40,  /*atkMs*/ 1100,
            /*rng*/    1,  /*rngTab*/  2)
{}

PEKKA::PEKKA(Equipo equipo)
    : Pieza(equipo, "PEKKA",
            TipoMovimiento::Tierra, TipoArma::CuerpoACuerpo,
            200, 55, 40, 1100, 1, 2)
{}


Dragon::Dragon(Equipo equipo)
    : Pieza(equipo, "Dragon",
            TipoMovimiento::Tierra, TipoArma::Magia,
            /*vida*/ 110,  /*vel*/  95,
            /*atk*/   25,  /*atkMs*/ 700,
            /*rng*/    4,  /*rngTab*/ 3)
{}

Dragon_infernal::Dragon_infernal(Equipo equipo)
    : Pieza(equipo, "Dragon_infernal",
            TipoMovimiento::Tierra, TipoArma::Magia,
            110, 95, 25, 700, 4, 3)
{}

Arqueras::Arqueras(Equipo equipo)
    : Pieza(equipo, "Arqueras",
            TipoMovimiento::Tierra, TipoArma::Proyectil,
            /*vida*/  70,  /*vel*/  90,
            /*atk*/   20,  /*atkMs*/ 600,
            /*rng*/    7,  /*rngTab*/ 3)
{}

Reina_arquera::Reina_arquera(Equipo equipo)
    : Pieza(equipo, "Reina_arquera",
            TipoMovimiento::Tierra, TipoArma::Proyectil,
            70, 90, 20, 600, 7, 3)
{}

// ---- Piezas voladoras ----

Valkiria::Valkiria(Equipo equipo)
    : Pieza(equipo, "Valkiria",
            TipoMovimiento::Vuelo, TipoArma::CuerpoACuerpo,
            /*vida*/  90,  /*vel*/ 140,
            /*atk*/   22,  /*atkMs*/ 500,
            /*rng*/    2,  /*rngTab*/ 4)
{}

Bandida::Bandida(Equipo equipo)
    : Pieza(equipo, "Bandida",
            TipoMovimiento::Vuelo, TipoArma::Magia,
            90, 120, 18, 550, 3, 4)
{}

Curandera::Curandera(Equipo equipo)
    : Pieza(equipo, "Curandera",
            TipoMovimiento::Vuelo, TipoArma::Magia,
            /*vida*/  80,  /*vel*/ 150,
            /*atk*/   28,  /*atkMs*/ 650,
            /*rng*/    5,  /*rngTab*/ 5)
{}


Murcielago::Murcielago(Equipo equipo)
    : Pieza(equipo, "Murcielago",
            TipoMovimiento::Vuelo, TipoArma::ExplosionArea,
            /*vida*/ 100,  /*vel*/ 130,
            /*atk*/   35,  /*atkMs*/ 900,
            /*rng*/    3,  /*rngTab*/ 4)
{}

void Murcielago::activarFormaFuego() {
    if (!formaFuego_) {
        formaFuego_       = true;
        tiempoFormaFuego_ = DURACION_FORMA_FUEGO;
    }
}

void Murcielago::actualizarArena(float deltams) {
    if (formaFuego_) {
        tiempoFormaFuego_ -= deltams;
        if (tiempoFormaFuego_ <= 0.0f) {
            formaFuego_       = false;
            tiempoFormaFuego_ = 0.0f;
        }
    }
}

Dragon_electrico::Dragon_electrico(Equipo equipo)
    : Pieza(equipo, "Dragon_electrico",
            TipoMovimiento::Vuelo, TipoArma::Proyectil,
            /*vida*/ 150,  /*vel*/ 100,
            /*atk*/   38,  /*atkMs*/ 800,
            /*rng*/    8,  /*rngTab*/ 5)
{}

// ---- Lanzadores de hechizos ----

LanzadorHechizos::LanzadorHechizos(Equipo equipo, const std::string& nombre)
    : Pieza(equipo, nombre,
            TipoMovimiento::Teletransporte, TipoArma::Magia,
            /*vida*/  80,  /*vel*/ 110,
            /*atk*/   20,  /*atkMs*/ 750,
            /*rng*/    5,  /*rngTab*/ 99)  // rango 99 = teletransporte total
{}

bool LanzadorHechizos::puedeHechizar(IdHechizo hechizo) const {
    return !hechizosUsados_[static_cast<int>(hechizo)];
}

void LanzadorHechizos::marcarUsado(IdHechizo hechizo) {
    hechizosUsados_[static_cast<int>(hechizo)] = true;
}

Mago::Mago(Equipo equipo)
    : LanzadorHechizos(equipo, "Mago")
{}

Bruja::Bruja(Equipo equipo)
    : LanzadorHechizos(equipo, "Bruja")
{}

// ---- Changeling ----

Changeling::Changeling(Equipo equipo)
    : Pieza(equipo, "Changeling",
            TipoMovimiento::Vuelo, TipoArma::Magia,
            /*vida*/ 100,  /*vel*/ 120,
            /*atk*/   25,  /*atkMs*/ 600,
            /*rng*/    4,  /*rngTab*/ 4)
{}

void Changeling::adoptarForma(const Pieza& objetivo) {
    tipoAdoptado_  = objetivo.getTipoMovimiento();
    rangoAdoptado_ = objetivo.getRangoTablero();
    adoptado_      = true;
}

TipoMovimiento Changeling::getTipoMovimientoEfectivo() const {
    return adoptado_ ? tipoAdoptado_ : tipoMov_;
}

int Changeling::getRangoTableroEfectivo() const {
    return adoptado_ ? rangoAdoptado_ : rangoTablero_;
}
