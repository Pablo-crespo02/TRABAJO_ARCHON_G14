#include "ClaseLider.h"
#include "ClaseKnight.h"
#include "ClaseHelicoptero.h"
#include <cmath> 
#include <iostream>



ClaseLider::ClaseLider(Bando b, sf::Vector2i pos, std::string tipo)
    : PiezaTeletransporte(b, pos)
{
    //Estadísticas:
    this->stats.nombre = tipo;
    this->stats.vida = 50.0f;
    this->stats.vidaMaxima = 50.0f; // Ajusta a valores en el futuro
    this->stats.ataque = 12.0f;
    this->stats.defensa = 20.0f;
    this->stats.velAtaque = 0.9f;
    // Lógica de tipos:
    this->stats.esRango = true;    // El Lider es Rango

    this->rangoMovimiento = 5;
    this->patronMovimiento = PatronMovimiento::Ambos;
    this->tipoMov = TipoMovimiento::Teletransporte;  // Solo para el HUD

    //Tamaño de los sprites:
    this->piezaAlturaTablero = 80.0f;
    this->piezaAlturaArena = 150.0f;


    //Carga de los sprites:
    cargarConfigurarSprites(tipo);

    // Animación "invocando":
    if (animador) {
        animador->agreganAnimacion("INVOCANDO", 1, 2, 2, 0.4, true);
        animador->agreganAnimacion("ATAQUE", 1, 0, 1, 0.15f, true); //APAÑO spritesheet
    }
    // Carga sonido invocacines
    if (bufferMinionXeno.loadFromFile("sonidos/melee.mp3")) {
        sonidoMinionXeno.setBuffer(bufferMinionXeno);
        sonidoMinionXeno.setVolume(90.f);
    }

}

// Físicas y animación:
void ClaseLider::procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) {
    //Dejamos que la clase padre (PiezaTerrestre) mueva las coordenadas físicas
    PiezaTeletransporte::procesarMovimientoArena(direccion, dt, arena);

    //Actualizamos la imagen visible con nuestra máquina de estados
    if (this->stats.nombre == "CAPTAIN" || this->stats.nombre == "HIVE TYRANT") {
        animar(dt, direccion);
    }
}

void ClaseLider::animar(float dt, sf::Vector2f direccion) { // Sobreescrito por "animación exclusiva"
    if (!animador) return;

    // Leemos el reloj de la habilidad. Si hace menos de 0.4 segundos que pulsamos la M, mostramos la pose.
    bool estaInvocando = (this->stats.relojHabilidad.getElapsedTime().asSeconds() < 0.4f);
 
    if (estaInvocando) {
        animador->reproducir("INVOCANDO");
        actualizarAnimacion(dt);
    }

    else {
        Pieza::Animar(dt, direccion);
    } 
}

void ClaseLider::dibujar(sf::RenderWindow& window, Estado estadoActual) {
    if (estadoActual == Estado::Tablero) {
        this->sincronizarPosicionTablero();

        if (this->stats.nombre == "CAPTAIN" || this->stats.nombre == "HIVE TYRANT") {

            //Círculo de selección amarillo
            if (seleccionado) {
                dibujarAnilloSeleccion(window);
            }

            spriteTablero.setPosition(posicionAbsoluta);
            window.draw(spriteTablero);
        }
       
    }
    else if (estadoActual == Estado::Arena) {
        if (this->stats.nombre == "CAPTAIN" || this->stats.nombre == "HIVE TYRANT") {
            spriteArena.setPosition(posicionAbsoluta);
            window.draw(spriteArena);
        }
       
        // 2. Dibujo de los minions:
        for (Pieza* minion : minionsInvocados) {
            // Llamamos al propio método dibujar del minion (que al ser un Termagant ejecutará su sprite Chibi)
            minion->dibujar(window, estadoActual);
        }
        // Dibujo de la barra sobre la pieza:
        barrasArena.actualizar(stats.vida, stats.vidaMaxima, stats.velAtaque, posicionAbsoluta);
        barrasArena.dibujar(window);

    }
}

void ClaseLider::usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo) {
    if (!enemigo) return;

    this->stats.relojHabilidad.restart();
    limpiarMinions();

  //Líder del imperio invoca un helicóptero

    if (this->stats.nombre == "CAPTAIN") {
        // Spawnea un poco desplazado hacia arriba para simular que entra volando
        sf::Vector2f puntoSpawnHeli = this->posicionAbsoluta + sf::Vector2f(0.f, -50.f);

        ClaseHelicoptero* heli = new ClaseHelicoptero(this->bando, puntoSpawnHeli);
        minionsInvocados.push_back(heli);

        std::cout << "El Capitan ha solicitado apoyo aereo! Helicoptero desplegado." << std::endl;
    }
 
    //Líder tiránido invoca dos minions

    else {
        int numeroMinions = 2;
        float offsetsX[2] = { -40.f, 40.f };

        for (int i = 0; i < numeroMinions; i++) {
            sf::Vector2f puntoSpawn = this->posicionAbsoluta + sf::Vector2f(offsetsX[i], -30.f);

            ClaseKnight* minion = new ClaseKnight(this->bando, sf::Vector2i(0, 0), "TERMAGANT");

            minion->setPosicionAbsoluta(puntoSpawn);
            minion->setEscalaMinion(0.6f);

            minion->stats.vida = 15.0f;
            minion->stats.vidaMaxima = 15.0f;
            minion->stats.ataque = 3.0f;
            minion->stats.velAtaque = 1.0f;

            if (i == 0) {
                minion->stats.rol = 1; // Cazador
            }
            else {
                minion->stats.rol = 2; // Estratega
            }

            minionsInvocados.push_back(minion);
        }
        std::cout << "El lider enemigo ha invocado una horda de Termagants." << std::endl;
    }
}

