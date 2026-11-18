#include <iostream>
#include <string>
#include <vector>

#include "cola_de_prioridad.h"
#include "cola_de_usuarios.h"
#include "estaciones.h"
#include "redistribucion.h"
#include "viajes.h"

using namespace std;

void mostrarMenu()
{
    cout << "\n--- Menu principal ---\n"
         << "1. Mostrar estacion\n"
         << "2. Agregar estacion\n"
         << "3. Eliminar estacion\n"
         << "4. Listar estaciones\n"
         << "5. Listar estaciones con menos del 20% de ocupacion\n"
         << "6. Buscar recorrido entre estaciones\n"
         << "7. Cantidad de viajes por estacion entre dos fechas\n"
         << "8. Minutos pedaleados por un usuario\n"
         << "9. Encolar usuario para atencion\n"
         << "10. Atender siguiente usuario\n"
         << "11. Insertar bicicleta en el taller (cola de prioridad)\n"
         << "12. Extraer bicicleta del taller\n"
         << "13. Calcular redistribucion con camion\n"
         << "0. Salir\n"
         << "Seleccionar una opcion: ";
}

void mostrarSoluciones(
    ListaDobleEnlazada<ListaDobleEnlazada<string>>* soluciones
)
{
    int cantidad = 0;
    auto* nodoSolucion = soluciones->getPrimero();

    while (nodoSolucion != nullptr)
    {
        cantidad++;

        vector<string> estaciones;
        auto* nodoEstacion = nodoSolucion->dato->getPrimero();

        while (nodoEstacion != nullptr)
        {
            estaciones.push_back(*(nodoEstacion->dato));
            nodoEstacion = nodoEstacion->siguiente;
        }

        // La pila guarda el camino al reves, se imprime en orden natural.
        cout << "Solucion " << cantidad << ": ";

        for (int i = static_cast<int>(estaciones.size()) - 1; i >= 0; i--)
        {
            cout << estaciones[i];

            if (i > 0)
            {
                cout << " + ";
            }
        }

        cout << '\n';
        nodoSolucion = nodoSolucion->siguiente;
    }

    if (cantidad == 0)
    {
        cout << "No hay combinaciones posibles con esa capacidad.\n";
    }
    else
    {
        cout << "Total de soluciones: " << cantidad << '\n';
    }
}

