#include "Coordinador.h"
#include "Generador.h"
#include "Pieza.h"
Coordinador::Coordinador():motor(window, fuente)
{
    // Carga de la fuente:
    if (!fuente.loadFromFile("fuentes/fuente_pixel.ttf")) {
        std::cout << "Error cargando fuente" << std::endl;
        return;
    }

    // 1. Inicializamos menús no interactivos:
    pantallainfo.inicializarTextos();  

    // 2. Sonido del click en el menú
    if (!bufferClick.loadFromFile("sonidos/click.mp3")) {
        std::cout << "Aviso: No se pudo cargar el sonido click.wav" << std::endl;
    }
    else {
        sonidoClick.setBuffer(bufferClick);
        sonidoClick.setVolume(50.f);
    }

    //2.1 Música del menú:
    if (!musicaMenu.openFromFile("sonidos/musica_menu.mp3")) {
        std::cout << "Aviso: No se encontro musica_menu.mp3" << std::endl;
    }
    else {
        musicaMenu.setLoop(true);   // la canción se repita en bucle
        musicaMenu.setVolume(60.f);
        musicaMenu.play();
    }

    // 3. Creación de la ventana:
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    window.create(desktop, "ARCHON WARHAMMER 40K", sf::Style::Fullscreen);

    // 4. Se inicializan las pantallas:
    pantallaCarga = new PantallaCarga(fuente, window.getSize());
    menuPausa = new MenuPausa(fuente, window.getSize());
    menuNombre = new MenuNombre(fuente, window.getSize());
    

    // 5. Configuración final:
    estadoActual = Estado::MenuPrincipal;
    vistaUI = window.getDefaultView();
    vistaTablero.setSize(620.f, 620.f);
    vistaTablero.setCenter(270.f, 270.f);
    vistaTablero.setViewport(sf::FloatRect(0.04f, 0.10f, 0.60f, 0.80f));

    //cargar las partidas guardadas
    cargarDatosDeFichero();
}

void Coordinador::ejecutar() {
    sf::Clock reloj;
    while (window.isOpen()) {
        float dt = reloj.restart().asSeconds();
        gestionarEventos();
        actualizar(dt);
        dibujar();
    }
}

