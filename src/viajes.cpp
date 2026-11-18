#include <iostream>
#include <string>
#include <fstream>
#include <bits/stdc++.h>
#include "viajes.h"
using namespace std;

int cantidadDeViajesEntreFechaYFecha(ListaDeViajes* lista, int fecha1, int fecha2, string estacion){
    const auto* primero = lista->getPrimero();
    const auto* ultimo = lista->getUltimo();
    int cantidadDeViajes=0;

    if (fecha2 > fecha1) swap(fecha1,fecha2);

    if (!lista->vacia())
    {
        int diferenciaInicio = fecha1 - primero->viaje.fecha;
        int diferenciaFinal = ultimo->viaje.fecha - fecha2;

        auto* nodoActual = primero;
        if(diferenciaInicio<=diferenciaFinal)
        {
            while(nodoActual != nullptr && nodoActual->viaje.fecha <= fecha2)
            {
                if(nodoActual->viaje.estacion_origen == estacion && nodoActual->viaje.fecha >= fecha1)
                {
                    cantidadDeViajes += 1;
                }
                nodoActual = nodoActual->siguiente;
            }
        }
        else
        {
            nodoActual = ultimo;
            while(nodoActual!= nullptr && nodoActual->viaje.fecha >= fecha1)
            {
                if(nodoActual->viaje.estacion_origen == estacion && nodoActual->viaje.fecha <= fecha2)
                {
                    cantidadDeViajes += 1;
                }
                nodoActual = nodoActual->anterior;
            }
        }
    }

    return cantidadDeViajes;
}

int minutosPedaleadosPorusuario(ListaDeViajes* lista, int dniUsuario)
{
    int minutosPedaleados = 0;
    auto* nodoActual = lista->getPrimero();

    while (nodoActual != nullptr)
    {
        if (nodoActual->viaje.dni_usuario == dniUsuario)
        {
            minutosPedaleados += nodoActual->viaje.duracion_min;
        }

        nodoActual = nodoActual->siguiente;
    }


    return minutosPedaleados;
}


void llenarConDatos(ListaDeViajes* lista)
{
    string linea = "", estacion_origen = "", estacion_destino = "";
    int viaje_id = 0, bici_id = 0, dni_usuario = 0, fecha = 0, duracion_min = 0;
    fstream DatosDeViaje;
    DatosDeViaje.open("./src/datos de entrada/viajes.txt");

    while (DatosDeViaje >> viaje_id >> bici_id >> dni_usuario >> estacion_origen >> estacion_destino >> fecha >> duracion_min)
    {
        Viaje aux;
        aux.id = viaje_id;
        aux.bici_id = bici_id;
        aux.dni_usuario = dni_usuario;
        aux.estacion_origen = estacion_origen;
        aux.estacion_destino = estacion_destino;
        aux.fecha = fecha;
        aux.duracion_min = duracion_min;

        lista->push(aux);
    }

    DatosDeViaje.close();
};

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

        void Push(int dni)
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

        int Pop()
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


int main()
{
    ListaDeViajes lista;
    llenarConDatos(&lista);

    return 0;
}