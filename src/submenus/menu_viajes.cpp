// Este archivo NO se compila por separado: se incluye desde main.cpp.
#pragma once

void listarBicicletasPorKilometraje(
    NodoBici* nodo,
    int kilometros,
    int& encontradas
)
{
    if (nodo == nullptr)
    {
        return;
    }

    listarBicicletasPorKilometraje(nodo->hijoIzquierda, kilometros, encontradas);

    if (nodo->bici->getKilometros() > kilometros)
    {
        nodo->bici->mostrarDatos();
        encontradas++;
    }

    listarBicicletasPorKilometraje(nodo->hijoDerecha, kilometros, encontradas);
}

void menuViajes(Sistema& sistema)
{
    int opcion;

    do
    {
        cout << "\n--- B. Viajes y colas de espera ---\n"
             << "1. Cantidad de viajes desde una estacion en un rango de fechas\n"
             << "2. Viajes de un usuario y total de minutos\n"
             << "3. Bicicletas con mas de X kilometros\n"
             << "4. Cola de espera de una estacion\n"
             << "5. Taller de reparaciones (cola de prioridad)\n"
             << "0. Volver\n";

        opcion = leerEntero("Seleccionar una opcion: ");

        switch (opcion)
        {
        case 1:
        {
            string estacion;
            cout << "Codigo de la estacion de origen: ";
            cin >> estacion;

            int fecha1 = leerEntero("Fecha desde (AAAAMMDD): ");
            int fecha2 = leerEntero("Fecha hasta (AAAAMMDD): ");

            int desde = (fecha1 < fecha2) ? fecha1 : fecha2;
            int hasta = (fecha1 < fecha2) ? fecha2 : fecha1;

            int cantidad = cantidadDeViajesEntreFechaYFecha(
                &sistema.viajes,
                desde,
                hasta,
                estacion
            );

            cout << "Viajes desde " << estacion << " entre " << desde
                 << " y " << hasta << ": " << cantidad << '\n';
            break;
        }
        case 2:
        {
            int dni = leerEntero("DNI del usuario: ");
            int cantidad = 0;
            auto* nodo = sistema.viajes.getPrimero();

            while (nodo != nullptr)
            {
                if (nodo->viaje.dniUsuario == dni)
                {
                    cout << "  Viaje " << nodo->viaje.id
                         << " | bici " << nodo->viaje.biciId
                         << " | " << nodo->viaje.estacionOrigen
                         << " -> " << nodo->viaje.estacionDestino
                         << " | " << nodo->viaje.fecha
                         << " | " << nodo->viaje.duracionMin << " min\n";
                    cantidad++;
                }

                nodo = nodo->siguiente;
            }

            if (cantidad == 0)
            {
                cout << "El usuario no tiene viajes registrados.\n";
            }
            else
            {
                cout << "Total: " << cantidad << " viajes, "
                     << minutosPedaleadosPorUsuario(&sistema.viajes, dni)
                     << " minutos pedaleados.\n";
            }

            break;
        }
        case 3:
        {
            int kilometros = leerEntero("Kilometros minimos (X): ");
            int encontradas = 0;

            listarBicicletasPorKilometraje(
                sistema.arbol.obtenerRaiz(),
                kilometros,
                encontradas
            );

            if (encontradas == 0)
            {
                cout << "No hay bicicletas con mas de " << kilometros
                     << " km.\n";
            }

            break;
        }
        case 4:
        {
            int accion = leerEntero(
                "1. Retirar bicicleta / 2. Devolver bicicleta: "
            );

            string codigo;
            cout << "Codigo de la estacion: ";
            cin >> codigo;

            Estacion* estacion = sistema.tabla.buscar(codigo);

            if (estacion == nullptr)
            {
                cout << "No existe una estacion con ese codigo.\n";
                break;
            }

            if (accion == 1)
            {
                int dni = leerEntero("DNI del usuario: ");

                if (estacion->bicisDisponibles > 0)
                {
                    estacion->bicisDisponibles--;
                    cout << "Bicicleta retirada. Quedan "
                         << estacion->bicisDisponibles
                         << " en la estacion.\n";
                }
                else
                {
                    sistema.esperas.encolar(codigo, dni);
                    cout << "No hay bicicletas disponibles. El usuario "
                         << dni << " quedo en la cola de espera.\n";
                }
            }
            else if (accion == 2)
            {
                if (sistema.esperas.hayEspera(codigo))
                {
                    int dni = sistema.esperas.desencolar(codigo);
                    cout << "Bicicleta asignada al usuario " << dni
                         << " (primero de la cola).\n";
                }
                else if (estacion->bicisDisponibles < estacion->capacidad)
                {
                    estacion->bicisDisponibles++;
                    cout << "Bicicleta devuelta. Ahora hay "
                         << estacion->bicisDisponibles
                         << " en la estacion.\n";
                }
                else
                {
                    cout << "La estacion no tiene anclajes libres.\n";
                }
            }
            else
            {
                cout << "Opcion invalida.\n";
            }

            break;
        }
        case 5:
        {
            int accion = leerEntero(
                "1. Insertar bicicleta / 2. Extraer mas urgente: "
            );

            if (accion == 1)
            {
                int biciId = leerEntero("ID de la bicicleta: ");
                Bicicleta* bicicleta =
                    sistema.arbol.obtenerPorId(biciId);

                if (bicicleta == nullptr)
                {
                    cout << "No existe una bicicleta con ese ID.\n";
                    break;
                }

                sistema.taller.insertar(bicicleta);
                cout << "Bicicleta " << biciId
                     << " insertada en el taller.\n";
            }
            else if (accion == 2)
            {
                Bicicleta* bicicleta =
                    sistema.taller.extraerMasUrgente();

                if (bicicleta == nullptr)
                {
                    cout << "El taller no tiene bicicletas en espera.\n";
                }
                else
                {
                    cout << "Se extrajo la bicicleta mas urgente:\n";
                    bicicleta->mostrarDatos();
                }
            }
            else
            {
                cout << "Opcion invalida.\n";
            }

            break;
        }
        case 0:
            break;
        default:
            cout << "Opcion invalida.\n";
        }
    }
    while (opcion != 0);
}
