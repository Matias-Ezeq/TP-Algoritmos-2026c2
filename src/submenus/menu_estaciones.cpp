// Este archivo NO se compila por separado: se incluye desde main.cpp.
#pragma once

void menuEstaciones(Sistema& sistema)
{
    int opcion;

    do
    {
        cout << "\n--- A. Gestion de estaciones ---\n"
             << "1. Mostrar estacion\n"
             << "2. Agregar estacion\n"
             << "3. Eliminar estacion\n"
             << "4. Listar estaciones ordenadas\n"
             << "5. Estaciones con menos del 20% de ocupacion\n"
             << "6. Recorrido con menos tramos por ciclovias (BFS)\n"
             << "7. Colisiones y factor de carga de la tabla\n"
             << "0. Volver\n";

        opcion = leerEntero("Seleccionar una opcion: ");

        switch (opcion)
        {
        case 1:
        {
            string codigo;
            cout << "Codigo de la estacion: ";
            cin >> codigo;
            sistema.tabla.mostrarEstacionPorCodigo(codigo);
            break;
        }
        case 2:
        {
            string codigo;
            string nombre;
            string barrio;

            cout << "Codigo: ";
            cin >> codigo;
            cout << "Nombre: ";
            cin >> nombre;
            cout << "Barrio: ";
            cin >> barrio;

            int capacidad = leerEntero("Capacidad de anclajes: ");
            int disponibles = leerEntero("Bicicletas disponibles: ");

            Estacion nueva(codigo, nombre, barrio, capacidad, disponibles);

            if (!sistema.tabla.agregarEstacion(nueva))
            {
                cout << "No se pudo agregar: el codigo ya existe.\n";
                break;
            }

            if (!sistema.grafo.agregarEstacion(codigo))
            {
                sistema.tabla.eliminarEstacion(codigo);
                cout << "No se pudo agregar la estacion al grafo.\n";
                break;
            }

            cout << "Estacion agregada correctamente.\n";
            break;
        }
        case 3:
        {
            string codigo;
            cout << "Codigo de la estacion: ";
            cin >> codigo;

            sistema.tabla.eliminarEstacion(codigo);
            sistema.grafo.eliminarEstacion(codigo);
            break;
        }
        case 4:
        {
            int criterio = leerEntero(
                "1. Capacidad de anclajes / 2. Bicicletas disponibles: "
            );

            if (criterio == 1)
            {
                listarEstacionesOrdenadas(sistema.tabla, true);
            }
            else if (criterio == 2)
            {
                listarEstacionesOrdenadas(sistema.tabla, false);
            }
            else
            {
                cout << "Opcion invalida.\n";
            }

            break;
        }
        case 5:
            listarEstacionesBajaOcupacion(sistema.tabla);
            break;
        case 6:
        {
            string origen;
            string destino;

            cout << "Codigo de origen: ";
            cin >> origen;
            cout << "Codigo de destino: ";
            cin >> destino;

            sistema.grafo.mostrarRecorridoBFS(origen, destino);
            break;
        }
        case 7:
            cout << "Estaciones cargadas: "
                 << sistema.tabla.obtenerCantidadEstaciones() << '\n'
                 << "Colisiones: "
                 << sistema.tabla.obtenerCantidadColisiones() << '\n'
                 << "Factor de carga: "
                 << sistema.tabla.obtenerFactorCarga() << '\n';
            break;
        case 0:
            break;
        default:
            cout << "Opcion invalida.\n";
        }
    }
    while (opcion != 0);
}
