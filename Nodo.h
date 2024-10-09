#pragma once
template <class T>
class Nodo
{
public:
	T data;
	Nodo<T>* next;
	Nodo<T>* prev;
	Nodo(T data)
	{
		this->data = data;
		next = nullptr;
		prev = nullptr;
	}
};
