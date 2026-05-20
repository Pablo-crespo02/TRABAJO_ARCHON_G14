#include "MenusNoInteractivos.h"
#include <iostream>


void MenuNoInteractivo::inicializarTextos() {

    //Carga del font y manejo del error en caso contrario:
    if (!fuente.loadFromFile("fuentes/fuente_pixel.ttf")) {
        std::cout << "Error cargando fuente" << std::endl;
        return;
    }

    //Configuración de las instrucciones:
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
   
    //Configuración de los créditos:
    textoCreditos.setFont(fuente);
    textoCreditos.setCharacterSize(35);
    textoCreditos.setFillColor(sf::Color::Yellow);
    textoCreditos.setOutlineColor(sf::Color::Black);
    textoCreditos.setOutlineThickness(4.0f);
    textoCreditos.setString(
        "               DESARROLLO Y PROGRAMACION\n\n"
        "               Javier Monrio\n"
        "               Gonzalo Castro\n"
        "               Pablo Crespo\n"
        "               Javier Lerin\n"
        "               Cecilia Barrio\n\n\n"
        "               BASADO EN\n"
        "               Archon: The Light and the Dark (1983)\n\n\n"
        "               UNIVERSO Y LORE\n"
        "               Warhammer 40,000 (Games Workshop)\n\n\n"
        "      (Pulsa ESC para volver al Menu Principal)"
    );
    textoCreditos.setPosition(150.f, 120.f);

    //Configuración de los elementos comunes de la pantalla de victoria:
    textoVictoria.setFont(fuente);
    textoVictoria.setCharacterSize(80);
    textoVictoria.setOutlineColor(sf::Color::Black);
    textoVictoria.setOutlineThickness(4.0f);
    textoContinuar.setFont(fuente);
    textoContinuar.setString("Pulsa ENTER para volver al menu");
    textoContinuar.setCharacterSize(30);
    textoContinuar.setOutlineColor(sf::Color::Black);
    textoContinuar.setOutlineThickness(2.0f);
    textoContinuar.setFillColor(sf::Color::White);

   }

void MenuNoInteractivo::configurarPantallaVictoria(int ganador, int ptosLuz, int ptosOscuridad, float tiempoJugado, sf::RenderWindow& window)
{
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

        spriteFondo.setTexture(texturaFondo,true);  //true garantiza el correcto scaling de la imagen
        textoVictoria.setString("VICTORIA DEL IMPERIUM");
        textoVictoria.setFillColor(sf::Color::Yellow);
        break;

    case 2:

        if (!texturaFondo.loadFromFile("imagenes/Pantalla_Victoria_Tiranidos.png")) {
            std::cout << "Error cargando la pantalla de Victoria OSCURIDAD" << std::endl;
            return;
        }

        spriteFondo.setTexture(texturaFondo,true);
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

    float escalaX = (float)tamanoVentana.x/ tamanoImg.x;
    float escalaY = (float)tamanoVentana.y / tamanoImg.y;
    spriteFondo.setScale(escalaX, escalaY);


    //CONFIGURACIÓN DEL TEXTO:
    // Texto principal:
    sf::FloatRect textRect = textoVictoria.getLocalBounds();
    textoVictoria.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    textoVictoria.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 120.f); // Lo subimos un poco

    // CONFIGURACIÓN TEXTO DE PUNTUACIONES
    textoPuntuaciones.setFont(fuente);
    textoPuntuaciones.setCharacterSize(45);
    textoPuntuaciones.setOutlineColor(sf::Color::Black);
    textoPuntuaciones.setOutlineThickness(3.0f);
    textoPuntuaciones.setFillColor(sf::Color::White);

    std::string txtPuntos = "PUNTUACION IMPERIUM: " + std::to_string(ptosLuz) + "\n" +
        "PUNTUACION XENOS: " + std::to_string(ptosOscuridad);
    textoPuntuaciones.setString(txtPuntos);

    sf::FloatRect ptRect = textoPuntuaciones.getLocalBounds();
    textoPuntuaciones.setOrigin(ptRect.left + ptRect.width / 2.0f, ptRect.top + ptRect.height / 2.0f);
    textoPuntuaciones.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f + 20.f);
    //CONFIGURACIÓN TEXTO DE TIEMPO
    textoTiempo.setFont(fuente);
    textoTiempo.setCharacterSize(40);
    textoTiempo.setOutlineColor(sf::Color::Black);
    textoTiempo.setOutlineThickness(3.0f);
    textoTiempo.setFillColor(sf::Color::Cyan); // Color cian para que destaque

    // Cálculo matemático de minutos y segundos
    int minutos = static_cast<int>(tiempoJugado) / 60;
    int segundos = static_cast<int>(tiempoJugado) % 60;

    // Formato con cero a la izquierda si los segundos son menos de 10 (ej: 05)
    std::string strSegundos = (segundos < 10 ? "0" : "") + std::to_string(segundos);
    std::string strTiempo = "TIEMPO DE PARTIDA: " + std::to_string(minutos) + ":" + strSegundos;

    textoTiempo.setString(strTiempo);

    sf::FloatRect tiemRect = textoTiempo.getLocalBounds();
    textoTiempo.setOrigin(tiemRect.left + tiemRect.width / 2.0f, tiemRect.top + tiemRect.height / 2.0f);
    textoTiempo.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f + 90.f);

 // Texto de volver al menú:
    sf::FloatRect contRect = textoContinuar.getLocalBounds();
    textoContinuar.setOrigin(contRect.left + contRect.width / 2.0f, contRect.top + contRect.height / 2.0f);
    textoContinuar.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f + 180.0f); 
}

