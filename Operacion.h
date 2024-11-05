#pragma once
#include "Elemento.h"
#include "ListaDoble.h"
#include "string"
#include "sstream"
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
	int idCanal;
	int idSede;
public:
	Operacion();
	Operacion(int id, int idClienteOrigen,
		int idClienteDestino, int idCuentaBancariaOrigen,
		int idCuentaBancariaDestino, int tipo, double monto, int idCanal, int idSede);
	Operacion(int id, int idClienteOrigen,
		int idClienteDestino, int idCuentaBancariaOrigen,
		int idCuentaBancariaDestino, TipoOperacion tipo, double monto, int idCanal, int idSede);
	~Operacion();

	int getIdClienteOrigen();
	int getIdClienteDestino();
	int getIdCuentaBancariaOrigen();
	int getIdCuentaBancariaDestino();
	TipoOperacion getTipo();
	double getMonto();
	int getIdCanal();
	int getIdSede();

	EstadoOperacion getEstado();

	void setEstado(EstadoOperacion estado);

	//FILE
	string escribirLinea();
	void leerLinea(string linea);
	string escribirCabecera() override;

	//PRINT
	void print();

	//VALIDATORS
	void validarOperacion();

	//Ordenamiento avanzado
	template <typename L>
	static void ordenarPorFecha(L lista, bool ascendente = true);
	template <typename L>
	static void ordenarPorMonto(L lista, bool ascendente = true);
};


Operacion::Operacion() : Elemento() {
	idClienteOrigen = 0;
	idClienteDestino = 0;
	idCuentaBancariaOrigen = 0;
	idCuentaBancariaDestino = 0;
	tipo = Ninguno;
	estado = Pendiente;
	monto = 0;
	idCanal = 0;
	idSede = 0;
}
Operacion::Operacion(int id, int idClienteOrigen,
	int idClienteDestino, int idCuentaBancariaOrigen,
	int idCuentaBancariaDestino, int tipo, double monto, int idCanal, int idSede = 0)
	: Elemento(id), idClienteOrigen(idClienteOrigen),
	idClienteDestino(idClienteDestino),
	idCuentaBancariaOrigen(idCuentaBancariaOrigen),
	idCuentaBancariaDestino(idCuentaBancariaDestino), monto(monto) {
	tipo = TipoOperacion(tipo);
	this->idCanal = idCanal;
	this->idSede = idSede;
	estado = Pendiente;
}
Operacion::Operacion(int id, int idClienteOrigen,
	int idClienteDestino, int idCuentaBancariaOrigen,
	int idCuentaBancariaDestino, TipoOperacion tipo, double monto, int idCanal, int idSede = 0)
	: Elemento(id), idClienteOrigen(idClienteOrigen),
	idClienteDestino(idClienteDestino),
	idCuentaBancariaOrigen(idCuentaBancariaOrigen),
	idCuentaBancariaDestino(idCuentaBancariaDestino), monto(monto) {
	this->tipo = tipo;
	this->idCanal = idCanal;
	this->idSede = idSede;
	estado = Pendiente;
}
Operacion::~Operacion() {
}

int Operacion::getIdClienteOrigen() { return idClienteOrigen; }
int Operacion::getIdClienteDestino() { return idClienteDestino; }
int Operacion::getIdCuentaBancariaOrigen() { return idCuentaBancariaOrigen; }
int Operacion::getIdCuentaBancariaDestino() { return idCuentaBancariaDestino; }
TipoOperacion Operacion::getTipo() { return tipo; }
double Operacion::getMonto() { return monto; }
int Operacion::getIdCanal() { return idCanal; }
int Operacion::getIdSede() { return idSede; }

EstadoOperacion Operacion::getEstado() { return estado; }

inline void Operacion::setEstado(EstadoOperacion estado)
{
	this->estado = estado;
}

string Operacion::escribirLinea() {
	stringstream ss;
	ss << id << "," << idClienteOrigen << "," << idClienteDestino << "," << idCuentaBancariaOrigen << "," << idCuentaBancariaDestino << "," << tipo << "," << estado << "," << monto << "," << idCanal << "," << idSede << "," << fechaCreacion << "," << fechaEdicion;
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
		idCanal = stoi(item);
		getline(ss, item, ',');
		idSede = stoi(item);
		getline(ss, item, ',');
		fechaCreacion = stoll(item);
		getline(ss, item, ',');
		fechaEdicion = stoll(item);
	}
}

inline string Operacion::escribirCabecera()
{
	return "id,idClienteOrigen,idClienteDestino,idCuentaBancariaOrigen,idCuentaBancariaDestino,tipo,estado,monto,idCanal,idSede,fechaCreacion,fechaEdicion";
}

void Operacion::print() {

	string tipoOperacion[] = { "Ninguno", "Transferencia", "Deposito", "Retiro" };
	string estadoOperacion[] = { "Pendiente", "Realizada", "Rechazada" };

	cout << "ID: " << getId() << endl;
	cout << "IdClienteOrigen: " << idClienteOrigen << endl;
	cout << "IdClienteDestino: " << idClienteDestino << endl;
	cout << "IdCuentaBancariaOrigen: " << idCuentaBancariaOrigen << endl;
	cout << "IdCuentaBancariaDestino: " << idCuentaBancariaDestino << endl;
	cout << "Tipo: " << tipoOperacion[tipo] << endl;
	cout << "Estado: " << estadoOperacion[estado] << endl;
	cout << "Monto: " << monto << endl;
	if (idCanal != 0 ) cout << "IdCanal: " << idCanal << endl;
	if (idSede != 0)  cout << "IdSede: " << idSede << endl;
	cout << "Fecha creacion: " << put_time(localtime(&fechaCreacion), "%Y-%m-%d %H:%M") << endl;
	cout << "Fecha edicion: " << put_time(localtime(&fechaEdicion), "%Y-%m-%d %H:%M") << endl;
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

template <typename L>
inline void Operacion::ordenarPorFecha(L lista, bool ascendente)
{
	auto compararFechas = [&](Operacion* a, Operacion* b) {
		return ascendente ? a->getFechaCreacion() < b->getFechaCreacion() : a->getFechaCreacion() > b->getFechaCreacion();
	};

	lista->mergeSort(&lista->head, compararFechas);
}

template <typename L>
inline void Operacion::ordenarPorMonto(L lista, bool ascendente)
{
	auto compararMontos = [&](Operacion* a, Operacion* b) {
		return ascendente ? a->getMonto() < b->getMonto() : a->getMonto() > b->getMonto();
		};

	lista->mergeSort(&lista->head, compararMontos);
}

