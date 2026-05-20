#include "Coordinador.h"
#include <fstream>   
#include <algorithm> 

Coordinador::Coordinador()
    : motor(window, fuenteGlobal)
{
    if (!fuenteGlobal.loadFromFile("fuentes/fuente_pixel.ttf")) {
        std::cout << "Error critico: Fuente no encontrada" << std::endl;
    }

    if (!bufferClick.loadFromFile("sonidos/click.mp3")) {
        std::cout << "Aviso: No se pudo cargar el sonido click.wav" << std::endl;
    }
    else {
        sonidoClick.setBuffer(bufferClick);
        sonidoClick.setVolume(50.f);
    }

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    window.create(desktop, "ARCHON WARHAMMER 40K", sf::Style::Fullscreen);

    pantallaCarga = new PantallaCarga(fuenteGlobal, window.getSize());
    menuPausa = new MenuPausa(fuenteGlobal, window.getSize());

    estadoActual = Estado::MenuPrincipal;
    vistaUI = window.getDefaultView();
    vistaTablero.setSize(700.f, 700.f);
    vistaTablero.setCenter(350.f, 350.f);
    vistaTablero.setViewport(sf::FloatRect(0.10f, 0.20f, 0.60f, 0.80f));
}

void Coordinador::ejecutar() {
    sf::Clock reloj;
    while (window.isOpen()) {
        float dt = reloj.restart().asSeconds();
        gestionarEventos();
        actualizar(dt);
        dibujar();
    }
}

void Coordinador::gestionarEventos() {
    sf::Event evento;
    while (window.pollEvent(evento)) {
        if (evento.type == sf::Event::Closed) window.close();

        if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape) {
            if (estadoActual == Estado::Tablero || estadoActual == Estado::Arena) {
                estadoAnterior = estadoActual;
                estadoActual = Estado::Pausa;
            }
            else if (estadoActual == Estado::Pausa) {
                estadoActual = estadoAnterior;
            }
            else if (estadoActual == Estado::Instrucciones || estadoActual == Estado::Creditos || estadoActual == Estado::SeleccionCarga || estadoActual == Estado::Historial) {
                estadoActual = Estado::MenuPrincipal;
            }
        }

        if (estadoActual == Estado::MenuPrincipal) {
            pantallaInicio.setPartidaActiva(partidaEnCurso);

            if (evento.type == sf::Event::KeyPressed) {
                if (evento.key.code == sf::Keyboard::Up)
                {
                    pantallaInicio.moverArriba();
                    sonidoClick.play();
                }
                if (evento.key.code == sf::Keyboard::Down)
                {
                    pantallaInicio.moverAbajo();
                    sonidoClick.play();
                }

                if (evento.key.code == sf::Keyboard::Enter || evento.key.code == sf::Keyboard::Return) {
                    int seleccion = pantallaInicio.getIndiceSeleccionado();
                    switch (seleccion) {
                    case 0:
                        this->reiniciarPartida();
                        partidaEnCurso = true;
                        estadoActual = Estado::Tablero;
                        motor.setEstado(Estado::Tablero);
                        break;
                    case 1:
                        if (partidaEnCurso) {
                            estadoActual = Estado::Tablero;
                            motor.setEstado(Estado::Tablero);
                        }
                        break;
                    case 2: estadoActual = Estado::Instrucciones; break;
                    case 3: estadoActual = Estado::Creditos; break;
                    case 4: window.close(); break;
                    case 5:
                        modoGuardar = false;
                        pantallaCarga->actualizarTextosRanuras(ranuras[0].ocupada, ranuras[1].ocupada, ranuras[2].ocupada);
                        estadoActual = Estado::SeleccionCarga;
                        break;
                    case 6:
                        estadoActual = Estado::Historial;
                        break;
                    }
                }
            }
        }
        else if (estadoActual == Estado::Pausa) {
            if (evento.type == sf::Event::KeyPressed) {
                if (evento.key.code == sf::Keyboard::Up)
                {
                    menuPausa->moverArriba();
                    sonidoClick.play();
                }
                if (evento.key.code == sf::Keyboard::Down)
                {
                    menuPausa->moverAbajo();
                    sonidoClick.play();
                }

                if (evento.key.code == sf::Keyboard::Enter || evento.key.code == sf::Keyboard::Return) {
                    int selPausa = menuPausa->getIndiceSeleccionado();
                    switch (selPausa) {
                    case 0: estadoActual = estadoAnterior; break;
                    case 1: reiniciarPartida(); estadoActual = Estado::Tablero; break;
                    case 2: estadoActual = Estado::MenuPrincipal; break;
                    case 3: estadoActual = Estado::Instrucciones; break;
                    case 4:
                        modoGuardar = true;
                        pantallaCarga->actualizarTextosRanuras(ranuras[0].ocupada, ranuras[1].ocupada, ranuras[2].ocupada);
                        estadoActual = Estado::SeleccionCarga;
                        break;
                    case 5: window.close(); break;
                    }
                }
            }
        }
        else if (estadoActual == Estado::SeleccionCarga) {
            if (evento.type == sf::Event::KeyPressed) {
                if (evento.key.code == sf::Keyboard::Up)
                {
                    pantallaCarga->moverArriba();
                    sonidoClick.play();
                }
                if (evento.key.code == sf::Keyboard::Down)
                {
                    pantallaCarga->moverAbajo();
                    sonidoClick.play();
                }

                if (evento.key.code == sf::Keyboard::Enter) {
                    int ranura = pantallaCarga->getIndiceSeleccionado();
                    if (ranura == 3) {
                        estadoActual = modoGuardar ? Estado::Pausa : Estado::MenuPrincipal;
                    }
                    else {
                        if (modoGuardar) {
                            this->guardarEnRanura(ranura);
                            pantallaCarga->actualizarTextosRanuras(ranuras[0].ocupada, ranuras[1].ocupada, ranuras[2].ocupada);
                            estadoActual = Estado::Pausa;
                        }
                        else {
                            this->cargarDesdeRanura(ranura);
                            partidaEnCurso = true;
                        }
                    }
                }
            }
        }
        else if (estadoActual == Estado::Victoria) {
            if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Enter) {
                estadoActual = Estado::MenuPrincipal;
                motor.reiniciarJuego();
            }
        }
        else {
            motor.gestionarEntrada(evento, vistaTablero);
        }
    }
}

