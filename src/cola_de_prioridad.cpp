#include <iostream>
#include <string>
#include "modelos.h"

using namespace std;

class ColaDePrioridad
{
    enum prioridad
    {
        nula, alta, media, baja
    };

    struct Nodo
    {
        Bicicleta* bici;
        int prioridad = nula;
        Nodo* siguiente = nullptr;
        Nodo* anterior = nullptr;

        Nodo(Bicicleta* nuevaBici) : bici(nuevaBici)
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
        }
    };

    Nodo* primero = nullptr;
    Nodo* ultimo = nullptr;
    int cantidad = 0;

    void insertarRecorriendo(Nodo* nuevoNodo)
    {
        // Complejidad O(n): se recorre desde el extremo mas cercano.
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
            Nodo* nodoActual = primero;

            while (nodoActual != nullptr
                   && nodoActual->prioridad <= nuevoNodo->prioridad)
            {
                nodoActual = nodoActual->siguiente;
            }

            nuevoNodo->siguiente = nodoActual;
            nuevoNodo->anterior = nodoActual->anterior;

            nodoActual->anterior->siguiente = nuevoNodo;
            nodoActual->anterior = nuevoNodo;
        }
    }

public:
    ~ColaDePrioridad()
    {
        while (primero != nullptr)
        {
            Nodo* aux = primero;
            primero = aux->siguiente;

            // La cola no es duena de las bicicletas: pertenecen al arbol.
            delete aux;
        }

        ultimo = nullptr;
        cantidad = 0;
    }

    bool estaVacia() const
    {
        return primero == nullptr;
    }

    int obtenerCantidad() const
    {
        return cantidad;
    }

    bool contieneBicicleta(int biciId) const
    {
        const Nodo* nodoActual = primero;

        while (nodoActual != nullptr)
        {
            if (nodoActual->bici->getBiciId() == biciId)
            {
                return true;
            }

            nodoActual = nodoActual->siguiente;
        }

        return false;
    }

    void insertar(Bicicleta* bicicleta)
    {
        Nodo* aux = new Nodo(bicicleta);

        if (primero == nullptr)
        {
            primero = aux;
            ultimo = aux;
        }
        else
        {
            insertarRecorriendo(aux);
        }

        cantidad++;
    }

    Bicicleta* extraerMasUrgente()
    {
        if (primero == nullptr)
        {
            return nullptr;
        }

        Bicicleta* bicicleta = primero->bici;
        Nodo* aux = primero;

        primero = aux->siguiente;

        if (primero != nullptr)
        {
            primero->anterior = nullptr;
        }
        else
        {
            ultimo = nullptr;
        }

        aux->siguiente = nullptr;
        aux->anterior = nullptr;
        delete aux;
        cantidad--;

        return bicicleta;
    }
};
