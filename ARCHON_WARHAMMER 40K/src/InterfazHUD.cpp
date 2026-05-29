#include "InterfazHUD.h"
#include "Color.h"
#include <string>

InterfazHUD::InterfazHUD(sf::RenderWindow& win, sf::Font& font) {
    this->window = &win;
    this->fuente = &font;
    cargarTexturas();
}

void InterfazHUD::cargarTexturas()  {

        mapaTexturas["IMP_LOGO"].loadFromFile("imagenes/logo_ultramarines.png");    
        mapaTexturas["XEN_LOGO"].loadFromFile("imagenes/logo_tiranidos.png");
        
        mapaTexturas["CAPTAIN"].loadFromFile("imagenes/RETRATO_CAPTAIN.png");
        mapaTexturas["LIBRARIAN"].loadFromFile("imagenes/RETRATO_LIBRARIAN.png");
        mapaTexturas["CULEXUS"].loadFromFile("imagenes/RETRATO_CULEXUS.png");
        mapaTexturas["ASSAULT MARINE"].loadFromFile("imagenes/RETRATO_ASSAULT_MARINE.png");
        mapaTexturas["DREADNOUGHT"].loadFromFile("imagenes/RETRATO_DREADNOUGHT.png");
        mapaTexturas["PRIMARIS"].loadFromFile("imagenes/RETRATO_PRIMARIS.png");
        mapaTexturas["VINDICARE"].loadFromFile("imagenes/RETRATO_VINDICARE.png");
        mapaTexturas["INTERCESSOR"].loadFromFile("imagenes/RETRATO_INTERCESSOR.png");

        mapaTexturas["HIVE TYRANT"].loadFromFile("imagenes/RETRATO_HIVE_TYRANT.png");
        mapaTexturas["HARPY"].loadFromFile("imagenes/RETRATO_HARPY.png");
        mapaTexturas["GENESTEALER"].loadFromFile("imagenes/RETRATO_GENESTEALER.png");
        mapaTexturas["CARNIFEX"].loadFromFile("imagenes/RETRATO_CARNIFEX.png");
        mapaTexturas["TOXICRENO"].loadFromFile("imagenes/RETRATO_TOXICRENO.png");
        mapaTexturas["GARGOLA"].loadFromFile("imagenes/RETRATO_GARGOLA.png"); 
        mapaTexturas["LICTOR"].loadFromFile("imagenes/RETRATO_LICTOR.png");
        mapaTexturas["TERMAGANT"].loadFromFile("imagenes/RETRATO_TERMAGANT.png");
    }


