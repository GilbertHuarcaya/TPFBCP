#pragma once
#include "iostream"
#include "vector"
#include "GVertice.h"

using namespace std;

template <typename T, typename Q>
class Grafo
{
	vector<GVertice<T, Q>*>* vertices;
	int lastPos;
	bool esRepetido(T dato);
	int getPosDato(T dato);
public:
	Grafo();
	Grafo(T dato_a, T dato_b, Q arista);
	Grafo(T dato, Q arista);
	int getLastPosVertice();
	int cantidadVertices();
	bool agregarVertice(T dato);
	void agregarVerticesEnlazadosNoDirigido(T dato_a, T dato_b, Q arista);
	void agregarVerticesEnlazadosDirigido(T dato_a, T dato_b, Q arista);
	void agregarVerticeconBucle(T dato, Q arista);
	void enlazarVerticesNoDirigido(T dato_a, T dato_b, Q arista);
	void enlazarVerticesDirigido(T dato_a, T dato_b, Q arista);
	void enlazarVerticeBucle(T dato, Q arista);
	T getUltimoDato();
	T getDatoporPos(int pos);
	Q getAristaporPos(int pos_vertice, int pos_arista);
};

template <typename T, typename Q>
Grafo<T, Q>::Grafo()
{
	vertices = new vector<GVertice<T, Q>*>;
	lastPos = -1;
}

template <typename T, typename Q>
Grafo<T, Q>::Grafo(T dato_a, T dato_b, Q arista)
{
	vertices = new vector<GVertice<T, Q>*>;
	vertices->push_back(new GVertice<T, Q>(dato_a));
	vertices->push_back(new GVertice<T, Q>(dato_b));
	vertices->at(0)->agregarArista(new GArista<Q>(arista, 0 , 1));
	vertices->at(1)->agregarArista(new GArista<Q>(arista, 1 , 0));
	lastPos = 1;
}

template <typename T, typename Q>
Grafo<T, Q>::Grafo(T dato, Q arista)
{
	vertices = new vector<GVertice<T, Q>*>;
	vertices->push_back(new GVertice<T, Q>(dato));
	vertices->at(0)->agregarArista(new GArista<Q>(arista,0,0));
	lastPos = 0;
}

template <typename T, typename Q>
int Grafo<T, Q>::getLastPosVertice()
{
	return lastPos;
}

template <typename T, typename Q>
int Grafo<T, Q>::cantidadVertices()
{
	return vertices->size();
}

template <typename T, typename Q>
bool Grafo<T, Q>::agregarVertice(T dato)
{
	if (esRepetido(dato))return false;
	GVertice<T, Q>* aux = new GVertice<T, Q>(dato);
	aux->setDato(dato);
	vertices->push_back(aux);
	lastPos++;
	return true;
}

template <typename T, typename Q>
void Grafo<T, Q>::agregarVerticesEnlazadosNoDirigido(T dato_a, T dato_b, Q arista)
{
	if (!agregarVertice(dato_a))return;
	if (!agregarVertice(dato_b))return;
	int aux_a = lastPos - 1;
	vertices->at(lastPos)->agregarArista(new GArista<Q>(arista, lastPos ,aux_a));
	vertices->at(aux_a)->agregarArista(new GArista<Q>(arista, aux_a, lastPos));
}

template <typename T, typename Q>
void Grafo<T, Q>::agregarVerticesEnlazadosDirigido(T dato_a, T dato_b, Q arista)
{
	if (!agregarVertice(dato_a))return;
	if (!agregarVertice(dato_b))return;
	int aux_a = lastPos - 1;
	vertices->at(aux_a)->agregarArista(new GArista<Q>(arista, aux_a, lastPos));
}

template <typename T, typename Q>
void Grafo<T, Q>::agregarVerticeconBucle(T dato, Q arista)
{
	if(!agregarVertice(dato))return;
	vertices->at(lastPos)->agregarArista(new GArista<Q>(arista, lastPos, lastPos));
}

template <typename T, typename Q>
void Grafo<T, Q>::enlazarVerticesNoDirigido(T dato_a, T dato_b, Q arista)
{
	if (getPosDato(dato_a) == -1||getPosDato(dato_b) == -1)return;
	vertices->at(getPosDato(dato_a))->agregarArista(new GArista<Q>(arista, getPosDato(dato_a), getPosDato(dato_b)));
	vertices->at(getPosDato(dato_b))->agregarArista(new GArista<Q>(arista, getPosDato(dato_b), getPosDato(dato_a)));
}

template <typename T, typename Q>
void Grafo<T, Q>::enlazarVerticesDirigido(T dato_a, T dato_b, Q arista)
{
	if (getPosDato(dato_a) == -1 || getPosDato(dato_b) == -1)return;
	vertices->at(getPosDato(dato_a))->agregarArista(new GArista<Q>(arista, getPosDato(dato_a), getPosDato(dato_b)));
}

template <typename T, typename Q>
void Grafo<T, Q>::enlazarVerticeBucle(T dato, Q arista)
{
	if (getPosDato(dato) == -1)return;
	vertices->at(getPosDato(dato))->agregarArista(new GArista<Q>(arista, getPosDato(dato), getPosDato(dato)));
}

template <typename T, typename Q>
T Grafo<T, Q>::getUltimoDato()
{
	if (lastPos == -1)return nullptr;
	return vertices->at(lastPos)->getDato();
}

template <typename T, typename Q>
T Grafo<T, Q>::getDatoporPos(int pos)
{
	if (pos < 0 || pos >= vertices->size())return nullptr;
	return vertices->at(pos)->getDato();
}

template <typename T, typename Q>
Q Grafo<T, Q>::getAristaporPos(int pos_vertice, int pos_arista)
{
	if (pos_vertice < 0 || pos_vertice >= vertices->size())return nullptr;
	return vertices->at(pos_vertice)->getAristaPorPos(pos_arista)->getDato();
}

template <typename T, typename Q>
bool Grafo<T, Q>::esRepetido(T dato)
{
	bool verif = false;
	for (int i = 0; i < vertices->size(); i++)
	{
		if (dato == vertices->at(i)->getDato())
		{
			verif = true;
			break;
		}
	}
	return verif;
}

template <typename T, typename Q>
int Grafo<T, Q>::getPosDato(T dato)
{
	int aux = -1;
	for (int i = 0; i < vertices->size(); i++)
	{
		if (dato == vertices->at(i)->getDato())
		{
			aux = i;
			break;
		}
	}
	return aux;
}
