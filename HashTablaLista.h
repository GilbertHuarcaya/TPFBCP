#pragma once
#include "string"
#include "iostream"
#include "functional"
#include "HashEntidadNodo.h"
#include "Elemento.h"
using namespace std;

template<typename T = Elemento*, typename KEY = int>
class HashTablaLista
{
protected:
	string nombreArchivo;

	void splitList(HashEntidadNodo<T>* source, HashEntidadNodo<T>** frontRef, HashEntidadNodo<T>** backRef);
	template <typename Compare>
	HashEntidadNodo<T>* sortedMerge(HashEntidadNodo<T>* a, HashEntidadNodo<T>* b, Compare comp);
	template <typename Compare>
	void heapify(int heapSize, int i, Compare comp);
	void insertarByPos(T data, int position);
	void insertarByPos(T data, KEY key, int position);

public:
	HashEntidadNodo<T>* head;
	HashEntidadNodo<T>* tail;
	int nextId = 1; // Para identificar el último HashEntidadNodo agregado, el id debe ser inmodificable cuando es diferente a 0
	int size = 0;

	HashTablaLista();
	HashTablaLista(string nombreArchivo);
	~HashTablaLista();
	string getNombreArchivo();
	//insertar
	void push_back(T data, KEY key);
	void push_front(T data, KEY key);

	//sacar y devolver el elemento sacado de la lista
	T pop_back();
	T pop_front();

	//eliminar
	void remove(HashEntidadNodo<T>* hashEntidadNodo);
	void remove(int key);
	void removeByPos(int position);

	//actualizar solo la data
	void update(T data);
	void update(T data, int key);

	//limpiar la lista
	void clear();

	//imprimir la lista
	void print();

	template<typename Format>
	void printWithFormat(Format formato);


	//imprimir la lista paginada
	void printPaginado(int cantidadPorPagina = 5);

	//obtener un elemento de la lista por posición y formatearlo
	template<typename Format>
	string getFormattedByPos(Format formato, int position);

	//buscar y devolver el nodo encontrado
	HashEntidadNodo<T>* search(int id);
	HashEntidadNodo<T>* search(T data);
	template <typename C>
	HashEntidadNodo<T>* searchByValue(C callback);
	template <typename C>
	HashTablaLista<T, KEY>* searchMultipleByValue(C callback);

	//obtener el nodo anterior, siguiente, último y primero
	HashEntidadNodo<T>* getPrevious(HashEntidadNodo<T>* hashEntidadNodo);
	HashEntidadNodo<T>* getByPosition(int position);
	HashEntidadNodo<T>* getNext(HashEntidadNodo<T>* hashEntidadNodo);
	HashEntidadNodo<T>* getLast();
	HashEntidadNodo<T>* getFirst();
	int getSize();
	int getNextId();
	int hash(KEY key);

	//ordenar
	//merge sort
	template <typename Compare>
	void mergeSort(HashEntidadNodo<T>** headRef, Compare comp);

	//heapsort
	template <typename Compare>
	void heapsort(Compare comp);

	//quick sort
	void QuickSort(function<bool(T, T)> comp, int inicio, int fin);
	int particion_QS(function<bool(T, T)> comp, int inicio, int fin);
};

template<typename T, typename KEY>
inline void HashTablaLista<T, KEY>::splitList(HashEntidadNodo<T>* source, HashEntidadNodo<T>** frontRef, HashEntidadNodo<T>** backRef)
{
	HashEntidadNodo<T>* fast;
	HashEntidadNodo<T>* slow;
	slow = source;
	fast = source->next;

	// Avanza 'fast' dos HashEntidadNodos, y 'slow' uno
	while (fast != nullptr)
	{
		fast = fast->next;
		if (fast != nullptr)
		{
			slow = slow->next;
			fast = fast->next;
		}
	}

	// 'slow' es el HashEntidadNodo intermedio, así que lo usamos para dividir la lista en dos
	*frontRef = source;
	*backRef = slow->next;
	slow->next = nullptr;
	if (*backRef != nullptr) {
		(*backRef)->prev = nullptr;
	}
}

template<typename T, typename KEY>
template<typename Compare>
inline HashEntidadNodo<T>* HashTablaLista<T, KEY>::sortedMerge(HashEntidadNodo<T>* a, HashEntidadNodo<T>* b, Compare comp)
{
	HashEntidadNodo<T>* result = nullptr;

	// Caso base
	if (a == nullptr)
		return b;
	else if (b == nullptr)
		return a;

	// Selecciona 'a' o 'b' y recursivamente llama a sortedMerge
	if (comp(a->data, b->data))
	{
		result = a;
		result->next = sortedMerge(a->next, b, comp);
	}
	else
	{
		result = b;
		result->next = sortedMerge(a, b->next, comp);
	}
	return result;
}


