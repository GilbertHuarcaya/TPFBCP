#pragma once
#include "string"
#include "iostream"
#include "Nodo.h"
#include "functional"
#include "fstream"
#include "sstream"
using namespace std;

template <class T>
class ListaDoble
{
protected:
	string nombreArchivo;

	void splitList(Nodo<T>* source, Nodo<T>** frontRef, Nodo<T>** backRef);
	template <typename Compare>
	Nodo<T>* sortedMerge(Nodo<T>* a, Nodo<T>* b, Compare comp);
	template <typename Compare>
	void heapify(int heapSize, int i, Compare comp);

public:
	Nodo<T>* head;
	Nodo<T>* tail;
	int nextId = 1; // Para identificar el último nodo agregado
	int size = 0;

	ListaDoble();
	ListaDoble(string nombreArchivo);
	~ListaDoble();
	string getNombreArchivo();
	void push_back(T data, int id);
	void push_front(T data, int id);
	T pop_back();
	T pop_front();
	void removeElement(Nodo<T>* nodo);
	void removeElement(T data);
	void updateElement(Nodo<T>* nodo);
	void updateElement(T data);
	void clear();
	void print();
	template<typename Format>
	void printWithFormat(Format formato);
	void printPaginado(int cantidadPorPagina = 5);
	template<typename Format>
	string getFormattedByPos(Format formato, int position);
	Nodo<T>* search(int id);
	Nodo<T>* search(T data);
	template <typename C>
	Nodo<T>* searchByValue(C callback);
	Nodo<T>* getNodoByPos(int index);
	template <typename C>
	ListaDoble<T>* searchMultipleByValue(C callback);
	Nodo<T>* getPrevious(Nodo<T>* nodo);
	Nodo<T>* getByPosition(int position);
	Nodo<T>* getNext(Nodo<T>* nodo);
	Nodo<T>* getLast();
	Nodo<T>* getFirst();
	int getSize();
	int getNextId();

	//Ordenamientos
	template <typename Compare>
	void mergeSort(Nodo<T>** headRef, Compare comp);

	template<typename Compare>
	void heapsort(Compare comp);

	void QuickSort(function<bool(T, T)> comp, int inicio, int fin);
	int particion_QS(function<bool(T, T)> comp, int inicio, int fin);
	
	//data set
	void escribir();
	void leer();
	void recargar();
	void agregar(T item);
	void editar(T item);
	void eliminar(T item);
};

template<class T>
inline void ListaDoble<T>::splitList(Nodo<T>* source, Nodo<T>** frontRef, Nodo<T>** backRef)
{
	Nodo<T>* fast;
	Nodo<T>* slow;
	slow = source;
	fast = source->next;

	// Avanza 'fast' dos nodos, y 'slow' uno
	while (fast != nullptr)
	{
		fast = fast->next;
		if (fast != nullptr)
		{
			slow = slow->next;
			fast = fast->next;
		}
	}

	// 'slow' es el nodo intermedio, así que lo usamos para dividir la lista en dos
	*frontRef = source;
	*backRef = slow->next;
	slow->next = nullptr;
	if (*backRef != nullptr) {
		(*backRef)->prev = nullptr;
	}
}

template<class T>
template<typename Compare>
inline Nodo<T>* ListaDoble<T>::sortedMerge(Nodo<T>* a, Nodo<T>* b, Compare comp)
{
	Nodo<T>* result = nullptr;

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


template<class T>
template<typename Compare>
inline void ListaDoble<T>::heapify(int heapSize, int i, Compare comp)
{
	int largest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;

	// Si el hijo izquierdo es más grande que la raíz
	if (left < heapSize && comp(getNodoByPos(left)->data, getNodoByPos(largest)->data))
		largest = left;

	// Si el hijo derecho es más grande que el más grande hasta ahora
	if (right < heapSize && comp(getNodoByPos(right)->data, getNodoByPos(largest)->data))
		largest = right;

	// Si el más grande no es la raíz
	if (largest != i) {
		swap(getNodoByPos(i)->data, getNodoByPos(largest)->data);
		heapify(heapSize, largest, comp);
	}
}

template <class T>
ListaDoble<T>::ListaDoble()
{
	nombreArchivo = "";
	head = nullptr;
	tail = nullptr;
}

template <class T>
ListaDoble<T>::ListaDoble(string nombreArchivo)
{
	this->nombreArchivo = nombreArchivo;
	head = nullptr;
	tail = nullptr;
}

template <class T>
ListaDoble<T>::~ListaDoble()
{
	clear();
}

template <class T>
string ListaDoble<T>::getNombreArchivo()
{
	return nombreArchivo;
}

template <class T>
void ListaDoble<T>::push_back(T data, int id)
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

	if (id > nextId)
	{
		nextId = id + 1;
	}
	else {
		nextId++;
	}
	size++;
}

