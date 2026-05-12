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
        // 1. Evento de cierre (siempre activo)
        if (evento.type == sf::Event::Closed) {
            window.close();
        }

        // 2. Lógica PARA EL MENÚ
        if (estadoActual == Estado::MenuPrincipal) {
            if (evento.type == sf::Event::KeyPressed) {
                // Navegación
                if (evento.key.code == sf::Keyboard::Up)    pantallaInicio.moverArriba();
                if (evento.key.code == sf::Keyboard::Down)  pantallaInicio.moverAbajo();

                // Selección
                if (evento.key.code == sf::Keyboard::Enter) {
                    int seleccion = pantallaInicio.getIndiceSeleccionado();
                     
                    switch (seleccion) {
                    case 0: // INICIAR PARTIDA
                        this->reiniciarPartida();
                        motor.setEstado(Estado::Tablero);
                        estadoActual = Estado::Tablero;
                        break;
                    case 1: // REANUDAR PARTIDA
                        estadoActual = Estado::Tablero;
                        break;
                    case 2: // INSTRUCCIONES
                        estadoActual = Estado::Instrucciones;
                        break;
                    case 3: // CREDITOS
                        estadoActual = Estado::Creditos;
                        break;
                    case 4: // SALIR
                        window.close();
                        break;
                    case 5: // CARGAR PARTIDA
                        // Tu lógica de carga aquí
                        break;
                    }
                    // Sincronizamos el cambio de estado con el motor
                    motor.setEstado(estadoActual);
                }
            }
        }
        
        // --- LÓGICA PARA VOLVER AL MENÚ DESDE VICTORIA ---
        else if (estadoActual == Estado::Victoria) {
            if (evento.type == sf::Event::KeyPressed) {
                if (evento.key.code == sf::Keyboard::Enter) {
                    estadoActual = Estado::MenuPrincipal;
                    motor.reiniciarJuego();
                }
            }
        }
        // --- LÓGICA PARA VOLVER AL MENÚ DESDE INSTRUCCIONES O CREDITOS ---
        else if (estadoActual == Estado::Instrucciones || estadoActual == Estado::Creditos) {
            if (evento.type == sf::Event::KeyPressed) {
                if (evento.key.code == sf::Keyboard::Escape || evento.key.code == sf::Keyboard::Enter) {
                    estadoActual = Estado::MenuPrincipal;
                    motor.setEstado(estadoActual);
                }
            }
        }
        else {
            // --- ESTADO TABLERO / ARENA / VICTORIA ---
            // Aquí delegamos al motor. 
            // Como estamos DENTRO del Coordinador, le pasamos nuestra 'vistaTablero' 
            // directamente al motor sin que la función de arriba necesite argumentos.
            motor.gestionarEntrada(evento, vistaTablero);
        }
    }
}

void Coordinador::dibujar() {
    window.clear();

    if (estadoActual == Estado::MenuPrincipal) {
        pantallaInicio.dibujar(window);
    }
    else if (estadoActual == Estado::Tablero) {
        window.setView(vistaTablero);
        motor.renderizar();

        window.setView(vistaUI);
        motor.dibujarHUD();
    }
    else if (estadoActual == Estado::Arena) {
        window.setView(vistaTablero); // O la vista que uses para arena
        motor.renderizar();

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
}
void Coordinador::reiniciarPartida() {
    // Le ordenamos al motor que se resetee
    motor.limpiarDatos();

    // El coordinador se encarga de la parte visual/navegación
    estadoActual = Estado::MenuPrincipal;

    // Opcional: Si quieres que empiece el juego directamente tras ganar:
    // estadoActual = Estado::Tablero; 
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