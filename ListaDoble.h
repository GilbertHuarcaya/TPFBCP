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

	//dividir la lista en dos mitades
	void splitList(Nodo<T>* source, Nodo<T>** frontRef, Nodo<T>** backRef) {
		if (source == nullptr || source->next == nullptr) {
			*frontRef = source;
			*backRef = nullptr;
			return;
		}

		Nodo<T>* slow = source;
		Nodo<T>* fast = source->next;

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
		if (*backRef != nullptr) {
			(*backRef)->prev = nullptr;
		}
	}

	//fusionar dos listas ordenadas
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

	// Función para reorganizar el subárbol con raíz en 'i' para que cumpla con las propiedades de un heap
	template <typename Compare>
	void heapify(int heapSize, int i, Compare comp) {
		int largest = i;
		int left = 2 * i + 1;
		int right = 2 * i + 2;

		Nodo<T>* largestNode = getNodoByPos(largest);
		Nodo<T>* leftNode = getNodoByPos(left);
		Nodo<T>* rightNode = getNodoByPos(right);

		if (left < heapSize && comp(leftNode->data, largestNode->data)) {
			largest = left;
			largestNode = leftNode;
		}

		if (right < heapSize && comp(rightNode->data, largestNode->data)) {
			largest = right;
			largestNode = rightNode;
		}

		if (largest != i) {
			swap(getNodoByPos(i)->data, largestNode->data);
			heapify<Compare>(heapSize, largest, comp);
		}
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
		Nodo<T>* nodo = this->search(data->getId());
		if (nodo != nullptr)
		{
			nodo->data = data;
		}
		else {
			cout << "No se encontro el elemento" << endl;
			data->print();
			system("pause");
		}
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
	template<typename Format>
	void printWithFormat(Format formato)
	{
		Nodo<T>* temp = head;
		while (temp != nullptr && temp->data != nullptr)
		{
			formato(temp->data);
			cout << endl;
			temp = temp->next;
		}
		cout << endl;
	}
	template<typename Format>
	string getFormattedByPos(Format formato, int position)
	{
		Nodo<T>* temp = getByPosition(position);
		if (temp != nullptr && temp->data != nullptr)
		{
			return formato(temp->data);
		}
		return "";
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
	Nodo<T>* getNodoByPos(int index) {
		Nodo<T>* current = head;
		for (int i = 0; current != nullptr && i < index; ++i) {
			current = current->next;
		}
		return current;
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
	* lista.mergeSort(compareClientes);
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
		splitList(head, &a, &b);

		// Ordena recursivamente las dos mitades
		mergeSort<Compare>(&a, comp);
		mergeSort<Compare>(&b, comp);

		// Fusiona las dos mitades ordenadas
		*headRef = sortedMerge<Compare>(a, b, comp);
	}

	void printPaginado(int cantidadPorPagina = 5) {
		int i = 0;
		bool forward = true; // Para controlar la dirección de la paginación

		while (true) {
			system("cls"); // Limpia la pantalla
			int count = 0;
			Nodo<T>* current = getNodoByPos(i * cantidadPorPagina);

			// Imprime la página actual
			while (current != nullptr && count < cantidadPorPagina) {
				current->data->print();
				cout << endl;
				current = current->next;
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
					if (i > 0) {
						i--;
					}
					break;
				case 77: // Flecha derecha
					if (getNodoByPos((i + 1) * cantidadPorPagina) != nullptr) {
						i++;
					}
					break;
				}
			}
		}
	}

	/*
	* usar asi:
	* auto compareClientes = [](const Cliente& a, const Cliente& b) {
	* return a.getNombre() < b.getNombre();};
	* lista.heapsort(compareClientes);
	*/
	template <typename Compare>
	void heapsort(Compare comp) {
		int heapSize = size;

		// Construir el heap (reorganizar la lista)
		for (int i = heapSize / 2 - 1; i >= 0; i--) {
			heapify<Compare>(heapSize, i, comp);
		}

		// Extraer elementos del heap uno por uno
		for (int i = heapSize - 1; i > 0; i--) {
			swap(getNodoByPos(0)->data, getNodoByPos(i)->data);
			heapify<Compare>(i, 0, comp);
		}
	}
};
