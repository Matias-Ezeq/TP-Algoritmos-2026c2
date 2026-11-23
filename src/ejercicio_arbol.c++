#include <algorithm>
#include <iostream>
#include <string>

#include "modelos.h"

using namespace std;

struct NodoBici {
    Bicicleta* bici;
    NodoBici* hijoIzquierda;
    NodoBici* hijoDerecha;

    NodoBici(int id, string est, string t, string e, int km)
        : bici(new Bicicleta(id,est,t,e,km)), hijoIzquierda(nullptr), hijoDerecha(nullptr) {}

    ~NodoBici() {
        delete bici;
    }
};

class ArbolBicicletas {
private:
    NodoBici* raiz;

    NodoBici* insertarRecursivo(NodoBici* actual, int id, string est, string t, string e, int km) {
        if (actual == nullptr) {
            return new NodoBici(id, est, t, e, km);
        }
        if (id < actual->bici->getBiciId()) {
            actual->hijoIzquierda = insertarRecursivo(actual->hijoIzquierda, id, est, t, e, km);
        } else if (id > actual->bici->getBiciId()) {
            actual->hijoDerecha = insertarRecursivo(actual->hijoDerecha, id, est, t, e, km);
        }
        return actual;
    }

    NodoBici* buscarRecursivo(NodoBici* actual, int id) const {
        if (actual == nullptr || actual->bici->getBiciId() == id) {
            return actual;
        }
        if (id < actual->bici->getBiciId()) {
            return buscarRecursivo(actual->hijoIzquierda, id);
        }
        return buscarRecursivo(actual->hijoDerecha, id);
    }

    void inordenRecursivo(NodoBici* actual) const {
        if (actual != nullptr) {
            inordenRecursivo(actual->hijoIzquierda);
            cout << "  ";
            actual->bici->mostrarDatos();
            inordenRecursivo(actual->hijoDerecha);
        }
    }

    int contarAveriadasRecursivo(NodoBici* actual) const {
        if (actual == nullptr) return 0;
        int contadorLocal = (actual->bici->getEstado() == "Averiada") ? 1 : 0;
        return contadorLocal + contarAveriadasRecursivo(actual->hijoIzquierda) 
               + contarAveriadasRecursivo(actual->hijoDerecha);
    }

    int calcularAlturaRecursivo(NodoBici* actual) const {
        if (actual == nullptr) return 0;
        return 1 + max(calcularAlturaRecursivo(actual->hijoIzquierda), 
                       calcularAlturaRecursivo(actual->hijoDerecha));
    }

    void liberarPostorden(NodoBici* actual) {
        if (actual != nullptr) {
            liberarPostorden(actual->hijoIzquierda);
            liberarPostorden(actual->hijoDerecha);
            delete actual;
        }
    }

    NodoBici* obtenerMinimo(NodoBici* actual) {
        while (actual != nullptr && actual->hijoIzquierda != nullptr) {
            actual = actual->hijoIzquierda;
        }
        return actual;
    }

    NodoBici* eliminarRecursivo(NodoBici* actual, int id) {
        if (actual == nullptr) return nullptr;

        if (id < actual->bici->getBiciId()) {
            actual->hijoIzquierda = eliminarRecursivo(actual->hijoIzquierda, id);
        } else if (id > actual->bici->getBiciId()) {
            actual->hijoDerecha = eliminarRecursivo(actual->hijoDerecha, id);
        } else {
            if (actual->hijoIzquierda == nullptr && actual->hijoDerecha == nullptr) {
                delete actual;
                return nullptr;
            }
            else if (actual->hijoIzquierda == nullptr) {
                NodoBici* temp = actual->hijoDerecha;
                delete actual;
                return temp;
            }
            else if (actual->hijoDerecha == nullptr) {
                NodoBici* temp = actual->hijoIzquierda;
                delete actual;
                return temp;
            }
            // Se intercambia la bicicleta con el sucesor (minimo del
            // subarbol derecho) y se elimina el nodo sucesor. Asi se
            // conservan los dos subarboles y no se pierde memoria.
            NodoBici* sucesor = obtenerMinimo(actual->hijoDerecha);
            Bicicleta* biciEliminada = actual->bici;
            int idEliminado = biciEliminada->getBiciId();

            actual->bici = sucesor->bici;
            sucesor->bici = biciEliminada;
            actual->hijoDerecha = eliminarRecursivo(
                actual->hijoDerecha,
                idEliminado
            );
        }
        return actual;
    }

public:
    ArbolBicicletas() : raiz(nullptr) {}

    ArbolBicicletas(const ArbolBicicletas&) = delete;
    ArbolBicicletas& operator=(const ArbolBicicletas&) = delete;

    ~ArbolBicicletas() {
        liberarPostorden(raiz);
        raiz = nullptr;
    }

    NodoBici* obtenerRaiz() const {
        return raiz;
    }

    bool insertar(int id, string est, string t, string e, int km) {
        if (buscarRecursivo(raiz, id) != nullptr) {
            return false;
        }

        raiz = insertarRecursivo(raiz, id, est, t, e, km);
        return true;
    }

    bool buscarPorId(int id) const {
        NodoBici* buscado = buscarRecursivo(raiz, id);
        if (buscado != nullptr) {
            cout << "Bicicleta> ";
            buscado->bici->mostrarDatos();
            return true;
        }
        cout << "Bicicleta con id " << id << " no encontrada en el sistema." << endl;
        return false;
    }

    Bicicleta* obtenerPorId(int id) const
    {
        NodoBici* buscado = buscarRecursivo(raiz, id);

        if (buscado == nullptr)
        {
            return nullptr;
        }

        return buscado->bici;
    }

    void listarInorden() const {
        cout << " Lista de bicicletas \n";
        inordenRecursivo(raiz);
    }

    int contarAveriadas() const {
        return contarAveriadasRecursivo(raiz);
    }

    int calcularAltura() const {
        return calcularAlturaRecursivo(raiz);
    }

    bool eliminar(int id) {
        if (buscarRecursivo(raiz, id) == nullptr) {
            return false;
        }

        raiz = eliminarRecursivo(raiz, id);
        return true;
    }
};