int main()
{
    TablaHashEstaciones tabla;
    GrafoCiclovias grafo;
    ListaDeViajes viajes;
    ColaDeUsuarios espera;
    ColaDePrioridad taller;

    if (!tabla.cargarDesdeArchivo(
            "src/datos de entrada/estaciones.txt"))
    {
        cout << "No se pudo cargar el archivo de estaciones.\n";
        return 1;
    }

    int cantidadEstaciones = tabla.obtenerCantidadEstaciones();
    Estacion* estaciones = new Estacion[cantidadEstaciones];
    tabla.copiarEstaciones(estaciones);

    for (int i = 0; i < cantidadEstaciones; i++)
    {
        if (!grafo.agregarEstacion(estaciones[i].codigo))
        {
            cout << "No se pudo agregar una estacion al grafo.\n";
            delete[] estaciones;
            return 1;
        }
    }

    delete[] estaciones;

    if (!grafo.cargarDesdeArchivo(
            "src/datos de entrada/ciclovias.txt"))
    {
        cout << "No se pudo cargar el archivo de ciclovias.\n";
        return 1;
    }

    llenarConDatos(&viajes);

    int opcion;

    do
    {
        mostrarMenu();
        cin >> opcion;

        switch (opcion)
        {
        case 1:
        {
            string codigo;
            cout << "Ingresar el codigo de la estacion: ";
            cin >> codigo;
            tabla.mostrarEstacionPorCodigo(codigo);
            break;
        }
        case 2:
        {
            string codigo;
            string nombre;
            string barrio;
            int capacidad;
            int bicisDisponibles;

            cout << "Ingresar el codigo: ";
            cin >> codigo;
            cout << "Ingresar el nombre: ";
            cin >> nombre;
            cout << "Ingresar el barrio: ";
            cin >> barrio;
            cout << "Ingresar la capacidad de anclajes: ";
            cin >> capacidad;
            cout << "Ingresar las bicicletas disponibles: ";
            cin >> bicisDisponibles;

            Estacion nuevaEstacion(
                codigo,
                nombre,
                barrio,
                capacidad,
                bicisDisponibles
            );

            if (!tabla.agregarEstacion(nuevaEstacion))
            {
                cout << "No se pudo agregar la estacion "
                     << "El codigo ya existe\n";
                break;
            }

            if (!grafo.agregarEstacion(codigo))
            {
                cout << "No se pudo agregar la estacion\n";
                tabla.eliminarEstacion(codigo);
                break;
            }

            cout << "La estacion fue agregada correctamente\n";
            break;
        }
        case 3:
        {
            string codigo;
            cout << "Ingresar el codigo de la estacion: ";
            cin >> codigo;
            tabla.eliminarEstacion(codigo);
            break;
        }
        case 4:
        {
            int criterio;
            cout << "1. Capacidad de anclajes\n"
                 << "2. Bicicletas disponibles\n"
                 << "Seleccione por que desea ordenar: ";
            cin >> criterio;

            if (criterio == 1)
            {
                listarEstacionesOrdenadas(tabla, true);
            }
            else if (criterio == 2)
            {
                listarEstacionesOrdenadas(tabla, false);
            }
            else
            {
                cout << "Opcion invalida";
            }

            break;
        }
        case 5:
            listarEstacionesBajaOcupacion(tabla);
            break;
        case 6:
        {
            string origen;
            string destino;

            cout << "Ingresar el codigo de origen: ";
            cin >> origen;
            cout << "Ingresar el codigo de destino: ";
            cin >> destino;

            grafo.mostrarRecorridoBFS(origen, destino);
            break;
        }
        case 7:
        {
            int fecha1;
            int fecha2;
            string estacion;

            cout << "Ingresar la fecha desde (AAAAMMDD): ";
            cin >> fecha1;
            cout << "Ingresar la fecha hasta (AAAAMMDD): ";
            cin >> fecha2;
            cout << "Ingresar el codigo de la estacion de origen: ";
            cin >> estacion;

            int cantidad = cantidadDeViajesEntreFechaYFecha(
                &viajes,
                fecha1,
                fecha2,
                estacion
            );

            cout << "Cantidad de viajes: " << cantidad << '\n';
            break;
        }
        case 8:
        {
            int dni;
            cout << "Ingresar el DNI del usuario: ";
            cin >> dni;

            int minutos = minutosPedaleadosPorusuario(&viajes, dni);
            cout << "Minutos pedaleados: " << minutos << '\n';
            break;
        }
        case 9:
        {
            int dni;
            cout << "Ingresar el DNI del usuario: ";
            cin >> dni;

            espera.Push(dni);
            cout << "El usuario fue encolado\n";
            break;
        }
        case 10:
        {
            if (espera.vacia())
            {
                cout << "No hay usuarios en espera\n";
                break;
            }

            cout << "Atendiendo al usuario con DNI "
                 << espera.Pop() << '\n';
            break;
        }
        case 11:
        {
            int id;
            string codigoEstacion;
            string tipo;
            string estado;
            int kilometros;

            cout << "Ingresar el id de la bicicleta: ";
            cin >> id;
            cout << "Ingresar el codigo de la estacion: ";
            cin >> codigoEstacion;
            cout << "Ingresar el tipo (Electrica/Mecanica): ";
            cin >> tipo;
            cout << "Ingresar el estado (OK/Averiada): ";
            cin >> estado;
            cout << "Ingresar los kilometros: ";
            cin >> kilometros;

            taller.insertar(new Bicicleta(
                id,
                codigoEstacion,
                tipo,
                estado,
                kilometros
            ));

            cout << "La bicicleta fue insertada en el taller\n";
            break;
        }
        case 12:
        {
            Bicicleta* bicicleta = taller.extraer();

            if (bicicleta == nullptr)
            {
                cout << "No hay bicicletas en el taller\n";
                break;
            }

            cout << "Bicicleta a reparar:\n";
            bicicleta->mostrarDatos();
            delete bicicleta;
            break;
        }
        case 13:
        {
            int capacidadCamion;
            cout << "Ingresar la capacidad del camion: ";
            cin >> capacidadCamion;

            if (capacidadCamion <= 0)
            {
                cout << "La capacidad debe ser mayor a cero\n";
                break;
            }

            int cantidad = tabla.obtenerCantidadEstaciones();
            Estacion* estacionesTabla = new Estacion[cantidad];
            int copiadas = tabla.copiarEstaciones(estacionesTabla);

            ListaDobleEnlazada<Tupla<string, int>> faltantes;

            for (int i = 0; i < copiadas; i++)
            {
                int faltante = estacionesTabla[i].capacidad
                               - estacionesTabla[i].bicisDisponibles;

                faltantes.push(new Tupla<string, int>(
                    estacionesTabla[i].codigo,
                    faltante
                ));
            }

            delete[] estacionesTabla;

            auto* soluciones = redistribuir(capacidadCamion, faltantes);
            mostrarSoluciones(soluciones);
            delete soluciones;
            break;
        }
        case 0:
            cout << "Menu finalizado\n";
            break;
        default:
            cout << "Opcion invalida\n";
        }
    }
    while (opcion != 0);

    return 0;
}
