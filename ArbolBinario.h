#pragma once
#include "NodoArbol.h"
#include "functional"
template <class T>
class ArbolBinario {
	NodoArbol<T>* raiz;
	function<bool(T,T)> requisito_division;//Puntero a funcion que decidira si el nodo ingresado ira a la izq o a la der 
	int total;
	int _altura(NodoArbol<T>* nodo)
	{
		int ai, ad;
		if (nodo != nullptr)
		{
			ai = 1 + _altura(nodo->izquierda);
			ad = 1 + _altura(nodo->derecha);
			return ai > ad ? ai : ad;
		}
		else return 0;
	}
	void _insertar(T dato, NodoArbol<T>*& nodo)
	{
		if (nodo != nullptr)
		{
			NodoArbol<T>* aux;
			if (requisito_division(dato, nodo->dato))
			{
				return _insertar(dato, nodo->izquierda);
			}
			else
			{
				return _insertar(dato, nodo->derecha);
			}
		}
		else
		{
			nodo = new NodoArbol<T>(dato);
			total++;
		}
	}
	void _PreOrden(function<void(T)> funcion, NodoArbol<T>* nodo)
	{
		if (nodo == nullptr)return;
		if (nodo->dato == nullptr)return;
		funcion(nodo->dato);
		_PreOrden(funcion, nodo->izquierda);
		_PreOrden(funcion, nodo->derecha);
	}
	void _EnOrden(function<void(T)> funcion, NodoArbol<T>* nodo)
	{
		if (nodo == nullptr)return;
		_EnOrden(funcion, nodo->izquierda);
		funcion(nodo->dato);
		_EnOrden(funcion, nodo->derecha);

	}
	void _PostOrden(function<void(T)> funcion, NodoArbol<T>* nodo)
	{
		if (nodo == nullptr)return;
		_PostOrden(funcion, nodo->izquierda);
		_PostOrden(funcion, nodo->derecha);
		funcion(nodo->dato);
	}
public:
	ArbolBinario()
	{
		raiz = nullptr;
		requisito_division = [](T a,T b) -> bool {
			return false};
		total = 0;
	}
	ArbolBinario(function<bool(T, T)> funcion)
	{
		raiz = nullptr;
		requisito_division = funcion;
		total = 0;
	}
	ArbolBinario(T dato, function<bool(T,T)> funcion)
	{
		raiz = new NodoArbol<T>(dato);
		requisito_division = funcion;
		total = 1;
	}
	NodoArbol<T>* getRaiz() { return raiz; }
	int altura()
	{
		return _altura(raiz);
	}
	void insertar(T dato)
	{
		return _insertar(dato, raiz);
	}
	// Raiz -> Izquierda -> Derecha
	void PreOrden(function<void(T)> funcion)
	{
		return _PreOrden(funcion, getRaiz());
	}
	// Izquierda -> Raiz -> Derecha
	void EnOrden(function<void(T)> funcion)
	{
		return _EnOrden(funcion,raiz);
	}
	// Izquierda -> Derecha -> Raiz
	void PostOrden(function<void(T)> funcion)
	{
		return _PostOrden(funcion, raiz);
	}
	NodoArbol<T>* buscarNodoPorId(int id, NodoArbol<T>* aux)
	{
		if (aux != nullptr) {
			if (aux->dato->getId() != id)
			{
				if (buscarNodoPorId(id, aux->izquierda) != nullptr)
					return buscarNodoPorId(id,aux->izquierda);
				if (buscarNodoPorId(id, aux->derecha) != nullptr)
					return buscarNodoPorId(id,aux->derecha);
				return nullptr;
			}
			else return aux;
		}
		else return aux;
	}
};