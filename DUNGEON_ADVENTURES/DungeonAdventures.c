#include "DungeonAdventures.h"

// Muestra un mensaje y agrega tres puntos con pausas
// para simular que el personaje camina por el calabozo.
void animacionCaminando() {
    printf("\nAvanzas por un pasillo oscuro. Escucha cada paso");
    for (int i = 0; i < 3; i++) {
        sleep_ms(400);
        printf(".");
        fflush(stdout);
    }
    printf("\n");
}

// Muestra un mensaje y agrega signos con pausas
// para simular que el personaje se prepara para pelear.
void animacionCombate() {
    printf("\nEl monstruo se acerca. Prepara tu espada y tu escudo");
    for (int i = 0; i < 3; i++) {
        sleep_ms(300);
        printf(" !");
        fflush(stdout);
    }
    printf("\n");
}

// Procesa el siguiente evento de la cola.
// Puede cambiar la vida, el oro y el inventario del jugador.
// Tambien guarda el resultado del evento en el historial.
void procesarSiguienteEvento(
    struct Jugador* heroe,
    Inventario* inv,
    PilaHistorial* pila,
    ColaEventos* c,
    int* juegoTerminado
) {
    // Si no quedan eventos, termina la aventura.
    if (colaVacia(*c)) {
        printf("\nNo quedan caminos por explorar. Has recorrido todo el calabozo!\n");
        *juegoTerminado = 1;
        return;
    }

    animacionCaminando();

    // Saca el primer evento de la cola.
    char* eventoActual = desencolarEvento(c);

    if (eventoActual != NULL) {
        printf("\nUn nuevo desafio aparece ante ti: %s\n", eventoActual);

        char buffer[150];
        int decision = 0;

        // Evento del cofre de oro.
        if (strcmp(eventoActual, "Cofre de Oro") == 0) {
            printf("  1. Forzar el cofre con tus herramientas\n");
            printf("  2. Revisar el cofre en busca de trampas\n");
            printf("  Elige tu accion, aventurero: ");
            scanf("%d", &decision);
            while (getchar() != '\n');

            if (decision == 1) {
                heroe->oro += 20;
                sprintf(buffer, "Abriste el cofre: encontraste 20 monedas de oro.");
            }
            else {
                heroe->oro += 35;
                agregarItemInventario(inv, "Gema Brillante");
                sprintf(buffer, "Desarmaste la trampa: ganaste 35 monedas y una Gema Brillante.");
            }

            printf(" %s\n", buffer);
            pushHistorial(pila, buffer);
        }

        // Evento de encontrarse con un monstruo.
        else if (strcmp(eventoActual, "Trampa de Monstruo") == 0) {
            animacionCombate();
            printf("  1. Esquivar al monstruo y buscar una salida\n");
            printf("  2. Enfrentar al monstruo con tu arma\n");
            printf("  Elige tu accion, aventurero: ");
            scanf("%d", &decision);
            while (getchar() != '\n');

            if (decision == 1) {
                heroe->vida -= 20;
                sprintf(buffer, "Escapaste por poco, pero perdiste 20 puntos de vida.");
            }
            else {
                heroe->vida -= 35;
                sprintf(buffer, "El monstruo te golpeo con fuerza: perdiste 35 puntos de vida.");
            }

            printf(" %s\n", buffer);
            pushHistorial(pila, buffer);
        }

        // Evento de encontrar una fuente curativa.
        else if (strcmp(eventoActual, "Fuente Curativa") == 0) {
            printf("  1. Tomar un sorbo de la fuente\n");
            printf("  2. Beber hasta recuperar fuerzas\n");
            printf("  Elige tu accion, aventurero: ");
            scanf("%d", &decision);
            while (getchar() != '\n');

            if (decision == 1) {
                heroe->vida += 10;
                sprintf(buffer, "Un sorbo de la fuente te devuelve 10 puntos de vida.");
            }
            else {
                heroe->vida += 30;
                sprintf(buffer, "La fuente restaura 30 puntos de vida. Tus fuerzas regresan.");
            }

            // La vida del jugador no puede superar 100.
            if (heroe->vida > 100) heroe->vida = 100;

            printf(" %s\n", buffer);
            pushHistorial(pila, buffer);
        }

        // Evento de enfrentarse al guardian final.
        else if (strcmp(eventoActual, "Guardian Final") == 0) {
            animacionCombate();
            printf("  El guardian final bloquea tu camino. Esta es la batalla decisiva!\n");
            printf("  1. Luchar contra el guardian final\n");
            printf("  2. Intentar escapar del guardian\n");
            printf("  Elige tu accion, aventurero: ");
            scanf("%d", &decision);
            while (getchar() != '\n');

            if (decision == 1) {
                heroe->vida -= 75;

                // Si el jugador sigue vivo, gana la pelea.
                if (heroe->vida > 0) {
                    heroe->oro += 100;
                    agregarItemInventario(inv, "Amuleto del Guardian");
                    sprintf(buffer, "Venciste al guardian! +100 oro y obtuviste el Amuleto del Guardian.");
                    *juegoTerminado = 1;
                }
                else {
                    sprintf(buffer, "El guardian te derribo en combate. El calabozo guardara tu historia.");
                }
            }
            else {
                heroe->vida -= 100;
                sprintf(buffer, "Intentaste huir, pero el guardian te alcanzo. Perdiste 100 puntos de vida.");
            }

            printf(" %s\n", buffer);
            pushHistorial(pila, buffer);
        }

        // Libera el texto del evento que ya se proceso.
        free(eventoActual);
    }

    // Avisa si la vida del jugador llego a cero.
    if (heroe->vida <= 0) {
        printf("\nTu vida llego a cero. El calabozo ha vencido esta vez...\n");
    }
}

