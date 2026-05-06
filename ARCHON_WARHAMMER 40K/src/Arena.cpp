#include "Arena.h"
#include <ctime>
#include <algorithm>
#include <iostream>
#include <cmath>   

Arena::Arena() {
    //Configuración básica del Suelo (Solo valores, no dibuja)
    suelo.setSize(sf::Vector2f(ANCHO_MAPA, ALTO_MAPA));
    suelo.setFillColor(sf::Color(80, 70, 60));

    //Configuración de la Cámara/Vista
    vistaArena.setCenter(ANCHO_MAPA / 2.f, ALTO_MAPA / 2.f);
    vistaArena.setSize(ANCHO_MAPA * 1.1f, ALTO_MAPA * 1.1f);
}

HitboxesArena Arena::obtenerColisiones() const {
    HitboxesArena hitboxes;

    // Procesamos los muros aquí
    // Los muros se tratan como rectángulos alineados a los ejes
    for (const auto& muro : muros) {
        // push_back añade el rectángulo de colisión
        hitboxes.rectangulos.push_back(muro.getGlobalBounds());
    }

    // Procesamos las rocas también
    // Convierte las cajas delimitadoras de las rocas en formas circulares.
    for (const auto& roca : rocas) {
        sf::FloatRect bounds = roca.getGlobalBounds();

        // Calcula el radio tomando el lado más largo
        float radio = std::max(bounds.width, bounds.height) / 2.0f;

        // Calcula el centro geométrico del objeto.
        sf::Vector2f centro(bounds.left + (bounds.width / 2.0f),
            bounds.top + (bounds.height / 2.0f));

        // Almacena el círculo resultante (centro + radio).
        hitboxes.circulos.push_back({ centro, radio });
    }

    return hitboxes;
}

bool Arena::esPosicionValida(sf::Vector2f pos, float radio, bool esVoladora) const {
    // 1. Obtenemos todas las hitboxes del mapa.
    HitboxesArena hitboxes = obtenerColisiones();

    // 2. Colisiones con los muros
    // Se comprueba si el punto 'pos' está dentro de algún muro.
    for (auto& rect : hitboxes.rectangulos) {
        if (rect.contains(pos)) return false; // Si el punto está dentro, la posición no es válida.
    }

    // 3. Colisión entre el círculo del spawn y los objetos que se puedan generar ahí
    // Si la entidad "vuela", ignoramos las colisiones del ambiente
    if (!esVoladora) {
        for (auto& circ : hitboxes.circulos) {
            // Cálculo de distancia al cuadrado 
            float dx = pos.x - circ.centro.x;
            float dy = pos.y - circ.centro.y;
            float distSq = dx * dx + dy * dy; // Distancia entre centros al cuadrado.

            float radiosSumados = radio + circ.radio;

            // Si la distancia al cuadrado es menor que la suma de radios al cuadrado, hay colisión.
            if (distSq < (radiosSumados * radiosSumados)) return false;
        }
    }

    // Si no chocó con nada, la posición es apta.
    return true;
}

void Arena::dibujar(sf::RenderWindow& window) const {
    // Aplicamos la vista específica de la arena (zoom/posición)
    window.setView(this->vistaArena);

    // Capa Base: Suelo y Rejillas
    window.draw(this->suelo);
    for (const auto& r : this->rejillasSuelo) {
        window.draw(r.marco);
        for (const auto& barra : r.barras) window.draw(barra);
    }

    // Capa de Suelo: Sangre
    for (const auto& mancha : this->charcosSangre) window.draw(mancha);
    for (const auto& gota : this->gotasSangre) window.draw(gota);

    // Capa de Sombras de Muros 
    for (const auto& sombraMuro : this->sombrasMuros) {
        window.draw(sombraMuro);
    }

    // Rocas con efecto 3D
    // Nota: sf::ConvexShape no es costoso de copiar para el dibujo, pero usamos const para seguridad
    for (size_t i = 0; i < this->rocas.size(); ++i) {
        window.draw(this->sombras[i]); // Sombra proyectada

        sf::ConvexShape cuerpo = this->rocas[i];
        cuerpo.setFillColor(sf::Color(80, 80, 85));

        // Extrusión para efecto 3D
        for (int j = 0; j < 10; ++j) {
            cuerpo.move(0.f, -1.f);
            window.draw(cuerpo);
        }

        // Tapa superior
        sf::ConvexShape tapa = this->rocas[i];
        tapa.move(0.f, -10.f);
        tapa.setFillColor(sf::Color(100, 100, 105));
        window.draw(tapa);
    }

    // Capa Final: Muros
    for (const auto& muro : this->muros) {
        window.draw(muro);
    }
}


/*
  Valida si una posición es apta para spawnear un objeto sin colisionar con otros.
  posActual    Coordenada (x, y) donde se intenta generar el objeto.
  radioActual  Radio de colisión del objeto a generar.
  tipo         Categoría del objeto (Roca, Item, etc.) para aplicar reglas específicas.
  true        Si la posición está despejada.
  false       Si hay una colisión o solapamiento detectado.
 */
bool Arena::esGeneracionValida(sf::Vector2f posActual, float radioActual, TipoObjeto tipo) {

    // (SPAWNS)
    float radioSpawn = 70.f;              // Radio de seguridad alrededor de los puntos de inicio
    sf::Vector2f spawnLuz(150.f, 300.f);  // Punto de spawn luminoso
    sf::Vector2f spawnOsc(650.f, 300.f);  // Punto de spawn oscuro

    // Distancia
    // Usamos el Teorema de Pitágoras:
    auto distancia = [](sf::Vector2f p1, sf::Vector2f p2) {
        return std::sqrt(std::pow(p2.x - p1.x, 2) + std::pow(p2.y - p1.y, 2));
        };

    // Compara el círculo del spawn  con los objetos
    // Si la distancia entre el nuevo objeto y el spawn es menor que la suma de sus radios, hay solapamiento.
    if (distancia(posActual, spawnLuz) < (radioActual + radioSpawn)) return false;
    if (distancia(posActual, spawnOsc) < (radioActual + radioSpawn)) return false;

    // Comprobación con rejillas, para que no spwanee una piedra encima de una rejilla
    // Recorre todas las rejillas existentes en el suelo.
    for (const auto& rej : rejillasSuelo) {
        // Se usa un margen fijo de 40 unidades adicionales para las rejillas.
        if (distancia(posActual, rej.marco.getPosition()) < (radioActual + 40.f)) {
            return false;
        }
    }

    // Colisión roca con roca
    // Si el objeto que estamos intentando crear es una Roca, comprobamos que no choque con otras rocas.
    if (tipo == TipoObjeto::Roca) {
        for (const auto& roca : rocas) {
            // Se aplica un margen de 30 unidades para el espaciado entre rocas.
            if (distancia(posActual, roca.getPosition()) < (radioActual + 30.f)) {
                return false;
            }
        }
    }

    // Si pasó todas las pruebas anteriores, la posición es válida.
    return true;
}
// Hay que mejorarla, no se nota suficiente cambio en la arena
void Arena::establecerAmbiente(sf::Color colorCasilla) {
    if (colorCasilla == sf::Color::White || (colorCasilla.r > 200 && colorCasilla.g > 200)) {
        suelo.setFillColor(sf::Color(255, 250, 220));
    }
    else if (colorCasilla.r < 50 && colorCasilla.g < 50) {
        suelo.setFillColor(sf::Color(20, 20, 25));
    }
    else {
        sf::Color grisSuave = colorCasilla;
        grisSuave.a = 255;
        suelo.setFillColor(grisSuave);
    }
}