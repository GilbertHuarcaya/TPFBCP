#pragma once
#include "iostream"
#include "vector"
#include "GArista.h"
using namespace std;

template <typename T, typename Q>
class GVertice
{
	T dato;
public:
	vector<GArista<Q>*>* aristas;
	GVertice();
	GVertice(T dato);
	void setDato(T dato);
	T getDato();
	GArista<Q>* getAristaPorPos(int pos);
	void agregarArista(GArista<Q>* arista);
};

template <typename T, typename Q>
GVertice<T, Q>::GVertice()
{
	dato = nullptr;
	aristas = new vector<GArista<Q>*>;
}

template <typename T, typename Q>
GVertice<T, Q>::GVertice(T dato)
{
	this->dato = dato;
	aristas = new vector<GArista<Q>*>;
}

template <typename T, typename Q>
void GVertice<T, Q>::setDato(T dato)
{
	this->dato = dato;
}

template <typename T, typename Q>
T GVertice<T, Q>::getDato()
{
	return dato;
}

template <typename T, typename Q>
GArista<Q>* GVertice<T, Q>::getAristaPorPos(int pos)
{
	if (aristas == nullptr)return nullptr;
	if (pos < 0)return nullptr;
	return aristas->size() > pos ? aristas->at(pos) : nullptr;
}

template <typename T, typename Q>
void GVertice<T, Q>::agregarArista(GArista<Q>* arista)
{
	aristas->push_back(arista);
}
