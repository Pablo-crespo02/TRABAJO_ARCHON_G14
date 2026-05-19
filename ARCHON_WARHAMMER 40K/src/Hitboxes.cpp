#include "Hitboxes.h"
#include "Color.h"

//Constructor actualizado
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
    tiempoCambioDir = 0.0f;     // Inicializar temporizador
    atacante = propietario;
    dano = (float)danoHitbox;
    tiempoVida = (float)tiempodevida;
    //Basilisco:
    causaInmovilizacion = inmoviliza; 
    duracionCC = duracionControl;     
    //Granada:
  
    this->esGranada = esGranadaParam;
    this->radioExplosion = (float)radioExp;
    this->danoExplosion = (float)danoHitbox;

    // Parpadea en el suelo:
    this->estaEnSuelo = false;
    this->explotando = false;
    // 1.Vuela 0.5 segundos
    this->temporizadorVuelo = 0.5f;
    // 2. Parpadea 1.5 segundos.
    this->temporizadorSuelo = 1.5f;

    if (this->esGranada) {
        // No hace daño antes de detonar
        this->dano = 0.0f;
    }
    else {
        this->dano = (float)danoHitbox;
    }
    explotando = false;
    yaDanoAtacante = false;
    yaDanoDefensor = false;
}

void Hitbox::ActualizarHitbox(double dt) {

    // 3 estados de la granada:
    if (esGranada && activo) {

        // 1. La granada está volando
        if (!estaEnSuelo && !explotando) {
            temporizadorVuelo -= (float)dt;
            forma.move(velocidad * (float)dt); // La granada avanza

            // ¿Se acabó el tiempo de vuelo? Pasamos a la Fase 2.
            if (temporizadorVuelo <= 0.0f) {
                estaEnSuelo = true;
                velocidad = { 0.f, 0.f }; // Frenamos la granada en seco en el suelo
            }
        }

        // 2. Parpadea en el suelo
        else if (estaEnSuelo && !explotando) {
            temporizadorSuelo -= (float)dt;
            // Cambia de color cada 0.2 segundos
            if (std::fmod(temporizadorSuelo, 0.4f) < 0.2f) {
                forma.setFillColor(sf::Color::Red);
            }
            else {
                forma.setFillColor(sf::Color(80, 80, 80)); // Gris oscuro armada
            }

            // ¿Se acabó el tiempo de espera? Pasamos a la Fase 3.
            if (temporizadorSuelo <= 0.0f) {
                estaEnSuelo = false;
                Detonar(); 
            }
        }
        // 3.Explosión
        if (explotando && tiempoVida < 0.15f) {
            dano = 0.0f; // Quitamos el daño mortal casi de inmediato
        }

        tiempoVida -= (float)dt;
        if (tiempoVida <= 0.0f) activo = false; // La eliminamos de la arena

        return; // Salimos para saltarnos las físicas estándar de proyectiles normales
    }
    // Movimiento errático (Tornado Djinn
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
void Hitbox::Detonar() {
    if (!explotando) {
        explotando = true;
        estaEnSuelo = false;

        dano = danoExplosion; // Restauramos el daño mortal de la granada
        haHechoDano = false; 

        // Transformamos el proyectil en la onda expansiva gigante
        forma.setRadius(radioExplosion);
        forma.setOrigin(radioExplosion, radioExplosion);
        forma.setFillColor(sf::Color(255, 120, 0, 180));

        tiempoVida = 0.2f;
        velocidad = { 0.f, 0.f };
    }
}