template <class T>
void ListaDoble<T>::push_front(T data, int id)
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

	if (id > nextId)
	{
		nextId = id + 1;
	}
	else {
		nextId++;
	}
	size++;
}

template <class T>
T ListaDoble<T>::pop_back()
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

template <class T>
T ListaDoble<T>::pop_front()
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

template <class T>
void ListaDoble<T>::removeElement(Nodo<T>* nodo)
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

template<class T>
inline void ListaDoble<T>::removeElement(T data)
{
	Nodo<T>* nodo = search(data);
	if (nodo != nullptr)
	{
		removeElement(nodo);
	}
	else {
		cout << "No se encontro el elemento" << endl;
		data->print();
		system("pause");
	}
}

template<class T>
inline void ListaDoble<T>::updateElement(Nodo<T>* nodo)
{
	Nodo<T>* temp = head;
	while (temp != nullptr)
	{
		if (temp == nodo)
		{
			temp->data = nodo->data;
			return;
		}
		temp = temp->next;
	}
	cout << "No se encontro el elemento" << endl;
	nodo->data->print();
	system("pause");
}

template <class T>
void ListaDoble<T>::updateElement(T data)
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

template <class T>
void ListaDoble<T>::clear()
{
	while (head != nullptr)
	{
		pop_front();
	}
}

template <class T>
void ListaDoble<T>::print()
{
	Nodo<T>* temp = head;
	while (temp != nullptr && temp->data != nullptr)
	{
		temp->data->print();
		cout << "------------------------------------------------------------------" << endl;
		cout << endl;
		temp = temp->next;
	}
	cout << endl;
}

template <class T>
template<typename Format>
void ListaDoble<T>::printWithFormat(Format formato)
{
	Nodo<T>* temp = head;
	while (temp != nullptr && temp->data != nullptr)
	{
		formato(temp->data);
		cout << "------------------------------------------------------------------" << endl;
		cout << endl;
		temp = temp->next;
	}
	cout << endl;
}

template <class T>
template<typename Format>
string ListaDoble<T>::getFormattedByPos(Format formato, int position)
{
	Nodo<T>* temp = getByPosition(position);
	if (temp != nullptr && temp->data != nullptr)
	{
		return formato(temp->data);
	}
	return "";
}

template <class T>
Nodo<T>* ListaDoble<T>::search(int id)
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

template <class T>
Nodo<T>* ListaDoble<T>::search(T data)
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

