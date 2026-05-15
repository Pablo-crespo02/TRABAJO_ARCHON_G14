#include "Coordinador.h"

Coordinador::Coordinador()
    : motor(window, fuenteGlobal)
{
    pantallaCarga = new PantallaCarga(fuenteGlobal, window.getSize());
    // Cargamos la fuente una sola vez
    if (!fuenteGlobal.loadFromFile("fuentes/fuente_pixel.ttf")) {
        std::cout << "Error critico: Fuente no encontrada" << std::endl;
    }

    // Creamos la ventana
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    window.create(desktop, "ARCHON WARHAMMER 40K", sf::Style::Fullscreen);

    // El estado inicial lo maneja el Coordinador
    pantallaCarga = new PantallaCarga(fuenteGlobal, window.getSize());
    menuPausa = new MenuPausa(fuenteGlobal, window.getSize());
    

    estadoActual = Estado::MenuPrincipal;
    // Inicializamos las vistas que pasaste
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

        // 1. Tecla ESCAPE (Pausa/Volver)
        if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape) {
            if (estadoActual == Estado::Tablero || estadoActual == Estado::Arena) {
                estadoAnterior = estadoActual;
                estadoActual = Estado::Pausa;
            }
            else if (estadoActual == Estado::Pausa) {
                estadoActual = estadoAnterior;
            }
            else if (estadoActual == Estado::Instrucciones || estadoActual == Estado::Creditos || estadoActual == Estado::SeleccionCarga) {
                estadoActual = Estado::MenuPrincipal;
            }
        }
        //MENÚ PRINCIPAL
        if (estadoActual == Estado::MenuPrincipal) {
            // Actualizamos la apariencia del botón "Reanudar" (gris o normal)
            pantallaInicio.setPartidaActiva(partidaEnCurso);

            if (evento.type == sf::Event::KeyPressed) {
                if (evento.key.code == sf::Keyboard::Up)    pantallaInicio.moverArriba();
                if (evento.key.code == sf::Keyboard::Down)  pantallaInicio.moverAbajo();

                if (evento.key.code == sf::Keyboard::Enter || evento.key.code == sf::Keyboard::Return) {
                    int seleccion = pantallaInicio.getIndiceSeleccionado();
                    switch (seleccion) {
                    case 0: // INICIAR PARTIDA
                        this->reiniciarPartida();
                        partidaEnCurso = true; // <--- Marcamos que ya hay juego activo
                        estadoActual = Estado::Tablero;
                        motor.setEstado(Estado::Tablero);
                        break;

                    case 1: // REANUDAR PARTIDA
                        if (partidaEnCurso) { // <--- Solo entra si hemos iniciado/cargado algo
                            estadoActual = Estado::Tablero;
                            motor.setEstado(Estado::Tablero);
                        }
                        break;

                    case 2: estadoActual = Estado::Instrucciones; break;
                    case 3: estadoActual = Estado::Creditos; break;

                    case 4: // SALIR DEL JUEGO
                        window.close();
                        break;

                    case 5: // CARGAR PARTIDA (Lleva al menú de ranuras)
                        modoGuardar = false;
                        // Actualizamos los textos para que ponga (VACÍA) o (DATOS)
                        pantallaCarga->actualizarTextosRanuras(ranuras[0].ocupada, ranuras[1].ocupada, ranuras[2].ocupada);
                        estadoActual = Estado::SeleccionCarga;
                        break;
                    }
                }
            }
        }

        //MENÚ DE PAUSA
        else if (estadoActual == Estado::Pausa) {
            if (evento.type == sf::Event::KeyPressed) {
                if (evento.key.code == sf::Keyboard::Up)    menuPausa->moverArriba();
                if (evento.key.code == sf::Keyboard::Down)  menuPausa->moverAbajo();

                if (evento.key.code == sf::Keyboard::Enter || evento.key.code == sf::Keyboard::Return) {
                    int selPausa = menuPausa->getIndiceSeleccionado();
                    switch (selPausa) {
                    case 0: estadoActual = estadoAnterior; break; // REANUDAR
                    case 1: reiniciarPartida(); estadoActual = Estado::Tablero; break; // REINICIAR
                    case 2: estadoActual = Estado::MenuPrincipal; break; // VOLVER AL MENU
                    case 3: estadoActual = Estado::Instrucciones; break; // INSTRUCCIONES

                    case 4: // GUARDAR PARTIDA (Lleva al menú de ranuras)
                        modoGuardar = true;
                        pantallaCarga->actualizarTextosRanuras(ranuras[0].ocupada, ranuras[1].ocupada, ranuras[2].ocupada);
                        estadoActual = Estado::SeleccionCarga;
                        break;

                    case 5: // SALIR AL ESCRITORIO
                        window.close();
                        break;
                    }
                }
            }
        }
        else if (estadoActual == Estado::SeleccionCarga) {
            if (evento.type == sf::Event::KeyPressed) {
                if (evento.key.code == sf::Keyboard::Up) pantallaCarga->moverArriba();
                if (evento.key.code == sf::Keyboard::Down) pantallaCarga->moverAbajo();

                if (evento.key.code == sf::Keyboard::Enter) {
                    int ranura = pantallaCarga->getIndiceSeleccionado();
                    if (ranura == 3) { // Opción "VOLVER"
                        // Volvemos a donde estábamos
                        estadoActual = modoGuardar ? Estado::Pausa : Estado::MenuPrincipal;
                    }
                    else {
                        if (modoGuardar) {
                            this->guardarEnRanura(ranura);
                            pantallaCarga->actualizarTextosRanuras(ranuras[0].ocupada, ranuras[1].ocupada, ranuras[2].ocupada);
                            estadoActual = Estado::Pausa; // Te devuelve a la pausa tras guardar
                        }
                        else {
                            this->cargarDesdeRanura(ranura);
                            partidaEnCurso = true; // ¡Iniciamos una partida!
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
            // Juego normal (Tablero o Arena)
            motor.gestionarEntrada(evento, vistaTablero);
        }
    } // Cierra el while
} // Cierra la función gestionarEventos

void Coordinador::dibujar() {
    window.clear();

    if (estadoActual == Estado::MenuPrincipal) {
        pantallaInicio.dibujar(window);
    }
    // Si estamos jugando O en pausa, dibujamos el mundo
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

    // SI ES PAUSA, dibujamos el menú de pausa al final del todo (encima de todo)
    if (estadoActual == Estado::Pausa) {
        menuPausa->dibujar(window);
    }
    else if (estadoActual == Estado::Victoria) {
        window.setView(vistaUI);
        pantallavictoria.dibujar(window);
    }
    // 5. PANTALLA INSTRUCCIONES
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
    //6. menu cargar ranuras
    else if (estadoActual == Estado::SeleccionCarga) {
        pantallaInicio.dibujar(window); // Dibujamos el fondo del marine y el tiranido
        pantallaCarga->dibujar(window); // Dibujamos las ranuras encima
    }
    // 7. PANTALLA CRÉDITOS
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
    // Sincronizamos el estado para que el Coordinador sepa qué dibujar
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
    
    // CORRECCIÓN: Ahora el motor SOLO se actualiza si NO estamos en pausa
    if (estadoActual != Estado::Pausa) {
        motor.actualizar(dt);
    }
}

void Coordinador::reiniciarPartida() {
    motor.limpiarDatos();
    std::cout << "DEBUG: Datos del motor limpiados y unidades desplegadas." << std::endl;
}


void Coordinador::guardarEnRanura(int indice) {
    // 1. Si la ranura ya tenía una partida vieja, limpiamos su memoria para no saturar la RAM
    for (Pieza* p : ranuras[indice].piezas) {
        delete p;
    }
    ranuras[indice].piezas.clear();

    // 2. Extraemos las piezas actuales del tablero
    std::vector<Pieza*> piezasActuales = motor.getListaPiezas();

    // 3. CLONACIÓN: Recorremos cada pieza y creamos una copia exacta en la ranura
    for (Pieza* p : piezasActuales) {
        ranuras[indice].piezas.push_back(p->clonar());
    }

    ranuras[indice].ocupada = true;
    std::cout << " Partida guardada con exito en la ranura " << indice + 1 << "!" << std::endl;
}

void Coordinador::cargarDesdeRanura(int indice) {
    if (ranuras[indice].ocupada) {
        // 1. Vaciamos el tablero actual usando tu función
        motor.limpiarDatos();

        // 2. CLONACIÓN INVERSA: Copiamos las piezas desde la ranura para enviarlas al motor
        // (Debemos clonarlas de nuevo, o la ranura se quedaría vacía tras jugar)
        std::vector<Pieza*> piezasCargadas;
        for (Pieza* p : ranuras[indice].piezas) {
            piezasCargadas.push_back(p->clonar());
        }

        // 3. Inyectamos los clones en el Motor
        motor.setListaPiezas(piezasCargadas);

        // 4. Cambiamos los estados para reanudar el juego
        estadoActual = Estado::Tablero;
        motor.setEstado(Estado::Tablero);
        std::cout << "Partida cargada desde la ranura " << indice + 1 << "!" << std::endl;
    }
    else {
        std::cout << "La ranura " << indice + 1 << " esta vacia." << std::endl;
    }
}
