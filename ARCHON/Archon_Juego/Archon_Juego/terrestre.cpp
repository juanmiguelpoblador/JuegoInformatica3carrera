#include "Terrestre.h"
#include <cmath>


// El constructor de Terrestre le pasa los datos al constructor de Pieza
terrestre::terrestre(int v, int f, int va, int tr, int rm, int b)
    : personaje(v, f, va, tr, rm, b) {
}

// Implementación del movimiento
void terrestre::moverEnTablero() {
    // Aquí irá el código lógico de movimiento.
    
}


bool terrestre::esmovimientovalido(int forigen, int corigen, int fdestino, int cdestino) {
    // calculamos cuántas casillas hay de diferencia en horizontal y vertical
    int distfila = std::abs(forigen - fdestino);
    int distcol = std::abs(corigen - cdestino);

    // no puede moverse en diagonal (una de las dos distancias debe ser 0)
    if (distfila != 0 && distcol != 0) {
        return false;
    }

    //  la distancia no puede ser mayor a su radiomovimiento
    int distanciatotal = distfila + distcol;
    if (distanciatotal > radioMovimiento) {
        return false;
    }

    // no puede "moverse" a la misma casilla en la que ya está
    if (distanciatotal == 0) {
        return false;
    }

    return true; // si cumple, el movimiento matemático es válido
}