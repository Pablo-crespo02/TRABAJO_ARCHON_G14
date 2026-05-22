#include "MenusNoInteractivos.h"
#include <iostream>

void MenuNoInteractivo::inicializarTextos() {

    // Cargamos las fuentes y manejamos el error en caso contrario:
    if (!fuente.loadFromFile("fuentes/fuente_pixel.ttf")) {
        std::cout << "Error cargando fuente" << std::endl;
        return;
    }

    // Configuración de las instrucciones:
    textoInstrucciones.setFont(fuente);
    textoInstrucciones.setCharacterSize(35);
    textoInstrucciones.setFillColor(sf::Color::Yellow);
    textoInstrucciones.setOutlineColor(sf::Color::Black);
    textoInstrucciones.setOutlineThickness(4.0f);
    textoInstrucciones.setString(
        "         OBJETIVO DE LA CRUZADA\n"
        "Domina los 5 Nodos de Poder o aniquila al enemigo.\n\n"
        "         FASE ESTRATEGICA (Tablero)\n"
        "- Raton (Click Izquierdo) para mover unidades.\n\n"
        "         FASE DE COMBATE (Arena)\n"
        "- IMPERIUM: WASD para mover. ESPACIO dispara. Q Hechizo.\n"
        "- XENOS: FLECHAS para mover. ENTER dispara. M Hechizo.\n\n\n"
        "      (Pulsa ESC para volver al Menu Principal)"
    );

    // Configuración de los créditos:
    textoCreditos.setFont(fuente);
    textoCreditos.setCharacterSize(40);
    textoCreditos.setFillColor(sf::Color::White);
    textoCreditos.setOutlineColor(sf::Color::Black);
    textoCreditos.setOutlineThickness(3.0f);
    textoCreditos.setString(
        "           CREDITOS DE LA OBRA\n\n"
        "Desarrollado de forma integra por:\n"
        "            [Tu Nombre Aqui]\n\n"
        "Musica y Efectos: Warhammer 40K Fan Assets\n"
        "Libreria Grafica: SFML 2.6.x\n\n\n"
        "    (Pulsa ESC para volver al Menu Principal)"
    );

    // Asignamos la fuente a los textos de la pantalla de victoria
    textoVictoria.setFont(fuente);
    textoJugadorGanador.setFont(fuente);
    textoContinuar.setFont(fuente);
    textoTiempo.setFont(fuente);
    textoPuntuaciones.setFont(fuente);

    // Configuramos el texto de continuar estándar
    textoContinuar.setString("PULSA ENTER PARA VOLVER AL MENU");
    textoContinuar.setCharacterSize(30);
    textoContinuar.setFillColor(sf::Color::White);
    textoContinuar.setOutlineColor(sf::Color::Black);
    textoContinuar.setOutlineThickness(2.0f);
}

