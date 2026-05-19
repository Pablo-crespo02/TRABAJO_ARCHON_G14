#include "ClaseLider.h"
#include "ClaseKnight.h"
#include <cmath> 
#include <iostream>

const float PIEZA_ALTURA_TABLERO = 80.0f;
const float PIEZA_ALTURA_ARENA = 150.0f;

ClaseLider::ClaseLider(Bando b, sf::Vector2i pos, std::string tipo)
    : PiezaTeletransporte(b, pos)
{
    //ESTADÍSTICAS 
    this->stats.nombre = tipo;
    this->stats.vida = 50.0f;
    this->stats.vidaMaxima = 50.0f; // Ajusta a valores en el futuro
    this->stats.ataque = 12.0f;
    this->stats.defensa = 20.0f;
    this->stats.velAtaque = 0.9f;
    // --- Lógica de tipos ---
    this->stats.esRango = true;    // El Lider es Rango

    this->rangoMovimiento = 5;
    this->patronMovimiento = PatronMovimiento::Ambos;
    this->tipoMov = TipoMovimiento::Teletransporte;  // Solo para el HUD
    //CARGA DE SPRITES (Chibi)
    if (tipo == "CAPTAIN" || tipo == "HIVE_TYRANT") {

        std::string rutaTablero = (tipo == "CAPTAIN") ? "imagenes/BASE-CAPTAIN-Humanidad.png" : "imagenes/BASE-HIVE_TYRANT-TYRANIDS.png";
        std::string rutaArena = (tipo == "CAPTAIN") ? "imagenes/Chibi-CAPTAIN-Humanidad-1.0.png" : "imagenes/Chibi-HIVE_TYRANT-TYRANIDS-1.0.png";
        int columnas = 5;
        int filas = 2;

        if (!texturaTablero.loadFromFile(rutaTablero)) {
            std::cout << "Error: No se encontro " << rutaTablero << std::endl;
        }
        spriteTablero.setTexture(texturaTablero);
        spriteTablero.setOrigin(texturaTablero.getSize().x / 2.0f, texturaTablero.getSize().y / 2.0f);

        float escalaTablero = PIEZA_ALTURA_TABLERO / texturaTablero.getSize().y;
        spriteTablero.setScale(escalaTablero, escalaTablero);

        if (!texturaArena.loadFromFile(rutaArena)) {
            std::cout << "Error: No se encontro " << rutaArena << std::endl;
        }
        spriteArena.setTexture(texturaArena);

        anchoFrame = texturaArena.getSize().x / columnas;
        altoFrame = texturaArena.getSize().y / filas;

        spriteArena.setTextureRect(sf::IntRect(0, 0, anchoFrame, altoFrame));
        spriteArena.setOrigin(anchoFrame / 2.0f, altoFrame / 2.0f);

        float escalaArena = PIEZA_ALTURA_ARENA / altoFrame;
        if (this->bando == Bando::OSCURIDAD) {
            spriteArena.setScale(-escalaArena, escalaArena);
        }
        else {
            spriteArena.setScale(escalaArena, escalaArena);
        }

        frameActual = 0;
        temporizadorAnimacion = 0.0f;
    }
}

//ENLACE DE FÍSICAS Y ANIMACIÓN
void ClaseLider::procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) {
    //Dejamos que la clase padre (PiezaTerrestre) mueva las coordenadas físicas
    PiezaTeletransporte::procesarMovimientoArena(direccion, dt, arena);

    //Actualizamos la imagen visible con nuestra máquina de estados
    if (this->stats.nombre == "CAPTAIN" || this->stats.nombre == "HIVE_TYRANT") {
        animar(dt, direccion);
    }
}

