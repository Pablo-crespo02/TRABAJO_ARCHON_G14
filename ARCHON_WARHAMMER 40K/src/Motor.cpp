#include "Motor.h"
#include "Generador.h"
#include"Renderizador.h"
#include <iostream>
#include <vector>
#include<cmath>


Motor::Motor() {
    window.create(sf::VideoMode(anchopantalla, altopantalla), "ARCHON WARHAMMER 40K");
    window.setFramerateLimit(60); // Recomendable para no fundir la CPU
    jugadorActual = 1;
    cicloActual = 1;
    rondaActual = 1;
    estadoActual = Estado::MenuPrincipal;

    //LLAMADAS A INICIALIZACIONES, FUNCIONES DE LA CLASE 'GENERADOR':
    // 
    //Inicializa el tablero:
    Generador::GenerarTablero(tablero);

    //Inicicializa las piezas de cada ejército y la lista de piezas
    Generador::GenerarDespliegueUnidades(*this);
}

// Destructor para evitar fugas de memoria     || HACE FALTA?
Motor::~Motor() {
    for (auto p : listaPiezas) {
        delete p;
    }
    listaPiezas.clear();
};

void Motor::renderizar() {

    window.clear();
    //dibujar pantalla inicio
    if (estadoActual == Estado::MenuPrincipal) {
        pantallaInicio.dibujar(window);
    }
    if (estadoActual == Estado::Tablero) {
        // El motor decide que toca dibujar tablero
        Renderizador::dibujarTablero(window, tablero);
        for (auto p : listaPiezas) {
            Renderizador::dibujarPieza(window, p, estadoActual);
        }
    }
    else if (estadoActual == Estado::Arena) {
        // El motor decide que toca dibujar la arena de combate
        Renderizador::dibujarArena(window, arena);

        //Dibujar los proyectiles activos:
        for (const auto& p : proyectiles) {
            window.draw(p.getFormaProyectil());
        }

        // ¡CRÍTICO!: Solo dibujar si existen
        if (piezaAtacante != nullptr && piezaDefensor != nullptr) {
            Renderizador::dibujarPieza(window, piezaAtacante, estadoActual);
            Renderizador::dibujarPieza(window, piezaDefensor, estadoActual);
        }
    }
    window.display();
}



void Motor::intentarAccionJugador(int idJugador) {
    // Solo hacemos algo si el ID del jugador que pulsó coincide con el turno actual
    if (idJugador == jugadorActual) {

        std::cout << "Accion validada para Jugador " << idJugador << std::endl;

        if (jugadorActual == 1) {
            jugadorActual = 2;
        }
        else {
            // Si el 2 termina, reseteamos a 1 y avanzamos ciclo
            jugadorActual = 1;
            cicloActual++;

            if (cicloActual > 12) {
                cicloActual = 1;
                rondaActual++;
                std::cout << "\n--- NUEVA RONDA: " << rondaActual << " ---\n" << std::endl;
            }

            tablero.actualizarColores(cicloActual);
        }

        imprimirEstado();
    }
    else {
        //Avisar que no es su turno
        std::cout << "¡No es el turno del Jugador " << idJugador << "!" << std::endl;
    }
}
void Motor::iniciarCombate(Pieza* atacante, Pieza* defensor) {
    piezaAtacante = atacante;
    piezaDefensor = defensor;

    // Limpia estados de selección previos
    piezaAtacante->setSeleccionado(false);
    piezaDefensor->setSeleccionado(false);

    // Definine los puntos de spawn fijos
    sf::Vector2f spawnIzquierda(150.f, 300.f);
    sf::Vector2f spawnDerecha(650.f, 300.f);

    // LÓGICA DE POSICIONAMIENTO: Luz siempre a la izquierda
    if (piezaAtacante->getBando() == Bando::LUZ) {
        piezaAtacante->setPosicionAbsoluta(spawnIzquierda);
        piezaDefensor->setPosicionAbsoluta(spawnDerecha);
        //Inicialización:
        piezaAtacante->setultimadireccion(sf::Vector2f(1.f, 0.f));  // Luz mira a la derecha
        piezaDefensor->setultimadireccion(sf::Vector2f(-1.f, 0.f)); // Oscuridad mira a la izq
    }
    else {
        // Si el atacante es oscuridad, él va a la derecha y el defensor (Luz) a la izquierda
        piezaAtacante->setPosicionAbsoluta(spawnDerecha);
        piezaDefensor->setPosicionAbsoluta(spawnIzquierda);
        //Inicialización:
        // --- DIRECCIÓN INICIAL ---
        piezaAtacante->setultimadireccion(sf::Vector2f(-1.f, 0.f)); // Oscuridad mira a la izq
        piezaDefensor->setultimadireccion(sf::Vector2f(1.f, 0.f));  // Luz mira a la derecha

    }

    // Preparar la Arena
    sf::Color colorA = piezaAtacante->getColorVisual();
    sf::Color colorD = piezaDefensor->getColorVisual();

    // Usamos colores genéricos de SFML para que no te de error de "identificador no declarado"
    GeneradorArena::generarMapa(arena, sf::Color::White, sf::Color(50, 50, 50));

    estadoActual = Estado::Arena;
}
void Motor::imprimirEstado() {
    std::cout << "Ronda " << rondaActual
        << " | Ciclo [" << cicloActual << "/12] | "
        << "Siguiente Turno: Jugador " << jugadorActual << std::endl;
}

