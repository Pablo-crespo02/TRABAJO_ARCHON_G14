#include "Motor.h"
#include "Generador.h"
#include "Color.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include<cmath>


// Constructor de Motor adaptado al Coordinador
Motor::Motor(sf::RenderWindow& win, sf::Font& fuente)
    : window(win),           // Conectamos con la ventana del Coordinador
    fuenteGlobal(fuente), // Conectamos con la fuente del Coordinador
    hud(window,fuente)
{
    // Ya NO cargamos la fuente aquí (ya la cargó el Coordinador)
    // Ya NO creamos la ventana aquí (ya la creó el Coordinador)

    // 1. Inicialización de variables de estado
    jugadorActual = 1;
    cicloActual = 1;
    rondaActual = 1;
    ganadorPartida = 0;
    piezaSeleccionada = nullptr;
    piezaAtacante = nullptr;
    piezaDefensor = nullptr;

    // 2. Generar el mundo inicial
    // Llamamos a tus funciones de generación
    Generador::GenerarTablero(tablero);
    Generador::GenerarDespliegueUnidades(*this);

    std::cout << "Motor inicializado correctamente vinculado al Coordinador." << std::endl;
}
// Destructor para evitar fugas de memoria     
Motor::~Motor() {
    for (auto p : listaPiezas) {
        delete p;
    }
    listaPiezas.clear();
};
void Motor::limpiarDatos() {
    // 1. Borrado de memoria (Importante para evitar fugas/leaks)
    for (auto p : listaPiezas) {
        delete p;
    }
    listaPiezas.clear();
    Hitboxes.clear();

    // 2. Reseteo de variables lógicas
    jugadorActual = 1;
    cicloActual = 1;
    rondaActual = 1;
    ganadorPartida = 0;
    piezaSeleccionada = nullptr;
    piezaAtacante = nullptr;
    piezaDefensor = nullptr;

    // 3. Regeneración del mundo
    Generador::GenerarTablero(tablero);
    Generador::GenerarDespliegueUnidades(*this);
}
void Motor::renderizar() {

    if (estadoActual == Estado::Tablero) {
        tablero.dibujar(window);
        for (auto p : listaPiezas) {
            p->dibujar(window, estadoActual);
        }
    }
    else if (estadoActual == Estado::Arena) {
        arena.dibujar(window);
        for (const auto& h : Hitboxes) {
            window.draw(h.getFormaHitbox());
        }
        if (piezaAtacante && piezaDefensor) {
            piezaAtacante->dibujar(window, estadoActual);
            piezaDefensor->dibujar(window, estadoActual);
        }
    }
}

/////////////////////////  CÁLCULO DEL MODIFICADOR DEL TERRENO  //////////////////////////

double calcularmodificadorterreno(Bando bando, ColorActual colorcasilla) {

    double ventaja = 0; //Un valor positivo favorece a LUZ, un valor negativo favorece a OSCURIDAD

    // Valores de modificador en escalera:
    switch (colorcasilla) {
    case ColorActual::Blanco_pico:ventaja = 30; break;   // 30%
    case ColorActual::Blanco:ventaja = 20; break;        // 20%
    case ColorActual::Gris_claro:ventaja = 10; break;    // 10%
    case ColorActual::Gris_medio:ventaja = 0; break;     // 0%
    case ColorActual::Gris_oscuro: ventaja = -10; break;
    case ColorActual::Negro:       ventaja = -20; break;
    case ColorActual::Negro_pico:  ventaja = -30; break;
    }

    // Cambio de signo para la ventaja de las piezas OSCURIDAD
    if (bando == Bando::OSCURIDAD) {
        ventaja = -ventaja;
    }

    //Convertimos el valor en un porcentaje aplicable:
    return 1 + (ventaja * 0.01);
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
    }
    else {
        //Avisar que no es su turno
        std::cout << "¡No es el turno del Jugador " << idJugador << "!" << std::endl;
    }
}

/////////////////////////////////  REINICIO DEL JUEGO  //////////////////////////////////

