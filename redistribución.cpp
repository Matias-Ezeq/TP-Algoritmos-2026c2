#include <iostream>
#include <string>
#include <fstream>
using namespace std;


template <typename T>
class ListaDobleEnlazada
{
    struct Nodo
    {
        T* dato;
        Nodo* siguiente = nullptr;
        Nodo* anterior = nullptr;

        Nodo(T* dato){this->dato = dato;}
    };

    Nodo * primero = nullptr;
    Nodo * ultimo = nullptr;

    void liberarNodos()
    {
        Nodo* actual = primero;

        while (actual != nullptr)
        {
            Nodo* siguiente = actual->siguiente;
            delete actual->dato;
            delete actual;
            actual = siguiente;
        }

        primero = nullptr;
        ultimo = nullptr;
    }

    public:
        ListaDobleEnlazada() = default;

        // La lista es dueña de sus datos. Una copia superficial haria que
        // dos destructores liberen los mismos punteros.
        ListaDobleEnlazada(const ListaDobleEnlazada&) = delete;
        ListaDobleEnlazada& operator=(const ListaDobleEnlazada&) = delete;

        ~ListaDobleEnlazada()
        {
            liberarNodos();
        }

        Nodo * getPrimero() const
        {
            return primero;
        }
    
        void push(T * dato)
        {
            Nodo* nuevoNodo = new Nodo(dato);
            if (primero == nullptr)
            {
                primero = nuevoNodo;
                ultimo = primero;
            }
            else
            {
                ultimo->siguiente = nuevoNodo;
                nuevoNodo->anterior = ultimo;
                ultimo = nuevoNodo;
            }
        }

        T * pop()
        {
            if (primero == nullptr)
            {
                return nullptr;
            }

            T * dato = primero->dato;
            Nodo * aux = primero->siguiente;
            delete primero;
            primero = aux;

            if (primero == nullptr)
            {
                ultimo = nullptr;
            }
            else
            {
                primero->anterior = nullptr;
            }

            return dato;
        }
};

template <typename T,typename Q>
struct Tupla
{
    T dato1;
    Q dato2;

    Tupla(T dato1, Q dato2)
    {
        this->dato1 = dato1;
        this->dato2 = dato2;
    }
};


template <typename T>
class Pila
{
    struct Nodo
    {
        T* dato;
        Nodo* siguiente = nullptr;

        Nodo(T* dato){this->dato = dato;}
    };

    Nodo * primero = nullptr;

    public:
        Pila() = default;
        Pila(const Pila&) = delete;
        Pila& operator=(const Pila&) = delete;

        ~Pila()
        {
            while (primero != nullptr)
            {
                Nodo* aux = primero;
                primero = primero->siguiente;
                delete aux->dato;
                delete aux;
            }
        }

        Nodo * getPrimero()
        {
            return primero;
        }

        void push(T * dato)
        {
            Nodo* nuevoNodo = new Nodo(dato);
            nuevoNodo->siguiente = primero;
            primero = nuevoNodo;
        }

        T * pop()
        {
            if (primero == nullptr)
            {
                return nullptr;
            }

            T * dato = primero->dato;
            Nodo * aux = primero->siguiente;
            delete primero;
            primero = aux;
            return dato;
        }
};


void guardarSolucion(
    Pila<string>& camino,
    ListaDobleEnlazada<ListaDobleEnlazada<string>>& soluciones
)
{
    ListaDobleEnlazada<string>* solucion = new ListaDobleEnlazada<string>();
    auto* nodo = camino.getPrimero();

    while (nodo != nullptr)
    {
        solucion->push(new string(*(nodo->dato)));
        nodo = nodo->siguiente;
    }

    soluciones.push(solucion);
}


template <typename NodoEstacion>
void buscarSubconjuntosRecursivo(
    NodoEstacion* nodo,
    int restante,
    Pila<string>& camino,
    ListaDobleEnlazada<ListaDobleEnlazada<string>>& soluciones
)
{
    if (restante == 0)
    {
        guardarSolucion(camino, soluciones);
        return;
    }

    if (nodo == nullptr || restante < 0)
    {
        return;
    }

    if (nodo->dato->dato2 <= restante)
    {
        camino.push(new string(nodo->dato->dato1));

        buscarSubconjuntosRecursivo(
            nodo->siguiente,
            restante - nodo->dato->dato2,
            camino,
            soluciones
        );

        delete camino.pop();
    }

    buscarSubconjuntosRecursivo(
        nodo->siguiente,
        restante,
        camino,
        soluciones
    );
}


ListaDobleEnlazada<ListaDobleEnlazada<string>> * redistribuir(
    int camion,
    const ListaDobleEnlazada<Tupla<string,int>>& estaciones
)
{
    ListaDobleEnlazada<Tupla<string,int>> candidatas;
    auto* nodoActual = estaciones.getPrimero();

    while (nodoActual != nullptr)
    {
        if (nodoActual->dato->dato2 > 0
            && nodoActual->dato->dato2 <= camion)
        {
            candidatas.push(new Tupla<string,int>(
                nodoActual->dato->dato1,
                nodoActual->dato->dato2
            ));
        }

        nodoActual = nodoActual->siguiente;
    }

    auto* out = new ListaDobleEnlazada<ListaDobleEnlazada<string>>();
    Pila<string> camino;

    buscarSubconjuntosRecursivo(
        candidatas.getPrimero(),
        camion,
        camino,
        *out
    );

    return out;
}
