#include <iostream>
#include <fstream>
#include <string>

#include "modelos.h"

using namespace std;


class TablaHashEstaciones
{
private:
    struct Nodo
    {
        Estacion estacion;
        Nodo* siguiente;

        Nodo(const Estacion& estacion)
            : estacion(estacion), siguiente(nullptr)
        {
        }
    };

    static const int capacidad = 43;
    Nodo* tabla[capacidad] = {};
    int cantidadEstaciones = 0;
    int cantidadColisiones = 0;

    int calcularHash(const string& codigo) const
    {
        unsigned long valor = 0;

        for (char caracter : codigo)
        {
            valor = valor * 31 + caracter;
        }

        return static_cast<int>(valor % capacidad);
    }

public:
    ~TablaHashEstaciones()
    {
        for (int i = 0; i < capacidad; i++)
        {
            Nodo* actual = tabla[i];

            while (actual != nullptr)
            {
                Nodo* siguiente = actual->siguiente;
                delete actual;
                actual = siguiente;
            }
        }
    }

    Estacion* buscar(const string& codigoBuscado)
    {
        int posicion = calcularHash(codigoBuscado);
        Nodo* actual = tabla[posicion];

        while (actual != nullptr)
        {
            if (actual->estacion.codigo == codigoBuscado)
            {
                return &actual->estacion;
            }

            actual = actual->siguiente;
        }

        return nullptr;
    }

    bool mostrarEstacionPorCodigo(const string& codigoBuscado)
    {
        Estacion* estacion = buscar(codigoBuscado);

        if (estacion == nullptr)
        {
            cout << "No existe una estacion con el codigo "
                 << codigoBuscado << ".\n";
            return false;
        }

        estacion->mostrar();
        return true;
    }

    bool agregarEstacion(const Estacion& nuevaEstacion)
    {
        int posicion = calcularHash(nuevaEstacion.codigo);

        if (buscar(nuevaEstacion.codigo) != nullptr)
        {
            return false;
        }

        if (tabla[posicion] != nullptr)
        {
            cantidadColisiones++;
        }

        Nodo* nuevo = new Nodo(nuevaEstacion);
        nuevo->siguiente = tabla[posicion];
        tabla[posicion] = nuevo;
        cantidadEstaciones++;

        return true;
    }

    bool eliminarEstacion(const string& codigoBuscado)
    {
        int posicion = calcularHash(codigoBuscado);
        Nodo* actual = tabla[posicion];
        Nodo* anterior = nullptr;

        while (actual != nullptr)
        {
            if (actual->estacion.codigo == codigoBuscado)
            {
                if (anterior == nullptr)
                {
                    tabla[posicion] = actual->siguiente;
                }
                else
                {
                    anterior->siguiente = actual->siguiente;
                }

                delete actual;
                cantidadEstaciones--;

                cout << "La estacion " << codigoBuscado
                     << " fue eliminada correctamente.\n";
                return true;
            }

            anterior = actual;
            actual = actual->siguiente;
        }

        cout << "No existe una estacion con el codigo "
             << codigoBuscado << ".\n";
        return false;
    }

    int obtenerCantidadEstaciones() const
    {
        return cantidadEstaciones;
    }

    int obtenerCantidadColisiones() const
    {
        return cantidadColisiones;
    }

    double obtenerFactorCarga() const
    {
        return static_cast<double>(cantidadEstaciones) / capacidad;
    }

    int copiarEstaciones(Estacion estaciones[]) const
    {
        int posicion = 0;

        for (int i = 0; i < capacidad; i++)
        {
            Nodo* actual = tabla[i];

            while (actual != nullptr)
            {
                estaciones[posicion] = actual->estacion;
                posicion++;
                actual = actual->siguiente;
            }
        }

        return posicion;
    }

    bool cargarDesdeArchivo(const string& ruta)
    {
        ifstream archivo(ruta);

        if (!archivo.is_open())
        {
            return false;
        }

        string codigo;
        string nombre;
        string barrio;
        int capacidadAnclajes;
        int bicisDisponibles;

        while (archivo >> codigo >> nombre >> barrio
                       >> capacidadAnclajes >> bicisDisponibles)
        {
            Estacion estacion(
                codigo,
                nombre,
                barrio,
                capacidadAnclajes,
                bicisDisponibles
            );

            agregarEstacion(estacion);
        }

        return true;
    }
};

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
