#ifndef REDISTRIBUCION_H
#define REDISTRIBUCION_H

#include <string>

template <typename T>
class ListaDobleEnlazada
{
    struct Nodo
    {
        T* dato;
        Nodo* siguiente = nullptr;
        Nodo* anterior = nullptr;

        Nodo(T* dato) { this->dato = dato; }
    };

    Nodo* primero = nullptr;
    Nodo* ultimo = nullptr;

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

    Nodo* getPrimero() const
    {
        return primero;
    }

    void push(T* dato)
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

    T* pop()
    {
        if (primero == nullptr)
        {
            return nullptr;
        }

        T* dato = primero->dato;
        Nodo* aux = primero->siguiente;
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

template <typename T, typename Q>
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

        Nodo(T* dato) { this->dato = dato; }
    };

    Nodo* primero = nullptr;

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

    Nodo* getPrimero()
    {
        return primero;
    }

    void push(T* dato)
    {
        Nodo* nuevoNodo = new Nodo(dato);
        nuevoNodo->siguiente = primero;
        primero = nuevoNodo;
    }

    T* pop()
    {
        if (primero == nullptr)
        {
            return nullptr;
        }

        T* dato = primero->dato;
        Nodo* aux = primero->siguiente;
        delete primero;
        primero = aux;
        return dato;
    }
};

inline void guardarSolucion(
    Pila<std::string>& camino,
    ListaDobleEnlazada<ListaDobleEnlazada<std::string>>& soluciones
)
{
    ListaDobleEnlazada<std::string>* solucion =
        new ListaDobleEnlazada<std::string>();
    auto* nodo = camino.getPrimero();

    while (nodo != nullptr)
    {
        // Copia profunda: la solucion no puede compartir los strings del camino
        solucion->push(new std::string(*(nodo->dato)));
        nodo = nodo->siguiente;
    }

    soluciones.push(solucion);
}

template <typename NodoEstacion>
void buscarSubconjuntosRecursivo(
    NodoEstacion* nodo,
    int restante,
    Pila<std::string>& camino,
    ListaDobleEnlazada<ListaDobleEnlazada<std::string>>& soluciones
)
{
    // Solucion: el subconjunto suma exactamente la capacidad del camion
    if (restante == 0)
    {
        guardarSolucion(camino, soluciones);
        return;
    }

    // Poda: se terminaron las estaciones o ya me pase del limite
    if (nodo == nullptr || restante < 0)
    {
        return;
    }

    // Rama 1: incluir esta estacion
    if (nodo->dato->dato2 <= restante)
    {
        camino.push(new std::string(nodo->dato->dato1));

        buscarSubconjuntosRecursivo(
            nodo->siguiente,
            restante - nodo->dato->dato2,
            camino,
            soluciones
        );

        delete camino.pop();
    }

    // Rama 2: no incluir esta estacion
    buscarSubconjuntosRecursivo(
        nodo->siguiente,
        restante,
        camino,
        soluciones
    );
}

inline ListaDobleEnlazada<ListaDobleEnlazada<std::string>>* redistribuir(
    int camion,
    const ListaDobleEnlazada<Tupla<std::string,int>>& estaciones
)
{
    ListaDobleEnlazada<Tupla<std::string,int>> candidatas;
    auto* nodoActual = estaciones.getPrimero();

    // Descarta las estaciones cuyo faltante solo ya supera al camion.
    // Tambien descarta faltantes nulos, que generarian soluciones repetidas.
    while (nodoActual != nullptr)
    {
        if (nodoActual->dato->dato2 > 0
            && nodoActual->dato->dato2 <= camion)
        {
            candidatas.push(new Tupla<std::string,int>(
                nodoActual->dato->dato1,
                nodoActual->dato->dato2
            ));
        }

        nodoActual = nodoActual->siguiente;
    }

    auto* out = new ListaDobleEnlazada<ListaDobleEnlazada<std::string>>();
    Pila<std::string> camino;

    buscarSubconjuntosRecursivo(
        candidatas.getPrimero(),
        camion,
        camino,
        *out
    );

    return out;
}

#endif
