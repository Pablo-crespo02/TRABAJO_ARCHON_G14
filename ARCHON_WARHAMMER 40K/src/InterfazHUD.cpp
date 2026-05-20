#include "InterfazHUD.h"
#include "Color.h"
#include <string>

InterfazHUD::InterfazHUD(sf::RenderWindow& win, sf::Font& font) {
    this->window = &win;
    this->fuente = &font;
}

bool InterfazHUD::cargarFuente(const std::string& ruta) {
    return fuente->loadFromFile(ruta);
}

void InterfazHUD::dibujar(sf::RenderWindow& window, int ronda, int ciclo, int jugadorActual, Pieza* seleccionada) {
    float ancho = (float)window.getSize().x;
    float alto = (float)window.getSize().y;
    float inicioUI = ancho * 0.66f;
    float anchoHUD = ancho - inicioUI;

    // 1.CABECERA (RONDA, CICLO, TURNO) ---
    sf::Text textoTop;
    textoTop.setFont(*fuente);
    textoTop.setOutlineThickness(2);
    textoTop.setOutlineColor(sf::Color::Black);

    // Ronda y Ciclo
    textoTop.setCharacterSize(35);
    textoTop.setFillColor(sf::Color(180, 180, 180));
    textoTop.setString("RONDA: " + std::to_string(ronda));
    textoTop.setPosition(ancho * 0.10f, 35.f);
    window.draw(textoTop);

    textoTop.setString("CICLO: " + std::to_string(ciclo));
    textoTop.setPosition(ancho * 0.30f, 35.f);
    window.draw(textoTop);

    // Turno del jugador
    if (jugadorActual == 1) {
        textoTop.setFillColor(Colores::ColorFichaLuz);
        textoTop.setString("TURNO: IMPERIUM");
    }
    else {
        textoTop.setFillColor(Colores::ColorFichaOscuridad);
        textoTop.setString("TURNO: XENOS");
    }
    textoTop.setPosition(ancho * 0.50f, 35.f);
    window.draw(textoTop);

    //  2. PANEL DE INFORMACIÓN DE PIEZA (DERECHA) ---
    if (seleccionada != nullptr) {
        float margenX = inicioUI + 30.f;
        float yActual = 150.f;

        // Nombre de la Unidad
        sf::Text txtNombre;
        txtNombre.setFont(*fuente);
        txtNombre.setString(seleccionada->stats.nombre);
        txtNombre.setCharacterSize(40);
        txtNombre.setFillColor(seleccionada->getBando() == Bando::LUZ ? Colores::ColorFichaLuz : Colores::ColorFichaOscuridad);
        txtNombre.setOutlineThickness(2);
        txtNombre.setOutlineColor(sf::Color::Black);
        txtNombre.setPosition(margenX, yActual);
        window.draw(txtNombre);
        yActual += 60.f;

        // Atributos Base
        dibujarDato(window, "VIDA:", std::to_string((int)seleccionada->stats.vida) + " / " + std::to_string((int)seleccionada->stats.vidaMaxima), margenX, yActual, sf::Color::Green);
        dibujarDato(window, "ATAQUE:", std::to_string((int)seleccionada->stats.ataque), margenX, yActual, sf::Color::Red);
        dibujarDato(window, "DEFENSA:", std::to_string((int)seleccionada->stats.defensa), margenX, yActual, sf::Color::Blue);

        // Tipo de Combate (Rango o Melee)
        sf::Color colCom = seleccionada->stats.esRango ? sf::Color(255, 215, 0) : sf::Color(255, 80, 80);
        dibujarDato(window, "COMBATE:", seleccionada->stats.esRango ? "DISTANCIA" : "MELEE", margenX, yActual, colCom);

        // --- VALOR EN PUNTOS DE LA PIEZA SELECCIONADA ---
        int ptsValor = 0;
        std::string n = seleccionada->stats.nombre;
        if (n == "CAPTAIN" || n == "HIVE_TYRANT") ptsValor = 2000;
        else if (n == "LIBRARIAN" || n == "HARPY") ptsValor = 750;
        else if (n == "CULEXUS" || n == "GENESTEALER") ptsValor = 750;
        else if (n == "ASSAULT_MARINE" || n == "GARGOLA") ptsValor = 300;
        else if (n == "DREADNOUGHT" || n == "CARNIFEX") ptsValor = 300;
        else if (n == "PRIMARIS" || n == "TOXICRENO") ptsValor = 300;
        else if (n == "VINDICARE" || n == "LICTOR") ptsValor = 150;
        else if (n == "INTERCESSOR" || n == "TERMAGANT") ptsValor = 100;

        dibujarDato(window, "VALOR PTS:", std::to_string(ptsValor), margenX, yActual, sf::Color(150, 255, 150));

        // Velocidad de Ataque
        dibujarDato(window, "VEL. ATQ:", std::to_string((int)seleccionada->stats.velAtaque), margenX, yActual, sf::Color::White);

        // Patrón de movimiento
        std::string pTxt = "";
        switch (seleccionada->patronMovimiento) {
        case PatronMovimiento::Ambos:     pTxt = "DIAGONAL Y ORTOGONAL (*)"; break;
        case PatronMovimiento::Ortogonal: pTxt = "ORTOGONAL (+)";             break;
        case PatronMovimiento::Diagonal:  pTxt = "DIAGONAL (X)";              break;
        default:                          pTxt = "OTRO";                      break;
        }
        dibujarDato(window, "PATRON:", pTxt, margenX, yActual, sf::Color(255, 255, 150));
    }

    // 3.MARCADOR DE PUNTOS EN EL TABLERO (ABAJO A LA IZQUIERDA) 
    sf::Text textoPuntosHUD;
    textoPuntosHUD.setFont(*fuente);
    textoPuntosHUD.setCharacterSize(28);
    textoPuntosHUD.setFillColor(sf::Color::White);
    textoPuntosHUD.setOutlineColor(sf::Color::Black);
    textoPuntosHUD.setOutlineThickness(2.f);

    // Utilizamos los atributos internos de la clase que seteamos previamente
    std::string strPuntos = "PUNTOS IMPERIUM: " + std::to_string(this->puntosLuz) + "\n" +
        "PUNTOS XENOS:    " + std::to_string(this->puntosOscuridad);
    textoPuntosHUD.setString(strPuntos);

    textoPuntosHUD.setPosition(40.f, alto - 90.f);
    window.draw(textoPuntosHUD);

    //4.CRONOMETRO DE LA PARTIDA 
    sf::Text textoTiempoHUD;
    textoTiempoHUD.setFont(*fuente);
    textoTiempoHUD.setCharacterSize(28);
    textoTiempoHUD.setFillColor(sf::Color::Cyan);
    textoTiempoHUD.setOutlineColor(sf::Color::Black);
    textoTiempoHUD.setOutlineThickness(2.f);

    int minutos = static_cast<int>(this->tiempoJugado) / 60;
    int segundos = static_cast<int>(this->tiempoJugado) % 60;

    std::string strSegundos = (segundos < 10 ? "0" : "") + std::to_string(segundos);
    textoTiempoHUD.setString("TIEMPO: " + std::to_string(minutos) + ":" + strSegundos);

    textoTiempoHUD.setPosition(450.f, alto - 75.f);
    window.draw(textoTiempoHUD);
}

void InterfazHUD::dibujarDato(sf::RenderWindow& window, std::string etiqueta, std::string valor, float x, float& yActual, sf::Color colorVal) {
    sf::Text t;
    t.setFont(*fuente);
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