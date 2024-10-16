#pragma once
#include "Elemento.h"
#include "ListaDoble.h"
#include "Operacion.h"
#include "Tarjeta.h"
#include <sstream>
#include "File.h"
using namespace std;

class CuentaBancaria : public Elemento
{
private:
	int idCliente;
	string password;
	string numeroCuenta;
	double saldo;
	Tarjeta* tarjeta;
	ListaDoble<Operacion*>* operaciones;

public:
	CuentaBancaria();
	CuentaBancaria(int id);
	CuentaBancaria(int id, int idCliente, string password, string numeroCuenta, double saldo);

	int getIdCliente();
	string getPassword();
	string getNumeroCuenta();
	double getSaldo();
	Tarjeta* getTarjeta();
	ListaDoble<Operacion*>* getOperaciones();

	void setIdCliente(int idCliente);
	void setPassword(string password);
	void setNumeroCuenta(string numeroCuenta);
	void setSaldo(double saldo);
	void setTarjeta(Tarjeta* tarjeta);
	void setOperaciones(ListaDoble<Operacion*>* operaciones);

	string escribirLinea();
	void leerLinea(string File);

	void agregarOperacion(Operacion* operacion);
	void retirar(double cantidad);
	void depositar(double cantidad);
	void transferir(CuentaBancaria* cuenta, double cantidad);
	void imprimirOperaciones();
	void print();

	static CuentaBancaria* buscarCuentaPorNumero(string numeroCuenta);

	void loadTarjeta();
	void loadOperaciones();
};


CuentaBancaria::CuentaBancaria() : Elemento() {
	idCliente = 0;
	password = "";
	numeroCuenta = "";
	saldo = 0;
	tarjeta = nullptr;
	operaciones = new ListaDoble<Operacion*>();
}

CuentaBancaria::CuentaBancaria(int id) : Elemento(id) {
	idCliente = 0;
	password = "";
	numeroCuenta = "";
	saldo = 0;
	tarjeta = nullptr;
	operaciones = new ListaDoble<Operacion*>();
}

CuentaBancaria::CuentaBancaria(int id, int idCliente, string password, string numeroCuenta, double saldo)
	: Elemento(id), idCliente(idCliente), password(password), numeroCuenta(numeroCuenta), saldo(saldo) {
	tarjeta = nullptr;
	operaciones = new ListaDoble<Operacion*>();
}

int CuentaBancaria::getIdCliente() { return idCliente; }
string CuentaBancaria::getPassword() { return password; }
string CuentaBancaria::getNumeroCuenta() { return numeroCuenta; }
double CuentaBancaria::getSaldo() { return saldo; }
Tarjeta* CuentaBancaria::getTarjeta() { return tarjeta; }
ListaDoble<Operacion*>* CuentaBancaria::getOperaciones() { return operaciones; }

void CuentaBancaria::setIdCliente(int idCliente) { this->idCliente = idCliente; }
void CuentaBancaria::setPassword(string password) { this->password = password; }
void CuentaBancaria::setNumeroCuenta(string numeroCuenta) { this->numeroCuenta = numeroCuenta; }
void CuentaBancaria::setSaldo(double saldo) { this->saldo = saldo; }
void CuentaBancaria::setTarjeta(Tarjeta* tarjeta) { this->tarjeta = tarjeta; }
void CuentaBancaria::setOperaciones(ListaDoble<Operacion*>* operaciones) { this->operaciones = operaciones; }

string CuentaBancaria::escribirLinea() {
	return to_string(id) + "," + to_string(idCliente) + "," + password + "," + numeroCuenta + "," + to_string(saldo);
}

void CuentaBancaria::leerLinea(string File) {
	stringstream ss(File);
	string idString;
	getline(ss, idString, ',');
	if (idString != "") {
		id = stoi(idString);
		string idClienteString;
		getline(ss, idClienteString, ',');
		idCliente = stoi(idClienteString);
		getline(ss, password, ',');
		string numeroCuentaString;
		getline(ss, numeroCuentaString, ',');
		numeroCuenta = numeroCuentaString;
		string saldoString;
		getline(ss, saldoString, ',');
		saldo = stod(saldoString);
	}
	loadOperaciones();
	loadTarjeta();
}

