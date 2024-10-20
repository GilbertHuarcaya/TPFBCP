#pragma once
#pragma once
#include <fstream>

using namespace std;

template <class L, class T>
class File {
public:
	static void escribir(string path, L lista) {
		ofstream file(path);
		Nodo<T*>* temp = lista->head;
		while (temp != nullptr) {
			file << temp->data->escribirLinea();
			if (temp->next != nullptr) {
				file << "\n";
			}
			temp = temp->next;
		}
		file.close();
	}

	static L leer(string path, L lista) {
		ifstream file(path);
		string line;
		while (getline(file, line)) {
			T* item = new T();
			item->leerLinea(line);
			if (item->getId() != 0) {
				lista->push_back(item);
			}
		}
		file.close();
		return lista;
	}

	static void agregar(string path, T* item) {
		ofstream file(path, ios::app);
		file << item->escribirLinea() << "\n";
		file.close();
	}

	static void editar(string path, T* item) {
		ifstream file(path);
		ofstream temp("temp.txt");
		string line;
		while (getline(file, line)) {
			T* tempItem = new T();
			tempItem->leerLinea(line);
			if (tempItem->getId() == item->getId()) {
				temp << item->escribirLinea() << "\n";
			}
			else {
				temp << line << "\n";
			}
		}
		file.close();
		temp.close();
		remove(path.c_str());
		rename("temp.txt", path.c_str());
	}

	static void eliminar(string path, T* item) {
		ifstream file(path);
		ofstream temp("temp.txt");
		string line;
		while (getline(file, line)) {
			T* tempItem = new T();
			tempItem->leerLinea(line);
			if (tempItem->getId() != item->getId()) {
				temp << line << "\n";
			}
		}
		file.close();
		temp.close();
		remove(path.c_str());
		rename("temp.txt", path.c_str());
	}

	static void escribirHash(string path, L hashTabla) {
		ofstream file(path);

		for (int i = 0; i < hashTabla->size(); i++)
		{
			if (hashTabla->getByPos(i) != nullptr) {
				file << to_string(hashTabla->getByPos(i)->getKey()) + "," + hashTabla->getByPos(i)->getValue()->escribirLinea() + "\n";
			}
			else {
				file << "\n";
			}
		}
		file.close();
	}
	/*
	static L leer(string path, L lista) {
		ifstream file(path);
		string line;
		while (getline(file, line)) {
			T* item = new T();
			item->leerLinea(line);
			if (item->getId() != 0) {
				lista->push_back(item);
			}
		}
		file.close();
		return lista;
	}

	static void agregar(string path, T* item) {
		ofstream file(path, ios::app);
		file << item->escribirLinea() << "\n";
		file.close();
	}

	static void editar(string path, T* item) {
		ifstream file(path);
		ofstream temp("temp.txt");
		string line;
		while (getline(file, line)) {
			T* tempItem = new T();
			tempItem->leerLinea(line);
			if (tempItem->getId() == item->getId()) {
				temp << item->escribirLinea() << "\n";
			}
			else {
				temp << line << "\n";
			}
		}
		file.close();
		temp.close();
		remove(path.c_str());
		rename("temp.txt", path.c_str());
	}

	static void eliminar(string path, T* item) {
		ifstream file(path);
		ofstream temp("temp.txt");
		string line;
		while (getline(file, line)) {
			T* tempItem = new T();
			tempItem->leerLinea(line);
			if (tempItem->getId() != item->getId()) {
				temp << line << "\n";
			}
		}
		file.close();
		temp.close();
		remove(path.c_str());
		rename("temp.txt", path.c_str());
	}
	*/
};
