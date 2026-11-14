#include "DungeonAdventures.h"

// Funcion principal: prepara los datos y ejecuta el menu del juego.
int main() {
    
    // Declara al jugador y las estructuras del juego.
    struct Jugador heroe;
    Inventario miInventario = NULL;
    PilaHistorial historial = NULL;
    ColaEventos colaCalabozo;

    // Inicializa la cola y los datos del jugador.
    inicializarCola(&colaCalabozo);
    inicializarJugador(&heroe);

    // Agrega los eventos que apareceran durante la aventura.
    encolarEvento(&colaCalabozo, "Cofre de Oro");
    encolarEvento(&colaCalabozo, "Trampa de Monstruo");
    encolarEvento(&colaCalabozo, "Fuente Curativa");
    encolarEvento(&colaCalabozo, "Guardian Final");

    // Agrega el inicio de la aventura al diario.
    pushHistorial(
        &historial,
        "Has cruzado las puertas del calabozo. La expedicion comienza."
    );

    int opcion = 0;
    int juegoTerminado = 0;

    // Repite el menu hasta que el jugador salga o termine la partida.
    do {
        printf("\n========================================\n");
        printf("       DUNGEON ADVENTURES       \n");
        printf("========================================\n");
        printf("1. Avanzar por el calabozo \n");
        printf("2. Revisar tu equipo y estado \n");
        printf("3. Consultar el diario de aventura \n");
        printf("4. Abandonar la expedicion\n");
        printf("Elige tu siguiente movimiento: ");

        // Revisa que la opcion ingresada sea un numero.
        if (scanf("%d", &opcion) != 1) {
            while (getchar() != '\n');
            continue;
        }

        // Limpia lo que quede en la linea de entrada.
        while (getchar() != '\n');

        // Ejecuta la opcion que eligio el jugador.
        switch (opcion) {
        case 1:
            // Procesa el siguiente evento del calabozo.
            procesarSiguienteEvento(
                &heroe,
                &miInventario,
                &historial,
                &colaCalabozo,
                &juegoTerminado
            );

            // Sale del menu si el jugador murio o el juego termino.
            if (heroe.vida <= 0 || juegoTerminado) {
                opcion = 4;
            }
            break;

        case 2:
            // Muestra el estado del jugador y sus objetos.
            mostrarEstado(heroe, miInventario);
            break;

        case 3:
            // Muestra los eventos guardados en el diario.
            mostrarHistorial(historial);
            break;

        case 4:
            // Avisa si el jugador abandona antes de terminar.
            if (!juegoTerminado && heroe.vida > 0) {
                printf("\nGuardando tu expedicion y saliendo del calabozo...\n");
            }
            break;

        default:
            // Avisa si la opcion no existe en el menu.
            printf("\nEsa ruta no existe. Elige una opcion del mapa.\n");
        }
    } while (opcion != 4);

    // Muestra el mensaje final segun el resultado de la aventura.
    if (heroe.vida <= 0) {
        printf("\n==========================================\n");
        printf("     El calabozo te ha vencido. Prepara otra expedicion.\n");
        printf("============================================\n");
    }
    else if (juegoTerminado) {
        printf("\n====================================================\n");
        printf("   Victoria! El guardian ha caido y tu leyenda comienza.\n");
        printf("======================================================\n");
    }

    // Espera que el jugador presione ENTER antes de cerrar.
    printf("\nPulsa ENTER para cerrar tu diario de aventura.");
    getchar();

    // Libera la memoria usada por las listas y la cola.
    liberarMemoria(&miInventario, &historial, &colaCalabozo);

    return 0;
}
