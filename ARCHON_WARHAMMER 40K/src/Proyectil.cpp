#include "Proyectil.h"
#include "Color.h"

//Constructor actualizado
Proyectil::Proyectil(sf::Vector2f posicionInicial, sf::Vector2f direccion, double rapidez, sf::Color color, Pieza* owner, float danoProyectil) {
    forma.setRadius(16);
    forma.setOrigin(16, 16);
    forma.setPosition(posicionInicial.x, posicionInicial.y);
    forma.setFillColor(color);

    velocidad.x = direccion.x * rapidez;
    velocidad.y = direccion.y * rapidez;

    activo = true;
    disparador = owner;   // Almacenamos el puntero físico
    dano = danoProyectil;
}

void Proyectil::ActualizarProyectil() {
    forma.move(velocidad);
}