void Coordinador::gestionarEventos() {
    sf::Event evento;
    while (window.pollEvent(evento)) {
        if (evento.type == sf::Event::Closed) window.close();

        // 1. Tecla ESCAPE (Pausa/Volver)
        if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape) {

            if (estadoActual == Estado::Tablero || estadoActual == Estado::Arena) {
                estadoAnterior = estadoActual;
                estadoActual = Estado::Pausa;
            }
            else if (estadoActual == Estado::Pausa) {
                estadoActual = estadoAnterior;
            }
            else if (estadoActual == Estado::Instrucciones || estadoActual == Estado::Creditos || estadoActual == Estado::SeleccionCarga || estadoActual == Estado::Ranking) {
                estadoActual = estadoAnterior; }
            }
        
        //Menú principal:
        if (estadoActual == Estado::MenuPrincipal) {
            // Actualizamos la apariencia del botón "Reanudar" (gris o normal)
            pantallaInicio.setPartidaActiva(partidaEnCurso);

            //sonido cuando te mueves arriba y abajo en el menu principal
            if (evento.type == sf::Event::KeyPressed) {
                if (evento.key.code == sf::Keyboard::Up)
                {
                    pantallaInicio.moverArriba();
                    sonidoClick.play();
                }
                if (evento.key.code == sf::Keyboard::Down)
                {
                    pantallaInicio.moverAbajo();
                    sonidoClick.play();
                }
                if (evento.key.code == sf::Keyboard::Enter||evento.key.code == sf::Keyboard::Return) {
                    int seleccion = pantallaInicio.getIndiceSeleccionado();
                    switch (seleccion) {
                    case 0: // Iniciar partida:
                        this->reiniciarPartida();
                        partidaEnCurso = true;
                        estadoActual = Estado::Tablero;
                        motor.setEstado(Estado::Tablero);
                        break;

                    case 1: // Reanudar partida:
                        if (partidaEnCurso) {
                            estadoActual = Estado::Tablero;
                            motor.setEstado(Estado::Tablero);
                        }
                        break;

                    case 2: // Instrucciones:
                    {
                          estadoAnterior = Estado::MenuPrincipal;
                          estadoActual = Estado::Instrucciones; 
                          break;
                    }
                    case 3:// Ranking:
                    {
                        estadoAnterior = Estado::MenuPrincipal;
                        estadoActual = Estado::Ranking;
                        break;
                    }
                    case 4: //CREDITOS 
                    {
                        estadoAnterior = Estado::MenuPrincipal;
                        estadoActual = Estado::Creditos; 
                        break;
                    }
                    case 5: // Salir del juego:
                        window.close();
                        break;

                    case 6: // Cargar partida (Lleva al menú de ranuras)
                        modoGuardar = false;
                        pantallaCarga->actualizarTextosRanuras(ranuras[0].ocupada, ranuras[1].ocupada, ranuras[2].ocupada);
                        estadoActual = Estado::SeleccionCarga;
                        break;
                    }
                }
            }
        }
        //Menú de pausa
        else if (estadoActual == Estado::Pausa) {
            if (evento.type == sf::Event::KeyPressed) {
                if (evento.key.code == sf::Keyboard::Up)
                {
                    menuPausa->moverArriba();
                    sonidoClick.play();
                }
                if (evento.key.code == sf::Keyboard::Down)
                {
                    menuPausa->moverAbajo();
                    sonidoClick.play();
                }

                if (evento.key.code == sf::Keyboard::Enter || evento.key.code == sf::Keyboard::Return) {
                    int selPausa = menuPausa->getIndiceSeleccionado();
                    switch (selPausa) {
                    case 0: estadoActual = estadoAnterior; break; // Reanudar
                    case 1: reiniciarPartida(); estadoActual = Estado::Tablero; break; // Reiniciar
                    case 2: estadoActual = Estado::MenuPrincipal; break; // Volver al menú
                    case 3: { 
                        estadoAnterior = Estado::Pausa;
                        estadoActual = Estado::Instrucciones; break; 
                    } // Instrucciones

                    case 4: // Guardar partida (Lleva al menú de ranuras)
                        modoGuardar = true;
                        pantallaCarga->actualizarTextosRanuras(ranuras[0].ocupada, ranuras[1].ocupada, ranuras[2].ocupada);
                        estadoActual = Estado::SeleccionGuardar;
                        break;

                    case 5: // Salir al escritorio
                        window.close();
                        break;
                    }
                }
            }
        }
        //Menús de carga y de guardado:
        else if (estadoActual == Estado::SeleccionCarga || estadoActual == Estado::SeleccionGuardar) {
            if (evento.type == sf::Event::KeyPressed) {
                if (evento.key.code == sf::Keyboard::Up)
                {
                    pantallaCarga->moverArriba();
                    sonidoClick.play();
                }
                if (evento.key.code == sf::Keyboard::Down)
                {
                    pantallaCarga->moverAbajo();
                    sonidoClick.play();
                }

                if (evento.key.code == sf::Keyboard::Enter) {
                    int ranura = pantallaCarga->getIndiceSeleccionado();
                    if (ranura == 3) { // Opción "volver"
                        estadoActual = (estadoActual == Estado::SeleccionGuardar) ? Estado::Pausa : Estado::MenuPrincipal;
                        }
                    else {
                        if (estadoActual == Estado::SeleccionGuardar) {
                            this->guardarEnRanura(ranura);
                            pantallaCarga->actualizarTextosRanuras(ranuras[0].ocupada, ranuras[1].ocupada, ranuras[2].ocupada);
                            estadoActual = Estado::Pausa;
                        }
                        else {
                            this->cargarDesdeRanura(ranura);
                            partidaEnCurso = true;
                        }
                    }
                }
            }
        } 
        //Menu Nombre
        if (estadoActual == Estado::Nombre) {
            if (evento.type == sf::Event::TextEntered) {
                if (evento.text.unicode == '\b') {
                    menuNombre->borrarLetra();
                }
                else {
                    menuNombre->procesarTexto(evento.text.unicode);
                }
            }
            else if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Enter) {
                if (!menuNombre->getNombre().empty()) {
                    //  Escritura en el fichero de guardado de partidas:
                    std::ofstream archivoEscritura("ranking.txt", std::ios::app); 
                    if (archivoEscritura.is_open()) {
                        std::string bandoGanador = (motor.getGanador() == 1) ? "IMPERIUM" : "XENOS";

                        // Guardamos cada variable en una línea independiente
                        archivoEscritura << menuNombre->getNombre() << "\n"
                            << bandoGanador << "\n"
                            << motor.getPuntosLuz() << "\n"
                            << motor.getPuntosOscuridad() << "\n"
                            << motor.getTiempoJugado() << "\n";

                        archivoEscritura.close();
                        std::cout << "Partida registrada en el archivo de historial con exito." << std::endl;
                    }
                    estadoActual = Estado::Victoria;
                    pantallainfo.configurarPantallaVictoria(motor.getGanador(), motor.getPuntosLuz(), motor.getPuntosOscuridad(), motor.getTiempoJugado(), menuNombre->getNombre(), window);
                }
            }
        }
        //Pantalla de victoria
        else if (estadoActual == Estado::Victoria) {
            if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape) {
                estadoActual = Estado::MenuPrincipal;
                motor.reiniciarJuego();
            }
        }
        else {
            // Juego normal (Tablero o Arena)
            motor.gestionarEntrada(evento, vistaTablero);
        }
    } // Cierra el while
} // Cierra la función gestionarEventos

