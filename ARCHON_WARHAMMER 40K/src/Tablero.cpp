#include "Tablero.h"
#include <SFML/Graphics.hpp>
Tablero::Tablero() {
    int mapa[9][9] = {
        {1, 0, 1, 2, 3, 2, 0, 1, 0},
        {0, 1, 2, 0, 2, 1, 2, 0, 1},
        {1, 2, 0, 1, 2, 0, 1, 2, 0},
        {2, 0, 1, 0, 2, 1, 0, 1, 2},
        {3, 2, 2, 2, 3, 2, 2, 2, 3},
        {2, 0, 1, 0, 2, 1, 0, 1, 2},
        {1, 2, 0, 1, 2, 0, 1, 2, 0},
        {0, 1, 2, 0, 2, 1, 2, 0, 1},
        {1, 0, 1, 2, 3, 2, 0, 1, 0}
    };

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            // Asignamos el tipo según el mapa
            switch (mapa[i][j]) {
            case 0: matriz[i][j].tipo = TipoCasilla::PERMANENTE_LUZ;      break;
            case 1: matriz[i][j].tipo = TipoCasilla::PERMANENTE_OSCURIDAD; break;
            case 2: matriz[i][j].tipo = TipoCasilla::CAMBIANTE;           break;
            case 3: matriz[i][j].tipo = TipoCasilla::POWER_POINT;         break;
            }

            // Colores iniciales por defecto
            if (matriz[i][j].tipo == TipoCasilla::PERMANENTE_OSCURIDAD)
                matriz[i][j].colorVisual = ColorActual::NEGRO_PICO;
            else
                matriz[i][j].colorVisual = ColorActual::BLANCO_PICO;
        }
    }
}

void Tablero::actualizarColores(int numCiclo) {
    // Definimos la secuencia de colores (los 12 estados)
    ColorActual secuencia[] = {
        ColorActual::BLANCO_PICO, ColorActual::BLANCO, ColorActual::GRIS_CLARO,
        ColorActual::GRIS_MEDIO,  ColorActual::GRIS_OSCURO, ColorActual::NEGRO,
        ColorActual::NEGRO_PICO,  ColorActual::NEGRO, ColorActual::GRIS_OSCURO,
        ColorActual::GRIS_MEDIO,  ColorActual::GRIS_CLARO, ColorActual::BLANCO
    };

    // Obtenemos el color correspondiente (ajustando de 1-12 a índice 0-11)
    ColorActual colorDelCiclo = secuencia[numCiclo - 1];

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            // Actualizamos solo las casillas que NO son permanentes
            if (matriz[i][j].tipo == TipoCasilla::CAMBIANTE ||
                matriz[i][j].tipo == TipoCasilla::POWER_POINT) {

                matriz[i][j].colorVisual = colorDelCiclo;
            }
        }
    }
}

// ... (Función dibujar() con el rombo de los PowerPoints igual que antes) ...

void Tablero::dibujar(sf::RenderWindow& window) {
    const float tamCasilla = 60.0f;
    const float margen = 2.0f;

    // 1. Base de la casilla (el cuadrado de color)
    sf::RectangleShape cuadro(sf::Vector2f(tamCasilla - margen, tamCasilla - margen));

    // --- NUEVO: Forma característica para los PowerPoints ---
    // Vamos a usar un rombo (un cuadrado girado 45 grados)
    sf::RectangleShape glifoPower(sf::Vector2f(20.0f, 20.0f)); // Tamaño del símbolo central
    glifoPower.setOrigin(10.0f, 10.0f); // Ponemos el origen en su centro para girarlo bien
    glifoPower.setRotation(45.0f); // Giramos 45 grados para que parezca un rombo
    glifoPower.setFillColor(sf::Color(255, 215, 0)); // Color Oro (Gold) para que resalte
    glifoPower.setOutlineThickness(1.5f);
    glifoPower.setOutlineColor(sf::Color::Black); // Borde negro para que se vea sobre fondo blanco
    // ---------------------------------------------------------

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {

            // --- A) Lógica de Color (igual que antes) ---
            sf::Color colorSF;
            switch (matriz[i][j].colorVisual) {
            case ColorActual::BLANCO_PICO: colorSF = sf::Color(255, 255, 255); break;
            case ColorActual::BLANCO:      colorSF = sf::Color(220, 220, 220); break;
            case ColorActual::GRIS_CLARO:  colorSF = sf::Color(180, 180, 180); break;
            case ColorActual::GRIS_MEDIO:  colorSF = sf::Color(120, 120, 120); break;
            case ColorActual::GRIS_OSCURO: colorSF = sf::Color(60, 60, 60);    break;
            case ColorActual::NEGRO:       colorSF = sf::Color(30, 30, 30);    break;
            case ColorActual::NEGRO_PICO:  colorSF = sf::Color(0, 0, 0);       break;
            default:                       colorSF = sf::Color::Magenta;       break; // Error visual
            }

            // --- B) Dibujar la Base ---
            cuadro.setFillColor(colorSF);
            float posX = static_cast<float>(j) * tamCasilla + 50.0f;
            float posY = static_cast<float>(i) * tamCasilla + 30.0f;
            cuadro.setPosition(posX, posY);

            // Dibujamos primero el fondo de la casilla
            window.draw(cuadro);

            // --- C) NUEVO: Dibujar el Símbolo si es PowerPoint ---
            if (matriz[i][j].tipo == TipoCasilla::POWER_POINT) {
                // Calculamos el centro exacto de la casilla actual
                float centroX = posX + (tamCasilla - margen) / 2.0f;
                float centroY = posY + (tamCasilla - margen) / 2.0f;

                // Posicionamos el glifo en el centro
                glifoPower.setPosition(centroX, centroY);

                // Lo dibujamos ENCIMA del cuadrado
                window.draw(glifoPower);
            }
        }
    }
}
