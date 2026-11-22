#include <iostream>
#include <string>
#include <fstream>

#include "modelos.h"
#include "estructuras.h"
using namespace std;

class ColaDePrioridad
{
    enum prioridad
    {
        nula, alta, media, baja
    };
    struct Nodo {
        Bicicleta * bici;
        int prioridad = nula;
        Nodo * siguiente = nullptr;
        Nodo * anterior = nullptr;

        Nodo(Bicicleta* nuevaBici)
        {

            if (nuevaBici->getTipo() == "Electrica")
            {
                prioridad = alta;
            }
            else if (nuevaBici->getEstado() == "Averiada")
            {
                prioridad = media;
            }
            else
            {
                prioridad = baja;
            }
        };
    };

    Nodo * primero = nullptr;
    Nodo * ultimo = nullptr;

    void insertarRecorriendo(Nodo * nuevoNodo)
    {
        //complejidad O(n)
        if (nuevoNodo->prioridad < primero->prioridad)
        {
            nuevoNodo->siguiente = primero;
            primero->anterior = nuevoNodo;
            primero = nuevoNodo;
        }
        else if (nuevoNodo->prioridad >= ultimo->prioridad)
        {
            nuevoNodo->anterior = ultimo;
            ultimo->siguiente = nuevoNodo;
            ultimo = nuevoNodo;
        }
        else
        {
            Nodo * nodoActual = primero->siguiente;
            while(nuevoNodo->prioridad >= nodoActual->prioridad)
            {
                nodoActual = nodoActual->siguiente;
            }

            nuevoNodo->siguiente = nodoActual->siguiente;
            nuevoNodo->anterior = nodoActual;

            nodoActual->siguiente->anterior = nuevoNodo;
            nodoActual->siguiente = nuevoNodo;
        }
    }


    
    public:

        void insertar(Bicicleta * bicicleta)
        {
            Nodo * aux = new Nodo(bicicleta);

            if (primero == nullptr)
            {
                primero = aux;
                ultimo = aux;
            }
            else
            {
                insertarRecorriendo(aux);
            }
            
        }

        Bicicleta * extraer()
        {
            Bicicleta * bicicleta = primero->bici;
            Nodo * aux = primero;

            primero = aux->siguiente;

            aux->siguiente = nullptr;
            aux->anterior = nullptr;
            delete aux;


            return bicicleta;
        }

};