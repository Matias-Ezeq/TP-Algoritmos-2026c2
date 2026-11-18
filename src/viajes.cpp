#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

#include "viajes.h"

using namespace std;

int cantidadDeViajesEntreFechaYFecha(
    ListaDeViajes* lista,
    int fecha1,
    int fecha2,
    const string& estacion
)
{
    const auto* primero = lista->getPrimero();
    const auto* ultimo = lista->getUltimo();
    int cantidadDeViajes = 0;

    if (fecha1 > fecha2) swap(fecha1, fecha2);

    if (!lista->vacia())
    {
        int diferenciaInicio = fecha1 - primero->viaje.fecha;
        int diferenciaFinal = ultimo->viaje.fecha - fecha2;

        auto* nodoActual = primero;

        if (diferenciaInicio <= diferenciaFinal)
        {
            while (nodoActual != nullptr && nodoActual->viaje.fecha <= fecha2)
            {
                if (nodoActual->viaje.estacion_origen == estacion
                    && nodoActual->viaje.fecha >= fecha1)
                {
                    cantidadDeViajes += 1;
                }

                nodoActual = nodoActual->siguiente;
            }
        }
        else
        {
            nodoActual = ultimo;

            while (nodoActual != nullptr && nodoActual->viaje.fecha >= fecha1)
            {
                if (nodoActual->viaje.estacion_origen == estacion
                    && nodoActual->viaje.fecha <= fecha2)
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
    string estacion_origen = "";
    string estacion_destino = "";
    int viaje_id = 0;
    int bici_id = 0;
    int dni_usuario = 0;
    int fecha = 0;
    int duracion_min = 0;

    fstream DatosDeViaje;
    DatosDeViaje.open("./src/datos de entrada/viajes.txt");

    while (DatosDeViaje >> viaje_id >> bici_id >> dni_usuario
                       >> estacion_origen >> estacion_destino
                       >> fecha >> duracion_min)
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
}
