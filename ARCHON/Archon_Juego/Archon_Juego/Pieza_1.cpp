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

void Pieza::recibirDano(int cantidad) {
    vida_ = std::max(0, vida_ - cantidad); // Para evitar que la vida baje de 0
}

void Pieza::curar(int cantidad) {
    vida_ = std::min(vidaMax_, vida_ + cantidad); // Para evitar que la vida supere el máximo
}


// ---- Piezas de tierra ----

Caballero::Caballero(Equipo equipo)
    : Pieza(equipo, "Caballero",
            TipoMovimiento::Tierra, TipoArma::CuerpoACuerpo,
            /*vida*/ 60,  /*vel*/ 130,
            /*fuerzaAtaque*/  15,  /*velocidadAtaque*/ 400,
            /*alcanceAtaque*/   1,  /*rangoTablero*/ 3)
{}

Caballero_oscuro::Caballero_oscuro(Equipo equipo)
    : Pieza(equipo, "Caballero_oscuro",
            TipoMovimiento::Tierra, TipoArma::CuerpoACuerpo,
            60, 130, 15, 400, 1, 3)
{}


Golem::Golem(Equipo equipo)
    : Pieza(equipo, "Golem",
            TipoMovimiento::Tierra, TipoArma::CuerpoACuerpo,
            /*vida*/ 200,  /*vel*/  55,
            /*fuerzaAtaque*/   40,  /*velocidadAtaque*/ 1100,
            /*alcanceAtaque*/    1,  /*rangoTablero*/  2)
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
            /*fuerzaAtaque*/   25,  /*velocidadAtaque*/ 700,
            /*alcanceAtaque*/    4,  /*rangoTablero*/ 3)
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
            /*fuerzaAtaque*/   20,  /*velocidadAtaque*/ 600,
            /*alcanceAtaque*/    7,  /*rangoTablero*/ 3)
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
            /*fuerzaAtaque*/   22,  /*avelocidadAtaque*/ 500,
            /*alcanceAtaque*/    2,  /*rangoTablero*/ 4)
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
            /*fuerzaAtaque*/   28,  /*velocidadAtaque*/ 650,
            /*alcanceAtaque*/    5,  /*rangoTablero*/ 5)
{}


Murcielago::Murcielago(Equipo equipo)
    : Pieza(equipo, "Murcielago",
            TipoMovimiento::Vuelo, TipoArma::ExplosionArea,
            /*vida*/ 100,  /*vel*/ 130,
            /*fuerzaAtaque*/   35,  /*velocidadAtaque*/ 900,
            /*alcanceAtaque*/    3,  /*rangoTablero*/ 4)
{}

void Murcielago::activarFormaFuego() {
    if (!formaFuego_) {
        formaFuego_       = true;
        tiempoFormaFuego_ = DURACION_FORMA_FUEGO; //Si no está en forma fuego, la activa y pone en marcha el temporizador de 3s
    }
}

void Murcielago::actualizarArena(float deltams) {
    if (formaFuego_) {
        tiempoFormaFuego_ -= deltams; // descuenta el tiempo transcurrido desde el frame anterior
        if (tiempoFormaFuego_ <= 0.0f) {
            formaFuego_       = false;
            tiempoFormaFuego_ = 0.0f; // Cuanto el temporizador llega a cero, desactiva la forma de fuego y resetea el temporizador
        }
    }
}

Esbirro::Esbirro(Equipo equipo)
    : Pieza(equipo, "Esbirro",
        TipoMovimiento::Vuelo, TipoArma::ExplosionArea,
        /*vida*/ 100,  /*vel*/ 130,
        /*fuerzaAtaque*/   35,  /*velocidadAtaque*/ 900,
        /*alcanceAtaque*/    3,  /*rangoTablero*/ 4)
{}

void Esbirro::activarFormaFuego() {
    if (!formaFuego_) {
        formaFuego_ = true;
        tiempoFormaFuego_ = DURACION_FORMA_FUEGO;
    }
}

void Esbirro::actualizarArena(float deltams) {
    if (formaFuego_) {
        tiempoFormaFuego_ -= deltams;
        if (tiempoFormaFuego_ <= 0.0f) {
            formaFuego_ = false;
            tiempoFormaFuego_ = 0.0f;
        }
    }
}

Dragon_electrico::Dragon_electrico(Equipo equipo)
    : Pieza(equipo, "Dragon_electrico",
            TipoMovimiento::Vuelo, TipoArma::Proyectil,
            /*vida*/ 150,  /*vel*/ 100,
            /*fuerzaAtaque*/   38,  /*velocidadAtaque*/ 800,
            /*alcanceAtaque*/    8,  /*rangoTablero*/ 5)
{}

// ---- Lanzadores de hechizos ----

LanzadorHechizos::LanzadorHechizos(Equipo equipo, const std::string& nombre)
    : Pieza(equipo, nombre,
            TipoMovimiento::Teletransporte, TipoArma::Magia,
            /*vida*/  80,  /*vel*/ 110,
            /*fuerzaAtaque*/   20,  /*velocidadAtaque*/ 750,
            /*alcanceAtaque*/    5,  /*rangoTablero*/ 99)  // rango 99 = teletransporte total
{}

bool LanzadorHechizos::puedeHechizar(IdHechizo hechizo) const {
    return !hechizosUsados_[static_cast<int>(hechizo)]; // Devuelve true si el hechizo todavía no ha sido usado.
}

void LanzadorHechizos::marcarUsado(IdHechizo hechizo) {
    hechizosUsados_[static_cast<int>(hechizo)] = true; // Marca el hechizo como usado poniéndolo a true en el array.
}

Mago::Mago(Equipo equipo)
    : LanzadorHechizos(equipo, "Mago")
{}

Bruja::Bruja(Equipo equipo)
    : LanzadorHechizos(equipo, "Bruja")
{}

//  Changeling 

Changeling::Changeling(Equipo equipo)
    : Pieza(equipo, "Changeling",
            TipoMovimiento::Vuelo, TipoArma::Magia,
            /*vida*/ 100,  /*vel*/ 120,
            /*fuerzaAtaque*/   25,  /*velocidadAtaque*/ 600,
            /*alcanceAtaque*/    4,  /*rangoTablero*/ 4)
{}

void Changeling::adoptarForma(const Pieza& objetivo) {
    tipoAdoptado_  = objetivo.getTipoMovimiento(); //Copia el tipo de movimiento del rival
    rangoAdoptado_ = objetivo.getRangoTablero(); //Copia el rango del rival
    adoptado_      = true; // Marca que ya ha adoptado una forma
}

TipoMovimiento Changeling::getTipoMovimientoEfectivo() const {
    return adoptado_ ? tipoAdoptado_ : tipoMov_; // Si ya adoptó una forma devuelve el tipo de movimiento del rival, si no el suyo propio.
}

int Changeling::getRangoTableroEfectivo() const {
    return adoptado_ ? rangoAdoptado_ : rangoTablero_; // Si ya adoptó una forma devuelve el rango del rival, si no el suyo propio.
}
