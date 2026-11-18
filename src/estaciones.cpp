#include <iostream>
#include "estaciones.h"

using namespace std;

void ordenarPorSeleccion(
    Estacion estaciones[],
    int cantidadEstaciones,
    bool ordenarPorCapacidad
)
{
    for (int i = 0; i < cantidadEstaciones - 1; i++)
    {
        int menor = i;

        for (int j = i + 1; j < cantidadEstaciones; j++)
        {
            bool esMenor;

            if (ordenarPorCapacidad)
            {
                esMenor = estaciones[j].capacidad
                          < estaciones[menor].capacidad;
            }
            else
            {
                esMenor = estaciones[j].bicisDisponibles
                          < estaciones[menor].bicisDisponibles;
            }

            if (esMenor)
            {
                menor = j;
            }
        }

        Estacion auxiliar = estaciones[i];
        estaciones[i] = estaciones[menor];
        estaciones[menor] = auxiliar;
    }
}

void listarEstacionesOrdenadas(
    const TablaHashEstaciones& tabla,
    bool ordenarPorCapacidad
)
{
    int cantidad = tabla.obtenerCantidadEstaciones();
    Estacion* estaciones = new Estacion[cantidad];
    tabla.copiarEstaciones(estaciones);

    ordenarPorSeleccion(estaciones, cantidad, ordenarPorCapacidad);

    for (int i = 0; i < cantidad; i++)
    {
        estaciones[i].mostrar();
        cout << '\n';
    }

    delete[] estaciones;
}

void listarEstacionesBajaOcupacion(const TablaHashEstaciones& tabla)
{
    int cantidad = tabla.obtenerCantidadEstaciones();
    Estacion* estaciones = new Estacion[cantidad];
    tabla.copiarEstaciones(estaciones);
    bool encontroEstacion = false;

    for (int i = 0; i < cantidad; i++)
    {
        if (estaciones[i].capacidad > 0
            && estaciones[i].bicisDisponibles * 100
               < estaciones[i].capacidad * 20)
        {
            estaciones[i].mostrar();
            cout << '\n';
            encontroEstacion = true;
        }
    }

    if (!encontroEstacion)
    {
        cout << "No hay estaciones con menos del 20% de ocupacion.\n";
    }

    delete[] estaciones;
}
