#pragma once
#include "iostream"
#include "vector"
using namespace std;

template <typename T>
class GArista
{
	int posInicio;
	int posLlegada;
	T dato;
public:
	GArista();
	GArista(T dato);
	GArista(T dato, int posInicio, int posLlegada);
	void setDato(T dato);
	void setPosLlegada(int pos);
	T getDato();
	int getPosLlegada();
};

template <typename T>
GArista<T>::GArista()
{
	posLlegada = -1;
	dato = nullptr;
}

template <typename T>
GArista<T>::GArista(T dato)
{
	this->dato = dato;
	this->posLlegada = -1;
}

template <typename T>
GArista<T>::GArista(T dato, int posInicio, int posLlegada)
{
	this->dato = dato;
	this->posInicio = posInicio;
	this->posLlegada = posLlegada;
}

template <typename T>
void GArista<T>::setDato(T dato)
{
	this->dato = dato;
}

template <typename T>
void GArista<T>::setPosLlegada(int pos)
{
	this->posLlegada = pos;
}

template <typename T>
T GArista<T>::getDato()
{
	return dato;
}

template <typename T>
int GArista<T>::getPosLlegada()
{
	return posLlegada;
}