void MenuNoInteractivo::configurarPantallaVictoria(int ganador, int ptosLuz, int ptosOscuridad, float tiempoJugado, std::string nombreGanador, sf::RenderWindow& window) {
    
    //CARGA DE RECURSOS Y MENSAJE EN CASO DE ERROR, FUENTE:
    if (!fuente.loadFromFile("fuentes/fuente_pixel.ttf")) {
        std::cout << "Error cargando fuente" << std::endl;
        return;
    }

    //VICTORIA IMPERIUM O XENOS:
    switch (ganador) {
    case 1:

        //Carga de recursos y mensaje de error
        if (!texturaFondo.loadFromFile("imagenes/Pantalla_Victoria_Imperium.png")) {
            std::cout << "Error cargando la pantalla de Victoria LUZ" << std::endl;
            return;
        }

        spriteFondo.setTexture(texturaFondo, true);  //true garantiza el correcto escalado de la imagen
        textoVictoria.setString("VICTORIA DEL IMPERIUM");
        textoVictoria.setFillColor(sf::Color::Yellow);
        break;

    case 2:

        if (!texturaFondo.loadFromFile("imagenes/Pantalla_Victoria_Tiranidos.png")) {
            std::cout << "Error cargando la pantalla de Victoria OSCURIDAD" << std::endl;
            return;
        }

        spriteFondo.setTexture(texturaFondo, true);
        textoVictoria.setString("VICTORIA XENOS");
        textoVictoria.setFillColor(sf::Color::Red);
        break;

    default:
        std::cout << "Error de entrada a la función constrcutora de PantallaVictoria, entero distinto de 1-2";
        return;
    }
    //AJUSTE DE ESCALA DEL MONITOR:
    sf::Vector2u tamanoVentana = window.getSize();
    sf::Vector2u tamanoImg = spriteFondo.getTexture()->getSize();

    float escalaX = (float)tamanoVentana.x / tamanoImg.x;
    float escalaY = (float)tamanoVentana.y / tamanoImg.y;
    spriteFondo.setScale(escalaX, escalaY);

    //CONFIGURACIÓN DEL TÍTULO DE VICTORIA
    textoVictoria.setCharacterSize(65);
    textoVictoria.setOutlineColor(sf::Color::Black);
    textoVictoria.setOutlineThickness(4.0f);

    if (ganador == 1) {
        textoVictoria.setString("VICTORIA DEL IMPERIUM");
        textoVictoria.setFillColor(sf::Color::Yellow);
    }
    else {
        textoVictoria.setString("VICTORIA DE LOS XENOS");
        textoVictoria.setFillColor(sf::Color::Magenta);
    }

    sf::FloatRect textRect = textoVictoria.getLocalBounds();
    textoVictoria.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    textoVictoria.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 180.f);

    //CONFIGURACIÓN DEL TEXTO DEL JUGADOR GANADOR
    textoJugadorGanador.setCharacterSize(45);
    textoJugadorGanador.setOutlineColor(sf::Color::Black);
    textoJugadorGanador.setOutlineThickness(3.0f);
    textoJugadorGanador.setFillColor(sf::Color(150, 255, 150)); // Verde claro
    textoJugadorGanador.setString("Jugador ganador: " + nombreGanador);

    sf::FloatRect jgRect = textoJugadorGanador.getLocalBounds();
    textoJugadorGanador.setOrigin(jgRect.left + jgRect.width / 2.0f, jgRect.top + jgRect.height / 2.0f);
    textoJugadorGanador.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 90.f);

    //CONFIGURACIÓN DEL TEXTO DE PUNTUACIONES
    textoPuntuaciones.setCharacterSize(40);
    textoPuntuaciones.setOutlineColor(sf::Color::Black);
    textoPuntuaciones.setOutlineThickness(3.0f);
    textoPuntuaciones.setFillColor(sf::Color::White);

    std::string txtPuntos = "PUNTUACION IMPERIUM: " + std::to_string(ptosLuz) + "\n" +
        "PUNTUACION XENOS:    " + std::to_string(ptosOscuridad);
    textoPuntuaciones.setString(txtPuntos);

    sf::FloatRect ptRect = textoPuntuaciones.getLocalBounds();
    textoPuntuaciones.setOrigin(ptRect.left + ptRect.width / 2.0f, ptRect.top + ptRect.height / 2.0f);
    textoPuntuaciones.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f + 20.f);

    // CONFIGURACIÓN DEL TEXTO DEL TEMPORIZADOR
    textoTiempo.setCharacterSize(40);
    textoTiempo.setOutlineColor(sf::Color::Black);
    textoTiempo.setOutlineThickness(3.0f);
    textoTiempo.setFillColor(sf::Color::Cyan);

    int minutos = static_cast<int>(tiempoJugado) / 60;
    int segundos = static_cast<int>(tiempoJugado) % 60;
    std::string strSegundos = (segundos < 10 ? "0" : "") + std::to_string(segundos);
    textoTiempo.setString("TIEMPO DE PARTIDA: " + std::to_string(minutos) + ":" + strSegundos);

    sf::FloatRect tiemRect = textoTiempo.getLocalBounds();
    textoTiempo.setOrigin(tiemRect.left + tiemRect.width / 2.0f, tiemRect.top + tiemRect.height / 2.0f);
    textoTiempo.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f + 120.f);

    //TEXTO PARA CONTINUAR/VOLVER AL MENÚ
    sf::FloatRect contRect = textoContinuar.getLocalBounds();
    textoContinuar.setOrigin(contRect.left + contRect.width / 2.0f, contRect.top + contRect.height / 2.0f);
    textoContinuar.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f + 220.0f);
}

