#include "Motor.h"
#include "Generador.h"
#include"Renderizador.h"
#include <iostream>


Motor::Motor() {
    window.create(sf::VideoMode(800, 600), "ARCHON WARHAMMER 40K");
    window.setFramerateLimit(60); // Recomendable para no fundir la CPU
    jugadorActual = 1;
    cicloActual = 1;
    rondaActual = 1;
    estadoActual = Estado::Tablero;
    //LLAMADAS A INICIALIZACIONES, FUNCIONES DE LA CLASE 'GENERADOR':

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

    // Definimos los puntos de spawn fijos
    sf::Vector2f spawnIzquierda(150.f, 300.f);
    sf::Vector2f spawnDerecha(650.f, 300.f);

    // LÓGICA DE POSICIONAMIENTO POR BANDO
    if (piezaAtacante->bando == Bando::LUZ) {
        // Atacante es Luz (Izquierda), Defensor es Oscuridad (Derecha)
        piezaAtacante->posicionAbsoluta = spawnIzquierda;
        piezaDefensor->posicionAbsoluta = spawnDerecha;
    }
    else {
        // Atacante es Oscuridad (Derecha), Defensor es Luz (Izquierda) //Lo tenemos que quitar, luz siempre izquierda
        piezaAtacante->posicionAbsoluta = spawnDerecha;
        piezaDefensor->posicionAbsoluta = spawnIzquierda;
    }
    sf::Color colorA = piezaAtacante->formaVisual.getFillColor();
    sf::Color colorD = piezaDefensor->formaVisual.getFillColor();
    // Generar el escenario
    arena.prepararSpawns(colorA, colorD);
    arena.generarMapaProcedural();

    // Cambiar el estado para que el Motor empiece a usar la lógica de Arena
    estadoActual = Estado::Arena;
}
void Motor::imprimirEstado() {
    std::cout << "Ronda " << rondaActual
        << " | Ciclo [" << cicloActual << "/12] | "
        << "Siguiente Turno: Jugador " << jugadorActual << std::endl;
}

void Motor::manejarClick(sf::Vector2i mousePos) {
    if (estadoActual != Estado::Tablero) return;
    //Comprueba que está en el tablero
    std::cout << "DEBUG: Clic en pixeles (" << mousePos.x << "," << mousePos.y << ")" << std::endl;
    // Conversión de píxeles a coordenadas de tablero (0-8)
    int tableroX = (mousePos.x - 50) / 60;
    int tableroY = (mousePos.y - 30) / 60;

    std::cout << "DEBUG: Celda calculada [" << tableroX << "," << tableroY << "]" << std::endl;

    if (tableroX < 0 || tableroX > 8 || tableroY < 0 || tableroY > 8) {
        std::cout << "DEBUG: Clic fuera del tablero" << std::endl;
        return;
    }

    // comprobamos si hay pieza en la casilla
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
        //Si hemos escogido una pieza hay que volver a haver click para deseleccionarla
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
void Motor::actualizar() {
    // 1. Siempre reiniciamos el reloj al principio para tener el dt disponible
    float dt = reloj.restart().asSeconds();

    if (estadoActual == Estado::Tablero) {
        // Lógica de clics, selección de piezas, etc.
    }
    else if (estadoActual == Estado::Arena) {
        //esntramos al movimiento de la arena
        if (!piezaAtacante || !piezaDefensor) return;

        float velocidadBase = 250.f;

        // Vemos si es luminosa u oscura
        // (Esto nos permite asignar teclas fijas a cada bando)
        Pieza* pLuz = (piezaAtacante->bando == Bando::LUZ) ? piezaAtacante : piezaDefensor;
        Pieza* pOsc = (piezaAtacante->bando == Bando::OSCURIDAD) ? piezaAtacante : piezaDefensor;

        // Movimiento bando luminoso
        sf::Vector2f movLuz(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movLuz.y -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movLuz.y += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) movLuz.x -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movLuz.x += 1.f;

        if (movLuz != sf::Vector2f(0.f, 0.f)) {
            float dx = movLuz.x * velocidadBase * dt;
            float dy = movLuz.y * velocidadBase * dt;

            bool luzVuela = (pLuz->tipoMov == TipoMovimiento::VOLADOR);
            // PASAMOS EL ATRIBUTO voladora AQUÍ:
            if (arena.esPosicionValida(pLuz->posicionAbsoluta + sf::Vector2f(dx, dy), 20.f, luzVuela)) {
                pLuz->moverEnArena(dx, dy);
            }
        }

        // Movimiento bando oscuridad
        sf::Vector2f movOsc(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    movOsc.y -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  movOsc.y += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  movOsc.x -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) movOsc.x += 1.f;

        // Para el bando Oscuridad
        if (movOsc != sf::Vector2f(0.f, 0.f)) {
            float dx = movOsc.x * velocidadBase * dt;
            float dy = movOsc.y * velocidadBase * dt;

            bool oscVuela = (pOsc->tipoMov == TipoMovimiento::VOLADOR);

            if (arena.esPosicionValida(pOsc->posicionAbsoluta + sf::Vector2f(dx, dy), 20.f, oscVuela)) {
                pOsc->moverEnArena(dx, dy);
            }
        }
    }
}
