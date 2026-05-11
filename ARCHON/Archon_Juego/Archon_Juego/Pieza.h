#pragma once
#include <string>
#include <vector>
#include <array>
#include <algorithm>

// =========================================================
// 1. ENUMERACIONES (Tipos básicos)
// =========================================================

enum class Equipo { Azul, Rojo };

enum class TipoMovimiento { Tierra, Vuelo, Teletransporte };

enum class TipoArma { CuerpoACuerpo, Proyectil, Magia, ExplosionArea };

enum class IdHechizo {
    Teleportar = 0,
    Curar = 1,
    AlterarTiempo = 2,
    Intercambiar = 3,
    InvocarElemental = 4,
    Revivir = 5,
    Encarcelar = 6
};

// =========================================================
// 2. CLASE BASE PRINCIPAL (Pieza)
// =========================================================

class Pieza {
public:
    Pieza(Equipo equipo, const std::string& nombre, TipoMovimiento tipoMov, TipoArma tipoArma,
        int vidaMax, int velocidad, int fuerzaAtaque, int velocidadAtaque, int alcanceAtaque, int rangoTablero);

    virtual ~Pieza() = default;

    // Getters básicos
    const std::string& getNombre() const { return nombre_; }
    Equipo getEquipo() const { return equipo_; }
    TipoMovimiento getTipoMovimiento() const { return tipoMov_; }
    int getRangoTablero() const { return rangoTablero_; }

    // Métodos virtuales para el Changeling y polimorfismo
    virtual TipoMovimiento getTipoMovimientoEfectivo() const { return tipoMov_; }
    virtual int getRangoTableroEfectivo() const { return rangoTablero_; }

    // Combate
    TipoArma getArma() const { return tipoArma_; }
    int getFuerzaAtaque() const { return fuerzaAtaque_; }
    int getVelocidadAtaque() const { return velocidadAtaque_; }
    int getAlcanceAtaque() const { return alcanceAtaque_; }
    int getVelocidad() const { return velocidad_; }

    // Vida
    int getVida() const { return vida_; }
    int getVidaMax() const { return vidaMax_; }
    bool estaMuerta() const { return vida_ <= 0; }

    void recibirDano(int cantidad);
    void curar(int cantidad);
    void curarTotal() { vida_ = vidaMax_; }

    // Reemplaza tu setVida por este en Pieza.h
    void setVida(int v) {
        if (v < 0) {
            vida_ = 0;
        }
        else if (v > vidaMax_) {
            vida_ = vidaMax_;
        }
        else {
            vida_ = v;
        }
    }
    // Estado
    bool estaEncarcelada() const { return encarcelada_; }
    void setEncarcelada(bool estado) { encarcelada_ = estado; }

    virtual void actualizarArena(float deltams) {}
    virtual std::string habilidadEspecial() const { return "Ninguna"; }

protected:
    Equipo equipo_;
    std::string nombre_;
    TipoMovimiento tipoMov_;
    TipoArma tipoArma_;

    int vida_;
    int vidaMax_;
    int velocidad_;
    int fuerzaAtaque_;
    int velocidadAtaque_;
    int alcanceAtaque_;
    int rangoTablero_;

    bool encarcelada_ = false;
};

// =========================================================
// 3. CLASES DE CATEGORÍA (Intermedias)
// =========================================================

class PiezaTierra : public Pieza {
public:
    using Pieza::Pieza;
    TipoMovimiento getTipoMovimientoEfectivo() const override { return TipoMovimiento::Tierra; }
};

class PiezaVuelo : public Pieza {
public:
    using Pieza::Pieza;
    TipoMovimiento getTipoMovimientoEfectivo() const override { return TipoMovimiento::Vuelo; }
};

class PiezaTeleport : public Pieza {
public:
    using Pieza::Pieza;
    TipoMovimiento getTipoMovimientoEfectivo() const override { return TipoMovimiento::Teletransporte; }
};

// =========================================================
// 4. CLASES ESPECIALES (Lanzador de Hechizos)
// =========================================================

class LanzadorHechizos : public PiezaTeleport {
public:
    LanzadorHechizos(Equipo equipo, const std::string& nombre);

    bool puedeHechizar(IdHechizo hechizo) const;
    void marcarUsado(IdHechizo hechizo);
    const std::array<bool, 7>& getHechizosUsados() const { return hechizosUsados_; }

    std::string habilidadEspecial() const override {
        return "Puede lanzar 7 hechizos unicos (uno por partida cada uno)";
    }

private:
    std::array<bool, 7> hechizosUsados_ = {};
};

// =========================================================
// 5. PIEZAS CONCRETAS (Personajes)
// =========================================================

// --- Tierra ---
class Caballero : public PiezaTierra {
public:
    explicit Caballero(Equipo equipo);
    std::string habilidadEspecial() const override { return "Ataque cuerpo a cuerpo rapido, HP bajo"; }
};

class Caballero_oscuro : public PiezaTierra {
public:
    explicit Caballero_oscuro(Equipo equipo);
    std::string habilidadEspecial() const override { return "Ataque cuerpo a cuerpo rapido, HP bajo"; }
};

class Golem : public PiezaTierra {
public:
    explicit Golem(Equipo equipo);
    std::string habilidadEspecial() const override { return "Lento pero muy resistente y con alto dano"; }
};

class PEKKA : public PiezaTierra {
public:
    explicit PEKKA(Equipo equipo);
    std::string habilidadEspecial() const override { return "Lento pero muy resistente y con alto dano"; }
};

