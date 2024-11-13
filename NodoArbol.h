#pragma once
template <class T>
class NodoArbol {
public:
	T data;
	NodoArbol<T>* izquierda;
	NodoArbol<T>* derecha;
	NodoArbol(T data)
	{
		this->data = data;
		this->izquierda = nullptr;
		this->derecha = nullptr;
	}
	void setDato(T data) { this->data = data;}
	void setNodoIzq(NodoArbol<T>* izq) { this->izquierda = izq; }
	void setNodoDer(NodoArbol<T>* der) { this->derecha = der; }
};