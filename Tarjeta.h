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
	Tarjeta() : Elemento() {
		id = 0;
		idCliente = 0;
		idCuentaBancaria = 0;
		numero = generarNumero();
		fechaVencimiento = calcularFechaVencimiento(5);
		cvv = generarCVV();
	}

	Tarjeta(int id) : Elemento(id) {
		idCliente = 0;
		idCuentaBancaria = 0;
		numero = generarNumero();
		fechaVencimiento = calcularFechaVencimiento(5);
		cvv = generarCVV();
	};

	Tarjeta(int id, int idCliente, int idCuentaBancaria) : Elemento(id) {
		this->idCliente = idCliente;
		this->idCuentaBancaria = idCuentaBancaria;
		numero = generarNumero();
		fechaVencimiento = calcularFechaVencimiento(5);
		cvv = generarCVV();
	}

	Tarjeta(int id, int idCliente, int idCuentaBancaria, string numero, time_t fechaVencimiento, string cvv) : Elemento(id) {
		this->idCliente = idCliente;
		this->idCuentaBancaria = idCuentaBancaria;
		this->numero = numero;
		this->fechaVencimiento = fechaVencimiento;
		this->cvv = cvv;
	}

	int getIdCliente() {
		return idCliente;
	}

	int getIdCuentaBancaria() {
		return idCuentaBancaria;
	}

	string getNumero() {
		return numero;
	}

	time_t getFechaVencimiento() {
		return fechaVencimiento;
	}

	string getCvv() {
		return cvv;
	}

	string generarNumero() {
		string numero = "";
		for (int i = 0; i < 16; i++) {
			numero += to_string(rand() % 10);
		}
		return numero;
	}

	string generarCVV() {
		return to_string(rand() % 1000 + 100);
	}

	time_t calcularFechaVencimiento(int anios) {
		time_t fecha = time(0);
		struct tm* tm = localtime(&fecha);
		tm->tm_year += anios;
		return mktime(tm);
	}

	bool validarFechaVencimiento() {
		time_t fechaActual = time(0);
		return difftime(fechaVencimiento, fechaActual) > 0;
	}

	string escribirLinea() {
		stringstream ss;
		ss << id << "," << idCliente << "," << idCuentaBancaria << "," << numero << "," << put_time(localtime(&fechaVencimiento), "%Y-%m-%d") << "," << cvv;
		return ss.str();
	}

	void leerLinea(string csv) {
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

	void print() {
		cout << "ID: " << id << endl;
		cout << "ID Cliente: " << idCliente << endl;
		cout << "ID Cuenta Bancaria: " << idCuentaBancaria << endl;
		cout << "Numero: " << numero << endl;
		cout << "Fecha de Vencimiento: " << put_time(localtime(&fechaVencimiento), "%Y-%m-%d") << endl;
		cout << "CVV: " << cvv << endl;
		cout << endl;
	}
};