void Motor::reiniciarJuego(){
    //Se limpian los contenedores de piezas y hitboxes:
    for (auto p : listaPiezas) {
        delete p;
    }

    listaPiezas.clear();
    Hitboxes.clear();

    //Se resetean los estados y variables a los DEFAULT:
    jugadorActual = 1;
    cicloActual = 1;
    rondaActual = 1;
    ganadorPartida = 0;
    piezaSeleccionada = nullptr;
    piezaAtacante = nullptr;
    piezaDefensor = nullptr;

    //Se vuelve a generar el tablero y las piezas:
    Generador::GenerarTablero(tablero);
    Generador::GenerarDespliegueUnidades(*this);

    estadoActual = Estado::MenuPrincipal;  
}

/////////////////////  COMPROBACIÓN DE LAS CONDICIONES DE VICTORIA  /////////////////////

void Motor::VerificarVictoria() {
    int piezasLuz = 0;
    int piezasOscuridad = 0;
    int powerPointsLuz = 0;
    int powerPointsOscuridad = 0;

    //Se escanean todas las piezas supervivientes del contenedor de piezas:
    for (auto p : listaPiezas) {
        if (p->bando == Bando::LUZ) {
            piezasLuz++;

            //Consultamos si se encuentra en un PowePoint:
            if (tablero.getpowerpoint(p->posicionTablero)) {
                powerPointsLuz++;
            }
        }

        else if (p->bando == Bando::OSCURIDAD) {
            piezasOscuridad++;

            //Consultamos si se encuentra en un PowePoint:
            if (tablero.getpowerpoint(p->posicionTablero)) {
                powerPointsOscuridad++;
            }
        }
    }

    //CHIVATOS DEBUG:
    std::cout << "[DEBUG VICTORIA] Imperium -> Piezas: " << piezasLuz << " | PowerPoints: " << powerPointsLuz << std::endl;
    std::cout << "[DEBUG VICTORIA] Xenos -> Piezas: " << piezasOscuridad << " | PowerPoints: " << powerPointsOscuridad << std::endl;

    //Comprobamos las condiciones de victoria una vez se ha recorrido todo el contenedor:
    //Condiciones LUZ:
    if (piezasOscuridad == 0 || powerPointsLuz >= 2) {
        estadoActual = Estado::Victoria;
        ganadorPartida = 1;
        std::cout << "  VICTORIA DEL IMPERIUM" << std::endl;

    }

    //Condiciones OSCURIDAD:
    else if (piezasLuz == 0 || powerPointsOscuridad >= 5) {
        estadoActual = Estado::Victoria;
        ganadorPartida = 2;
        std::cout << "  VICTORIA DE LOS XENOS" << std::endl;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////


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
    //Calculamos los modificadores de daño y defnsa en función de la casilla en la que se combate:
    sf::Vector2i posTableroCombate = piezaDefensor->getPosicionTablero();
    ColorActual colorArenaCombate = tablero.getcoloractualcasilla(posTableroCombate);

    piezaAtacante->multiplicadorArena = calcularmodificadorterreno(piezaAtacante->getBando(), colorArenaCombate);
    piezaDefensor->multiplicadorArena = calcularmodificadorterreno(piezaDefensor->getBando(), colorArenaCombate);

    //CHIVATOS DEBUG:
    std::cout << "DEBUG MULTIPLICADORES: Modificador Atacante: " << piezaAtacante->multiplicadorArena << "x" << std::endl;
    std::cout << "DEBUG MULTIPLICADORES: Modificador Defensor: " << piezaDefensor->multiplicadorArena << "x" << std::endl;

   estadoActual = Estado::Arena;
}

//MANEJO DE CLICKS EN EL TABLERO:

void Motor::manejarClick(sf::Vector2i mousePos, const sf::View& vistaTablero) {

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

                    //Verificamos si no se cumplen las condicciones de victoria pacífica por control de 5 power points:
                    VerificarVictoria();

                    //Sólo se procede con el juego si el último movimiento no supuso una victoria:
                    if (estadoActual != Estado::Victoria) {
                    intentarAccionJugador(jugadorActual);
                }
                }
            }
            else {
                std::cout << "Movimiento denegado: Camino bloqueado o fuera de rango." << std::endl;
            }
        }
    }
}

