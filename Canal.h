#pragma once
#include <string>
#include <sstream>
#include "Elemento.h"
using namespace std;

enum EstadoCanal {
	CANAL_INACTIVO,
	CANAL_ACTIVO
};

enum ETipoDeCanal {
	OTROCANAL = 0,
	VENTANILLA = 1,
	AGENTE,
	WEB,
	APP,
	YAPE,
	CAJERO,
};

class Canal : public Elemento
{
private:
	string nombre;
	string direccion;
	string ciudad;
	string distrito;
	string departamento;
	EstadoCanal estado = CANAL_INACTIVO;
	ETipoDeCanal tipoDeCanal;
public:
	Canal();
	Canal(int id, string nombre, string direccion, string ciudad, string distrito, string departamento, ETipoDeCanal tipoDeCanal);
	Canal(int id, string nombre, string direccion, string ciudad, string distrito, string departamento, int tipoDeCanal);
	Canal(int id, ETipoDeCanal tipoDeCanal);
	~Canal();
	void print();
	string getNombre();
	string getDireccion();
	string getCiudad();
	string getDistrito();
	string getDepartamento();
	ETipoDeCanal getTipoDeCanal();
	EstadoCanal getEstado();
	bool isActivo();
	void setNombre(string nombre);
	void setDireccion(string direccion);
	void setCiudad(string ciudad);
	void setDistrito(string distrito);
	void setDepartamento(string departamento);
	void setTipoDeCanal(ETipoDeCanal tipoDeCanal);
	void setActivo(bool activo);

	void leerLinea(string linea) override;
	string escribirLinea() override;

};

Canal::Canal() : Elemento()
{
	nombre = "";
	direccion = "";
	ciudad = "";
	distrito = "";
	departamento = "";
	estado = CANAL_INACTIVO;
	tipoDeCanal = OTROCANAL;
}

Canal::Canal(int id, string nombre, string direccion, string ciudad, string distrito, string departamento, ETipoDeCanal tipoDeCanal) : Elemento(id)
{
	this->nombre = nombre;
	this->direccion = direccion;
	this->ciudad = ciudad;
	this->distrito = distrito;
	this->departamento = departamento;
	this->tipoDeCanal = tipoDeCanal;
}

Canal::Canal(int id, string nombre, string direccion, string ciudad, string distrito, string departamento, int tipoDeCanal) : Elemento(id)
{
	this->nombre = nombre;
	this->direccion = direccion;
	this->ciudad = ciudad;
	this->distrito = distrito;
	this->departamento = departamento;
	this->tipoDeCanal = ETipoDeCanal(tipoDeCanal);
}

Canal::Canal(int id, ETipoDeCanal tipoDeCanal) : Elemento(id)
{
	this->nombre = "";
	this->direccion = "";
	this->ciudad = "";
	this->distrito = "";
	this->departamento = "";
	this->tipoDeCanal = tipoDeCanal;
}

Canal::~Canal()
{
}

string Canal::getNombre()
{
	return nombre;
}

void Canal::print()
{
	cout << "ID:" << endl;
	cout << "Nombre: " << nombre << endl;
	cout << "Direccion: " << direccion << endl;
	cout << "Ciudad: " << ciudad << endl;
	cout << "Distrito: " << distrito << endl;
	cout << "Departamento: " << departamento << endl;
	cout << "Tipo de Canal: " << tipoDeCanal << endl;
	cout << "Estado: " << estado << endl;
}

string Canal::getDireccion()
{
	return direccion;
}

string Canal::getCiudad()
{
	return ciudad;
}

string Canal::getDistrito()
{
	return distrito;
}

string Canal::getDepartamento()
{
	return departamento;
}

ETipoDeCanal Canal::getTipoDeCanal()
{
	return tipoDeCanal;
}

inline EstadoCanal Canal::getEstado()
{
	return estado;
}

bool Canal::isActivo()
{
	return estado == EstadoCanal(1);
}

void Canal::setNombre(string nombre)
{
	this->nombre = nombre;
}

void Canal::setDireccion(string direccion)
{
	this->direccion = direccion;
}

void Canal::setCiudad(string ciudad)
{
	this->ciudad = ciudad;
}

void Canal::setDistrito(string distrito)
{
	this->distrito = distrito;
}

void Canal::setDepartamento(string departamento)
{
	this->departamento = departamento;
}

void Canal::setTipoDeCanal(ETipoDeCanal tipoDeCanal)
{
	this->tipoDeCanal = tipoDeCanal;
}

inline void Canal::setActivo(bool activo)
{
	estado = activo ? CANAL_ACTIVO : CANAL_INACTIVO;
}


void Canal::leerLinea(string linea)
{
	stringstream ss(linea);
	string id;
	getline(ss, id, ',');
	setId(stoi(id));
	getline(ss, nombre, ',');
	getline(ss, direccion, ',');
	getline(ss, ciudad, ',');
	getline(ss, distrito, ',');
	getline(ss, departamento, ',');
	string tipoDeCanal;
	getline(ss, tipoDeCanal, ',');
	setTipoDeCanal(ETipoDeCanal(stoi(tipoDeCanal)));
	string estado;
	getline(ss, estado, ',');
	setActivo(stoi(estado));
}

string Canal::escribirLinea()
{
	stringstream ss;
	ss << getId() << "," << nombre << "," << direccion << "," << ciudad << "," << distrito << "," << departamento << "," << tipoDeCanal << "," << estado;
	return ss.str();
}