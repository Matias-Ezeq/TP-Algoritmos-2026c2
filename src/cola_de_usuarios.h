#ifndef COLA_DE_USUARIOS_H
#define COLA_DE_USUARIOS_H

class ColaDeUsuarios
{
    struct Nodo
    {
        int dni = 0;
        Nodo* siguiente = nullptr;
    };

    Nodo* primero = nullptr;
    Nodo* ultimo = nullptr;

public:
    ~ColaDeUsuarios()
    {
        while (primero != nullptr)
        {
            Nodo* aux = primero;
            primero = primero->siguiente;
            delete aux;
        }

        ultimo = nullptr;
    }

    bool vacia() const
    {
        return primero == nullptr;
    }

    void Push(int dni)
    {
        Nodo* aux = new Nodo;
        aux->dni = dni;
        aux->siguiente = nullptr;

        if (ultimo == nullptr)
        {
            primero = aux;
            ultimo = aux;
        }
        else
        {
            ultimo->siguiente = aux;
            ultimo = aux;
        }
    }

    int Pop()
    {
        // Devuelve -1 si la cola esta vacia; conviene consultar vacia() antes.
        if (primero == nullptr)
        {
            return -1;
        }

        Nodo* aux = primero;
        primero = primero->siguiente;
        int dni = aux->dni;
        aux->siguiente = nullptr;
        delete aux;

        if (primero == nullptr)
        {
            ultimo = nullptr;
        }

        return dni;
    }
};

#endif