void MenuNoInteractivo::dibujarPantallaVictoria(sf::RenderWindow& window) {
    window.setView(window.getDefaultView());

    window.draw(spriteFondo);
    window.draw(textoVictoria);
    window.draw(textoJugadorGanador);
    window.draw(textoPuntuaciones);
    window.draw(textoTiempo);
    window.draw(textoContinuar);
}

void MenuNoInteractivo::dibujarPantallaInstrucciones(sf::RenderWindow& window) {
    window.setView(window.getDefaultView());

    // Aseguramos cargar el fondo que queramos para las instrucciones
    if (!texturaFondo.loadFromFile("imagenes/pantalla_inicio.png")) {
        std::cout << "Error cargando la pantalla de Instrucciones" << std::endl;
    }

    float escalaX = (float)window.getSize().x / texturaFondo.getSize().x;
    float escalaY = (float)window.getSize().y / texturaFondo.getSize().y;
    spriteFondo.setScale(escalaX, escalaY);
    spriteFondo.setTexture(texturaFondo, true);

    // Centrado de texto
    sf::FloatRect textRect = textoInstrucciones.getLocalBounds();
    textoInstrucciones.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    textoInstrucciones.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

    window.draw(spriteFondo);
    window.draw(textoInstrucciones);
}

void MenuNoInteractivo::dibujarPantallaCreditos(sf::RenderWindow& window) {
    window.setView(window.getDefaultView());

    // Carga la imagen de fondo:
    if (!texturaFondo.loadFromFile("imagenes/Pantalla_Creditos.png")) {
        std::cout << "Error cargando la pantalla de CREDITOS" << std::endl;
    }

    float escalaX = (float)window.getSize().x / texturaFondo.getSize().x;
    float escalaY = (float)window.getSize().y / texturaFondo.getSize().y;
    spriteFondo.setScale(escalaX, escalaY);
    spriteFondo.setTexture(texturaFondo, true);

    // Centrado de texto
    sf::FloatRect textRect = textoCreditos.getLocalBounds();
    textoCreditos.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    textoCreditos.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

    window.draw(spriteFondo);
    window.draw(textoCreditos);
}

