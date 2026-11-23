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
        else if (viaje.fecha <= primero->dato->fecha)
        {
            aux->siguiente = primero;
            primero->anterior = aux;
            primero = aux;
        }
        else if (viaje.fecha >= ultimo->dato->fecha)
        {
            aux->anterior = ultimo;
            ultimo->siguiente = aux;
            ultimo = aux;
        }
        else
        {
            int fechaPrimero = primero -> dato -> fecha;
            int fechaUltimo = ultimo -> dato -> fecha;
            if (viaje->fecha <= fechaPrimero)
            {
                aux -> siguiente = this -> primero;
                this -> primero -> anterior = aux;
                this -> primero = aux;
            }
            else if (viaje->fecha >= fechaUltimo)
            {
                aux -> anterior = this -> ultimo;
                this -> ultimo -> siguiente = aux;
                this -> ultimo = aux;
            }
            //si la fecha es posterior a la del primero y anterior al del ultimo, compara para ver desde que lado iterar para asegurar orden por fecha.
            else {
                int difPrimero = viaje->fecha - fechaPrimero;
                int difUltimo = fechaUltimo - viaje->fecha;
                NodoListaDobleEnlazada<Viaje> * nodoActual = nullptr;

                if(difPrimero <= difUltimo)
                {
                    nodoActual = this -> primero -> siguiente;
                    while(nodoActual -> dato->fecha <= viaje->fecha)
                    {
                        nodoActual = nodoActual -> siguiente;
                    }
                    nodoActual = nodoActual -> anterior;

                }
                else
                {
                    nodoActual = this -> ultimo -> anterior;
                    while(nodoActual -> dato->fecha >= viaje->fecha)
                    {
                        nodoActual = nodoActual->anterior;
                    }
                }
                aux -> siguiente = nodoActual -> siguiente;
                aux -> anterior = nodoActual;
                nodoActual -> siguiente = aux;
                aux -> siguiente -> anterior = aux;
            }

            aux->anterior = nodoActual->anterior;
            aux->siguiente = nodoActual;
            nodoActual->anterior->siguiente = aux;
            nodoActual->anterior = aux;
        }
    }

   ~ListaDeViajes() {
    for (NodoListaDobleEnlazada<Viaje>* n = primero; n; )
    {
        NodoListaDobleEnlazada<Viaje>* sig = n->siguiente; 
        delete n->dato;
        delete n; 
        n = sig;
    }
} 
};

int cantidadDeViajesEntreFechaYFecha(ListaDeViajes* lista, int fecha1, int fecha2,string estacion){
    const auto* primero = lista->getPrimero();
    const auto* ultimo = lista->getUltimo();
    int cantidadDeViajes=0;

    if (fecha1 > fecha2)
    {
        int diferenciaInicio = fecha1 - primero->dato->fecha;
        int diferenciaFinal = ultimo->dato->fecha - fecha2;

        auto* nodoActual = primero;
        if(diferenciaInicio<=diferenciaFinal)
        {
            while(nodoActual != nullptr && nodoActual->dato->fecha <= fecha2)
            {
                if(nodoActual ->dato->estacion_origen == estacion && nodoActual->dato->fecha >= fecha1)
                {
                    cantidadDeViajes += 1;
                }
                nodoActual = nodoActual->siguiente;
            }
        }
        else
        {
            nodoActual = ultimo;
            while(nodoActual!= nullptr && nodoActual->dato->fecha >= fecha1)
            {
                if(nodoActual ->dato->estacion_origen == estacion && nodoActual->dato->fecha <= fecha2)
                {
                    cantidadDeViajes += 1;
                }
                nodoActual = nodoActual->anterior;
            }
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
        if (nodoActual->dato->dni_usuario == dniUsuario)
        {
            minutosPedaleados += nodoActual->dato->duracion_min;
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
        Viaje * aux;
        aux->id = viaje_id;
        aux->bici_id = bici_id;
        aux->dni_usuario = dni_usuario;
        aux->estacion_origen = estacion_origen;
        aux->estacion_destino = estacion_destino;
        aux->fecha = fecha;
        aux->duracion_min = duracion_min;

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
