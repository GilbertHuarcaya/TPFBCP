#pragma once
#include "iostream"
#include "vector"
#include "GVertice.h"
#include "GArista.h"
#include "fstream"
#include "string"
#include "functional"

using namespace std;

template <typename T, typename Q>
class Grafo
{
	string nombreArchivoVertices;
	string nombreArchivoAristas;
	function<bool(T, T, Q)> requisito_arcos;
	int total;
	int nextId = 1;
	vector<GVertice<T, Q>*>* vertices;
	int lastPos;
	bool esRepetido(T dato);
	int getPosDato(T dato);
public:
	Grafo();
	Grafo(function<bool(T, T, Q)> requisito_arcos);
	Grafo(string nombreArchivoVertices, string nombreArchivoAristas, function<bool(T, T, Q)> requisito_arcos);
	Grafo(T dato_a, T dato_b, Q arista);
	Grafo(T dato, Q arista);
	int getLastPosVertice();
	int cantidadVertices();
	bool agregarVertice(T dato);
	void agregarVerticesEnlazadosNoDirigido(T dato_a, T dato_b, Q arista);
	void agregarVerticesEnlazadosDirigido(T dato_a, T dato_b, Q arista);
	void agregarVerticeconBucle(T dato, Q arista);
	void enlazarVerticesNoDirigido(T dato_a, T dato_b, Q arista);
	void enlazarVerticesDirigido(T dato_a, T dato_b, Q arista);
	void enlazarVerticeBucle(T dato, Q arista);
	T getUltimoDato();
	T getDatoporPos(int pos);
	Q getAristaporPos(int pos_vertice, int pos_arista);

	//enlazar vertices automaticamente
	void enlazarAutomatico(Q arista);

	//recorridos Vertices
	template<typename C>
	void RecorridoVertices(C callback);

	//recorridos Aristas
	template<typename C>
	void RecorridoInOrden(GVertice<T, Q>* vertice, C callback);
	template<typename C>
	void RecorridoPreOrden(GVertice<T, Q>* vertice, C callback);
	template<typename C>
	void RecorridoPostOrden(GVertice<T, Q>* vertice, C callback);

	//recorridos Vertices y aristas en orden
	template<typename C>
	void RecorridoVerticesYAristasEnOrden(C callback);

	//insertar
	void insertar(T data);

	//eliminar
	void removeElement(T data);
	void removeElement(int id);

	//actualizar solo la data
	void updateElement(T data);
	void updateElement(T data, int id);

	//buscar
	T search(int id);
	T search(T data);
	template <typename C>
	Grafo<T, Q>* searchMultipleByValue(C callback);

