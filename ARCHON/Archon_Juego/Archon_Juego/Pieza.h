#pragma once
#include <string>
#include <vector>
#include <array>
#include <algorithm>

// Equipos
 class Equipo {
    Azul,
    Rojo
};

 //Tipos de movimiento
class TipoMovimiento {
    Tierra,      
    Vuelo,      
    Teletransporte  
};

//Tipos de armas
class TipoArma {
    CuerpoACuerpo,  
    Proyectil,      
    Magia,          
    ExplosionArea   
};


class Pieza {
public:
    
    Pieza(Equipo equipo,
          const std::string& nombre,
          TipoMovimiento tipoMov,
          TipoArma       tipoArma,
          int            vidaMax,
          int            velocidad,        
          int            fuerzaAtaque,     
          int            velocidadAtaque,  // milisegundos entre ataques
          int            alcanceAtaque,    // unidades de alcance en arena
          int            rangoTablero);    // casillas que puede moverse

    virtual ~Pieza() = default; //Destructor virtual: se ejecuta al destruir un objeto Pieza

    const std::string& getNombre()  const { return nombre_; }
    Equipo             getEquipo()  const { return equipo_; }

    TipoMovimiento getTipoMovimiento()          const { return tipoMov_; } // Devuelve el tipo de movimiento
    int            getRangoTablero()            const { return rangoTablero_; } // Devuelve el rango del tablero

    

    virtual TipoMovimiento getTipoMovimientoEfectivo() const { return tipoMov_; } // Para que el changeling devuelva los valores del rival
    virtual int            getRangoTableroEfectivo()   const { return rangoTablero_; }

    //  Propiedades de combate 
    TipoArma getArma()             const { return tipoArma_; }
    int      getFuerzaAtaque()     const { return fuerzaAtaque_; }
    int      getVelocidadAtaque()  const { return velocidadAtaque_; }
    int      getAlcanceAtaque()    const { return alcanceAtaque_; }
    int      getVelocidad()        const { return velocidad_; }

    // Vida 
    int  getVida()        const { return vida_; }
    int  getVidaMax()     const { return vidaMax_; }
    bool estaMuerta()     const { return vida_ <= 0; }

    void recibirDano(int cantidad);
    void curar(int cantidad);
    void curarTotal()              { vida_ = vidaMax_; }
    void setVida(int v)            { vida_ = std::clamp(v, 0, vidaMax_); }

    // Encarcelamiento 
    bool estaEncarcelada()         const { return encarcelada_; }
    void setEncarcelada(bool estado)     { encarcelada_ = estado; }

 
    // Actualización por frame en la arena
    virtual void actualizarArena(float deltams) {}

    // Descripción de la habilidad especial 
    virtual std::string habilidadEspecial() const { return "Ninguna"; }

protected:
    Equipo      equipo_;
    std::string nombre_;
    TipoMovimiento tipoMov_;
    TipoArma    tipoArma_;

    int vida_;         // vida instantanea
    int vidaMax_;      // vida inical
    int velocidad_;
    int fuerzaAtaque_;
    int velocidadAtaque_;  // ms de recarga
    int alcanceAtaque_;    // unidades de arena
    int rangoTablero_;     // casillas de tablero

    bool encarcelada_ = false;
};


class Caballero : public Pieza {
public:
    explicit Caballero(Equipo equipo); // Constructor: se ejecuta al crear un Caballero. Recibe el equipo al que pertenece
    std::string habilidadEspecial() const override { return "Ataque cuerpo a cuerpo rapido, HP bajo"; } //Descripción de la pieza
};

class Caballero_oscuro : public Pieza {
public:
    explicit Caballero_oscuro(Equipo equipo);
    std::string habilidadEspecial() const override { return "Ataque cuerpo a cuerpo rapido, HP bajo"; }
};

class Golem : public Pieza {
public:
    explicit Golem(Equipo equipo);
    std::string habilidadEspecial() const override { return "Lento pero muy resistente y con alto dano"; }
};

class PEKKA : public Pieza {
public:
    explicit PEKKA(Equipo equipo);
    std::string habilidadEspecial() const override { return "Lento pero muy resistente y con alto dano"; }
};

class Dragon: public Pieza {
public:
    explicit Dragon(Equipo equipo);
    std::string habilidadEspecial() const override { return "Ataque magico de alcance medio, equilibrado"; }
};

class Dragon_infernal : public Pieza {
public:
    explicit Dragon_infernal(Equipo equipo);
    std::string habilidadEspecial() const override { return "Ataque magico de alcance medio, equilibrado"; }
};

class Arqueras : public Pieza {
public:
    explicit Arqueras(Equipo equipo);
    std::string habilidadEspecial() const override { return "Proyectil de largo alcance, fragil"; }
};

class Reina_arquera : public Pieza {
public:
    explicit Reina_arquera(Equipo equipo);
    std::string habilidadEspecial() const override { return "Proyectil de largo alcance, fragil"; }
};

// ---- Piezas voladoras ----

class Valkiria : public Pieza {
public:
    explicit Valkiria(Equipo equipo);
    std::string habilidadEspecial() const override { return "Voladora, espada rapida"; }
};