void Coordinador::dibujar() {
    window.clear();

    if (estadoActual == Estado::MenuPrincipal) {
        pantallaInicio.dibujar(window);
    }
    else if (estadoActual == Estado::Tablero || (estadoAnterior == Estado::Tablero && estadoActual == Estado::Pausa)) {
        window.setView(vistaTablero);
        motor.renderizar();
        window.setView(vistaUI);
        motor.dibujarHUD();
    }
    else if (estadoActual == Estado::Arena || (estadoAnterior == Estado::Arena && estadoActual == Estado::Pausa)) {
        window.setView(vistaTablero);
        motor.renderizar();
    }

    if (estadoActual == Estado::Pausa) {
        menuPausa->dibujar(window);
    }
    else if (estadoActual == Estado::Victoria) {
        window.setView(vistaUI);
        pantallavictoria.dibujar(window);
    }
    else if (estadoActual == Estado::Instrucciones) {
        window.setView(vistaUI);
        sf::Text textoInstrucciones;
        textoInstrucciones.setFont(fuenteGlobal);
        textoInstrucciones.setCharacterSize(35);
        textoInstrucciones.setFillColor(sf::Color::White);

        textoInstrucciones.setString(
            "         OBJETIVO DE LA CRUZADA\n"
            "Domina los 5 Nodos de Poder o aniquila al enemigo.\n\n"
            "         FASE ESTRATEGICA (Tablero)\n"
            "- Raton (Click Izquierdo) para mover unidades.\n\n"
            "         FASE DE COMBATE (Arena)\n"
            "- IMPERIUM: WASD para mover. ESPACIO dispara. Q Hechizo.\n"
            "- XENOS: FLECHAS para mover. ENTER dispara. M Hechizo.\n\n\n"
            "      (Pulsa ESC para volver al Menu Principal)"
        );
        textoInstrucciones.setPosition(100.f, 150.f);
        window.draw(textoInstrucciones);
    }
    else if (estadoActual == Estado::SeleccionCarga) {
        pantallaInicio.dibujar(window);
        pantallaCarga->dibujar(window);
    }
    else if (estadoActual == Estado::Historial) {
        window.setView(vistaUI);

        sf::Text textoHistorial;
        textoHistorial.setFont(fuenteGlobal);
        textoHistorial.setCharacterSize(26);
        textoHistorial.setFillColor(sf::Color::White);
        textoHistorial.setOutlineThickness(1.5f);
        textoHistorial.setOutlineColor(sf::Color::Black);

        std::string registrosFormateados = "             === REGISTRO LOCAL DE COMBATES ===\n\n";

        std::ifstream archivo("historial.txt");
        if (archivo.is_open()) {
            std::string linea;
            std::vector<std::string> todasLasLineas;

            while (std::getline(archivo, linea)) {
                todasLasLineas.push_back(linea);
            }
            archivo.close();

            int primerRegistroAMostrar = std::max(0, static_cast<int>(todasLasLineas.size()) - 12);
            for (size_t i = primerRegistroAMostrar; i < todasLasLineas.size(); ++i) {
                registrosFormateados += " " + todasLasLineas[i] + "\n";
            }
        }
        else {
            registrosFormateados += "   No se registran combates en los archivos de la Cruzada.\n   ¡Juega una partida para inaugurar el historial!\n";
        }

        registrosFormateados += "\n\n       (Pulsa ESC para regresar al Menu Principal)";
        textoHistorial.setString(registrosFormateados);
        textoHistorial.setPosition(150.f, 120.f);
        window.draw(textoHistorial);
    }
    else if (estadoActual == Estado::Creditos) {
        window.setView(vistaUI);
        sf::Text textoCreditos;
        textoCreditos.setFont(fuenteGlobal);
        textoCreditos.setCharacterSize(35);
        textoCreditos.setFillColor(sf::Color::Yellow);

        textoCreditos.setString(
            "               DESARROLLO Y PROGRAMACION\n\n"
            "               Javier Monrio\n"
            "               Gonzalo Castro\n"
            "               Pablo Crespo\n"
            "               Javier Lerin\n"
            "               Cecilia Barrio\n\n\n"
            "               BASADO EN\n"
            "               Archon: The Light and the Dark (1983)\n\n\n"
            "               UNIVERSO Y LORE\n"
            "               Warhammer 40,000 (Games Workshop)\n\n\n"
            "      (Pulsa ESC para volver al Menu Principal)"
        );
        textoCreditos.setPosition(150.f, 120.f);
        window.draw(textoCreditos);
    }

    window.display();
}

