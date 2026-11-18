#ifndef VIAJES_H
#define VIAJES_H

#include <string>

struct Viaje
{
    int id = 0;
    int bici_id = 0;
    int dni_usuario = 0;
    std::string estacion_origen;
    std::string estacion_destino;
    int fecha = 0;
    int duracion_min = 0;
};

// Lista ordenada doblemente enlazada para permitir busquedas e inserciones
// ordenadas desde cualquier extremo. Complejidad O(N) de busqueda y O(log N)
// de insercion, ya que elige el extremo con menor diferencia respecto de la
// fecha del viaje a insertar.
class ListaDeViajes
{
    struct Nodo
    {
        Viaje viaje;
        Nodo* siguiente = nullptr;
        Nodo* anterior = nullptr;

        explicit Nodo(const Viaje& viaje) : viaje(viaje) {}
    };

    Nodo* primero = nullptr;
    Nodo* ultimo = nullptr;

public:
    ~ListaDeViajes()
    {
        Nodo* actual = primero;

        while (actual != nullptr)
        {
            Nodo* siguiente = actual->siguiente;
            delete actual;
            actual = siguiente;
        }

        primero = nullptr;
        ultimo = nullptr;
    }

    bool vacia() const
    {
        return primero == nullptr;
    }

    const Nodo* getPrimero() const
    {
        return primero;
    }

    const Nodo* getUltimo() const
    {
        return ultimo;
    }

    void push(const Viaje& viaje)
    {
        Nodo* aux = new Nodo(viaje);

        if (vacia())
        {
            primero = aux;
            ultimo = aux;
        }
        else
        {
            int fechaPrimero = primero->viaje.fecha;
            int fechaUltimo = ultimo->viaje.fecha;

            if (viaje.fecha <= fechaPrimero)
            {
                aux->siguiente = primero;
                primero->anterior = aux;
                primero = aux;
            }
            else if (viaje.fecha >= fechaUltimo)
            {
                aux->anterior = ultimo;
                ultimo->siguiente = aux;
                ultimo = aux;
            }
            // Si la fecha es posterior a la del primero y anterior a la del
            // ultimo, compara para ver desde que lado iterar.
            else
            {
                int difPrimero = viaje.fecha - fechaPrimero;
                int difUltimo = fechaUltimo - viaje.fecha;
                Nodo* nodoActual;

                if (difPrimero <= difUltimo)
                {
                    nodoActual = primero->siguiente;

                    while (nodoActual->viaje.fecha <= viaje.fecha)
                    {
                        nodoActual = nodoActual->siguiente;
                    }

                    nodoActual = nodoActual->anterior;
                }
                else
                {
                    nodoActual = ultimo->anterior;

                    while (nodoActual->viaje.fecha >= viaje.fecha)
                    {
                        nodoActual = nodoActual->anterior;
                    }
                }

                aux->siguiente = nodoActual->siguiente;
                aux->anterior = nodoActual;
                nodoActual->siguiente = aux;
                aux->siguiente->anterior = aux;
            }
        }
    }
};

void llenarConDatos(ListaDeViajes* lista);

int cantidadDeViajesEntreFechaYFecha(
    ListaDeViajes* lista,
    int fecha1,
    int fecha2,
    const std::string& estacion
);

int minutosPedaleadosPorusuario(ListaDeViajes* lista, int dniUsuario);

#endif
