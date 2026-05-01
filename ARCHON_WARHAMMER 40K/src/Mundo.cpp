#include "Mundo.h"
#include <iostream>
Mundo::Mundo() {
    // Inicializamos variables
    jugadorActual = 1;
    cicloActual = 1;
    rondaActual = 1;

    // 1. Creamos la pieza de la LUZ
    Pieza* luz = new Pieza();
    luz->setBando(Bando::LUZ);
    luz->setTipoMov(TipoMovimiento::VOLADOR);
    luz->setPatronMov(PatronMovimiento::ORTOGONAL);
    luz->setRango(4);
    luz->setPosicion(sf::Vector2i(0, 4));
    luz->setColor(sf::Color::Cyan);
    listaPiezas.push_back(luz);




    // 2. Creamos la pieza de la OSCURIDAD
    Pieza* oscuridad = new Pieza();
    oscuridad->setBando(Bando::OSCURIDAD);
    oscuridad->setTipoMov(TipoMovimiento::TERRESTRE);
    oscuridad->setPatronMov(PatronMovimiento::DIAGONAL);
    oscuridad->setRango(2);
    oscuridad->setPosicion(sf::Vector2i(8, 4));
    oscuridad->setColor(sf::Color::Red);
    listaPiezas.push_back(oscuridad);
}

// Destructor para evitar fugas de memoria
Mundo::~Mundo() {
    for (auto p : listaPiezas) {
        delete p;
    }
    listaPiezas.clear();
}


void Mundo::dibujar(sf::RenderWindow& window) {
    tablero.dibujar(window);
    for (auto p : listaPiezas) {
        p->dibujar(window);
    }
}

void Mundo::intentarAccionJugador(int idJugador) {
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

void Mundo::imprimirEstado() {
    std::cout << "Ronda " << rondaActual
        << " | Ciclo [" << cicloActual << "/12] | "
        << "Siguiente Turno: Jugador " << jugadorActual << std::endl;
}

void Mundo::manejarClick(sf::Vector2i mousePos) {
    int tableroX = (mousePos.x - 50) / 60;
    int tableroY = (mousePos.y - 30) / 60;

    if (tableroX < 0 || tableroX > 8 || tableroY < 0 || tableroY > 8) return;
    sf::Vector2i celdaClickeada(tableroX, tableroY);

    if (piezaSeleccionada == nullptr) {
        // --- BUSCAR PIEZA PARA SELECCIONAR ---
        for (auto p : listaPiezas) {
            if (p->getPosicion() == celdaClickeada) {
                // Comprobar turno (Luz = Jugador 1, Oscuridad = Jugador 2)
                if ((jugadorActual == 1 && p->getBando() == Bando::LUZ) ||
                    (jugadorActual == 2 && p->getBando() == Bando::OSCURIDAD)) {

                    piezaSeleccionada = p;
                    piezaSeleccionada->setSeleccionada(true); //activa el borde dorado
                }
                return;
            }
        }
    }
    else {
        // 1. Si el usuario hace click en la misma pieza que ya tiene seleccionada, la deselecciona
        if (celdaClickeada == piezaSeleccionada->getPosicion()) {
            piezaSeleccionada->setSeleccionada(false);
            piezaSeleccionada = nullptr;
        }
        else {
            // 2. Intentamos validar el movimiento con la nueva lógica de colisiones
            // Pasamos 'listaPiezas' para que pueda revisar si hay obstáculos en el camino
            if (piezaSeleccionada->poderMover(celdaClickeada, listaPiezas, false)) {

                // Si poderMover devuelve true, ejecutamos el movimiento
                piezaSeleccionada->setPosicion(celdaClickeada);

                // 3. Comprobar si hemos caído encima de un enemigo para el combate
                if (piezaSeleccionada->detectarConflicto(listaPiezas)) {
                    piezaSeleccionada->iniciarConflicto();
                    // Aquí podrías añadir lógica para eliminar la pieza defensora 
                    // o cambiar a la pantalla de batalla.
                }

                // 4. Limpieza: quitamos el borde amarillo y liberamos el puntero
                piezaSeleccionada->setSeleccionada(false);
                piezaSeleccionada = nullptr;

                // 5. Finalizar turno: Esto cambia de jugador y avanza el ciclo del tablero
                intentarAccionJugador(jugadorActual);
            }
            else {
                // Si poderMover devuelve false (por ser terrestre bloqueado o fuera de rango)
                std::cout << "Movimiento denegado: Camino bloqueado o fuera de rango." << std::endl;
            }
        }
    }
}
