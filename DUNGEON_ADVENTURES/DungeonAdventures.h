#ifndef DUNGEON_ADVENTURES_H
#define DUNGEON_ADVENTURES_H

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define una pausa en milisegundos para Windows y otros sistemas.
#ifdef _WIN32
#include <windows.h>
#define sleep_ms(ms) Sleep(ms)
#else
#include <unistd.h>
#define sleep_ms(ms) usleep((ms) * 1000)
#endif

// Guarda los datos principales del jugador.
struct Jugador {
    char nombre[30];
    int vida;
    int oro;
};

// Nodo de la lista enlazada del inventario.
struct NodoInventario {
    char nombreItem[30];
    struct NodoInventario* siguiente;
};

// Nodo de la pila que guarda el historial de eventos.
struct NodoHistorial {
    char descripcion[80];
    struct NodoHistorial* siguiente;
};

// Nodo de la cola que guarda los eventos pendientes.
struct NodoColaEvento {
    char tipoEvento[30];
    struct NodoColaEvento* siguiente;
};

// Tipos para trabajar mas facilmente con las listas.
typedef struct NodoInventario* Inventario;
typedef struct NodoHistorial* PilaHistorial;

// Guarda el primer y el ultimo evento de la cola.
typedef struct {
    struct NodoColaEvento* frente;
    struct NodoColaEvento* final;
} ColaEventos;

// Declaraciones de las funciones del juego.
void inicializarJugador(struct Jugador* j);
void mostrarEstado(struct Jugador j, Inventario inv);
void agregarItemInventario(Inventario* inv, const char* item);
void mostrarInventario(Inventario inv);
void pushHistorial(PilaHistorial* pila, const char* texto);
void mostrarHistorial(PilaHistorial pila);
void inicializarCola(ColaEventos* c);
void encolarEvento(ColaEventos* c, const char* tipo);
char* desencolarEvento(ColaEventos* c);
int colaVacia(ColaEventos c);
void liberarMemoria(Inventario* inv, PilaHistorial* pila, ColaEventos* c);
void animacionCaminando();
void animacionCombate();
void procesarSiguienteEvento(
    struct Jugador* heroe,
    Inventario* inv,
    PilaHistorial* pila,
    ColaEventos* c,
    int* juegoTerminado
);

#endif
