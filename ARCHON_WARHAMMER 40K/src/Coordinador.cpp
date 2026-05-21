#include "Coordinador.h"
#include "Generador.h"

Coordinador::Coordinador():motor(window, fuente)
{
    // CARGA DE LA FUENTE:
    if (!fuente.loadFromFile("fuentes/fuente_pixel.ttf")) {
        std::cout << "Error cargando fuente" << std::endl;
        return;
    }

    // 1. INIIALIZAMOS LOS MENÚS NO INTERACTIVOS:
    pantallainfo.inicializarTextos();  

    // 2. CARGAMOS EL SONIDO
    if (!bufferClick.loadFromFile("sonidos/click.mp3")) {
        std::cout << "Aviso: No se pudo cargar el sonido click.wav" << std::endl;
    }
    else {
        sonidoClick.setBuffer(bufferClick);
        sonidoClick.setVolume(50.f);
    }

    // 3. CREAMOS LA VENTANA
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    window.create(desktop, "ARCHON WARHAMMER 40K", sf::Style::Fullscreen);

    // 4. INICIALIZAMOS LAS PANTALLAS
    pantallaCarga = new PantallaCarga(fuente, window.getSize());
    menuPausa = new MenuPausa(fuente, window.getSize());
    

    // 5. CONFIGURACIÓN FINAL
    estadoActual = Estado::MenuPrincipal;
    vistaUI = window.getDefaultView();
    vistaTablero.setSize(700.f, 700.f);
    vistaTablero.setCenter(350.f, 350.f);
    vistaTablero.setViewport(sf::FloatRect(0.10f, 0.20f, 0.60f, 0.80f));

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
            else if (estadoActual == Estado::Instrucciones || estadoActual == Estado::Creditos || estadoActual == Estado::SeleccionCarga) {
                estadoActual = estadoAnterior; }
            }
        
        //MENÚ PRINCIPAL
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

                if (evento.key.code == sf::Keyboard::Enter || evento.key.code == sf::Keyboard::Return) {
                    int seleccion = pantallaInicio.getIndiceSeleccionado();
                    switch (seleccion) {
                    case 0: // INICIAR PARTIDA
                        this->reiniciarPartida();
                        partidaEnCurso = true;
                        estadoActual = Estado::Tablero;
                        motor.setEstado(Estado::Tablero);
                        break;

                    case 1: // REANUDAR PARTIDA
                        if (partidaEnCurso) {
                            estadoActual = Estado::Tablero;
                            motor.setEstado(Estado::Tablero);
                        }
                        break;

                    case 2: {
                        estadoAnterior = Estado::MenuPrincipal;
                          estadoActual = Estado::Instrucciones; 
                          break;
                    }
                    case 3: {
                        estadoAnterior = Estado::MenuPrincipal;
                        estadoActual = Estado::Creditos; 
                        break;
                    }

                    case 4: // SALIR DEL JUEGO
                        window.close();
                        break;

                    case 5: // CARGAR PARTIDA (Lleva al menú de ranuras)
                        modoGuardar = false;
                        pantallaCarga->actualizarTextosRanuras(ranuras[0].ocupada, ranuras[1].ocupada, ranuras[2].ocupada);
                        estadoActual = Estado::SeleccionCarga;
                        break;
                    }
                }
            }
        }

        //MENÚ DE PAUSA
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
                    case 0: estadoActual = estadoAnterior; break; // REANUDAR
                    case 1: reiniciarPartida(); estadoActual = Estado::Tablero; break; // REINICIAR
                    case 2: estadoActual = Estado::MenuPrincipal; break; // VOLVER AL MENU
                    case 3: { 
                        estadoAnterior = Estado::Pausa;
                        estadoActual = Estado::Instrucciones; break; 
                    } // INSTRUCCIONES

                    case 4: // GUARDAR PARTIDA (Lleva al menú de ranuras)
                        modoGuardar = true;
                        pantallaCarga->actualizarTextosRanuras(ranuras[0].ocupada, ranuras[1].ocupada, ranuras[2].ocupada);
                        estadoActual = Estado::SeleccionCarga;
                        break;

                    case 5: // SALIR AL ESCRITORIO
                        window.close();
                        break;
                    }
                }
            }
        }
        //MENU DE CARGA
        else if (estadoActual == Estado::SeleccionCarga) {
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
                    if (ranura == 3) { // Opción "VOLVER"
                        
                        estadoActual = modoGuardar ? Estado::Pausa : Estado::MenuPrincipal;
                    }
                    else {
                        if (modoGuardar) {
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
        else if (estadoActual == Estado::Victoria) {
            if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Enter) {
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

    // SI ES PAUSA, dibujamos el menú de pausa al final del todo (encima de todo)
    if (estadoActual == Estado::Pausa) {
        menuPausa->dibujar(window);
    }
    else if (estadoActual == Estado::Victoria) {
        window.setView(vistaUI);
        pantallainfo.dibujarPantallaVictoria(window);
    }
    // 5. PANTALLA INSTRUCCIONES
    else if (estadoActual == Estado::Instrucciones) {
        window.setView(vistaUI);
        pantallainfo.dibujarPantallaInstrucciones(window);
    }
    //6. menu cargar ranuras
    else if (estadoActual == Estado::SeleccionCarga) {
        pantallaInicio.dibujar(window); // Dibujamos el fondo del marine y el tiranido
        pantallaCarga->dibujar(window); // Dibujamos las ranuras encima
    }
    // 7. PANTALLA CRÉDITOS
    else if (estadoActual == Estado::Creditos) {
        window.setView(vistaUI);
        pantallainfo.dibujarPantallaCreditos(window);
    }

    window.display();
}

void Coordinador::actualizar(float dt) {
    //  EL TEMPORIZADOR GLOBAL 
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
    else if (motor.getEstado() == Estado::Victoria && estadoActual != Estado::Victoria) {
        estadoActual = Estado::Victoria;
        int ganador = motor.getGanador();
        // Pasamos las puntuaciones y el tiempo
        pantallainfo.configurarPantallaVictoria(ganador, motor.getPuntosLuz(), motor.getPuntosOscuridad(), motor.getTiempoJugado(), window);
    }
    // CORRECCIÓN: Ahora el motor SOLO se actualiza si NO estamos en pausa
    if (estadoActual != Estado::Pausa) {
        motor.actualizar(dt);
    }
}

void Coordinador::reiniciarPartida() {
    motor.limpiarDatos();
    std::cout << "DEBUG: Datos del motor limpiados y unidades desplegadas." << std::endl;
}

void Coordinador::guardarEnRanura(int indice) {
    // 1. Si la ranura ya tenía una partida vieja, limpiamos su memoria para no saturar la RAM
    for (Pieza* p : ranuras[indice].piezas) {
        delete p;
    }
    ranuras[indice].piezas.clear();

    // 2. Extraemos las piezas actuales del tablero
    std::vector<Pieza*> piezasActuales = motor.getListaPiezas();

    // 3. CLONACIÓN: Recorremos cada pieza y creamos una copia exacta en la ranura
    for (Pieza* p : piezasActuales) {
        ranuras[indice].piezas.push_back(p->clonar());
    }

    ranuras[indice].ronda = motor.getRondaActual();
    ranuras[indice].ciclo = motor.getCicloActual();
    ranuras[indice].jugador = motor.getJugadorActual();

    //  Guardamos en las partidas actuales puntos y tiempo
    ranuras[indice].puntosLuz = motor.getPuntosLuz();
    ranuras[indice].puntosOscuridad = motor.getPuntosOscuridad();
    ranuras[indice].tiempoJugado = motor.getTiempoJugado();
    // 5. Sloth ocupado
    ranuras[indice].ocupada = true;
    std::cout << " Partida guardada con exito en la ranura " << indice + 1 << "!" << std::endl;

    guardarDatosEnFichero();
}

void Coordinador::cargarDesdeRanura(int indice) {
    if (ranuras[indice].ocupada) {
        // 1. Vaciamos el tablero actual usando tu función
        motor.limpiarDatos();

        // 2. CLONACIÓN INVERSA: Copiamos las piezas desde la ranura para enviarlas al motor
        // (Debemos clonarlas de nuevo, o la ranura se quedaría vacía tras jugar)
        std::vector<Pieza*> piezasCargadas;
        for (Pieza* p : ranuras[indice].piezas) {
            piezasCargadas.push_back(p->clonar());
        }

        // 3. Inyectamos los clones en el Motor
        motor.setListaPiezas(piezasCargadas);

        motor.setRondaActual(ranuras[indice].ronda);
        motor.setCicloActual(ranuras[indice].ciclo);
        motor.setJugadorActual(ranuras[indice].jugador);

        //4: Restauramos puntos y tiempo de las partidas guardadas
        motor.setPuntosLuz(ranuras[indice].puntosLuz);
        motor.setPuntosOscuridad(ranuras[indice].puntosOscuridad);
        motor.setTiempoJugado(ranuras[indice].tiempoJugado);
        // 5. Cambiamos los estados para reanudar el juego
        estadoActual = Estado::Tablero;
        motor.setEstado(Estado::Tablero);
        std::cout << "Partida cargada desde la ranura " << indice + 1 << "!" << std::endl;
    }
    else {
        std::cout << "La ranura " << indice + 1 << " esta vacia." << std::endl;
    }
}

// FUNCION QUE LEE EL ARCHIVO
void Coordinador::cargarDatosDeFichero() {
    std::ifstream archivo("partidas_guardadas.txt");

    // Si el archivo no existe (primera vez que juegas), no pasa nada
    if (!archivo.is_open()) {
        std::cout << "Aviso: NO HAY ARCHIVO GUARDADO PREVIO. Se creara uno nuevo al jugar." << std::endl;
        return;
    }

    for (int i = 0; i < 3; i++) {
        archivo >> ranuras[i].ocupada;
        if (ranuras[i].ocupada) {
            //variables globales de la partida
            archivo >> ranuras[i].ronda
                >> ranuras[i].ciclo
                >> ranuras[i].jugador
                >> ranuras[i].puntosLuz
                >> ranuras[i].puntosOscuridad
                >> ranuras[i].tiempoJugado;

            int numPiezas;
            archivo >> numPiezas;

            // Limpiamos la ranura por si acaso había basura en memoria
            for (Pieza* p : ranuras[i].piezas) delete p;
            ranuras[i].piezas.clear();

            // Reconstruimos pieza a pieza
            for (int j = 0; j < numPiezas; j++) {
                int bandoInt, x, y;
                std::string nombre;
                float vida;
                archivo >> bandoInt >> x >> y >> nombre >> vida;

                Bando b = static_cast<Bando>(bandoInt);
                sf::Vector2i pos(x, y);

                // se usa el motor temporalmente para crear la pieza correcta
                motor.limpiarDatos();
                Generador::AnadirUnidad(motor, b, nombre, pos);

                if (!motor.getListaPiezas().empty()) {
                    Pieza* p = motor.getListaPiezas().back();
                    p->stats.vida = vida; // Le ponemos la salud que tenía al guardar
                    ranuras[i].piezas.push_back(p->clonar()); // La clonamos a la ranura segura
                }
            }
            motor.limpiarDatos(); // Dejamos el motor limpio
        }
    }
    archivo.close();
    std::cout << "Datos cargados desde partidas_guardadas.txt" << std::endl;
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

            // Guardar cuántas piezas hay vivas
            archivo << ranuras[i].piezas.size() << " ";

            // Guardamos los datos de cada pieza viva
            for (Pieza* p : ranuras[i].piezas) {
                archivo << static_cast<int>(p->getBando()) << " "
                    << p->getPosicionTablero().x << " "
                    << p->getPosicionTablero().y << " "
                    << p->stats.nombre << " "
                    << p->stats.vida << " ";
            }
        }
        archivo << "\n";
    }
    archivo.close();
}