void Motor::manejarClick(sf::Vector2i mousePos) {
    if (estadoActual != Estado::Tablero) return;
    // Comprueba que está en el tablero
    std::cout << "DEBUG: Clic en pixeles (" << mousePos.x << "," << mousePos.y << ")" << std::endl;
    // Conversión de píxeles a coordenadas de tablero (0-8)
    int tableroX = (mousePos.x - 50) / 60;
    int tableroY = (mousePos.y - 30) / 60;

    std::cout << "DEBUG: Celda calculada [" << tableroX << "," << tableroY << "]" << std::endl;

    if (tableroX < 0 || tableroX > 8 || tableroY < 0 || tableroY > 8) {
        std::cout << "DEBUG: Clic fuera del tablero" << std::endl;
        return;
    }

    // Comprobamos si hay pieza en la casilla
    std::cout << "DEBUG: Buscando pieza en la celda... Total piezas: " << listaPiezas.size() << std::endl;

    sf::Vector2i celdaClickeada(tableroX, tableroY);

    if (piezaSeleccionada == nullptr) {
        // Accedemos directamente a la lista de piezas y sus miembros
        for (auto p : listaPiezas) {
            if (p->posicionTablero == celdaClickeada) {
                // Comprobación de turno usando acceso directo a p->bando
                if ((jugadorActual == 1 && p->bando == Bando::LUZ) ||
                    (jugadorActual == 2 && p->bando == Bando::OSCURIDAD)) {

                    piezaSeleccionada = p;
                    piezaSeleccionada->seleccionado = true; // Setter eliminado por acceso directo
                    std::cout << "Seleccionado: " << p->stats.nombre << std::endl;
                }
                else {
                    std::cout << "No puedes seleccionar piezas enemigas." << std::endl;
                }
                return;
            }
        }
    }
    else {
        // Tenemos una piza seleccionada

        // Deseleccionar si se clica en la misma casilla 
        // Si hemos escogido una pieza hay que volver a haver click para deseleccionarla
        if (celdaClickeada == piezaSeleccionada->posicionTablero) {
            piezaSeleccionada->seleccionado = false;
            piezaSeleccionada = nullptr;
            std::cout << "Pieza deseleccionada." << std::endl;
        }
        else {
            // Validar movimiento
            if (piezaSeleccionada->poderMover(celdaClickeada, listaPiezas, false)) {

                // Buscar si hay un enemigo en el destino
                Pieza* enemigo = nullptr;
                for (auto p : listaPiezas) {
                    if (p->posicionTablero == celdaClickeada) {
                        enemigo = p;
                        break;
                    }
                }

                if (enemigo != nullptr && enemigo->bando != piezaSeleccionada->bando) {
                    // Si es enemigo, iniciamos combate
                    iniciarCombate(piezaSeleccionada, enemigo);
                }
                else {
                    // Si la casilla está vacía, movemos
                    piezaSeleccionada->posicionTablero = celdaClickeada;
                    piezaSeleccionada->sincronizarPosicionTablero(); // Actualiza los píxeles visuales

                    std::cout << "Movimiento realizado con exito." << std::endl;

                    piezaSeleccionada->seleccionado = false;
                    piezaSeleccionada = nullptr;

                    intentarAccionJugador(jugadorActual);
                }
            }
            else {
                std::cout << "Movimiento denegado: Camino bloqueado o fuera de rango." << std::endl;
            }
        }
    }
}
void Motor::manejarEventos() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (estadoActual == Estado::MenuPrincipal)
        {
            if (event.type == sf::Event::KeyPressed) {
                // al pulsa enter, vamos al juego
                if (event.key.code == sf::Keyboard::Enter) {
                    estadoActual = Estado::Tablero;
                    std::cout << "Iniciando partida... ¡Al Tablero!" << std::endl;
                }
            }
        }
        if (event.type == sf::Event::Closed)
            window.close();

        // Captura los clicks en el tablero
        if (estadoActual == Estado::Tablero && event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                // Pasamos la posición del ratón RELATIVA a la ventana
                manejarClick(sf::Mouse::getPosition(window));
            }
        }

        // Control en la arena
        if (estadoActual == Estado::Arena) {
            if (event.type == sf::Event::KeyPressed) {
                // Salir del combate manualmente
                if (event.key.code == sf::Keyboard::Escape) {
                    estadoActual = Estado::Tablero;
                    std::cout << "Volviendo al Tablero..." << std::endl;
                }
            }

        }
    }
}


