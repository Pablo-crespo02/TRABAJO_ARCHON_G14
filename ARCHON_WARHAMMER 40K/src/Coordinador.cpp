    #include "Coordinador.h"

Coordinador::Coordinador()
    : motor(window, fuenteGlobal) 
{
    // Cargamos la fuente una sola vez
    if (!fuenteGlobal.loadFromFile("fuentes/fuente_pixel.ttf")) {
        std::cout << "Error critico: Fuente no encontrada" << std::endl;
    }

    // Creamos la ventana
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    window.create(desktop, "ARCHON WARHAMMER 40K", sf::Style::Fullscreen);

    // El estado inicial lo maneja el Coordinador
    estadoActual = Estado::MenuPrincipal;
    menuPausa = new MenuPausa(fuenteGlobal, window.getSize());

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
        // Detectar la tecla ESCAPE para pausar/reanudar en cualquier momento
        if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape) {
            // Si estamos jugando, guardamos el estado y pausamos
            if (estadoActual == Estado::Tablero || estadoActual == Estado::Arena) {
                estadoAnterior = estadoActual;
                estadoActual = Estado::Pausa;
            }
            // Si ya estamos en pausa, volvemos a donde estábamos
            else if (estadoActual == Estado::Pausa) {
                estadoActual = estadoAnterior;
            }
            // Si estás en Instrucciones o Créditos, volver al menú
            else if (estadoActual == Estado::Instrucciones || estadoActual == Estado::Creditos) {
                estadoActual = Estado::MenuPrincipal;
            }
        }

        // Gestión del menú principal
        if (estadoActual == Estado::MenuPrincipal) {
            if (evento.type == sf::Event::KeyPressed) {
                if (evento.key.code == sf::Keyboard::Up)    pantallaInicio.moverArriba();
                if (evento.key.code == sf::Keyboard::Down)  pantallaInicio.moverAbajo();

                if (evento.key.code == sf::Keyboard::Enter || evento.key.code == sf::Keyboard::Return) {
                    int seleccion = pantallaInicio.getIndiceSeleccionado();
                    switch (seleccion) {
                    case 0: // INICIAR PARTIDA
                        this->reiniciarPartida();
                        estadoActual = Estado::Tablero;
                        motor.setEstado(Estado::Tablero);
                        break;
                    case 1: // REANUDAR PARTIDA
                        estadoActual = Estado::Tablero;
                        motor.setEstado(Estado::Tablero);
                        break;
                    case 2: estadoActual = Estado::Instrucciones; break;
                    case 3: estadoActual = Estado::Creditos; break;
                    case 4: window.close(); break;
                    }
                }
            }
        }
        // Gestión de los botones dentro del Menú de Pausa
        else if (estadoActual == Estado::Pausa) {
            if (evento.type == sf::Event::KeyPressed) {
                if (evento.key.code == sf::Keyboard::Up)    menuPausa->moverArriba();
                if (evento.key.code == sf::Keyboard::Down)  menuPausa->moverAbajo();

                if (evento.key.code == sf::Keyboard::Enter || evento.key.code == sf::Keyboard::Return) {
                    int seleccion = menuPausa->getIndiceSeleccionado();
                    switch (seleccion) {
                    case 0: // REANUDAR
                        estadoActual = estadoAnterior;
                        break;
                    case 1: // REINICIAR PARTIDA
                        reiniciarPartida();
                        estadoActual = Estado::Tablero;
                        motor.setEstado(Estado::Tablero);
                        break;
                    case 2: // MENU PRINCIPAL
                        estadoActual = Estado::MenuPrincipal;
                        break;
                    case 3: // INSTRUCCIONES
                        estadoActual = Estado::Instrucciones;
                        break;
                    case 4: // GUARDAR PARTIDA (Falta tu lógica)
                        break;
                    case 5: // SALIR AL ESCRITORIO
                        window.close();
                        break;
                    }
                }
            }
        }
        // 4. Gestión del motor (Juego normal)
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
    // Si estamos jugando O en pausa, dibujamos el mundo
    else if (estadoActual == Estado::Tablero || estadoAnterior == Estado::Tablero && estadoActual == Estado::Pausa) {
        window.setView(vistaTablero);
        motor.renderizar();
        window.setView(vistaUI);
        motor.dibujarHUD();
    }
    else if (estadoActual == Estado::Arena || estadoAnterior == Estado::Arena && estadoActual == Estado::Pausa) {
        window.setView(vistaTablero);
        motor.renderizar();
    }

    // SI ES PAUSA, dibujamos el menú de pausa al final del todo (encima de todo)
    if (estadoActual == Estado::Pausa) {
        menuPausa->dibujar(window);
    }
    else if (estadoActual == Estado::Victoria) {
        window.setView(vistaUI);
        sf::Text textoVictoria;
        dibujarPantallaVictoria();
    }
    // 5. PANTALLA INSTRUCCIONES
    else if (estadoActual == Estado::Instrucciones) {
        window.setView(vistaUI); // Usamos la vista completa
        sf::Text textoInstrucciones;
        textoInstrucciones.setFont(fuenteGlobal);
        textoInstrucciones.setCharacterSize(35);
        textoInstrucciones.setFillColor(sf::Color::White);

        // El \n sirve para hacer saltos de línea en el texto
        textoInstrucciones.setString(
            "         OBJETIVO DE LA CRUZADA\n"
            "Domina los 5 Nodos de Poder o aniquila al enemigo.\n\n"
            "         FASE ESTRATEGICA (Tablero)\n"
            "- Raton (Click Izquierdo) para mover unidades.\n\n"
            "         FASE DE COMBATE (Arena)\n"
            "- IMPERIUM: WASD para mover. ESPACIO dispara. Q Hechizo.\n"
            "- XENOS: FLECHAS para mover. ENTER dispara. M Hechizo.\n\n\n"
            "      (Pulsa ESC para volver al menu)"
        );
        textoInstrucciones.setPosition(100.f, 150.f);
        window.draw(textoInstrucciones);
    }
    // 6. PANTALLA CRÉDITOS
    else if (estadoActual == Estado::Creditos) {
        window.setView(vistaUI);
        sf::Text textoCreditos;
        textoCreditos.setFont(fuenteGlobal);
        textoCreditos.setCharacterSize(35);
        textoCreditos.setFillColor(sf::Color::Yellow); // Color amarillo para que destaque

        // Aquí es donde añadimos vuestros nombres
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
            "      (Pulsa ESC para volver al menu)"
        );

        // Ajustamos un poco la posición para que quepan todos los nombres
        textoCreditos.setPosition(150.f, 120.f);
        window.draw(textoCreditos);
    }

    window.display();
}