void Motor::manejarEventos(const sf::View& vistaTablero) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        //menu principal
        if (estadoActual == Estado::MenuPrincipal) {
            if (event.type == sf::Event::KeyPressed) {
                // Navegar por las opciones con las flechas de arriba y abajo
                if (event.key.code == sf::Keyboard::Up) {
                    pantallaInicio.moverArriba();
                }
                if (event.key.code == sf::Keyboard::Down) {
                    pantallaInicio.moverAbajo();
                }

                // Confirmar selección con Enter
                if (event.key.code == sf::Keyboard::Enter) {
                    int seleccion = pantallaInicio.getIndiceSeleccionado();

                    if (seleccion == 0) { // op 0 : iniciar partida
                        std::cout << "Iniciando partida... Al Tablero" << std::endl;
                        reiniciarJuego();
                        estadoActual = Estado::Tablero;
                    }
                    else if (seleccion == 1) { //op 1: reanudar partida
                        std::cout << "Reanudando partida..." << std::endl;
                        estadoActual = Estado::Tablero;
                    }
                    else if (seleccion == 2) { //op 2: INSTRUCCIONES
                        estadoActual = Estado::Instrucciones;
                    }
                    else if (seleccion == 3) { //op 3: CREDITOS
                        estadoActual = Estado::Creditos;
                    }
                }
            }
        }
        //pulsando enter o escape para salir 
        else if (estadoActual == Estado::Instrucciones || estadoActual == Estado::Creditos) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Enter) {
                    estadoActual = Estado::MenuPrincipal; // Vuelve al menú
                }
            }
        }
        //tablero
        else if (estadoActual == Estado::Tablero) {
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    manejarClick(sf::Mouse::getPosition(window), vistaTablero);
                }
            }
        }

        //control arena
        else if (estadoActual == Estado::Arena) {
            if (event.type == sf::Event::KeyPressed) {
                // Salir del combate manualmente
                if (event.key.code == sf::Keyboard::Escape) {
                    estadoActual = Estado::Tablero;
                    std::cout << "Volviendo al Tablero..." << std::endl;
                }
            }

        }

        //control pantalla victoria
        else if (estadoActual == Estado::Victoria) {
            if (event.type == sf::Event::KeyPressed) {
                // Reiniciar juego y volver al menú principal
                if (event.key.code == sf::Keyboard::Enter) {
                    reiniciarJuego(); // Resetea variables y vuelve a MenuPrincipal
                }
            }
        }
    }
}


