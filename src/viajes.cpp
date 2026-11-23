#include <iostream>
#include <string>
#include <fstream>
#include "modelos.h"
#include "estructuras.h"


using namespace std;



//Lista Ordenada doblemente enlazada para permitir busquedas e inserciones ordenadas desde cualquier extremo. Complejidad O(N) de busqueda y O(log N) de inserción ya que elige el extremo con menor diferencia respecto a la fecha del viaje a insertar.


class ListaDeViajes: public ListaDobleEnlazada<Viaje>
{
    public:
    void push(const Viaje& viaje)
    {
        NodoListaDobleEnlazada<Viaje>* nuevoNodo =
            new NodoListaDobleEnlazada<Viaje>(new Viaje(viaje));

        if (this->vacia())
        {
            this->primero = nuevoNodo;
            this->ultimo = nuevoNodo;
            return;
        }

        if (viaje.fecha <= this->primero->dato->fecha)
        {
            nuevoNodo->siguiente = this->primero;
            this->primero->anterior = nuevoNodo;
            this->primero = nuevoNodo;
            return;
        }

        if (viaje.fecha >= this->ultimo->dato->fecha)
        {
            nuevoNodo->anterior = this->ultimo;
            this->ultimo->siguiente = nuevoNodo;
            this->ultimo = nuevoNodo;
            return;
        }

        int fechaPrimero = this->primero->dato->fecha;
        int fechaUltimo = this->ultimo->dato->fecha;
        NodoListaDobleEnlazada<Viaje>* nodoActual = nullptr;

        // Se recorre desde el extremo cuya fecha esta mas cerca de la del
        // viaje nuevo. nodoActual queda en el primer nodo con fecha mayor.
        if (viaje.fecha - fechaPrimero <= fechaUltimo - viaje.fecha)
        {
            nodoActual = this->primero->siguiente;

            while (nodoActual->dato->fecha <= viaje.fecha)
            {
                nodoActual = nodoActual->siguiente;
            }
        }
        else
        {
            nodoActual = this->ultimo;

            while (nodoActual->anterior != nullptr
                   && nodoActual->anterior->dato->fecha > viaje.fecha)
            {
                nodoActual = nodoActual->anterior;
            }
        }

        // Se conserva el orden por fecha; los viajes con la misma fecha
        // quedan en el orden en que fueron cargados.
        nuevoNodo->anterior = nodoActual->anterior;
        nuevoNodo->siguiente = nodoActual;
        nodoActual->anterior->siguiente = nuevoNodo;
        nodoActual->anterior = nuevoNodo;
    }
};

int cantidadDeViajesEntreFechaYFecha(
    ListaDeViajes* lista,
    int fecha1,
    int fecha2,
    string estacion
)
{
    if (fecha1 > fecha2)
    {
        int auxiliar = fecha1;
        fecha1 = fecha2;
        fecha2 = auxiliar;
    }

    int cantidadDeViajes = 0;
    const NodoListaDobleEnlazada<Viaje>* nodoActual = lista->getPrimero();

    // La lista esta ordenada por fecha, pero se recorre desde el inicio
    // porque los viajes con la misma fecha pueden estar intercalados por
    // estacion de origen.
    while (nodoActual != nullptr && nodoActual->dato->fecha <= fecha2)
    {
        if (nodoActual->dato->fecha >= fecha1
            && nodoActual->dato->estacionOrigen == estacion)
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
    const NodoListaDobleEnlazada<Viaje>* nodoActual = lista->getPrimero();

    while (nodoActual != nullptr)
    {
        if (nodoActual->dato->dniUsuario == dniUsuario)
        {
            minutosPedaleados += nodoActual->dato->duracionMin;
        }

        nodoActual = nodoActual->siguiente;
    }

    return minutosPedaleados;
}


bool llenarConDatos(ListaDeViajes* lista)
{
    string estacionOrigen = "";
    string estacionDestino = "";
    int viajeId = 0;
    int biciId = 0;
    int dniUsuario = 0;
    int fecha = 0;
    int duracionMin = 0;

    ifstream datosDeViaje("./src/datos de entrada/viajes.txt");

    if (!datosDeViaje.is_open())
    {
        return false;
    }

    while (datosDeViaje >> viajeId >> biciId >> dniUsuario
           >> estacionOrigen >> estacionDestino >> fecha >> duracionMin)
    {
        Viaje viaje;
        viaje.id = viajeId;
        viaje.biciId = biciId;
        viaje.dniUsuario = dniUsuario;
        viaje.estacionOrigen = estacionOrigen;
        viaje.estacionDestino = estacionDestino;
        viaje.fecha = fecha;
        viaje.duracionMin = duracionMin;

        lista->push(viaje);
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