// Pide el nombre del aventurero y asigna sus valores iniciales.
void inicializarJugador(struct Jugador* j) {
    printf("\nEscribe el nombre de tu aventurero: ");

    // Si falla la lectura del nombre, usa "Heroe".
    if (scanf("%29s", j->nombre) != 1) {
        strcpy(j->nombre, "Heroe");
    }

    while (getchar() != '\n');

    // Asigna la vida y el oro con que empieza el jugador.
    j->vida = 100;
    j->oro = 10;

    printf("La expedicion comienza! Tienes 100 puntos de vida y 10 monedas de oro.\n");
}

// Muestra los datos del jugador y su inventario.
void mostrarEstado(struct Jugador j, Inventario inv) {
    printf("\n--- FICHA DEL AVENTURERO ---\n");
    printf(" Nombre: %s\n", j.nombre);
    printf(" Vida: %d / 100\n", j.vida);
    printf(" Oro: %d monedas\n", j.oro);
    mostrarInventario(inv);
    printf("-------------------------\n");
}

// Reserva memoria para un objeto y lo agrega al inicio del inventario.
void agregarItemInventario(Inventario* inv, const char* item) {
    Inventario nuevo = (Inventario)malloc(sizeof(struct NodoInventario));

    if (nuevo != NULL) {
        strcpy(nuevo->nombreItem, item);
        nuevo->siguiente = *inv;
        *inv = nuevo;
    }
}

// Recorre y muestra los objetos del inventario.
void mostrarInventario(Inventario inv) {
    printf("Inventario de la expedicion:\n");
    Inventario actual = inv;

    if (actual == NULL) {
        printf("No llevas objetos todavia. Sigue explorando el calabozo.\n");
        return;
    }

    while (actual != NULL) {
        printf("    - %s\n", actual->nombreItem);
        actual = actual->siguiente;
    }
}

// Agrega una nueva descripcion al inicio de la pila del historial.
void pushHistorial(PilaHistorial* pila, const char* texto) {
    PilaHistorial nuevo = (PilaHistorial)malloc(sizeof(struct NodoHistorial));

    if (nuevo != NULL) {
        strcpy(nuevo->descripcion, texto);
        nuevo->siguiente = *pila;
        *pila = nuevo;
    }
}

// Recorre y muestra los eventos guardados en el diario.
void mostrarHistorial(PilaHistorial pila) {
    printf("\n--- DIARIO DE AVENTURA ---\n");
    PilaHistorial actual = pila;

    if (actual == NULL) {
        printf("Tu diario esta en blanco. Aun no has vivido ningun encuentro.\n");
        return;
    }

    while (actual != NULL) {
        printf(" > %s\n", actual->descripcion);
        actual = actual->siguiente;
    }

    printf("--------------------------------------\n");
}

// Deja la cola sin eventos para comenzar a usarla.
void inicializarCola(ColaEventos* c) {
    c->frente = NULL;
    c->final = NULL;
}

// Devuelve 1 si la cola esta vacia y 0 si tiene eventos.
int colaVacia(ColaEventos c) {
    return c.frente == NULL;
}

// Agrega un nuevo evento al final de la cola.
void encolarEvento(ColaEventos* c, const char* tipo) {
    struct NodoColaEvento* nuevo =
        (struct NodoColaEvento*)malloc(sizeof(struct NodoColaEvento));

    if (nuevo != NULL) {
        strcpy(nuevo->tipoEvento, tipo);
        nuevo->siguiente = NULL;

        // Si la cola esta vacia, el nuevo evento tambien queda al frente.
        if (c->final == NULL) {
            c->frente = nuevo;
        }
        else {
            c->final->siguiente = nuevo;
        }

        c->final = nuevo;
    }
}

// Saca el primer evento de la cola y devuelve una copia de su nombre.
// La funcion que recibe ese texto debe liberarlo con free().
char* desencolarEvento(ColaEventos* c) {
    if (colaVacia(*c)) return NULL;

    struct NodoColaEvento* temp = c->frente;
    char* tipo = (char*)malloc(30);

    if (tipo != NULL) {
        strcpy(tipo, temp->tipoEvento);
    }

    c->frente = c->frente->siguiente;

    // Si ya no quedan eventos, la cola tampoco tiene nodo final.
    if (c->frente == NULL) {
        c->final = NULL;
    }

    free(temp);
    return tipo;
}

// Libera la memoria reservada para el inventario, el historial y la cola.
void liberarMemoria(Inventario* inv, PilaHistorial* pila, ColaEventos* c) {
    // Libera todos los nodos del inventario.
    if (inv != NULL) {
        while (*inv != NULL) {
            Inventario tempInv = *inv;
            *inv = (*inv)->siguiente;
            free(tempInv);
        }
    }

    // Libera todos los nodos del historial.
    if (pila != NULL) {
        while (*pila != NULL) {
            PilaHistorial tempP = *pila;
            *pila = (*pila)->siguiente;
            free(tempP);
        }
    }

    // Saca los eventos pendientes y libera su memoria.
    if (c != NULL) {
        while (!colaVacia(*c)) {
            char* t = desencolarEvento(c);
            if (t != NULL) free(t);
        }
    }
}