template<typename T, typename KEY>
template<typename Compare>
inline void HashTablaLista<T, KEY>::heapify(int heapSize, int i, Compare comp)
{
	int largest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;

	// Si el hijo izquierdo es más grande que la raíz
	if (left < heapSize && comp(getByPosition(left)->data, getByPosition(largest)->data))
		largest = left;

	// Si el hijo derecho es más grande que el más grande hasta ahora
	if (right < heapSize && comp(getByPosition(right)->data, getByPosition(largest)->data))
		largest = right;

	// Si el más grande no es la raíz
	if (largest != i) {
		swap(getByPosition(i)->data, getByPosition(largest)->data);
		heapify(heapSize, largest, comp);
	}
}

template<typename T, typename KEY>
HashTablaLista<T, KEY>::HashTablaLista()
{
	nombreArchivo = "";
	head = nullptr;
	tail = nullptr;
}

template<typename T, typename KEY>
HashTablaLista<T, KEY>::HashTablaLista(string nombreArchivo)
{
	this->nombreArchivo = nombreArchivo;
	head = nullptr;
	tail = nullptr;
}

template<typename T, typename KEY>
HashTablaLista<T, KEY>::~HashTablaLista()
{
	clear();
}

template<typename T, typename KEY>
string HashTablaLista<T, KEY>::getNombreArchivo()
{
	return nombreArchivo;
}

template<typename T, typename KEY>
inline void HashTablaLista<T, KEY>::push_back(T data, KEY key)
{
	HashEntidadNodo<T, KEY>* hashEntidadNodo = new HashEntidadNodo<T, KEY>(data, key);
	if (head == nullptr)
	{
		head = hashEntidadNodo;
		tail = hashEntidadNodo;
	}
	else
	{
		tail->next = hashEntidadNodo;
		hashEntidadNodo->prev = tail;
		tail = hashEntidadNodo;
	}

	if (hashEntidadNodo->data->getId() > nextId)
	{
		nextId = hashEntidadNodo->data->getId() + 1;
	}
	else {
		nextId++;
	}
	size++;
}

template<typename T, typename KEY>
inline void HashTablaLista<T, KEY>::push_front(T data, KEY key)
{
	HashEntidadNodo<T, KEY>* hashEntidadNodo = new HashEntidadNodo<T, KEY>(data, key);
	if (head == nullptr)
	{
		head = hashEntidadNodo;
		tail = hashEntidadNodo;

	}
	else
	{
		hashEntidadNodo->next = head;
		head->prev = hashEntidadNodo;
		head = hashEntidadNodo;
	}

	if (hashEntidadNodo->data->getId() > nextId)
	{
		nextId = hashEntidadNodo->data->getId() + 1;
	}
	size++;
}

template<typename T, typename KEY>
inline void HashTablaLista<T, KEY>::insertarByPos(T data, int position)
{
	HashEntidadNodo<T>* hashEntidadNodo = new HashEntidadNodo<T>(data);
	if (head == nullptr)
	{
		head = hashEntidadNodo;
		tail = hashEntidadNodo;
	}
	else
	{
		if (position == 0)
		{
			push_front(data);
			return;
		}
		HashEntidadNodo<T>* temp = head;
		for (int i = 0; i < position - 1; i++)
		{
			temp = temp->next;
		}
		hashEntidadNodo->next = temp->next;
		hashEntidadNodo->prev = temp;
		temp->next = hashEntidadNodo;
		hashEntidadNodo->next->prev = hashEntidadNodo;
	}
	if (hashEntidadNodo->data->getId() > nextId)
	{
		nextId = hashEntidadNodo->data->getId() + 1;
	}
	size++;
}

template<typename T, typename KEY>
inline void HashTablaLista<T, KEY>::insertarByPos(T data, KEY key, int position)
{
	HashEntidadNodo<T, KEY>* hashEntidadNodo = new HashEntidadNodo<T, KEY>(data, key);
	if (head == nullptr)
	{
		head = hashEntidadNodo;
		tail = hashEntidadNodo;
	}
	else
	{
		if (position == 0)
		{
			push_front(data, key);
			return;
		}
		HashEntidadNodo<T>* temp = head;
		for (int i = 0; i < position - 1; i++)
		{
			temp = temp->next;
		}
		hashEntidadNodo->next = temp->next;
		hashEntidadNodo->prev = temp;
		temp->next = hashEntidadNodo;
		hashEntidadNodo->next->prev = hashEntidadNodo;
	}
	if (hashEntidadNodo->data->getId() > nextId)
	{
		nextId = hashEntidadNodo->data->getId() + 1;
	}
	size++;
}