//GESTIÓN DEL TECLADO ARENA
void Motor::actualizar(double dt) {
    // 1. Filtro de estado obligatorio
    if (estadoActual != Estado::Arena || !piezaAtacante || !piezaDefensor) return;

    // 2. Identificación de bandos
    Pieza* pLuz = (piezaAtacante->getBando() == Bando::LUZ) ? piezaAtacante : piezaDefensor;
    Pieza* pOsc = (piezaAtacante->getBando() == Bando::OSCURIDAD) ? piezaAtacante : piezaDefensor;

    // 3. Llamadas al nuevo método de la clase (pasándole el dt que ahora es un float)
    float dtFloat = static_cast<float>(dt);
    procesarInput(pLuz, sf::Keyboard::W, sf::Keyboard::S, sf::Keyboard::A, sf::Keyboard::D, sf::Keyboard::Space, sf::Vector2f(1.f, 0.f), dtFloat);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        if (pLuz->getHechizoDisponible()) {
            pLuz->usarHechizo(Hitboxes, pOsc);
            pLuz->setHechizoDisponible(false);
            std::cout << pLuz->stats.nombre << " hizo uso de su hechizo!" << std::endl;
        }
    }

    procesarInput(pOsc, sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Left, sf::Keyboard::Right, sf::Keyboard::Enter, sf::Vector2f(-1.f, 0.f), dtFloat);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
        if (pOsc->getHechizoDisponible()) {
            pOsc->usarHechizo(Hitboxes, pLuz);
            pOsc->setHechizoDisponible(false);
            std::cout << pOsc->stats.nombre << " hizo uso de su hechizo!" << std::endl;
        }
    }

    // 4. Bucle de actualización y colisiones de Hitboxes
    for (size_t i = 0; i < Hitboxes.size(); ++i) {
        Hitboxes[i].ActualizarHitbox(dt);
        if (!Hitboxes[i].getEstadoHitbox()) continue;

        sf::Vector2f posH = Hitboxes[i].getPosicionHitbox();
        float radioH = Hitboxes[i].getFormaHitbox().getRadius();

        // Colisión con el escenario
       // --- COLISIÓN CON EL ESCENARIO (CORREGIDO PARA LA GRANADA) ---
        if (!arena.esPosicionValida(posH, radioH, false)) {
            if (Hitboxes[i].getEsErratico()) {
                Hitboxes[i].rebotar();
                posH = Hitboxes[i].getPosicionHitbox();
            }
            else if (Hitboxes[i].esGranada) {
                // CORRECCIÓN: Usamos la función getTiempoVuelo() que lee 'temporizadorVuelo'
                if (Hitboxes[i].getTiempoVuelo() > 0.0f) {
                    Hitboxes[i].Detonar();
                }
            }
            else {
                Hitboxes[i].setEstadoHitbox(false);
                continue;
            }
        }

        // Colisión con los combatientes
        Pieza* objetivos[2] = { piezaDefensor, piezaAtacante };

        for (Pieza* obj : objetivos) {
            if (!obj) continue;

            // 1. Identificamos si la pieza que estamos revisando es el atacante original de la arena o el defensor
            bool esAtacanteArena = (obj == piezaAtacante);

            // 2. Leemos la bandera individual correspondiente a esta pieza
            bool yaFueDañado = esAtacanteArena ? Hitboxes[i].getYaDanoAtacante() : Hitboxes[i].getYaDanoDefensor();

            // 3. Filtro de fuego amigo para proyectiles normales (ignoramos si es granada)
            if (!Hitboxes[i].esGranada && Hitboxes[i].getAtacante() == obj) {
                continue;
            }

            sf::Vector2f posE = obj->getPosicionAbsoluta();
            float distSq = std::pow(posH.x - posE.x, 2) + std::pow(posH.y - posE.y, 2);
            float limiteSq = std::pow(radioH + 20.f, 2); // 20.f es el radio aproximado de la pieza

            if (distSq < limiteSq) {
                // CASO A: DAÑO CONTINUO (DoT)
                if (Hitboxes[i].getEsDanoContinuo()) {
                    if (!obj->getInvulnerable()) {
                        obj->stats.vida -= Hitboxes[i].getDano() * dtFloat;
                    }
                }
                // CASO B: GRANADA (Daño único por entidad)
                else if (Hitboxes[i].esGranada) {
                    // Forzar detonación si impacta en vuelo
                    if (Hitboxes[i].getTiempoVuelo() > 0.0f) {
                        Hitboxes[i].Detonar();
                        posH = Hitboxes[i].getPosicionHitbox();
                        radioH = Hitboxes[i].getFormaHitbox().getRadius();
                    }

                    // Comprobación estricta: ¿Esta pieza ya recibió el daño de ESTA granada?
                    if (!yaFueDañado && Hitboxes[i].getDano() > 0.0f && !obj->getInvulnerable()) {

                        // Aplicar exactamente el daño base de la granada (ej: 10)
                        obj->stats.vida -= Hitboxes[i].getDano();
                        std::cout << "¡Granada aplico " << Hitboxes[i].getDano() << " de dano a " << obj->stats.nombre << "!" << std::endl;

                        // Marcar que ESTA pieza ya recibió el daño para ignorarla en los siguientes frames
                        if (esAtacanteArena) Hitboxes[i].setYaDanoAtacante(true);
                        else Hitboxes[i].setYaDanoDefensor(true);
                    }
                }
                // CASO C: PROYECTILES NORMALES
                else if (!yaFueDañado) {
                    if (!obj->getInvulnerable()) {
                        obj->stats.vida -= Hitboxes[i].getDano();
                    }

                    // Marcar daño recibido
                    if (esAtacanteArena) Hitboxes[i].setYaDanoAtacante(true);
                    else Hitboxes[i].setYaDanoDefensor(true);

                    // Aplicar efectos secundarios
                    if (Hitboxes[i].getCausaInmovilizacion()) {
                        obj->aplicarInmovilizacion(Hitboxes[i].getDuracionCC());
                    }

                    // Destruir el proyectil tras el impacto
                    sf::Vector2f vel = Hitboxes[i].getVelocidadHitbox();
                    if (vel.x != 0.f || vel.y != 0.f) {
                        Hitboxes[i].setEstadoHitbox(false);
                    }
                }
            }
        }
    }
    // 5. Limpieza de contenedores clásica por índice inverso
    for (int i = static_cast<int>(Hitboxes.size()) - 1; i >= 0; --i) {
        if (!Hitboxes[i].getEstadoHitbox()) {
            Hitboxes.erase(Hitboxes.begin() + i);
        }
    }

    // 6. Comprobación de bajas
    if (piezaAtacante->stats.vida <= 0.f || piezaDefensor->stats.vida <= 0.f) {
        Pieza* perdedor = (piezaAtacante->stats.vida <= 0.f) ? piezaAtacante : piezaDefensor;
        Pieza* ganador = (perdedor == piezaAtacante) ? piezaDefensor : piezaAtacante;

        sf::Vector2i destinoFinal = piezaDefensor->getPosicionTablero();
        auto deteccionperdedor = std::find(listaPiezas.begin(), listaPiezas.end(), perdedor);

        if (deteccionperdedor != listaPiezas.end()) {
            delete* deteccionperdedor;
            listaPiezas.erase(deteccionperdedor);
        }

        ganador->mover(destinoFinal);

        piezaAtacante = nullptr;
        piezaDefensor = nullptr;
        piezaSeleccionada = nullptr;

        Hitboxes.clear();
        estadoActual = Estado::Tablero;

        VerificarVictoria();

        if (estadoActual != Estado::Victoria) {
            intentarAccionJugador(jugadorActual);
        }
    }
}

   
void Motor::dibujarHUD() {
    // Usamos el estadoActual de la CLASE, no uno pasado por fuera
    if (this->estadoActual == Estado::Tablero) {
        hud.dibujar(window, rondaActual, cicloActual, jugadorActual, piezaSeleccionada);
    }
}
void Motor::gestionarEntrada(sf::Event& evento, const sf::View& vistaTablero) {
    if (estadoActual == Estado::Tablero) {
        if (evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left) {
            manejarClick(sf::Mouse::getPosition(window), vistaTablero);
        }
    }

}
void Motor::procesarInput(Pieza* p, sf::Keyboard::Key arriba, sf::Keyboard::Key abajo,
    sf::Keyboard::Key izqda, sf::Keyboard::Key dcha,
    sf::Keyboard::Key ataque, sf::Vector2f dirPorDefecto, float dt)
{
    sf::Vector2f dir(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(arriba)) dir.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(abajo))  dir.y += 1.f;
    if (sf::Keyboard::isKeyPressed(izqda))  dir.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(dcha))   dir.x += 1.f;

    p->setultimadireccion(dir);

    if (sf::Keyboard::isKeyPressed(ataque) && p->puedeAtacar()) {
        sf::Vector2f dirAtaque = p->getultimadireccion();
        float magnitud = std::hypot(dirAtaque.x, dirAtaque.y);
        dirAtaque = (magnitud != 0.f) ? (dirAtaque / magnitud) : dirPorDefecto;

        sf::Vector2f puntoSpawnAtaque = p->getPosicionAbsoluta() + (dirAtaque * 35.f);

        if (p->stats.esRango) {
            Hitboxes.emplace_back(puntoSpawnAtaque, dirAtaque, 500, Colores::ColorProyectil, p, (p->stats.ataque * p->multiplicadorArena), 60, 15);
        }
        else {
            Hitboxes.emplace_back(puntoSpawnAtaque, dirAtaque, 0, Colores::ColorProyectil, p, (p->stats.ataque * p->multiplicadorArena), 0.2, 35);
        }

        p->reiniciarRelojHitbox();
    }

    p->procesarMovimientoArena(dir, dt, this->arena);
}