#pragma once
#include "Elemento.h"
#include "ListaDoble.h"
#include "Operacion.h"
#include "Tarjeta.h"
#include "sstream"
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
	void borrarTarjeta();
	ListaDoble<Operacion*>* getOperaciones();

	void setIdCliente(int idCliente);
	void setPassword(string password);
	void setNumeroCuenta(string numeroCuenta);
	void setSaldo(double saldo);
	void setTarjeta(Tarjeta* tarjeta);
	void setOperaciones(ListaDoble<Operacion*>* operaciones);

	bool validarPassword(string  password);

	//UPDATE
	void agregarOperacion(Operacion* operacion);
	void retirar(double cantidad);
	void depositar(double cantidad);
	void transferir(CuentaBancaria* cuenta, double cantidad);

	//SEARCH
	static CuentaBancaria* buscarCuentaPorNumero(string numeroCuenta);

	//LOAD TO LIST
	void loadTarjeta();
	void loadOperaciones();

	//OPERACIONES
	Operacion* crearDeposito(int id, string numeroCuentaBancaria, double monto, int idCanal, int idSede);
	Operacion* crearTransferencia(int id, string numeroCuentaBancariaOrigen, string cuentaDestino, double monto, int idCanal, int idSede);
	Operacion* crearRetiro(int id, string numeroCuentaBancaria, double monto, int idCanal, int idSede);

	//FILE
	string escribirLinea();
	void leerLinea(string File);
	string escribirCabecera() override;

	//PRINT
	void print();
	void imprimirOperaciones();

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
inline void CuentaBancaria::borrarTarjeta()
{
	tarjeta = nullptr;
}
ListaDoble<Operacion*>* CuentaBancaria::getOperaciones() { return operaciones; }

void CuentaBancaria::setIdCliente(int idCliente) { this->idCliente = idCliente; }
void CuentaBancaria::setPassword(string password) { this->password = password; }
void CuentaBancaria::setNumeroCuenta(string numeroCuenta) { this->numeroCuenta = numeroCuenta; }
void CuentaBancaria::setSaldo(double saldo) { this->saldo = saldo; }
void CuentaBancaria::setTarjeta(Tarjeta* tarjeta) { this->tarjeta = tarjeta; }
void CuentaBancaria::setOperaciones(ListaDoble<Operacion*>* operaciones) { this->operaciones = operaciones; }

inline bool CuentaBancaria::validarPassword(string password)
{
	return this->password == password;
}

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

inline string CuentaBancaria::escribirCabecera()
{
	return "id,idCliente,password,numeroCuenta,saldo";
}

void CuentaBancaria::agregarOperacion(Operacion* operacion)
{
	operaciones->push_back(operacion, operacion->getId());
}

void CuentaBancaria::retirar(double cantidad)
{
	if (saldo >= cantidad)
	{
		saldo -= cantidad;
		ListaDoble<CuentaBancaria*>* todasLasCuentas = new ListaDoble<CuentaBancaria*>("Datos/Cuentas.csv");
		todasLasCuentas->editar(this);
	}
	else
	{
		cout << "Saldo insuficiente" << endl;
	}
}

void CuentaBancaria::depositar(double cantidad)
{
	saldo += cantidad;
	ListaDoble<CuentaBancaria*>* todasLasCuentas = new ListaDoble<CuentaBancaria*>("Datos/Cuentas.csv");
	todasLasCuentas->editar(this);
}

void CuentaBancaria::transferir(CuentaBancaria* cuenta, double cantidad)
{
	if (saldo >= cantidad)
	{
		saldo -= cantidad;
		cuenta->depositar(cantidad);
		ListaDoble<CuentaBancaria*>* todasLasCuentas = new ListaDoble<CuentaBancaria*>("Datos/Cuentas.csv");
		todasLasCuentas->editar(this);
	}
	else
	{
		cout << "Saldo insuficiente" << endl;
	}
}

void CuentaBancaria::imprimirOperaciones()
{

	if (operaciones->head == nullptr)
	{
		cout << "	No hay operaciones" << endl;
	}
	else 
	{
		cout << "	Tiene " << operaciones->getSize() << " operaciones." << endl;
	}
}

