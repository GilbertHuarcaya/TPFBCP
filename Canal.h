#pragma once
#include "Elemento.h"
#include "string"
#include "sstream"
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
	int idSede = 0;
	string nombre;
	EstadoCanal estado = CANAL_INACTIVO;
	ETipoDeCanal tipoDeCanal;

public:
	Canal();
	Canal(int id, string nombre, ETipoDeCanal tipoDeCanal, int idSede = 0);
	Canal(int id, string nombre, int tipoDeCanal, int idSede = 0);
	Canal(int id, ETipoDeCanal tipoDeCanal, int idSede = 0);
	~Canal();

	// GETTERS
	string getNombre();
	ETipoDeCanal getTipoDeCanal();
	EstadoCanal getEstado();
	int getIdSede();

	// SETTERS
	void setNombre(string nombre);
	void setIdSede(int idSede);
	void setTipoDeCanal(ETipoDeCanal tipoDeCanal);
	void setActivo(bool activo);

	//UPDATE
	void activar();
	void desactivar();

	// STATES
	bool isActivo();

	// SEARCH

	// FILE
	void leerLinea(string linea) override;
	string escribirLinea() override;

	// PRINT
	void print();
};

Canal::Canal() : Elemento()
{
	nombre = "";
	estado = CANAL_INACTIVO;
	tipoDeCanal = OTROCANAL;
}

inline Canal::Canal(int id, string nombre, ETipoDeCanal tipoDeCanal, int idSede)
{
	this->id = id;
	this->nombre = nombre;
	this->tipoDeCanal = tipoDeCanal;
	this->idSede = idSede;
}

inline Canal::Canal(int id, string nombre, int tipoDeCanal, int idSede)
{
	this->id = id;
	this->nombre = nombre;
	this->tipoDeCanal = ETipoDeCanal(tipoDeCanal);
	this->idSede = idSede;
}

inline Canal::Canal(int id, ETipoDeCanal tipoDeCanal, int idSede)
{
	this->id = id;
	this->tipoDeCanal = tipoDeCanal;
	this->idSede = idSede;
}

Canal::~Canal()
{
}

string Canal::getNombre()
{
	return nombre;
}


ETipoDeCanal Canal::getTipoDeCanal()
{
	return tipoDeCanal;
}

inline EstadoCanal Canal::getEstado()
{
	return estado;
}

inline int Canal::getIdSede()
{
	return idSede;
}

bool Canal::isActivo()
{
	return estado == EstadoCanal(1);
}

void Canal::setNombre(string nombre)
{
	this->nombre = nombre;
}

inline void Canal::setIdSede(int idSede)
{
	this->idSede = idSede;
}

void Canal::setTipoDeCanal(ETipoDeCanal tipoDeCanal)
{
	this->tipoDeCanal = tipoDeCanal;
}

inline void Canal::setActivo(bool activo)
{
	estado = activo ? CANAL_ACTIVO : CANAL_INACTIVO;
}

inline void Canal::activar()
{
	estado = CANAL_ACTIVO;
}

inline void Canal::desactivar()
{
	estado = CANAL_INACTIVO;
}

void Canal::leerLinea(string linea)
{
	stringstream ss(linea);
	string item;
	getline(ss, item, ',');
	if (item != "" || item != "id") {
		id = stoi(item);
		getline(ss, item, ',');
		if (item != "") {
			idSede = stoi(item);
		}
		getline(ss, nombre, ',');
		getline(ss, item, ',');
		tipoDeCanal = ETipoDeCanal(stoi(item));
		getline(ss, item, ',');
		estado = EstadoCanal(stoi(item));
	}
}

string Canal::escribirLinea()
{
	stringstream ss;
	ss << getId() << "," << idSede << "," << nombre << "," << tipoDeCanal << "," << estado;
	return ss.str();
}

void Canal::print()
{
	cout << "ID:" << endl;
	if (idSede != 0) {
		cout << "ID Sede: " << idSede << endl;
	}
	cout << "Nombre: " << nombre << endl;
	cout << "Tipo de Canal: " << tipoDeCanal << endl;
	cout << "Estado: " << estado << endl;

}