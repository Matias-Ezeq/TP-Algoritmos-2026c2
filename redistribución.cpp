#include <iostream>
#include <string>
#include <fstream>


template <typename T>
class ListaDobleEnlazada
{
    struct Nodo
    {
        T* dato;
        Nodo* siguiente = nullptr;
        Nodo* anterior = nullptr;

        Nodo(T* dato){this->dato = dato};
    };

    Nodo * primero = nullptr;
    Nodo * ultimo = nullptr;

    public:
        Nodo * getPrimero(){
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
            T * dato = primero->dato;
            Nodo * aux = primero->siguiente;
            delete primero;
            primero = aux;

            return dato;
        }
};

template <typename T>
struct Tupla
{
    T dato1;
    T dato2;

    Tupla(T dato1, T dato2)
    {
        this->dato1 = dato1;
        this->dato2 = dato2;
    }
};

ListaDobleEnlazada<ListaDobleEnlazada<char*>> redistribuir(int camion,ListaDobleEnlazada<char*>estaciones)
{
    //logica
}


