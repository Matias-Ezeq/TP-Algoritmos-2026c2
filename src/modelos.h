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


#endif