void CuentaBancaria::agregarOperacion(Operacion* operacion)
{
	operaciones->push_back(operacion);
}

void CuentaBancaria::retirar(double cantidad)
{
	if (saldo >= cantidad)
	{
		saldo -= cantidad;
		File<ListaDoble<CuentaBancaria*>*, CuentaBancaria>::editar("Cuentas.csv", this);
	}
	else
	{
		cout << "Saldo insuficiente" << endl;
	}
}

void CuentaBancaria::depositar(double cantidad)
{
	saldo += cantidad;
	File<ListaDoble<CuentaBancaria*>*, CuentaBancaria>::editar("Cuentas.csv", this);
}

void CuentaBancaria::transferir(CuentaBancaria* cuenta, double cantidad)
{
	if (saldo >= cantidad)
	{
		saldo -= cantidad;
		cuenta->depositar(cantidad);
		File<ListaDoble<CuentaBancaria*>*, CuentaBancaria>::editar("Cuentas.csv", this);
	}
	else
	{
		cout << "Saldo insuficiente" << endl;
	}
}

void CuentaBancaria::imprimirOperaciones()
{
	Nodo<Operacion*>* temp = operaciones->head;
	while (temp != nullptr)
	{
		temp->data->print();
		temp = temp->next;
	}
	if (operaciones->head == nullptr)
	{
		cout << "No hay operaciones" << endl;
	}
}

void CuentaBancaria::print()
{
	cout << "ID: " << id << endl;
	cout << "ID Cliente: " << idCliente << endl;
	cout << "Password: " << password << endl;
	cout << "Numero de Cuenta: " << numeroCuenta << endl;
	cout << "Saldo: " << saldo << endl;
	cout << "Tarjeta: " << endl;
	if (tarjeta != nullptr) {
		tarjeta->print();
	}
	else {
		cout << "No hay tarjeta asociada" << endl;
	}
	cout << "Operaciones: " << endl;
	imprimirOperaciones();
}

inline CuentaBancaria* CuentaBancaria::buscarCuentaPorNumero(string numeroCuenta)
{
	ListaDoble<CuentaBancaria*>* todasLasCuentas = new ListaDoble<CuentaBancaria*>("Cuentas.csv");
	File<ListaDoble<CuentaBancaria*>*, CuentaBancaria>::leer("Cuentas.csv", todasLasCuentas);
	Nodo<CuentaBancaria*>* temp = todasLasCuentas->head;
	while (temp != nullptr)
	{
		if (temp->data->getNumeroCuenta() == numeroCuenta)
		{
			return temp->data;
		}
		temp = temp->next;
	}
	return nullptr;

}

void CuentaBancaria::loadTarjeta()
{
	//Bool para verificar si se encontro la tarjeta de la cuenta bancaria
	bool encontrado = false;
	ListaDoble<Tarjeta*>* todasLasTarjetas = new ListaDoble<Tarjeta*>("Tarjetas.csv");
	File<ListaDoble<Tarjeta*>*, Tarjeta>::leer("Tarjetas.csv", todasLasTarjetas);
	Nodo<Tarjeta*>* temp = todasLasTarjetas->head;
	while (temp != nullptr)
	{
		if (temp->data->getIdCuentaBancaria() == id)
		{
			tarjeta = temp->data;
			//Pasa a true si lo encuentra
			encontrado = true;
		}
		temp = temp->next;
	}
	//Si no lo encuentra, tarjeta sera null
	if (!encontrado) tarjeta = nullptr;
}

void CuentaBancaria::loadOperaciones()
{
	ListaDoble<Operacion*>* todasLasOperaciones = new ListaDoble<Operacion*>("Operaciones.csv");
	File<ListaDoble<Operacion*>*, Operacion>::leer("Operaciones.csv", todasLasOperaciones);
	Nodo<Operacion*>* temp = todasLasOperaciones->head;
	while (temp != nullptr)
	{
		if (temp->data->getIdCuentaBancariaOrigen() == id || temp->data->getIdCuentaBancariaDestino() == id)
		{
			operaciones->push_back(temp->data);
		}
		temp = temp->next;
	}
}
