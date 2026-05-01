#include <SFML/Graphics.hpp>
#include "Mundo.h"

int main() {
    //instanciamos el objeto 'window de la clase sf::RenderWindow'
    //Recibe dos parámetros, un vector con las dimensiones y título de la ventana
    sf::RenderWindow window(sf::VideoMode(800, 650), "Archon 40k - Tablero Prototipo");
    window.setFramerateLimit(60);
    Mundo mimundo;

    //Bucle principal del juego: se ejecuta mientras la ventana esté abierta.
    //Se ejecutará miles de veces por segundo mientras que la ventana siga abierta

           //---A) GESTIÓN DE EVENTOS (entrada de teclado, ratón, cerrar ventana)
        //Pregunta al sistema si ha habido entrada del usuario (evento)
        //Mientras que haya un evento entra en el bucle while. Si no, sale.

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // NUEVO: Detectar click del ratón
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // Obtenemos posición del ratón relativa a la ventana
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    mimundo.manejarClick(mousePos);
                }
            }
        }




        //---B) LÓGICA DEL JUEGO (UPDATE)
         // Aquí van los cálculos matemáticos, reglas y movimientos




        // ---C) RENDERIZADO (DRAW)

        window.clear(sf::Color(30, 30, 30)); // Fondo oscuro para que resalte el tablero        //1º: Limpiar la ventana (color de fondo) para evitar que se dibuje una imagen sobre otra
        mimundo.dibujar(window); // Dibujamos el tablero     //2º:Dibujar el fotograma actual, aquí irían las funciones DIBUJA()
        window.display();       // 3º:Mostrar en pantalla
        //window.display() coge todo lo dibujado en el buffer de memoria y lo imprime en pantalla

    }

    return 0;
}
