#pragma once
#include "PiezaTeletransporte.h"
#include "Pieza.h"      // <-- Inclusión directa de seguridad
#include "Hitboxes.h"   // <-- Inclusión directa de seguridad
#include "Arena.h"
#include "Color.h"
#include <string>
#include <vector>
#include <SFML/Graphics.hpp> 

class ClaseLider : public PiezaTeletransporte {
private:
    std::vector<Pieza*> minionsInvocados;

    // SPRITES Y TEXTURAS 
    sf::Texture texturaTablero;
    sf::Sprite spriteTablero;
    sf::Texture texturaArena;
    sf::Sprite spriteArena;

    // VARIABLES DE ANIMACIÓN (ARENA)
    int frameActual;
    float temporizadorAnimacion;
    int anchoFrame;
    int altoFrame;

public:
    ClaseLider(Bando b, sf::Vector2i pos, std::string tipo);
    ~ClaseLider();

    Pieza* clonar() const override {
        ClaseLider* clon = new ClaseLider(*this);
        // Reconectamos sus propios sprites a sus propias texturas
        clon->spriteTablero.setTexture(clon->texturaTablero);
        clon->spriteArena.setTexture(clon->texturaArena);

        // Vaciamos los minions del clon para que no haya duplicados al cargar partida
        clon->minionsInvocados.clear();
        return clon;
    }

    // FUNCIONES DE MOVIMIENTO Y HECHIZOS
    void procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) override;
    void usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo) override;
    void dibujar(sf::RenderWindow& window, Estado estadoActual) override;
    void animar(float dt, sf::Vector2f direccion);

    // FUNCIONES DE GESTIÓN DE MINIONS
    void actualizarMinions(float dt, Arena& arena, Pieza* enemigo) override;
    void limpiarMinions() override;
    std::vector<Pieza*>& getMinionsInvocados() override;
};