void ClaseLider::animar(float dt, sf::Vector2f direccion) {
    int fila = 0;
    int colInicial = 0;
    int colFinal = 0;
    // Leemos el reloj de la habilidad. Si hace menos de 0.4 segundos que pulsamos la M, mostramos la pose.
    bool estaInvocando = (this->stats.relojHabilidad.getElapsedTime().asSeconds() < 0.4f);
    // Leemos el reloj interno. Si hace menos de 0.2 segundos que disparamos, estamos atacando.
    bool estaAtacando = (this->stats.relojHitbox.getElapsedTime().asSeconds() < 0.2f);

    if (estaAtacando) {
        //FOTOGRAMA DE ATAQUE 
        fila = 1;
        colInicial = 0;
        colFinal = 1;
    }
    else if (estaInvocando) {
        // FOTOGRAMA DE INVOCACIÓN 
        fila = 1;
        colInicial = 2;
        colFinal = 2; 
    }
    else if (direccion.x != 0) {
        //FOTOGRAMA DE CAMINAR LATERAL 
        fila = 0;
        colInicial = 1;
        colFinal = 4;
    }
    else if (direccion.y > 0) {
        //FOTOGRAMA DE ABAJO 
        fila = 1;
        colInicial = 3;
        colFinal = 3;
    }
    else if (direccion.y < 0) {
        // FOTOGRAMA DE ARRIBA 
        fila = 1;
        colInicial = 4;
        colFinal = 4;
    }
    else {
        //FOTOGRAMA QUIETO
        fila = 0;
        colInicial = 0;
        colFinal = 0;
    }

    int posY_Textura = fila * altoFrame;
    int posY_Actual = spriteArena.getTextureRect().top;

    if (posY_Actual != posY_Textura || frameActual < colInicial || frameActual > colFinal) {
        frameActual = colInicial;
        spriteArena.setTextureRect(sf::IntRect(frameActual * anchoFrame, posY_Textura, anchoFrame, altoFrame));
        temporizadorAnimacion = 0.0f;
    }

    if (colInicial != colFinal) {
        temporizadorAnimacion += dt;
        float velocidadAnimacion = 0.15f;

        if (temporizadorAnimacion >= velocidadAnimacion) {
            temporizadorAnimacion = 0.0f;
            frameActual++;

            if (frameActual > colFinal) {
                frameActual = colInicial;
            }

            spriteArena.setTextureRect(sf::IntRect(frameActual * anchoFrame, posY_Textura, anchoFrame, altoFrame));
        }
    }

    //ARREGLO DEL EFECTO ESPEJO
    float escalaArena = PIEZA_ALTURA_ARENA / altoFrame;
    if (direccion.x < 0) {
        spriteArena.setScale(-escalaArena, escalaArena); // Mira a la izquierda
    }
    else if (direccion.x > 0) {
        spriteArena.setScale(escalaArena, escalaArena);  // Mira a la derecha
    }
    else {
        //Si va hacia arriba, abajo, ataca o se queda quieto, respeta la dirección a la que miraba
        float escalaActualX = (spriteArena.getScale().x > 0) ? escalaArena : -escalaArena;
        spriteArena.setScale(escalaActualX, escalaArena);
    }
}

void ClaseLider::dibujar(sf::RenderWindow& window, Estado estadoActual) {
    if (estadoActual == Estado::Tablero) {
        this->sincronizarPosicionTablero();

        if (this->stats.nombre == "CAPTAIN1" || this->stats.nombre == "HIVE_TYRANT") {

            //CÍRCULO DE SELECCIÓN AMARILLO
            if (seleccionado) {
                sf::CircleShape anilloSeleccion(25.f);
                anilloSeleccion.setOrigin(25.f, 25.f);
                anilloSeleccion.setPosition(posicionAbsoluta);
                anilloSeleccion.setFillColor(sf::Color::Transparent); // Fondo vacío
                anilloSeleccion.setOutlineThickness(4.f);             // Borde grueso
                anilloSeleccion.setOutlineColor(Colores::ColorOutlineSeleccion); // Amarillo
                window.draw(anilloSeleccion);
            }

            spriteTablero.setPosition(posicionAbsoluta);
            window.draw(spriteTablero);
        }
        else {
            formaVisual.setPosition(posicionAbsoluta);
            formaVisual.setFillColor(bando == Bando::LUZ ? Colores::ColorFichaLuz : Colores::ColorFichaOscuridad);
            if (seleccionado) {
                formaVisual.setOutlineThickness(4.0f);
                formaVisual.setOutlineColor(Colores::ColorOutlineSeleccion);
            }
            else {
                formaVisual.setOutlineThickness(0.0f);
            }
            window.draw(formaVisual);
        }
    }
    else if (estadoActual == Estado::Arena) {
        if (this->stats.nombre == "CAPTAIN1" || this->stats.nombre == "HIVE_TYRANT") {
            spriteArena.setPosition(posicionAbsoluta);
            window.draw(spriteArena);
        }
        else {
            formaVisual.setPosition(posicionAbsoluta);
            window.draw(formaVisual);
        }
        // 2. DIBUJADO DE SUS MINIONS
        for (Pieza* minion : minionsInvocados) {
            // Llamamos al propio método dibujar del minion (que al ser un Termagant ejecutará su sprite Chibi)
            minion->dibujar(window, estadoActual);
        }
        //DIBUJAMOS BARRA DE VIDA SOBRE LA PIEZA
        barrasArena.actualizar(stats.vida, stats.vidaMaxima, stats.velAtaque, posicionAbsoluta);
        barrasArena.dibujar(window);

    }
}

