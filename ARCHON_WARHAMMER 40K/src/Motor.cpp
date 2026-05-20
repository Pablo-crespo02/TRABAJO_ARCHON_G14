#include "Motor.h"
#include "Generador.h"
#include "Color.h"
#include <fstream>
#include <cmath>
#include <algorithm> 
#include <iostream>
#include <vector>

// 🚨 Visibilidad de clases para los casteos
#include "ClaseLider.h"
#include "ClaseFenix.h"
#include "ClaseDjinn.h"
#include "ClaseGolem.h"
#include "ClaseUnicornio.h"
#include "ClaseArcher.h"
#include "ClaseKnight.h"

Motor::Motor(sf::RenderWindow& win, sf::Font& fuente)
    : window(win), fuenteGlobal(fuente), hud(window, fuente)
{
    jugadorActual = 1;
    cicloActual = 1;
    rondaActual = 1;
    ganadorPartida = 0;
    piezaSeleccionada = nullptr;
    piezaAtacante = nullptr;
    piezaDefensor = nullptr;

    if (!bufferMover.loadFromFile("sonidos/mover.mp3")) {
        std::cout << "Aviso: No se pudo cargar el sonido mover.mp3" << std::endl;
    }
    else {
        sonidoMover.setBuffer(bufferMover);
        sonidoMover.setVolume(70.f);
    }

    if (!bufferError.loadFromFile("sonidos/error.mp3")) {
        std::cout << "Aviso: No se pudo cargar el sonido error.mp3" << std::endl;
    }
    else {
        sonidoError.setBuffer(bufferError);
        sonidoError.setVolume(70.f);
    }

    puntuacionImperium = 0;
    puntuacionTyranidos = 0;
    relojPartida.restart();
}

Motor::~Motor() {
    limpiarDatos();
}

void Motor::limpiarDatos() {
    for (Pieza* p : listaPiezas) {
        delete p;
    }
    listaPiezas.clear();
    jugadorActual = 1;
    rondaActual = 1;
    cicloActual = 1;
    ganadorPartida = 0;
    piezaSeleccionada = nullptr;
    piezaAtacante = nullptr;
    piezaDefensor = nullptr;

    puntuacionImperium = 0;
    puntuacionTyranidos = 0;
    relojPartida.restart();
}

void Motor::reiniciarJuego() {
    limpiarDatos();
    Generador::GenerarTablero(tablero);
    Generador::GenerarDespliegueUnidades(*this);
    estadoActual = Estado::Tablero;

    puntuacionImperium = 0;
    puntuacionTyranidos = 0;
    relojPartida.restart();
}

void Motor::guardarPartidaEnHistorial(std::string bandoGanador) {
    std::ofstream archivo("historial.txt", std::ios::app);
    if (archivo.is_open()) {
        int tiempoTotalSegundos = static_cast<int>(relojPartida.getElapsedTime().asSeconds());
        int minutos = tiempoTotalSegundos / 60;
        int segundos = tiempoTotalSegundos % 60;

        archivo << "Ganador: " << bandoGanador
            << " | Tiempo: " << minutos << "m " << segundos << "s"
            << " | Ptos Imperium: " << puntuacionImperium
            << " | Ptos Tyranidos: " << puntuacionTyranidos << "\n";
        archivo.close();
        std::cout << "DEBUG: Registro guardado en historial.txt" << std::endl;
    }
    else {
        std::cout << "Error: No se pudo escribir en historial.txt" << std::endl;
    }
}