void CuentaBancaria::print()
{
	cout << "ID: " << id << endl;
	cout << "ID Cliente: " << idCliente << endl;
	cout << "Password: " << password << endl;
	cout << "Numero de Cuenta: " << numeroCuenta << endl;
	cout << "Saldo: " << saldo << endl << endl;
	cout << "Tarjeta: " << endl;
	if (tarjeta != nullptr) {
		tarjeta->print();
	}
	else {
		cout << "	No hay tarjeta asociada" << endl << endl;
	}
	cout << "Operaciones: " << endl;
	imprimirOperaciones();
}

inline CuentaBancaria* CuentaBancaria::buscarCuentaPorNumero(string numeroCuenta)
{
	ListaDoble<CuentaBancaria*>* todasLasCuentas = new ListaDoble<CuentaBancaria*>("Datos/Cuentas.csv");
	todasLasCuentas->leer();
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

	ListaDoble<Tarjeta*>* todasLasTarjetas = new ListaDoble<Tarjeta*>("Datos/Tarjetas.csv");
	todasLasTarjetas->leer();
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
	ListaDoble<Operacion*>* todasLasOperaciones = new ListaDoble<Operacion*>("Datos/Operaciones.csv");
	todasLasOperaciones->leer();
	Nodo<Operacion*>* temp = todasLasOperaciones->head;
	while (temp != nullptr)
	{
		if (temp->data->getIdCuentaBancariaOrigen() == id || temp->data->getIdCuentaBancariaDestino() == id)
		{
			Nodo<Operacion*>* temp2 = operaciones->search(temp->data->getId());
			if (operaciones->getSize() != 0 && temp2 != nullptr)
			{
				operaciones->updateElement(temp->data);
			}
			else
			{
				operaciones->push_back(temp->data, temp->data->getId());
			}
		}
		temp = temp->next;
	}
}

inline Operacion* CuentaBancaria::crearDeposito(int id, string numeroCuentaBancaria, double monto, int idCanal, int idSede)
{
	CuentaBancaria* cuentaBancaria = buscarCuentaPorNumero(numeroCuentaBancaria);
	if (cuentaBancaria == nullptr) {
		cout << endl << "Cuenta no encontrada, intenta escribir sin espacios ni guiones entre los digitos." << endl;
		system("pause");
		return nullptr;
	}

	return new Operacion(id, 0, cuentaBancaria->getIdCliente(), 0, cuentaBancaria->getId(), Deposito, monto, idCanal, idSede);
}


Operacion* CuentaBancaria::crearTransferencia(int id, string numeroCuentaBancariaOrigen, string cuentaDestino, double monto, int idCanal, int idSede) {
	CuentaBancaria* cuentaBancariaOrigen = buscarCuentaPorNumero(numeroCuentaBancariaOrigen);
	if (cuentaBancariaOrigen == nullptr) {
		cout << endl << "Cuenta origen no encontrada, intenta escribir sin espacios ni guiones entre los digitos." << endl;
		system("pause");
		return nullptr;
	}

	CuentaBancaria* cuentaBancariaDestino = buscarCuentaPorNumero(cuentaDestino);
	if (cuentaBancariaDestino == nullptr) {
		cout << endl << "Cuenta destino no encontrada, intenta escribir sin espacios ni guiones entre los digitos." << endl;
		system("pause");
		return nullptr;
	}

	if (monto > cuentaBancariaOrigen->getSaldo()) {
		cout << endl << "Saldo insuficiente" << endl;
		system("pause");
		return nullptr;
	}

	return new Operacion(id, cuentaBancariaOrigen->getIdCliente(), cuentaBancariaDestino->getIdCliente(), cuentaBancariaOrigen->getId(), cuentaBancariaDestino->getId(), Transferencia, monto, idCanal, idSede);
}

Operacion* CuentaBancaria::crearRetiro(int id, string numeroCuentaBancaria, double monto, int idCanal, int idSede) {
	CuentaBancaria* cuentaBancaria = buscarCuentaPorNumero(numeroCuentaBancaria);
	if (cuentaBancaria == nullptr) {
		cout << endl << "Cuenta no encontrada, intenta escribir sin espacios ni guiones entre los digitos." << endl;
		system("pause");
		return nullptr;
	}

	if (monto > cuentaBancaria->getSaldo()) {
		cout << endl << "Saldo insuficiente" << endl;
		system("pause");
		return nullptr;
	}

	return new Operacion(id, cuentaBancaria->getIdCliente(), 0, cuentaBancaria->getId(), 0, Retiro, monto, idCanal, idSede);
}