void ClaseLider::usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo) {
    if (!enemigo) return;

    if (this->stats.nombre == "CAPTAIN") return;

    this->stats.relojHabilidad.restart();

    limpiarMinions();

    // --- CAMBIADO A 2 MONSTRUOS ---
    int numeroMinions = 2;
    float offsetsX[2] = { -40.f, 40.f };

    for (int i = 0; i < numeroMinions; i++) {
        sf::Vector2f puntoSpawn = this->posicionAbsoluta + sf::Vector2f(offsetsX[i], -30.f);

        ClaseKnight* minion = new ClaseKnight(this->bando, sf::Vector2i(0, 0), "TERMAGANT");

        minion->setPosicionAbsoluta(puntoSpawn);
        minion->setEscalaMinion(0.6f);

        minion->stats.vida = 15.0f;
        minion->stats.vidaMaxima = 15.0f;
        minion->stats.ataque = 3.0f;
        minion->stats.velAtaque = 1.0f;

        // --- ASIGNACIÓN DE COMPORTAMIENTO ---
        // Usamos la estadística de defensa de forma temporal para marcar su rol sin tocar el .h
        if (i == 0) {
            minion->stats.rol = 1; // Rol: Cazador (Persecución directa)
        }
        else {
            minion->stats.rol = 2; // Rol: Estratega (Predicción / Intercepción)
        }

        minionsInvocados.push_back(minion);
    }
}

