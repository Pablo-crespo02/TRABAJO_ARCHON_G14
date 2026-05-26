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
    textoTop.setPosition(ancho * 0.125f, 35.f);
    window.draw(textoTop);

    textoTop.setString("CICLO: " + std::to_string(ciclo));
    textoTop.setPosition(ancho * 0.25f, 35.f);
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
    textoTop.setPosition(ancho * 0.375f, 35.f);
    window.draw(textoTop);
    // --- PANEL LATERAL ---
    sf::RectangleShape panel({ anchoHUD, alto });
    panel.setPosition(inicioUI, 0);
    panel.setFillColor(sf::Color(18, 18, 22));
    window.draw(panel);

    float margenX = inicioUI + 25.f;
    float yActual = 60.f;

    sf::Text textoPanel;
    textoPanel.setFont(*fuente);
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
        sf::Text textoNombre;
        textoNombre.setFont(*fuente);
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

        // --- VALOR EN PUNTOS DE LA PIEZA SELECCIONADA ---
        int ptsValor = 0;
        std::string n = seleccionada->stats.nombre;
        if (n == "CAPTAIN" || n == "HIVE TYRANT") ptsValor = 2000;
        else if (n == "LIBRARIAN" || n == "HARPY") ptsValor = 750;
        else if (n == "CULEXUS" || n == "GENESTEALER") ptsValor = 750;
        else if (n == "ASSAULT MARINE" || n == "GARGOLA") ptsValor = 300;
        else if (n == "DREADNOUGHT" || n == "CARNIFEX") ptsValor = 300;
        else if (n == "PRIMARIS" || n == "TOXICRENO") ptsValor = 300;
        else if (n == "VINDICARE" || n == "LICTOR") ptsValor = 150;
        else if (n == "INTERCESSOR" || n == "TERMAGANT") ptsValor = 100;

        dibujarDato(window, "VALOR PTS:", std::to_string(ptsValor), margenX, yActual, sf::Color(150, 255, 150));


        // Patrón de movimiento
        std::string pTxt = "";
        switch (seleccionada->patronMovimiento) {
        case PatronMovimiento::Ambos:     pTxt = "DIAGONAL Y ORTOGONAL (*)"; break;
        case PatronMovimiento::Ortogonal: pTxt = "ORTOGONAL (+)";             break;
        case PatronMovimiento::Diagonal:  pTxt = "DIAGONAL (X)";              break;
        default:                          pTxt = "OTRO";                      break;
        }
        dibujarDato(window, "PATRON:", pTxt, margenX, yActual, sf::Color(255, 255, 150));

        //Hechizos

        // 1. Contador de hechizos restantes
        int restantes = seleccionada->getHechizosRestantes();
        sf::Color colHechizo = (restantes > 0) ? sf::Color(100, 200, 255) : sf::Color::Red;

        dibujarDato(window, "HECHIZOS:", std::to_string(restantes) + " / 3", margenX, yActual, colHechizo);

        // 2. Descripción del hechizo
        // Nota: Si quieres que el texto sea pequeño, ajusta la lógica de dibujarDato
        // o crea una llamada específica para texto largo.
        dibujarDato(window, "EFECTO:", seleccionada->getDescripcionHechizo(), margenX, yActual, sf::Color(200, 200, 200));
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