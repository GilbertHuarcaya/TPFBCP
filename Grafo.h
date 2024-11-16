#pragma once
#include <iostream>
#include <vector>
using namespace std;
template <typename T>
class GArista 
{
	int posInicio;//Posicion del vertice de inicio
	int posLlegada;//Posicion del Vertice de llegada
	T dato;//Dato de la arista
public:
	GArista()
	{
		posLlegada = -1;
		dato = nullptr;
	}
	GArista(T dato)
	{
		this->dato = dato;
		this->posLlegada = -1;
	}
	GArista(T dato, int posInicio, int posLlegada)
	{
		this->dato = dato;
		this->posInicio = posInicio;
		this->posLlegada = posLlegada;
	}
	void setDato(T dato)
	{
		this->dato = dato;
	}
	void setPosLlegada(int pos)
	{
		this->posLlegada = pos;
	}
	T getDato()
	{
		return dato;
	}
	int getPosLlegada()
	{
		return posLlegada;
	}
};

template <typename T, typename Q>
//T para el dato del vertice, Q para el dato de la arista
class GVertice 
{
	T dato;
	vector<GArista<Q>*>* aristas;
public:
	GVertice()
	{
		dato = nullptr;
		aristas = new vector<GArista<Q>*>;
	}
	GVertice(T dato)
	{
		this->dato = dato;
		aristas = new vector<GArista<Q>*>;
	}
	void setDato(T dato)
	{
		this->dato = dato;
	}
	T getDato()
	{
		return dato;
	}
	GArista<Q>* getAristaPorPos(int pos)
	{
		if (aristas == nullptr)return nullptr;
		if (pos < 0)return nullptr;
		return aristas->size() > pos ? aristas->at(pos) : nullptr;
	}
	void agregarArista(GArista<Q>* arista)
	{
		aristas->push_back(arista);
	}
};

template <typename T, typename Q>
//T para el dato del vertice, Q para el dato de la arista
class Grafo
{
	vector<GVertice<T, Q>*>* vertices;
	int lastPos;
	bool esRepetido(T dato)
	//Devuelve false si el dato no esta repetido
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
	int getPosDato(T dato)
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
public:
	Grafo()
	{
		vertices = new vector<GVertice<T, Q>*>;
		lastPos = -1;//CAMBIO==========================
	}
	Grafo(T dato_a, T dato_b, Q arista)
	//Enlaza dos datos diferentes con una arista 
	{
		vertices = new vector<GVertice<T, Q>*>;
		vertices->push_back(new GVertice<T, Q>(dato_a));
		vertices->push_back(new GVertice<T, Q>(dato_b));
		vertices->at(0)->agregarArista(new GArista<Q>(arista, 0 , 1));
		vertices->at(1)->agregarArista(new GArista<Q>(arista, 1 , 0));
		lastPos = 1;
	}
	Grafo(T dato, Q arista)
	//Dato igual que se enlaza a si mismo (Ejemplo: Una persona retira de su misma cuenta 200 soles, el cual hace que la cuenta del cliente haga una operacion en si mismo)
	{
		vertices = new vector<GVertice<T, Q>*>;
		vertices->push_back(new GVertice<T, Q>(dato));
		vertices->at(0)->agregarArista(new GArista<Q>(arista,0,0));
		lastPos = 0;
	}
	int getLastPosVertice()
	//Devuelve posicion del ultimo vertice ingresado
	{
		return lastPos;
	}
	int cantidadVertices()
	//Devuelve cantidad de vertices
	{
		return vertices->size();
	}
	bool agregarVertice(T dato)
	//Agrega un nuevo vertice
	//Devuelve true si llego a agregarlo, si no, false
	{
		if (esRepetido(dato))return false;
		GVertice<T, Q>* aux = new GVertice<T, Q>(dato);
		aux->setDato(dato);
		vertices->push_back(aux);
		lastPos++;
		return true;
	}
	void agregarVerticesEnlazadosNoDirigido(T dato_a, T dato_b, Q arista)
	//Agrega dos nuevos vertices y los enlaza con una arista 
	{
		if (!agregarVertice(dato_a))return;
		if (!agregarVertice(dato_b))return;
		int aux_a = lastPos - 1;//Posicion de dato_a
		vertices->at(lastPos)->agregarArista(new GArista<Q>(arista, lastPos ,aux_a));
		vertices->at(aux_a)->agregarArista(new GArista<Q>(arista, aux_a, lastPos));
	}
	void agregarVerticesEnlazadosDirigido(T dato_a, T dato_b, Q arista)
	{
		//Agrega dos nuevos vertices y enlaza el dato_a apuntado a dato_b
		if (!agregarVertice(dato_a))return;
		if (!agregarVertice(dato_b))return;
		int aux_a = lastPos - 1;//Posicion de dato_a
		vertices->at(aux_a)->agregarArista(new GArista<Q>(arista, aux_a, lastPos));
	}
	void agregarVerticeconBucle(T dato, Q arista)
	//Agrega un nuevo dato y enlaza el dato a si mismo con una arista
	{
		if(!agregarVertice(dato))return;
		vertices->at(lastPos)->agregarArista(new GArista<Q>(arista, lastPos, lastPos));
	}
	void enlazarVerticesNoDirigido(T dato_a, T dato_b, Q arista)
	//Tienen que estar en el vector de vertices
	{
		if (getPosDato(dato_a) == -1||getPosDato(dato_b) == -1)return;
		vertices->at(getPosDato(dato_a))->agregarArista(new GArista<Q>(arista, getPosDato(dato_a), getPosDato(dato_b)));
		vertices->at(getPosDato(dato_b))->agregarArista(new GArista<Q>(arista, getPosDato(dato_b), getPosDato(dato_a)));
	}
	void enlazarVerticesDirigido(T dato_a, T dato_b, Q arista)
	//Enlaza dato_a a dato_b (ya en el vector de vertices)
	{
		if (getPosDato(dato_a) == -1 || getPosDato(dato_b) == -1)return;
		vertices->at(getPosDato(dato_a))->agregarArista(new GArista<Q>(arista, getPosDato(dato_a), getPosDato(dato_b)));
	}
	void enlazarVerticeBucle(T dato, Q arista)
	//Enlaza un dato en el vector de vertices a si mismo
	{
		if (getPosDato(dato) == -1)return;
		vertices->at(getPosDato(dato))->agregarArista(new GArista<Q>(arista, getPosDato(dato), getPosDato(dato)));
	}
	T getUltimoDato()
	{
		if (lastPos == -1)return nullptr;
		return vertices->at(lastPos)->getDato();
	}
	T getDatoporPos(int pos)
	//Obtiene el dato de un vertice mediante su posicion en el vector
	{
		if (pos < 0 || pos >= vertices->size())return nullptr;
		return vertices->at(pos)->getDato();
	}
	Q getAristaporPos(int pos_vertice, int pos_arista)
	//Obtiene el dato de la arista mediante la posicion del vertice y la posicion de la arista
	{
		if (pos_vertice < 0 || pos_vertice >= vertices->size())return nullptr;
		return vertices->at(pos_vertice)->getAristaPorPos(pos_arista)->getDato();
	}
};