void ClaseLider::actualizarMinions(float dt, Arena& arena, Pieza* enemigo) {
    if (!enemigo) return;

    auto it = minionsInvocados.begin();
    while (it != minionsInvocados.end()) {
        Pieza* minion = *it;

        sf::Vector2f posicionJugador = enemigo->getPosicionAbsoluta();
        sf::Vector2f puntoObjetivo = posicionJugador;

        // Calculamos la distancia REAL en línea recta hacia el cuerpo del jugador
        sf::Vector2f dirRealJugador = posicionJugador - minion->getPosicionAbsoluta();
        float distanciaRealJugador = std::hypot(dirRealJugador.x, dirRealJugador.y);

        // Rango de ataque asignado
        float rangoAtaque = 45.f;

        // 1. IA COMPORTAMENTAL (Solo calcula el punto si está fuera de rango)
        if (minion->stats.rol == 2. && distanciaRealJugador > 120.f) {
            float factorAnticipacion = 100.f;
            sf::Vector2f dirEnemigo = enemigo->getUltimaDireccion();
            puntoObjetivo = posicionJugador + (dirEnemigo * factorAnticipacion);
        }

        // 2. MOVIMIENTO CONDICIONADO (¡CON SENSADO DE OBSTÁCULOS NATIVO!)
        if (distanciaRealJugador > rangoAtaque) {
            sf::Vector2f dirHaciaObjetivo = puntoObjetivo - minion->getPosicionAbsoluta();
            float distanciaAlObjetivo = std::hypot(dirHaciaObjetivo.x, dirHaciaObjetivo.y);

            if (distanciaAlObjetivo > 0.f) {
                sf::Vector2f dirFinal = dirHaciaObjetivo / distanciaAlObjetivo;

                // ---------------------------------------------------------
                // --- RADAR ANTICOLISIÓN: ESQUIVE DE PIEDRAS/MUROS ---
                // ---------------------------------------------------------
                float distanciaRadar = 70.f; // Distancia para empezar a predecir el choque
                sf::Vector2f posicionAdelantada = minion->getPosicionAbsoluta() + (dirFinal * distanciaRadar);
                float radioMinion = 28.f;

                // Le preguntamos a tu arena si la posición a la que va a llegar es sólida
                if (!arena.esPosicionValida(posicionAdelantada, radioMinion, false)) {
                    // ¡Alerta! Hay un obstáculo delante. Vamos a calcular una dirección de desvío lateral.
                    // Probamos a desviarnos de forma perpendicular (90 grados) para rodear el objeto.
                    // Vector perpendicular a la derecha: (-dirFinal.y, dirFinal.x)
                    sf::Vector2f desvioLateral(-dirFinal.y, dirFinal.x);

                    // Probamos si el desvío a la derecha está libre
                    sf::Vector2f pruebaDerecha = minion->getPosicionAbsoluta() + (desvioLateral * distanciaRadar);

                    if (arena.esPosicionValida(pruebaDerecha, radioMinion, false)) {
                        // Si la derecha está libre, combinamos el avance con el desvío para rodear la piedra
                        dirFinal = (dirFinal * 0.2f) + (desvioLateral * 0.8f);
                    }
                    else {
                        // Si la derecha está bloqueada, intentamos esquivar por la izquierda
                        sf::Vector2f desvioIzquierda(dirFinal.y, -dirFinal.x);
                        dirFinal = (dirFinal * 0.2f) + (desvioIzquierda * 0.8f);
                    }

                    // Volvemos a normalizar el vector resultante para mantener la velocidad constante
                    float largoDir = std::hypot(dirFinal.x, dirFinal.y);
                    if (largoDir > 0.f) dirFinal /= largoDir;
                }
                // ---------------------------------------------------------

                // Aplicamos el movimiento (con el vector corregido si había piedra)
                minion->procesarMovimientoArena(dirFinal, dt, arena);

                ClaseKnight* knightMinion = dynamic_cast<ClaseKnight*>(minion);
                if (knightMinion != nullptr) {
                    knightMinion->animar(dt, dirFinal);
                }
            }
        }

        // 3. ATAQUE POR COOLDOWN
        bool haAtacadoEnEsteFrame = false; // Variable auxiliar para avisar a la barra

        if (distanciaRealJugador < rangoAtaque) {
            if (minion->stats.relojHitbox.getElapsedTime().asSeconds() >= minion->stats.velAtaque) {
                if (!enemigo->getInvulnerable()) {
                    enemigo->stats.vida -= minion->stats.ataque;
                    std::cout << "Un Termagant ha mordido al enemigo haciendo " << minion->stats.ataque << " de dano!" << std::endl;
                }

                // 1. Reiniciamos el reloj físico del minion para la lógica
                minion->stats.relojHitbox.restart();

                // Marcamos que ha atacado para indicárselo a la barra un poco más abajo
                haAtacadoEnEsteFrame = true;
            }
        }

        // --- ACTUALIZACIÓN DE BARRAS DESDE LA FUNCIÓN PÚBLICA DE PIEZA ---
        // Le pasamos todos los datos necesarios. Si 'haAtacadoEnEsteFrame' es true, reiniciará el reloj visual internamente.
        minion->gestionarBarraAtaqueMinion(
            minion->stats.vida,
            minion->stats.vidaMaxima,
            minion->stats.velAtaque,
            minion->getPosicionAbsoluta(),
            haAtacadoEnEsteFrame
        );


        // 4. CONTROL DE MUERTE
        if (minion->stats.vida <= 0.f) {
            delete minion;
            it = minionsInvocados.erase(it);
            std::cout << "Un minion Termagant ha muerto en combate." << std::endl;
        }
        else {
            ++it;
        }
    }
}

void ClaseLider::limpiarMinions() {
    for (Pieza* minion : minionsInvocados) {
        delete minion;
    }
    minionsInvocados.clear();
}

ClaseLider::~ClaseLider() {
    limpiarMinions();
}