#ifndef ESTRUCTURAS_H

#include <iostream>
#include <string>

#define ESTRUCTURAS_H

template <typename T>
struct NodoListaEnlazada
{
    T * dato = nullptr;
    NodoListaEnlazada<T> * siguiente = nullptr;

    NodoListaEnlazada(T * nuevoDato)
    {
        this->dato = nuevoDato;
    }
};

template <typename T>
class ListaEnlazada
{
    NodoListaEnlazada<T> * primero = nullptr;
    NodoListaEnlazada<T> * ultimo = nullptr;

    public:
        NodoListaEnlazada<T> * getPrimero(){
            return this->primero;
        }

        ~ListaEnlazada()
        {
            NodoListaEnlazada<T>* actual = this->primero;

            while (actual != nullptr)
            {
                NodoListaEnlazada<T>* siguiente = actual->siguiente;
                delete actual;
                actual = siguiente;
            }

            this->primero = nullptr;
            this->ultimo = nullptr;
        }
    
        void push(T * dato)
        {
           NodoListaEnlazada<T>* nuevoNodo = new NodoListaEnlazada<T>(dato);
            if (this->primero == nullptr)
            {
                this->primero = nuevoNodo;
                this->ultimo = this->primero;
            }
            else
            {
                this->ultimo->siguiente = nuevoNodo;
                this->ultimo = nuevoNodo;
            }
        }

        T * pop()
        {
            T * dato = this->primero->dato;
            NodoListaEnlazada<T> * aux = this->primero->siguiente;
            delete this->primero;
            this->primero = aux;

            return dato;
        }
};


template <typename T>
struct NodoListaDobleEnlazada
{
    T * dato = nullptr;
    NodoListaDobleEnlazada<T> * siguiente = nullptr;
    NodoListaDobleEnlazada<T> * anterior = nullptr;

    NodoListaDobleEnlazada(T * nuevoDato)
    {
        this->dato = nuevoDato;
    }
};

template <typename T>
class ListaDobleEnlazada
{
    NodoListaDobleEnlazada<T> * primero = nullptr;
    NodoListaDobleEnlazada<T> * ultimo = nullptr;

    public:
    bool vacia() const { return primero == nullptr;};
    const NodoListaDobleEnlazada<T> * getPrimero() const { return primero;};
    const NodoListaDobleEnlazada<T> * getUltimo() const { return ultimo;};
        
    
    void push(T * dato)
    {
        NodoListaDobleEnlazada<T>* aux = new NodoListaDobleEnlazada<T>(dato);
        if(vacia())
        {
            aux -> anterior = nullptr;
            aux -> siguiente = nullptr;
            this->primero = aux;
            this->ultimo = aux;
        }
        else
        {
            aux -> anterior = this->ultimo;
            this->ultimo -> siguiente = aux;
            this->ultimo = aux;
        }
    }

   ~ListaDobleEnlazada()
        {
            NodoListaDobleEnlazada<T>* actual = this->primero;

            while (actual != nullptr)
            {
                NodoListaDobleEnlazada<T>* siguiente = actual->siguiente;
                delete actual->dato;
                delete actual;
                actual = siguiente;
            }

            this->primero = nullptr;
            this->ultimo = nullptr;
        }
};

#endif