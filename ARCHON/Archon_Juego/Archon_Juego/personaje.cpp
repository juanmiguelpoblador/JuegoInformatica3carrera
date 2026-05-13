#include "personaje.h"


// Implementación del constructor
personaje::personaje(int v, int f, int va, int tr, int rm, int b) {
    vida = v;
    fuerza = f;
    velocidadAtaque = va;
    tiempoRecarga = tr;
    radioMovimiento = rm;
    mi_bando = (bando)b;
}

void personaje::recibirDano(int cantidad) {
    vida -= cantidad;
    if (vida < 0) {
        vida = 0; // Para que la vida no sea negativa
    }
}

int personaje::obtenerVida() {
    return vida;
}

bool personaje::estaVivo() {
    return vida > 0;
}

void personaje::curar(int cantidad) {
    vida += cantidad;
}


