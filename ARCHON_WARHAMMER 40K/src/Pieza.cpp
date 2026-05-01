#include "Pieza.h"
#include <iostream>
#include <cmath>

Pieza::Pieza() {
    // Inicialización por defecto según pediste
    bando = Bando::LUZ;
    tipoMov = TipoMovimiento::TERRESTRE;
    patron = PatronMovimiento::AMBOS;
    rangoMovimiento = 2;
    posicionTablero = { 0, 0 };

    stats = { 100.0f, 1.0f, false, 10, 5, 1.0f }; //Vida, cooldown, EsRango, ataque, defensa y velocidad de ataque

    formaVisual.setRadius(20.f);
    formaVisual.setOrigin(20.f, 20.f);
}

bool Pieza::poderMover(sf::Vector2i destino, const std::vector<Pieza*>& otrasPiezas, bool esDestinoOcupado) {
    // Cálculo de distancia
    int distX = std::abs(destino.x - posicionTablero.x);
    int distY = std::abs(destino.y - posicionTablero.y);

    // Comprobar rango
    if (distX > rangoMovimiento || distY > rangoMovimiento) return false;

    // ---VALIDACIÓN DE PATRÓN DE MOVIMIENTO ---
    bool patronValido = false;

    if (patron == PatronMovimiento::ORTOGONAL) {
        // En ortogonal, una de las distancias debe ser 0 (solo se mueve en cruz)
        if (distX == 0 || distY == 0) patronValido = true;
    }
    else if (patron == PatronMovimiento::DIAGONAL) {
        // En diagonal, el desplazamiento en X e Y debe ser idéntico (45 grados)
        if (distX == distY) patronValido = true;
    }
    else if (patron == PatronMovimiento::AMBOS) {
        // Acepta cualquiera de las dos reglas anteriores
        if (distX == 0 || distY == 0 || distX == distY) patronValido = true;
    }

    if (!patronValido) return false; // Si no cumple el dibujo del patrón, fuera.

    // ---VALIDACIÓN DE TIPO DE MOVIMIENTO ---

    // Si es terrestre, revisamos si hay alguien entre la pieza y el destino

    if (tipoMov == TipoMovimiento::TERRESTRE) {
        // Calculamos la dirección del paso (-1, 0, o 1)
        int stepX = (destino.x > posicionTablero.x) ? 1 : (destino.x < posicionTablero.x ? -1 : 0);
        int stepY = (destino.y > posicionTablero.y) ? 1 : (destino.y < posicionTablero.y ? -1 : 0);

        sf::Vector2i revision = posicionTablero;

        // Bucle que avanza por el camino hasta la casilla ANTERIOR al destino
        while (revision.x + stepX != destino.x || revision.y + stepY != destino.y) {//lo que hace es revisar las casillas anteriores al destino, si hay una ficha antes,
            if (revision.x + stepX != destino.x) revision.x += stepX;// colisiona, si es destino final, van a la arena a luchar
            if (revision.y + stepY != destino.y) revision.y += stepY;//revision es una mirada al futuro, step+ revision es ir a la siguiente casilla

            // Buscamos si hay alguna pieza (amiga o enemiga) bloqueando el paso
            for (const auto* otra : otrasPiezas) {
                if (otra->getPosicion() == revision) {// revision es la siguiente casilla, si esta ocupada otra==True, retorna false a la funcion podermover()
                    return false; // CAMINO BLOQUEADO
                }
            }
            // Seguridad: si el movimiento es de solo 1 casilla, no entra aquí
            if (distX <= 1 && distY <= 1) break;
        }
    }
    // Si es VOLADOR, el bloque anterior se ignora (salta piezas)

    // VALIDACIÓN DEL DESTINO FINAL
    for (const auto* otra : otrasPiezas) {
        if (otra->getPosicion() == destino) {
            // No podemos aterrizar sobre un aliado
            if (otra->getBando() == this->bando) return false;
            // Si es enemigo, permitimos (se activará el combate en Mundo.cpp)
        }
    }

    return true;



    // Comprobar ocupación
    for (const auto* otra : otrasPiezas) {
        if (otra->getPosicion() == destino) {
            if (otra->getBando() == this->bando) {
                return false; // Aliado: No se puede mover aquí
            }
            // Si es enemigo, devolvemos true porque iniciará conflicto
        }
    }

    return true;
}

void Pieza::mover(sf::Vector2i destino) {
    animacionMover();
    posicionTablero = destino;
}

void Pieza::animacionMover() {
    // Por ahora un mensaje, luego será una transición de posición suave
    std::cout << "Animando movimiento..." << std::endl;
}

bool Pieza::detectarConflicto(const std::vector<Pieza*>& otrasPiezas) {
    for (const auto* otra : otrasPiezas) {
        if (otra->getPosicion() == this->posicionTablero && otra->getBando() != this->bando) {
            return true;
        }
    }
    return false;
}

void Pieza::iniciarConflicto() {
    std::cout << "TRANSICION: Entrando en modo batalla tiempo real..." << std::endl;
    // Aquí se llamaría al motor de lucha
}

void Pieza::dibujar(sf::RenderWindow& window) {
    float px = static_cast<float>(posicionTablero.x) * 60.0f + 50.0f + 30.0f;
    float py = static_cast<float>(posicionTablero.y) * 60.0f + 30.0f + 30.0f;

    formaVisual.setPosition(px, py);

    // --- EFECTO VISUAL DE SELECCIÓN ---
    if (seleccionado) {
        formaVisual.setOutlineThickness(4.0f);   // Grosor del borde
        formaVisual.setOutlineColor(sf::Color::Yellow); // Color del borde
    }
    else {
        formaVisual.setOutlineThickness(0.0f);   // Sin borde si no está seleccionada
    }

    window.draw(formaVisual);
}
