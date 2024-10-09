#pragma once
#include "Elemento.h"
#include <string>
#include <sstream>
using namespace std;

enum EstadoSede {
	SEDE_INACTIVO,
	SEDE_ACTIVO
};

class Sede : public Elemento
{
private:
	string nombre;
	string direccion;
	string telefono;
	string email;
	EstadoSede estado;
public:
	Sede();
	Sede(int id);
	Sede(int id, string nombre, string direccion, string telefono, string email);
	string getNombre();
	string getDireccion();
	string getTelefono();
	string getEmail();
	void setNombre(string nombre);
	void setDireccion(string direccion);
	void setTelefono(string telefono);
	void setEmail(string email);
	void setEstado(EstadoSede estado);
	EstadoSede getEstado();
	void activar();
	void desactivar();
	string escribirLinea() override;
	void leerLinea(string linea) override;

	void print();
	
};

Sede::Sede() : Elemento() {
	this->id = 0;
	this->nombre = "";
	this->direccion = "";
	this->telefono = "";
	this->email = "";
	this->estado = SEDE_INACTIVO;
};
Sede::Sede(int id) : Elemento(id) {
	this->nombre = "";
	this->direccion = "";
	this->telefono = "";
	this->email = "";
	this->estado = SEDE_INACTIVO;
};

Sede::Sede(int id, string nombre, string direccion, string telefono, string email)
	: Elemento(id), nombre(nombre), direccion(direccion), telefono(telefono), email(email) {
	estado = SEDE_INACTIVO;
}

string Sede::getNombre() {
	return nombre;
}

string Sede::getDireccion() {
	return direccion;
}

string Sede::getTelefono() {
	return telefono;
}

string Sede::getEmail() {
	return email;
}

void Sede::setNombre(string nombre) {
	this->nombre = nombre;
}

void Sede::setDireccion(string direccion) {
	this->direccion = direccion;
}

void Sede::setTelefono(string telefono) {
	this->telefono = telefono;
}

void Sede::setEmail(string email) {
	this->email = email;
}

void Sede::setEstado(EstadoSede estado) {
	this->estado = estado;
}

EstadoSede Sede::getEstado() {
	return estado;
}

void Sede::activar() {
	estado = SEDE_ACTIVO;
}

void Sede::desactivar() {
	estado = SEDE_INACTIVO;
}

string Sede::escribirLinea() {
	stringstream ss;
	ss << id << "," << nombre << "," << direccion << "," << telefono << "," << email << "," << estado;
	return ss.str();
}

void Sede::leerLinea(string linea) {
	stringstream ss(linea);
	string item;
	getline(ss, item, ',');
	if (item != "" || item != "id") {
		id = stoi(item);
		getline(ss, nombre, ',');
		getline(ss, direccion, ',');
		getline(ss, telefono, ',');
		getline(ss, email, ',');
		getline(ss, item, ',');
		estado = item != "" ? EstadoSede(stoi(item)) : SEDE_INACTIVO;
	}

}

inline void Sede::print()
{
	cout << "ID: " << id << endl;
	cout << "Nombre: " << nombre << endl;
	cout << "Direccion: " << direccion << endl;
	cout << "Telefono: " << telefono << endl;
	cout << "Email: " << email << endl;
	cout << "Estado: " << estado << endl;
}
