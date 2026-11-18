#include <iostream>
#include <string>

#include "estaciones.cpp"
#include "ciclovias.cpp"

using namespace std;

void mostrarMenu()
{
    cout << "\n--- Gestion de estaciones ---\n"
         << "1. Mostrar estacion\n"
         << "2. Agregar estacion\n"
         << "3. Eliminar estacion\n"
         << "4. Listar estaciones\n"
         << "5. Listar estaciones con menos del 20% de ocupacion\n"
         << "6. Buscar recorrido entre estaciones\n"
         << "0. Salir\n"
         << "Seleccionar una opcion: ";
}

int main()
{
    TablaHashEstaciones tabla;
    GrafoCiclovias grafo;

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
        case 0:
            cout << "Menú finalizado\n";
            break;
        default:
            cout << "Opcion invalida\n";
        }
    }
    while (opcion != 0);

    return 0;
}