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
        if (evento.type == sf::Event::Closed) window.close();

        if (evento.type == sf::Event::KeyPressed) {
            // --- ESTADO MENU PRINCIPAL ---
            if (estadoActual == Estado::MenuPrincipal) {
                if (evento.key.code == sf::Keyboard::Up)    pantallaInicio.moverArriba();
                if (evento.key.code == sf::Keyboard::Down)  pantallaInicio.moverAbajo();

                if (evento.key.code == sf::Keyboard::Enter) {
                    int seleccion = pantallaInicio.getIndiceSeleccionado();

                    switch (seleccion) {
                    case 0: // INICIAR PARTIDA
                        this->reiniciarPartida(); // Tu función de limpieza
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
                    case 4: // SALIR (En tu string es la opción 4)
                        window.close();
                        break;
                    case 5: // CARGAR PARTIDA
                        // AQUI EL CODIGO DE CARGAR PARTIDA
                        break;
                    }
                    // Sincronizamos el estado con el motor
                    motor.setEstado(estadoActual);
                }
            }
            // --- OTROS ESTADOS (VOLVER AL MENU) ---
            else if (estadoActual == Estado::Instrucciones || estadoActual == Estado::Creditos) {
                if (evento.key.code == sf::Keyboard::Escape) {
                    estadoActual = Estado::MenuPrincipal;
                }
            }
            // --- JUEGO (DELEGAMOS AL MOTOR) ---
            else {
                motor.gestionarEntrada(evento);
            }
        }
    }
}

void Coordinador::dibujar() {
    window.clear();

    // 1. PANTALLA DE INICIO
    if (estadoActual == Estado::MenuPrincipal) {
        window.setView(vistaUI);
        // Pasamos el índice seleccionado al dibujar
        motor.pantallaInicio.dibujar(window);
    }
    // 2. MODO TABLERO
    else if (estadoActual == Estado::Tablero) {
        window.setView(vistaTablero);
        motor.renderizar(); // Dibuja tablero y piezas

        window.setView(vistaUI);
        motor.dibujarHUD(); // HUD siempre en vista UI
    }
    // 3. MODO ARENA
    else if (estadoActual == Estado::Arena) {
        window.setView(vistaTablero);
        motor.renderizar(); // Dibuja arena y combatientes

        // Si quieres HUD en la arena, descomenta la siguiente línea:
        // window.setView(vistaUI); motor.dibujarHUD();
    }
    // 4. PANTALLA DE VICTORIA
    else if (estadoActual == Estado::Victoria) {
        window.setView(vistaUI);
        dibujarPantallaVictoria(); // La hemos movido a una función aparte
    }
    // 5. PANTALLA INSTRUCCIONES
    else if (estadoActual == Estado::Instrucciones) {
        window.setView(vistaUI);
        sf::Text txt;
        txt.setFont(fuenteGlobal);
        txt.setCharacterSize(35);
        txt.setFillColor(sf::Color::White);
        txt.setString("OBJETIVO DE LA CRUZADA\nDomina los 5 Nodos...\n\n(ESC para volver)");
        txt.setPosition(100.f, 150.f);
        window.draw(txt);
    }
    // 6. PANTALLA CRÉDITOS
    else if (estadoActual == Estado::Creditos) {
        window.setView(vistaUI);
        sf::Text txt;
        txt.setFont(fuenteGlobal);
        txt.setCharacterSize(35);
        txt.setFillColor(sf::Color::Yellow);
        txt.setString("DESARROLLO Y PROGRAMACION\n\nJavier Monrio...");
        txt.setPosition(150.f, 120.f);
        window.draw(txt);
    }

    window.display();
}
void Coordinador::actualizar(float dt) {
    if (estadoActual == Estado::Tablero || estadoActual == Estado::Arena) {
        motor.actualizar(dt, estadoActual); // El motor solo actualiza si hay juego
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