void Coordinador::dibujar() {
    window.clear();

    if (estadoActual == Estado::MenuPrincipal) {
        pantallaInicio.dibujar(window);
    }
    // Si estamos jugando O en pausa, dibujamos el mundo
    else if (estadoActual == Estado::Tablero || (estadoAnterior == Estado::Tablero && estadoActual == Estado::Pausa)) {
        window.setView(vistaTablero);
        motor.renderizar();
        window.setView(vistaUI);
        motor.dibujarHUD();
    }
    else if (estadoActual == Estado::Arena || (estadoAnterior == Estado::Arena && estadoActual == Estado::Pausa)) {
        window.setView(vistaTablero);
        motor.renderizar();
    }
    // Si es pausa, dibujamos el menú de pausa al final del todo (encima de todo)
    if (estadoActual == Estado::Pausa) {
        menuPausa->dibujar(window);
    }
    //Menú nombre:
    else if (estadoActual == Estado::Nombre) {
        menuNombre->dibujar(window);
    }
    //Pantalla Victoria:
    else if (estadoActual == Estado::Victoria) {
        window.setView(vistaUI);
        pantallainfo.dibujarPantallaVictoria(window);
    }
    else if (estadoActual == Estado::Ranking) {
        window.setView(vistaUI);
        pantallainfo.dibujarPantallaRanking(window);
    }
    //Pantalla instrucciones:
    else if (estadoActual == Estado::Instrucciones) {
        window.setView(vistaUI);
        pantallainfo.dibujarPantallaInstrucciones(window);
    }
    //Menú de carga:
    else if (estadoActual == Estado::SeleccionCarga) {
        pantallaInicio.dibujar(window); // Dibujamos el fondo del marine y el tiranido
        window.setView(vistaUI);
        pantallaCarga->dibujar(window); // Dibujamos las ranuras encima
    }
    // 7. Menú de guardado:
    else if (estadoActual == Estado::SeleccionGuardar) {

        //Se respeta el renderizado del mapa y el fondo anterior, sólo se puede acceder a esta pantalla desde el menú de pausa del juego:
        menuPausa->dibujarFondo(window);

        window.setView(vistaUI);
        pantallaCarga->dibujar(window);
    }
    // 8. Pantalla de créditos
    else if (estadoActual == Estado::Creditos) {
        window.setView(vistaUI);
        pantallainfo.dibujarPantallaCreditos(window);
    }
    window.display();
}

