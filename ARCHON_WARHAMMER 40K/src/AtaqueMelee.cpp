#include "AtaqueMelee.h"
#include "Pieza.h"

AtaqueMelee::AtaqueMelee(sf::Vector2f posicion, Pieza* owner, float danoAtaque) {
    forma.setRadius(25.f); // Radio del golpe (un poco más grande que la pieza para alcanzar)
    forma.setOrigin(25.f, 25.f);
    forma.setPosition(posicion);

    // Area del golpe
    forma.setFillColor(sf::Color(255, 0, 0, 150));

    tiempoDeVida = 0.2f; // El golpe dura 0.2 segundos 
    activo = true;
    atacante = owner;
    dano = danoAtaque;
}

void AtaqueMelee::actualizar(float dt) {
    if (activo) {
        tiempoDeVida -= dt;
        if (tiempoDeVida <= 0.f) {
            activo = false; // El golpe desaparece cuando se acaba el tiempo
        }
    }
}