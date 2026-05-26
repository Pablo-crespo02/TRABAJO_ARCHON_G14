#include "ClaseValkyria.h"
#include <cmath> 
#include <iostream>

const float PIEZA_ALTURA_TABLERO = 80.0f;
const float PIEZA_ALTURA_ARENA = 120.0f;

ClaseValkyria::ClaseValkyria(Bando b, sf::Vector2i pos, std::string tipo)
    : PiezaVoladora(b, pos)
{
    //ESTADÍSTICAS 
    this->stats.nombre = tipo;
    this->stats.vida = 35.0f;
    this->stats.vidaMaxima = 35.0f; // Ajusta a valores en el futuro
    this->stats.ataque = 5.0f;
    this->stats.defensa = 20.0f;
    this->stats.velAtaque = 1.2f;
    // --- Lógica de tipos ---
    this->stats.esRango = true;    // La valkiria es rango

    this->rangoMovimiento = 4;
    this->patronMovimiento = PatronMovimiento::Ambos;
    this->tipoMov = TipoMovimiento::Volador;  // Solo para el HUD

    //TAMAÑO DE LOS SPRITES:
    this->piezaAlturaTablero = 80.0f;
    this->piezaAlturaArena = 120.0f;

    //CARGA DE SPRITES (Chibi)
    cargarConfigurarSprites(tipo);

}

//ENLACE DE FÍSICAS Y ANIMACIÓN
void ClaseValkyria::procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) {
    //Dejamos que la clase padre (PiezaTerrestre) mueva las coordenadas físicas
    PiezaVoladora::procesarMovimientoArena(direccion, dt, arena);

    //Actualizamos la imagen visible con nuestra máquina de estados

    if (this->stats.nombre == "ASSAULT MARINE" || this->stats.nombre == "GARGOLA") {
        Pieza::Animar(dt, direccion);
    }
}

void ClaseValkyria::dibujar(sf::RenderWindow& window, Estado estadoActual) {
    if (estadoActual == Estado::Tablero) {
        this->sincronizarPosicionTablero();

        if (this->stats.nombre == "ASSAULT MARINE" || this->stats.nombre == "GARGOLA") {

            //CÍRCULO DE SELECCIÓN AMARILLO
            if (seleccionado) {
                dibujarAnilloSeleccion(window);
            }

            spriteTablero.setPosition(posicionAbsoluta);
            window.draw(spriteTablero);
        }
       
    }
    else if (estadoActual == Estado::Arena) {
        if (this->stats.nombre == "ASSAULT MARINE" || this->stats.nombre == "GARGOLA") {
            spriteArena.setPosition(posicionAbsoluta);
            window.draw(spriteArena);
        }
       

        //DIBUJAMOS BARRA DE VIDA SOBRE LA PIEZA
        barrasArena.actualizar(stats.vida, stats.vidaMaxima, stats.velAtaque, posicionAbsoluta);
        barrasArena.dibujar(window);

    }
}
void ClaseValkyria::usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo) {
   
    if (this->stats.nombre == "ASSAULT_MARINE") {
        sf::Vector2f dirFija(0.f, 0.f);

        hitboxes.emplace_back(
            this->posicionAbsoluta,
            dirFija,
            0.0,
            sf::Color(255, 255, 100, 160),         // Circulo amarillo
            this,
            0.0,                                   // No hace daño
            3.0,                                   // AUMENTADO: El escudo dura 3 segundos activo
            150.0,                                 // AUMENTADO: Un área generosa alrededor de la pieza
            false, false, false, 0.0, false, 0.0,
            true,                                  // causaEmpuje activado
            600.0f                                 
        );

        this->stats.relojHabilidad.restart();
        std::cout << "¡La Valkyria despliega un campo repulsor!" << std::endl;
    }
    else if (this->stats.nombre == "GARGOLA") {
        // Calculamos la dirección hacia el enemigo para lanzar un proyectil (o puede ser estático)
        sf::Vector2f dirAtaque = enemigo->getPosicionAbsoluta() - this->posicionAbsoluta;
        float magnitud = std::hypot(dirAtaque.x, dirAtaque.y);
        if (magnitud != 0.f) dirAtaque /= magnitud; // Normalizamos

        enemigo->aplicarRalentizacion(0.5, 4.0); //50% de ralentización durante 4 segundos

        this->stats.relojHabilidad.restart();
        std::cout << "¡GARGOLA lanza una maldición que ralentiza al enemigo!" << std::endl;
    }
}