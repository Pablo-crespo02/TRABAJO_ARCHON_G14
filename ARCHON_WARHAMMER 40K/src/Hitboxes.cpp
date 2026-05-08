#include "Hitboxes.h"
#include "Color.h"

//Constructor actualizado
Hitbox::Hitbox(sf::Vector2f posicionInicial, sf::Vector2f direccion, double rapidez, sf::Color color, Pieza* propietario, double danoHitbox, double tiempodevida, double radio) {
    forma.setRadius(radio);
    forma.setOrigin(radio, radio);
    forma.setPosition(posicionInicial.x, posicionInicial.y);
    forma.setFillColor(color);

    velocidad.x = direccion.x * rapidez;
    velocidad.y = direccion.y * rapidez;

    activo = true;
    atacante = propietario;   // Almacenamos el puntero físico
    dano = danoHitbox;

    tiempoVida = tiempodevida;
}

void Hitbox::ActualizarHitbox(double dt) {

    //Multiplicamos la velocidad por dt:
    forma.move(velocidad.x*dt,velocidad.y*dt);

    //Restamos al reloj de vida el delta time:
    tiempoVida -= dt;

    //Eliminamos si el reloj de vida llega a 0:
    if (tiempoVida <= 0)activo = false;
}