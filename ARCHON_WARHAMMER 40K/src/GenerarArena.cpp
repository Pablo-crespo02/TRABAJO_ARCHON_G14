#include "GenerarArena.h"
#include "Arena.h"
#include <ctime>
#include <cmath>

void GeneradorArena::generarMapa(Arena& arena, sf::Color colorLuz, sf::Color colorOsc) {
    // 1. Limpieza total a través de la arena
    arena.limpiarTodo();

    // 2. Levantamos la infraestructura (Spawns y Muros)
    prepararSpawns(arena, colorLuz, colorOsc);

    // 3. Inicializamos semilla y generamos elementos procedurales
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    generarRejillas(arena);
    generarRocas(arena);
    generarSangre(arena);
}

bool GeneradorArena::esPosicionValida(sf::Vector2f pos, float radio, const Arena& arena) {
    // REGLA 1: No pisar los SpawnPoints (usando el método de la clase SpawnPoint)
    for (const auto& sp : arena.getSpawns()) {
        if (sp.contienePunto(pos, radio + 10.f)) {
            return false;
        }
    }

    // REGLA 2: No pisar otras rocas ya existentes
    for (const auto& roca : arena.getRocas()) {
        sf::Vector2f diff = pos - roca.getPosition();
        float distSq = diff.x * diff.x + diff.y * diff.y;
        // Radio de seguridad para evitar solapamiento entre rocas
        if (distSq < (radio + 40.f) * (radio + 40.f)) return false;
    }

    return true;
}

void GeneradorArena::generarRocas(Arena& arena) {
    int numRocas = std::rand() % 3 + 4;
    int intentos = 200;

    while (numRocas > 0 && intentos > 0) {
        intentos--;
        float px = static_cast<float>(std::rand() % (int)(ANCHO_MAPA - 200) + 100);
        float py = static_cast<float>(std::rand() % (int)(ALTO_MAPA - 200) + 100);
        float radioBase = 30.f;

        if (esPosicionValida({ px, py }, radioBase, arena)) {
            sf::ConvexShape roca;
            int puntos = 15;
            roca.setPointCount(puntos);

            for (int i = 0; i < puntos; ++i) {
                float angulo = i * 2 * 3.14159f / puntos;
                float r = radioBase + (std::rand() % 15);
                roca.setPoint(i, sf::Vector2f(r * std::cos(angulo), r * std::sin(angulo)));
            }

            roca.setPosition(px, py);
            roca.setFillColor(sf::Color(100, 100, 105));

            // Creamos la sombra
            sf::ConvexShape sombra = roca;
            sombra.setFillColor(sf::Color(0, 0, 0, 110));
            sombra.move(8.f, 12.f);

            // Inyectamos en la arena
            arena.addRoca(roca, sombra);
            numRocas--;
        }
    }
}

void GeneradorArena::generarRejillas(Arena& arena) {
    int numRejillas = std::rand() % 3 + 3;
    int intentos = 100;

    while (numRejillas > 0 && intentos > 0) {
        intentos--;
        float px = static_cast<float>(std::rand() % (int)(ANCHO_MAPA - 150) + 75);
        float py = static_cast<float>(std::rand() % (int)(ALTO_MAPA - 150) + 75);

        sf::Vector2f centro(px + 30.f, py + 20.f);
        float radioAprox = 45.f;

        if (esPosicionValida(centro, radioAprox, arena)) {
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

            arena.addRejilla(r);
            numRejillas--;
        }
    }
}

void GeneradorArena::generarSangre(Arena& arena) {
    int numManchasDeseadas = 4;
    int intentos = 150;

    while (numManchasDeseadas > 0 && intentos > 0) {
        intentos--;
        float px = static_cast<float>(std::rand() % (int)(ANCHO_MAPA - 150) + 75);
        float py = static_cast<float>(std::rand() % (int)(ALTO_MAPA - 100) + 50);
        float radioCharco = 30.f;

        if (esPosicionValida({ px, py }, radioCharco, arena)) {
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
            arena.addCharcoSangre(charco);

            for (int j = 0; j < 5; ++j) {
                sf::CircleShape gota(static_cast<float>(std::rand() % 3 + 2));
                gota.setFillColor(colorSangre);
                float offX = static_cast<float>(std::rand() % 150 - 50);
                float offY = static_cast<float>(std::rand() % 80 - 20);
                gota.setPosition(px + offX, py + offY);
                arena.addGotaSangre(gota);
            }
            numManchasDeseadas--;
        }
    }
}

void GeneradorArena::prepararSpawns(Arena& arena, sf::Color colorLuz, sf::Color colorOscuridad) {
    // 1. Inyectamos los SpawnPoints en la Arena
    arena.addSpawnPoint(SpawnPoint(sf::Vector2f(150.f, 300.f), 30.f, colorLuz));
    arena.addSpawnPoint(SpawnPoint(sf::Vector2f(650.f, 300.f), 30.f, colorOscuridad));

    // 2. Colores de los muros
    sf::Color colorRelleno(30, 32, 40);
    sf::Color colorContorno(80, 85, 95);
    sf::Color colorSombra(0, 0, 0, 120);

    // 3. Prototipos de muros
    sf::RectangleShape muroH(sf::Vector2f(ANCHO_MAPA + GROSOR_MURO * 2, GROSOR_MURO));
    muroH.setFillColor(colorRelleno);
    muroH.setOutlineThickness(-2.f);
    muroH.setOutlineColor(colorContorno);

    sf::RectangleShape muroV(sf::Vector2f(GROSOR_MURO, ALTO_MAPA));
    muroV.setFillColor(colorRelleno);
    muroV.setOutlineThickness(-2.f);
    muroV.setOutlineColor(colorContorno);

    auto fabricarMuro = [&](sf::RectangleShape forma, float x, float y) {
        forma.setPosition(x, y);
        sf::RectangleShape sombra = forma;
        sombra.setFillColor(colorSombra);
        sombra.setOutlineThickness(0);
        sombra.move(6.f, 6.f);
        arena.addMuro(forma, sombra);
        };

    // 4. Posicionamiento final
    fabricarMuro(muroH, -GROSOR_MURO, -GROSOR_MURO); // Arriba
    fabricarMuro(muroH, -GROSOR_MURO, ALTO_MAPA);    // Abajo
    fabricarMuro(muroV, -GROSOR_MURO, 0);            // Izquierda
    fabricarMuro(muroV, ANCHO_MAPA, 0);              // Derecha
}