#pragma once
#include <iostream>
#include <string>
#include <ctime>
#include <iomanip> // get_time
using namespace std;

class Elemento
{
protected:
	int id;
	time_t fechaCreacion;
	time_t fechaEdicion;
public:
	Elemento();
	Elemento(int id);
	~Elemento();
	virtual string escribirLinea() = 0;
	virtual void leerLinea(string linea) = 0;
	void setId(int id);
	int getId();
	void setFechaCreacion(time_t fechaCreacion);
	time_t getFechaCreacion();
	void setFechaEdicion(time_t fechaEdicion);
	time_t getFechaEdicion();
};
