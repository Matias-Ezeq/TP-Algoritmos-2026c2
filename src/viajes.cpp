#include <iostream>
#include <string>
#include <fstream>
#include "modelos.h"
using namespace std;
class ListaDeViajes
{
    struct Nodo {
        Viaje viaje;
        Nodo * siguiente = nullptr;
        Nodo * anterior = nullptr;
        explicit Nodo(const Viaje& v) : viaje(v) {}
    };
    
    Nodo * primero = nullptr;
    Nodo * ultimo = nullptr;

    public:
    bool vacia() const { return primero == nullptr;};
    const Nodo * getPrimero() const { return primero;};
    const Nodo * getUltimo() const { return ultimo;};    
    
    void push(const Viaje& viaje)
    {
        Nodo* aux = new Nodo(viaje);

        if (vacia())
        {
            primero = aux;
            ultimo = aux;
        }
        else if (viaje.fecha <= primero->viaje.fecha)
        {
            aux->siguiente = primero;
            primero->anterior = aux;
            primero = aux;
        }
        else if (viaje.fecha >= ultimo->viaje.fecha)
        {
            aux->anterior = ultimo;
            ultimo->siguiente = aux;
            ultimo = aux;
        }
        else
        {
            Nodo* nodoActual = primero->siguiente;

            while (nodoActual != nullptr
                   && nodoActual->viaje.fecha < viaje.fecha)
            {
                nodoActual = nodoActual->siguiente;
            }

            aux->anterior = nodoActual->anterior;
            aux->siguiente = nodoActual;
            nodoActual->anterior->siguiente = aux;
            nodoActual->anterior = aux;
        }
    }

   ~ListaDeViajes() {
    for (Nodo* n = primero; n; ) { Nodo* sig = n->siguiente; delete n; n = sig; }
} 
};

int cantidadDeViajesEntreFechaYFecha(ListaDeViajes* lista, int fecha1, int fecha2, string estacion){
    int cantidadDeViajes=0;

    if (fecha1 > fecha2)
    {
        int auxiliar = fecha1;
        fecha1 = fecha2;
        fecha2 = auxiliar;
    }

    auto* nodoActual = lista->getPrimero();

    while (nodoActual != nullptr)
    {
        if (nodoActual->viaje.estacionOrigen == estacion
            && nodoActual->viaje.fecha >= fecha1
            && nodoActual->viaje.fecha <= fecha2)
        {
            cantidadDeViajes++;
        }

        nodoActual = nodoActual->siguiente;
    }

    return cantidadDeViajes;
}

int minutosPedaleadosPorUsuario(ListaDeViajes* lista, int dniUsuario)
{
    int minutosPedaleados = 0;
    auto* nodoActual = lista->getPrimero();

    while (nodoActual != nullptr)
    {
        if (nodoActual->viaje.dniUsuario == dniUsuario)
        {
            minutosPedaleados += nodoActual->viaje.duracionMin;
        }

        nodoActual = nodoActual->siguiente;
    }

    return minutosPedaleados;
}


bool llenarConDatos(ListaDeViajes* lista)
{
    string estacionOrigen = "", estacionDestino = "";
    int viajeId = 0, biciId = 0, dniUsuario = 0, fecha = 0, duracionMin = 0;
    fstream datosDeViaje;
    datosDeViaje.open("./src/datos de entrada/viajes.txt");

    if (!datosDeViaje.is_open())
    {
        return false;
    }

    while (datosDeViaje >> viajeId >> biciId >> dniUsuario >> estacionOrigen >> estacionDestino >> fecha >> duracionMin)
    {
        Viaje aux;
        aux.id = viajeId;
        aux.biciId = biciId;
        aux.dniUsuario = dniUsuario;
        aux.estacionOrigen = estacionOrigen;
        aux.estacionDestino = estacionDestino;
        aux.fecha = fecha;
        aux.duracionMin = duracionMin;

        lista->push(aux);
    }

    datosDeViaje.close();
    return true;
}

class ColaDeUsuarios {
    private:
        struct Nodo
        {
            int dni;
            Nodo* siguiente = nullptr;
        };

        Nodo* primero = nullptr;
        Nodo* ultimo = nullptr;

    public:
        ~ColaDeUsuarios()
        {
            while (primero != nullptr)
            {
                Nodo* aux = primero;
                primero = primero->siguiente;
                delete aux;
            }

            ultimo = nullptr;
        }

        bool vacia() const
        {
            return primero == nullptr;
        }

        void push(int dni)
        {
            Nodo* aux = new Nodo;
            aux -> dni = dni;
            aux -> siguiente = nullptr;

            if (ultimo == nullptr)
            {
                primero = aux;
                ultimo = aux;
            }
            else
            {
                ultimo -> siguiente = aux;
                ultimo = aux;
            }
        }

        int pop()
        {
            // Devuelve -1 si la cola esta vacia; conviene consultar vacia() antes.
            if (primero == nullptr)
            {
                return -1;
            }

            Nodo* aux = primero;
            primero = primero->siguiente;
            int dni = aux->dni;
            aux->siguiente = nullptr;
            delete aux;

            if (primero == nullptr)
            {
                ultimo = nullptr;
            }

            return dni;
        }
};