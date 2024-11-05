#pragma once
#include "Elemento.h"
#include "ListaDoble.h"
#include "Canal.h"
#include "File.h"
#include "string"
#include "sstream"

using namespace std;

enum EstadoSede {
	SEDE_INACTIVO,
	SEDE_ACTIVO
};

class Sede : public Elemento
{
private:
	string nombre;
	string direccion;
	string ciudad;
	string distrito;
	string departamento;
	string telefono;
	string email;
	EstadoSede estado;
	ListaDoble<Canal*>* canales = new ListaDoble<Canal*>();
public:
	Sede();
	Sede(int id);
	Sede(int id, string nombre, string direccion, string ciudad, string distrito, string departamento, string telefono, string email);
	~Sede();

	//GETTERS
	string getNombre();
	string getDireccion();
	string getCiudad();
	string getDistrito();
	string getDepartamento();
	string getTelefono();
	string getEmail();
	EstadoSede getEstado();
	ListaDoble<Canal*>* getCanales();

	//SETTERS
	void setNombre(string nombre);
	void setDireccion(string direccion);
	void setCiudad(string ciudad);
	void setDistrito(string distrito);
	void setDepartamento(string departamento);
	void setTelefono(string telefono);
	void setEmail(string email);
	void setEstado(EstadoSede estado);

	//UPDATE
	void activar(); // esto no activa todos los canales de la sede
	void desactivar(); // esto no desactiva todos los canales de la sede
	void activarSedeYCanalesAsociados(); // esto activa la sede y todos los canales asociados
	void desactivarSedeYCanalesAsociados(); // esto desactiva la sede y todos los canales asociados

	//SEARCH
	ListaDoble<Canal*>* buscarCanalesPorTipo(ETipoDeCanal tipo);

	//FILE
	string escribirLinea() override;
	void leerLinea(string linea) override;
	string escribirCabecera() override;

	void loadCanales();

	//PRINT
	void print();


	//SORT
	template <typename L>
	static void ordenarPorNombre(L lista, bool ascendente = true);
	
};

Sede::Sede() : Elemento() {
	this->id = 0;
	this->nombre = "";
	this->direccion = "";
	this->ciudad = "";
	this->distrito = "";
	this->departamento = "";
	this->telefono = "";
	this->email = "";
	this->estado = SEDE_INACTIVO;
};
Sede::Sede(int id) : Elemento(id) {
	this->nombre = "";
	this->direccion = "";
	this->ciudad = "";
	this->distrito = "";
	this->departamento = "";
	this->telefono = "";
	this->email = "";
	this->estado = SEDE_INACTIVO;
}
inline Sede::Sede(int id, string nombre, string direccion, string ciudad, string distrito, string departamento, string telefono, string email)
{
	this->id = id;
	this->nombre = nombre;
	this->direccion = direccion;
	this->ciudad = ciudad;
	this->distrito = distrito;
	this->departamento = departamento;
	this->telefono = telefono;
	this->email = email;
	this->estado = SEDE_INACTIVO;
}
inline Sede::~Sede()
{
	Nodo<Canal*>* temp = canales->head;
	while (temp != nullptr)
	{
		delete temp->data;
		temp = temp->next;
	}
	delete canales;
}

string Sede::getNombre() {
	return nombre;
}

string Sede::getDireccion() {
	return direccion;
}

inline string Sede::getCiudad()
{
	return ciudad;
}

inline string Sede::getDistrito()
{
	return distrito;
}

inline string Sede::getDepartamento()
{
	return departamento;
}

string Sede::getTelefono() {
	return telefono;
}

string Sede::getEmail() {
	return email;
}

void Sede::setNombre(string nombre) {
	this->nombre = nombre;
}

void Sede::setDireccion(string direccion) {
	this->direccion = direccion;
}

inline void Sede::setCiudad(string ciudad)
{
	this->ciudad = ciudad;
}

inline void Sede::setDistrito(string distrito)
{
	this->distrito = distrito;
}

inline void Sede::setDepartamento(string departamento)
{
	this->departamento = departamento;
}

void Sede::setTelefono(string telefono) {
	this->telefono = telefono;
}

void Sede::setEmail(string email) {
	this->email = email;
}

void Sede::setEstado(EstadoSede estado) {
	this->estado = estado;
}

EstadoSede Sede::getEstado() {
	return estado;
}