//GESTIÓN DEL TECLADO ARENA

void Motor::actualizar() {
    float dt = reloj.restart().asSeconds();

    // SÓLO APLICABLE EN LA ARENA
    if (estadoActual == Estado::Arena) {
        if (!piezaAtacante || !piezaDefensor) return;

        // Identificamos quién es quién para asignar controles
        Pieza* pLuz = (piezaAtacante->getBando() == Bando::LUZ) ? piezaAtacante : piezaDefensor;
        Pieza* pOsc = (piezaAtacante->getBando() == Bando::OSCURIDAD) ? piezaAtacante : piezaDefensor;

        // Movimiento Luz
        sf::Vector2f dirLuz(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) dirLuz.y -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) dirLuz.y += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) dirLuz.x -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) dirLuz.x += 1.f;

        // 1. Guardamos la última dirección en la que intentó moverse
        pLuz->setultimadireccion(dirLuz);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            if (pLuz->puedeDisparar()) {
                sf::Vector2f origen = pLuz->getPosicionAbsoluta();

                // 2. Extraemos la dirección de apuntado y la normalizamos
                sf::Vector2f dirDisparo = pLuz->getultimadireccion();
                float magnitud = std::sqrt(dirDisparo.x * dirDisparo.x + dirDisparo.y * dirDisparo.y);
                if (magnitud != 0) {
                    dirDisparo.x /= magnitud;
                    dirDisparo.y /= magnitud;
                }

                // 3. Asignamos 'dirDisparo' al nuevo proyectil
                proyectiles.emplace_back(origen, dirDisparo, 15, Colores::ColorProyectil, pLuz, pLuz->stats.ataque);
                pLuz->reiniciarRelojProyectil();
            }
        }
        pLuz->procesarMovimientoArena(dirLuz, dt, this->arena);

        // Movimiento oscuridad
        sf::Vector2f dirOsc(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) dirOsc.y -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) dirOsc.y += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) dirOsc.x -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) dirOsc.x += 1.f;

        // 1. Guardamos la última dirección en la que intentó moverse
        pOsc->setultimadireccion(dirOsc);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            if (pOsc->puedeDisparar()) {
                sf::Vector2f origen = pOsc->getPosicionAbsoluta();

                // 2. Extraemos la dirección de apuntado y la normalizamos
                sf::Vector2f dirDisparo = pOsc->getultimadireccion();
                float magnitud = std::sqrt(dirDisparo.x * dirDisparo.x + dirDisparo.y * dirDisparo.y);
                if (magnitud != 0) {
                    dirDisparo.x /= magnitud;
                    dirDisparo.y /= magnitud;
                }

                // 3. Asignamos 'dirDisparo' al nuevo proyectil
                proyectiles.emplace_back(origen, dirDisparo, 15, Colores::ColorProyectil, pOsc, pOsc->stats.ataque);
                pOsc->reiniciarRelojProyectil();
            }
        }
        pOsc->procesarMovimientoArena(dirOsc, dt, this->arena);

        // Proyectiles
        for (auto& p : proyectiles) {
            p.ActualizarProyectil();

            sf::Vector2f posP = p.getPosicionProyectil();
            float radioP = p.getFormaProyectil().getRadius();
            bool haImpactado = false;

            // Umbral de colisión circular: (Radio 16 + Radio 20)^2 = 1296
            float limiteColisionSq = 1300.f;

            if (p.getEstadoProyectil()) {
                // 1. Verificar impacto contra Atacante (si el proyectil no es suyo)
                if (piezaAtacante && p.getDisparador() != piezaAtacante) { //comprobacion de ¿eres tú el que dispara?
                    sf::Vector2f posE = piezaAtacante->getPosicionAbsoluta();
                    float distSq = std::pow(posP.x - posE.x, 2) + std::pow(posP.y - posE.y, 2);

                    if (distSq < limiteColisionSq) {
                        piezaAtacante->stats.vida -= p.getDano();
                        p.setEstadoProyectil(false);
                        haImpactado = true;
                    }
                }

                // 2. Verificar impacto contra Defensor (si no ha impactado ya)
                if (!haImpactado && piezaDefensor && p.getDisparador() != piezaDefensor) {
                    sf::Vector2f posE = piezaDefensor->getPosicionAbsoluta();
                    float distSq = std::pow(posP.x - posE.x, 2) + std::pow(posP.y - posE.y, 2);

                    if (distSq < limiteColisionSq) {
                        piezaDefensor->stats.vida -= p.getDano();
                        p.setEstadoProyectil(false);
                        haImpactado = true;
                    }
                }
            }

            // 3. Colisión con entorno (solo si no dio a una pieza)
            if (!haImpactado && p.getEstadoProyectil()) {
                if (!arena.esPosicionValida(posP, radioP, false)) {
                    p.setEstadoProyectil(false);
                }
            }
        }

        // Limpieza de proyectiles muertos
        proyectiles.erase(
            std::remove_if(proyectiles.begin(), proyectiles.end(), [](const Proyectil& p) {
                return !p.getEstadoProyectil();
                }), proyectiles.end());

        // Piezas muertas tras combate
        Pieza* ganador = nullptr;
        Pieza* perdedor = nullptr;

        if (piezaAtacante->stats.vida <= 0) {
            perdedor = piezaAtacante;
            ganador = piezaDefensor;
        }
        else if (piezaDefensor->stats.vida <= 0) {
            perdedor = piezaDefensor;
            ganador = piezaAtacante;
        }

        if (perdedor) {
            // Casilla del defensor
            sf::Vector2i destinoFinal = piezaDefensor->getPosicionTablero();

            // Eliminar pieza de la memoria y del vector
            auto it = std::find(listaPiezas.begin(), listaPiezas.end(), perdedor);
            if (it != listaPiezas.end()) {
                delete* it;
                listaPiezas.erase(it);
            }

            // El ganador se mueve a la casilla del defensor
            ganador->mover(destinoFinal);

            // Limpieza de estados
            piezaAtacante = nullptr;
            piezaDefensor = nullptr;
            piezaSeleccionada = nullptr;
            proyectiles.clear();

            // Volver al tablero
            estadoActual = Estado::Tablero;
            intentarAccionJugador(jugadorActual);
        }
    }
}