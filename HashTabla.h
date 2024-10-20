#pragma once
#include "iostream"
#include "HashEntidad.h"

template <typename T>
class HashTabla {
private:
	HashEntidad<T>** tabla;
	int numElementos;
	int TABLE_SIZE;

public:
	HashTabla(int TABLE_SIZE = 128) {
		this->TABLE_SIZE = TABLE_SIZE;
		tabla = new HashEntidad<T> *[TABLE_SIZE];
		for (int i = 0; i < TABLE_SIZE; ++i) {
			tabla[i] = nullptr;
		}
		numElementos = 0;
	}
	~HashTabla()
	{
		for (int i = 0; i < TABLE_SIZE; ++i) {
			if (tabla[i] != nullptr) {
				delete tabla[i];
			}
		}
		delete[] tabla;
	}

	void insertar(int key, T value) {
		//Hash prima
		int base, step, hash;
		//validar si la tabla está llena
		if (numElementos == TABLE_SIZE)
		{
			cout << "La tabla está llena." << endl;
			return;
		}
		//Función Hash1
		base = key % TABLE_SIZE;
		hash = base;
		//constante para Hash2
		step = 0;
		while (tabla[hash] != nullptr)
		{
			//Función Hash2
			hash = (base + step) % TABLE_SIZE;
			step++;
		}
		//almacenarlo en la tabla
		tabla[hash] = new HashEntidad<T>(key, value);
		numElementos++;
		cout << "Registro agregado correctamente." << endl;
	}
	void eliminar(int key) {
		int id = buscar(key);
		if (id == -1) {
			cout << "No se encontró el elemento" << endl;
			return;
		}
		delete tabla[id];
		tabla[id] = nullptr;
		numElementos--;
		cout << "Registro eliminado correctamente." << endl;
	}
	int size() {
		return TABLE_SIZE;
	}
	int sizeactual() {
		return numElementos;
	}
	HashEntidad<T>* getByPos(int pos) {
		if (tabla[pos] == nullptr) return nullptr;
		return tabla[pos];
	}
	int buscar(int key) {
		int step = 0;
		int i, base;
		i = base = key % TABLE_SIZE; //hash1 es = a hash2 cuando step=0;
		while (true)
		{
			if (tabla[i] == nullptr)
			{
				cout << "No se encontro el registro." << endl;
				return -1;
			}
			else if (tabla[i]->getKey() == key) {
				cout << "Registro encontrado." << endl;
				return i;
			}
			else step++;

			i = (base + step) % TABLE_SIZE;
		}
	}
	T get(int key) {
		int id = buscar(key);
		if (id == -1) {
			return nullptr;
		}
		return tabla[id]->getValue();
	}
	void print() {
		for (int i = 0; i < TABLE_SIZE; i++) {
			if (tabla[i] != nullptr) {
				cout << i << " : ";
				tabla[i]->getValue()->print();
				cout << endl;
			}
		}
	}
	void recalcular(int oldSize) {
		HashEntidad<T>** temp = tabla;
		tabla = new HashEntidad<T> *[TABLE_SIZE];
		for (int i = 0; i < TABLE_SIZE; ++i) {
			tabla[i] = nullptr;
		}
		numElementos = 0;
		for (int i = 0; i < oldSize; i++) {
			if (temp[i] != nullptr) {
				insertar(temp[i]->getKey(), temp[i]->getValue());
			}
		}
		delete[] temp;
	}
	void rehash() {
		int oldSize = TABLE_SIZE;
		TABLE_SIZE *= 2;
		recalcular(oldSize);
		cout << "Rehashing completado. Tamaño de la tabla: " << TABLE_SIZE << endl;
	}
	void resize(int newSize) {
		int oldSize = TABLE_SIZE;
		TABLE_SIZE = newSize;
		recalcular(oldSize);
		cout << "Rehashing completado. Tamaño de la tabla: " << TABLE_SIZE << endl;
	}
};
