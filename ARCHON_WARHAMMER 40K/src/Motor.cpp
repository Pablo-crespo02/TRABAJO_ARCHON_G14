#include "Motor.h"
#include "Generador.h"
#include "Color.h"
#include <iostream>
#include <vector>
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

    //Reinicia el reloj interno del motor y guarda el tiempo en segundos. Desacopla el movimeinto de los FPS:
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
                Hitboxes.emplace_back(puntoSpawnAtaque, dirAtaque, 500, Colores::ColorProyectil, p, (p->stats.ataque * p->multiplicadorArena), 60, 15);
            }

            else {
                //Generamos un ataque melee velocidad 0, tiempo de vida 0.2s, radio 35
                Hitboxes.emplace_back(puntoSpawnAtaque, dirAtaque, 0, Colores::ColorProyectil, p, (p->stats.ataque * p->multiplicadorArena), 0.2, 35);
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
            std::cout << pLuz->stats.nombre << " hizo uso de su hechizo!" << std::endl;
        }
    }

    //OSCURIDAD: FLECHAS, disparo con ENTER, inicialmente mira hacia la izq:
    procesarInput(pOsc, sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Left, sf::Keyboard::Right, sf::Keyboard::Enter, sf::Vector2f(-1, 0));
    // Lanzar Hechizo Oscuridad (RShift o la tecla que prefieras)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
        if (pOsc->getHechizoDisponible()) {
            pOsc->usarHechizo(Hitboxes, pLuz);
            pOsc->setHechizoDisponible(false);
            std::cout << pOsc->stats.nombre << " hizo uso de su hechizo!" << std::endl;
        }
    }
    //Proyectiles

    //Creamos una función auxiliar "lambda" que comprueba si la distancia entre dos puntos es menor que un límte, es decir, si han colisionado:
    auto comprobarColision = [](const sf::Vector2f pos1, const sf::Vector2f pos2, double limite) {
        return (std::sqrt(std::pow((pos1.x - pos2.x), 2) + std::pow((pos1.y - pos2.y), 2))) < limite;
        };

    //Recorremos todos los proyectiles activos del contenedor:
    for (auto& h : Hitboxes) {
        h.ActualizarHitbox(dt);
        if (!h.getEstadoHitbox()) continue;

        sf::Vector2f posH = h.getPosicionHitbox();
        float radioH = h.getFormaHitbox().getRadius();


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


        Pieza* objetivos[2] = { piezaDefensor, piezaAtacante };

        // Gestión de los ataques
        for (Pieza* obj : objetivos) {
            if (obj && h.getAtacante() != obj) {
                sf::Vector2f posE = obj->getPosicionAbsoluta();
                float distSq = std::pow(posH.x - posE.x, 2) + std::pow(posH.y - posE.y, 2);
                float limiteSq = std::pow(radioH + 20.f, 2);

                if (distSq < limiteSq) {

                    // Daño en el tiempo como hechizo del djinn o fenix
                    if (h.getEsDanoContinuo()) {
                        //Solo recibe daño si no es invulnerable
                        if (!obj->getInvulnerable()) {
                            obj->stats.vida -= h.getDano() * dt;
                        }
                    }
                    // Daño de ataques básicos
                    else if (!h.getYaHizoDano()) {
                        if (!obj->getInvulnerable()) { //Sólo recibe daño si no es invulnerable
                            obj->stats.vida -= h.getDano();
                        }
                        // Marcamos la hitbox como que ya impactó (aunque el escudo haya bloqueado el daño)
                        h.setYaHizoDano(true);
                        if (h.getVelocidadHitbox().x != 0 || h.getVelocidadHitbox().y != 0) {
                            h.setEstadoHitbox(false);
                        }
                        //Paralisis del basilisco:
                        if (h.getCausaInmovilizacion()) {
                            obj->aplicarInmovilizacion(h.getDuracionCC());
                            std::cout << "¡" << obj->stats.nombre << " ha sido inmovilizado!" << std::endl;
                        }
                        sf::Vector2f vel = h.getVelocidadHitbox();
                        if (vel.x != 0.f || vel.y != 0.f) {
                            h.setEstadoHitbox(false); // Proyectil muere
                        }
                    }
                }
            }
        }
    }


    //Limpieza de contenedores

    Hitboxes.erase(std::remove_if(Hitboxes.begin(), Hitboxes.end(), [](const Hitbox& h) {return !h.getEstadoHitbox(); }), Hitboxes.end());


    //Muerte y fin de combate

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

        //Comprobamos si la muerte del perdedor significa que se han satisfecho las condiciones de victoria:
        VerificarVictoria();

        //Llamamos a la lógica de avance de turno sólo si no ha terminado la partida:
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
    // No pongas el movimiento aquí, ya lo tienes en Motor::actualizar con isKeyPressed
}