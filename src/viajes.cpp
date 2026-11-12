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

//idea: cada esración cuenta con una lista de viajes.
class ListaDeViajes
{
    private:
        struct Nodo{

            Viaje viaje;
            Nodo * siguiente;
            Nodo * anterior;
        };
    public:
        Nodo * primero;
        Nodo * ultimo;
    
    void push(Viaje viaje)
    {
        Nodo aux;
        aux.viaje = viaje;
        if(ultimo == nullptr)
        {
            aux.anterior = nullptr;
            aux.siguiente = nullptr;
            primero = &aux;
            ultimo = &aux;
        }
        else
        {
            //comparar fechas para ver desde que lado iterar para asegurar orden por fecha.
            int fechaPrimero = primero->viaje.fecha;
            int fechaUltimo = ultimo->viaje.fecha;
            if (viaje.fecha <= fechaPrimero)
            {

            }
            else if (viaje.fecha >= fechaUltimo)
            {

            }
            else {
                int difPrimero = fechaPrimero - viaje.fecha;
                int difUltimo = viaje.fecha - fechaUltimo;

                if(difPrimero <= difUltimo)
                {
                    //iterar desde el inicio
                }
                else
                {
                    //iterar desde el final
                }
            }
        }
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

class Bicicleta{
    private:
        int id;
        string tipo;
        string estado;
        int kilometros;
    
    int id(){return id;};
    string estadoActual(){return estado;};
    int kilometrosRecorridos(){return kilometros;};
};



class ColaDeUsuarios {
    private:
        struct Nodo
        {
            int dni;
            Nodo* siguiente;
        };

        Nodo* primero;
        Nodo* ultimo;

    void Push(int dni)
    {
        Nodo aux;
        aux.dni = dni;
        aux.siguiente = nullptr;

        if (ultimo == nullptr)
        {
            ultimo, primero = &aux;
        }
        else
        {
        ultimo->siguiente = &aux;
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