void InterfazHUD::dibujar(sf::RenderWindow& window, int ronda, int ciclo, int jugadorActual, Pieza* seleccionada) {
    float ancho = (float)window.getSize().x;
    float alto = (float)window.getSize().y;
    float inicioUI = ancho * 0.66f;
    float anchoHUD = ancho - inicioUI;

    // Cabecera
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

    // Turno
    textoTop.setFillColor(jugadorActual == 1 ? Colores::ColorFichaLuz : Colores::ColorFichaOscuridad);
    textoTop.setString(jugadorActual == 1 ? "TURNO: IMPERIUM" : "TURNO: XENOS");
    textoTop.setPosition(ancho * 0.375f, 35.f);
    window.draw(textoTop);

    // Panel lateral
    sf::RectangleShape panel({ anchoHUD, alto });
    panel.setPosition(inicioUI, 0);
    panel.setFillColor(sf::Color(18, 18, 22));
    window.draw(panel);

    float margenX = inicioUI + 25.f;
    float yActual = 60.f;

    // Titulo Unidad
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
    sf::RectangleShape marco({ anchoMarco, 290.f });
    marco.setOutlineThickness(3);
    marco.setOutlineColor(sf::Color(80, 80, 90));
    marco.setFillColor(sf::Color(25, 25, 30));

    marco.setPosition(margenX, yActual);
    window.draw(marco);
    marco.setPosition(margenX + anchoMarco + 20.f, yActual);
    window.draw(marco);

    // Dibujo de iconos (Solo si hay selección)
    if (seleccionada != nullptr) {
       

        std::string claveBando = (seleccionada->getBando() == Bando::LUZ ? "IMP_LOGO" : "XEN_LOGO");
        //Dibujo bando (derecha)
        dibujarDesdeMapa(window, claveBando, { margenX + anchoMarco + 20.f, yActual }, { anchoMarco, 290.f });
        //Dibujo Unidad (izq)
        dibujarDesdeMapa(window, seleccionada->stats.nombre,{ margenX, yActual }, { anchoMarco, 290.f });

    
         yActual += 310.f;

        // --- 4. DATOS DE LA PIEZA ---
        sf::Text textoNombre;
        textoNombre.setFont(*fuente);
        textoNombre.setCharacterSize(55);
        textoNombre.setFillColor(sf::Color::White);
        textoNombre.setString(seleccionada->stats.nombre);
        textoNombre.setPosition(margenX, yActual);
        window.draw(textoNombre);
        yActual += 90.f;

        // Lista de datos:

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

        // Valor de los puntos de cada pieza:
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

    // 3. Marcador de puntos en el tablero:
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

    //4.Cronómetro:
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

    //CONTADOR DE TURNO
    float tiempoRestante = limiteTurno - tiempoTurno;
    if (tiempoRestante < 0.f) tiempoRestante = 0.f;

    // Color según urgencia
    sf::Color colorTiempo;
    if (tiempoRestante > 15.f)     colorTiempo = sf::Color::Green;
    else if (tiempoRestante > 7.f) colorTiempo = sf::Color::Yellow;
    else                           colorTiempo = sf::Color::Red;

    // Número de segundos restantes
    sf::Text textoTurno;
    textoTurno.setFont(*fuente);
    textoTurno.setCharacterSize(40);
    textoTurno.setFillColor(colorTiempo);
    textoTurno.setOutlineColor(sf::Color::Black);
    textoTurno.setOutlineThickness(2.f);
    textoTurno.setString("TURNO: " + std::to_string((int)tiempoRestante) + "s");
    textoTurno.setPosition(450.f, alto - 120.f);
    window.draw(textoTurno);

    // Barra visual debajo del número
    float anchoBarraTurno = 200.f;
    float porcentaje = tiempoRestante / limiteTurno;

    sf::RectangleShape fondoBarra(sf::Vector2f(anchoBarraTurno, 10.f));
    fondoBarra.setFillColor(sf::Color(50, 50, 50));
    fondoBarra.setPosition(450.f, alto - 75.f);
    window.draw(fondoBarra);

    sf::RectangleShape barraTurno(sf::Vector2f(anchoBarraTurno * porcentaje, 10.f));
    barraTurno.setFillColor(colorTiempo);
    barraTurno.setPosition(450.f, alto - 75.f);
    window.draw(barraTurno);
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
void InterfazHUD::dibujarDesdeMapa(sf::RenderWindow& window, std::string nombre, sf::Vector2f pos, sf::Vector2f tamano) {
    // Verificamos si la textura existe en el mapa para no romper el juego
    if (mapaTexturas.find(nombre) != mapaTexturas.end()) {
        sf::Sprite sprite;
        sprite.setTexture(mapaTexturas[nombre]);

        // Escalar para que ajuste al marco
        float escalaX = tamano.x / sprite.getLocalBounds().width;
        float escalaY = tamano.y / sprite.getLocalBounds().height;
        sprite.setScale(escalaX, escalaY);

        sprite.setPosition(pos);
        window.draw(sprite);
    }
    else {
        // Opcional: imprimir error si no encuentra la textura
        // std::cout << "Textura no encontrada: " << nombre << std::endl;
    }
}
