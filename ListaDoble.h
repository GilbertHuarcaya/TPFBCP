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

	// Función para dividir la lista en dos mitades
	void frontBackSplit(Nodo<T>* source, Nodo<T>** frontRef, Nodo<T>** backRef) {
		Nodo<T>* fast;
		Nodo<T>* slow;
		slow = source;
		fast = source->next;

		// Avanza 'fast' dos nodos y 'slow' un nodo
		while (fast != nullptr) {
			fast = fast->next;
			if (fast != nullptr) {
				slow = slow->next;
				fast = fast->next;
			}
		}

		// 'slow' es el punto de división
		*frontRef = source;
		*backRef = slow->next;
		slow->next = nullptr;
	}

	// Función para fusionar dos listas ordenadas
	template <typename Compare>
	Nodo<T>* sortedMerge(Nodo<T>* a, Nodo<T>* b, Compare comp) {
		Nodo<T>* result = nullptr;

		// Caso base
		if (a == nullptr)
			return b;
		else if (b == nullptr)
			return a;

		// Elige a o b y recursivamente fusiona el resto
		if (comp(a->data, b->data)) {
			result = a;
			result->next = sortedMerge(a->next, b, comp);
			result->next->prev = result;
			result->prev = nullptr;
		}
		else {
			result = b;
			result->next = sortedMerge(a, b->next, comp);
			result->next->prev = result;
			result->prev = nullptr;
		}
		return result;
	}
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
	void update(T data)
	{
		search(data->getId())->data = data;
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
		system("pause");
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
	/*
	Se usa solo para buscar por valores que se sepan que son unicos
	*/
	template <typename C>
	Nodo<T>* searchByValue(C callback) {
		Nodo<T>* temp = head;
		while (temp != nullptr)
		{
			if (callback(temp->data))
			{
				return temp;
			}
			temp = temp->next;
		}
		return nullptr;
	}

	/*
	Se usa solo para buscar y devolver listas
	validar que head no sea nullptr
	*/
	template <typename C>
	ListaDoble<T>* searchMultipleByValue(C callback) {
		ListaDoble<T>* lista = new ListaDoble<T>();
		Nodo<T>* temp = head;
		while (temp != nullptr)
		{
			if (callback(temp->data))
			{
				lista->push_back(temp->data);
			}
			temp = temp->next;
		}
		return lista;
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

	/*
	* usar asi:
	* auto compareClientes = [](const Cliente& a, const Cliente& b) {
	*	return a.getNombre() < b.getNombre();};
	* lista.mergeSort(&lista.head, compareClientes);
	*/
	template <typename Compare>
	void mergeSort(Nodo<T>** headRef, Compare comp) {
		Nodo<T>* head = *headRef;
		if ((head == nullptr) || (head->next == nullptr)) {
			return;
		}

		Nodo<T>* a;
		Nodo<T>* b;

		// Divide la lista en dos mitades
		frontBackSplit(head, &a, &b);

		// Ordena recursivamente las dos mitades
		mergeSort<Compare>(&a, comp);
		mergeSort<Compare>(&b, comp);

		// Fusiona las dos mitades ordenadas
		*headRef = sortedMerge<Compare>(a, b, comp);
	}

	void printPaginaSoloIda(int cantidadPorPagina) {
		int i = 0;
		Nodo<T>* temp = head;
		bool forward = true; // Para controlar la dirección de la paginación

		while (true) {
			system("cls"); // Limpia la pantalla
			int count = 0;
			Nodo<T>* current = temp;

			// Imprime la página actual
			while (current != nullptr && count < cantidadPorPagina) {
				current->data->print();
				cout << endl;
				current = forward ? current->next : current->prev;
				count++;
			}

			// Espera a que el usuario presione una tecla
			cout << "Pagina #" << i + 1 << endl;
			cout << "Use la flecha derecha para navegar, ESC para salir" << endl;
			int key = _getch();

			// Maneja la entrada del usuario
			if (key == 27) { // ESC
				break;
			}
			else if (key == 224) { // Tecla especial (flechas)
				key = _getch();
				switch (key) {
				case 75: // Flecha izquierda
					/*
					if (i > 0) {
						forward = false;
						for (int j = 0; j < cantidadPorPagina && temp != nullptr; j++) {
							temp = temp->prev;
						}
						i--;
					}
					*/
					break;
				case 77: // Flecha derecha
					if (temp != nullptr && temp->next != nullptr) {
						forward = true;
						for (int j = 0; j < cantidadPorPagina && temp != nullptr; j++) {
							temp = temp->next;
						}
						i++;
					}
					break;
				}
			}

			// Asegura que temp no sea nullptr después de la navegación
			if (temp == nullptr) {
				i = 0;
				temp = forward ? head : tail;
			}
		}
	}
};
