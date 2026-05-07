#include "Motor.h"
#include "Generador.h"
#include"Renderizador.h"
#include <iostream>
#include <vector>


Motor::Motor() {
    window.create(sf::VideoMode(800, 600), "ARCHON WARHAMMER 40K");
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
    }
    else {
        // Si el atacante es oscuridad, él va a la derecha y el defensor (Luz) a la izquierda
        piezaAtacante->setPosicionAbsoluta(spawnDerecha);
        piezaDefensor->setPosicionAbsoluta(spawnIzquierda);
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

    //SÓLO APLICABLE EN LA ARENA:
    if (estadoActual == Estado::Arena) {
        if (!piezaAtacante || !piezaDefensor) return;

        // Identificamos quién es quién solo para asignar los controles
        Pieza* pLuz = (piezaAtacante->getBando() == Bando::LUZ) ? piezaAtacante : piezaDefensor;
        Pieza* pOsc = (piezaAtacante->getBando() == Bando::OSCURIDAD) ? piezaAtacante : piezaDefensor;

        // Capturar intención de movimiento Luz (WASD)
        sf::Vector2f dirLuz(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) dirLuz.y -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) dirLuz.y += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) dirLuz.x -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) dirLuz.x += 1.f;

        //Capturar intención de disparo proyectil Luz (ESPACIO)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            if (pLuz->puedeDisparar()) {
                sf::Vector2i OrigenDisparo = pLuz->getPosicionAbsoluta();

                //Definición de la dirección del proyectil:
                sf::Vector2i direccionProyectilLuz(1, 0); //Provisionalmente va hacia la dcha.
                proyectiles.emplace_back(OrigenDisparo, direccionProyectilLuz, 15, Colores::ColorProyectil); //Añade un ítem al contenedor de proyectiles
                pLuz->reiniciarRelojProyectil();
            }
        }

        // Ordena a la pieza que se mueva ella misma
        pLuz->procesarMovimientoArena(dirLuz, dt, this->arena);

        // Capturar intención de movimiento Oscuridad (Flechas)
        sf::Vector2f dirOsc(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    dirOsc.y -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  dirOsc.y += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  dirOsc.x -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) dirOsc.x += 1.f;

        //Capturar disparo proyectil pieza Oscuridad (ENTER)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            if (pOsc->puedeDisparar()) {
                sf::Vector2i OrigenDisparo = pOsc->getPosicionAbsoluta();

                //Definición de la dirección del proyectil:
                sf::Vector2i direccionProyectilOscuridad(-1, 0); //Provisionalmente va hacia la dcha.
                proyectiles.emplace_back(OrigenDisparo, direccionProyectilOscuridad, 15, Colores::ColorProyectil); //Añade un ítem al contenedor de proyectiles
                pOsc->reiniciarRelojProyectil();
            }
        }

        // Ordena a la pieza que se mueva ella misma
        pOsc->procesarMovimientoArena(dirOsc, dt, this->arena);

        //Ordena a todos los proyectiles exixstentes que se actualicen:
        for (auto& p : proyectiles) {
            p.ActualizarProyectil();
        }
    }
}