void Coordinador::actualizar(float dt) {
    // Llamamos al motor solo con el tiempo
    motor.actualizar(dt);

    // Sincronizamos el estado para que el Coordinador sepa qué dibujar
    // (Asegúrate de tener el método getEstado() en Motor.h)
    if (motor.getEstado() == Estado::Tablero && estadoActual == Estado::Arena) {
        estadoActual = Estado::Tablero;
    }
    else if (motor.getEstado() == Estado::Arena && estadoActual == Estado::Tablero) {
        estadoActual = Estado::Arena;
    }
    else if (motor.getEstado()== Estado::Victoria && estadoActual != Estado::Victoria) {
        estadoActual = Estado::Victoria;
        std::cout << "Coordinador: ¡Detectada victoria en el Motor!" << std::endl;
    }
    if (estadoActual != Estado::Pausa) {
        motor.actualizar(dt);
        // ... el resto de tus sincronizaciones de victoria ...
    }
}

void Coordinador::reiniciarPartida() {
    // Solo limpiamos y regeneramos, no tocamos el estadoActual aquí
    motor.limpiarDatos();
    std::cout << "DEBUG: Datos del motor limpiados y unidades desplegadas." << std::endl;
}

void Coordinador::dibujarPantallaVictoria() {
    sf::Text textoVictoria;
    textoVictoria.setFont(fuenteGlobal);

    // Le preguntamos al motor quién ganó
    int ganador = motor.getGanador();

    if (ganador == 1) {
        textoVictoria.setString("VICTORIA DEL IMPERIUM");
        textoVictoria.setFillColor(sf::Color::Yellow);
    }
    else {
        textoVictoria.setString("VICTORIA XENOS");
        textoVictoria.setFillColor(sf::Color::Red);
    }
    //Establecemos el tamaño del texto:
    textoVictoria.setCharacterSize(80);

    // Centramos el texto principal
    sf::FloatRect textRect = textoVictoria.getLocalBounds();
    textoVictoria.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    textoVictoria.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 50.f);

    // Texto de "Pulsa Enter":
    sf::Text textoContinuar;
    textoContinuar.setFont(fuenteGlobal);
    textoContinuar.setString("Pulsa ENTER para volver al menu");
    textoContinuar.setCharacterSize(30);
    textoContinuar.setFillColor(sf::Color::White);

    // Centramos el texto secundario:
    sf::FloatRect contRect = textoContinuar.getLocalBounds();
    textoContinuar.setOrigin(contRect.left + contRect.width / 2.0f, contRect.top + contRect.height / 2.0f);
    textoContinuar.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f + 50.f);

    window.draw(textoVictoria);
    window.draw(textoContinuar);
}