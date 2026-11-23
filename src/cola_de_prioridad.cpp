#include <iostream>
#include <string>
#include "modelos.h"

using namespace std;

class ColaDePrioridad
{
private:
    Bicicleta** heap;
    int cantidad;
    int capacidad;

    int obtenerPrioridad(const Bicicleta* bicicleta) const
    {
        if (bicicleta->getTipo() == "Electrica")
        {
            return 1;
        }

        if (bicicleta->getEstado() == "Averiada")
        {
            return 2;
        }

        return 3;
    }

    void subir(int indice)
    {
        while (indice > 0)
        {
            int padre = (indice - 1) / 2;

            if (obtenerPrioridad(heap[indice])
                >= obtenerPrioridad(heap[padre]))
            {
                break;
            }

            Bicicleta* auxiliar = heap[indice];
            heap[indice] = heap[padre];
            heap[padre] = auxiliar;
            indice = padre;
        }
    }

    void bajar(int indice)
    {
        while (true)
        {
            int hijoIzquierdo = indice * 2 + 1;

            if (hijoIzquierdo >= cantidad)
            {
                break;
            }

            int hijoDerecho = hijoIzquierdo + 1;
            int hijoMenor = hijoIzquierdo;

            if (hijoDerecho < cantidad
                && obtenerPrioridad(heap[hijoDerecho])
                   < obtenerPrioridad(heap[hijoMenor]))
            {
                hijoMenor = hijoDerecho;
            }

            if (obtenerPrioridad(heap[indice])
                <= obtenerPrioridad(heap[hijoMenor]))
            {
                break;
            }

            Bicicleta* auxiliar = heap[indice];
            heap[indice] = heap[hijoMenor];
            heap[hijoMenor] = auxiliar;
            indice = hijoMenor;
        }
    }

    void ampliar()
    {
        int nuevaCapacidad;

        if (capacidad == 0)
        {
            nuevaCapacidad = 4;
        }
        else
        {
            nuevaCapacidad = capacidad * 2;
        }

        Bicicleta** nuevoHeap = new Bicicleta*[nuevaCapacidad];

        for (int i = 0; i < cantidad; i++)
        {
            nuevoHeap[i] = heap[i];
        }

        delete[] heap;
        heap = nuevoHeap;
        capacidad = nuevaCapacidad;
    }

public:
    ColaDePrioridad()
        : heap(nullptr), cantidad(0), capacidad(0)
    {
    }

    ~ColaDePrioridad()
    {
        delete[] heap;
    }

    bool estaVacia() const
    {
        return cantidad == 0;
    }

    int obtenerCantidad() const
    {
        return cantidad;
    }

    bool contieneBicicleta(int biciId) const
    {
        for (int i = 0; i < cantidad; i++)
        {
            if (heap[i]->getBiciId() == biciId)
            {
                return true;
            }
        }

        return false;
    }

    void insertar(Bicicleta* bicicleta)
    {
        if (cantidad == capacidad)
        {
            ampliar();
        }

        heap[cantidad] = bicicleta;
        subir(cantidad);
        cantidad++;
    }

    Bicicleta* extraerMasUrgente()
    {
        if (estaVacia())
        {
            return nullptr;
        }

        Bicicleta* masUrgente = heap[0];
        cantidad--;
        heap[0] = heap[cantidad];
        bajar(0);

        return masUrgente;
    }
};