void MenuNoInteractivo::dibujarPantallaRanking(sf::RenderWindow& window) {
    window.setView(window.getDefaultView());

    //Cargamos el fondo de la pantalla de inicio
    if (!texturaFondo.loadFromFile("imagenes/Ranking.png")) {
        std::cout << "Error cargando fondo en Ranking" << std::endl;
    }
    spriteFondo.setTexture(texturaFondo, true);
    spriteFondo.setScale((float)window.getSize().x / texturaFondo.getSize().x, (float)window.getSize().y / texturaFondo.getSize().y);
    window.draw(spriteFondo);

    //Configuraramos el título del Ranking
    textoRankingTitulo.setFont(fuente);
    textoRankingTitulo.setCharacterSize(70);
    textoRankingTitulo.setFillColor(sf::Color::Yellow);
    textoRankingTitulo.setOutlineColor(sf::Color::Black);
    textoRankingTitulo.setOutlineThickness(3.f);
    textoRankingTitulo.setString("SALON DE LA FAMA - TOP JUGADORES");

    sf::FloatRect rectT = textoRankingTitulo.getLocalBounds();
    textoRankingTitulo.setOrigin(rectT.left + rectT.width / 2.0f, rectT.top + rectT.height / 2.0f);
    textoRankingTitulo.setPosition(window.getSize().x / 2.0f, 80.f);
    window.draw(textoRankingTitulo);

    //Lee el ranking.txt
    std::vector<RegistroPartida> listaPartidas;
    std::ifstream archivoLectura("ranking.txt");

    if (archivoLectura.is_open()) {
        RegistroPartida reg;
        while (archivoLectura >> reg.nombre >> reg.bando >> reg.puntosLuz >> reg.puntosOscuridad >> reg.tiempo) {
            listaPartidas.push_back(reg);
        }
        archivoLectura.close();
    }

    //Hace un "ranking". Ordenamos de menor tiempo a mayor tiempo
    std::sort(listaPartidas.begin(), listaPartidas.end(), [](const RegistroPartida& a, const RegistroPartida& b) {
        return a.tiempo < b.tiempo;
        });

    //Renderizamos las filas (Mostramos como máximo el Top 7 para que no se sature la pantalla)
    textoRankingLineas.setFont(fuente);
    textoRankingLineas.setCharacterSize(40);
    textoRankingLineas.setOutlineColor(sf::Color::Black);
    textoRankingLineas.setOutlineThickness(2.f);

    float yFila = 200.f;
    int maxMuestras = std::min(static_cast<int>(listaPartidas.size()), 7); 

    if (maxMuestras == 0) {
        textoRankingLineas.setString("NO HAY PARTIDAS REGISTRADAS TODAVIA.");
        sf::FloatRect r = textoRankingLineas.getLocalBounds();
        textoRankingLineas.setOrigin(r.left + r.width / 2.f, 0);
        textoRankingLineas.setPosition(window.getSize().x / 2.f, yFila + 100.f);
        window.draw(textoRankingLineas);
    }
    else {
        // Cabecera de la tabla de posiciones
        textoRankingLineas.setFillColor(sf::Color::Red);
        textoRankingLineas.setString("POS     JUGADOR            BANDO            IMP_PTS        XEN_PTS        TIEMPO");
        sf::FloatRect rCab = textoRankingLineas.getLocalBounds();
        textoRankingLineas.setOrigin(rCab.left + rCab.width / 2.f, 0);
        textoRankingLineas.setPosition(window.getSize().x / 2.f, yFila);
        window.draw(textoRankingLineas);
        yFila += 60.f;

        // Pintamos cada registro ordenado
        for (int i = 0; i < maxMuestras; i++) {
            RegistroPartida p = listaPartidas[i];

            // Color dependiendo del bando que ganó
            if (p.bando == "IMPERIUM") {
                textoRankingLineas.setFillColor(sf::Color::Cyan); // Azul Humanidad
            }
            else {
                textoRankingLineas.setFillColor(sf::Color::Magenta); // Violeta Xenos
            }

            // Formatear tiempo
            int min = static_cast<int>(p.tiempo) / 60;
            int seg = static_cast<int>(p.tiempo) % 60;
            std::string strSeg = (seg < 10 ? "0" : "") + std::to_string(seg);
            std::string strTiempo = std::to_string(min) + ":" + strSeg;

            // Construir línea
            char buffer[200];
            snprintf(buffer, sizeof(buffer), "#%-3d %-16s %-14s %-12d %-12d %s",
                (i + 1), p.nombre.c_str(), p.bando.c_str(), p.puntosLuz, p.puntosOscuridad, strTiempo.c_str());

            textoRankingLineas.setString(buffer);

            sf::FloatRect rectFila = textoRankingLineas.getLocalBounds();
            textoRankingLineas.setOrigin(rectFila.left + rectFila.width / 2.f, 0);
            textoRankingLineas.setPosition(window.getSize().x / 2.f, yFila);

            window.draw(textoRankingLineas);
            yFila += 50.f; // Espacio entre filas
        }
    }
    textoContinuar.setCharacterSize(40);
    textoRankingLineas.setOutlineColor(sf::Color::Black);
    textoContinuar.setFillColor(sf::Color::Yellow);
    textoContinuar.setString("PULSA ESC PARA VOLVER AL MENU PRINCIPAL");
    sf::FloatRect rectC = textoContinuar.getLocalBounds();
    textoContinuar.setOrigin(rectC.left + rectC.width / 2.0f, rectC.top + rectC.height / 2.0f);
    textoContinuar.setPosition(window.getSize().x / 2.0f, window.getSize().y - 60.f);
    window.draw(textoContinuar);
}