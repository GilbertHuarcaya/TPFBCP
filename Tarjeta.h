#pragma once
#include "Elemento.h"
#include <sstream>

class Tarjeta : public Elemento
{
private:
	int idCliente;
	int idCuentaBancaria;
	string numero;
	time_t fechaVencimiento;
	string cvv;
public:

	Tarjeta();
	Tarjeta(int id);
	Tarjeta(int id, int idCliente, int idCuentaBancaria);
	Tarjeta(int id, int idCliente, int idCuentaBancaria, string numero, time_t fechaVencimiento, string cvv);

	//Get-Set
	int getIdCliente();
	int getIdCuentaBancaria();
	string getNumero();
	time_t getFechaVencimiento();
	string getFechaVencimientoStr();
	string getFechaVencimientoMMAA();
	string getCvv();
	void setCvv(string cvv);
	
	string generarNumero();
	string generarCVV();
	time_t calcularFechaVencimiento(int anios);
	bool validarSiEstaVencida();
	bool validarCVV(string cvv);
	bool validarNumero(string numero);
	bool validarFechaVencimiento(string vencimiento);
	bool validarTarjeta(string numero, string venciminto, string cvv);

	//FILE
	string escribirLinea() override;
	void leerLinea(string csv) override;
	string escribirCabecera() override;

	//PRINT
	void print();

};

Tarjeta::Tarjeta() : Elemento() {
	id = 0;
	idCliente = 0;
	idCuentaBancaria = 0;
	numero = generarNumero();
	fechaVencimiento = calcularFechaVencimiento(5);
	cvv = generarCVV();
}

Tarjeta::Tarjeta(int id) : Elemento(id) {
	idCliente = 0;
	idCuentaBancaria = 0;
	numero = generarNumero();
	fechaVencimiento = calcularFechaVencimiento(5);
	cvv = generarCVV();
};

Tarjeta::Tarjeta(int id, int idCliente, int idCuentaBancaria) : Elemento(id) {
	this->idCliente = idCliente;
	this->idCuentaBancaria = idCuentaBancaria;
	numero = generarNumero();
	fechaVencimiento = calcularFechaVencimiento(5);
	cvv = generarCVV();
}

Tarjeta::Tarjeta(int id, int idCliente, int idCuentaBancaria, string numero, time_t fechaVencimiento, string cvv) : Elemento(id) {
	this->idCliente = idCliente;
	this->idCuentaBancaria = idCuentaBancaria;
	this->numero = numero;
	this->fechaVencimiento = fechaVencimiento;
	this->cvv = cvv;
}

int Tarjeta::getIdCliente() {
	return idCliente;
}

int Tarjeta::getIdCuentaBancaria() {
	return idCuentaBancaria;
}

string Tarjeta::getNumero() {
	return numero;
}

time_t Tarjeta::getFechaVencimiento() {
	return fechaVencimiento;
}

inline string Tarjeta::getFechaVencimientoStr()
{
	stringstream ss;
	ss << put_time(localtime(&fechaVencimiento), "%Y-%m-%d");
	return ss.str();
}

inline string Tarjeta::getFechaVencimientoMMAA()
{
	stringstream ss;
	ss << put_time(localtime(&fechaVencimiento), "%m/%y");
	return ss.str();
}

string Tarjeta::getCvv() {
	return cvv;
}

void Tarjeta::setCvv(string cvv)
{
	this->cvv = cvv;
}

string Tarjeta::generarNumero() {
	string numero = "";
	for (int i = 0; i < 16; i++) {
		numero += to_string(rand() % 10);
	}
	return numero;
}

string Tarjeta::generarCVV() {
	return to_string(rand() % 1000 + 100);
}

time_t Tarjeta::calcularFechaVencimiento(int anios = 5) {
	time_t fecha = time(0);
	struct tm* tm = localtime(&fecha);
	tm->tm_year += anios;
	return mktime(tm);
}

bool Tarjeta::validarSiEstaVencida() {
	time_t fechaActual = time(0);
	return difftime(fechaVencimiento, fechaActual) <= 0;
}

bool Tarjeta::validarCVV(string cvv) {
	return this->cvv == cvv;
}

bool Tarjeta::validarNumero(string numero) {
	return this->numero == numero;
}

bool Tarjeta::validarFechaVencimiento(string vencimiento) {
	return getFechaVencimientoMMAA() == vencimiento;
}

bool Tarjeta::validarTarjeta(string numero, string vencimiento, string cvv) {
	return validarNumero(numero) && validarCVV(cvv) && validarFechaVencimiento(vencimiento);
}

string Tarjeta::escribirLinea() {
	stringstream ss;
	ss << id << "," << idCliente << "," << idCuentaBancaria << "," << numero << "," << fechaVencimiento << "," << cvv << "," << fechaCreacion << "," << fechaEdicion;
	return ss.str();
}

void Tarjeta::leerLinea(string csv) {
	stringstream ss(csv);
	string temp;
	getline(ss, temp, ',');
	if (temp != "") {
		id = stoi(temp);
		getline(ss, temp, ',');
		idCliente = stoi(temp);
		getline(ss, temp, ',');
		idCuentaBancaria = stoi(temp);
		getline(ss, numero, ',');
		getline(ss, temp, ',');
		fechaVencimiento = stoll(temp);
		getline(ss, cvv, ',');
		getline(ss, temp, ',');
		fechaCreacion = stoll(temp);
		getline(ss, temp, ',');
		fechaEdicion = stoll(temp);
	}
}

inline string Tarjeta::escribirCabecera()
{
	return "id,idCliente,idCuentaBancaria,numero,fechaVencimiento,cvv,fechaCreacion,fechaEdicion";
}

void Tarjeta::print() {
	cout << "ID: " << id << endl;
	cout << "ID Cliente: " << idCliente << endl;
	cout << "ID Cuenta Bancaria: " << idCuentaBancaria << endl;
	cout << "Numero: " << numero << endl;
	cout << "Fecha de Vencimiento: " << put_time(localtime(&fechaVencimiento), "%Y-%m-%d") << endl;
	cout << "CVV: " << cvv << endl;
	cout << "Fecha de Creacion: " << put_time(localtime(&fechaCreacion), "%Y-%m-%d %H:%M") << endl;
	cout << "Fecha de Edicion: " << put_time(localtime(&fechaEdicion), "%Y-%m-%d %H:%M") << endl;
	cout << endl;
}