void Coordinador::actualizar(float dt) {

    //Control físico de los menús
    bool estamosEnUnMenu = (estadoActual == Estado::MenuPrincipal ||
        estadoActual == Estado::Pausa ||
        estadoActual == Estado::Instrucciones ||
        estadoActual == Estado::Creditos ||
        estadoActual == Estado::SeleccionGuardar ||
        estadoActual == Estado::SeleccionCarga);

    if (estamosEnUnMenu) {
        // Si venimos de jugar y la música está pausada, la reanudamos:
        if (musicaMenu.getStatus() != sf::SoundSource::Playing) {
            musicaMenu.play();
        }
    }
    else {
        // Si estamos jugando (Tablero/Arena/Victoria) y la música sigue sonando, la pausamos
        if (musicaMenu.getStatus() == sf::SoundSource::Playing) {
            musicaMenu.pause();
        }
    }
    //  El temporizador global:
    if (estadoActual == Estado::Tablero || estadoActual == Estado::Arena) {
        motor.setTiempoJugado(motor.getTiempoJugado() + dt);
    }

    // Sincronizamos el estado para que el Coordinador sepa qué dibujar
    if (motor.getEstado() == Estado::Tablero && estadoActual == Estado::Arena) {
        estadoActual = Estado::Tablero;
    }
    else if (motor.getEstado() == Estado::Arena && estadoActual == Estado::Tablero) {
        estadoActual = Estado::Arena;
    }
    else if (motor.getEstado() == Estado::Victoria && estadoActual != Estado::Victoria && estadoActual != Estado::Nombre) {
        estadoActual = Estado::Nombre;
        menuNombre->configurarGanador(motor.getGanador());
    }
    else if (estadoActual == Estado::Nombre) {
        menuNombre->dibujar(window);
    }

    // Sólo se actualiza si NO estamos en pausa
    if (estadoActual != Estado::Pausa) {
        motor.actualizar(dt);
    }
}

void Coordinador::reiniciarPartida() {
    motor.limpiarDatos();
    std::cout << "DEBUG: Datos del motor limpiados y unidades desplegadas." << std::endl;
}

void Coordinador::guardarEnRanura(int indice) {
    // 1. Limpiamos los datos ligeros viejos de la ranura
    ranuras[indice].piezasLigeras.clear();

    // 2. Extraemos las piezas actuales del tablero
    std::vector<Pieza*> piezasActuales = motor.getListaPiezas();

    // 3. Convertimos las piezas físicas a nuestro nuevo formato "Ligero"
    for (Pieza* p : piezasActuales) {
        DatosPiezaLigera datosP;
        datosP.bando = static_cast<int>(p->getBando());
        datosP.posX = p->getPosicionTablero().x;
        datosP.posY = p->getPosicionTablero().y;
        datosP.nombre = p->stats.nombre;
        datosP.vida = p->stats.vida;

        ranuras[indice].piezasLigeras.push_back(datosP);
    }

    // 4. Guardamos el estado global
    ranuras[indice].ronda = motor.getRondaActual();
    ranuras[indice].ciclo = motor.getCicloActual();
    ranuras[indice].jugador = motor.getJugadorActual();
    ranuras[indice].puntosLuz = motor.getPuntosLuz();
    ranuras[indice].puntosOscuridad = motor.getPuntosOscuridad();
    ranuras[indice].tiempoJugado = motor.getTiempoJugado();

    // 5. Marcamos el Slot como ocupado y guardamos en el txt
    ranuras[indice].ocupada = true;
    std::cout << " Partida guardada con exito en la ranura " << indice + 1 << "!" << std::endl;

    guardarDatosEnFichero();
}

