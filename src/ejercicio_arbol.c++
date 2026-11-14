#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

struct NodoBici {
    int biciId;
    string codigoEstacion;
    string tipo;
    string estado;
    int kilometros;
    NodoBici* hijoIzquierda;
    NodoBici* hijoDerecha;

    NodoBici(int id, string est, string t, string e, int km)
        : biciId(id), codigoEstacion(est), tipo(t), estado(e), kilometros(km),
          hijoIzquierda(nullptr), hijoDerecha(nullptr) {}
};

class ArbolBicicletas {
private:
    NodoBici* raiz;

    NodoBici* insertarRecursivo(NodoBici* actual, int id, string est, string t, string e, int km) {
        if (actual == nullptr) {
            return new NodoBici(id, est, t, e, km);
        }
        if (id < actual->biciId) {
            actual->hijoIzquierda = insertarRecursivo(actual->hijoIzquierda, id, est, t, e, km);
        } else if (id > actual->biciId) {
            actual->hijoDerecha = insertarRecursivo(actual->hijoDerecha, id, est, t, e, km);
        }
        return actual;
    }

    NodoBici* buscarRecursivo(NodoBici* actual, int id) const {
        if (actual == nullptr || actual->biciId == id) {
            return actual;
        }
        if (id < actual->biciId) {
            return buscarRecursivo(actual->hijoIzquierda, id);
        }
        return buscarRecursivo(actual->hijoDerecha, id);
    }

    void inordenRecursivo(NodoBici* actual) const {
        if (actual != nullptr) {
            inordenRecursivo(actual->hijoIzquierda);
            cout << "  ID: " << actual->biciId 
                 << "  Estacion: " << actual->codigoEstacion 
                 << "  Tipo: " << actual->tipo 
                 << "  Estado: " << actual->estado 
                 << "  Km: " << actual->kilometros << endl;
            inordenRecursivo(actual->hijoDerecha);
        }
    }

    int contarAveriadasRecursivo(NodoBici* actual) const {
        if (actual == nullptr) return 0;
        int contadorLocal = (actual->estado == "Averiada") ? 1 : 0;
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

        if (id < actual->biciId) {
            actual->hijoIzquierda = eliminarRecursivo(actual->hijoIzquierda, id);
        } else if (id > actual->biciId) {
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
            NodoBici* temp = obtenerMinimo(actual->hijoDerecha);
            actual->biciId = temp->biciId;
            actual->codigoEstacion = temp->codigoEstacion;
            actual->tipo = temp->tipo;
            actual->estado = temp->estado;
            actual->kilometros = temp->kilometros;
            actual->hijoDerecha = eliminarRecursivo(actual->hijoDerecha, temp->biciId);
        }
        return actual;
    }

public:
    ArbolBicicletas() : raiz(nullptr) {}

    ~ArbolBicicletas() {
        liberarPostorden(raiz);
        raiz = nullptr;
    }

    void insertar(int id, string est, string t, string e, int km) {
        raiz = insertarRecursivo(raiz, id, est, t, e, km);
    }

    bool buscarPorId(int id) const {
        NodoBici* buscado = buscarRecursivo(raiz, id);
        if (buscado != nullptr) {
            cout << "Bicicleta Encontrada -> ID: " << buscado->biciId 
                 << " | Estacion: " << buscado->codigoEstacion 
                 << " | Tipo: " << buscado->tipo 
                 << " | Estado: " << buscado->estado 
                 << " | Km: " << buscado->kilometros << endl;
            return true;
        }
        cout << "Bicicleta con ID " << id << " no encontrada en el sistema." << endl;
        return false;
    }

    void listarInorden() const {
        cout << "--- Lista de bicicletas ---\n";
        inordenRecursivo(raiz);
    }

    int contarAveriadas() const {
        return contarAveriadasRecursivo(raiz);
    }

    int calcularAltura() const {
        return calcularAlturaRecursivo(raiz);
    }

    void eliminar(int id) {
        raiz = eliminarRecursivo(raiz, id);
    }
};