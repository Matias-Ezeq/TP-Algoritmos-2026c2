// Este archivo NO se compila por separado: se incluye desde main.cpp.
#pragma once

EstacionFaltante * construirFaltantes(
    const TablaHashEstaciones & tabla,
    int capacidadCamion,
    int & total
)
{
    int cantidad = tabla.obtenerCantidadEstaciones();
    Estacion * estaciones = new Estacion[cantidad];
    tabla.copiarEstaciones(estaciones);

    int candidatas = 0;

    for (int i = 0; i < cantidad; i++)
    {
        int faltante = estaciones[i].capacidad - estaciones[i].bicisDisponibles;

        if (faltante > 0 && faltante <= capacidadCamion)
        {
            candidatas++;
        }
    }

    EstacionFaltante * salida = new EstacionFaltante[candidatas];
    int posicion = 0;

    for (int i = 0; i < cantidad; i++)
    {
        int faltante = estaciones[i].capacidad - estaciones[i].bicisDisponibles;

        if (faltante > 0 && faltante <= capacidadCamion)
        {
            salida[posicion].codigo = estaciones[i].codigo;
            salida[posicion].faltante = faltante;
            posicion++;
        }
    }

    delete[] estaciones;
    total = posicion;

    return salida;
}


void listarFaltantes(const TablaHashEstaciones & tabla)
{
    int cantidad = tabla.obtenerCantidadEstaciones();
    Estacion * estaciones = new Estacion[cantidad];
    tabla.copiarEstaciones(estaciones);

    int conFaltante = 0;
    int faltanteTotal = 0;

    cout << "\nCodigo | Capacidad | Disponibles | Faltan\n"
         << "-------+-----------+-------------+-------\n";

    for (int i = 0; i < cantidad; i++)
    {
        int faltante = estaciones[i].capacidad - estaciones[i].bicisDisponibles;

        if (faltante > 0)
        {
            cout << estaciones[i].codigo << " | "
                 << estaciones[i].capacidad << " | "
                 << estaciones[i].bicisDisponibles << " | "
                 << faltante << '\n';
            conFaltante++;
            faltanteTotal += faltante;
        }
    }

    if (conFaltante == 0)
    {
        cout << "Ninguna estacion necesita bicicletas.\n";
    }
    else
    {
        cout << "Estaciones con faltante: " << conFaltante
             << " | Faltantes en total: " << faltanteTotal << '\n';
    }

    delete[] estaciones;
}


const int maximoCandidatas = 22;


bool confirmarBusqueda(const string& mensaje)
{
    cout << mensaje;
    int respuesta = leerEntero("");
    return respuesta == 1;
}


void correrBacktracking(EstacionFaltante * candidatas, int total, int capacidad)
{
    if (total > maximoCandidatas)
    {
        cout << "Hay " << total << " estaciones candidatas: el peor caso es "
             << "O(n * 2^n) y puede tardar.\n";

        if (!confirmarBusqueda("Continuar igualmente? (1 = si, 0 = no): "))
        {
            cout << "Se cancelo la busqueda.\n";
            delete[] candidatas;
            return;
        }
    }

    RedistribucionCamion camion(candidatas, total, capacidad);
    camion.ejecutarRedistribucion();
    delete[] candidatas;
}


void menuRedistribucion(Sistema& sistema)
{
    int opcion;

    do
    {
        cout << "\n--- D. Redistribucion con backtracking ---\n"
             << "1. Buscar subconjuntos con las estaciones cargadas\n"
             << "2. Ejemplo del enunciado (camion de 10 bicicletas)\n"
             << "3. Cargar estaciones a mano\n"
             << "4. Listar faltantes de las estaciones cargadas\n"
             << "0. Volver\n";

        opcion = leerEntero("Seleccionar una opcion: ");

        switch (opcion)
        {
        case 1:
        {
            int capacidad = leerEntero("Capacidad del camion (C): ");

            if (capacidad <= 0)
            {
                cout << "La capacidad debe ser un numero positivo.\n";
                break;
            }

            int total = 0;
            EstacionFaltante * candidatas =
                construirFaltantes(sistema.tabla, capacidad, total);

            if (total == 0)
            {
                cout << "Ninguna estacion necesita entre 1 y " << capacidad
                     << " bicicletas: no hay candidatas para el camion.\n";
                delete[] candidatas;
                break;
            }

            cout << "Estaciones candidatas (faltante entre 1 y "
                 << capacidad << "): " << total << '\n';

            for (int i = 0; i < total; i++)
            {
                cout << "  " << candidatas[i].codigo << " ("
                     << candidatas[i].faltante << ")\n";
            }

            correrBacktracking(candidatas, total, capacidad);
            break;
        }
        case 2:
            probarEjemploEnunciado();
            break;
        case 3:
            probarConDatosPropios();
            break;
        case 4:
            listarFaltantes(sistema.tabla);
            break;
        case 0:
            break;
        default:
            cout << "Opcion invalida.\n";
        }
    }
    while (opcion != 0);
}
