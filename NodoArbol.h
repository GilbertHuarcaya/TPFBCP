#pragma once
template <class T>
class NodoArbol {
public:
	T dato;
	NodoArbol<T>* izquierda;
	NodoArbol<T>* derecha;
	NodoArbol(T dato)
	{
		this->dato = dato;
		this->izquierda = nullptr;
		this->derecha = nullptr;
	}
	void setDato(T dato) { this->dato = dato;}
	void setNodoIzq(NodoArbol<T>* izq) { this->izquierda = izq; }
	void setNodoDer(NodoArbol<T>* der) { this->derecha = der; }
};