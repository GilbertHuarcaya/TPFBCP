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

	static L leer(string path, L lista) {
		ifstream file(path);
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
				T* item = new T();
				item->leerLinea(line);
				if (item->getId() != 0) {
					lista->push_back(item);
				}
			}
		}
		file.close();
		return lista;
	}

	static void recargar(string path, L lista) {
		ifstream file(path);
		string line;
		while (getline(file, line)) {
			T* item = new T();
			item->leerLinea(line);
			if (item->getId() != 0) {
				if (lista->search(item->getId()) != nullptr) {
					lista->update(item);
				}
				else {
					lista->push_back(item);
				}

			}
		}
	}

	static void agregar(string path, T* item) {
		//leer primero para ver si hay cabecera
		ifstream file(path);
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
				file << to_string(hashTabla->getByPos(i)->getKey()) << "," << hashTabla->getByPos(i)->getValue()->escribirLinea() << "\n";
			}
		}
		file.close();
	}

	static L leerHash(string path, L hashTabla) {
		ifstream file(path);
		string line;
		string aux;
		while (getline(file, line)) {
			if (line != "")
			{
				T* item = new T();
				aux = line.substr(0, line.find(",") - 1);
				int key = stoi(aux);
				line = line.substr(line.find(",") + 1);
				item->leerLinea(line);
				if (item->getId() != 0) {
					hashTabla->insertar(key, item);
				}
			}
		}
		file.close();
		return hashTabla;
	}

	static void agregarHash(string path, T* item, int key) {
		ofstream file(path, ios::app);
		file << to_string(key) << "," << item->escribirLinea() << "\n";
		file.close();
	}

	static void editarHash(string path, T* item, int key) {
		ifstream file(path);
		ofstream temp("temp.txt");
		string line;
		while (getline(file, line)) {
			T* tempItem = new T();
			tempItem->leerLinea(line);
			if (tempItem->getId() == item->getId()) {
				temp << to_string(key) << "," << item->escribirLinea() << "\n";
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

	static void eliminarHash(string path, T* item) {
		ifstream file(path);
		ofstream temp("temp.txt");
		string line;
		while (getline(file, line)) {
			T* tempItem = new T();
			line = line.substr(line.find(',') + 1);
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

	static void escribirHashTablaLista(string path, L hashTablaLista) {
		ofstream file(path);
		HashEntidadNodo<T*>* temp = hashTablaLista->head;
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

	static L leerHashTablaLista(string path, L hashTablaLista) {
		ifstream file(path);
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
				T* item = new T();
				item->leerLinea(line);
				if (item->getId() != 0) {
					int key = item->getId();
					hashTablaLista->push_back(item, key);
				}
			}
		}
		file.close();
		return hashTablaLista;
	}

	static void agregarHashTablaLista(string path, T* item) {
		//leer primero para ver si hay cabecera
		ifstream file(path);
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

	static void recargarHashTablaLista(string path, L lista) {
		ifstream file(path);
		string line;
		while (getline(file, line)) {
			T* item = new T();
			item->leerLinea(line);
			if (item->getId() != 0) {
				int key = item->getId();
				if (lista->search(key) != nullptr) {
					lista->update(item, key);
				}
				else {
					lista->push_back(item, key);
				}
			}
		}

	}

	static void editarHashTablaLista(string path, T* item) {
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

	static void eliminarHashTablaLista(string path, T* item) {
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

};
