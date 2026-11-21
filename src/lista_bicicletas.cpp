#include "ejercicio_arbol.c++"
#include "modelos.h"
#include "estructuras.h"

using namespace std;

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