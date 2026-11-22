#include <iostream>
#include <string>
#include <fstream>
#include <bits/stdc++.h>

#include "modelos.h"
#include "estructuras.h"


using namespace std;



//Lista Ordenada doblemente enlazada para permitir busquedas e inserciones ordenadas desde cualquier extremo. Complejidad O(N) de busqueda y O(log N) de inserción ya que elige el extremo con menor diferencia respecto a la fecha del viaje a insertar.


class ListaDeViajes: public ListaDobleEnlazada<Viaje>
{
    public:
    void push(Viaje * viaje)
    {
        NodoListaDobleEnlazada<Viaje> * aux = new NodoListaDobleEnlazada(viaje);
        if(vacia())
        {
            aux -> anterior = nullptr;
            aux -> siguiente = nullptr;
            this->primero = aux;
            this-> ultimo = aux;
        }
        else
        {
            int fechaPrimero = primero -> dato -> fecha;
            int fechaUltimo = ultimo -> dato -> fecha;
            if (viaje->fecha <= fechaPrimero)
            {
                aux -> siguiente = this -> primero;
                this -> primero -> anterior = aux;
                this -> primero = aux;
            }
            else if (viaje->fecha >= fechaUltimo)
            {
                aux -> anterior = this -> ultimo;
                this -> ultimo -> siguiente = aux;
                this -> ultimo = aux;
            }
            //si la fecha es posterior a la del primero y anterior al del ultimo, compara para ver desde que lado iterar para asegurar orden por fecha.
            else {
                int difPrimero = viaje->fecha - fechaPrimero;
                int difUltimo = fechaUltimo - viaje->fecha;
                NodoListaDobleEnlazada<Viaje> * nodoActual = nullptr;

                if(difPrimero <= difUltimo)
                {
                    nodoActual = this -> primero -> siguiente;
                    while(nodoActual -> dato->fecha <= viaje->fecha)
                    {
                        nodoActual = nodoActual -> siguiente;
                    }
                    nodoActual = nodoActual -> anterior;

                }
                else
                {
                    nodoActual = this -> ultimo -> anterior;
                    while(nodoActual -> dato->fecha >= viaje->fecha)
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
    for (NodoListaDobleEnlazada<Viaje>* n = primero; n; )
    {
        NodoListaDobleEnlazada<Viaje>* sig = n->siguiente; 
        delete n->dato;
        delete n; 
        n = sig;
    }
} 
};

int cantidadDeViajesEntreFechaYFecha(ListaDeViajes* lista, int fecha1, int fecha2){
    const auto* primero = lista->getPrimero();
    const auto* ultimo = lista->getUltimo();
    int cantidadDeViajes=0;

    if (fecha1 > fecha2) swap(fecha1,fecha2);

    if (!lista->vacia())
    {
        int diferenciaInicio = fecha1 - primero->dato->fecha;
        int diferenciaFinal = ultimo->dato->fecha - fecha2;

        auto* nodoActual = primero;
        if(diferenciaInicio<=diferenciaFinal)
        {
            while(nodoActual != nullptr && nodoActual->dato->fecha <= fecha2)
            {
                if(nodoActual->dato->fecha >= fecha1)
                {
                    cantidadDeViajes += 1;
                }
                nodoActual = nodoActual->siguiente;
            }
        }
        else
        {
            nodoActual = ultimo;
            while(nodoActual!= nullptr && nodoActual->dato->fecha >= fecha1)
            {
                if(nodoActual->dato->fecha <= fecha2)
                {
                    cantidadDeViajes += 1;
                }
                nodoActual = nodoActual->anterior;
            }
        }
    }

    return cantidadDeViajes;
}

int minutosPedaleadosPorusuario(ListaDeViajes* lista, int dniUsuario)
{
    int minutosPedaleados = 0;
    auto* nodoActual = lista->getPrimero();

    while (nodoActual != nullptr)
    {
        if (nodoActual->dato->dni_usuario == dniUsuario)
        {
            minutosPedaleados += nodoActual->dato->duracion_min;
        }

        nodoActual = nodoActual->siguiente;
    }


    return minutosPedaleados;
}


void llenarConDatos(ListaDeViajes* lista)
{
    string linea = "", estacion_origen = "", estacion_destino = "";
    int viaje_id = 0, bici_id = 0, dni_usuario = 0, fecha = 0, duracion_min = 0;
    fstream DatosDeViaje;
    DatosDeViaje.open("./src/datos de entrada/viajes.txt");

    while (DatosDeViaje >> viaje_id >> bici_id >> dni_usuario >> estacion_origen >> estacion_destino >> fecha >> duracion_min)
    {
        Viaje * aux;
        aux->id = viaje_id;
        aux->bici_id = bici_id;
        aux->dni_usuario = dni_usuario;
        aux->estacion_origen = estacion_origen;
        aux->estacion_destino = estacion_destino;
        aux->fecha = fecha;
        aux->duracion_min = duracion_min;

        lista->push(aux);
    }

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
            aux -> dni = dni;
            aux -> siguiente = nullptr;

            if (ultimo == nullptr)
            {
                primero = aux;
                ultimo = aux;
            }
            else
            {
                ultimo -> siguiente = aux;
                ultimo = aux;
            }
        }

        int Pop()
        {
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


int main()
{
    ListaDeViajes lista;
    llenarConDatos(&lista);

    return 0;
}