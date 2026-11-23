#include <iostream>
#include <limits>
#include <string>

using namespace std;

struct EstacionFaltante {
    string codigo;
    int faltante;
};

class RedistribucionCamion {
private:
    EstacionFaltante* estaciones;
    string* subconjuntoActual;
    int cantidadEstaciones;
    int capacidadCamion;
    int cantidadSeleccionada;
    int solucionesEncontradas;

    void imprimirSubconjunto() const {
        cout << "  Opcion " << solucionesEncontradas << ": ";
        for (int i = 0; i < cantidadSeleccionada; ++i) {
            cout << subconjuntoActual[i];
            if (i + 1 < cantidadSeleccionada) {
                cout << " + ";
            }
        }
        cout << "\n";
    }

    void buscarSubconjuntosRec(int indiceActual, int sumaActual) {
        if (sumaActual == capacidadCamion) {
            ++solucionesEncontradas;
            imprimirSubconjunto();
            return;
        }

        if (sumaActual > capacidadCamion || indiceActual == cantidadEstaciones) {
            return;
        }

        subconjuntoActual[cantidadSeleccionada] = estaciones[indiceActual].codigo;
        ++cantidadSeleccionada;
        buscarSubconjuntosRec(indiceActual + 1, sumaActual + estaciones[indiceActual].faltante);
        --cantidadSeleccionada;

        buscarSubconjuntosRec(indiceActual + 1, sumaActual);
    }

public:
    RedistribucionCamion(const EstacionFaltante* lista, int total, int capacidad)
        : estaciones(new EstacionFaltante[total]),
          subconjuntoActual(new string[total]),
          cantidadEstaciones(total),
          capacidadCamion(capacidad),
          cantidadSeleccionada(0),
          solucionesEncontradas(0) {
        for (int i = 0; i < total; ++i) {
            estaciones[i] = lista[i];
        }
    }

    RedistribucionCamion(const RedistribucionCamion&) = delete;
    RedistribucionCamion& operator=(const RedistribucionCamion&) = delete;

    ~RedistribucionCamion() {
        delete[] estaciones;
        delete[] subconjuntoActual;
    }

    void ejecutarRedistribucion() {
        cout << "\nBuscando combinaciones de estaciones que completen exactamente "
             << capacidadCamion << " bicicletas...\n\n";

        cantidadSeleccionada = 0;
        solucionesEncontradas = 0;
        buscarSubconjuntosRec(0, 0);

        if (solucionesEncontradas == 0) {
            cout << "No hay ninguna combinacion de estaciones que sume exactamente "
                 << capacidadCamion << " bicicletas.\n";
        } else if (solucionesEncontradas == 1) {
            cout << "\nSe encontro 1 combinacion posible.\n";
        } else {
            cout << "\nSe encontraron " << solucionesEncontradas << " combinaciones posibles.\n";
        }
    }
};

int leerEntero(const string& mensaje, int minimo) {
    int valor = 0;
    while (true) {
        cout << mensaje;
        if (cin >> valor && valor >= minimo) {
            return valor;
        }
        if (cin.eof()) {
            return minimo;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Ese valor no sirve. Ingresa un numero entero mayor o igual a " << minimo << ".\n";
    }
}

void probarEjemploEnunciado() {
    EstacionFaltante ejemplo[5] = {
        {"PLZ", 3}, {"CAB", 7}, {"RET", 5}, {"CON", 2}, {"BOE", 5}
    };

    cout << "\nEjemplo del enunciado: un camion con lugar para 10 bicicletas.\n"
         << "Faltan bicicletas en: PLZ (3), CAB (7), RET (5), CON (2) y BOE (5).\n";

    RedistribucionCamion redistribuidor(ejemplo, 5, 10);
    redistribuidor.ejecutarRedistribucion();
}

void probarConDatosPropios() {
    cout << "\n";
    int capacidad = leerEntero("Cuantas bicicletas puede llevar el camion? ", 1);
    int total = leerEntero("Cuantas estaciones necesitan bicicletas? ", 1);

    EstacionFaltante* lista = new EstacionFaltante[total];
    for (int i = 0; i < total; ++i) {
        cout << "\nEstacion " << (i + 1) << " de " << total << "\n";
        cout << "  Codigo (por ejemplo PLZ): ";
        cin >> lista[i].codigo;
        lista[i].faltante = leerEntero("  Cuantas bicicletas le faltan? ", 1);
    }

    RedistribucionCamion redistribuidor(lista, total, capacidad);
    redistribuidor.ejecutarRedistribucion();

    delete[] lista;
}

int main() {
    int opcion = -1;
    while (opcion != 0) {
        cout << "\n=== Redistribucion de bicicletas ===\n"
             << "1. Ver el ejemplo del enunciado\n"
             << "2. Cargar mis propios datos\n"
             << "0. Salir\n";
        opcion = leerEntero("Elegi una opcion: ", 0);

        if (opcion == 1) {
            probarEjemploEnunciado();
        } else if (opcion == 2) {
            probarConDatosPropios();
        } else if (opcion != 0) {
            cout << "Esa opcion no existe, proba de nuevo.\n";
        }
    
    return 0;
}