#include "ClaseArcher.h"
#include <iostream>

ClaseArcher::ClaseArcher(Bando b, sf::Vector2i pos, std::string tipo)
    : PiezaTerrestre(b, pos) // Llama al constructor de la clase intermedia
{
    this->stats.nombre = tipo;
    this->stats.vida = 18.0f;
    this->stats.vidaMaxima = 18.0f;
    this->stats.ataque = 5.0f;
    this->stats.defensa = 8.0f;
    this->rangoMovimiento = 3;
    this->stats.velAtaque = 1.2f;
    this->stats.esRango = true;

    // Asignación del patrón de movimiento
    this->patronMovimiento = PatronMovimiento::Ambos;
    //Control de la invisibilidad (bando luz):
    esInvisible = false;
    temporizadorInvisibilidad = 0.0;
    duracionInvisibilidad = 4.0; // 4.0 segundos de duración
    //Control del boost de velocidad (bando oscuridad)
    tieneBoostVelocidad = false;
    temporizadorBoost = 0.0;
    duracionBoost = 5.0; // 5 segundos de velocidad extrema
    multiplicadorVelocidad = 1.0f; // 1.0 = velocidad normal
}

// Aquí NO deben ir las funciones de movimiento. Ya están en PiezaTeletransporte.cpp.

void ClaseArcher::dibujar(sf::RenderWindow& window, Estado estadoActual) {
    if (estadoActual == Estado::Tablero) {
        this->sincronizarPosicionTablero();
        formaVisual.setFillColor(bando == Bando::LUZ ? Colores::ColorFichaLuz : Colores::ColorFichaOscuridad);

        if (seleccionado) {
            formaVisual.setOutlineThickness(4.0f);
            formaVisual.setOutlineColor(Colores::ColorOutlineSeleccion);
        }
        else {
            formaVisual.setOutlineThickness(0.0f);
        }
    }
    else if (estadoActual == Estado::Arena) {
        formaVisual.setPosition(posicionAbsoluta);
        formaVisual.setOutlineThickness(0.0f);
        formaVisual.setOrigin(20.f, 20.f);
        //Barra de salud:
        barrasArena.actualizar(stats.vida, stats.vidaMaxima, stats.velAtaque,posicionAbsoluta);
        barrasArena.dibujar(window);
    }
    window.draw(formaVisual);
}
//Hechizo 
void ClaseArcher::usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo) {
    if (this->bando == Bando::LUZ) {
        // Hechizo luminoso: Invisibilidad
        esInvisible = true;
        temporizadorInvisibilidad = duracionInvisibilidad;
        std::cout << "El Arquero (Luz) activa su camuflaje optico." << std::endl;
    }
    else if (this->bando == Bando::OSCURIDAD) {
        tieneBoostVelocidad = true;
        temporizadorBoost = duracionBoost;
        multiplicadorVelocidad = 3.0f;//x3 de multiplicador de velocidad
        std::cout << "El Arquero (Oscuridad) activa Sprint Sombrio." << std::endl;
    }
}
//Unicamente afecta al bando luz y se encarga de medir el tiempo que permanece en invisible
void ClaseArcher::gestionarInvisibilidad(double dt) {
    if (esInvisible) {
        // Restamos el delta time convertido a double
        temporizadorInvisibilidad -= dt;

        sf::Color colorAct = formaVisual.getFillColor();
        colorAct.a = 40; //Si lo ponemos a cero se vuelve completamente invisible
        formaVisual.setFillColor(colorAct);

        // Si el tiempo se agota, restauramos el estado original
        if (temporizadorInvisibilidad <= 0.0) {
            esInvisible = false;
            temporizadorInvisibilidad = 0.0;

            colorAct.a = 255; // Color normal
            formaVisual.setFillColor(colorAct);
            std::cout << "El camuflaje del Arquero se ha agotado." << std::endl;
        }
    }
}
//Unicamente afecta al bando oscuridad y mide el tiempo que pasa con el x3 de velocidad
void ClaseArcher::gestionarBoostVelocidad(double dt) {
    if (tieneBoostVelocidad) {
        // Restamos el tiempo
        temporizadorBoost -= dt;
        if (temporizadorBoost <= 0.0) {
            // Fin del hechizo, devolvemos la velocidad a la normalidad
            tieneBoostVelocidad = false;
            temporizadorBoost = 0.0;
            multiplicadorVelocidad = 1.0f;
            std::cout << "El Sprint Sombrio del Arquero se ha agotado." << std::endl;
        }
    }
}
// Interceptamos la función de movimiento para insertar nuestra actualización de tiempo
//Sirve para que el motor de las físicas (Motor::actualizar) se ponga de acuerdo con "dt"
void ClaseArcher::procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) {
    //  Cronómetro de cada pieza
    double deltaTiempo = static_cast<double>(dt);
    gestionarInvisibilidad(deltaTiempo);
    gestionarBoostVelocidad(deltaTiempo);

    // Inactividad
    if (direccion == sf::Vector2f(0.f, 0.f)) return;

   //Reescribe la velocidad por defecto de PiezaTerrestre
    float velocidadBase = 200.f;
    float velocidadFinal = velocidadBase * multiplicadorVelocidad;

    sf::Vector2f desplazamiento = direccion * velocidadFinal * dt;
    sf::Vector2f nuevaPos = posicionAbsoluta + desplazamiento;

    // Evaluamos la colisión contra los muros de la Arena (false = no vuela)
    if (arena.esPosicionValida(nuevaPos, 20.f, false)) {
        this->moverEnArena(desplazamiento.x, desplazamiento.y);
    }
}