template <class T>
template <typename C>
Nodo<T>* ListaDoble<T>::searchByValue(C callback)
{
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

template <class T>
Nodo<T>* ListaDoble<T>::getNodoByPos(int index)
{
	Nodo<T>* current = head;
	for (int i = 0; current != nullptr && i < index; ++i) {
		current = current->next;
	}
	return current;
}

template <class T>
template <typename C>
ListaDoble<T>* ListaDoble<T>::searchMultipleByValue(C callback)
{
	ListaDoble<T>* lista = new ListaDoble<T>();
	Nodo<T>* temp = head;
	while (temp != nullptr)
	{
		if (callback(temp->data))
		{
			lista->push_back(temp->data, temp->data->getId());
		}
		temp = temp->next;
	}
	return lista;
}

template <class T>
Nodo<T>* ListaDoble<T>::getPrevious(Nodo<T>* nodo)
{
	return nodo->prev;
}

template <class T>
Nodo<T>* ListaDoble<T>::getByPosition(int position)
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

template <class T>
Nodo<T>* ListaDoble<T>::getNext(Nodo<T>* nodo)
{
	return nodo->next;
}

template <class T>
Nodo<T>* ListaDoble<T>::getLast()
{
	return tail;
}

template <class T>
Nodo<T>* ListaDoble<T>::getFirst()
{
	return head;
}

template <class T>
int ListaDoble<T>::getSize()
{
	return size;
}

template <class T>
int ListaDoble<T>::getNextId()
{
	return nextId;
}

template <class T>
template <typename Compare>
void ListaDoble<T>::mergeSort(Nodo<T>** headRef, Compare comp)
{
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

template <class T>
void ListaDoble<T>::printPaginado(int cantidadPorPagina)
{
	int i = 0;
	bool forward = true; // Para controlar la dirección de la paginación

	while (true) {
		system("cls"); // Limpia la pantalla
		int count = 0;
		Nodo<T>* current = getNodoByPos(i * cantidadPorPagina);

		// Imprime la página actual
		while (current != nullptr && count < cantidadPorPagina) {
			current->data->print();
			cout << "------------------------------------------------------------------"<< endl;
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
template<class T>
template<typename Compare>
inline void ListaDoble<T>::heapsort(Compare comp)
{
	int heapSize = size;
	// Construye el heap
	for (int i = heapSize / 2; i >= 0; i--) {
		heapify(heapSize, i, comp);
	}

	// Extrae elementos del heap uno por uno
	for (int i = heapSize - 1; i >= 0; i--) {
		swap(getNodoByPos(0)->data, getNodoByPos(i)->data);
		heapSize--;
		heapify(heapSize, 0, comp);
	}
}


template<class T>
inline void ListaDoble<T>::QuickSort(function<bool(T, T)> comp, int inicio, int fin)
{
	if (inicio < fin)
	{
		int pivote = particion_QS(comp, inicio, fin);
		QuickSort(comp, inicio, pivote - 1);
		QuickSort(comp, pivote + 1, fin);
	}
}

template<class T>
int ListaDoble<T>::particion_QS(function<bool(T, T)> comp, int inicio, int fin)
{
	Nodo<T>* pivote = getByPosition(fin - 1);
	int i = inicio - 1;
	for (int j = inicio; j < fin; j++)
	{
		if (comp(getByPosition(j)->data, pivote->data))
		{
			i++;
			T aux = getByPosition(j)->data;
			getByPosition(j)->data = getByPosition(i)->data;
			getByPosition(i)->data = aux;
		}
	}
	T aux = getByPosition(fin)->data;
	getByPosition(fin)->data = getByPosition(i + 1)->data;
	getByPosition(i + 1)->data = aux;
	return i + 1;
}

/*
* reemplaza por completo todo el archivo csv
*/
template<class T>
inline void ListaDoble<T>::escribir() {
	ofstream file(nombreArchivo);
	Nodo<T>* temp = this->head;
	bool header = true;
	while (temp != nullptr) {
		if (header) {
			file << temp->data->escribirCabecera() << "\n";
			file << temp->data->escribirLinea();
			if (temp->next != nullptr) {
				file << "\n";
			}
			header = false;
		}
		else {
			file << temp->data->escribirLinea();
			if (temp->next != nullptr) {
				file << "\n";
			}
		}
		temp = temp->next;
	}
	file.close();
}

template<class T>
inline void ListaDoble<T>::leer() {
	ifstream file(nombreArchivo);
	string line;
	bool header = true;
	while (getline(file, line)) {
		stringstream ss(line);
		string idStr;
		getline(ss, idStr, ',');

		if (header && idStr == "id") {
			header = false;
		}
		else {
			T data = new remove_pointer<T>::type();
			data->leerLinea(line);
			if (data->getId() != 0) {
				push_back(data, data->getId());
			}
		}
	}
	file.close();
}

template<class T>
inline void ListaDoble<T>::recargar() {
	ifstream file(nombreArchivo);
	string line;
	bool header = true;
    while (getline(file, line)) {
		stringstream ss(line);
		string idStr;
		getline(ss, idStr, ',');

		if (header && idStr == "id") {
			header = false;
		}
		else {
			T data = new remove_pointer<T>::type();
			data->leerLinea(line);
			if (data->getId() != 0) {
				if (search(data->getId()) != nullptr) {
					updateElement(data);
				}
				else {
					push_back(data, data->getId());
				}
			}
		}
    }
}

template<class T>
inline void ListaDoble<T>::agregar(T item) {
	//leer primero para ver si hay cabecera
	ifstream file(nombreArchivo);
	string line;
	bool header = false;

	//verificar si hay cabecera
	getline(file, line);
	stringstream ss(line);
	string idStr;
	getline(ss, idStr, ',');
	if (idStr == "id") {
		header = false;
	}
	file.close();

	//agregar item
	ofstream file(path);
	if (!header) {
		file << item->escribirCabecera() << "\n";
	}
	file << item->escribirLinea() << "\n";
	file.close();
}

template<class T>
inline void ListaDoble<T>::editar(T item) {
	ifstream file(nombreArchivo);
	ofstream temp("temp.txt");
	string line;
	bool header = true;
	while (getline(file, line)) {
		stringstream ss(line);
		string idStr;
		getline(ss, idStr, ',');

		if (header && idStr == "id") {
			header = false;
		}
		else {
			T data = new remove_pointer<T>::type();
			data->leerLinea(line);
			if (data->getId() == item->getId()) {
				temp << item->escribirLinea() << "\n";
			}
			else {
				temp << line << "\n";
			}
		}
	}
	file.close();
	temp.close();
	remove(nombreArchivo.c_str());
	rename("temp.txt", nombreArchivo.c_str());
}

template<class T>
inline void ListaDoble<T>::eliminar(T item) {
	ifstream file(nombreArchivo);
	ofstream temp("temp.txt");
	string line;
	bool header = true;
    while (getline(file, line)) {
		stringstream ss(line);
		string idStr;
		getline(ss, idStr, ',');

		if (header && idStr == "id") {
			header = false;
		}
		else {
			T data = new remove_pointer<T>::type();
			data->leerLinea(line);
			if (data->getId() != item->getId()) {
				temp << line << "\n";
			}
		}
    }
	file.close();
	temp.close();
	remove(nombreArchivo.c_str());
	rename("temp.txt", nombreArchivo.c_str());
}

