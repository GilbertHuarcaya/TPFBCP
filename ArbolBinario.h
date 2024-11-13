#pragma once
#include "NodoArbol.h"
#include "functional"
#include "string"

template <class T>
class ArbolBinario {
private:
	string nombreArchivo;
	NodoArbol<T>* raiz;
	function<bool(T, T)> requisito_division;
	int total;
	int nextId = 1;
	int _altura(NodoArbol<T>* nodo);
	void _insertar(T data, NodoArbol<T>*& nodo);
	void _PreOrden(function<void(T)> funcion, NodoArbol<T>* nodo);
	void _EnOrden(function<void(T)> funcion, NodoArbol<T>* nodo);
	void _PostOrden(function<void(T)> funcion, NodoArbol<T>* nodo);
	NodoArbol<T>* _buscar(int id, NodoArbol<T>* aux);
public:
	ArbolBinario(string nombreArchivo = "");
	ArbolBinario(function<bool(T, T)> funcion, string nombreArchivo = "");
	ArbolBinario(T data, function<bool(T, T)> funcion, string nombreArchivo = "");
	NodoArbol<T>* getRaiz();
	int altura();
	//insertar
	void insertar(T data);

	//eliminar
	void removeElement(NodoArbol<T>* nodo);
	void removeElement(T data);
	void removeElement(int id);

	//actualizar solo la data
	void updateElement(T data);
	void updateElement(T data, int id);

	//buscar
	NodoArbol<T>* search(int id);
	NodoArbol<T>* search(T adata);
	template <typename C>
	ArbolBinario<T>* searchMultipleByValue(C callback);

	void PreOrden(function<void(T)> funcion);
	void EnOrden(function<void(T)> funcion);
	void PostOrden(function<void(T)> funcion);


	template <typename C>
	void printTree(NodoArbol<T>* nodo, int espacio, C callback, string prefijo = "", bool esIzquierda = true);
	int getSize();

	template <typename C>
	void ordenar(C callback);


	int getNextId();
	void escribir();
	void leer();
	void recargar();
	void agregar(T item);
	void editar(T item);
	void eliminar(T item);
};

template <class T>
int ArbolBinario<T>::_altura(NodoArbol<T>* nodo)
{
	int ai, ad;
	if (nodo != nullptr)
	{
		ai = 1 + _altura(nodo->izquierda);
		ad = 1 + _altura(nodo->derecha);
		return ai > ad ? ai : ad;
	}
	else return 0;
}

template <class T>
void ArbolBinario<T>::_insertar(T data, NodoArbol<T>*& nodo)
{
	if (nodo != nullptr)
	{
		NodoArbol<T>* aux;
		if (requisito_division(data, nodo->data))
		{
			return _insertar(data, nodo->izquierda);
		}
		else
		{
			return _insertar(data, nodo->derecha);
		}
	}
	else
	{
		nodo = new NodoArbol<T>(data);
		total++;
		nextId++;
	}
}

template <class T>
void ArbolBinario<T>::_PreOrden(function<void(T)> funcion, NodoArbol<T>* nodo)
{
	if (nodo == nullptr)return;
	if (nodo->data == nullptr)return;
	funcion(nodo->data);
	_PreOrden(funcion, nodo->izquierda);
	_PreOrden(funcion, nodo->derecha);
}

template <class T>
void ArbolBinario<T>::_EnOrden(function<void(T)> funcion, NodoArbol<T>* nodo)
{
	if (nodo == nullptr)return;
	_EnOrden(funcion, nodo->izquierda);
	funcion(nodo->data);
	_EnOrden(funcion, nodo->derecha);
}

template <class T>
void ArbolBinario<T>::_PostOrden(function<void(T)> funcion, NodoArbol<T>* nodo)
{
	if (nodo == nullptr)return;
	_PostOrden(funcion, nodo->izquierda);
	_PostOrden(funcion, nodo->derecha);
	funcion(nodo->data);
}

