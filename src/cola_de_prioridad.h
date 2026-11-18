#ifndef COLA_DE_PRIORIDAD_H
#define COLA_DE_PRIORIDAD_H

#include <iostream>
#include <string>

class Bicicleta
{
private:
    int biciId;
    std::string codigoEstacion;
    std::string tipo;
    std::string estado;
    int kilometros;

public:
    Bicicleta(int id, std::string est, std::string t, std::string e, int km)
        : biciId(id), codigoEstacion(est), tipo(t), estado(e), kilometros(km) {}

    int getBiciId() const { return biciId; }
    std::string getCodigoEstacion() const { return codigoEstacion; }
    std::string getTipo() const { return tipo; }
    std::string getEstado() const { return estado; }
    int getKilometros() const { return kilometros; }

    void setCodigoEstacion(const std::string& est) { codigoEstacion = est; }
    void setEstado(const std::string& nuevoEstado) { estado = nuevoEstado; }
    void setKilometros(int km) { kilometros = km; }

    void mostrarDatos() const
    {
        std::cout << "Id: " << biciId
                  << " | Estacion: " << codigoEstacion
                  << " | Tipo: " << tipo
                  << " | Estado: " << estado
                  << " | Km: " << kilometros << std::endl;
    }
};

class ColaDePrioridad
{
    enum prioridad
    {
        nula, alta, media, baja
    };

    struct Nodo
    {
        Bicicleta* bici;
        int prioridad = nula;
        Nodo* siguiente = nullptr;
        Nodo* anterior = nullptr;

        Nodo(Bicicleta* nuevaBici) : bici(nuevaBici)
        {
            if (nuevaBici->getTipo() == "Electrica")
            {
                prioridad = alta;
            }
            else if (nuevaBici->getEstado() == "Averiada")
            {
                prioridad = media;
            }
            else
            {
                prioridad = baja;
            }
        }
    };

    Nodo* primero = nullptr;
    Nodo* ultimo = nullptr;

    void insertarRecorriendo(Nodo* nuevoNodo)
    {
        // Complejidad O(n)
        if (nuevoNodo->prioridad < primero->prioridad)
        {
            nuevoNodo->siguiente = primero;
            primero->anterior = nuevoNodo;
            primero = nuevoNodo;
        }
        else if (nuevoNodo->prioridad >= ultimo->prioridad)
        {
            nuevoNodo->anterior = ultimo;
            ultimo->siguiente = nuevoNodo;
            ultimo = nuevoNodo;
        }
        else
        {
            Nodo* nodoActual = primero;

            while (nodoActual != nullptr
                   && nodoActual->prioridad <= nuevoNodo->prioridad)
            {
                nodoActual = nodoActual->siguiente;
            }

            nuevoNodo->siguiente = nodoActual;
            nuevoNodo->anterior = nodoActual->anterior;

            nodoActual->anterior->siguiente = nuevoNodo;
            nodoActual->anterior = nuevoNodo;
        }
    }

public:
    ~ColaDePrioridad()
    {
        // La cola es dueña de las bicicletas que todavia no se extrajeron.
        while (primero != nullptr)
        {
            Nodo* aux = primero;
            primero = aux->siguiente;
            delete aux->bici;
            delete aux;
        }

        ultimo = nullptr;
    }

    void insertar(Bicicleta* bicicleta)
    {
        Nodo* aux = new Nodo(bicicleta);

        if (primero == nullptr)
        {
            primero = aux;
            ultimo = aux;
        }
        else
        {
            insertarRecorriendo(aux);
        }
    }

    Bicicleta* extraer()
    {
        if (primero == nullptr)
        {
            return nullptr;
        }

        Bicicleta* bicicleta = primero->bici;
        Nodo* aux = primero;

        primero = aux->siguiente;

        aux->siguiente = nullptr;
        aux->anterior = nullptr;
        delete aux;

        return bicicleta;
    }
};

#endif
