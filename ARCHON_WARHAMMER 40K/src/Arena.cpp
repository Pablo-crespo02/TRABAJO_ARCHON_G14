#include "Arena.h"
#include <ctime>
#include <algorithm>
#include <iostream>
#include <cmath>   

Arena::Arena() {
    // 1. Configuración básica del Suelo (Solo valores, no dibuja)
    suelo.setSize(sf::Vector2f(ANCHO_MAPA, ALTO_MAPA));
    suelo.setFillColor(sf::Color(80, 70, 60));

    // 2. Configuración de la Cámara/Vista
    vistaArena.setCenter(ANCHO_MAPA / 2.f, ALTO_MAPA / 2.f);
    vistaArena.setSize(ANCHO_MAPA * 1.1f, ALTO_MAPA * 1.1f);

    //No llamamos a generarMapaProcedural aquí, 
    // lo hará el Generador cuando sea necesario.
}

void Arena::prepararSpawns(sf::Color colorAtacante, sf::Color colorDefensor) {
    float radioProteccion = 30.f;

    muros.clear();
    sombrasMuros.clear();

    // Definición lógica de las zonas de spawn
    zonaSpawnLuz = sf::FloatRect(150.f - radioProteccion, 300.f - radioProteccion, radioProteccion * 2, radioProteccion * 2);
    zonaSpawnOscuridad = sf::FloatRect(650.f - radioProteccion, 300.f - radioProteccion, radioProteccion * 2, radioProteccion * 2);

    // Configuración de círculos decorativos
    circuloLuz.setRadius(radioProteccion);
    circuloLuz.setOrigin(radioProteccion, radioProteccion);
    circuloLuz.setPosition(150.f, 300.f);
    circuloLuz.setOutlineThickness(1.f);
    circuloLuz.setOutlineColor(colorAtacante);
    circuloLuz.setFillColor(sf::Color::Transparent);

    circuloOscuridad.setRadius(radioProteccion);
    circuloOscuridad.setOrigin(radioProteccion, radioProteccion);
    circuloOscuridad.setPosition(650.f, 300.f);
    circuloOscuridad.setOutlineThickness(1.f);
    circuloOscuridad.setOutlineColor(colorDefensor);
    circuloOscuridad.setFillColor(sf::Color::Transparent);

    // Inicializamos los muros perimetrales
    // Asegúrate de tener este vector en Arena.h: std::vector<sf::RectangleShape> sombrasMuros;

    // Paleta de colores profesional
    sf::Color colorRelleno(30, 32, 40);      // Gris azulado muy oscuro
    sf::Color colorContorno(80, 85, 95);    // Gris claro para el brillo de la arista
    sf::Color colorSombra(0, 0, 0, 120);    // Sombra negra semitransparente

    // 1. Configurar prototipo Horizontal
    sf::RectangleShape muroH(sf::Vector2f(ANCHO_MAPA + GROSOR_MURO * 2, GROSOR_MURO));
    muroH.setFillColor(colorRelleno);
    muroH.setOutlineThickness(-2.f); // Contorno hacia adentro para no alterar el tamaño
    muroH.setOutlineColor(colorContorno);

    // 2. Configurar prototipo Vertical
    sf::RectangleShape muroV(sf::Vector2f(GROSOR_MURO, ALTO_MAPA));
    muroV.setFillColor(colorRelleno);
    muroV.setOutlineThickness(-2.f);
    muroV.setOutlineColor(colorContorno);

    //  FUNCIÓN SOMBRA 
    auto agregarMuroConSombra = [&](sf::RectangleShape& forma, float x, float y) {
        forma.setPosition(x, y);

        // Crear sombra desplazada
        sf::RectangleShape sombra = forma;
        sombra.setFillColor(colorSombra);
        sombra.setOutlineThickness(0); // La sombra no tiene brillo
        sombra.move(6.f, 6.f);         // Desplazamiento de la sombra proyectada

        sombrasMuros.push_back(sombra);
        muros.push_back(forma);
        };

    //  Posicionamiento de los 4 muros
    agregarMuroConSombra(muroH, -GROSOR_MURO, -GROSOR_MURO); // Arriba
    agregarMuroConSombra(muroH, -GROSOR_MURO, ALTO_MAPA);    // Abajo
    agregarMuroConSombra(muroV, -GROSOR_MURO, 0);            // Izquierda
    agregarMuroConSombra(muroV, ANCHO_MAPA, 0);              // Derecha
}

