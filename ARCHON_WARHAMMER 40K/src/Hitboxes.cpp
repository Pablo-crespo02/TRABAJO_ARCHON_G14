#include "Hitboxes.h"
#include "Color.h"
#include <cmath>

Hitbox::Hitbox(sf::Vector2f posicionInicial, sf::Vector2f direccion, double rapidez, sf::Color color,
    Pieza* propietario, double danoHitbox, double tiempodevida, double radio,
    bool esDoT, bool erratico, bool inmoviliza, double duracionControl,
    bool esGranadaParam, double radioExp) {

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
    tiempoCambioDir = 0.0f;
    atacante = propietario;
    dano = (float)danoHitbox;
    tiempoVida = (float)tiempodevida;

    //Basilisco:
    causaInmovilizacion = inmoviliza;
    duracionCC = duracionControl;

    //Granada:
    this->esGranada = esGranadaParam;
    this->radioExplosion = (float)radioExp;
    this->danoExplosion = (float)(danoHitbox * 2.0);
    this->temporizadorVuelo = 0.5f;
    this->temporizadorSuelo = 1.5f;
    this->estaEnSuelo = false;
    this->explotando = false;
    this->yaDanoAtacante = false;
    this->yaDanoDefensor = false;
}

void Hitbox::ActualizarHitbox(double dt) {
    if (!activo) return;

    if (esGranada) {
        if (temporizadorVuelo > 0.0f) {
            temporizadorVuelo -= (float)dt;
            forma.move(velocidad * (float)dt);
            if (temporizadorVuelo <= 0.0f) {
                estaEnSuelo = true;
                velocidad = sf::Vector2f(0.f, 0.f);
            }
        }
        else if (temporizadorSuelo > 0.0f && !explotando) {
            temporizadorSuelo -= (float)dt;

            float parpadeo = std::fmod(temporizadorSuelo, 0.2f);
            if (parpadeo > 0.1f) {
                forma.setFillColor(sf::Color::Red);
            }
            else {
                forma.setFillColor(sf::Color(100, 100, 100));
            }

            if (temporizadorSuelo <= 0.0f) {
                Detonar();
            }
        }
        else if (explotando) {
            tiempoVida -= (float)dt;
            if (tiempoVida <= 0.0f) {
                activo = false;
            }
        }
        return;
    }

    if (esErratico && activo) {
        tiempoCambioDir += (float)dt;
        if (tiempoCambioDir >= 0.5f) {
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
    velocidad.x = -velocidad.x;
    velocidad.y = -velocidad.y;
    forma.move(velocidad.x * 0.05f, velocidad.y * 0.05f);
}

void Hitbox::Detonar() {
    if (!explotando) {
        explotando = true;
        estaEnSuelo = false;

        forma.setRadius(radioExplosion);
        forma.setOrigin(radioExplosion, radioExplosion);
        forma.setFillColor(sf::Color(255, 100, 0, 200));
        dano = danoExplosion;
        tiempoVida = 0.2f;
    }
}