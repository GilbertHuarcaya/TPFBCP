#include "pch.h"
#include "Elemento.h"


Elemento::Elemento()
{
	id = 0;
	fechaCreacion = time(0);
	fechaEdicion = time(0);
}

Elemento::Elemento(int id)
{
	this->id = id;
	fechaCreacion = time(0);
	fechaEdicion = time(0);
}

Elemento::~Elemento()
{
}

void Elemento::setId(int id)
{
	this->id = id;
}

int Elemento::getId()
{
	return id;
}

void Elemento::setFechaCreacion(time_t fechaCreacion)
{
	this->fechaCreacion = fechaCreacion;
}

time_t Elemento::getFechaCreacion()
{
	return fechaCreacion;
}

void Elemento::setFechaEdicion(time_t fechaEdicion)
{
	this->fechaEdicion = fechaEdicion;
}

time_t Elemento::getFechaEdicion()
{
	return fechaEdicion;
}