void Motor::manejarClick(sf::Vector2i mousePos, const sf::View& vistaTablero) {
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos, vistaTablero);
    int col = static_cast<int>(worldPos.x / 60.f);
    int fila = static_cast<int>(worldPos.y / 60.f);

    if (col < 0 || col >= 9 || fila < 0 || fila >= 9) return;
    sf::Vector2i posClic(col, fila);

    Pieza* piezaClickeada = nullptr;
    for (Pieza* p : listaPiezas) {
        if (p->getPosicionTablero() == posClic) {
            piezaClickeada = p;
            break;
        }
    }

    if (piezaSeleccionada == nullptr) {
        if (piezaClickeada != nullptr) {
            Bando bandoActual = (jugadorActual == 1) ? Bando::LUZ : Bando::OSCURIDAD;
            if (piezaClickeada->getBando() == bandoActual) {
                piezaSeleccionada = piezaClickeada;
                piezaSeleccionada->setSeleccionado(true);
                sonidoMover.play();
            }
            else {
                sonidoError.play();
            }
        }
    }
    else {
        if (piezaClickeada != nullptr && piezaClickeada->getBando() == piezaSeleccionada->getBando()) {
            piezaSeleccionada->setSeleccionado(false);
            piezaSeleccionada = piezaClickeada;
            piezaSeleccionada->setSeleccionado(true);
            sonidoMover.play();
        }
        else {
            bool esOcupado = (piezaClickeada != nullptr);
            if (piezaSeleccionada->poderMover(posClic, listaPiezas, esOcupado)) {
                if (esOcupado) {
                    iniciarCombate(piezaSeleccionada, piezaClickeada);
                }
                else {
                    piezaSeleccionada->posicionTablero = posClic;
                    piezaSeleccionada->sincronizarPosicionTablero();
                    sonidoMover.play();
                    piezaSeleccionada->setSeleccionado(false);
                    piezaSeleccionada = nullptr;
                    intentarAccionJugador(jugadorActual);
                }
            }
            else {
                sonidoError.play();
                piezaSeleccionada->setSeleccionado(false);
                piezaSeleccionada = nullptr;
            }
        }
    }
}

void Motor::iniciarCombate(Pieza* atacante, Pieza* defensor) {
    piezaAtacante = atacante;
    piezaDefensor = defensor;

    sf::Color colorCasillaCombate = sf::Color::White;
    ColorActual colEnum = tablero.getcoloractualcasilla(defensor->getPosicionTablero());

    if (colEnum == ColorActual::Blanco_pico || colEnum == ColorActual::Blanco) colorCasillaCombate = sf::Color::White;
    else if (colEnum == ColorActual::Negro_pico || colEnum == ColorActual::Negro) colorCasillaCombate = sf::Color(50, 50, 50);
    else colorCasillaCombate = sf::Color(128, 128, 128);

    GeneradorArena::generarMapa(this->arena, sf::Color(200, 200, 200), sf::Color(80, 80, 80));

    if (piezaAtacante->getBando() == Bando::LUZ) {
        piezaAtacante->setPosicionAbsoluta(sf::Vector2f(200.f, 300.f));
        piezaDefensor->setPosicionAbsoluta(sf::Vector2f(600.f, 300.f));
    }
    else {
        piezaAtacante->setPosicionAbsoluta(sf::Vector2f(600.f, 300.f));
        piezaDefensor->setPosicionAbsoluta(sf::Vector2f(200.f, 300.f));
    }

    piezaAtacante->multiplicadorArena = 1.0f;
    piezaDefensor->multiplicadorArena = 1.0f;

    Hitboxes.clear();
    estadoActual = Estado::Arena;
}

void Motor::intentarAccionJugador(int idJugador) {
    jugadorActual = (jugadorActual == 1) ? 2 : 1;
    if (jugadorActual == 1) {
        cicloActual++;
        if (cicloActual > 12) cicloActual = 1;
        tablero.actualizarColores(cicloActual);
        rondaActual++;
    }
    VerificarVictoria();
}

