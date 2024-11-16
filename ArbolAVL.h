#pragma once
#include "NodoArbol.h"
#include "functional"
#include "string"
#include "functional"

using namespace std;

template <class T>
class ArbolAVL {
private:
	string nombreArchivo;
	NodoArbol<T>* root;
	function<bool(T, T)> requisito_division;
	int total;
	int nextId = 1;
	void _PreOrden(function<void(T)> funcion, NodoArbol<T>* nodo);
	void _EnOrden(function<void(T)> funcion, NodoArbol<T>* nodo);
	void _PostOrden(function<void(T)> funcion, NodoArbol<T>* nodo);
	NodoArbol<T>* _buscar(int id, NodoArbol<T>* aux);
	NodoArbol<T>* RotarDer(NodoArbol<T>* y);
	NodoArbol<T>* RotarIzq(NodoArbol<T>* x);
	NodoArbol<T>* Insertar(NodoArbol<T>* raiz, NodoArbol<T>* nuevo);
	NodoArbol<T>* ValorIzq(NodoArbol<T>* raiz);
	NodoArbol<T>* Borrar(NodoArbol<T>* raiz, T valor);
	int ObtFactorEquil(NodoArbol<T>* raiz);
public:
	ArbolAVL(string nombreArchivo = "");
	ArbolAVL(function<bool(T, T)> funcion, string nombreArchivo = "");

	bool EstaVacio();
	int Altura(NodoArbol<T>* raiz);

	void ImpPreorden(NodoArbol<T>* raiz);
	void ImpEnorden(NodoArbol<T>* raiz);
	void ImpPostorden(NodoArbol<T>* raiz);

	NodoArbol<T>* BuscaRecursiva(NodoArbol<T>* raiz, T valor);

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
	NodoArbol<T>* search(T data);
	template <typename C>
	ArbolAVL<T>* searchMultipleByValue(C callback);

	template <typename C>
	void printTree(NodoArbol<T>* nodo, int espacio, C callback, string prefijo = "", bool esIzquierda = true);

	NodoArbol<T>* getRaiz();
	int getSize();


	void PreOrden(function<void(T)> funcion);
	void EnOrden(function<void(T)> funcion);
	void PostOrden(function<void(T)> funcion);

