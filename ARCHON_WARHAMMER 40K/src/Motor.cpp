#include "Motor.h"
#include "Generador.h"
#include"Renderizador.h"
#include <iostream>
#include <vector>
#include<cmath>


Motor::Motor() {
    if (!fuenteGlobal.loadFromFile("fuentes/fuente_pixel.ttf")) {
        // Si entra aquí, es que no encuentra el archivo
        std::cout << "Error: No se encontro el archivo de fuente!" << std::endl;
    }
    // Configuración de Ventana en Pantalla Completa
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    window.create(desktop, "ARCHON WARHAMMER 40K", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    // Configuración de la Vista del Tablero (Mundo del Juego)
    // Definimos un tamaño lógico fijo para el tablero (ej: 800x800)
    vistaTablero.setSize(700.f, 700.f);
    vistaTablero.setCenter(350.f, 350.f); // Centro lógico (size / 2)

    // Define dónde aparece el tablero en la pantalla (de 0.0 a 1.0)
    // Lo situamos un poco a la izquierda (x=0.10)
    vistaTablero.setViewport(sf::FloatRect(0.10f, 0.20f, 0.60f, 0.80f));

    // Configuración de la Vista de Interfaz (HUD)
    // Esta vista usa las coordenadas píxel a píxel de la ventana para que sea fácil posicionar elementos
    vistaUI = window.getDefaultView();
    jugadorActual = 1;
    cicloActual = 1;
    rondaActual = 1;
    estadoActual = Estado::MenuPrincipal;
    piezaSeleccionada = nullptr; // Importante para la interfaz de la derecha

    // Inicializaciones de lógica
    Generador::GenerarTablero(tablero);
    Generador::GenerarDespliegueUnidades(*this);
}
// Destructor para evitar fugas de memoria     
Motor::~Motor() {
    for (auto p : listaPiezas) {
        delete p;
    }
    listaPiezas.clear();
};

void Motor::renderizar() {
    window.clear();

    // 1. PANTALLA DE INICIO (Menu principal)
    if (estadoActual == Estado::MenuPrincipal) {
        window.setView(vistaUI);
        pantallaInicio.dibujar(window);
    }

    // 2. MODO TABLERO
    else if (estadoActual == Estado::Tablero) {
        window.setView(vistaTablero);
        Renderizador::dibujarTablero(window, tablero);
        for (auto p : listaPiezas) {
            Renderizador::dibujarPieza(window, p, estadoActual);
        }
            // --- VISTA DE INTERFAZ (HUD) ---
        // Solo llamamos al HUD aquí, en el estado Tablero
            window.setView(vistaUI);
            dibujarHUD();
        }

    // 3. MODO ARENA DE COMBATE
    else if (estadoActual == Estado::Arena) {

        window.setView(vistaTablero);
        // Dibujamos el escenario base
        Renderizador::dibujarArena(window, arena);

        // Capa de ataques Melee (el círculo rojo de daño físico)
        for (const auto& m : ataquesMelee) {
            window.draw(m.getForma());
        }

        // Capa de proyectiles (disparos a distancia)
        for (const auto& p : proyectiles) {
            window.draw(p.getFormaProyectil());
        }

        // Capa de combatientes
        if (piezaAtacante != nullptr && piezaDefensor != nullptr) {
            Renderizador::dibujarPieza(window, piezaAtacante, estadoActual);
            Renderizador::dibujarPieza(window, piezaDefensor, estadoActual);
        }
    }

    window.display();
}

void Motor::dibujarHUD() {
    float ancho = (float)window.getSize().x;
    float alto = (float)window.getSize().y;
    float inicioUI = ancho * 0.66f;
    float anchoHUD = ancho - inicioUI;

    // --- 1. CABECERA SOBRE EL TABLERO ---
    sf::Text textoTop;
    textoTop.setFont(fuenteGlobal);
    textoTop.setOutlineThickness(2);
    textoTop.setOutlineColor(sf::Color::Black);

    // RONDA Y CICLO (Un poco más a la izquierda para separar del turno)
    textoTop.setCharacterSize(35);
    textoTop.setFillColor(sf::Color(180, 180, 180));
    textoTop.setString("RONDA: " + std::to_string(rondaActual));
    textoTop.setPosition(ancho * 0.10f, 35.f);
    window.draw(textoTop);

    textoTop.setString("CICLO: " + std::to_string(cicloActual) + " / 12");
    textoTop.setPosition(ancho * 0.10f, 80.f);
    window.draw(textoTop);

    // TURNO ACTUAL (Separado del Ciclo/Ronda)
    std::string nombreTurno = (jugadorActual == 1) ? "TURNO: IMPERIUM (LUZ)" : "TURNO: TIRANIDOS (OSCURIDAD)";
    sf::Color colorTurno = (jugadorActual == 1) ? sf::Color(255, 255, 150) : sf::Color(180, 100, 255);

    textoTop.setCharacterSize(40);
    textoTop.setFillColor(colorTurno);
    textoTop.setString(nombreTurno);
    // Posición centrada respecto al tablero, bajada un poco para no pegarse arriba
    textoTop.setPosition((inicioUI / 2.f) - 150.f, 35.f);
    window.draw(textoTop);


    // --- 2. PANEL LATERAL (HUD) ---
    sf::RectangleShape panel({ anchoHUD, alto });
    panel.setPosition(inicioUI, 0);
    panel.setFillColor(sf::Color(18, 18, 22));
    window.draw(panel);

    float margenLateral = 25.f;
    float margenX = inicioUI + margenLateral;
    float yActual = 60.f;

    // Título Unidad
    sf::Text texto;
    texto.setFont(fuenteGlobal);
    texto.setCharacterSize(35);
    texto.setFillColor(sf::Color(120, 120, 130));
    texto.setString("UNIDAD");
    texto.setPosition(margenX, yActual);
    window.draw(texto);

    yActual += 70.f;

    // 3. RECTÁNGULOS DE IMAGEN
    float anchoTotalDisponible = anchoHUD - (margenLateral * 2);
    float anchoMarco = (anchoTotalDisponible - 20.f) / 2.f;
    sf::Vector2f tamanoMarco(anchoMarco, 320.f);

    sf::RectangleShape marco(tamanoMarco);
    marco.setOutlineThickness(3);
    marco.setOutlineColor(sf::Color(80, 80, 90));
    marco.setFillColor(sf::Color(25, 25, 30));

    marco.setPosition(margenX, yActual);
    window.draw(marco);

    marco.setPosition(margenX + anchoMarco + 20.f, yActual);
    window.draw(marco);

    yActual += tamanoMarco.y + 40.f;

    if (piezaSeleccionada != nullptr) {
        texto.setCharacterSize(50);
        texto.setFillColor(sf::Color::White);
        texto.setString(piezaSeleccionada->stats.nombre);
        texto.setPosition(margenX, yActual);
        window.draw(texto);

        yActual += 85.f;

        // Lambda con posición de valor corregida
        auto dibujarDato = [&](std::string etiqueta, std::string valor, sf::Color colorVal = sf::Color::White) {
            texto.setCharacterSize(18);
            texto.setFillColor(sf::Color(140, 140, 150));
            texto.setString(etiqueta);
            texto.setPosition(margenX, yActual);
            window.draw(texto);

            texto.setCharacterSize(28);
            texto.setFillColor(colorVal);
            texto.setString(valor);

            // CORRECCIÓN: Volvemos a una posición relativa al margen izquierdo del HUD
            // para que no se salgan por la derecha
            texto.setPosition(margenX + 210.f, yActual - 5.f);

            window.draw(texto);
            yActual += 48.f;
            };

        // --- LISTA DE DATOS ---
        dibujarDato("VIDA:", std::to_string((int)piezaSeleccionada->stats.vida), sf::Color(100, 255, 100));
        dibujarDato("ATAQUE:", std::to_string((int)piezaSeleccionada->stats.ataque), sf::Color(255, 120, 120));
        dibujarDato("RANGO MOV:", std::to_string(piezaSeleccionada->rangoMovimiento) + " CASILLAS");

        sf::Color colMov = piezaSeleccionada->stats.esVolador ? sf::Color(100, 200, 255) : sf::Color(200, 150, 100);
        dibujarDato("MOVIMIENTO:", piezaSeleccionada->stats.esVolador ? "VOLADOR" : "TERRESTRE", colMov);

        sf::Color colCom = piezaSeleccionada->stats.esRango ? sf::Color(255, 215, 0) : sf::Color(255, 80, 80);
        dibujarDato("COMBATE:", piezaSeleccionada->stats.esRango ? "DISTANCIA" : "MELEE", colCom);

        dibujarDato("VEL. ATQ:", std::to_string((int)piezaSeleccionada->stats.velAtaque));
        dibujarDato("COOLDOWN:", std::to_string((int)piezaSeleccionada->stats.cooldown));

        std::string pTxt = "";
        switch (piezaSeleccionada->patronMovimiento) {
        case PatronMovimiento::Ambos: pTxt = "DIAGONAL Y ORTOGONAL (*)"; break;
        case PatronMovimiento::Ortogonal: pTxt = "ORTOGONAL (+)"; break;
        case PatronMovimiento::Diagonal: pTxt = "DIAGONAL (X)"; break;
        default: pTxt = "OTRO"; break;
        }
        dibujarDato("PATRON:", pTxt, sf::Color(255, 255, 150));
    }
}

void Motor::dibujarInfoPieza(sf::RenderWindow& window, Pieza* pieza, float x, float y) {
    if (pieza == nullptr) return;

    sf::Text texto;
    texto.setFont(fuenteGlobal);
    texto.setOutlineColor(sf::Color::Black); // Borde negro para que resalte
    texto.setOutlineThickness(1);            // Grosor sutil del borde
    texto.setLetterSpacing(1.2);             // Opcional: separa un poco las letras si se ven muy juntas
    float espaciado = 32.f;

    // --- NOMBRE ---
    texto.setCharacterSize(26);
    texto.setFillColor(sf::Color::White);
    texto.setString(pieza->stats.nombre);
    texto.setPosition(x, y);
    window.draw(texto);

    float yFila = y + 50.f;

    // Lambda auxiliar para dibujar cada dato         IMPORTANTE: aprender que es una funcion LAMBDA
    auto dibujarDato = [&](std::string etiqueta, std::string valor) {
        texto.setCharacterSize(14);
        texto.setFillColor(sf::Color(150, 150, 150));
        texto.setString(etiqueta);
        texto.setPosition(x, yFila);
        window.draw(texto);

        texto.setCharacterSize(18);
        texto.setFillColor(sf::Color::White);
        texto.setString(valor);
        texto.setPosition(x + 140.f, yFila - 2.f);
        window.draw(texto);

        yFila += espaciado;
        };

    // --- DATOS REALES DE TU STRUCT ---
    dibujarDato("VIDA:", std::to_string((int)pieza->stats.vida));
    dibujarDato("ATAQUE:", std::to_string((int)pieza->stats.ataque));
    dibujarDato("DEFENSA:", std::to_string((int)pieza->stats.defensa));
    dibujarDato("VEL. ATQ:", std::to_string((int)pieza->stats.velAtaque));
    dibujarDato("COOLDOWN:", std::to_string((int)pieza->stats.cooldown));

    // Para el booleano esRango
    std::string tipoRango = pieza->stats.esRango ? "DISTANCIA" : "CUERPO A CUERPO";
    dibujarDato("TIPO:", tipoRango);
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
    // Comprueba que está en el tablero
    if (estadoActual != Estado::Tablero) return;

    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos, vistaTablero);
   
    std::cout << "DEBUG: Clic en pixeles (" << mousePos.x << "," << mousePos.y << ")" << std::endl;
    // Conversión de píxeles a coordenadas de tablero (0-8)
    int tableroX = static_cast<int>(worldPos.x / 60.f);
    int tableroY = static_cast<int>(worldPos.y / 60.f);

    std::cout << "DEBUG: Celda calculada [" << tableroX << "," << tableroY << "]" << std::endl;

    if (tableroX < 0 || tableroX > 8 || tableroY < 0 || tableroY > 8) {
        std::cout << "DEBUG: Clic fuera del tablero" << std::endl;
        return;
    }

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
        // LÓGICA CON PIEZA SELECCIONADA
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
                    std::cout << "Iniciando partida... Al Tablero" << std::endl;
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
        //Ataque bando Luz
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            // Usamos tu función puedeDisparar() que ya gestiona el reloj interno
            if (pLuz->puedeDisparar()) {
                sf::Vector2f dirDisparo = pLuz->getultimadireccion();

                // Normalizamos la dirección
                float magnitud = std::sqrt(dirDisparo.x * dirDisparo.x + dirDisparo.y * dirDisparo.y);
                if (magnitud != 0) dirDisparo /= magnitud;
                else dirDisparo = sf::Vector2f(1.f, 0.f);

                if (pLuz->stats.esRango) {
                    // Creamos proyectil con el daño de la pieza
                    proyectiles.emplace_back(pLuz->getPosicionAbsoluta(), dirDisparo, 15.0, Colores::ColorProyectil, pLuz, pLuz->stats.ataque);
                }
                else {
                    // Lógica Melee
                    sf::Vector2f posMelee = pLuz->getPosicionAbsoluta() + (dirDisparo * 35.f);
                    ataquesMelee.emplace_back(posMelee, pLuz, pLuz->stats.ataque);
                }

                // LLAMADA CRÍTICA: Reinicia el reloj interno de la pieza
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
        //Ataque bando Oscuridad
        // Ataque bando Oscuridad (Enter)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            if (pOsc->puedeDisparar()) {
                sf::Vector2f dirDisparo = pOsc->getultimadireccion();

                float magnitud = std::sqrt(dirDisparo.x * dirDisparo.x + dirDisparo.y * dirDisparo.y);
                if (magnitud != 0) dirDisparo /= magnitud;
                else dirDisparo = sf::Vector2f(-1.f, 0.f);

                if (pOsc->stats.esRango) {
                    proyectiles.emplace_back(pOsc->getPosicionAbsoluta(), dirDisparo, 15.0, Colores::ColorProyectil, pOsc, pOsc->stats.ataque);
                }
                else {
                    sf::Vector2f posMelee = pOsc->getPosicionAbsoluta() + (dirDisparo * 35.f);
                    ataquesMelee.emplace_back(posMelee, pOsc, pOsc->stats.ataque);
                }

                // Reinicia el reloj para que no pueda disparar hasta el siguiente ciclo de cooldown
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
        // Ataques melee
        for (auto& m : ataquesMelee) {
            m.actualizar(dt); // Resta el tiempo de vida (0.2s)

            if (m.getEstado()) {
                sf::Vector2f posM = m.getPosicion();
                // Umbral: (Radio Melee 25 + Radio Pieza 20)^2 = 45^2 = 2025
                float limiteColisionSq = 2050.f;

                // Impacto en Atacante
                if (piezaAtacante && m.getAtacante() != piezaAtacante) {
                    sf::Vector2f posE = piezaAtacante->getPosicionAbsoluta();
                    float distSq = std::pow(posM.x - posE.x, 2) + std::pow(posM.y - posE.y, 2);
                    if (distSq < limiteColisionSq) {
                        piezaAtacante->stats.vida -= m.getDano();
                        m.setEstado(false); // Desactiva el golpe tras acertar (no pega dos veces)
                        std::cout << "¡Zasca de Melee al Atacante! Vida: " << piezaAtacante->stats.vida << std::endl;
                    }
                }

                // Impacto en Defensor
                if (m.getEstado() && piezaDefensor && m.getAtacante() != piezaDefensor) {
                    sf::Vector2f posE = piezaDefensor->getPosicionAbsoluta();
                    float distSq = std::pow(posM.x - posE.x, 2) + std::pow(posM.y - posE.y, 2);
                    if (distSq < limiteColisionSq) {
                        piezaDefensor->stats.vida -= m.getDano();
                        m.setEstado(false);
                        std::cout << "¡Zasca de Melee al Defensor! Vida: " << piezaDefensor->stats.vida << std::endl;
                    }
                }
            }
        }

        // Limpieza de ataques Melee expirados o que ya golpearon
        ataquesMelee.erase(
            std::remove_if(ataquesMelee.begin(), ataquesMelee.end(), [](const AtaqueMelee& m) {
                return !m.getEstado();
                }), ataquesMelee.end());
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