void Motor::VerificarVictoria() {
    int piezasLuz = 0, piezasOscuridad = 0;
    int powerPointsLuz = 0, powerPointsOscuridad = 0;

    for (Pieza* p : listaPiezas) {
        if (p->getBando() == Bando::LUZ) {
            piezasLuz++;
            if (tablero.getpowerpoint(p->getPosicionTablero())) powerPointsLuz++;
        }
        else {
            piezasOscuridad++;
            if (tablero.getpowerpoint(p->getPosicionTablero())) powerPointsOscuridad++;
        }
    }

    if (piezasOscuridad == 0 || powerPointsLuz >= 5) {
        estadoActual = Estado::Victoria;
        ganadorPartida = 1;
        guardarPartidaEnHistorial("IMPERIUM");
    }
    else if (piezasLuz == 0 || powerPointsOscuridad >= 5) {
        estadoActual = Estado::Victoria;
        ganadorPartida = 2;
        guardarPartidaEnHistorial("TYRANIDOS");
    }
}

void Motor::renderizar() {
    if (estadoActual == Estado::Tablero) {
        tablero.dibujar(window);
        for (Pieza* p : listaPiezas) {
            p->dibujar(window, Estado::Tablero);
        }
    }
    else if (estadoActual == Estado::Arena) {
        window.draw(sf::RectangleShape(sf::Vector2f(800.f, 600.f)));
        piezaAtacante->dibujar(window, Estado::Arena);
        piezaDefensor->dibujar(window, Estado::Arena);

        for (const auto& hb : Hitboxes) {
            if (hb.getEstadoHitbox()) window.draw(hb.getFormaHitbox());
        }
    }
}

void Motor::dibujarHUD() {
    hud.dibujar(window, rondaActual, cicloActual, jugadorActual, piezaSeleccionada);
}

void Motor::gestionarEntrada(const sf::Event& evento, const sf::View& vistaTablero) {
    if (estadoActual == Estado::Tablero && evento.type == sf::Event::MouseButtonPressed) {
        if (evento.mouseButton.button == sf::Mouse::Left) {
            manejarClick(sf::Mouse::getPosition(window), vistaTablero);
        }
    }
}

// 🚨 CORRECCIÓN 3: Reintegramos el Input de forma limpia sin Lambdas extrañas
void Motor::procesarInput(Pieza* p, sf::Keyboard::Key arriba, sf::Keyboard::Key abajo,
    sf::Keyboard::Key izqda, sf::Keyboard::Key dcha,
    sf::Keyboard::Key ataque, sf::Vector2f dirPorDefecto, float dt)
{
    sf::Vector2f dir(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(arriba)) dir.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(abajo))  dir.y += 1.f;
    if (sf::Keyboard::isKeyPressed(izqda))  dir.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(dcha))   dir.x += 1.f;

    if (dir.x != 0.f || dir.y != 0.f) {
        p->setultimadireccion(dir);
    }

    if (sf::Keyboard::isKeyPressed(ataque) && p->puedeAtacar()) {
        sf::Vector2f dirAtaque = p->getultimadireccion();
        float magnitud = std::hypot(dirAtaque.x, dirAtaque.y);
        dirAtaque = (magnitud != 0.f) ? (dirAtaque / magnitud) : dirPorDefecto;

        sf::Vector2f puntoSpawnAtaque = p->getPosicionAbsoluta() + (dirAtaque * 35.f);

        if (p->stats.esRango) {
            Hitboxes.emplace_back(puntoSpawnAtaque, dirAtaque, 500, Colores::ColorProyectil, p, (p->stats.ataque * p->multiplicadorArena), 60, 15);
        }
        else {
            Hitboxes.emplace_back(puntoSpawnAtaque, dirAtaque, 0, Colores::ColorProyectil, p, (p->stats.ataque * p->multiplicadorArena), 0.2, 35);
        }

        p->reiniciarRelojHitbox();
    }

    p->procesarMovimientoArena(dir, dt, this->arena);
}

