#include <iostream>
#include <string>
#include <fstream>
#include <bits/stdc++.h>
using namespace std;

class Viaje
{
    public:
        int id;
        int bici_id;
        int dni_usuario;
        string estacion_origen;
        string estacion_destino;
        int fecha;
        int durecion_min;
};

//idea: cada estación cuenta con una lista de viajes. cada viaje se guarda de forma ordenada en una lista bidireccional
class ListaDeViajes
{
    struct Nodo {
        Viaje viaje;
        Nodo * siguiente = nullptr;
        Nodo * anterior = nullptr;
        explicit Nodo(const Viaje& v) : viaje(v) {}
    };
    
    Nodo * primero = nullptr;
    Nodo * ultimo = nullptr;

    public:
    bool vacia() const { return primero == nullptr;};
    const Nodo * getPrimero() const { return primero;};
    const Nodo * getUltimo() const { return ultimo;};
        
    
    void push(const Viaje& viaje)
    {
        Nodo* aux = new Nodo(viaje);
        if(vacia())
        {
            aux -> anterior = nullptr;
            aux -> siguiente = nullptr;
            primero = aux;
            ultimo = aux;
        }
        else
        {
            int fechaPrimero = primero -> viaje.fecha;
            int fechaUltimo = ultimo -> viaje.fecha;
            if (viaje.fecha <= fechaPrimero)
            {
                aux -> siguiente = primero;
                primero -> anterior = aux;
                primero = aux;
            }
            else if (viaje.fecha >= fechaUltimo)
            {
                aux -> anterior = ultimo;
                ultimo -> siguiente = aux;
                ultimo = aux;
            }
            //si la fecha es posterior a la del primero y anterior al del ultimo, compara para ver desde que lado iterar para asegurar orden por fecha.
            else {
                int difPrimero = viaje.fecha - fechaPrimero;
                int difUltimo = fechaUltimo - viaje.fecha;
                Nodo * nodoActual;

                if(difPrimero <= difUltimo)
                {
                    nodoActual = primero -> siguiente;
                    while(nodoActual -> viaje.fecha <= viaje.fecha)
                    {
                        nodoActual = nodoActual -> siguiente;
                    }
                    nodoActual = nodoActual -> anterior;

                }
                else
                {
                    nodoActual = ultimo -> anterior;
                    while(nodoActual -> viaje.fecha >= viaje.fecha)
                    {
                        nodoActual = nodoActual->anterior;
                    }
                }
                aux -> siguiente = nodoActual -> siguiente;
                aux -> anterior = nodoActual;
                nodoActual -> siguiente = aux;
                aux -> siguiente -> anterior = aux;
            }
        }
    }

   ~ListaDeViajes() {
    for (Nodo* n = primero; n; ) { Nodo* sig = n->siguiente; delete n; n = sig; }
} 
};


void parser()
{
    string linea = "";
    fstream DatosDeViaje;
    DatosDeViaje.open("./src/datos de entrada/viajes.txt");
    getline(DatosDeViaje,linea);

    stringstream ss(linea);


    DatosDeViaje.close();
};

class ColaDeUsuarios {
    private:
        struct Nodo
        {
            int dni;
            Nodo* siguiente = nullptr;
        };

        Nodo* primero = nullptr;
        Nodo* ultimo = nullptr;

    void Push(int dni)
    {
        Nodo* aux = new Nodo;
        aux -> dni = dni;
        aux -> siguiente = nullptr;

        if (ultimo == nullptr)
        {
            ultimo = aux;
            primero = ultimo;
        }
        else
        {
        ultimo -> siguiente = aux;
        }
    }

    int Pop()
    {
        Nodo* aux = primero;
        primero = primero->siguiente;
        int dni = aux->dni;
        aux->siguiente = nullptr;
        delete aux;
        return dni;
    }    
};


int main()
{
    parser();

    return 0;
}