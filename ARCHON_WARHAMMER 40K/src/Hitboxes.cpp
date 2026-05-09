#include "Hitboxes.h"
#include "Color.h"

//Constructor actualizado
Hitbox::Hitbox(sf::Vector2f posicionInicial, sf::Vector2f direccion, double rapidez, sf::Color color,
    Pieza* propietario, double danoHitbox, double tiempodevida, double radio,
    bool esDoT, bool erratico, bool inmoviliza, double duracionControl) {
    forma.setRadius(radio);
    forma.setOrigin(radio, radio);
    forma.setPosition(posicionInicial);
    forma.setFillColor(color);

    rapidezOriginal = (float)rapidez;
    velocidad = direccion * rapidezOriginal;

    activo = true;
    //Fenix:
    haHechoDano = false;
    esDanoContinuo = esDoT;
    //Djinn
    esErratico = erratico;     
    tiempoCambioDir = 0.0f;     // Inicializar temporizador
    atacante = propietario;
    dano = (float)danoHitbox;
    tiempoVida = (float)tiempodevida;
    //Basilisco:
    causaInmovilizacion = inmoviliza; 
    duracionCC = duracionControl;     
}

void Hitbox::ActualizarHitbox(double dt) {
    // LÓGICA DE MOVIMIENTO ERRÁTICO
    if (esErratico && activo) {
        tiempoCambioDir += (float)dt;
        if (tiempoCambioDir >= 0.5f) { // Cambia de rumbo cada 0.5 segundos
            // Generar dirección aleatoria
            float angulo = static_cast<float>(std::rand() % 360) * 3.14159f / 180.f;
            velocidad.x = std::cos(angulo) * rapidezOriginal;
            velocidad.y = std::sin(angulo) * rapidezOriginal;
            tiempoCambioDir = 0.0f;
        }
    }

    forma.move(velocidad * (float)dt);
    tiempoVida -= (float)dt;
    if (tiempoVida <= 0) activo = false;
}
void Hitbox::rebotar() {
    // Velocidad invertida para que rebote
    velocidad.x = -velocidad.x;
    velocidad.y = -velocidad.y;

    //Para que no se quede rebotando dentro de la pared
    forma.move(velocidad.x * 0.05f, velocidad.y * 0.05f);
}