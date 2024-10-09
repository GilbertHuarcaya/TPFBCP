#pragma once
#include <string>
#include "ListaDoble.h"
using namespace std;

template <class T>
class Cola : public ListaDoble<T>
{
public:
	Cola() : ListaDoble<T>() {}
	Cola(string nombreArchivo) : ListaDoble<T>(nombreArchivo) {}
	void encolar(T data)
	{
		ListaDoble<T>::push_back(data);
	}
	T desencolar()
	{
		return ListaDoble<T>::pop_front();
	}
	T front()
	{
		return ListaDoble<T>::head->data;
	}
	T back()
	{
		return ListaDoble<T>::tail->data;
	}
	bool empty()
	{
		return ListaDoble<T>::head == nullptr;
	}
};
