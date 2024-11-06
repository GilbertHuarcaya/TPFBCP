#pragma once
#include "Nodo.h"
#include "Elemento.h"

template <typename T = Elemento*, typename KEY = int>
class HashEntidadNodo {
public:
	KEY key;
	T data;
	HashEntidadNodo<T>* next;
	HashEntidadNodo<T>* prev;
	HashEntidadNodo(T data, KEY key = 0)
	{
		this->key = key;
		this->data = data;
		next = nullptr;
		prev = nullptr;
	}
};
