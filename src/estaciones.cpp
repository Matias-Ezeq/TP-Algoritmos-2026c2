#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Estacion
{
public:
    string codigo;
    string nombre;
    string barrio;
    int capacidad;
    int bicisDisponibles;

    Estacion(
        string codigo,
        string nombre,
        string barrio,
        int capacidad,
        int bicisDisponibles
    ) : codigo(codigo),
        nombre(nombre),
        barrio(barrio),
        capacidad(capacidad),
        bicisDisponibles(bicisDisponibles)
    {
    }

    Estacion()
        : codigo(""),
          nombre(""),
          barrio(""),
          capacidad(0),
          bicisDisponibles(0)
    {
    }

    void mostrar() const
    {
        cout << "Codigo: " << codigo << '\n'
             << "Nombre: " << nombre << '\n'
             << "Barrio: " << barrio << '\n'
             << "Capacidad de anclajes: " << capacidad << '\n'
             << "Bicicletas disponibles: " << bicisDisponibles << '\n';
    }

};

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

class GrafoCiclovias
{
private:
    struct NodoVecino
    {
        int vertice;
        NodoVecino* siguiente;

        NodoVecino(int vertice)
            : vertice(vertice), siguiente(nullptr)
        {
        }
    };

    struct Vertice
    {
        string codigo;
        NodoVecino* vecinos;

        Vertice()
            : codigo(""), vecinos(nullptr)
        {
        }
    };

    struct NodoCola
    {
        int vertice;
        NodoCola* siguiente;

        NodoCola(int vertice)
            : vertice(vertice), siguiente(nullptr)
        {
        }
    };

    Vertice* vertices = nullptr;
    int capacidadVertices = 0;
    int cantidadVertices = 0;
    NodoCola* primero = nullptr;
    NodoCola* ultimo = nullptr;

    void ampliarVertices()
    {
        int nuevaCapacidad;

        if (capacidadVertices == 0)
        {
            nuevaCapacidad = 4;
        }
        else
        {
            nuevaCapacidad = capacidadVertices * 2;
        }

        Vertice* nuevaLista = new Vertice[nuevaCapacidad];

        for (int i = 0; i < cantidadVertices; i++)
        {
            nuevaLista[i] = vertices[i];
        }

        delete[] vertices;
        vertices = nuevaLista;
        capacidadVertices = nuevaCapacidad;
    }

    int buscarVertice(const string& codigo) const
    {
        for (int i = 0; i < cantidadVertices; i++)
        {
            if (vertices[i].codigo == codigo)
            {
                return i;
            }
        }

        return -1;
    }

    void encolar(int vertice)
    {
        NodoCola* nuevo = new NodoCola(vertice);

        if (ultimo == nullptr)
        {
            primero = nuevo;
            ultimo = nuevo;
        }
        else
        {
            ultimo->siguiente = nuevo;
            ultimo = nuevo;
        }
    }

    int desencolar()
    {
        NodoCola* nodo = primero;
        int vertice = nodo->vertice;
        primero = primero->siguiente;

        if (primero == nullptr)
        {
            ultimo = nullptr;
        }

        delete nodo;
        return vertice;
    }

    void agregarVecino(int origen, int destino)
    {
        NodoVecino* nuevo = new NodoVecino(destino);
        nuevo->siguiente = vertices[origen].vecinos;
        vertices[origen].vecinos = nuevo;
    }

public:
    ~GrafoCiclovias()
    {
        for (int i = 0; i < cantidadVertices; i++)
        {
            NodoVecino* actual = vertices[i].vecinos;

            while (actual != nullptr)
            {
                NodoVecino* siguiente = actual->siguiente;
                delete actual;
                actual = siguiente;
            }
        }

        while (primero != nullptr)
        {
            desencolar();
        }

        delete[] vertices;
    }

    bool agregarEstacion(const string& codigo)
    {
        if (buscarVertice(codigo) != -1)
        {
            return true;
        }

        if (cantidadVertices == capacidadVertices)
        {
            ampliarVertices();
        }

        vertices[cantidadVertices].codigo = codigo;
        cantidadVertices++;
        return true;
    }

    bool agregarCiclovia(
        const string& codigoOrigen,
        const string& codigoDestino
    )
    {
        int origen = buscarVertice(codigoOrigen);
        int destino = buscarVertice(codigoDestino);

        if (origen == -1 || destino == -1)
        {
            return false;
        }

        agregarVecino(origen, destino);
        agregarVecino(destino, origen);
        return true;
    }

    bool cargarDesdeArchivo(const string& ruta)
    {
        ifstream archivo(ruta);

        if (!archivo.is_open())
        {
            return false;
        }

        string codigoOrigen;
        string codigoDestino;

        while (archivo >> codigoOrigen >> codigoDestino)
        {
            agregarCiclovia(codigoOrigen, codigoDestino);
        }

        return true;
    }

    bool mostrarRecorridoBFS(
        const string& codigoOrigen,
        const string& codigoDestino
    )
    {
        int origen = buscarVertice(codigoOrigen);
        int destino = buscarVertice(codigoDestino);

        if (origen == -1 || destino == -1)
        {
            cout << "Una de las estaciones no existe en el grafo.\n";
            return false;
        }

        bool* visitado = new bool[cantidadVertices];
        int* predecesor = new int[cantidadVertices];
        int* camino = new int[cantidadVertices];

        for (int i = 0; i < cantidadVertices; i++)
        {
            visitado[i] = false;
            predecesor[i] = -1;
        }

        encolar(origen);
        visitado[origen] = true;

        while (primero != nullptr)
        {
            int actual = desencolar();

            if (actual == destino)
            {
                break;
            }

            NodoVecino* vecino = vertices[actual].vecinos;

            while (vecino != nullptr)
            {
                if (!visitado[vecino->vertice])
                {
                    visitado[vecino->vertice] = true;
                    predecesor[vecino->vertice] = actual;
                    encolar(vecino->vertice);
                }

                vecino = vecino->siguiente;
            }
        }

        if (!visitado[destino])
        {
            cout << "No existe un recorrido entre las estaciones.\n";
            delete[] camino;
            delete[] predecesor;
            delete[] visitado;
            return false;
        }

        int cantidadCamino = 0;
        int actual = destino;

        while (actual != -1)
        {
            camino[cantidadCamino] = actual;
            cantidadCamino++;
            actual = predecesor[actual];
        }

        cout << "Recorrido con menor cantidad de tramos: ";

        for (int i = cantidadCamino - 1; i >= 0; i--)
        {
            cout << vertices[camino[i]].codigo;

            if (i > 0)
            {
                cout << " -> ";
            }
        }

        cout << '\n';
        delete[] camino;
        delete[] predecesor;
        delete[] visitado;
        return true;
    }
};
