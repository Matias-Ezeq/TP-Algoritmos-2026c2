// Este archivo NO se compila por separado: se incluye desde main.cpp.
#pragma once

string leerTipoValido()
{
    string tipo;

    while (true)
    {
        cout << "Tipo (Mecanica/Electrica): ";
        cin >> tipo;

        if (tipo == "Mecanica" || tipo == "Electrica")
        {
            return tipo;
        }

        cout << "Tipo invalido. Ingrese Mecanica o Electrica.\n";
    }
}

string leerEstadoValido()
{
    string estado;

    while (true)
    {
        cout << "Estado (OK/Averiada): ";
        cin >> estado;

        if (estado == "OK" || estado == "Averiada")
        {
            return estado;
        }

        cout << "Estado invalido. Ingrese OK o Averiada.\n";
    }
}

void menuBicicletas(Sistema& sistema)
{
    int opcion;

    do
    {
        cout << "\n--- C. Bicicletas (arbol binario de busqueda) ---\n"
             << "1. Insertar bicicleta\n"
             << "2. Buscar bicicleta por ID\n"
             << "3. Listar bicicletas ordenadas por ID\n"
             << "4. Contar bicicletas averiadas\n"
             << "5. Altura del arbol\n"
             << "6. Eliminar bicicleta (opcional)\n"
             << "0. Volver\n";

        opcion = leerEntero("Seleccionar una opcion: ");

        switch (opcion)
        {
        case 1:
        {
            int biciId = leerEntero("ID: ");

            string codigoEstacion;
            string tipo;
            string estado;

            cout << "Estacion: ";
            cin >> codigoEstacion;
            tipo = leerTipoValido();
            estado = leerEstadoValido();

            int kilometros = leerEntero("Kilometros: ");

            if (!sistema.arbol.insertar(
                    biciId,
                    codigoEstacion,
                    tipo,
                    estado,
                    kilometros))
            {
                cout << "Ya existe una bicicleta con el ID " << biciId
                     << ".\n";
                break;
            }

            cout << "Bicicleta insertada.\n";
            break;
        }
        case 2:
        {
            int biciId = leerEntero("ID a buscar: ");
            sistema.arbol.buscarPorId(biciId);
            break;
        }
        case 3:
            sistema.arbol.listarInorden();
            break;
        case 4:
            cout << "Bicicletas averiadas: "
                 << sistema.arbol.contarAveriadas() << '\n';
            break;
        case 5:
            cout << "Altura del arbol: "
                 << sistema.arbol.calcularAltura() << '\n';
            break;
        case 6:
        {
            int biciId = leerEntero("ID a eliminar: ");

            if (sistema.taller.contieneBicicleta(biciId))
            {
                cout << "No se puede eliminar: la bicicleta esta en el "
                     << "taller de reparaciones.\n";
                break;
            }

            if (sistema.arbol.eliminar(biciId))
            {
                cout << "Bicicleta " << biciId
                     << " eliminada correctamente.\n";
            }
            else
            {
                cout << "No existe una bicicleta con ese ID.\n";
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
