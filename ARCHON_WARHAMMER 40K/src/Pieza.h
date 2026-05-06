#pragma once
#include <SFML/Graphics.hpp>
#include "Arena.h"
#include "EstadoJuego.h"
#include <vector>
#include <string>

// Enums básicos para todo el juego
enum class Bando { LUZ, OSCURIDAD };

// Enum para leer los patrones de movimiento fácilmente
enum class PatronMovimiento {
    Ortogonal,
    Diagonal,
    Ambos,
    Libre     // Para teletransporte
};

>>>>>>> ae9d14edd8fb2b3f35330778e2828e95dd315d36
// Estructura de estadísticas que heredará cada pieza
struct Stats {
    std::string nombre;
    float vida;
    float cooldown;
    bool esRango;
    float ataque;
    float defensa;
    float velAtaque;
};
enum class PatronMovimiento { Ortogonal, Diagonal, Ambos };

class Pieza {
protected:
    // Atributos protegidos: las clases hijas (Golem, Fénix) pueden usarlos directamente
    Bando bando;
    int rangoMovimiento;
    Stats stats;
    PatronMovimiento patronMovimiento;

    bool seleccionado;
    sf::Vector2i posicionTablero;
    sf::Vector2f posicionAbsoluta;
    sf::CircleShape formaVisual;

    // Amistades para que el Renderizador y el Motor sigan funcionando sin cambios pesados
    friend class Motor;
    friend class Renderizador;
    friend class Generador;

public:
    // Constructor: Solo pide lo básico para ubicar la pieza
    Pieza(Bando b, sf::Vector2i pos);

    // Destructor Virtual: CRÍTICO al usar herencia para evitar fugas de memoria
    virtual ~Pieza() = default;

    // Métodos Virtuales Puros: Obligan a las hijas a implementar su propia lógica
    virtual bool poderMover(sf::Vector2i destino, const std::vector<Pieza*>& otrasPiezas, bool esDestinoOcupado) = 0;
    // La pieza recibe la dirección deseada, el tiempo transcurrido y la referencia a la arena para validar
    virtual void procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) = 0;
    // La pieza recibe la ventana y el estado actual para saber cómo mostrarse
    virtual void dibujar(sf::RenderWindow& window, Estado estadoActual) = 0;

    // Métodos Comunes: Lógica que es igual para todos (implementada en Pieza.cpp)
    void mover(sf::Vector2i destino);
    void moverEnArena(float dx, float dy);
    void sincronizarPosicionTablero();
    bool detectarConflicto(const std::vector<Pieza*>& otrasPiezas);


    //Getters Públicos: Para que otras piezas puedan consultarse entre sí sin errores de acceso
    sf::Vector2i getPosicionTablero() const { return posicionTablero; }
    Bando getBando() const { return bando; }
    sf::Color getColorVisual() const {
        return formaVisual.getFillColor();
    }

    sf::FloatRect getHitbox() const {
        return sf::FloatRect(posicionAbsoluta.x - 15.f, posicionAbsoluta.y - 15.f, 30.f, 30.f);
    }

    //Setters públicos: 
   // Setter para la selección (quita el borde amarillo)
    void setSeleccionado(bool valor) {
        seleccionado = valor;
    }

    // Setter para la posición en la arena
    void setPosicionAbsoluta(sf::Vector2f nuevaPos) {
        posicionAbsoluta = nuevaPos;
        formaVisual.setPosition(posicionAbsoluta); // Actualizamos la forma visual al instante
    }
};;