#include "ClaseLider.h"
#include "ClaseKnight.h"
#include <cmath> 
#include <iostream>

const float PIEZA_ALTURA_TABLERO = 80.0f;
const float PIEZA_ALTURA_ARENA = 150.0f;

ClaseLider::ClaseLider(Bando b, sf::Vector2i pos, std::string tipo)
    : PiezaTeletransporte(b, pos)
{
    // ESTADÍSTICAS 
    this->stats.nombre = tipo;
    this->stats.vida = 50.0f;
    this->stats.vidaMaxima = 50.0f; 
    this->stats.ataque = 12.0f;
    this->stats.defensa = 20.0f;
    this->stats.velAtaque = 0.9f;
    this->stats.esRango = true;

    this->rangoMovimiento = 5;
    this->patronMovimiento = PatronMovimiento::Ambos;
    this->tipoMov = TipoMovimiento::Teletransporte;

    // CARGA DE SPRITES
    if (tipo == "CAPTAIN" || tipo == "HIVE_TYRANT") {
        std::string rutaTablero = (tipo == "CAPTAIN") ? "imagenes/BASE-CAPTAIN-Humanidad.png" : "imagenes/BASE-TYRANT-TYRANIDS.png";
        std::string rutaArena = (tipo == "CAPTAIN") ? "imagenes/Chibi_Captain.png" : "imagenes/Chibi_Tyrant.png";

        if (!texturaTablero.loadFromFile(rutaTablero)) std::cout << "Error tablero Lider\n";
        if (!texturaArena.loadFromFile(rutaArena)) std::cout << "Error arena Lider\n";
    }

    spriteTablero.setTexture(texturaTablero);
    spriteTablero.setOrigin(texturaTablero.getSize().x / 2.f, texturaTablero.getSize().y / 2.f);
    spriteTablero.setScale(0.12f, 0.12f);

    spriteArena.setTexture(texturaArena);
    anchoFrame = texturaArena.getSize().x / 4;
    altoFrame = texturaArena.getSize().y / 4;
    spriteArena.setTextureRect(sf::IntRect(0, 0, anchoFrame, altoFrame));
    spriteArena.setOrigin(anchoFrame / 2.f, altoFrame / 2.f);
    spriteArena.setScale(1.5f, 1.5f);

    frameActual = 0;
    temporizadorAnimacion = 0.0f;
}

ClaseLider::~ClaseLider() {
    limpiarMinions();
}

void ClaseLider::procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) {
    PiezaTeletransporte::procesarMovimientoArena(direccion, dt, arena);
    animar(dt, direccion);
}

void ClaseLider::animar(float dt, sf::Vector2f direccion) {
    bool estaAtacando = (this->stats.relojHitbox.getElapsedTime().asSeconds() < 0.2f);
    int fila = 0;

    if (estaAtacando) {
        fila = 3;
    }
    else if (direccion.x != 0.0f || direccion.y != 0.0f) {
        if (std::abs(direccion.x) > std::abs(direccion.y)) {
            fila = 1;
        }
        else {
            fila = (direccion.y > 0) ? 0 : 2;
        }
    }

    temporizadorAnimacion += dt;
    if (temporizadorAnimacion >= 0.15f) {
        frameActual = (frameActual + 1) % 4;
        temporizadorAnimacion = 0.0f;
    }

    spriteArena.setTextureRect(sf::IntRect(frameActual * anchoFrame, fila * altoFrame, anchoFrame, altoFrame));

    float escalaArena = 1.5f;
    // CORRECCIÓN: Nombre de la función cambiado a getultimadireccion()
    if (this->getultimadireccion().x < 0) {
        spriteArena.setScale(-escalaArena, escalaArena);
    }
    else {
        spriteArena.setScale(escalaArena, escalaArena);
    }
}
void ClaseLider::dibujar(sf::RenderWindow& window, Estado estadoActual) {
    if (estadoActual == Estado::Tablero) {
        this->sincronizarPosicionTablero();
        spriteTablero.setPosition(posicionAbsoluta.x, posicionAbsoluta.y - 15.f);

        if (seleccionado) {
            sf::CircleShape base(30.f);
            base.setOrigin(30.f, 30.f);
            base.setPosition(posicionAbsoluta);
            base.setFillColor(sf::Color::Transparent);
            base.setOutlineThickness(3.f);
            base.setOutlineColor(sf::Color::Yellow);
            window.draw(base);
        }
        window.draw(spriteTablero);
    }
    else if (estadoActual == Estado::Arena) {
        spriteArena.setPosition(posicionAbsoluta);
        window.draw(spriteArena);

        barrasArena.actualizar(stats.vida, stats.vidaMaxima, stats.velAtaque, posicionAbsoluta);
        barrasArena.dibujar(window);

        // DIBUJAR MINIONS
        for (Pieza* minion : minionsInvocados) {
            minion->dibujar(window, Estado::Arena);
        }
    }
}

