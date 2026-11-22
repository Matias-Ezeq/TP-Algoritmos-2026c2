#include <iostream>
#include <fstream>
#include <string>

using namespace std;

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

    bool eliminarEstacion(const string& codigo)
    {
        int indice = buscarVertice(codigo);

        if (indice == -1)
        {
            return false;
        }

        int ultimo = cantidadVertices - 1;

        // 1. Liberar las aristas del vertice que se elimina.
        NodoVecino* vecino = vertices[indice].vecinos;

        while (vecino != nullptr)
        {
            NodoVecino* siguiente = vecino->siguiente;
            delete vecino;
            vecino = siguiente;
        }

        vertices[indice].vecinos = nullptr;

        // 2. Mover el ultimo vertice a la posicion libre.
        if (indice != ultimo)
        {
            vertices[indice] = vertices[ultimo];
            vertices[ultimo].vecinos = nullptr;
            vertices[ultimo].codigo = "";
        }

        cantidadVertices--;

        // 3. Quitar aristas hacia el vertice borrado y reindexar el ultimo.
        for (int i = 0; i < cantidadVertices; i++)
        {
            NodoVecino* actual = vertices[i].vecinos;
            NodoVecino* anterior = nullptr;

            while (actual != nullptr)
            {
                NodoVecino* siguiente = actual->siguiente;

                if (actual->vertice == indice)
                {
                    if (anterior == nullptr)
                    {
                        vertices[i].vecinos = siguiente;
                    }
                    else
                    {
                        anterior->siguiente = siguiente;
                    }

                    delete actual;
                }
                else
                {
                    if (actual->vertice == ultimo)
                    {
                        actual->vertice = indice;
                    }

                    anterior = actual;
                }

                actual = siguiente;
            }
        }

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

            // No se corta al llegar al destino: hay que vaciar la cola para
            // que la proxima busqueda empiece limpia.
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
