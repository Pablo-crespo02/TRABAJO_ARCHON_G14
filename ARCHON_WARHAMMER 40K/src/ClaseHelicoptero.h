#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Pieza.h"
#include "PiezaVoladora.h"
#include "Arena.h"
#include "Hitboxes.h"
#include "Color.h"
#include <SFML/Audio.hpp>

class ClaseHelicoptero : public PiezaVoladora {
private:
    // Texturas y sprites (Solo Arena)
    sf::Texture texturaArena;
    sf::Sprite spriteArena;

    // Variables de animación:
    int frameActual;
    float temporizadorAnimacion;
    int anchoFrame;
    int altoFrame;

    // IA y disparo automático
    sf::Clock relojDisparoAuto; // Cooldown interno del helicóptero

    sf::SoundBuffer bufferHelic;
    sf::Sound sonidoHelic;
public:
    // Constructor adaptado (bando, posición inicial en la arena, y estadísticas base)
    ClaseHelicoptero(Bando b, sf::Vector2f posArena);
    // Método obligatorio de clonación (lo dejamos limpio por si acaso)
    Pieza* clonar() const override {
        ClaseHelicoptero* clon = new ClaseHelicoptero(this->bando, this->posicionAbsoluta);
        clon->spriteArena.setTexture(clon->texturaArena);
        return clon;
    }

    // El helicóptero no se dibuja en el tablero, solo en la arena
    void dibujar(sf::RenderWindow& window, Estado estadoActual) override;
    void reproducirSonidoAtaque() {sonidoHelic.play();}
    // IA autónoma: vuela persiguiendo al enemigo y disparando proyectiles
    void actualizarIA(float dt, Arena& arena, Pieza* enemigo, std::vector<Hitbox>& hitboxes);

    void animar(float dt, sf::Vector2f direccion);
};