void Motor::actualizar(double dt) {
    if (estadoActual != Estado::Arena) return;

    Pieza* pLuz = (piezaAtacante->getBando() == Bando::LUZ) ? piezaAtacante : piezaDefensor;
    Pieza* pOsc = (piezaAtacante->getBando() == Bando::OSCURIDAD) ? piezaAtacante : piezaDefensor;

    // Llamamos a la función limpia de los controles para el combate
    procesarInput(pLuz, sf::Keyboard::W, sf::Keyboard::S, sf::Keyboard::A, sf::Keyboard::D, sf::Keyboard::Space, sf::Vector2f(1, 0), static_cast<float>(dt));
    procesarInput(pOsc, sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Left, sf::Keyboard::Right, sf::Keyboard::Enter, sf::Vector2f(-1.f, 0.f), static_cast<float>(dt));

    // Hechizos
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && pLuz->getHechizoDisponible()) {
        pLuz->usarHechizo(Hitboxes, pOsc);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::M) && pOsc->getHechizoDisponible()) {
        pOsc->usarHechizo(Hitboxes, pLuz);
    }

    pLuz->actualizarMinions(static_cast<float>(dt), arena, pOsc);
    pOsc->actualizarMinions(static_cast<float>(dt), arena, pLuz);

    // Colisiones de hitboxes
    for (auto& hb : Hitboxes) {
        if (!hb.getEstadoHitbox()) continue;
        hb.ActualizarHitbox(dt);

        Pieza* enemigo = (hb.getAtacante()->getBando() == Bando::LUZ) ? pOsc : pLuz;
        sf::Vector2f posE = enemigo->getPosicionAbsoluta();
        sf::Vector2f posH = hb.getPosicionHitbox();

        float dist = std::hypot(posE.x - posH.x, posE.y - posH.y);
        if (dist < limitecolision) {
            if (!enemigo->getInvulnerable() && !hb.getYaDanoDefensor()) {
                enemigo->stats.vida -= hb.getDano();
                hb.Detonar();
            }
        }
    }

    // Resolucion de combate y fin de partida
    if (piezaAtacante->stats.vida <= 0.f || piezaDefensor->stats.vida <= 0.f) {
        Pieza* perdedor = (piezaAtacante->stats.vida <= 0.f) ? piezaAtacante : piezaDefensor;
        Pieza* ganador = (perdedor == piezaAtacante) ? piezaDefensor : piezaAtacante;

        // Calcular puntos del historial
        int puntosAsignados = 0;
        if (dynamic_cast<ClaseLider*>(perdedor))          puntosAsignados = 2000;
        else if (dynamic_cast<ClaseFenix*>(perdedor))     puntosAsignados = 750;
        else if (dynamic_cast<ClaseDjinn*>(perdedor))     puntosAsignados = 750;
        else if (dynamic_cast<ClaseGolem*>(perdedor))     puntosAsignados = 300;
        else if (dynamic_cast<ClaseUnicornio*>(perdedor)) puntosAsignados = 300;
        else if (dynamic_cast<ClaseArcher*>(perdedor))    puntosAsignados = 150;
        else if (dynamic_cast<ClaseKnight*>(perdedor))    puntosAsignados = 100;
        else puntosAsignados = 300;

        if (ganador->getBando() == Bando::LUZ) puntuacionImperium += puntosAsignados;
        else puntuacionTyranidos += puntosAsignados;

        // Limpieza fundamental antes de devolverlos al tablero
        perdedor->limpiarMinions();
        ganador->limpiarMinions();

        ganador->stats.vida = ganador->stats.vidaMaxima;
        ganador->setHechizoDisponible(true);

        sf::Vector2i destinoFinal = perdedor->getPosicionTablero();

        listaPiezas.erase(std::remove(listaPiezas.begin(), listaPiezas.end(), perdedor), listaPiezas.end());
        delete perdedor;

        ganador->posicionTablero = destinoFinal;
        ganador->sincronizarPosicionTablero();
        ganador->setSeleccionado(false);

        piezaSeleccionada = nullptr;
        piezaAtacante = nullptr;
        piezaDefensor = nullptr;

        estadoActual = Estado::Tablero;
        intentarAccionJugador(jugadorActual);
    }
}