void MenuNoInteractivo::dibujarPantallaVictoria(sf::RenderWindow& window) {

    // Aseguramos la vista por defecto para que el menú no se mueva con la cámara del juego
    window.setView(window.getDefaultView());

    window.draw(spriteFondo);
    window.draw(textoVictoria);
    window.draw(textoPuntuaciones);
    window.draw(textoTiempo);
    window.draw(textoContinuar);
}

void MenuNoInteractivo::dibujarPantallaInstrucciones(sf::RenderWindow& window) {
    window.setView(window.getDefaultView());

    //Centra el texto independientemente de la resolución de la pantalla:
    sf::FloatRect textRect = textoInstrucciones.getLocalBounds();
    textoInstrucciones.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

    //Lo coloca en el centro exacto de la ventana:
    textoInstrucciones.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

    //Carga la imagen de fondo:
    if (!texturaFondo.loadFromFile("imagenes/Pantalla_Instrucciones.png")) {
        std::cout << "Error cargando la pantalla de Victoria INSTRUCCIONES" << std::endl;
        return;
    }

    sf::Vector2u tamanoVentana = window.getSize();
    sf::Vector2u tamanoImg = texturaFondo.getSize();

    float escalaX = (float)tamanoVentana.x / tamanoImg.x;
    float escalaY = (float)tamanoVentana.y / tamanoImg.y;
    spriteFondo.setScale(escalaX, escalaY);

    spriteFondo.setTexture(texturaFondo, true);
    window.draw(spriteFondo);
    window.draw(textoInstrucciones);
}

void MenuNoInteractivo::dibujarPantallaCreditos(sf::RenderWindow& window) {
    window.setView(window.getDefaultView());

    //Centra el texto independientemente de la resolución de la pantalla:
    sf::FloatRect textRect = textoCreditos.getLocalBounds();
    textoCreditos.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

    //Lo coloca en el centro exacto de la ventana:
    textoCreditos.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

    //Carga la imagen de fondo:
    if (!texturaFondo.loadFromFile("imagenes/Pantalla_Creditos.png")) {
        std::cout << "Error cargando la pantalla de Victoria CREDITOS" << std::endl;
        return;
    }

    sf::Vector2u tamanoVentana = window.getSize();
    sf::Vector2u tamanoImg = texturaFondo.getSize();

    float escalaX = (float)tamanoVentana.x / tamanoImg.x;
    float escalaY = (float)tamanoVentana.y / tamanoImg.y;
    spriteFondo.setScale(escalaX, escalaY);

    spriteFondo.setTexture(texturaFondo, true);
    window.draw(spriteFondo);    
    window.draw(textoCreditos);
}
