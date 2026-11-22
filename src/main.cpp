#include <iostream>
#include <string>
#include <fstream>

#include "modelos.h"
#include "estructuras.h"
#include "estaciones.cpp"
#include "ciclovias.cpp"
#include "ejercicio_arbol.c++"

// viajes.cpp tiene su propio main: se renombra para poder reutilizar sus
// clases y funciones dentro de este menu.
#define main mainDeViajes
#include "viajes.cpp"
#undef main

using namespace std;

class ColaEsperaEstaciones
{
private:
    struct Nodo
    {
        string codigo;
        ColaDeUsuarios* cola;
        Nodo* siguiente;

        explicit Nodo(const string& codigo)
            : codigo(codigo), cola(new ColaDeUsuarios()), siguiente(nullptr)
        {
        }

        ~Nodo()
        {
            delete cola;
        }
    };

    static const int capacidad = 43;
    Nodo* tabla[capacidad] = {};

    int calcularHash(const string& codigo) const
    {
        unsigned long valor = 0;

        for (char caracter : codigo)
        {
            valor = valor * 31 + caracter;
        }

        return static_cast<int>(valor % capacidad);
    }

    Nodo* buscarNodo(const string& codigo) const
    {
        int posicion = calcularHash(codigo);
        Nodo* actual = tabla[posicion];

        while (actual != nullptr)
        {
            if (actual->codigo == codigo)
            {
                return actual;
            }

            actual = actual->siguiente;
        }

        return nullptr;
    }

    Nodo* obtenerNodo(const string& codigo)
    {
        Nodo* nodo = buscarNodo(codigo);

        if (nodo != nullptr)
        {
            return nodo;
        }

        int posicion = calcularHash(codigo);
        nodo = new Nodo(codigo);
        nodo->siguiente = tabla[posicion];
        tabla[posicion] = nodo;

        return nodo;
    }

public:
    ~ColaEsperaEstaciones()
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

    void encolar(const string& codigo, int dni)
    {
        obtenerNodo(codigo)->cola->Push(dni);
    }

    bool hayEspera(const string& codigo) const
    {
        Nodo* nodo = buscarNodo(codigo);
        return nodo != nullptr && !nodo->cola->vacia();
    }

    int desencolar(const string& codigo)
    {
        Nodo* nodo = buscarNodo(codigo);

        if (nodo == nullptr || nodo->cola->vacia())
        {
            return -1;
        }

        return nodo->cola->Pop();
    }
};

struct Sistema
{
    TablaHashEstaciones tabla;
    GrafoCiclovias grafo;
    ArbolBicicletas arbol;
    ListaDeViajes viajes;
    ColaEsperaEstaciones esperas;
};

int leerEntero(const string& mensaje)
{
    int valor;

    while (true)
    {
        cout << mensaje;

        if (cin >> valor)
        {
            return valor;
        }

        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Ingrese un numero valido.\n";
    }
}

bool cargarEstaciones(Sistema& sistema)
{
    if (!sistema.tabla.cargarDesdeArchivo(
            "src/datos de entrada/estaciones.txt"))
    {
        cout << "No se pudo cargar el archivo de estaciones.\n";
        return false;
    }

    int cantidad = sistema.tabla.obtenerCantidadEstaciones();
    Estacion* estaciones = new Estacion[cantidad];
    sistema.tabla.copiarEstaciones(estaciones);

    for (int i = 0; i < cantidad; i++)
    {
        if (!sistema.grafo.agregarEstacion(estaciones[i].codigo))
        {
            cout << "No se pudo agregar la estacion "
                 << estaciones[i].codigo << " al grafo.\n";
            delete[] estaciones;
            return false;
        }
    }

    delete[] estaciones;

    if (!sistema.grafo.cargarDesdeArchivo(
            "src/datos de entrada/ciclovias.txt"))
    {
        cout << "No se pudo cargar el archivo de ciclovias.\n";
        return false;
    }

    return true;
}

bool cargarBicicletas(Sistema& sistema)
{
    ifstream archivo("src/datos de entrada/bicicletas.txt");

    if (!archivo.is_open())
    {
        cout << "No se pudo cargar el archivo de bicicletas.\n";
        return false;
    }

    int biciId;
    string codigoEstacion;
    string tipo;
    string estado;
    int kilometros;

    while (archivo >> biciId >> codigoEstacion >> tipo >> estado >> kilometros)
    {
        sistema.arbol.insertar(
            biciId,
            codigoEstacion,
            tipo,
            estado,
            kilometros
        );
    }

    return true;
}

#include "submenus/menu_estaciones.cpp"
#include "submenus/menu_viajes.cpp"
#include "submenus/menu_bicicletas.cpp"
#include "submenus/menu_redistribucion.cpp"

int main()
{
    Sistema sistema;

    if (!cargarEstaciones(sistema) || !cargarBicicletas(sistema))
    {
        cout << "No se pudieron cargar los datos de entrada.\n";
        return 1;
    }

    llenarConDatos(&sistema.viajes);

    int opcion;

    do
    {
        cout << "\n=== Sistema de Bicicletas Publicas ===\n"
             << "1. Gestion de estaciones (A)\n"
             << "2. Viajes y colas de espera (B)\n"
             << "3. Bicicletas - ABB (C)\n"
             << "4. Redistribucion - backtracking (D) - PUNTO SIN FINALIZAR\n"
             << "0. Salir\n";

        opcion = leerEntero("Seleccionar una opcion: ");

        switch (opcion)
        {
        case 1:
            menuEstaciones(sistema);
            break;
        case 2:
            menuViajes(sistema);
            break;
        case 3:
            menuBicicletas(sistema);
            break;
        case 4:
            menuRedistribucion();
            break;
        case 0:
            cout << "Programa finalizado.\n";
            break;
        default:
            cout << "Opcion invalida.\n";
        }
    }
    while (opcion != 0);

    return 0;
}
