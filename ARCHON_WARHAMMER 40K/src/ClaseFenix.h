#pragma once
#include "Pieza.h"
#include "PiezaVoladora.h"
#include "Arena.h"
#include "Color.h"
#include "Hitboxes.h" 
#include <string>
#include <SFML/Graphics.hpp>

class ClaseFenix : public PiezaVoladora {
private:
  
    // Variables del hechizo (HARPY)
    Pieza* enemigoEnlazado;

    // Variables de animación (ARENA)
    int frameActual;
    float temporizadorAnimacion;
    
public:
    ClaseFenix(Bando b, sf::Vector2i pos, std::string tipo);

    // Función de clonación
    Pieza* clonar() const override {
        ClaseFenix* clon = new ClaseFenix(this->bando, this->posicionTablero, this->stats.nombre);
        return clon;
    }

    // Funciones de lógica y movimiento
    void usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo) override;
    void procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) override;

    // Método encapsulado para procesar el drenaje de vida frame a frame (se llama en motor.cpp)
    void actualizarLogicaHechizo(float dt);

    // Funciones visuales:
    void dibujar(sf::RenderWindow& window, Estado estadoActual) override;

    void animar(float dt, sf::Vector2f direccion);
    std::string getDescripcionHechizo() const override {
        if (this->stats.nombre == "LIBRARIAN") {
            return "GENERA UNA AREA DE FUEGO\nQUE DURA 10 SEGUNDOS ";
        }
        return "LANZA UN RAYO ROBAVIDA\nSI ESTA CERCA DEL RIVAL";
    }

};