#include "Hitboxes.h"
#include "Color.h"

//Constructor actualizado
Hitbox::Hitbox(sf::Vector2f posicionInicial, sf::Vector2f direccion, double rapidez, sf::Color color,
    Pieza* propietario, double danoHitbox, double tiempodevida, double radio, bool esDoT) {
    forma.setRadius(radio);
    forma.setOrigin(radio, radio);
    forma.setPosition(posicionInicial);
    forma.setFillColor(color);

    velocidad.x = direccion.x * (float)rapidez;
    velocidad.y = direccion.y * (float)rapidez;

    activo = true;
    haHechoDano = false;
    esDanoContinuo = esDoT; // Valor asignado
    atacante = propietario;
    dano = (float)danoHitbox;
    tiempoVida = (float)tiempodevida;
}

void Hitbox::ActualizarHitbox(double dt) {

    //Multiplicamos la velocidad por dt:
    forma.move(velocidad.x*dt,velocidad.y*dt);

    //Restamos al reloj de vida el delta time:
    tiempoVida -= dt;

    //Eliminamos si el reloj de vida llega a 0:
    if (tiempoVida <= 0)activo = false;
}