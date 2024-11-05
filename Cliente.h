#pragma once
#include "Elemento.h"
#include "ListaDoble.h"
#include "CuentaBancaria.h"
#include "sstream"
#include "string"

using namespace std;

class Cliente : public Elemento
{
private:
	string nombre;
	string apellido;
	string direccion;
	string telefono;
	string email;
	string password;
	ListaDoble<CuentaBancaria*>* cuentasBancarias;

public:

	Cliente();
	Cliente(int id);
	Cliente(string nombre, string apellido, string direccion, string telefono, string email, string password);
	Cliente(int id, string nombre, string apellido, string direccion, string telefono, string email, string password);

	string getNombre();
	string getApellido();
	string getDireccion();
	string getTelefono();
	string getEmail();
	string getPassword();
	void setNombre(string nombre);
	void setApellido(string apellido);
	void setDireccion(string direccion);
	void setTelefono(string telefono);
	void setEmail(string email);
	void setPassword(string password);

	void agregarCuentaBancaria(CuentaBancaria* cuentaBancaria);
	ListaDoble<CuentaBancaria*>* getCuentasBancarias();
	void actualizarDatos(string nombre, string apellido, string direccion, string telefono, string email, string password);
	string escribirLinea() override;
	string escribirCabecera() override;
	void leerLinea(string linea) override;
	void print();
	void loadCuentasBancarias();
};

Cliente::Cliente() : Elemento() {
	this->id = 0;
	this->nombre = "";
	this->apellido = "";
	this->direccion = "";
	this->telefono = "";
	this->email = "";
	this->password = "";
	cuentasBancarias = new ListaDoble<CuentaBancaria*>();
};
Cliente::Cliente(int id) : Elemento(id) {
	this->nombre = "";
	this->apellido = "";
	this->direccion = "";
	this->telefono = "";
	this->email = "";
	this->password = "";
	cuentasBancarias = new ListaDoble<CuentaBancaria*>();
};
Cliente::Cliente(string nombre, string apellido, string direccion, string telefono, string email, string password)
	: Elemento(id), nombre(nombre), apellido(apellido), direccion(direccion), telefono(telefono), email(email), password(password) {
	cuentasBancarias = new ListaDoble<CuentaBancaria*>();
}
Cliente::Cliente(int id, string nombre, string apellido, string direccion, string telefono, string email, string password)
	: Elemento(id), nombre(nombre), apellido(apellido), direccion(direccion), telefono(telefono), email(email), password(password) {
	cuentasBancarias = new ListaDoble<CuentaBancaria*>();
}

string Cliente::getNombre() {
	return nombre;
}
string Cliente::getApellido() {
	return apellido;
}
string Cliente::getDireccion() {
	return direccion;
}
string Cliente::getTelefono() {
	return telefono;
}
string Cliente::getEmail() {
	return email;
}
string Cliente::getPassword() {
	return password;
}
void Cliente::setNombre(string nombre) { this->nombre = nombre; }
void Cliente::setApellido(string apellido) { this->apellido = apellido; }
void Cliente::setDireccion(string direccion) { this->direccion = direccion; }
void Cliente::setTelefono(string telefono) { this->telefono = telefono; }
void Cliente::setEmail(string email) { this->email = email; }
void Cliente::setPassword(string password) { this->password = password; }

void Cliente::agregarCuentaBancaria(CuentaBancaria* cuentaBancaria) {
	cuentasBancarias->push_back(cuentaBancaria);
}

ListaDoble<CuentaBancaria*>* Cliente::getCuentasBancarias() {
	return cuentasBancarias;
}

void Cliente::actualizarDatos(string nombre, string apellido, string direccion, string telefono, string email, string password) {
	this->nombre = nombre;
	this->apellido = apellido;
	this->direccion = direccion;
	this->telefono = telefono;
	this->email = email;
	this->password = password;
}

string Cliente::escribirLinea() {
	stringstream ss;
	ss << id << "," << nombre << "," << apellido << "," << direccion << "," << telefono << "," << email << "," << password;
	return ss.str();
}

inline string Cliente::escribirCabecera()
{
	return "id,nombre,apellido,direccion,telefono,email,password";
}

void Cliente::leerLinea(string linea) {
	stringstream ss(linea);
	string idStr;
	getline(ss, idStr, ',');
	if (idStr != "") {
		id = stoi(idStr);
		getline(ss, nombre, ',');
		getline(ss, apellido, ',');
		getline(ss, direccion, ',');
		getline(ss, telefono, ',');
		getline(ss, email, ',');
		getline(ss, password, ',');
	}
	loadCuentasBancarias();
}

void Cliente::print() {
	cout << "ID: " << id << endl;
	cout << "Nombre: " << nombre << endl;
	cout << "Apellido: " << apellido << endl;
	cout << "Direccion: " << direccion << endl;
	cout << "Telefono: " << telefono << endl;
	cout << "Email: " << email << endl;
	cout << "Password: " << password << endl;
	cout << "Cuentas Bancarias: " << endl;
	if (cuentasBancarias->head == nullptr) {
		cout << "	No hay cuentas bancarias" << endl;
	}
	else {
		cout << "	Tiene " << cuentasBancarias->getSize() << " cuentas bancarias." << endl;
	}
	cout << endl;
}

void Cliente::loadCuentasBancarias() {
	ListaDoble<CuentaBancaria*>* todasLasCuentas = new ListaDoble<CuentaBancaria*>("Cuentas.csv");
	File<ListaDoble<CuentaBancaria*>*, CuentaBancaria>::leer("Cuentas.csv", todasLasCuentas);
	Nodo<CuentaBancaria*>* temp = todasLasCuentas->head;
	while (temp != nullptr) {
		if (temp->data->getIdCliente() == id) {
			Nodo<CuentaBancaria*>* temp2 = cuentasBancarias->search(temp->data->getId());
			if (cuentasBancarias->getSize() != 0 && temp2 != nullptr) {
				cuentasBancarias->update(temp->data);
			}
			else {
				cuentasBancarias->push_back(temp->data);
			}
		}
		temp = temp->next;
	}
}