	template<typename C>
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
ArbolAVL<T>::ArbolAVL(string nombreArchivo = "") {
	this->nombreArchivo = nombreArchivo;
	requisito_division = [](T a, T b) -> bool {
		return a->getId() < b->getId();
		};
	root = nullptr;
	total = 0;
}

template<class T>
inline ArbolAVL<T>::ArbolAVL(function<bool(T, T)> funcion, string nombreArchivo)
{
	this->nombreArchivo = nombreArchivo;
	root = nullptr;
	requisito_division = funcion;
	total = 0;
}

template<class T>
inline void ArbolAVL<T>::insertar(T data)
{
	NodoArbol<T>* nuevaRaiz = Insertar(root, new NodoArbol<T>(data));
	root = nuevaRaiz;
}

template <class T>
bool ArbolAVL<T>::EstaVacio() {
	if (root == nullptr) {
		return true;
	}
	else {
		return false;
	}
}

template <class T>
int ArbolAVL<T>::Altura(NodoArbol<T>* raiz) {
	if (raiz == nullptr) {
		return -1;
	}
	else {
		int AlturaIzq = Altura(raiz->izquierda);
		int AlturaDer = Altura(raiz->derecha);
		if (AlturaIzq > AlturaDer)
			return (AlturaIzq + 1);
		else
			return (AlturaDer + 1);
	}
}

template <class T>
int ArbolAVL<T>::ObtFactorEquil(NodoArbol<T>* raiz) {
	if (raiz == nullptr)
		return -1;
	return Altura(raiz->izquierda) - Altura(raiz->derecha);
}

template <class T>
NodoArbol<T>* ArbolAVL<T>::RotarDer(NodoArbol<T>* y) {
    if (y->izquierda == nullptr) {
        return y;
    }
    NodoArbol<T>* x = y->izquierda;
    NodoArbol<T>* temp = x->derecha;
    x->derecha = y;
    y->izquierda = temp;
    if (root == y) {
        root = x;
    }
    return x;
}

template <class T>
NodoArbol<T>* ArbolAVL<T>::RotarIzq(NodoArbol<T>* x) {
	if (x->derecha == nullptr) {
		return x;
	}
	NodoArbol<T>* y = x->derecha;
	NodoArbol<T>* temp = y->izquierda;
	y->izquierda = x;
	x->derecha = temp;
	return y;
}

template <class T>
NodoArbol<T>* ArbolAVL<T>::Insertar(NodoArbol<T>* raiz, NodoArbol<T>* nuevo) {
	if (raiz == nullptr) {
		raiz = nuevo;
		total++;
		nextId++;
		return raiz;
	}
	if (requisito_division(nuevo->data, raiz->data)) {
		raiz->izquierda = Insertar(raiz->izquierda, nuevo);
	}
	else if (requisito_division(raiz->data, nuevo->data)) {
		raiz->derecha = Insertar(raiz->derecha, nuevo);
	}
	else {
		//cout << "LOS DATOS SE REPITEN...\n";
		//return raiz;

		//Con esto hacemos que se mantengan los repetidos pero solo es para cuando se utiliza el ordenamiento por requisito_division
		raiz->izquierda = Insertar(raiz->izquierda, nuevo);
	}
	int FE = ObtFactorEquil(raiz);
	//CASO LL
	if (FE > 1 && requisito_division(nuevo->data, raiz->izquierda->data)) {
		return RotarDer(raiz);
	}
	//CASO RR
	if (FE < -1 && requisito_division(raiz->derecha->data, nuevo->data)) {
		return RotarIzq(raiz);
	}
	//CASO LR
	if (FE > 1 && requisito_division(raiz->izquierda->data, nuevo->data)) {
		raiz->izquierda = RotarIzq(raiz->izquierda);
		return RotarDer(raiz);
	}
	//CASO RL
	if (FE < -1 && requisito_division(nuevo->data, raiz->derecha->data)) {
		raiz->derecha = RotarDer(raiz->derecha);
		return RotarIzq(raiz);
	}
    return raiz;
}

template <class T>
NodoArbol<T>* ArbolAVL<T>::ValorIzq(NodoArbol<T>* raiz) {
	NodoArbol<T>* temp = raiz;
	while (temp->izquierda != nullptr) {
		temp = temp->izquierda;
	}
	return temp;
}


template <class T>
NodoArbol<T>* ArbolAVL<T>::Borrar(NodoArbol<T>* raiz, T valor) {
	if (raiz == nullptr) {
		return nullptr;
	}
	else if (requisito_division(valor, raiz->data)) {
		raiz->izquierda = Borrar(raiz->izquierda, valor);
	}
	else if (requisito_division(raiz->data, valor)) {
		raiz->derecha = Borrar(raiz->derecha, valor);
	}
	else {
		if (raiz->izquierda == nullptr) {
			NodoArbol<T>* temp = raiz->derecha;
			total--;
			delete raiz;
			return temp;
		}
		else if (raiz->derecha == nullptr) {
			NodoArbol<T>* temp = raiz->izquierda;
			total--;
			delete raiz;
			return temp;
		}
		else {
			NodoArbol<T>* temp = ValorIzq(raiz->derecha);
			raiz->data = temp->data;
			raiz->derecha = Borrar(raiz->derecha, temp->data);
		}
	}
	int FE = ObtFactorEquil(raiz);
	if (FE == 2 && ObtFactorEquil(raiz->izquierda) >= 0) {
		return RotarDer(raiz);
	}
	else if (FE == 2 && ObtFactorEquil(raiz->izquierda) == -1) {
		raiz->izquierda = RotarIzq(raiz->izquierda);
		return RotarDer(raiz);
	}
	else if (FE == -2 && ObtFactorEquil(raiz->derecha) <= 0) {
		return RotarIzq(raiz);
	}
	else if (FE == -2 && ObtFactorEquil(raiz->derecha) == 1) {
		raiz->derecha = RotarDer(raiz->derecha);
		return RotarIzq(raiz);
	}
	return raiz;
}

template<class T>
template <typename C>
inline void ArbolAVL<T>::printTree(NodoArbol<T>* nodo, int espacio, C callback, string prefijo, bool esIzquierda)
{
    if (nodo == nullptr)
    {
    return;
    }

    // Imprimir el nodo actual
    cout << prefijo;
    if (prefijo.empty()) {
		cout << "|-R-";
    } else {
		cout << (esIzquierda ? "|-I-" : "|-D-");
    }
    callback(nodo->data);
    cout << endl;

    // Procesar el subárbol izquierdo y derecho
    printTree(nodo->izquierda, espacio, callback, prefijo + (esIzquierda ? "| " : "  "), true);
    printTree(nodo->derecha, espacio, callback, prefijo + (esIzquierda ? "| " : "  "), false);
}

template<class T>
inline NodoArbol<T>* ArbolAVL<T>::getRaiz()
{
	return root;
}

template <class T>
void ArbolAVL<T>::ImpPreorden(NodoArbol<T>* raiz) {
	if (raiz == nullptr) {
		return;
	}
	cout << raiz->data << " ";
	ImpPreorden(raiz->izquierda);
	ImpPreorden(raiz->derecha);
}

template <class T>
void ArbolAVL<T>::ImpEnorden(NodoArbol<T>* raiz) {
	if (raiz == nullptr) {
		return;
	}
	ImpEnorden(raiz->izquierda);
	cout << raiz->data << " ";
	ImpEnorden(raiz->derecha);
}

template <class T>
void ArbolAVL<T>::ImpPostorden(NodoArbol<T>* raiz) {
	if (raiz == nullptr) {
		return;
	}
	ImpPostorden(raiz->izquierda);
	ImpPostorden(raiz->derecha);
	cout << raiz->data << " ";
}


template <class T>
void ArbolAVL<T>::_PreOrden(function<void(T)> funcion, NodoArbol<T>* nodo)
{
	if (nodo == nullptr)return;
	if (nodo->data == nullptr)return;
	funcion(nodo->data);
	_PreOrden(funcion, nodo->izquierda);
	_PreOrden(funcion, nodo->derecha);
}

template <class T>
void ArbolAVL<T>::_EnOrden(function<void(T)> funcion, NodoArbol<T>* nodo)
{
	if (nodo == nullptr)return;
	_EnOrden(funcion, nodo->izquierda);
	funcion(nodo->data);
	_EnOrden(funcion, nodo->derecha);
}

template <class T>
void ArbolAVL<T>::_PostOrden(function<void(T)> funcion, NodoArbol<T>* nodo)
{
	if (nodo == nullptr)return;
	_PostOrden(funcion, nodo->izquierda);
	_PostOrden(funcion, nodo->derecha);
	funcion(nodo->data);
}

template<class T>
inline NodoArbol<T>* ArbolAVL<T>::_buscar(int id, NodoArbol<T>* aux)
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

template<class T>
inline NodoArbol<T>* ArbolAVL<T>::search(int id)
{
	return _buscar(id, root);
}

template<class T>
inline NodoArbol<T>* ArbolAVL<T>::search(T adata)
{
	return _buscar(adata->getId(), root);
}

template <class T>
void ArbolAVL<T>::PreOrden(function<void(T)> funcion)
{
	return _PreOrden(funcion, root);
}

template <class T>
void ArbolAVL<T>::EnOrden(function<void(T)> funcion)
{
	return _EnOrden(funcion, root);
}

template <class T>
void ArbolAVL<T>::PostOrden(function<void(T)> funcion)
{
	return _PostOrden(funcion, root);
}


template <class T>
NodoArbol<T>* ArbolAVL<T>::BuscaRecursiva(NodoArbol<T>* raiz, T valor) {
	if (raiz == nullptr || raiz->data == valor) {
		return raiz;
	}
	else if (valor < raiz->data) {
		BuscaRecursiva(raiz->izquierda, valor);
	}
	else {
		BuscaRecursiva(raiz->derecha, valor);
	}
}

template<class T>
inline void ArbolAVL<T>::removeElement(NodoArbol<T>* nodo)
{
	root = Borrar(root, nodo->data);
}

template<class T>
inline void ArbolAVL<T>::removeElement(T data)
{
	root = Borrar(root, data);
}

template<class T>
inline void ArbolAVL<T>::removeElement(int id)
{
	NodoArbol<T>* nodo = buscar(id);
	if (nodo != nullptr) {
		root = Borrar(root, nodo->data);
	}
}

template<class T>
inline void ArbolAVL<T>::updateElement(T data)
{
	NodoArbol<T>* nodo = _buscar(data->getId(), root);
	if (nodo != nullptr) {
		nodo->data = data;
	}
	else {
		insertar(data);
	}
}

template<class T>
inline void ArbolAVL<T>::updateElement(T data, int id)
{
	NodoArbol<T>* nodo = _buscar(id, root);
	if (nodo != nullptr) {
		nodo->data = data;
	}
}

template<class T>
inline int ArbolAVL<T>::getSize()
{
	return total;
}

template<class T>
inline int ArbolAVL<T>::getNextId()
{
	return nextId;
}

template <class T>
void ArbolAVL<T>::escribir() {
	ofstream file(nombreArchivo);
	bool header = true;
	function<void(T)> escribirNodo = [&](T data) {
		if (header) {
			file << data->escribirCabecera() << "\n";
			header = false;
		}
		file << data->escribirLinea() << "\n";
	};
	PreOrden(escribirNodo);
	file.close();
}

template <class T>
void ArbolAVL<T>::leer() {
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
void ArbolAVL<T>::recargar() {
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
		NodoArbol<T>* nodo = _buscar(data->getId(), root);
		if (nodo != nullptr) {
			nodo->data = data;
		} else {
			insertar(data);
		}
	}
	file.close();
}

template <class T>
void ArbolAVL<T>::agregar(T item) {
	ofstream file(nombreArchivo, ios::app);
	file << item->escribirLinea() << "\n";
	file.close();
	insertar(item);
}

template <class T>
void ArbolAVL<T>::editar(T item) {
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
		} else {
			temp << line << "\n";
		}
	}
	file.close();
	temp.close();
	remove(nombreArchivo.c_str());
	rename("temp.csv", nombreArchivo.c_str());
	NodoArbol<T>* nodo = BuscaRecursiva(root, item->getId());
	if (nodo != nullptr) {
		nodo->data = item;
	}
}

template <class T>
void ArbolAVL<T>::eliminar(T item) {
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
	root = Borrar(root, item->getId());
}

template<class T>
template<typename C>
inline ArbolAVL<T>* ArbolAVL<T>::searchMultipleByValue(C callback)
{
	ArbolAVL<T>* result = new ArbolAVL<T>("");
	function<void(T)> buscar = [&](T data) {
		if (callback(data)) {
			result->insertar(data);
		}
		};
	EnOrden(buscar);
	return result;
}

template <class T>
template<typename C>
void ArbolAVL<T>::ordenar(C callback) {
	ArbolAVL<T>* result = new ArbolAVL<T>(callback);
	EnOrden([&](T data) {
		result->insertar(data);
		});
	this->root = result->root;
	this->requisito_division = result->requisito_division;
}