inline ListaDoble<Canal*>* Sede::getCanales()
{
	return canales;
}

void Sede::activar() {
	estado = SEDE_ACTIVO;
}

void Sede::desactivar() {
	estado = SEDE_INACTIVO;
}

inline void Sede::activarSedeYCanalesAsociados()
{
	estado = SEDE_ACTIVO;
	Nodo<Canal*>* temp = canales->head;
	while (temp != nullptr)
	{
		temp->data->activar();
		temp = temp->next;
	}

	cout << "Sede y canales asociados activados" << endl;
}

inline void Sede::desactivarSedeYCanalesAsociados()
{
	estado = SEDE_INACTIVO;
	Nodo<Canal*>* temp = canales->head;
	while (temp != nullptr)
	{
		temp->data->desactivar();
		temp = temp->next;
	}

	cout << "Sede y canales asociados desactivados" << endl;
}

inline ListaDoble<Canal*>* Sede::buscarCanalesPorTipo(ETipoDeCanal tipo)
{
	ListaDoble<Canal*>* canalesPorTipo = new ListaDoble<Canal*>();
	Nodo<Canal*>* temp = canales->head;
	while (temp != nullptr)
	{
		if (temp->data->getTipoDeCanal() == tipo)
		{
			canalesPorTipo->push_back(temp->data);
		}
		temp = temp->next;
	}

	if (canalesPorTipo->head == nullptr)
	{
		cout << "No se encontraron canales de este tipo" << endl;
	}

	return canalesPorTipo;
}

string Sede::escribirLinea() {
	stringstream ss;
	ss << id << "," << nombre << "," << direccion << "," << ciudad << "," << distrito << "," << departamento << "," << telefono << "," << email << "," << estado;
	return ss.str();
}

void Sede::leerLinea(string linea) {
	stringstream ss(linea);
	string item;
	getline(ss, item, ',');
	if (item != "" || item != "id") {
		id = stoi(item);
		getline(ss, nombre, ',');
		getline(ss, direccion, ',');
		getline(ss, ciudad, ',');
		getline(ss, distrito, ',');
		getline(ss, departamento, ',');
		getline(ss, telefono, ',');
		getline(ss, email, ',');
		getline(ss, item, ',');
		estado = item != "" ? EstadoSede(stoi(item)) : SEDE_INACTIVO;
	}
	loadCanales();

}

inline string Sede::escribirCabecera()
{
	return "id,nombre,direccion,ciudad,distrito,departamento,telefono,email,estado";
}

inline void Sede::loadCanales()
{
	ListaDoble<Canal*>* todosLosCanales = new ListaDoble<Canal*>("Canales.csv");
	File<ListaDoble<Canal*>*, Canal>::leer("Canales.csv", todosLosCanales);
	Nodo<Canal*>* temp = todosLosCanales->head;
	while (temp != nullptr)
	{
		if (temp->data->getIdSede() == id)
		{
			Nodo<Canal*>* temp2 = canales->search(temp->data->getId());
			if (canales->getSize() != 0 && temp2 != nullptr)
			{
				canales->update(temp->data);
			}
			else
			{
				canales->push_back(temp->data);
			}
		}
		temp = temp->next;
	}
}

inline void Sede::print()
{
	string estadoStr[] = {
	"INACTIVO",
	"ACTIVO"
	};

	cout << "ID: " << id << endl;
	cout << "Nombre: " << nombre << endl;
	cout << "Direccion: " << direccion << endl;
	cout << "Ciudad: " << ciudad << endl;
	cout << "Distrito: " << distrito << endl;
	cout << "Departamento: " << departamento << endl;
	cout << "Telefono: " << telefono << endl;
	cout << "Email: " << email << endl;
	cout << "Estado: " << estadoStr[estado] << endl;
	cout << "Cajeros y Ventanillas: " << endl;
	if (canales->head == nullptr) {
		cout << "No hay Cajeros ni ventanillas" << endl;
	}
	else {
		cout << "Tiene " << canales->getSize() << " canales." << endl;
	}
	cout << endl;
}

template <typename L>
inline void Sede::ordenarPorNombre(L lista, bool ascendente)
{
	auto compararNombres = [&](Sede* a, Sede* b) {
		return ascendente ? a->getNombre() < b->getNombre() : a->getNombre() > b->getNombre();
		};

	lista->heapsort(compararNombres);
}
