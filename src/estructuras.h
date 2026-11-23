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
    protected:
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
                delete actual -> dato;
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
            if (this->primero == nullptr)
            {
                return nullptr;
            }

            T * dato = this->primero->dato;
            NodoListaEnlazada<T> * aux = this->primero->siguiente;
            delete this->primero;
            this->primero = aux;

            if (this->primero == nullptr)
            {
                this->ultimo = nullptr;
            }

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
    protected:
    NodoListaDobleEnlazada<T> * primero = nullptr;
    NodoListaDobleEnlazada<T> * ultimo = nullptr;

    public:
    bool vacia() const { return primero == nullptr;};
    const NodoListaDobleEnlazada<T> * getPrimero() const { return primero;};
    const NodoListaDobleEnlazada<T> * getUltimo() const { return ultimo;};
        
    
    void push(T * dato)
    {
        NodoListaDobleEnlazada<T>* nuevoNodo = new NodoListaDobleEnlazada<T>(dato);
        if(vacia())
        {
            nuevoNodo -> anterior = nullptr;
            nuevoNodo -> siguiente = nullptr;
            this->primero = nuevoNodo;
            this->ultimo = nuevoNodo;
        }
        else
        {
            this->ultimo->siguiente = nuevoNodo;
            nuevoNodo->anterior = this->ultimo;
            this->ultimo = nuevoNodo;
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