class Bandida: public Pieza {
public:
    explicit Bandida(Equipo equipo);
    std::string habilidadEspecial() const override { return "Voladora, grito de area"; }
};

class Curandera : public Pieza {
public:
    explicit Curandera(Equipo equipo);
    std::string habilidadEspecial() const override { return "Volador, rayos electricos"; }
};

class Murcielago : public Pieza {
public:
    explicit Murcielago(Equipo equipo);
    // Forma de fuego: se convierte en bola de explosion de area
    bool  estaEnFormaFuego()    const { return formaFuego_; }
    void  activarFormaFuego();
    float getTiempoFormaFuego() const { return tiempoFormaFuego_; } // Devuelve los milisegundos que quedan de forma de fuego

    void actualizarArena(float deltams) override;
    std::string habilidadEspecial() const override {
        return "Puede transformarse en bola de fuego (explosion de area + escudo)";
    }

private:
    bool  formaFuego_      = false; // Indica si la forma de fuego está activa, empieza desactivada
    float tiempoFormaFuego_ = 0.0f;
    static constexpr float DURACION_FORMA_FUEGO = 3000.0f; // La forma de fuego dura 3 segundos
};

class Esbirro : public Pieza {
public:
    explicit Esbirro(Equipo equipo);

    bool  estaEnFormaFuego()    const { return formaFuego_; }
    void  activarFormaFuego();
    float getTiempoFormaFuego() const { return tiempoFormaFuego_; } // Devuelve los milisegundos que quedan de forma de fuego.

    void actualizarArena(float deltams) override;
    std::string habilidadEspecial() const override {
        return "Puede transformarse en bola de fuego (explosion de area + escudo)";
    }

private:
    bool  formaFuego_ = false;   // Indica si la forma de fuego está activa, empieza desactivada
    float tiempoFormaFuego_ = 0.0f;
    static constexpr float DURACION_FORMA_FUEGO = 3000.0f; // La forma de fuego dura 3 segundos
};

class Dragon_electrico : public Pieza {
public:
    explicit Dragon_electrico(Equipo equipo);
    std::string habilidadEspecial() const override { return "Volador, aliento de fuego de largo alcance"; }
};

// ---- Pieza lanzadora de hechizos ----

 class IdHechizo {
    Teleportar   = 0,  // Mover pieza aliada a cualquier casilla valida
    Curar        = 1,  // Restaurar HP completo de pieza aliada
    AlterarTiempo= 2,  // Avanzar el ciclo de oscilacion
    Intercambiar = 3,  // Intercambiar dos piezas del tablero
    InvocarElemental = 4,// Invocar un elemental temporal
    Revivir      = 5,  // Resucitar pieza aliada muerta
    Encarcelar   = 6   // Inmovilizar pieza enemiga en su casilla
};

 // LanzadorHechizos hereda de Pieza y añade la gestión de los 7 hechizos. Mago y Bruja heredan de LanzadorHechizos
class LanzadorHechizos : public Pieza {
public:
    LanzadorHechizos(Equipo equipo, const std::string& nombre);

    bool puedeHechizar(IdHechizo hechizo)  const;  // Devuelve true si el hechizo todavía no ha sido usado
    void marcarUsado  (IdHechizo hechizo);  // Marca el hechizo como usado para que no pueda lanzarse de nuevo
    const std::array<bool, 7>& getHechizosUsados() const { return hechizosUsados_; } // Devuelve el array completo para que la interfaz pueda mostrar que hechizos quedan disponibles

    std::string habilidadEspecial() const override {
        return "Puede lanzar 7 hechizos unicos (uno por partida cada uno)";
    }

private:
    std::array<bool, 7> hechizosUsados_ = {}; // Array de 7 hechizos
};

class Mago : public LanzadorHechizos {
public:
    explicit Mago(Equipo equipo = Equipo::Azul);
};

class Bruja : public LanzadorHechizos {
public:
    explicit Bruja(Equipo equipo = Equipo::Rojo);
};

// Changeling (copia la forma del rival)

class Changeling : public Pieza {
public:
    explicit Changeling(Equipo equipo = Equipo::Rojo);

    // Llamar antes del combate para adoptar el tipo de la pieza rival
    void adoptarForma(const Pieza& objetivo); // copia tipo y rango del rival
    bool haAdoptadoForma() const { return adoptado_; } // devuelve el del rival

    TipoMovimiento getTipoMovimientoEfectivo() const override; // Sobreescribe el método de Pieza para devolver el tipo de movimiento del rival si ya ha adoptado su forma, o el propio si no.
    int            getRangoTableroEfectivo()   const override;

    std::string habilidadEspecial() const override {
        return "Adopta el tipo de movimiento y rango de la pieza enemiga en combate";
    } 

private:
    bool           adoptado_      = false; // Indica si el Changeling ha copiado ya la forma de un rival, empieza en false porque al crearse aún no ha copiado a nadie
    TipoMovimiento tipoAdoptado_  = TipoMovimiento::Vuelo; // Tipo de movimiento copiado del rival, se inicializa a Vuelo como valor por defecto hasta que adopte una forma
    int            rangoAdoptado_ = 3; // Rango de tablero copiado del rival, se inicializa a 3 como valor por defecto
};