void Coordinador::cargarDesdeRanura(int indice) {
    if (ranuras[indice].ocupada) {
        // 1. Reseteamos el motor 
        motor.limpiarDatos();

        // 2. Vaciamos por completo el vector de piezas por defecto del motor
        // Liberamos su memoria para evitar fugas (Memory Leaks) y lo dejamos a 0.
        for (Pieza* p : motor.getListaPiezas()) {
            delete p;
        }
        std::vector<Pieza*> tableroVacio;
        motor.setListaPiezas(tableroVacio);
      
        // 3. Restauramos las variables globales de la partida guardada
        motor.setRondaActual(ranuras[indice].ronda);
        motor.setCicloActual(ranuras[indice].ciclo);
        motor.setJugadorActual(ranuras[indice].jugador);
        motor.setPuntosLuz(ranuras[indice].puntosLuz);
        motor.setPuntosOscuridad(ranuras[indice].puntosOscuridad);
        motor.setTiempoJugado(ranuras[indice].tiempoJugado);

        // 4. Materalizamos las piezas:
        for (const DatosPiezaLigera& datosP : ranuras[indice].piezasLigeras) {
            Bando b = static_cast<Bando>(datosP.bando);
            sf::Vector2i pos(datosP.posX, datosP.posY);

            Generador::AnadirUnidad(motor, b, datosP.nombre, pos);

            if (!motor.getListaPiezas().empty()) {
                motor.getListaPiezas().back()->stats.vida = datosP.vida;
            }
        }

        // 5. Cambiamos los estados para reanudar la partida
        estadoActual = Estado::Tablero;
        motor.setEstado(Estado::Tablero);
        std::cout << "Partida cargada desde la ranura " << indice + 1 << " de forma limpia y exitosa." << std::endl;
    }
    else {
        std::cout << "La ranura " << indice + 1 << " esta vacia." << std::endl;
    }
}

// Función que lee el archivo:
void Coordinador::cargarDatosDeFichero() {
    std::ifstream archivo("partidas_guardadas.txt");

    // Si el archivo no existe (primera vez que juegas), no pasa nada
    if (!archivo.is_open()) {
        std::cout << "Aviso: NO HAY ARCHIVO GUARDADO PREVIO. Se creara uno nuevo al jugar." << std::endl;
        return;
    }

    for (int i = 0; i < 3; i++) {
        if (!(archivo >> ranuras[i].ocupada)) break;

        if (ranuras[i].ocupada) {

            // Variables globales de la partida
            if (!(archivo >> ranuras[i].ronda
                >> ranuras[i].ciclo
                >> ranuras[i].jugador
                >> ranuras[i].puntosLuz
                >> ranuras[i].puntosOscuridad
                >> ranuras[i].tiempoJugado)) {
                std::cout << "Error leyendo cabecera de la ranura " << i + 1 << std::endl;
                break;
            }

            int numPiezas;
            if (!(archivo >> numPiezas)) break;

         
            ranuras[i].piezasLigeras.clear();

            // Reconstruimos en memoria de texto puro (Súper rápido, sin tocar imágenes)
            for (int j = 0; j < numPiezas; j++) {
                DatosPiezaLigera datosP;

                // Leemos exactamente las variables en tu orden original
                if (!(archivo >> datosP.bando >> datosP.posX >> datosP.posY >> datosP.nombre >> datosP.vida)) {
                    std::cout << "Error de formato en la pieza " << j << " de la ranura " << i + 1 << " CARGA ABORTADA" << std::endl;
                    break;
                }

                // Guardamos los datos puros en nuestra ranura optimizada
                ranuras[i].piezasLigeras.push_back(datosP);
            }
        }
    }
    archivo.close();
    std::cout << "Estructura de partidas guardadas inicializada al instante desde partidas_guardadas.txt" << std::endl;
}

//Esta función lo que hace es coger todo lo que haya en la memoria de las 3 ranuras y lo escribe en partidasa_guardadas.txt
void Coordinador::guardarDatosEnFichero() {
    std::ofstream archivo("partidas_guardadas.txt");
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo crear el archivo de guardado." << std::endl;
        return;
    }

    for (int i = 0; i < 3; i++) {
        archivo << ranuras[i].ocupada << " ";
        if (ranuras[i].ocupada) {
            // Guardar el estado global
            archivo << ranuras[i].ronda << " "
                << ranuras[i].ciclo << " "
                << ranuras[i].jugador << " "
                << ranuras[i].puntosLuz << " "
                << ranuras[i].puntosOscuridad << " "
                << ranuras[i].tiempoJugado << " ";

            // Guardar cuántas piezas hay vivas (usando el vector ligero)
            archivo << ranuras[i].piezasLigeras.size() << " ";

            // Guardamos los datos de cada pieza plana
            for (const DatosPiezaLigera& p : ranuras[i].piezasLigeras) {
                archivo << p.bando << " "
                    << p.posX << " "
                    << p.posY << " "
                    << p.nombre << " "
                    << p.vida << " ";
            }
        }
        archivo << "\n";
    }
    archivo.close();
}
