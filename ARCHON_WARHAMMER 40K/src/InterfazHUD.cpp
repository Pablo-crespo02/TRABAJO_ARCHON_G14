#include "InterfazHUD.h"
#include <string>

InterfazHUD::InterfazHUD(sf::RenderWindow& win, sf::Font& font) {
    this->window = &win;
    this->fuente = font;

    // Aquí puedes configurar tus textos
    // ejemploTexto.setFont(this->fuente);
    // ejemploTexto.setString("Vida: 100");
}

bool InterfazHUD::cargarFuente(const std::string& ruta) {
    return fuente.loadFromFile(ruta);
}

void InterfazHUD::dibujar(sf::RenderWindow& window, int ronda, int ciclo, int jugadorActual, Pieza* seleccionada) {
    float ancho = (float)window.getSize().x;
    float alto = (float)window.getSize().y;
    float inicioUI = ancho * 0.66f;
    float anchoHUD = ancho - inicioUI;

    // --- 1. CABECERA (RONDA, CICLO, TURNO) ---
    sf::Text textoTop;
    textoTop.setFont(fuente);
    textoTop.setOutlineThickness(2);
    textoTop.setOutlineColor(sf::Color::Black);

    // Ronda y Ciclo
    textoTop.setCharacterSize(35);
    textoTop.setFillColor(sf::Color(180, 180, 180));
    textoTop.setString("RONDA: " + std::to_string(ronda));
    textoTop.setPosition(ancho * 0.10f, 35.f);
    window.draw(textoTop);

    textoTop.setString("CICLO: " + std::to_string(ciclo) + " / 12");
    textoTop.setPosition(ancho * 0.10f, 80.f);
    window.draw(textoTop);

    // Turno Actual
    std::string nombreTurno = (jugadorActual == 1) ? "TURNO: IMPERIUM (LUZ)" : "TURNO: TIRANIDOS (OSCURIDAD)";
    sf::Color colorTurno = (jugadorActual == 1) ? sf::Color(255, 255, 150) : sf::Color(180, 100, 255);
    textoTop.setCharacterSize(40);
    textoTop.setFillColor(colorTurno);
    textoTop.setString(nombreTurno);
    textoTop.setPosition((inicioUI / 2.f) - 150.f, 35.f);
    window.draw(textoTop);

    // --- PANEL LATERAL ---
    sf::RectangleShape panel({ anchoHUD, alto });
    panel.setPosition(inicioUI, 0);
    panel.setFillColor(sf::Color(18, 18, 22));
    window.draw(panel);

    float margenX = inicioUI + 25.f;
    float yActual = 60.f;

    sf::Text textoPanel;
    textoPanel.setFont(fuente);
    textoPanel.setCharacterSize(35);
    textoPanel.setFillColor(sf::Color(120, 120, 130));
    textoPanel.setString("UNIDAD");
    textoPanel.setPosition(margenX, yActual);
    window.draw(textoPanel);

    yActual += 70.f;

    // Marcos de imagen
    float anchoMarco = (anchoHUD - 70.f) / 2.f;
    sf::RectangleShape marco({ anchoMarco, 320.f });
    marco.setOutlineThickness(3);
    marco.setOutlineColor(sf::Color(80, 80, 90));
    marco.setFillColor(sf::Color(25, 25, 30));

    marco.setPosition(margenX, yActual);
    window.draw(marco);
    marco.setPosition(margenX + anchoMarco + 20.f, yActual);
    window.draw(marco);

    yActual += 360.f;

    // --- DATOS DE LA PIEZA ---
    if (seleccionada != nullptr) {
        // --- TÍTULO DE LA UNIDAD (Estilo THUNDERHAWK) ---
        sf::Text textoNombre;
        textoNombre.setFont(fuente);
        textoNombre.setCharacterSize(55); // Aumentado para que destaque como en la foto
        textoNombre.setFillColor(sf::Color::White);
        textoNombre.setString(seleccionada->stats.nombre);

        // Posicionamiento tras los marcos de imagen
        // yActual debería estar aproximadamente en 450.f - 480.f aquí
        textoNombre.setPosition(margenX, yActual);
        window.draw(textoNombre);

        // Dejamos un margen generoso después del nombre
        yActual += 90.f;

        // LISTA DE DATOS 

        // Vida y Ataque
        dibujarDato(window, "VIDA:", std::to_string((int)seleccionada->stats.vida), margenX, yActual, sf::Color(100, 255, 100));
        dibujarDato(window, "ATAQUE:", std::to_string((int)seleccionada->stats.ataque), margenX, yActual, sf::Color(255, 120, 120));

        // Rango Mov
        dibujarDato(window, "RANGO MOV:", std::to_string(seleccionada->rangoMovimiento) + " CASILLAS", margenX, yActual, sf::Color::White);

        // Movimiento (Lógica de colores)
        std::string txtMov = "TERRESTRE";
        sf::Color colMov = sf::Color(200, 150, 100);
        if (seleccionada->tipoMov == TipoMovimiento::Volador) {
            txtMov = "VOLADOR";
            colMov = sf::Color(100, 200, 255);
        }
        else if (seleccionada->tipoMov == TipoMovimiento::Teletransporte) {
            txtMov = "TELETRANSPORTE";
            colMov = sf::Color(255, 100, 255);
        }
        dibujarDato(window, "MOVIMIENTO:", txtMov, margenX, yActual, colMov);

        // Combate
        sf::Color colCom = seleccionada->stats.esRango ? sf::Color(255, 215, 0) : sf::Color(255, 80, 80);
        dibujarDato(window, "COMBATE:", seleccionada->stats.esRango ? "DISTANCIA" : "MELEE", margenX, yActual, colCom);

        // Velocidad de Ataque
        dibujarDato(window, "VEL. ATQ:", std::to_string((int)seleccionada->stats.velAtaque), margenX, yActual, sf::Color::White);

        // Patrón (Con el color amarillo pálido de tu código)
        std::string pTxt = "";
        switch (seleccionada->patronMovimiento) {
        case PatronMovimiento::Ambos:     pTxt = "DIAGONAL Y ORTOGONAL (*)"; break;
        case PatronMovimiento::Ortogonal: pTxt = "ORTOGONAL (+)";             break;
        case PatronMovimiento::Diagonal:  pTxt = "DIAGONAL (X)";              break;
        default:                          pTxt = "OTRO";                      break;
        }
        dibujarDato(window, "PATRON:", pTxt, margenX, yActual, sf::Color(255, 255, 150));
    }
}

void InterfazHUD::dibujarDato(sf::RenderWindow& window, std::string etiqueta, std::string valor, float x, float& yActual, sf::Color colorVal) {
    sf::Text t;
    t.setFont(fuente);
    t.setCharacterSize(18);
    t.setFillColor(sf::Color(140, 140, 150));
    t.setString(etiqueta);
    t.setPosition(x, yActual);
    window.draw(t);

    t.setCharacterSize(28);
    t.setFillColor(colorVal);
    t.setString(valor);
    t.setPosition(x + 210.f, yActual - 5.f);
    window.draw(t);

    yActual += 48.f;
}