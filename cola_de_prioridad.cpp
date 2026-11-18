#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class Bicicleta {
private:
    int biciId;
    string codigoEstacion;
    string tipo;
    string estado;
    int kilometros;

public:
    Bicicleta(int id, string est, string t, string e, int km)
        : biciId(id), codigoEstacion(est), tipo(t), estado(e), kilometros(km) {}

    int getBiciId() const { return biciId; }
    string getCodigoEstacion() const { return codigoEstacion; }
    string getTipo() const { return tipo; }
    string getEstado() const { return estado; }
    int getKilometros() const { return kilometros; }

    void setCodigoEstacion(const string& est) { codigoEstacion = est; }
    void setEstado(const string& nuevoEstado) { estado = nuevoEstado; }
    void setKilometros(int km) { kilometros = km; }

    void mostrarDatos() const {
        cout << "Id: " << biciId 
             << " | Estacion: " << codigoEstacion 
             << " | Tipo: " << tipo 
             << " | Estado: " << estado 
             << " | Km: " << kilometros << endl;
    }
};

class ColaDePrioridad
{
    enum prioridad
    {
        nula, alta, media, baja
    };
    struct Nodo {
        Bicicleta * bici;
        int prioridad = nula;
        Nodo * siguiente = nullptr;
        Nodo * anterior = nullptr;

        Nodo(Bicicleta* nuevaBici)
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
        };
    };

    Nodo * primero = nullptr;
    Nodo * ultimo = nullptr;

    void insertarRecorriendo(Nodo * nuevoNodo)
    {
        //complejidad O(n)
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
            Nodo * nodoActual = primero->siguiente;
            while(nuevoNodo->prioridad >= nodoActual->prioridad)
            {
                nodoActual = nodoActual->siguiente;
            }

            nuevoNodo->siguiente = nodoActual->siguiente;
            nuevoNodo->anterior = nodoActual;

            nodoActual->siguiente->anterior = nuevoNodo;
            nodoActual->siguiente = nuevoNodo;
        }
    }


    
    public:

        void insertar(Bicicleta * bicicleta)
        {
            Nodo * aux = new Nodo(bicicleta);

            if (primero = nullptr)
            {
                primero = aux;
                ultimo = aux;
            }
            else
            {
                insertarRecorriendo(aux);
            }
            
        }

        Bicicleta * extraer()
        {
            Bicicleta * bicicleta = primero->bici;
            Nodo * aux = primero;

            primero = aux->siguiente;

            aux->siguiente = nullptr;
            aux->anterior = nullptr;
            delete aux;


            return bicicleta;
        }

};