void Coordinador::actualizar(float dt) {
    if (motor.getEstado() == Estado::Tablero && estadoActual == Estado::Arena) {
        estadoActual = Estado::Tablero;
    }
    else if (motor.getEstado() == Estado::Arena && estadoActual == Estado::Tablero) {
        estadoActual = Estado::Arena;
    }
    else if (motor.getEstado() == Estado::Victoria && estadoActual != Estado::Victoria) {
        estadoActual = Estado::Victoria;
        int ganador = motor.getGanador();
        pantallavictoria.configurarPantallaVictoria(ganador, window);
    }

    if (estadoActual != Estado::Pausa) {
        motor.actualizar(dt);
    }
}

void Coordinador::reiniciarPartida() {
    motor.limpiarDatos();
    std::cout << "DEBUG: Datos del motor limpiados y unidades desplegadas." << std::endl;
}

void Coordinador::guardarEnRanura(int indice) {
    for (Pieza* p : ranuras[indice].piezas) {
        delete p;
    }
    ranuras[indice].piezas.clear();

    std::vector<Pieza*> piezasActuales = motor.getListaPiezas();

    for (Pieza* p : piezasActuales) {
        ranuras[indice].piezas.push_back(p->clonar());
    }

    ranuras[indice].ronda = motor.getRondaActual();
    ranuras[indice].ciclo = motor.getCicloActual();
    ranuras[indice].jugador = motor.getJugadorActual();

    ranuras[indice].ocupada = true;
    std::cout << " Partida guardada con exito en la ranura " << indice + 1 << "!" << std::endl;
}

void Coordinador::cargarDesdeRanura(int indice) {
    if (ranuras[indice].ocupada) {
        motor.limpiarDatos();

        std::vector<Pieza*> piezasCargadas;
        for (Pieza* p : ranuras[indice].piezas) {
            piezasCargadas.push_back(p->clonar());
        }

        motor.setListaPiezas(piezasCargadas);
        motor.setRondaActual(ranuras[indice].ronda);
        motor.setCicloActual(ranuras[indice].ciclo);
        motor.setJugadorActual(ranuras[indice].jugador);

        estadoActual = Estado::Tablero;
        motor.setEstado(Estado::Tablero);
        std::cout << "Partida cargada desde la ranura " << indice + 1 << "!" << std::endl;
    }
    else {
        std::cout << "La ranura " << indice + 1 << " esta vacia." << std::endl;
    }
}