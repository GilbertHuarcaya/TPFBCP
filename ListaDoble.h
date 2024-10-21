#pragma once
#include <string>
#include <iostream>
#include "Nodo.h"
using namespace std;

template <class T>
class ListaDoble
{
protected:
	string nombreArchivo;
public:
	Nodo<T>* head;
	Nodo<T>* tail;
	int lastId = 0; // Para identificar el último nodo agregado
	int size = 0;
	ListaDoble()
	{
		nombreArchivo = "";
		head = nullptr;
		tail = nullptr;
	}
	ListaDoble(string nombreArchivo)
	{
		this->nombreArchivo = nombreArchivo;
		head = nullptr;
		tail = nullptr;
	}
	~ListaDoble()
	{
		clear();
	}
	string getNombreArchivo()
	{
		return nombreArchivo;
	}
	void push_back(T data)
	{
		Nodo<T>* nodo = new Nodo<T>(data);
		if (head == nullptr)
		{
			head = nodo;
			tail = nodo;
		}
		else
		{
			tail->next = nodo;
			nodo->prev = tail;
			tail = nodo;
		}

		if (nodo->data->getId() > lastId)
		{
			lastId = nodo->data->getId() + 1;
		}
		else {
			lastId++;
		}
		size++;
	}
	void push_front(T data)
	{
		Nodo<T>* nodo = new Nodo<T>(data);
		if (head == nullptr)
		{
			head = nodo;
			tail = nodo;

		}
		else
		{
			nodo->next = head;
			head->prev = nodo;
			head = nodo;
		}

		if (nodo->data->getId() > lastId)
		{
			lastId = nodo->data->getId() + 1;
		}
		size++;
	}
	T pop_back()
	{
		if (head == nullptr)
		{
			return nullptr;
		}
		if (size > 0) {
			size--;
		}
		if (head == tail)
		{
			T data = tail->data;
			delete head;
			head = nullptr;
			tail = nullptr;
			return data;
		}
		else
		{
			Nodo<T>* temp = tail;
			T data = temp->data;
			tail = tail->prev;
			tail->next = nullptr;
			delete temp;
			return data;
		}
		size--;
	}
	T pop_front()
	{
		if (head == nullptr)
		{
			return nullptr;
		}
		if (size > 0) {
			size--;
		}
		if (head == tail)
		{
			T data = head->data;
			delete head;
			head = nullptr;
			tail = nullptr;
			return data;
		}
		else
		{
			Nodo<T>* temp = head;
			T data = temp->data;
			head = head->next;
			head->prev = nullptr;
			delete temp;
			return data;
		}
	}
	void remove(Nodo<T>* nodo)
	{
		if (nodo == nullptr)
		{
			return;
		}
		if (nodo == head)
		{
			pop_front();
		}
		else if (nodo == tail)
		{
			pop_back();
		}
		else
		{
			nodo->prev->next = nodo->next;
			nodo->next->prev = nodo->prev;
			delete nodo;
		}
		size--;
	}
	void clear()
	{
		while (head != nullptr)
		{
			pop_front();
		}
	}
	void print()
	{
		Nodo<T>* temp = head;
		while (temp != nullptr && temp->data != nullptr)
		{
			temp->data->print();
			cout << endl;
			temp = temp->next;
		}
		cout << endl;
	}
	Nodo<T>* search(int id)
	{
		Nodo<T>* temp = head;
		while (temp != nullptr)
		{
			if (temp->data->getId() == id)
			{
				return temp;
			}
			temp = temp->next;
		}
		return nullptr;
	}
	Nodo<T>* search(T data)
	{
		Nodo<T>* temp = head;
		while (temp != nullptr)
		{
			if (temp->data == data)
			{
				return temp;
			}
			temp = temp->next;
		}
		return nullptr;
	}
	Nodo<T>* getPrevious(Nodo<T>* nodo)
	{
		return nodo->prev;
	}
	Nodo<T>* getByPosition(int position)
	{
		Nodo<T>* temp = head;
		int i = 0;
		while (temp != nullptr)
		{
			if (i == position)
			{
				return temp;
			}
			temp = temp->next;
			i++;
		}
		return nullptr;
	}
	Nodo<T>* getNext(Nodo<T>* nodo)
	{
		return nodo->next;
	}
	Nodo<T>* getLast()
	{
		return tail;
	}
	Nodo<T>* getFirst()
	{
		return head;
	}
	int getSize()
	{
		return size;
	}
	int getLastId()
	{
		return lastId;
	}
};
