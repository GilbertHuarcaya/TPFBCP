#pragma once
#include "Elemento.h"
#include "Canal.h"
#include "Sede.h"
#include "ListaDoble.h"
#include <string>
using namespace std;

enum TipoOperacion {
	Ninguno,
	Transferencia,
	Deposito,
	Retiro
};

enum EstadoOperacion {
	Pendiente,
	Realizada,
	Rechazada
};

class Operacion : public Elemento
{
private:
	int idClienteOrigen;
	int idClienteDestino;
	int idCuentaBancariaOrigen;
	int idCuentaBancariaDestino;
	TipoOperacion tipo;
	EstadoOperacion estado;
	double monto;
	Canal* canal;
	Sede* sede;
public:
	Operacion();
	Operacion(int id, int idClienteOrigen,
		int idClienteDestino, int idCuentaBancariaOrigen,
		int idCuentaBancariaDestino, int tipo, double monto, Canal* canal, Sede* sede);
	Operacion(int id, int idClienteOrigen,
		int idClienteDestino, int idCuentaBancariaOrigen,
		int idCuentaBancariaDestino, TipoOperacion tipo, double monto, Canal* canal, Sede* sede);
	~Operacion();

	int getIdClienteOrigen();
	int getIdClienteDestino();
	int getIdCuentaBancariaOrigen();
	int getIdCuentaBancariaDestino();
	TipoOperacion getTipo();
	double getMonto();
	Canal* getCanal();
	Sede* getSede();
	EstadoOperacion getEstado();
	void setEstado(EstadoOperacion estado) { this->estado = estado; }
	string escribirLinea();
	void leerLinea(string linea);
	void print();
	void validarOperacion();
};


Operacion::Operacion() : Elemento() {
	idClienteOrigen = 0;
	idClienteDestino = 0;
	idCuentaBancariaOrigen = 0;
	idCuentaBancariaDestino = 0;
	tipo = Ninguno;
	estado = Pendiente;
	monto = 0;
	canal = nullptr;
	sede = nullptr;
}
Operacion::Operacion(int id, int idClienteOrigen,
	int idClienteDestino, int idCuentaBancariaOrigen,
	int idCuentaBancariaDestino, int tipo, double monto, Canal* canal, Sede* sede = nullptr)
	: Elemento(id), idClienteOrigen(idClienteOrigen),
	idClienteDestino(idClienteDestino),
	idCuentaBancariaOrigen(idCuentaBancariaOrigen),
	idCuentaBancariaDestino(idCuentaBancariaDestino), monto(monto) {
	tipo = TipoOperacion(tipo);
	this->canal = canal;
	this->sede = sede;
	estado = Pendiente;
}
Operacion::Operacion(int id, int idClienteOrigen,
	int idClienteDestino, int idCuentaBancariaOrigen,
	int idCuentaBancariaDestino, TipoOperacion tipo, double monto, Canal* canal, Sede* sede = nullptr)
	: Elemento(id), idClienteOrigen(idClienteOrigen),
	idClienteDestino(idClienteDestino),
	idCuentaBancariaOrigen(idCuentaBancariaOrigen),
	idCuentaBancariaDestino(idCuentaBancariaDestino),
	tipo(tipo), monto(monto) {
	this->canal = canal;
	this->sede = sede;
	estado = Pendiente;
}
Operacion::~Operacion() {
	delete canal;
	delete sede;
}

int Operacion::getIdClienteOrigen() { return idClienteOrigen; }
int Operacion::getIdClienteDestino() { return idClienteDestino; }
int Operacion::getIdCuentaBancariaOrigen() { return idCuentaBancariaOrigen; }
int Operacion::getIdCuentaBancariaDestino() { return idCuentaBancariaDestino; }
TipoOperacion Operacion::getTipo() { return tipo; }
double Operacion::getMonto() { return monto; }
Canal* Operacion::getCanal() { return canal; }
Sede* Operacion::getSede() { return sede; }
EstadoOperacion Operacion::getEstado() { return estado; }

string Operacion::escribirLinea() {
	stringstream ss;
	ss << id << "," << idClienteOrigen << "," << idClienteDestino << "," << idCuentaBancariaOrigen << "," << idCuentaBancariaDestino << "," << tipo << "," << estado << "," << monto << "," << fechaCreacion << "," << fechaEdicion;
	return ss.str();
}

void Operacion::leerLinea(string linea) {
	stringstream ss(linea);
	string item;
	getline(ss, item, ',');
	if (item != "" || item != "id") {
		id = stoi(item);
		getline(ss, item, ',');
		idClienteOrigen = stoi(item);
		getline(ss, item, ',');
		idClienteDestino = stoi(item);
		getline(ss, item, ',');
		idCuentaBancariaOrigen = stoi(item);
		getline(ss, item, ',');
		idCuentaBancariaDestino = stoi(item);
		getline(ss, item, ',');
		tipo = item != "" ? TipoOperacion(stoi(item)) : Ninguno;
		getline(ss, item, ',');
		estado = item != "" ? EstadoOperacion(stoi(item)) : Pendiente;
		getline(ss, item, ',');
		monto = stod(item);
		getline(ss, item, ',');
		fechaCreacion = stoll(item);
		getline(ss, item, ',');
		fechaEdicion = stoll(item);
	}
}

void Operacion::print() {
	cout << "ID: " << getId() << endl;
	cout << "IdClienteOrigen: " << idClienteOrigen << endl;
	cout << "IdClienteDestino: " << idClienteDestino << endl;
	cout << "IdCuentaBancariaOrigen: " << idCuentaBancariaOrigen << endl;
	cout << "IdCuentaBancariaDestino: " << idCuentaBancariaDestino << endl;
	cout << "Tipo: " << TipoOperacion(tipo) << endl;
	cout << "Estado: " << EstadoOperacion(estado) << endl;
	cout << "Monto: " << monto << endl;
	cout << "Fecha creacion: " << put_time(localtime(&fechaCreacion), "%Y-%m-%d-%h-%m") << endl;
	cout << "Fecha edicion: " << put_time(localtime(&fechaEdicion), "%Y-%m-%d-%h-%m") << endl;
}

void Operacion::validarOperacion() {
	if (monto < 0) {
		cout << "Monto invalido" << endl;
		estado = Rechazada;
		return;
	}
	if (idClienteOrigen == idClienteDestino && tipo == Transferencia) {
		cout << "No se puede transferir a la misma cuenta" << endl;
		estado = Rechazada;
		return;
	}
}

