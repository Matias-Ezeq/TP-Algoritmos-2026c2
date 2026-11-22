// Este archivo NO se compila por separado: se incluye desde main.cpp.
#pragma once

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
            cout << "Tipo (Mecanica/Electrica): ";
            cin >> tipo;
            cout << "Estado (OK/Averiada): ";
            cin >> estado;

            int kilometros = leerEntero("Kilometros: ");

            sistema.arbol.insertar(
                biciId,
                codigoEstacion,
                tipo,
                estado,
                kilometros
            );

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
            cout << "Aviso: es la funcionalidad opcional y puede dejar "
                 << "memoria sin liberar.\n";
            sistema.arbol.eliminar(biciId);
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
