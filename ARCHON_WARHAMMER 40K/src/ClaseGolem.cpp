#include "ClaseGolem.h"
#include <cmath> 
#include <iostream>

ClaseGolem::ClaseGolem(Bando b, sf::Vector2i pos, std::string tipo)
    : PiezaTerrestre(b, pos)
{
    //ESTADÍSTICAS 
    this->stats.nombre = tipo;
    this->stats.vida = 30.0f;
    this->stats.vidaMaxima = 30.0f; // Ajusta a valores en el futuro
    this->stats.ataque = 6.0f;
    this->stats.defensa = 20.0f;
    this->stats.velAtaque = 1.0f;
    // --- Lógica de tipos ---
    this->stats.esRango = false;    // El Golem es melee

    this->rangoMovimiento = 2;
    this->patronMovimiento = PatronMovimiento::Ortogonal;
    this->tipoMov = TipoMovimiento::Terrestre;  // Solo para el HUD

    //Configuración de tamaños de los sprites:
    this->piezaAlturaTablero = 90;
    this->piezaAlturaArena = 120;

    //CARGA DE SPRITES
    cargarConfigurarSprites(tipo);

    if (animador) {
        animador->agreganAnimacion("ATAQUE", 1, 2, 2, 0.20f, true);
    }

}

//ENLACE DE FÍSICAS Y ANIMACIÓN
void ClaseGolem::procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) {
    //Dejamos que la clase padre (PiezaTerrestre) mueva las coordenadas físicas
    PiezaTerrestre::procesarMovimientoArena(direccion, dt, arena);

    //Actualizamos la imagen visible con nuestra máquina de estados
    if (this->stats.nombre == "DREADNOUGHT" || this->stats.nombre == "CARNIFEX") {
        Pieza::Animar(dt, direccion);
    }
}

void ClaseGolem::dibujar(sf::RenderWindow& window, Estado estadoActual) {
    if (estadoActual == Estado::Tablero) {
        this->sincronizarPosicionTablero();

        if (this->stats.nombre == "DREADNOUGHT" || this->stats.nombre == "CARNIFEX") {

            //CÍRCULO DE SELECCIÓN AMARILLO
            if (seleccionado) {
                dibujarAnilloSeleccion(window);
            }

            spriteTablero.setPosition(posicionAbsoluta);
            window.draw(spriteTablero);
        }
        
    }
    else if (estadoActual == Estado::Arena) {
        if (this->stats.nombre == "DREADNOUGHT" || this->stats.nombre == "CARNIFEX") {
            spriteArena.setPosition(posicionAbsoluta);
            window.draw(spriteArena);
        }
      

        //DIBUJAMOS BARRA DE VIDA SOBRE LA PIEZA
        barrasArena.actualizar(stats.vida, stats.vidaMaxima, stats.velAtaque, posicionAbsoluta);
        barrasArena.dibujar(window);

    }
}
void ClaseGolem::usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo) {
    // El Golem se repara a sí mismo (Heal)
    float curacion = 10.0f;
    this->stats.vida += curacion;

    // Evitamos que se cure por encima de su vida máxima
    if (this->stats.vida > this->stats.vidaMaxima) {
        this->stats.vida = this->stats.vidaMaxima;
    }
}