void ClaseLider::usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo) {
    if (!hechizoDisponible) return;

    std::string tipoMinion = (bando == Bando::LUZ) ? "INTERCESSOR" : "TERMAGANT";

    // Invocar 2 minions
    for(int i = 0; i < 2; i++) {
        sf::Vector2i posIrrelevante(0,0);
        ClaseKnight* nuevoMinion = new ClaseKnight(bando, posIrrelevante, tipoMinion);

        sf::Vector2f offset((i == 0 ? 40.f : -40.f), 40.f);
        nuevoMinion->setPosicionAbsoluta(this->posicionAbsoluta + offset);

        nuevoMinion->stats.vida = 10.f;
        nuevoMinion->stats.vidaMaxima = 10.f;
        nuevoMinion->stats.ataque = 4.f;

        minionsInvocados.push_back(nuevoMinion);
    }

    hechizoDisponible = false;
    std::cout << "El Lider ha invocado refuerzos!" << std::endl;
}

void ClaseLider::limpiarMinions() {
    for (Pieza* minion : minionsInvocados) {
        delete minion;
    }
    minionsInvocados.clear();
}

std::vector<Pieza*>& ClaseLider::getMinionsInvocados() {
    return minionsInvocados;
}

void ClaseLider::actualizarMinions(float dt, Arena& arena, Pieza* enemigo) {
    auto it = minionsInvocados.begin();
    while (it != minionsInvocados.end()) {
        Pieza* minion = *it;
        bool haAtacadoEnEsteFrame = false;

        // IA BÁSICA: Perseguir al enemigo
        sf::Vector2f dir = enemigo->getPosicionAbsoluta() - minion->getPosicionAbsoluta();
        float dist = std::hypot(dir.x, dir.y);
        
        if (dist > 35.f) { // Se mueve si está lejos
            dir /= dist; // Normalizar vector
            minion->procesarMovimientoArena(dir, dt, arena);
        } else if (minion->puedeAtacar()) { // Ataca si está cuerpo a cuerpo
            if (!enemigo->getInvulnerable()) {
                enemigo->stats.vida -= minion->stats.ataque;
                std::cout << "Un minion ha atacado haciendo " << minion->stats.ataque << " de dano!" << std::endl;
            }
            minion->reiniciarRelojHitbox();
            haAtacadoEnEsteFrame = true;
        }

        minion->gestionarBarraAtaqueMinion(
            minion->stats.vida,
            minion->stats.vidaMaxima,
            minion->stats.velAtaque,
            minion->getPosicionAbsoluta(),
            haAtacadoEnEsteFrame
        );

        // Control de muerte del minion
        if (minion->stats.vida <= 0.f) {
            delete minion;
            it = minionsInvocados.erase(it);
            std::cout << "Un minion ha muerto en combate." << std::endl;
        } else {
            ++it;
        }
    }
}