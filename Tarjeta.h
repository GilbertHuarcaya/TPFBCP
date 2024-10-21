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

	int getIdCliente();
	int getIdCuentaBancaria();
	string getNumero();
	time_t getFechaVencimiento();
	string getCvv();

	string generarNumero();
	string generarCVV();
	time_t calcularFechaVencimiento(int anios);
	bool validarFechaVencimiento();

	//FILE
	string escribirLinea() override;
	void leerLinea(string csv) override;

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

string Tarjeta::getCvv() {
	return cvv;
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

time_t Tarjeta::calcularFechaVencimiento(int anios) {
	time_t fecha = time(0);
	struct tm* tm = localtime(&fecha);
	tm->tm_year += anios;
	return mktime(tm);
}

bool Tarjeta::validarFechaVencimiento() {
	time_t fechaActual = time(0);
	return difftime(fechaVencimiento, fechaActual) > 0;
}

string Tarjeta::escribirLinea() {
	stringstream ss;
	ss << id << "," << idCliente << "," << idCuentaBancaria << "," << numero << "," << put_time(localtime(&fechaVencimiento), "%Y-%m-%d") << "," << cvv;
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
		istringstream(temp) >> get_time(localtime(&fechaVencimiento), "%Y-%m-%d");
		getline(ss, cvv, ',');
	}
}

void Tarjeta::print() {
	cout << "ID: " << id << endl;
	cout << "ID Cliente: " << idCliente << endl;
	cout << "ID Cuenta Bancaria: " << idCuentaBancaria << endl;
	cout << "Numero: " << numero << endl;
	cout << "Fecha de Vencimiento: " << put_time(localtime(&fechaVencimiento), "%Y-%m-%d") << endl;
	cout << "CVV: " << cvv << endl;
	cout << endl;
}