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
    textoContinuar.setString("PULSA ESC PARA VOLVER AL MENU");
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