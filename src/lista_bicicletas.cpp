#include "ejercicio_arbol.c++"
#include "modelos.h"
#include "estructuras.h"

using namespace std;

void recorrerArbolRecursivo(NodoBici * nodoActual, ListaEnlazada<Bicicleta> * bicicletas,int kilometraje)
{
    if (nodoActual != nullptr)
    {
        Bicicleta* bicicleta = nodoActual->bici;
        if (bicicleta->getKilometros() > kilometraje)
        {
            // La lista es propietaria de sus datos: se guarda una copia para
            // no liberar las bicicletas que pertenecen al arbol.
            bicicletas->push(new Bicicleta(*bicicleta));
        }
        recorrerArbolRecursivo(nodoActual->hijoIzquierda,bicicletas,kilometraje);
        recorrerArbolRecursivo(nodoActual->hijoDerecha,bicicletas,kilometraje);
    }
}

ListaEnlazada<Bicicleta> * bicicletasAReparar(ArbolBicicletas * arbol, int kilometraje)
{
    ListaEnlazada<Bicicleta> * bicicletas = new ListaEnlazada<Bicicleta>();

    recorrerArbolRecursivo(arbol->obtenerRaiz(),bicicletas,kilometraje);

    return  bicicletas;
}