void ClaseLider::actualizarMinions(float dt, Arena& arena, Pieza* enemigo, std::vector<Hitbox>& hitboxes) {
    if (!enemigo) return;

    auto it = minionsInvocados.begin();
    while (it != minionsInvocados.end()) {
        Pieza* minion = *it;

       //Comportamiento del helicóptero:
        ClaseHelicoptero* heliMinion = dynamic_cast<ClaseHelicoptero*>(minion);
        if (heliMinion != nullptr) {
            // Delegamos toda su IA de vuelo y disparo automático a su propia clase
            heliMinion->actualizarIA(dt, arena, enemigo, hitboxes);

            // Control de muerte del helicóptero
            if (heliMinion->stats.vida <= 0.f) {
                delete heliMinion;
                it = minionsInvocados.erase(it);
                std::cout << "El Helicoptero aliado ha sido derribado." << std::endl;
            }
            else {
                ++it;
            }
            continue; // Saltamos al siguiente minion, ignorando la lógica terrestre
        }

        //Comportamiento de los minions:
        sf::Vector2f posicionJugador = enemigo->getPosicionAbsoluta();
        sf::Vector2f puntoObjetivo = posicionJugador;

        sf::Vector2f dirRealJugador = posicionJugador - minion->getPosicionAbsoluta();
        float distanciaRealJugador = std::hypot(dirRealJugador.x, dirRealJugador.y);

        float rangoAtaque = 45.f;

        // 1. IA Comportamental
        if (minion->stats.rol == 2. && distanciaRealJugador > 120.f) {
            float factorAnticipacion = 100.f;
            sf::Vector2f dirEnemigo = enemigo->getultimadireccion();
            puntoObjetivo = posicionJugador + (dirEnemigo * factorAnticipacion);
        }

        // 2. Movimiento y Radar Anticolisión
        if (distanciaRealJugador > rangoAtaque) {
            sf::Vector2f dirHaciaObjetivo = puntoObjetivo - minion->getPosicionAbsoluta();
            float distanciaAlObjetivo = std::hypot(dirHaciaObjetivo.x, dirHaciaObjetivo.y);

            if (distanciaAlObjetivo > 0.f) {
                sf::Vector2f dirFinal = dirHaciaObjetivo / distanciaAlObjetivo;

                float distanciaRadar = 70.f;
                sf::Vector2f posicionAdelantada = minion->getPosicionAbsoluta() + (dirFinal * distanciaRadar);
                float radioMinion = 28.f;

                if (!arena.esPosicionValida(posicionAdelantada, radioMinion, false)) {
                    sf::Vector2f desvioLateral(-dirFinal.y, dirFinal.x);
                    sf::Vector2f pruebaDerecha = minion->getPosicionAbsoluta() + (desvioLateral * distanciaRadar);

                    if (arena.esPosicionValida(pruebaDerecha, radioMinion, false)) {
                        dirFinal = (dirFinal * 0.2f) + (desvioLateral * 0.8f);
                    }
                    else {
                        sf::Vector2f desvioIzquierda(dirFinal.y, -dirFinal.x);
                        dirFinal = (dirFinal * 0.2f) + (desvioIzquierda * 0.8f);
                    }

                    float largoDir = std::hypot(dirFinal.x, dirFinal.y);
                    if (largoDir > 0.f) dirFinal /= largoDir;
                }

                minion->procesarMovimientoArena(dirFinal, dt, arena);

                ClaseKnight* knightMinion = dynamic_cast<ClaseKnight*>(minion);
                if (knightMinion != nullptr) {
                    knightMinion->animar(dt, dirFinal);
                }
            }
        }

        // 3. Ataque Melee por Cooldown
        bool haAtacadoEnEsteFrame = false;

        if (distanciaRealJugador < rangoAtaque) {
            if (minion->stats.relojHitbox.getElapsedTime().asSeconds() >= minion->stats.velAtaque) {
                if (!enemigo->getInvulnerable()) {
                    enemigo->stats.vida -= minion->stats.ataque;
                    std::cout << "Un Termagant ha mordido al enemigo haciendo " << minion->stats.ataque << " de dano!" << std::endl;
                    sonidoMinionXeno.play();
                }
                minion->stats.relojHitbox.restart();
                haAtacadoEnEsteFrame = true;
            }
        }

        // Barra de ataque visual
        minion->gestionarBarraAtaqueMinion(
            minion->stats.vida,
            minion->stats.vidaMaxima,
            minion->stats.velAtaque,
            minion->getPosicionAbsoluta(),
            haAtacadoEnEsteFrame
        );

        // 4. Control de muerte Terrestre
        if (minion->stats.vida <= 0.f) {
            delete minion;
            it = minionsInvocados.erase(it);
            std::cout << "Un minion Termagant ha muerto en combate." << std::endl;
        }
        else {
            ++it;
        }
    }
}

void ClaseLider::limpiarMinions() {
    for (Pieza* minion : minionsInvocados) {
        delete minion;
    }
    minionsInvocados.clear();
}

ClaseLider::~ClaseLider() {
    limpiarMinions();
}