template<typename T, typename KEY>
T HashTablaLista<T, KEY>::pop_back()
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
		HashEntidadNodo<T>* temp = tail;
		T data = temp->data;
		tail = tail->prev;
		tail->next = nullptr;
		delete temp;
		return data;
	}
	size--;
}

template<typename T, typename KEY>
T HashTablaLista<T, KEY>::pop_front()
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
		HashEntidadNodo<T>* temp = head;
		T data = temp->data;
		head = head->next;
		head->prev = nullptr;
		delete temp;
		return data;
	}
}

template<typename T, typename KEY>
void HashTablaLista<T, KEY>::remove(HashEntidadNodo<T>* hashEntidadNodo)
{
	if (hashEntidadNodo == nullptr)
	{
		return;
	}
	if (hashEntidadNodo == head)
	{
		pop_front();
	}
	else if (hashEntidadNodo == tail)
	{
		pop_back();
	}
	else
	{
		hashEntidadNodo->prev->next = hashEntidadNodo->next;
		hashEntidadNodo->next->prev = hashEntidadNodo->prev;
		delete hashEntidadNodo;
	}
	size--;
}

template<typename T, typename KEY>
inline void HashTablaLista<T, KEY>::remove(int key)
{
	HashEntidadNodo<T>* hashEntidadNodo = search(key);
	if (hashEntidadNodo != nullptr)
	{
		remove(hashEntidadNodo);
	}
	else {
		cout << "No se encontro el elemento" << endl;
		system("pause");
	}
}

template<typename T, typename KEY>
inline void HashTablaLista<T, KEY>::removeByPos(int position)
{
	HashEntidadNodo<T>* hashEntidadNodo = getByPosition(position);
	if (hashEntidadNodo != nullptr)
	{
		remove(hashEntidadNodo);
	}
	else {
		cout << "No se encontro el elemento" << endl;
		system("pause");
	}
}

template<typename T, typename KEY>
void HashTablaLista<T, KEY>::update(T data)
{
	HashEntidadNodo<T>* hashEntidadNodo = this->search(data->getId());
	if (hashEntidadNodo != nullptr)
	{
		hashEntidadNodo->data = data;
	}
	else {
		cout << "No se encontro el elemento" << endl;
		data->print();
		system("pause");
	}
}

template<typename T, typename KEY>
inline void HashTablaLista<T, KEY>::update(T data, int key)
{
	HashEntidadNodo<T>* hashEntidadNodo = this->search(key);
	if (hashEntidadNodo != nullptr)
	{
		hashEntidadNodo->data = data;
	}
	else {
		cout << "No se encontro el elemento" << endl;
		data->print();
		system("pause");
	}
}

template<typename T, typename KEY>
void HashTablaLista<T, KEY>::clear()
{
	while (head != nullptr)
	{
		pop_front();
	}
}

template<typename T, typename KEY>
void HashTablaLista<T, KEY>::print()
{
	HashEntidadNodo<T>* temp = head;
	while (temp != nullptr && temp->data != nullptr)
	{
		temp->data->print();
		cout << endl;
		cout << "--------------------------------------------------------------------";
		cout << endl;
		temp = temp->next;
	}
	cout << endl;
}

template<typename T, typename KEY>
template<typename Format>
void HashTablaLista<T, KEY>::printWithFormat(Format formato)
{
	HashEntidadNodo<T>* temp = head;
	while (temp != nullptr && temp->data != nullptr)
	{
		formato(temp->data);
		cout << endl;
		temp = temp->next;
	}
	cout << endl;
}


template<typename T, typename KEY>
template<typename Format>
inline string HashTablaLista<T, KEY>::getFormattedByPos(Format formato, int position)
{
	HashEntidadNodo<T>* temp = getByPosition(position);
	if (temp != nullptr && temp->data != nullptr)
	{
		return formato(temp->data);
	}
	return "";
}
/*
* buscar por key
*/
template<typename T, typename KEY>
HashEntidadNodo<T>* HashTablaLista<T, KEY>::search(int key)
{
	HashEntidadNodo<T>* temp = head;
	while (temp != nullptr)
	{
		if (temp->key == key)
		{
			return temp;
		}
		temp = temp->next;
	}
	return nullptr;
}