class Dragon : public PiezaTierra {
public:
    explicit Dragon(Equipo equipo);
    std::string habilidadEspecial() const override { return "Ataque magico de alcance medio, equilibrado"; }
};

class Dragon_infernal : public PiezaTierra {
public:
    explicit Dragon_infernal(Equipo equipo);
    std::string habilidadEspecial() const override { return "Ataque magico de alcance medio, equilibrado"; }
};

class Arqueras : public PiezaTierra {
public:
    explicit Arqueras(Equipo equipo);
    std::string habilidadEspecial() const override { return "Proyectil de largo alcance, fragil"; }
};

class Reina_arquera : public PiezaTierra {
public:
    explicit Reina_arquera(Equipo equipo);
    std::string habilidadEspecial() const override { return "Proyectil de largo alcance, fragil"; }
};

// --- Voladoras ---
class Valkiria : public PiezaVuelo {
public:
    explicit Valkiria(Equipo equipo);
    std::string habilidadEspecial() const override { return "Voladora, espada rapida"; }
};

class Bandida : public PiezaVuelo {
public:
    explicit Bandida(Equipo equipo);
    std::string habilidadEspecial() const override { return "Voladora, grito de area"; }
};

class Curandera : public PiezaVuelo {
public:
    explicit Curandera(Equipo equipo);
    std::string habilidadEspecial() const override { return "Volador, rayos electricos"; }
};

class PiezaFuego : public PiezaVuelo {
public:
    using PiezaVuelo::PiezaVuelo; // hereda el constructor de PiezaVuelo

    bool  estaEnFormaFuego()    const { return formaFuego_; }
    void  activarFormaFuego();
    float getTiempoFormaFuego() const { return tiempoFormaFuego_; }  // Devuelve los milisegundos que quedan de forma de fuego

    void actualizarArena(float deltams) override;

protected:
    bool  formaFuego_ = false;  // Indica si la forma de fuego está activa, empieza desactivada
    float tiempoFormaFuego_ = 0.0f;
    static constexpr float DURACION_FORMA_FUEGO = 3000.0f; // La forma de fuego dura 3 segundos
};  

class Murcielago : public PiezaFuego {
public:
    explicit Murcielago(Equipo equipo);
<<<<<<< HEAD
<<<<<<< HEAD
    std::string habilidadEspecial() const override {
        return "Puede transformarse en bola de fuego (explosion de area + escudo)";
    }
=======
=======
>>>>>>> 07e339029c967d5cfb1b90d778162174fe21961d
    bool estaEnFormaFuego() const { return formaFuego_; }
    void activarFormaFuego();
    float getTiempoFormaFuego() const { return tiempoFormaFuego_; }
    void actualizarArena(float deltams) override;
    std::string habilidadEspecial() const override { return "Transformacion en bola de fuego"; }
private:
    bool formaFuego_ = false;
    float tiempoFormaFuego_ = 0.0f;
    static constexpr float DURACION_FORMA_FUEGO = 3000.0f;
<<<<<<< HEAD
>>>>>>> 07e339029c967d5cfb1b90d778162174fe21961d
=======
>>>>>>> 07e339029c967d5cfb1b90d778162174fe21961d
};

class Esbirro : public PiezaFuego {
public:
    explicit Esbirro(Equipo equipo);
<<<<<<< HEAD
<<<<<<< HEAD
    std::string habilidadEspecial() const override {
        return "Puede transformarse en bola de fuego (explosion de area + escudo)";
    }
=======
=======
>>>>>>> 07e339029c967d5cfb1b90d778162174fe21961d
    bool estaEnFormaFuego() const { return formaFuego_; }
    void activarFormaFuego();
    float getTiempoFormaFuego() const { return tiempoFormaFuego_; }
    void actualizarArena(float deltams) override;
    std::string habilidadEspecial() const override { return "Transformacion en bola de fuego"; }
private:
    bool formaFuego_ = false;
    float tiempoFormaFuego_ = 0.0f;
    static constexpr float DURACION_FORMA_FUEGO = 3000.0f;
<<<<<<< HEAD
>>>>>>> 07e339029c967d5cfb1b90d778162174fe21961d
=======
>>>>>>> 07e339029c967d5cfb1b90d778162174fe21961d
};
class Dragon_electrico : public PiezaVuelo {
public:
    explicit Dragon_electrico(Equipo equipo);
    std::string habilidadEspecial() const override { return "Volador, aliento electrico"; }
};

// --- Lanzadores ---
class Mago : public LanzadorHechizos {
public:
    explicit Mago(Equipo equipo = Equipo::Azul);
};

class Bruja : public LanzadorHechizos {
public:
    explicit Bruja(Equipo equipo = Equipo::Rojo);
};

// --- Especial ---
class Changeling : public PiezaVuelo {
public:
    explicit Changeling(Equipo equipo = Equipo::Rojo);
    void adoptarForma(const Pieza& objetivo);
    bool haAdoptadoForma() const { return adoptado_; }
    TipoMovimiento getTipoMovimientoEfectivo() const override;
    int getRangoTableroEfectivo() const override;
    std::string habilidadEspecial() const override { return "Copia forma del rival"; }
private:
    bool adoptado_ = false;
    TipoMovimiento tipoAdoptado_ = TipoMovimiento::Vuelo;
    int rangoAdoptado_ = 3;
};