void Arena::generarMapaProcedural() {
    //  Limpieza total de vectores
    rocas.clear();
    sombras.clear();
    charcosSangre.clear();
    gotasSangre.clear();
    rejillasSuelo.clear();

    std::srand(static_cast<unsigned>(std::time(nullptr)));
    int intentos;

    // generar rejillas
    int numRejillas = std::rand() % 3 + 3;
    intentos = 100;
    while (rejillasSuelo.size() < numRejillas && intentos > 0) {
        intentos--;

        float px = std::rand() % (int)(ANCHO_MAPA - 150) + 75;
        float py = std::rand() % (int)(ALTO_MAPA - 150) + 75;

        // Centro y radio aproximado para la rejilla (60x40px)
        sf::Vector2f centro(px + 30.f, py + 20.f);
        float radioAprox = 45.f; // Radio de seguridad

        if (esGeneracionValida(centro, radioAprox, TipoObjeto::Rejilla)) {
            Rejilla r;
            r.marco.setSize({ 60.f, 40.f });
            r.marco.setPosition(px, py);
            r.marco.setFillColor(sf::Color(50, 50, 55));
            r.marco.setOutlineThickness(1.f);
            r.marco.setOutlineColor(sf::Color(30, 30, 35));

            int numBarras = 4;
            for (int j = 0; j < numBarras; ++j) {
                sf::RectangleShape barra(sf::Vector2f(50.f, 2.0f));
                barra.setFillColor(sf::Color(25, 25, 30));
                barra.setPosition(px + 5.f, py + (j + 1) * (40.f / (numBarras + 1)));
                r.barras.push_back(barra);
            }
            rejillasSuelo.push_back(r);
        }
    }

    // Generar rocas
    int numRocasDeseadas = std::rand() % 3 + 4;
    intentos = 200;
    while (rocas.size() < numRocasDeseadas && intentos > 0) {
        intentos--;

        float px = std::rand() % (int)(ANCHO_MAPA - 200) + 100;
        float py = std::rand() % (int)(ALTO_MAPA - 200) + 100;
        float radioBase = 30.f;

        // Comprobamos contra spawns, rejillas y otras rocas
        if (esGeneracionValida({ px, py }, radioBase + 10.f, TipoObjeto::Roca)) {
            sf::ConvexShape roca;
            int puntos = 15;
            roca.setPointCount(puntos);
            for (int i = 0; i < puntos; ++i) {
                float angulo = i * 2 * 3.14159f / puntos;
                float r = radioBase + std::rand() % 15;
                roca.setPoint(i, sf::Vector2f(r * std::cos(angulo), r * std::sin(angulo)));
            }

            roca.setPosition(px, py);
            roca.setFillColor(sf::Color(100, 100, 105));
            rocas.push_back(roca);

            sf::ConvexShape sombra = roca;
            sombra.setFillColor(sf::Color(0, 0, 0, 110));
            sombra.move(8.f, 12.f);
            sombras.push_back(sombra);
        }
    }

    // Genera charcos y gotas
    int numManchasDeseadas = 4;
    intentos = 150;
    while (charcosSangre.size() < numManchasDeseadas && intentos > 0) {
        intentos--;

        float px = static_cast<float>(std::rand() % (int)(ANCHO_MAPA - 150) + 75);
        float py = static_cast<float>(std::rand() % (int)(ALTO_MAPA - 100) + 50);
        float radioCharco = 30.f;

        // Para los charcos solo nos importa que no pisen el spawn
        if (esGeneracionValida({ px, py }, radioCharco, TipoObjeto::Decoracion)) {

            int rojoVariado = 130 + (std::rand() % 50);
            sf::Color colorSangre(rojoVariado, 0, 0, 160);

            sf::ConvexShape charco;
            int puntos = 15;
            charco.setPointCount(puntos);
            charco.setPosition(px, py);
            charco.setFillColor(colorSangre);

            float radioBase = static_cast<float>(std::rand() % 15 + 15);
            for (int i = 0; i < puntos; ++i) {
                float angulo = i * 2 * 3.14159f / puntos;
                float variacion = static_cast<float>(std::rand() % 4);
                if (i % 4 == 0) variacion += 2.0f;
                float r = radioBase + variacion;
                charco.setPoint(i, sf::Vector2f(r * std::cos(angulo), r * std::sin(angulo)));
            }

            charco.setScale(2.0f, 0.8f);
            charcosSangre.push_back(charco);

            for (int j = 0; j < 5; ++j) {
                sf::CircleShape gota(static_cast<float>(std::rand() % 3 + 2));
                gota.setFillColor(colorSangre);
                float offX = static_cast<float>(std::rand() % 150 - 50);
                float offY = static_cast<float>(std::rand() % 80 - 20);
                gota.setPosition(px + offX, py + offY);
                gotasSangre.push_back(gota);
            }
        }
    }
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

/**
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
//Mejrorarla, no se nota suficiente cambio en la arena
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