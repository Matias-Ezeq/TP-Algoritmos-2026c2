#ifndef MODELOS_H

#include <iostream>
#include <string>

#define MODELOS_H

class Bicicleta {
private:
    int biciId;
    string codigoEstacion;
    string tipo;
    string estado;
    int kilometros;

public:
    Bicicleta(int id, string est, string t, string e, int km)
        : biciId(id), codigoEstacion(est), tipo(t), estado(e), kilometros(km) {}

    int getBiciId() const { return biciId; }
    string getCodigoEstacion() const { return codigoEstacion; }
    string getTipo() const { return tipo; }
    string getEstado() const { return estado; }
    int getKilometros() const { return kilometros; }

    void setCodigoEstacion(const string& est) { codigoEstacion = est; }
    void setEstado(const string& nuevoEstado) { estado = nuevoEstado; }
    void setKilometros(int km) { kilometros = km; }

    void mostrarDatos() const {
        cout << "Id: " << biciId 
             << " | Estacion: " << codigoEstacion 
             << " | Tipo: " << tipo 
             << " | Estado: " << estado 
             << " | Km: " << kilometros << endl;
    }
};

class Estacion
{
public:
    string codigo;
    string nombre;
    string barrio;
    int capacidad;
    int bicisDisponibles;

    Estacion(
        string codigo,
        string nombre,
        string barrio,
        int capacidad,
        int bicisDisponibles
    ) : codigo(codigo),
        nombre(nombre),
        barrio(barrio),
        capacidad(capacidad),
        bicisDisponibles(bicisDisponibles)
    {
    }

    Estacion()
        : codigo(""),
          nombre(""),
          barrio(""),
          capacidad(0),
          bicisDisponibles(0)
    {
    }

    void mostrar() const
    {
        cout << "Codigo: " << codigo << '\n'
             << "Nombre: " << nombre << '\n'
             << "Barrio: " << barrio << '\n'
             << "Capacidad de anclajes: " << capacidad << '\n'
             << "Bicicletas disponibles: " << bicisDisponibles << '\n';
    }

};

struct Viaje
{
    int id;
    int bici_id;
    int dni_usuario;
    string estacion_origen;
    string estacion_destino;
    int fecha;
    int duracion_min;
};

#endif