template<class T>
inline NodoArbol<T>* ArbolBinario<T>::_buscar(int id, NodoArbol<T>* aux)
{
	if (aux != nullptr) {
		if (aux->data->getId() != id)
		{
			if (_buscar(id, aux->izquierda) != nullptr)
				return _buscar(id, aux->izquierda);
			if (_buscar(id, aux->derecha) != nullptr)
				return _buscar(id, aux->derecha);
			return nullptr;
		}
		else return aux;
	}
	else return aux;
}

template <class T>
ArbolBinario<T>::ArbolBinario(string nombreArchivo)
{
	this->nombreArchivo = nombreArchivo;
	raiz = nullptr;
	requisito_division = [](T a, T b) -> bool {
		return a->getId() < b->getId();
		};
	total = 0;
}

template <class T>
ArbolBinario<T>::ArbolBinario(function<bool(T, T)> funcion, string nombreArchivo = "")
{
	this->nombreArchivo = nombreArchivo;
	raiz = nullptr;
	requisito_division = funcion;
	total = 0;
}

template <class T>
ArbolBinario<T>::ArbolBinario(T data, function<bool(T, T)> funcion, string nombreArchivo = "")
{
	this->nombreArchivo = nombreArchivo;
	raiz = new NodoArbol<T>(data);
	requisito_division = funcion;
	total = 1;
}

template <class T>
NodoArbol<T>* ArbolBinario<T>::getRaiz() { return raiz; }

template <class T>
int ArbolBinario<T>::altura()
{
	return _altura(raiz);
}

template <class T>
void ArbolBinario<T>::insertar(T data)
{
	return _insertar(data, raiz);
}

template<class T>
inline void ArbolBinario<T>::removeElement(NodoArbol<T>* nodo)
{
	removeElement(nodo->data->getId());
}

template<class T>
inline void ArbolBinario<T>::removeElement(T data)
{
	removeElement(data->getId());
}

template<class T>
inline void ArbolBinario<T>::removeElement(int id)
{
	NodoArbol<T>* nodo = buscarNodoPorId(id, raiz);
	if (nodo != nullptr) {
		if (nodo->izquierda == nullptr && nodo->derecha == nullptr) {
			delete nodo;
		}
		else if (nodo->izquierda == nullptr) {
			NodoArbol<T>* aux = nodo->derecha;
			nodo->data = aux->data;
			nodo->derecha = aux->derecha;
			nodo->izquierda = aux->izquierda;
			delete aux;
		}
		else if (nodo->derecha == nullptr) {
			NodoArbol<T>* aux = nodo->izquierda;
			nodo->data = aux->data;
			nodo->derecha = aux->derecha;
			nodo->izquierda = aux->izquierda;
			delete aux;
		}
		else {
			NodoArbol<T>* aux = nodo->derecha;
			while (aux->izquierda != nullptr) {
				aux = aux->izquierda;
			}
			nodo->data = aux->data;
			remover(aux->data->getId());
		}
	}
}

template<class T>
inline void ArbolBinario<T>::updateElement(T data)
{
	updateElement(data, data->getId());
}

template<class T>
inline void ArbolBinario<T>::updateElement(T data, int id)
{
	NodoArbol<T>* nodo = search(id);
	if (nodo != nullptr) {
		nodo->data = data;
	}
	else {
		insertar(data);
	}
}

template <class T>
void ArbolBinario<T>::PreOrden(function<void(T)> funcion)
{
	return _PreOrden(funcion, raiz);
}

template <class T>
void ArbolBinario<T>::EnOrden(function<void(T)> funcion)
{
	return _EnOrden(funcion, raiz);
}

template <class T>
void ArbolBinario<T>::PostOrden(function<void(T)> funcion)
{
	return _PostOrden(funcion, raiz);
}

template<class T>
inline NodoArbol<T>* ArbolBinario<T>::search(int id)
{
	return _buscar(id, raiz);
}

template<class T>
inline NodoArbol<T>* ArbolBinario<T>::search(T adata)
{
	return _buscar(adata->getId(), raiz);
}