	//print
	template<typename C>
	void printGrafo(C formatV);
	template<typename C>
	void printVertices(C format);
	template<typename C>
	void printAristas(C format);
	template<typename C>
	void printAristasDeVertice(int pos, C format);
	template<typename C>
	void printAristasDeVertice(T vertice, C format);

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

template <typename T, typename Q>
Grafo<T, Q>::Grafo()
{
	vertices = new vector<GVertice<T, Q>*>;
	lastPos = -1;
	requisito_arcos = nullptr;
}

template<typename T, typename Q>
inline Grafo<T, Q>::Grafo(function<bool(T, T, Q)> requisito_arcos)
{
	vertices = new vector<GVertice<T, Q>*>;
	lastPos = -1;
	this->requisito_arcos = requisito_arcos;
}

template<typename T, typename Q>
inline Grafo<T, Q>::Grafo(string nombreArchivoVertices, string nombreArchivoAristas, function<bool(T, T, Q)> requisito_arcos)
{
	this->nombreArchivoVertices = nombreArchivoVertices;
	this->nombreArchivoAristas = nombreArchivoAristas;
	vertices = new vector<GVertice<T, Q>*>;
	lastPos = -1;
	this->requisito_arcos = requisito_arcos;

}

template <typename T, typename Q>
Grafo<T, Q>::Grafo(T dato_a, T dato_b, Q arista)
{
	vertices = new vector<GVertice<T, Q>*>;
	vertices->push_back(new GVertice<T, Q>(dato_a));
	vertices->push_back(new GVertice<T, Q>(dato_b));
	vertices->at(0)->agregarArista(new GArista<Q>(arista, 0 , 1));
	vertices->at(1)->agregarArista(new GArista<Q>(arista, 1 , 0));
	lastPos = 1;
	requisito_arcos = nullptr;
}

template <typename T, typename Q>
Grafo<T, Q>::Grafo(T dato, Q arista)
{
	vertices = new vector<GVertice<T, Q>*>;
	vertices->push_back(new GVertice<T, Q>(dato));
	vertices->at(0)->agregarArista(new GArista<Q>(arista,0,0));
	lastPos = 0;
	requisito_arcos = nullptr;
}

template <typename T, typename Q>
int Grafo<T, Q>::getLastPosVertice()
{
	return lastPos;
}

template <typename T, typename Q>
int Grafo<T, Q>::cantidadVertices()
{
	return vertices->size();
}

template <typename T, typename Q>
bool Grafo<T, Q>::agregarVertice(T dato)
{
	if (esRepetido(dato))return false;
	GVertice<T, Q>* aux = new GVertice<T, Q>(dato);
	aux->setDato(dato);
	vertices->push_back(aux);
	lastPos++;
	return true;
}

template <typename T, typename Q>
void Grafo<T, Q>::agregarVerticesEnlazadosNoDirigido(T dato_a, T dato_b, Q arista)
{
	if (!agregarVertice(dato_a))return;
	if (!agregarVertice(dato_b))return;
	int aux_a = lastPos - 1;
	vertices->at(lastPos)->agregarArista(new GArista<Q>(arista, lastPos ,aux_a));
	vertices->at(aux_a)->agregarArista(new GArista<Q>(arista, aux_a, lastPos));
}

template <typename T, typename Q>
void Grafo<T, Q>::agregarVerticesEnlazadosDirigido(T dato_a, T dato_b, Q arista)
{
	if (!agregarVertice(dato_a))return;
	if (!agregarVertice(dato_b))return;
	int aux_a = lastPos - 1;
	vertices->at(aux_a)->agregarArista(new GArista<Q>(arista, aux_a, lastPos));
}

template <typename T, typename Q>
void Grafo<T, Q>::agregarVerticeconBucle(T dato, Q arista)
{
	if(!agregarVertice(dato))return;
	vertices->at(lastPos)->agregarArista(new GArista<Q>(arista, lastPos, lastPos));
}

template <typename T, typename Q>
void Grafo<T, Q>::enlazarVerticesNoDirigido(T dato_a, T dato_b, Q arista)
{
	if (getPosDato(dato_a) == -1||getPosDato(dato_b) == -1)return;
	vertices->at(getPosDato(dato_a))->agregarArista(new GArista<Q>(arista, getPosDato(dato_a), getPosDato(dato_b)));
	vertices->at(getPosDato(dato_b))->agregarArista(new GArista<Q>(arista, getPosDato(dato_b), getPosDato(dato_a)));
}

template <typename T, typename Q>
void Grafo<T, Q>::enlazarVerticesDirigido(T dato_a, T dato_b, Q arista)
{
	if (getPosDato(dato_a) == -1 || getPosDato(dato_b) == -1)return;
	vertices->at(getPosDato(dato_a))->agregarArista(new GArista<Q>(arista, getPosDato(dato_a), getPosDato(dato_b)));
}

template <typename T, typename Q>
void Grafo<T, Q>::enlazarVerticeBucle(T dato, Q arista)
{
	if (getPosDato(dato) == -1)return;
	vertices->at(getPosDato(dato))->agregarArista(new GArista<Q>(arista, getPosDato(dato), getPosDato(dato)));
}

template <typename T, typename Q>
T Grafo<T, Q>::getUltimoDato()
{
	if (lastPos == -1)return nullptr;
	return vertices->at(lastPos)->getDato();
}

template <typename T, typename Q>
T Grafo<T, Q>::getDatoporPos(int pos)
{
	if (pos < 0 || pos >= vertices->size())return nullptr;
	return vertices->at(pos)->getDato();
}

template <typename T, typename Q>
Q Grafo<T, Q>::getAristaporPos(int pos_vertice, int pos_arista)
{
	if (pos_vertice < 0 || pos_vertice >= vertices->size())return nullptr;
	return vertices->at(pos_vertice)->getAristaPorPos(pos_arista)->getDato();
}

template<typename T, typename Q>
inline void Grafo<T, Q>::enlazarAutomatico(Q arista)
{
	for (int i = 0; i < vertices->size(); i++)
	{
		for (int j = 0; j < vertices->size(); j++)
		{
			if (i == j)continue;
			if (requisito_arcos(vertices->at(i)->getDato(), vertices->at(j)->getDato(), arista))
			{
				vertices->at(i)->agregarArista(new GArista<Q>(arista, i, j));
			}
		}
	}
}

template<typename T, typename Q>
inline void Grafo<T, Q>::insertar(T data)
{
	agregarVertice(data);
	nextId++;
}

template<typename T, typename Q>
inline void Grafo<T, Q>::removeElement(T data)
{
	int pos = getPosDato(data);
	if (pos == -1)return;
	vertices->erase(vertices->begin() + pos);
	lastPos--;
}

template<typename T, typename Q>
inline void Grafo<T, Q>::removeElement(int id)
{
	int pos = -1;
	for (int i = 0; i < vertices->size(); i++)
	{
		if (vertices->at(i)->getId() == id)
		{
			pos = i;
			break;
		}
	}
	if (pos == -1)return;
	vertices->erase(vertices->begin() + pos);
	lastPos--;

}

template<typename T, typename Q>
inline void Grafo<T, Q>::updateElement(T data)
{
	int pos = getPosDato(data);
	if (pos == -1)return;
	vertices->at(pos)->setDato(data);
}

template<typename T, typename Q>
inline void Grafo<T, Q>::updateElement(T data, int id)
{
	int pos = -1;
	for (int i = 0; i < vertices->size(); i++)
	{
		if (vertices->at(i)->getId() == id)
		{
			pos = i;
			break;
		}
	}
	if (pos == -1)return;
	vertices->at(pos)->setDato(data);
}

template<typename T, typename Q>
inline T Grafo<T, Q>::search(int id)
{
	for (int i = 0; i < vertices->size(); i++)
	{
		if (vertices->at(i)->getId() == id)
		{
			return vertices->at(i)->getDato();
		}
	}
	return nullptr;
}

template<typename T, typename Q>
inline T Grafo<T, Q>::search(T data)
{
	for (int i = 0; i < vertices->size(); i++)
	{
		if (vertices->at(i)->getDato() == data)
		{
			return vertices->at(i)->getDato();
		}
	}
	return nullptr;
}

template<typename T, typename Q>
template<typename C>
inline void Grafo<T, Q>::printGrafo(C format)
{
	cout << endl << "Vertices: " << endl;

	for (int i = 0; i < vertices->size(); i++)
	{
		GVertice<T, Q>* vertice = vertices->at(i);
		cout << "Vertice: " << vertice->getDato()->getId() << endl;
		for (int j = 0; j < vertice->aristas->size(); j++)
		{

			cout << "	->: " << format(vertice->getAristaPorPos(j)->getDato()) << " -> Vertice LLegada: " << vertices->at(vertice->aristas->at(j)->getPosLlegada())->getDato()->getId()  << endl;
		}
	}
}

template<typename T, typename Q>
template<typename C>
inline void Grafo<T, Q>::printVertices(C format)
{
	cout << endl << "Vertices: " << endl;
	for (int i = 0; i < vertices->size(); i++)
	{
		cout << "Vertice: " << format(vertices->at(i)->getDato()) << endl;
	}
}

template<typename T, typename Q>
template<typename C>
inline void Grafo<T, Q>::printAristas(C format)
{
	cout << endl << "Aristas: " << endl;

	for (int i = 0; i < vertices->size(); i++)
	{
		GVertice<T, Q>* vertice = vertices->at(i);

		for (int j = 0; j < vertice->aristas->size(); j++)
		{
			cout << " ->: " << format(vertice->getAristaPorPos(j)->getDato()) << endl;
		}
	}
}

template<typename T, typename Q>
template<typename C>
inline void Grafo<T, Q>::printAristasDeVertice(int pos, C format)
{
	cout << endl << endl;
	if (pos < 0 || pos >= vertices->size())return;
	GVertice<T, Q>* vertice = vertices->at(pos);
	cout << "Vertice: " << vertice->getDato()->getId() << endl;
	for (int j = 0; j < vertice->aristas->size(); j++)
	{
		cout << "	->: " << format(vertice->getAristaPorPos(j)->getDato()) << " -> Vertice LLegada: " << vertices->at(vertice->aristas->at(j)->getPosLlegada())->getDato()->getId()  << endl;

	}
}

template<typename T, typename Q>
template<typename C>
inline void Grafo<T, Q>::printAristasDeVertice(T dato, C format)
{
	cout << endl << endl;
	int pos = getPosDato(dato);
	if (pos == -1)return;
	GVertice<T, Q>* vertice = vertices->at(pos);
	cout << "Vertice: " << vertice->getDato()->getId() << endl;
	for (int j = 0; j < vertice->aristas->size(); j++)
	{
		cout << "	->: " << format(vertice->getAristaPorPos(j)->getDato()) << " -> Vertice LLegada: " << vertices->at(vertice->aristas->at(j)->getPosLlegada())->getDato()->getId() << endl;

	}
}

template <typename T, typename Q>
bool Grafo<T, Q>::esRepetido(T dato)
{
	bool verif = false;
	for (int i = 0; i < vertices->size(); i++)
	{
		if (dato == vertices->at(i)->getDato())
		{
			verif = true;
			break;
		}
	}
	return verif;
}

template <typename T, typename Q>
int Grafo<T, Q>::getPosDato(T dato)
{
	int aux = -1;
	for (int i = 0; i < vertices->size(); i++)
	{
		if (dato == vertices->at(i)->getDato())
		{
			aux = i;
			break;
		}
	}
	return aux;
}


template<typename T, typename Q>
template<typename C>
inline void Grafo<T, Q>::RecorridoVertices(C callback)
{
	for (int i = 0; i < vertices->size(); i++)
	{
		callback(vertices->at(i)->getDato());
	}
}

template<typename T, typename Q>
template<typename C>
inline void Grafo<T, Q>::RecorridoInOrden(GVertice<T, Q>* vertice, C callback)
{
	if (vertice == nullptr)return;
	for (int i = 0; i < vertice->aristas->size(); i++)
	{
		RecorridoInOrden(vertices->at(vertice->aristas->at(i)->getPosLlegada()), callback);
	}
	callback(vertice->getDato());
}

template<typename T, typename Q>
template<typename C>
inline void Grafo<T, Q>::RecorridoPreOrden(GVertice<T, Q>* vertice, C callback)
{
	if (vertice == nullptr)return;
	callback(vertice->getDato());
	for (int i = 0; i < vertice->aristas->size(); i++)
	{
		RecorridoPreOrden(vertices->at(vertice->aristas->at(i)->getPosLlegada()), callback);
	}
}

template<typename T, typename Q>
template<typename C>
inline void Grafo<T, Q>::RecorridoPostOrden(GVertice<T, Q>* vertice, C callback)
{
	if (vertice == nullptr)return;
	for (int i = 0; i < vertice->aristas->size(); i++)
	{
		RecorridoPostOrden(vertices->at(vertice->aristas->at(i)->getPosLlegada()), callback);
	}
	callback(vertice->getDato());
}

template<typename T, typename Q>
template<typename C>
inline void Grafo<T, Q>::RecorridoVerticesYAristasEnOrden(C callback)
{
	for (int i = 0; i < vertices->size(); i++)
	{
		callback(vertices->at(i)->getDato());
		for (int j = 0; j < vertices->at(i)->aristas->size(); j++)
		{
			callback(vertices->at(vertices->at(i)->aristas->at(j)->getPosLlegada())->getDato());
		}
	}
}

template<typename T, typename Q>
template<typename C>
inline Grafo<T, Q>* Grafo<T, Q>::searchMultipleByValue(C callback)
{
	Grafo<T, Q>* aux = new Grafo<T, Q>();
	for (int i = 0; i < vertices->size(); i++)
	{
		if (callback(vertices->at(i)->getDato()))
		{
			aux->agregarVertice(vertices->at(i)->getDato());
		}
	}
	return aux;
}

template<typename T, typename Q>
template<typename C>
inline void Grafo<T, Q>::ordenar(C callback)
{
	for (int i = 0; i < vertices->size(); i++)
	{
		for (int j = 0; j < vertices->size() - 1; j++)
		{
			if (callback(vertices->at(j)->getDato(), vertices->at(j + 1)->getDato()))
			{
				T aux = vertices->at(j)->getDato();
				vertices->at(j)->setDato(vertices->at(j + 1)->getDato());
				vertices->at(j + 1)->setDato(aux);
			}
		}
	}
}

template<typename T, typename Q>
inline int Grafo<T, Q>::getNextId()
{
	return nextId;
}

template<typename T, typename Q>
void Grafo<T, Q>::escribir() {
	ofstream file(nombreArchivoVertices);
	bool header = true;
	function<void(T)> escribirVertice = [&](T data) {
		if (header) {
			file << data->escribirCabecera() << "\n";
			header = false;
		}
		file << data->escribirLinea() << "\n";
		};
	RecorridoVertices(escribirVertice);
	file.close();

	ofstream file(nombreArchivoAristas);
	bool header = true;
	function<void(T)> escribirArista = [&](T data) {
		if (header) {
			file << data->escribirCabecera() << "\n";
			header = false;
		}
		file << data->escribirLinea() << "\n";
		};
	RecorridoInOrden(escribirArista);

	file.close();
}

template<typename T, typename Q>
void Grafo<T, Q>::leer() {
	ifstream file(nombreArchivoVertices);
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

	ifstream file(nombreArchivoAristas);
	string line;
	bool header = true;
	while (getline(file, line)) {
		if (header) {
			header = false;
			continue;
		}
		T data = new remove_pointer<T>::type();
		data->leerLinea(line);
		enlazarAutomatico(data, requisito_arcos);
	}
	file.close();
}

template<typename T, typename Q>
void Grafo<T, Q>::recargar() {
	ifstream file(nombreArchivoVertices);
	string line;
	bool header = true;
	while (getline(file, line)) {
		if (header) {
			header = false;
			continue;
		}
		T data = new remove_pointer<T>::type();
		data->leerLinea(line);
	
		if (esRepetido(data)) {
			updateElement(data);
		}
		else {
			insertar(data);
		}
	}
	file.close();
}

template<typename T, typename Q>
void Grafo<T, Q>::agregar(T item) {
	ofstream file(nombreArchivoVertices, ios::app);
	file << item->escribirLinea() << "\n";
	file.close();
	insertar(item);
}

template<typename T, typename Q>
void Grafo<T, Q>::editar(T item) {
	ifstream file(nombreArchivoVertices);
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
	remove(nombreArchivoVertices.c_str());
	rename("temp.csv", nombreArchivoVertices.c_str());
}

template<typename T, typename Q>
void Grafo<T, Q>::eliminar(T item) {
	ifstream file(nombreArchivoVertices);
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
	remove(nombreArchivoVertices.c_str());
	rename("temp.csv", nombreArchivoVertices.c_str());
}