/*
* buscar por data
*/
template<typename T, typename KEY>
HashEntidadNodo<T>* HashTablaLista<T, KEY>::search(T data)
{
	HashEntidadNodo<T>* temp = head;
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

template<typename T, typename KEY>
template <typename C>
HashEntidadNodo<T>* HashTablaLista<T, KEY>::searchByValue(C callback)
{
	HashEntidadNodo<T>* temp = head;
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

template<typename T, typename KEY>
template <typename C>
HashTablaLista<T, KEY>* HashTablaLista<T, KEY>::searchMultipleByValue(C callback)
{
	HashTablaLista<T, KEY>* lista = new HashTablaLista<T, KEY>();
	HashEntidadNodo<T>* temp = head;
	while (temp != nullptr)
	{
		if (callback(temp->data))
		{
			lista->push_back(temp->data, temp->key);
		}
		temp = temp->next;
	}
	return lista;
}

template<typename T, typename KEY>
HashEntidadNodo<T>* HashTablaLista<T, KEY>::getPrevious(HashEntidadNodo<T>* hashEntidadNodo)
{
	return hashEntidadNodo->prev;
}

template<typename T, typename KEY>
HashEntidadNodo<T>* HashTablaLista<T, KEY>::getByPosition(int position)
{
	HashEntidadNodo<T>* temp = head;
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

template<typename T, typename KEY>
HashEntidadNodo<T>* HashTablaLista<T, KEY>::getNext(HashEntidadNodo<T>* hashEntidadNodo)
{
	return hashEntidadNodo->next;
}

template<typename T, typename KEY>
HashEntidadNodo<T>* HashTablaLista<T, KEY>::getLast()
{
	return tail;
}

template<typename T, typename KEY>
HashEntidadNodo<T>* HashTablaLista<T, KEY>::getFirst()
{
	return head;
}

template<typename T, typename KEY>
int HashTablaLista<T, KEY>::getSize()
{
	return size;
}

template<typename T, typename KEY>
int HashTablaLista<T, KEY>::getNextId()
{
	return nextId;
}

template<typename T, typename KEY>
inline int HashTablaLista<T, KEY>::hash(KEY key)
{
	return nextId + (key % size);
}

template<typename T, typename KEY>
template <typename Compare>
void HashTablaLista<T, KEY>::mergeSort(HashEntidadNodo<T>** headRef, Compare comp)
{
	HashEntidadNodo<T>* head = *headRef;
	if ((head == nullptr) || (head->next == nullptr)) {
		return;
	}

	HashEntidadNodo<T>* a;
	HashEntidadNodo<T>* b;

	// Divide la lista en dos mitades
	splitList(head, &a, &b);

	// Ordena recursivamente las dos mitades
	mergeSort<Compare>(&a, comp);
	mergeSort<Compare>(&b, comp);

	// Fusiona las dos mitades ordenadas
	*headRef = sortedMerge<Compare>(a, b, comp);
}

template<typename T, typename KEY>
void HashTablaLista<T, KEY>::printPaginado(int cantidadPorPagina)
{
	int i = 0;
	bool forward = true; // Para controlar la dirección de la paginación

	while (true) {
		system("cls"); // Limpia la pantalla
		int count = 0;
		HashEntidadNodo<T>* current = getByPosition(i * cantidadPorPagina);

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
				if (getByPosition((i + 1) * cantidadPorPagina) != nullptr) {
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
template<typename T, typename KEY>
template<typename Compare>
inline void HashTablaLista<T, KEY>::heapsort(Compare comp)
{
	int heapSize = size;
	// Construye el heap
	for (int i = heapSize / 2; i >= 0; i--) {
		heapify(heapSize, i, comp);
	}

	// Extrae elementos del heap uno por uno
	for (int i = heapSize - 1; i >= 0; i--) {
		swap(getByPosition(0)->data, getByPosition(i)->data);
		heapSize--;
		heapify(heapSize, 0, comp);
	}
}

template<typename T, typename KEY>
void HashTablaLista<T, KEY>::QuickSort(function<bool(T, T)> comp, int inicio, int fin)
{
	if (inicio < fin)
	{
		int pivote = particion_QS(comp, inicio, fin);
		QuickSort(comp, inicio, pivote - 1);
		QuickSort(comp, pivote + 1, fin);
	}
}

template<typename T, typename KEY>
int HashTablaLista<T, KEY>::particion_QS(function<bool(T, T)> comp, int inicio, int fin)
{
	T pivote = getByPosition(fin)->data;
	int i = inicio - 1;
	for (int j = inicio; j < fin; j++)
	{
		if (comp(getByPosition(j)->data, pivote))
		{
			i++;
			swap(getByPosition(i)->data, getByPosition(j)->data);
		}
	}
	swap(getByPosition(i + 1)->data, getByPosition(fin)->data);
	return i + 1;
}