template <class T>
template <typename C>
void ArbolBinario<T>::printTree(NodoArbol<T>* nodo, int espacio, C callback, string prefijo, bool esIzquierda)
{
	if (nodo == nullptr)
	{
		return;
	}

	// Imprimir el nodo actual
	cout << prefijo;
	if (prefijo.empty()) {
		cout << "|-R-";
	}
	else {
		cout << (esIzquierda ? "|-I-" : "|-D-");
	}
	callback(nodo->data);
	cout << endl;

	// Procesar el subárbol izquierdo y derecho
	printTree(nodo->izquierda, espacio, callback, prefijo + (esIzquierda ? "| " : "  "), true);
	printTree(nodo->derecha, espacio, callback, prefijo + (esIzquierda ? "| " : "  "), false);
}

template<class T>
inline int ArbolBinario<T>::getSize()
{
	return total;
}

template<class T>
inline int ArbolBinario<T>::getNextId()
{
	return nextId;
}

template <class T>
void ArbolBinario<T>::escribir() {
	ofstream file(nombreArchivo);
	bool header = true;
	function<void(T)> escribirNodo = [&](T data) {
		if (header) {
			file << data->escribirCabecera() << "\n";
			header = false;
		}
		file << data->escribirLinea() << "\n";
		};
	EnOrden(escribirNodo);
	file.close();
}

template <class T>
void ArbolBinario<T>::leer() {
	ifstream file(nombreArchivo);
	string line;
	bool header = true;
	while (getline(file, line)) {
		if (header) {
			header = false;
			continue;
		}
		T data = new remove_pointer<T>::type();
		data->leerLinea(line);
		insertar(data);
	}
	file.close();
}

template <class T>
void ArbolBinario<T>::recargar() {
	ifstream file(nombreArchivo);
	string line;
	bool header = true;
	while (getline(file, line)) {
		if (header) {
			header = false;
			continue;
		}
		T data = new remove_pointer<T>::type();
		data->leerLinea(line);
		NodoArbol<T>* nodo = _buscar(data->getId(), raiz);
		if (nodo != nullptr) {
			nodo->data = data;
		}
		else {
			insertar(data);
		}
	}
	file.close();
}

template <class T>
void ArbolBinario<T>::agregar(T item) {
	ofstream file(nombreArchivo, ios::app);
	file << item->escribirLinea() << "\n";
	file.close();
	insertar(item);
}

template <class T>
void ArbolBinario<T>::editar(T item) {
	ifstream file(nombreArchivo);
	ofstream temp("temp.csv");
	string line;
	bool header = true;
	while (getline(file, line)) {
		if (header) {
			temp << line << "\n";
			header = false;
			continue;
		}
		T data = new remove_pointer<T>::type();
		data->leerLinea(line);
		if (data->getId() == item->getId()) {
			temp << item->escribirLinea() << "\n";
		}
		else {
			temp << line << "\n";
		}
	}
	file.close();
	temp.close();
	remove(nombreArchivo.c_str());
	rename("temp.csv", nombreArchivo.c_str());
}

template <class T>
void ArbolBinario<T>::eliminar(T item) {
	ifstream file(nombreArchivo);
	ofstream temp("temp.csv");
	string line;
	bool header = true;
	while (getline(file, line)) {
		if (header) {
			temp << line << "\n";
			header = false;
			continue;
		}
		T data = new remove_pointer<T>::type();
		data->leerLinea(line);
		if (data->getId() != item->getId()) {
			temp << line << "\n";
		}
	}
	file.close();
	temp.close();
	remove(nombreArchivo.c_str());
	rename("temp.csv", nombreArchivo.c_str());
}

template<class T>
template<typename C>
inline ArbolBinario<T>* ArbolBinario<T>::searchMultipleByValue(C callback)
{
	ArbolBinario<T>* result = new ArbolBinario<T>();
	function<void(T)> buscar = [&](T data) {
		if (callback(data)) {
			result->insertar(data);
		}
		};
	EnOrden(buscar);
	return result;
}

template<class T>
template<typename C>
inline void ArbolBinario<T>::ordenar(C callback)
{
	ArbolBinario<T>* result = new ArbolBinario<T>(callback, "");
	EnOrden([&](T data) {
		result->insertar(data);
		});
	this->raiz = result->raiz;
	this->requisito_division = result->requisito_division;
}