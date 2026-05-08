#include "Motor.h"
#include "Generador.h"
#include "Color.h"
#include <iostream>
#include <vector>
#include<cmath>


Motor::Motor() {
    if (!hud.cargarFuente("fuentes/fuente_pixel.ttf")) {
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

        // El tablero se dibuja a sí mismo
        tablero.dibujar(window);

        // Cada pieza se dibuja a sí misma
        for (auto p : listaPiezas) {
            p->dibujar(window, estadoActual);
        }

        // --- VISTA DE INTERFAZ (HUD) ---
        window.setView(vistaUI);
        hud.dibujar(window, rondaActual, cicloActual, jugadorActual, piezaSeleccionada);
    }

    // 3. MODO ARENA DE COMBATE
    else if (estadoActual == Estado::Arena) {
        window.setView(vistaTablero);

        // LA ARENA SE DIBUJA A SÍ MISMA (suelo, rocas, sangre, muros)
        arena.dibujar(window);

        // CAPA DE HITBOXES UNIFICADA (Proyectiles a distancia y cortes Melee)
        // Sustituye a los dos bucles for anteriores
        for (const auto& h : Hitboxes) {
            window.draw(h.getFormaHitbox());
        }

        // LAS PIEZAS COMBATIENTES SE DIBUJAN A SÍ MISMAS
        if (piezaAtacante != nullptr && piezaDefensor != nullptr) {
            piezaAtacante->dibujar(window, estadoActual);
            piezaDefensor->dibujar(window, estadoActual);
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

    //Recarga el hechizo al entrar en la arena
    piezaAtacante->setHechizoDisponible(true);
    piezaDefensor->setHechizoDisponible(true);

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

//MANEJO DE CLICKS EN EL TABLERO:

void Motor::manejarClick(sf::Vector2i mousePos) {

    // Comprueba que el juego está en el estado tablero:
    if (estadoActual != Estado::Tablero) return;

    //Creación de un vector que indica la posición en el mundo del ratón:
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos, vistaTablero);

    // Conversión de posición en el mundo a coodenadas de tablero (0-8):
    int tableroX = static_cast<int>(worldPos.x / 60.f);
    int tableroY = static_cast<int>(worldPos.y / 60.f);

    //Si se pulsa fuera del tablero:
    if (tableroX < 0 || tableroX > 8 || tableroY < 0 || tableroY > 8) return;

    //Para la posición del ratón en coordenadas de tablero con las coordenadasde las piezas:
    sf::Vector2i celdaClickeada(tableroX, tableroY);

    //Selección de pieza:
    if (!piezaSeleccionada) {
        // Accedemos directamente a la lista de piezas y sus miembros
        for (auto p : listaPiezas) {
            if (p->posicionTablero == celdaClickeada) {
                // Comprobación de turno usando acceso directo a p->bando
                if ((jugadorActual == 1 && p->bando == Bando::LUZ) || (jugadorActual == 2 && p->bando == Bando::OSCURIDAD)) {
                    piezaSeleccionada = p;
                    piezaSeleccionada->seleccionado = true;
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

    //Reinicia el reloj interno del motor y guarda el tiempo en segundos. Desacopla el movimeinto de los FPS:
    double dt = reloj.restart().asSeconds();

    // Sólo aplicable en la arena. Si no se está en la arena, o faltan piezas atacantes o defensoras, no aplica:
    if (estadoActual != Estado::Arena || !piezaAtacante || !piezaDefensor)return;

    // Identificamos quién pertenece a qué bando para asignar controles WASD o flechas:
    Pieza* pLuz = (piezaAtacante->getBando() == Bando::LUZ) ? piezaAtacante : piezaDefensor;
    Pieza* pOsc = (piezaAtacante->getBando() == Bando::OSCURIDAD) ? piezaAtacante : piezaDefensor;

    //LÓGICA DE INPUT Y COMBATE REUTILIZABLE:
    //Creamos una variable anónima "lambda" que "captura" por referencia las variables del entorno: [capturas](parámetros)->tipo_retorrno{fucnción}
    //No especificamos el tipo de retorno porque en C++ no es imprescindible.
    auto procesarInput = [&](Pieza* p, sf::Keyboard::Key arriba, sf::Keyboard::Key abajo, sf::Keyboard::Key izqda, sf::Keyboard::Key dcha, sf::Keyboard::Key ataque, sf::Vector2f dirPorDefecto) {

        //Inicializamos el vector dirección a 0 en el primer frame:
        sf::Vector2f dir(0, 0);

        //Modificación del vector director según las teclas pulsadas:
        if (sf::Keyboard::isKeyPressed(arriba)) dir.y -= 1.f;
        if (sf::Keyboard::isKeyPressed(abajo)) dir.y += 1.f;
        if (sf::Keyboard::isKeyPressed(izqda)) dir.x -= 1.f;
        if (sf::Keyboard::isKeyPressed(dcha)) dir.x += 1.f;

        //Guardamos la última dirección en la que intentó moverse, a efectos de apuntar con proyectiles:
        p->setultimadireccion(dir);

        //Si se pulsa la tecla de ataque y el cooldown permite disparar:
        if (sf::Keyboard::isKeyPressed(ataque) && p->puedeAtacar()) {

            //Obtenemos hacia dónde está mirando la pieza:
            sf::Vector2f dirAtaque = p->getultimadireccion();

            //Calculamos la magnitud del vector para hacerlo unitario, evitando problemas con las diagonales:
            float magnitud = std::hypot(dirAtaque.x, dirAtaque.y);

            //Normalizamos el vector, y si no se ha movido utilizamos la dirección por defecto:
            dirAtaque = (magnitud != 0) ? (dirAtaque / magnitud) : dirPorDefecto;

            //Instanciamos el punto de spawn del hitbox ataque desplazado 35 píxeles del origen de la pieza en dirección del vector dirección ataque:
            sf::Vector2f puntoSpawnAtaque = (p->getPosicionAbsoluta()) + (dirAtaque * 35.f);

            //Evaluamos si la pieza es rango o melee, y generamos ataque a distancia o melee con distintos stats:

            if (p->stats.esRango) {
                //Generamos un proyectil velocidad 500, tiempo de vida 60s, radio 15:
                Hitboxes.emplace_back(puntoSpawnAtaque, dirAtaque, 500, Colores::ColorProyectil, p, p->stats.ataque, 60, 15);
            }

            else {
                //Generamos un ataque melee velocidad 0, tiempo de vida 0.2s, radio 35
                Hitboxes.emplace_back(puntoSpawnAtaque, dirAtaque, 0, Colores::ColorProyectil, p, p->stats.ataque, 0.2, 35);
            }

            //Reiniciamos el cooldown interno de la pieza:
            p->reiniciarRelojHitbox();
        }

        //Movemos físicamente la pieza en la arena aplicando colisiones:

        p->procesarMovimientoArena(dir, dt, this->arena);

        };

    //Aplicamos la función reutilizable lambda para el caso de jugador Luz y jugador Oscuridad:

    //LUZ: WASD, disparo con ESPACIO, inicialmente mira hacia la dcha:
    procesarInput(pLuz, sf::Keyboard::W, sf::Keyboard::S, sf::Keyboard::A, sf::Keyboard::D, sf::Keyboard::Space, sf::Vector2f(1, 0));
    // Lanzar Hechizo Luz (Tecla Q)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        if (pLuz->getHechizoDisponible()) {
            // Ejecuta la lógica única de la pieza
            pLuz->usarHechizo(Hitboxes, pOsc);
            // Lo marcamos como gastado
            pLuz->setHechizoDisponible(false);
            std::cout << pLuz->stats.nombre << " uso su hechizo!" << std::endl;
        }
    }

    //OSCURIDAD: FLECHAS, disparo con ENTER, inicialmente mira hacia la izq:
    procesarInput(pOsc, sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Left, sf::Keyboard::Right, sf::Keyboard::Enter, sf::Vector2f(-1, 0));
    // Lanzar Hechizo Oscuridad (RShift o la tecla que prefieras)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
        if (pOsc->getHechizoDisponible()) {
            pOsc->usarHechizo(Hitboxes, pLuz);
            pOsc->setHechizoDisponible(false);
            std::cout << pOsc->stats.nombre << " uso su hechizo!" << std::endl;
        }
    }
    //ACTUALIZACIÓN DE PROYECTILES:

    //Creamos una función auxiliar "lambda" que comprueba si la distancia entre dos puntos es menor que un límte, es decir, si han colisionado:
    auto comprobarColision = [](const sf::Vector2f pos1, const sf::Vector2f pos2, double limite) {
        return (std::sqrt(std::pow((pos1.x - pos2.x), 2) + std::pow((pos1.y - pos2.y), 2))) < limite;
        };

    //Recorremos todos los proyectiles activos del contenedor:
    for (auto& h : Hitboxes) {

        //Actualizamos su posición física en la arena:
        h.ActualizarHitbox(dt);

        //Si el proyectil está inactivo pero todavía no se ha borrado del contenedor, pasar al siguiente elemento:
        if (!h.getEstadoHitbox())continue;

        //Obtenemos la posición del proyectil en el mapa:
        sf::Vector2f posH = h.getPosicionHitbox();
        bool impactado = false; //Booleano que registra la colisión
        //Creamos un vector auxiliar "objetivos" con ambos jugadores para simplificar las comprobaciones de colisión:

        
        // Si la posición de la bala o el torbellino está dentro de un muro o roca:
        if (!arena.esPosicionValida(posH, radioH, false)) {

            if (h.getEsErratico()) {
                // Si es el Torbellino, rebota y sigue vivo
                h.rebotar();
                // Actualizamos la variable posH para el chequeo de enemigos tras el rebote
                posH = h.getPosicionHitbox();
            }
            else {
                // Si es una bala normal, choca contra la piedra/muro y se destruye
                h.setEstadoHitbox(false);
                continue; // Saltamos al siguiente elemento del bucle para que no haga daño fantasma
            }
        }

        Pieza* objetivos[2] = { piezaAtacante, piezaDefensor };

        //Recorremos el vector auxiliar:
        for (Pieza* obj : objetivos) {

            //Verificamos la existencia del jugador, eviamos autolesión y evaluamos límite de colisión:
            if (obj && h.getAtacante() != obj && comprobarColision(posH, obj->getPosicionAbsoluta(), limitecolision)) {
                //Restamos vida al jugador colisionado:
                obj->stats.vida -= h.getDano();
                //Desactivamos el proyectil:
                h.setEstadoHitbox(false);
                //Marcamos como true el booleano "impactado" para evitar múltiples colisiones en el mismo frame:
                impactado = true;
                //Rompemos el ciclo porque un proyectil sólo puede impactar un objeto por vez:
                break;
            }
        }

        //Comprobamos la colisión con el entorno (solo si no colisionó con una pieza anteriormente) para los ataques a distancia:
        //Los ataques melee tienen velocidad 0, mientras que los proyectiles tienen velocidad v.x + v.y. Es posible encontrar una comprobación más elegante en el futuro

        sf::Vector2f velocidadcomprobacion = h.getVelocidadHitbox();

        bool esProyectil = (velocidadcomprobacion.x != 0 || velocidadcomprobacion.y != 0);

        if (esProyectil) {
            if (!impactado && h.getEstadoHitbox()) {
                //Si el proyectil ocupa una posición no válida en la arena:
                if (!arena.esPosicionValida(posH, (h.getFormaHitbox().getRadius()), false)) {
                    //Desactivamos el proyectil
                    h.setEstadoHitbox(false);

            if (obj && h.getAtacante() != obj) {
                sf::Vector2f posE = obj->getPosicionAbsoluta();
                float distSq = std::pow(posH.x - posE.x, 2) + std::pow(posH.y - posE.y, 2);
                float limiteSq = std::pow(radioH + 20.f, 2);

                if (distSq < limiteSq) {

                    // A) ZONA DE DAÑO CONTINUO (Torbellino)
                    if (h.getEsDanoContinuo()) {
                        obj->stats.vida -= h.getDano() * dt;
                    }
                    // B) DAÑO INSTANTÁNEO (Balas y Melee)
                    else if (!h.getYaHizoDano()) {
                        obj->stats.vida -= h.getDano();
                        h.setYaHizoDano(true);

                        sf::Vector2f vel = h.getVelocidadHitbox();
                        if (vel.x != 0.f || vel.y != 0.f) {
                            h.setEstadoHitbox(false); // Proyectil muere
                        }
                    }
                }
            }
        }
    }


    //LIMPIEZA DE CONTENEDORES:

    Hitboxes.erase(std::remove_if(Hitboxes.begin(), Hitboxes.end(), [](const Hitbox& h) {return !h.getEstadoHitbox(); }), Hitboxes.end());


    //RESOLUCIÓN DE MUERTES Y FIN DE COMBATE:

    //Evaluamos si alguno de los dos combatientes tiene vida=0 tras las colisiones de proyectiles y melee:
    if (piezaAtacante->stats.vida <= 0 || piezaDefensor->stats.vida <= 0) {

        //Creamos un operador auxiliar para determinar quién es el ganador y quién el defensor:
        Pieza* perdedor = (piezaAtacante->stats.vida <= 0) ? piezaAtacante : piezaDefensor;

        //El que no es perdedor es automáticamente ganador:
        Pieza* ganador = (perdedor == piezaAtacante) ? piezaDefensor : piezaAtacante;

        //Guardamos la coordenada del tablero donde ocurrió el conflicto:
        sf::Vector2i destinoFinal = piezaDefensor->getPosicionTablero();

        //Buscamos al perdedor en el contenedor global de piezas:
        auto deteccionperdedor = std::find(listaPiezas.begin(), listaPiezas.end(), perdedor);

        if (deteccionperdedor != listaPiezas.end()) {

            //Eliminamos el perdedor de la lista de piezas:
            delete* deteccionperdedor;

            //Borramos el puntero nulo de la lista global de piezas:
            listaPiezas.erase(deteccionperdedor);
        }

        //El ganador ocupa la casilla conquistada:
        ganador->mover(destinoFinal);

        //Borramos los punteros para evitar pnteros colgantes:
        piezaAtacante = nullptr;
        piezaDefensor = nullptr;
        piezaSeleccionada = nullptr;

        //Forzamos el borrado de los contenedores de proyectiles y melee:
        Hitboxes.clear();

        //Cambiamos el estado del motor a tablero:
        estadoActual = Estado::Tablero;

        //Llamamos a la lógica de avance de turno:
        intentarAccionJugador(jugadorActual);
    }
}