#include <iostream>
#include <string>
#include <fstream>
#include <ejercicio_arbol.c++>
using namespace std;

template <typename T>
class ListaEnlazada
{
    struct Nodo
    {
        T* dato;
        Nodo* siguiente = nullptr;
    };

    Nodo * primero = nullptr;
    Nodo * ultimo = nullptr;

    public:
        Nodo * getPrimero(){
            return primero;
        }
    
        void push(T * dato)
        {
            if (primero == nullptr)
            {
                primero = dato;
                ultimo = primero;
            }
            else
            {
                ultimo->siguiente = dato;
                ultimo = dato;
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

void recorrerArbolRecursivo(NodoBici * nodoActual, ListaEnlazada<Bicicleta> * bicicletas,int kilometraje)
{
    if (nodoActual != nullptr)
    {
        Bicicleta* bicicleta = nodoActual->bici;
        if (bicicleta->getKilometros() >= kilometraje)
        {
            bicicletas->push(bicicleta);
        }
        recorrerArbolRecursivo(nodoActual->hijoIzquierda,bicicletas,kilometraje);
        recorrerArbolRecursivo(nodoActual->hijoDerecha,bicicletas,kilometraje);
    }
}

ListaEnlazada<Bicicleta> * bicicletasAReparar(ArbolBicicletas * arbol, int kilometraje)
{
    ListaEnlazada<Bicicleta> * bicicletas;

    recorrerArbolRecursivo(arbol->obtenerRaiz(),bicicletas,kilometraje);

    return  bicicletas;
}