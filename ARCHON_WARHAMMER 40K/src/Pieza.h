#pragma once
#include <SFML/Graphics.hpp>
#include "Arena.h"
#include "EstadoJuego.h"
#include <vector>
#include <string>
#include "BarraVida.h"
#include "Hitboxes.h"

// Enums básicos para todo el juego
enum class Bando { LUZ, OSCURIDAD };

// Enum para leer los patrones de movimiento fácilmente
enum class PatronMovimiento { Ortogonal, Diagonal, Ambos };

// Estructura de estadísticas que heredará cada pieza
struct Stats {

    std::string nombre;
    float vida;
    float vidaMaxima;
    bool esRango;
    bool esVolador;
    float ataque;
    float defensa;
    float velAtaque;
    float cooldown;

    //Variables encargadas de la gestión de proyectiles en la arena
    sf::Clock relojHitbox;//Reloj que avanza desde que se dispara
    sf::Clock relojProyectil;//para que cambie de fotograma al atacar

};


class Pieza {
protected:
    // Atributos protegidos: las clases hijas (Golem, Fénix) pueden usarlos directamente
    Bando bando;
    int rangoMovimiento;
    Stats stats;
    PatronMovimiento patronMovimiento;
    BarraVida barraSalud;

    sf::Vector2f ultimadireccion; //Memoria de la última dirección a la que miró la pieza, para "apuntar" los proyectiles
    bool hechizoDisponible;//Hechizo sólo una vez por combate
    bool seleccionado;
    sf::Vector2i posicionTablero;
    sf::Vector2f posicionAbsoluta;
    sf::CircleShape formaVisual;

    // Amistades para que el Renderizador y el Motor sigan funcionando sin cambios pesados
    friend class Motor;
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

    //Métodos de gestión de proyectiles en la arena:
    bool puedeAtacar()const;   //Comprueba si ha pasado sufucuente tiempo desde el disparo anterior
    void reiniciarRelojHitbox(); //Reinicia el reloj de disparo

    //Getters Públicos: Para que otras piezas puedan consultarse entre sí sin errores de acceso
    sf::Vector2i getPosicionTablero() const { return posicionTablero; }

    Bando getBando() const { return bando; }

    sf::Color getColorVisual() const {
        return formaVisual.getFillColor();
    }

    sf::FloatRect getHitbox() const {
        return sf::FloatRect(posicionAbsoluta.x - 15.f, posicionAbsoluta.y - 15.f, 30.f, 30.f);
    }

    sf::Vector2f getPosicionAbsoluta() const {
        return sf::Vector2f(posicionAbsoluta);
    }

    sf::Vector2f getultimadireccion()const {
        return sf::Vector2f(ultimadireccion);
    }

    //SETTERS PÚBLICOS:
    // Setter para la selección (quita el borde amarillo)
    void setSeleccionado(bool valor) {
        seleccionado = valor;
    }

    // Setter para la posición en la arena
    void setPosicionAbsoluta(sf::Vector2f nuevaPos) {
        posicionAbsoluta = nuevaPos;
        formaVisual.setPosition(posicionAbsoluta); // Actualizamos la forma visual al instante
    }

    //Setter para establecer tiempos de recarga diferentes para cada pieza:
    void setTiempoRecarga(double tiemporecarga) {
        stats.velAtaque = tiemporecarga;
    };

    //Setter para establecer la última dirección de mirada de la pieza, para apuntar los proyectiles:
    void setultimadireccion(sf::Vector2f nuevadireccion) {
        if (nuevadireccion.x != 0 || nuevadireccion.y != 0) {
            ultimadireccion = nuevadireccion;
        }
    }
    bool getHechizoDisponible() const { return hechizoDisponible; }
    void setHechizoDisponible(bool estado) { hechizoDisponible = estado; }
    // Función virtual. Pasamos el vector de Hitboxes y el enemigo por si el hechizo es de daño o control
    // OJO: Le ponemos "{}" al final y no "= 0" para que sea opcional. Así las piezas a las 
    // que aún no les hayas programado hechizo no darán error de compilación.
    virtual void usarHechizo(std::vector<Hitbox>& contenedordeAtaques, Pieza* enemigo) {}
};