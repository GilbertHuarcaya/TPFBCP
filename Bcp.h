#pragma once
#include <conio.h>
#include "colors.h"
#include "File.h"
#include "Tarjeta.h"
#include "Cliente.h"
#include "Sede.h"
#include "Canal.h"
#include "Cola.h"
#include "HashTabla.h"
#include "ListaDoble.h"
#include "functional"
#include "vector"
#include "iostream"

using namespace std;
using namespace System;

void gotoxy(int x, int y)
{
	Console::SetCursorPosition(x, y);
}

class Bcp
{

private:
	string nombre;
	ListaDoble<Cliente*>* clientes;
	ListaDoble<CuentaBancaria*>* cuentas;
	ListaDoble<Tarjeta*>* tarjetas;
	ListaDoble<Operacion*>* operaciones;
	HashTabla<Cliente*>* hashClientes;
	ListaDoble<Canal*>* canales;
	Cola<Operacion*>* colaOperaciones;
	ListaDoble<Sede*>* sedes;

public:
	Bcp()
	{
		this->nombre = "Banco de Credito del Peru";
		clientes = new ListaDoble<Cliente*>("Clientes.csv");
		cuentas = new ListaDoble<CuentaBancaria*>("Cuentas.csv");
		tarjetas = new ListaDoble<Tarjeta*>("Tarjetas.csv");
		operaciones = new ListaDoble<Operacion*>("Operaciones.csv");
		canales = new ListaDoble<Canal*>("Canales.csv");
		colaOperaciones = new Cola<Operacion*>("ColaOperaciones.csv");
		sedes = new ListaDoble<Sede*>("Sedes.csv");
		hashClientes = new HashTabla<Cliente*>("hashTabla.csv");
		loadAll();
	}

	string getNombre();
	ListaDoble<Cliente*>* getClientes();
	ListaDoble<CuentaBancaria*>* getCuentas();
	ListaDoble<Tarjeta*>* getTarjetas();
	ListaDoble<Operacion*>* getOperaciones();
	ListaDoble<Canal*>* getCanales();
	Cola<Operacion*>* getColaOperaciones();
	ListaDoble<Sede*>* getSedes();

	template <class T>
	int getLastId(T lista);

	template <class T, class E>
	Nodo<E*>* buscarPorId(int id, T lista);

	template <class T, class E>
	void agregar(E* item, T lista);

	template <class C, class E>
	void encolar(E* item, C cola);

	template <class C, class E>
	E* desencolar(C cola);

	template <class T, class E>
	void editar(E* item, T lista);

	template <class T, class E>
	void eliminar(E* item, T lista);

	template <class T>
	void listar(T lista);

	template <class T, class E>
	void save(T lista);

	template <class T, class E>
	void load(T lista);

	template<class T, class E>
	void reload(T lista);

	void loadAll();

	void reloadAll();

	template <class HT, class E>
	void saveHash(HT hash);

	template<class HT, class E>
	void loadHash(HT hash);

	template<typename T>
	void crearMenu(const vector<string>& opciones, T callback);

	template<typename T>
	void crearFormulario(const vector<string>& campos, T callback);

	ListaDoble<CuentaBancaria*>* buscarCuentaPorIdCliente(int idCliente);

	ListaDoble<Canal*>* buscarCanalesSinSede();

	void ejecutarOperacion(Operacion* operacion);

	//Ordenamientos
	template<class T>
	void QuickSort(ListaDoble<T>* lista, function<bool(T, T)> comp, int inicio, int fin);

	template<class T>
	int particion_QS(ListaDoble<T>* lista, function<bool(T, T)> comp, int inicio, int fin);

	//Menu y relacionados
	void LogoBCP(int x, int y);

	void MenuBCP();

	//MENU CLIENTE

	void MenuSoloCliente(Cliente* cliente);

	void MenuSoloCuentaBancaria(CuentaBancaria* CuentaB);

	void MenuElegirCanalOSede(CuentaBancaria* cuenta);

	void MenuOperacionPorSede(Sede* sede, CuentaBancaria* cuenta);

	void MenuOperacionPorCanal(Canal* canal, CuentaBancaria* cuenta);

	
	//MENU ADMINISTRADOR

	void MenuAdmin();

	//GESTION
	void MenuSedes();

	void MenuCanales();

	void MenuClientes();

	void MenuOperaciones();

	void MenuCuentas();

	void MenuTarjetas();

};

string Bcp::getNombre() { return nombre; }
ListaDoble<Cliente*>* Bcp::getClientes() { return clientes; }
ListaDoble<CuentaBancaria*>* Bcp::getCuentas() { return cuentas; }
ListaDoble<Tarjeta*>* Bcp::getTarjetas() { return tarjetas; }
ListaDoble<Operacion*>* Bcp::getOperaciones() { return operaciones; }
ListaDoble<Canal*>* Bcp::getCanales() { return canales; }
Cola<Operacion*>* Bcp::getColaOperaciones() { return colaOperaciones; }
ListaDoble<Sede*>* Bcp::getSedes() { return sedes; }

template <class T>
int Bcp::getLastId(T lista)
{
	if (lista->getSize() == 0)
		return 1;
	else
		return lista->getLastId();
}

template <class T, class E>
Nodo<E*>* Bcp::buscarPorId(int id, T lista)
{
	Nodo<E*>* nodo = lista->head;
	while (nodo != nullptr)
	{
		if (nodo->data->getId() == id)
			return nodo;
		nodo = nodo->next;
	}
	return nullptr;
}

template <class L, class E>
L buscarPorCantidadAleatorio(L lista, int cantidad)
{
	L listaAleatoria = new ListaDoble<E*>();
	int size = lista->getSize();
	if (size < cantidad)
	{
		return nullptr;
	}
	for (int i = 0; i < cantidad; i++)
	{
		int random = rand() % size;
		Nodo<E*>* nodo = lista->search(random);

		if (nodo != nullptr)
			listaAleatoria->push_back(nodo->data);
	}
	return listaAleatoria;
}

template <class T, class E>
void Bcp::agregar(E* item, T lista)
{
	lista->push_back(item);
	this->save<T, E>(lista);
}

template<class C, class E>
inline void Bcp::encolar(E* item, C cola)
{
	cola->encolar(item);
	this->save<C, E>(cola);
}

template<class C, class E>
inline E* Bcp::desencolar(C cola)
{
	E* item = cola->desencolar();
	this->save<C, E>(cola);
	return item;
}

template <class T, class E>
void Bcp::editar(E* item, T lista)
{
	Nodo<E*>* nodo = buscarPorId<T,E>(item->getId(), lista);
	if (nodo != nullptr)
	{
		nodo->data = item;
		this->save<T, E>(lista);
	}
}

template <class T, class E>
void Bcp::eliminar(E* item, T lista)
{
	Nodo<E*>* nodo = buscarPorId<T,E>(item->getId(), lista);
	if (nodo != nullptr)
	{
		lista->remove(nodo);
		this->save<T, E>(lista);
	}
}

template <class T>
void Bcp::listar(T lista) {
	lista->print();
}

template <class T, class E>
void Bcp::save(T lista)
{
	File<T, E>::escribir(lista->getNombreArchivo(), lista);
}

template <class T, class E>
void Bcp::load(T lista)
{
	File<T, E>::leer(lista->getNombreArchivo(), lista);
}

template <class T, class E>
void Bcp::reload(T lista)
{
	File<T, E>::recargar(lista->getNombreArchivo(), lista);
}

void Bcp::loadAll()
{
	load<ListaDoble<Cliente*>*, Cliente>(clientes);
	load<ListaDoble<CuentaBancaria*>*, CuentaBancaria>(cuentas);
	load<ListaDoble<Tarjeta*>*, Tarjeta>(tarjetas);
	load<ListaDoble<Operacion*>*, Operacion>(operaciones);
	load<ListaDoble<Canal*>*, Canal>(canales);
	load<ListaDoble<Sede*>*, Sede>(sedes);
	
}

void Bcp::reloadAll()
{
	reload<ListaDoble<Cliente*>*, Cliente>(clientes);
	reload<ListaDoble<CuentaBancaria*>*, CuentaBancaria>(cuentas);
	reload<ListaDoble<Tarjeta*>*, Tarjeta>(tarjetas);
	reload<ListaDoble<Operacion*>*, Operacion>(operaciones);
	reload<ListaDoble<Canal*>*, Canal>(canales);
	reload<ListaDoble<Sede*>*, Sede>(sedes);
}

//Archivos para hash
template <class HT, class E>
void Bcp::saveHash(HT hash)
{
	File<HT, E>::escribirHash(hash->getNombreArchivo(), hash);
}

template <class HT, class E>
void Bcp::loadHash(HT hash)
{
	File<HT, E>::leerHash(hash->getNombreArchivo(), hash);
}

//Cuentas

ListaDoble<CuentaBancaria*>* Bcp::buscarCuentaPorIdCliente(int idCliente)
{
	Nodo<CuentaBancaria*>* temp = cuentas->head;
	ListaDoble<CuentaBancaria*>* cuentasCliente = new ListaDoble<CuentaBancaria*>();
	while (temp != nullptr)
	{
		if (temp->data->getIdCliente() == idCliente)
		{
			cuentasCliente->push_back(temp->data);
		}
		temp = temp->next;
	}
	return cuentasCliente->head != nullptr ? cuentasCliente : nullptr;
}

inline ListaDoble<Canal*>* Bcp::buscarCanalesSinSede()
{
	Nodo<Canal*>* temp = canales->head;
	ListaDoble<Canal*>* canalesSinSede = new ListaDoble<Canal*>();
	while (temp != nullptr)
	{
		if (temp->data->getIdSede() == 0)
		{
			canalesSinSede->push_back(temp->data);
		}
		temp = temp->next;
	}
	return canalesSinSede->head != nullptr ? canalesSinSede : nullptr;
}

void Bcp::ejecutarOperacion(Operacion* operacion) {
	Nodo<CuentaBancaria*>* cuentaBancariaOrigen = cuentas->search(operacion->getIdCuentaBancariaOrigen());
	Nodo<CuentaBancaria*>* cuentaBancariaDestino = cuentas->search(operacion->getIdCuentaBancariaDestino());
	operacion->validarOperacion();
	if (operacion->getTipo() == Transferencia) {
		// Transferir
		if (cuentaBancariaOrigen == nullptr || cuentaBancariaDestino == nullptr) {
			operacion->setEstado(Rechazada);
			cout << "Cuenta origen o destino no encontrada" << endl;
			return;
		}

		if (cuentaBancariaOrigen->data->getSaldo() >= operacion->getMonto()) {
			cuentaBancariaOrigen->data->transferir(cuentaBancariaDestino->data, operacion->getMonto());
			operacion->setEstado(Realizada);
		}
		else {
			operacion->setEstado(Rechazada);
			cout << "Saldo insuficiente" << endl;
		}
	}
	else if (operacion->getTipo() == Deposito) {
		// Depositar
		if (cuentaBancariaDestino == nullptr) {
			operacion->setEstado(Rechazada);
			cout << "Cuenta destino no encontrada" << endl;
			return;
		}
		cuentaBancariaDestino->data->depositar(operacion->getMonto());
		operacion->setEstado(Realizada);
	}
	else if (operacion->getTipo() == Retiro) {
		// Retirar
		if (cuentaBancariaOrigen == nullptr) {
			operacion->setEstado(Rechazada);
			cout << "Cuenta destino no encontrada" << endl;
			return;
		}
		if (cuentaBancariaOrigen->data->getSaldo() >= operacion->getMonto()) {
			cuentaBancariaOrigen->data->retirar(operacion->getMonto());
			operacion->setEstado(Realizada);
		}
		else {
			operacion->setEstado(Rechazada);
			cout << "Saldo insuficiente" << endl;
		}
	}

	//recargar cuentas bancarias de los clientes
	if (cuentaBancariaOrigen != nullptr)
		buscarPorId<ListaDoble<Cliente*>*, Cliente>(cuentaBancariaOrigen->data->getIdCliente(), clientes)->data->loadCuentasBancarias();
	if (cuentaBancariaDestino != nullptr)
		buscarPorId<ListaDoble<Cliente*>*, Cliente>(cuentaBancariaDestino->data->getIdCliente(), clientes)->data->loadCuentasBancarias();
}

//Menu y relacionados
void Bcp::LogoBCP(int x, int y)
{
	gotoxy(x, y); cout << BLUE << "                *%%%%%%%%%+        *%%%%%%%%#   *%%%%%%%%#+" << RESET;
	gotoxy(x, y + 1); cout << BLUE << "  -#%%%*-       -%%%%+++*%%%%=   -%%%%%##**##%=  %%%%*+**#%%%=  -#%%%+-";
	gotoxy(x, y + 2); cout << BLUE << "  -*#%%#" << ORANGE << "===" << BLUE << "     *%%%+    +%%%*  *%%%%*          *%%%%     %%%#  =*%%%#" << ORANGE << "===" << RESET;
	gotoxy(x, y + 3); cout << BLUE << "      *+" << ORANGE << "====" << BLUE << "    #%%%=--=#%%%+  +%%%%=           #%%%=    =%%%+      *+" << ORANGE << "====" << RESET;
	gotoxy(x, y + 4); cout << ORANGE << "      -=====-" << BLUE << "  +%%%%%%%%%#=    #%%%*           =%%%%==-+#%%%+       " << ORANGE << "-======" << RESET;
	gotoxy(x, y + 5); cout << ORANGE << "     =======" << BLUE << "   %%%%-   -%%%%=  #%%%+           #%%%%%%%%%#=        " << ORANGE << "=======" << RESET;
	gotoxy(x, y + 6); cout << ORANGE << "  -=======" << BLUE << "    +%%%*     #%%%*  *%%%%          =%%%#             " << ORANGE << "-=======" << RESET;
	gotoxy(x, y + 7); cout << ORANGE << "-======-" << BLUE << "      *%%%+:-=+#%%%#    #%%%%#+==+*#  *%%%-           " << ORANGE << "=======-" << RESET;
	gotoxy(x, y + 8); cout << BLUE << "              =%%%%%%%%%%*       -#%%%%%%%%- -%%%*" << RESET;
}

void Bcp::MenuBCP()
{
	vector<string> opciones = { "Iniciar Sesion", "Registrarse", "Salir" };

	auto callback = [this](int opcion) {
		switch (opcion)
		{
		case 0:
		{
			//formulario inicio sesion:
			vector<string> campos = { "Correo", "Contraseña" };
			bool encontrado = false;
			Nodo<Cliente*>* ClienteEncontrado = nullptr;
			crearFormulario(campos, [&](const vector<string>& valores) -> bool {
				Nodo<Cliente*>* Clientes = this->clientes->getFirst();
		
				while (Clientes != nullptr)
				{
					if (Clientes->data->getEmail() == valores[0] && Clientes->data->getPassword() == valores[1])
					{
						ClienteEncontrado = Clientes;
						encontrado = true;
					}
					Clientes = Clientes->next;
				}

				if (!encontrado)
				{
					Console::Clear();
					LogoBCP(18, 1);
					gotoxy(40, 12); cout << "No se pudo encontrar la Cuenta del Cliente...";
					gotoxy(40, 13); system("pause");
					Console::Clear();

					char opcion;
					gotoxy(40, 13); cout << "¿Desea seguir intentando? (s/n): ";
					cin >> opcion;

					return opcion == 's' || opcion == 'S';
				}
			
				return false;

				});

			if (!encontrado)
			{
				break;
			}

			Console::Clear();
			if (ClienteEncontrado->data->getEmail() == "admin@bcp.pe") {
				//AGREGAR MENU DE ADMINISTRACION / GESTION DE TODAS LAS TABLAS/LISTAS
			}
			MenuSoloCliente(ClienteEncontrado->data);
			encontrado = false;
			ClienteEncontrado = nullptr;
			break;
		}
		case 1:
		{
			Console::Clear();
			
			string nombre, apellido, telefono, direccion, email, password;
			password = "0";
			

			//formulario registro de cliente:
			vector<string> camposRegistro = { "Nombre", "Apellido", "Direccion" };
			vector<string> camposRegistroTelefono = { "Telefono" };
			vector<string> camposRegistroEmail = { "Email" };
			vector<string> camposRegistroContrasenia = { "Contrasenia" };

			crearFormulario(camposRegistro, [&](const vector<string>& valores) -> bool {
				nombre = valores[0];
				apellido = valores[1]; 
				telefono = valores[2];
				return false;
				});
			crearFormulario(camposRegistroTelefono, [&](const vector<string>& valores) -> bool {
				bool repetido = false;
				
				Nodo<Cliente*>* cliente = clientes->searchByValue([&](Cliente* cliente) {
					return cliente->getTelefono() == valores[0];
					});
				repetido = cliente != nullptr;

				if (repetido)
				{
					gotoxy(45, 10); cout << "El telefono es repetido. Intente nuevamente";
					gotoxy(45, 12); system("pause");
					return true;
				}
				telefono = valores[0];
				return false;
				});
			crearFormulario(camposRegistroEmail, [&](const vector<string>& valores) -> bool {
				bool repetido = false;
				Nodo<Cliente*>* cliente = clientes->searchByValue([&](Cliente* cliente) {
					return cliente->getEmail() == valores[0];
					});
				repetido = cliente != nullptr;
				if (repetido)
				{
					gotoxy(45, 10); cout << "El email es repetido. Intente nuevamente";
					gotoxy(45, 12); system("pause");
					return true;
				}
				email = valores[0];
				return false;
				});
			crearFormulario(camposRegistroContrasenia, [&](const vector<string>& valores) -> bool {
				if (valores[0].size() != 6)
				{
					gotoxy(45, 10); cout << "La contrasenia debe de tener 6 digitos";
					gotoxy(43, 12); system("pause");
					return true;
				}
				password = valores[0];
				return false;
				});
			Cliente* cliente = new Cliente(this->getLastId(clientes), nombre, apellido, direccion, telefono, email, password);
			agregar(cliente, clientes);

			this->hashClientes->insertar(cliente->getId(), cliente);
			this->saveHash<HashTabla<Cliente*>*, Cliente>(hashClientes);

			gotoxy(45, 12); cout << LBLUE << "Cuenta Registrada Correctamente" << RESET;
			gotoxy(45, 14); cout << RED << system("pause") << RESET;
			Console::Clear();
			break;
		}
		case 2:
			return false;
			break;
		}
		return true;
		};

	crearMenu(opciones, callback);

}

void Bcp::MenuSoloCliente(Cliente* cliente)
{
	vector<string> opciones = { 
		"Ver mis Datos", 
		"Modificar uno de mis datos", 
		"Ver mis Cuentas Bancarias", 
		"Agregar una Cuenta Bancaria Random", 
		"Acceder a mi Cuenta Bancaria", 
		"Eliminar una de mis Cuentas Bancarias", 
		"Salir" };

	auto callback = [&](int opcion) {
		switch (opcion)
		{
		case 0:
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(45, 11); cout << LBLUE << "Datos del Cliente" << RESET;
			gotoxy(45, 14); cout << "Nombre: " << cliente->getNombre();
			gotoxy(45, 15); cout << "Apellido: " << cliente->getApellido();
			gotoxy(45, 16); cout << "Telefono: " << cliente->getTelefono();
			gotoxy(45, 17); cout << "Direccion: " << cliente->getDireccion();
			gotoxy(45, 18); cout << "Email: " << cliente->getEmail();
			gotoxy(45, 19); cout << "Contrasenia: " << cliente->getPassword();
			gotoxy(45, 20); cout << "No de Cuentas Bancarias: " << cliente->getCuentasBancarias()->getSize();
			gotoxy(45, 22); cout << RED << system("pause") << RESET;
			Console::Clear();
			break;
		case 1:
		{
			vector<string> opciones = { "Nombre", "Apellido", "Telefono", "Direccion", "Email", "Contrasenia", "Salir" };
			string aux;
			auto callback = [&](int opcion) {
				switch (opcion)
				{
				case 0:
					Console::Clear();
					LogoBCP(18, 1);
					gotoxy(40, 14); cout << "Nombre anterior: " << cliente->getNombre();
					gotoxy(40, 15); cout << "Nuevo Nombre: "; cin >> aux;
					cliente->setNombre(aux);
					editar(cliente, clientes);
					gotoxy(40, 17); cout << "Nombre cambiado correctamente";
					gotoxy(40, 18); system("pause");
					Console::Clear();
					break;
				case 1:
					Console::Clear();
					LogoBCP(18, 1);
					gotoxy(40, 14); cout << "Apellido anterior: " << cliente->getApellido();
					gotoxy(40, 15); cout << "Nuevo telefono: "; cin >> aux;
					cliente->setApellido(aux);
					editar(cliente, clientes);
					gotoxy(40, 17); cout << "Apellido cambiado correctamente";
					gotoxy(40, 18); system("pause");
					Console::Clear();
					break;
				case 2:
					Console::Clear();
					LogoBCP(18, 1);
					gotoxy(40, 14); cout << "Telefono anterior: " << cliente->getTelefono();
					gotoxy(40, 15); cout << "Nuevo telefono: "; cin >> aux;
					cliente->setTelefono(aux);
					editar(cliente, clientes);
					gotoxy(40, 17); cout << "Telefono cambiado correctamente";
					gotoxy(40, 18); system("pause");
					Console::Clear();
					break;
				case 3:
					Console::Clear();
					LogoBCP(18, 1);
					gotoxy(40, 14); cout << "Direccion anterior: " << cliente->getDireccion();
					gotoxy(40, 15); cout << "Nueva direccion: "; cin >> aux;
					cliente->setDireccion(aux);
					editar(cliente, clientes);
					gotoxy(40, 17); cout << "Direccion cambiada correctamente";
					gotoxy(40, 18); system("pause");
					Console::Clear();
					break;
				case 4:
					Console::Clear();
						LogoBCP(18, 1);
					gotoxy(40, 14); cout << "Email anterior: " << cliente->getEmail();
					gotoxy(40, 15); cout << "Nuevo email: "; cin >> aux;
					cliente->setEmail(aux);
					editar(cliente, clientes);
					gotoxy(40, 17); cout << "Email cambiado correctamente";
					gotoxy(40, 18); system("pause");
					Console::Clear();
					break;
				case 5:
					Console::Clear();
					LogoBCP(18, 1);
					gotoxy(40, 14); cout << "Contrasenia anterior: " << cliente->getPassword();
					gotoxy(40, 15); cout << "Nueva contrasenia: "; cin >> aux;
					cliente->setPassword(aux);
					editar(cliente, clientes);
					gotoxy(40, 17); cout << "Contrasenia cambiada correctamente";
					gotoxy(40, 18); system("pause");
					Console::Clear();
					break;
				case 6:
					return false;
					break;
				}
				return true;
				};
			crearMenu(opciones, callback);
			break;
		}
		case 2:
			Console::Clear();
			LogoBCP(18, 1);
			if (cliente->getCuentasBancarias()->getSize() != 0)
			{
				gotoxy(0, 12); cliente->getCuentasBancarias()->print();
				system("pause");
				Console::Clear();
				break;
			}
			gotoxy(40, 14); cout << LBLUE << "Usted no tiene ninguna cuenta bancaria a su nombre" << RESET;
			gotoxy(40, 16); system("pause");
			Console::Clear();
			break;
		case 3:
		{
			Console::Clear();
			LogoBCP(18, 1);
			string numero_cuenta;//14 digitos
			string contrasenia;
			bool verificacion;
			do
			{
				numero_cuenta = "";
				verificacion = false;
				Nodo<CuentaBancaria*>* aux_verif = this->cuentas->getFirst();
				for (int i = 0; i < 14; i++)
				{
					Random r;
					numero_cuenta.append(to_string(r.Next(0, 9)));
					_sleep(100);
				}
				do
				{
					if (numero_cuenta == aux_verif->data->getNumeroCuenta())
					{
						verificacion = true;
					}
					aux_verif = aux_verif->next;
				} while (aux_verif != cuentas->getFirst());
			} while (verificacion);
			do
			{
				gotoxy(30, 12); cout << "Ingrese la contrasenia para la cuenta (4 digitos)";
				gotoxy(50, 14); cin >> contrasenia;
				if (contrasenia.size() != 4)
				{
					Console::Clear();
					LogoBCP(18, 1);
				}
			} while (contrasenia.size() != 4);
			CuentaBancaria* nuevaCuentaBancaria = new CuentaBancaria(this->cuentas->getLastId(), cliente->getId(), contrasenia, numero_cuenta, 0);
			this->cuentas->push_back(nuevaCuentaBancaria);
			cliente->agregarCuentaBancaria(nuevaCuentaBancaria);
			agregar(nuevaCuentaBancaria, cuentas);
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Cuenta bancaria agregada correctamente";
			gotoxy(40, 14); system("pause");
			Console::Clear();
			break;
		}
		case 4:
		{
			vector<string> misCuentas;

			ListaDoble<CuentaBancaria*>* cuentasDelCliente = cliente->getCuentasBancarias();

			Nodo<CuentaBancaria*>* temp = cuentasDelCliente->head;
			while (temp != nullptr)
			{
				misCuentas.push_back(temp->data->getNumeroCuenta() + ", " + "SALDO: " + to_string(temp->data->getSaldo()));
				temp = temp->next;
			}

			auto callback1 = [&](int opcion) {

				if (cuentasDelCliente->head != nullptr) {
					MenuSoloCuentaBancaria(cuentas->search(cuentasDelCliente->getByPosition(opcion)->data->getId())->data);
					return false;
				}
		
				return false;
				};
			crearMenu(misCuentas, callback1);

			break;
		}
		case 5:
		{
			int n;
			Console::Clear();
			LogoBCP(18, 1);
			if (cliente->getCuentasBancarias()->getSize() != 0)
			{

				gotoxy(0, 12); cliente->getCuentasBancarias()->print();
				system("pause");
				cout << "\nIngrese el numero de id de la cuenta bancaria a eliminar\n";
				cin >> n;
				Nodo<CuentaBancaria*>* auxiliar = cliente->getCuentasBancarias()->search(n);
				if (auxiliar != nullptr)
				{
					Console::Clear();
					LogoBCP(18, 1);
					gotoxy(50, 14); cout << "Id: " << auxiliar->data->getId();
					gotoxy(50, 15); cout << "Id Cliente: " << auxiliar->data->getIdCliente();
					gotoxy(50, 16); cout << "Numero cuenta: " << auxiliar->data->getNumeroCuenta();
					gotoxy(50, 17); cout << "Contrasenia: " << auxiliar->data->getPassword();
					gotoxy(50, 18); cout << "Saldo: " << auxiliar->data->getSaldo();
					if (auxiliar->data->getTarjeta() != nullptr)
					{
						gotoxy(50, 19); cout << "Nro Tarjeta: " << auxiliar->data->getTarjeta()->getNumero();
						gotoxy(50, 20); cout << "CVV: " << auxiliar->data->getTarjeta()->getCvv();
					}
					else
					{
						gotoxy(50, 19); cout << "La cuenta no tiene tarjeta";
					}
					gotoxy(45, 22); cout << YELLOW << "Esta cuenta ha sido eliminada";
					gotoxy(45, 23); system("pause");
					cliente->getCuentasBancarias()->remove(auxiliar);
					this->cuentas->remove(auxiliar);
					eliminar<ListaDoble<CuentaBancaria*>*, CuentaBancaria>(auxiliar->data, cuentas);
					Console::Clear();
					break;
				}
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(45, 12); cout << "No se encontro la cuenta bancaria";
				gotoxy(45, 14); system("pause");
				Console::Clear();
				break;
			}
			gotoxy(40, 14); cout << LBLUE << "Usted no tiene ninguna cuenta bancaria a su nombre" << RESET;
			gotoxy(40, 16); system("pause");
			Console::Clear();
			break;
		}
		case 6:
			return false;
			break;
		}
		return true;
		};
	crearMenu(opciones, callback);
	Console::Clear();
}

void Bcp::MenuSoloCuentaBancaria(CuentaBancaria* CuentaB)
{
	vector<string> opciones = { "Ver los Datos de la Cuenta", "Cambiar contrasenia", "Ver las operaciones de la cuenta", "Agregar una Tarjeta a la cuenta", "Eliminar la Tarjeta de la cuenta", "Renovar Tarjeta", "Hacer una operacion", "Ver Cola de operaciones", "Salir" };

	auto callback = [&](int opcion) {
		switch (opcion)
		{
		case 0:
		{
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(45, 14); cout << "Id: " << CuentaB->getId();
			gotoxy(45, 15); cout << "Id Cliente: " << CuentaB->getIdCliente();
			gotoxy(45, 16); cout << "Numero de cuenta: " << CuentaB->getNumeroCuenta();
			gotoxy(45, 17); cout << "Contrasenia: " << CuentaB->getPassword();
			gotoxy(45, 18); cout << "Saldo: " << CuentaB->getSaldo();
			gotoxy(50, 19); cout << YELLOW << "Tarjeta:" << RESET;
			if (CuentaB->getTarjeta() != nullptr)
			{
				gotoxy(45, 21); cout << "Numero de tarjeta: " << CuentaB->getTarjeta()->getNumero();
				gotoxy(45, 22); cout << "CVV: " << CuentaB->getTarjeta()->getNumero();
			}
			else
			{
				gotoxy(45, 21); cout << "La cuenta no tiene ninguna tarjeta asignada";
			}
			gotoxy(45, 23); system("pause");
			Console::Clear();
			break;
		}
		case 1:
		{
			Console::Clear();
			string auxiliar;
			do {
				LogoBCP(18, 1);
				gotoxy(40, 14); cout << "Contrasenia anterior: " << CuentaB->getPassword();
				gotoxy(40, 15); cout << "Nueva contrasenia: "; cin >> auxiliar;
				if (auxiliar.size() != 4)
				{
					gotoxy(35, 17); cout << "La contrasenia debe tener 4 digitos";
					gotoxy(35, 18); system("pause");
					Console::Clear();
				}
			} while (auxiliar.size() != 4);
			CuentaB->setPassword(auxiliar);
			editar(CuentaB, cuentas);
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 14); cout << "Contrasenia cambiada correctamente";
			gotoxy(40, 15); system("pause");
			Console::Clear();
			break;
		}
		case 2:
		{
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(0, 12); CuentaB->getOperaciones()->print();
			system("pause");
			Console::Clear();
			break;
		}
		case 3:
		{
			Console::Clear();
			LogoBCP(18, 1);
			if (CuentaB->getTarjeta() != nullptr)
			{
				gotoxy(40, 12); cout << "Usted ya tiene una tarjeta asociada a la cuenta";
				system("pause");
			}
			else
			{
				Tarjeta* auxiliar = new Tarjeta(this->tarjetas->getLastId(), CuentaB->getIdCliente(), CuentaB->getId());
				this->tarjetas->push_back(auxiliar);
				CuentaB->setTarjeta(auxiliar);
				agregar(auxiliar, tarjetas);
				gotoxy(40, 12); cout << "Tarjeta aniadida correctamente";
				gotoxy(40, 13); system("pause");
			}
			Console::Clear();
			break;
		}
		case 4:
		{
			Console::Clear();
			LogoBCP(18, 1);
			Nodo<Tarjeta*>* auxiliar = this->tarjetas->search(CuentaB->getTarjeta()->getId());
			if (auxiliar != nullptr)
			{
				this->tarjetas->remove(auxiliar);
				CuentaB->setTarjeta(nullptr);
				eliminar(auxiliar->data, tarjetas);
				gotoxy(40, 12); cout << "La tarjeta ha sido removida correctamente";
				system("pause");
				break;
			}
			gotoxy(40, 12); cout << "Tu cuenta no tiene una tarjeta asignada";
			system("pause");
			Console::Clear();
			break;
		}
		case 5:
			Console::Clear();
			LogoBCP(18, 1);
			if (CuentaB->getTarjeta() != nullptr)
			{
				Tarjeta* auxiliar = new Tarjeta(this->tarjetas->getLastId(), CuentaB->getIdCliente(), CuentaB->getId());
				this->tarjetas->push_back(auxiliar);
				CuentaB->setTarjeta(auxiliar);
				agregar(auxiliar, tarjetas);
				gotoxy(40, 12); cout << "Tarjeta renovada correctamente";
				gotoxy(40, 13); system("pause");
			}
			else
			{
				gotoxy(40, 12); cout << "Usted NO tiene una tarjeta asociada a la cuenta";
				system("pause");
			}
			Console::Clear();
			break;
		case 6:
			MenuElegirCanalOSede(CuentaB);
			break;
		case 7:
			colaOperaciones->print();
			break;
		case 8:
			Console::Clear();
			return false;
			break;
		default:
			break;
		}
		return true;
	};
	crearMenu(opciones, callback);
}

inline void Bcp::MenuElegirCanalOSede(CuentaBancaria* cuenta)
{
	vector<string> opciones = {
		"Ver las sedes disponibles",
		"Ver los canales disponibles",
		"Salir"
	};

	auto callback2 = [&](int seleccion) {
		switch (seleccion) {
		case 0:
		{
			ListaDoble<Sede*>* sedesDisponibles = buscarPorCantidadAleatorio<ListaDoble<Sede*>*, Sede>(sedes, sedes->getSize() < 5 ? sedes->getSize() : 5);
			vector<string> opcionesSedes;
			for (int i = 0; i < sedesDisponibles->getSize(); i++)
			{
				opcionesSedes.push_back(sedesDisponibles->getByPosition(i)->data->getNombre());
			}
			auto callback = [sedesDisponibles, &cuenta, this](int seleccion) {
				Nodo<Sede*>* sede = sedesDisponibles->getByPosition(seleccion);
				if (sede != nullptr)
					MenuOperacionPorSede(sede->data, cuenta);
				return false;
				};

			crearMenu(opcionesSedes, callback);
			break;
		}
		case 1:
		{
			ListaDoble<Canal*>* canalesSinSedeDisponibles = buscarCanalesSinSede();

			vector<string> opcionesCanales;
			for (int i = 0; i < canalesSinSedeDisponibles->getSize(); i++)
			{
				opcionesCanales.push_back(canalesSinSedeDisponibles->getByPosition(i)->data->getNombre());
			}

			auto callback1 = [canalesSinSedeDisponibles, &cuenta, this](int seleccion) {
				Nodo<Canal*>* canal = canalesSinSedeDisponibles->getByPosition(seleccion);
				if (canal != nullptr)
					MenuOperacionPorCanal(canal->data, cuenta);
				return false;
				};
			crearMenu(opcionesCanales, callback1);
			break;
		}
		case 2:
			return false;
			break;
		}
		return true;
		};

	crearMenu(opciones, callback2);
}

inline void Bcp::MenuOperacionPorSede(Sede* sede, CuentaBancaria* cuenta)
{
	vector<string> opciones = {
		"Ver informacion de la sede",
		"Ver las ventanillas de la sede",
		"Ver los cajeros de la sede",
		"Salir"
	};

	auto callback = [&](int seleccion) {
		switch (seleccion) {
		case 0:
			sede->print();
			gotoxy(45, 22); cout << RED << system("pause") << RESET;
			break;
		case 1:
		{
			ListaDoble<Canal*>* ventanillas = sede->buscarCanalesPorTipo(VENTANILLA);
			vector<string> opcionesVentanillas;
			for (int i = 0; i < ventanillas->getSize(); i++)
			{
				opcionesVentanillas.push_back(ventanillas->getByPosition(i)->data->getNombre());
			}
			auto callback1 = [&cuenta, ventanillas, this](int seleccion1) {
				Nodo<Canal*>* canal = ventanillas->getByPosition(seleccion1);
				if (canal != nullptr)
					MenuOperacionPorCanal(canal->data, cuenta);
				return false;
				};
			crearMenu(opcionesVentanillas, callback1);
			break;
		}
		case 2:
		{
			ListaDoble<Canal*>* cajeros = sede->buscarCanalesPorTipo(CAJERO);
			vector<string> opcionesCajeros;
			for (int i = 0; i < cajeros->getSize(); i++)
			{
				opcionesCajeros.push_back(cajeros->getByPosition(i)->data->getNombre());
			}
			auto callback2 = [&cuenta, cajeros, this](int seleccion2) {
				Nodo<Canal*>* canal = cajeros->getByPosition(seleccion2);
				if (canal != nullptr)
					MenuOperacionPorCanal(canal->data, cuenta);
				return false;
				};
			crearMenu(opcionesCajeros, callback2);
			break;
		}
		case 3:
			return false;
			break;
		}
		return true;
	};
	crearMenu(opciones, callback);
}

inline void Bcp::MenuOperacionPorCanal(Canal* canal, CuentaBancaria* cuenta)
{
	double monto;
	char SN;
	string cuentaDestino;
	string cuentaOrigen = cuenta->getNumeroCuenta();
	Operacion* operacion = new Operacion();
	vector<string> opciones = {
		"Ver informacion del canal",
		"Transferir",
		"Depositar",
		"Retirar",
		"Salir"
	};
	auto callback = [canal, &monto, &SN, &cuentaDestino, &cuentaOrigen, &operacion, cuenta, this](int seleccion) {
		switch (seleccion) {
		case 0:
			canal->print();
			gotoxy(45, 22); cout << RED << system("pause") << RESET;
			break;
		case 1:
			cout << "Su SALDO actual es: " << cuenta->getSaldo() << endl << endl;

			cout << "Transferir..." << endl;

			cout << "Ingrese el monto a transferir: ";
			cin >> monto;

			cout << "Ingrese la cuenta de destino: ";
			cin >> cuentaDestino;

			operacion = cuenta->crearTransferencia(getLastId(colaOperaciones), cuentaOrigen, cuentaDestino, monto, canal->getId(), canal->getIdSede());
			if (operacion != nullptr) colaOperaciones->encolar(operacion);

			system("pause");
			break;
		case 2:
			cout << "Depositar..." << endl;

			cout << "Ingrese el monto a depositar: ";

			cin >> monto;

			cout << "Desea depositar a esta misma cuenta? (s: Si , n: A otra cuenta)";

			cin >> SN;
			if (SN == 's' || SN == 'S') {
				operacion = cuenta->crearDeposito(getLastId(colaOperaciones), cuentaOrigen, monto, canal->getId(), canal->getIdSede());
				if (operacion != nullptr) colaOperaciones->encolar(operacion);
			}
			else {
				cout << "Ingrese la cuenta de destino: ";

				cin >> cuentaDestino;

				operacion = cuenta->crearDeposito(getLastId(colaOperaciones), cuentaDestino, monto, canal->getId(), canal->getIdSede());
				if (operacion != nullptr) colaOperaciones->encolar(operacion);
			}
			system("pause");
			break;
		case 3:
			cout << "Su SALDO actual es: " << cuenta->getSaldo() << endl << endl;

			cout << "Retirar..." << endl;

			cout << "Ingrese el monto a retirar: ";

			cin >> monto;

			operacion = cuenta->crearRetiro(getLastId(colaOperaciones), cuentaOrigen, monto, canal->getId(), canal->getIdSede());
			if (operacion != nullptr) encolar<Cola<Operacion*>*, Operacion>(operacion, colaOperaciones);
			system("pause");
			break;
		case 4:
			return false;
			break;
		};

		//desencolar luego de cada encolamiento
		Operacion* opDesencolada = desencolar<Cola<Operacion*>*, Operacion>(colaOperaciones);
		if (opDesencolada != nullptr) {
			ejecutarOperacion(opDesencolada);
			opDesencolada->setId(getLastId(operaciones));
			agregar(opDesencolada, operaciones);
		}
		return true;
	};

	crearMenu(opciones, callback);
}

void Bcp::MenuCanales()
{
	vector<string> opciones = {
		"Listar todos los canales",
		"Listar los canales por tipo",
		"Listar los canales por estado",
		"Agregar un canal a una sede",
		"Editar un canal",
		"Eliminar un canal",
		"Activar un canal",
		"Desactivar un canal",
		"Salir"
	};

	int tipo=-1;
	int activo = -1;
	int id=0;

	auto callback = [this, &tipo, &activo, &id](int seleccion) {
		switch (seleccion) {
		case 0:
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(0, 12);canales->print();
			system("pause");
			break;
		case 1:
		{
			do{
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(40, 12); cout << "Ingrese el tipo de canal para buscar:";
				gotoxy(40, 13); cout << "(0: Otro, 1: Ventanilla, 2: Agente";
				gotoxy(40, 14); cout << "3: Web, 4: App, 5:Yape, 6: Cajero)";
				gotoxy(50, 15); cin >> tipo;
			} while (tipo < 0 || tipo>6);
			Nodo<Canal*>* aux = canales->getFirst();
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(0, 12);
			while(aux!=nullptr)
			{
				if (aux->data->getTipoDeCanal() == ETipoDeCanal(tipo))
				{
					aux->data->print();
				}
				aux = aux->next;
			}
			system("pause");
			break;
		}
		case 2:
		{
			do {
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(40, 12); cout << "Ingrese la sede de canal para buscar: ";
				gotoxy(40, 13); cout << "(0: Desactivado, 1: Activado)";
				gotoxy(40, 13); cin >> activo;
			} while (activo > 1 || activo < 0);
			Nodo<Canal*>* aux = canales->getFirst();
			gotoxy(0, 12);
			while(aux!=nullptr)
			{
				if (aux->data->isActivo()==bool(activo))
				{
					aux->data->print();
				}
				aux = aux->next;
			}
			system("pause");
			break;
		}
		case 3:
		{
			string nombre="";
			int id;
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el id de la sede";
			gotoxy(40, 13); cin >> id;
			Nodo<Sede*>* auxiliar_sede = buscarPorId<ListaDoble<Sede*>*, Sede>(id, sedes);
			if (auxiliar_sede == nullptr)
			{
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(40, 12); cout << "No se encontro la sede";
				gotoxy(40, 13); system("pause");
				break;
			}
			gotoxy(40, 12); cout << "Ingrese el nombre del canal";
			gotoxy(40, 13); cin >> nombre;
			do{
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(40, 12); cout << "Ingrese el tipo de canal para buscar:";
				gotoxy(40, 13); cout << "(0: Otro, 1: Ventanilla, 2: Agente";
				gotoxy(40, 14); cout << "3: Web, 4: App, 5:Yape, 6: Cajero)";
				gotoxy(50, 15); cin >> tipo;
			} while (tipo > 6 || tipo < 0);
			Canal* aux = new Canal(canales->getLastId(), nombre, tipo,auxiliar_sede->data->getId());
			agregar<ListaDoble<Canal*>*, Canal>(aux, canales);
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Canal agregado correctamente";
			gotoxy(40, 13); system("pause");
			break;
		}
		case 4:
		{
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el id del canal a editar";
			gotoxy(40, 13); cin >> id;
			Nodo<Canal*>* aux = buscarPorId<ListaDoble<Canal*>*,Canal>(id, canales);
			if (aux == nullptr)
			{
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(40, 12); cout << "No se encontro el canal";
				gotoxy(40, 13); system("pause");
				break;
			}
			vector<string> ops = { "Nombre", "Tipo de canal","Salir"};
			auto callback_ops = [&](int seleccion)
			{
				switch (seleccion)
				{
				case 0:
				{
					string auxiliar;
					Console::Clear();
					LogoBCP(18, 1);
					gotoxy(40, 12); cout << "Nombre anterior: " << aux->data->getNombre();
					gotoxy(40, 13); cout << "Nuevo nombre:"; cin >> auxiliar;
					editar(aux->data, canales);
					gotoxy(40, 15); cout << "Nombre cambiado correctamente";
					gotoxy(40, 16); system("pause");
					break;
				}
				case 1:
				{
					int auxiliar=-1;
					Console::Clear();
					do{
						LogoBCP(18, 1);
						gotoxy(40, 12); cout << "Ingrese el nuevo tipo de canal: ";
						gotoxy(40, 13); cout << "(0: Otro, 1: Ventanilla, 2: Agente";
						gotoxy(40, 14); cout << "3: Web, 4: App, 5:Yape, 6: Cajero)";
						gotoxy(40, 15); cin >> auxiliar;
					} while (auxiliar > 6 || auxiliar < 0);
					aux->data->setTipoDeCanal(ETipoDeCanal(auxiliar));
					editar(aux->data, canales);
					gotoxy(40, 17); cout << "Tipo de canal editado correctamente";
					gotoxy(40, 18); system("pause");
					break;
				}
				case 2:
					return false;
					break;
				}
				return true;
			};
			crearMenu(ops, callback_ops);
			break;
		}
		case 5:
		{
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el id del canal a eliminar";
			gotoxy(40, 13); cin >> id;
			Nodo<Canal*>* aux = buscarPorId<ListaDoble<Canal*>*, Canal>(id, canales);
			if (aux == nullptr)
			{
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(40, 12); cout << "No se encontro el canal";
				gotoxy(40, 13); system("pause");
				break;
			}
			eliminar(aux->data, canales);
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Canal eliminado correctamente";
			break;
		}
		case 6:
		{
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el id del canal a activar: ";
			gotoxy(40, 13); cin >> id;
			Nodo<Canal*>* aux = buscarPorId<ListaDoble<Canal*>*, Canal>(id, canales);
			if (aux == nullptr)
			{
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(40, 12); cout << "No se encontro el canal";
				gotoxy(40, 13); system("pause");
				break;
			}
			aux->data->activar();
			editar(aux->data, canales);
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Canal activado correctamente";
			gotoxy(40, 13); system("pause");
			break;
		}
		case 7:
		{
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el id del canal a activar: ";
			gotoxy(40, 13); cin >> id;
			Nodo<Canal*>* aux = buscarPorId<ListaDoble<Canal*>*, Canal>(id, canales);
			if (aux == nullptr)
			{
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(40, 12); cout << "No se encontro el canal";
				gotoxy(40, 13); system("pause");
				break;
			}
			aux->data->desactivar();
			editar(aux->data, canales);
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Canal desactivado correctamente";
			gotoxy(40, 13); system("pause");
			break;
		}
		case 8:
			return false;
			break;
		}
		return true;
		};

	crearMenu(opciones, callback);
}

void Bcp::MenuClientes()
{
	vector<string> opciones = {
		"Listar todos los Clientes",
		"Listar los clientes por nombre ascendente",
		"Listar los clientes por apellido ascendente",
		"Agregar un cliente",
		"Editar los datos de un cliente",
		"Buscar un Cliente por id",
		"Eliminar un cliente",
		"Salir"
	};

	int id = 0;

	auto callback = [this,&id](int seleccion) {
		switch(seleccion)
		{
			case 0:
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(0, 12); clientes->print();
				system("pause");
				break;
			case 1:
				break;
			case 2:
				break;
			case 3:
			{
				Console::Clear();
				string nombre, apellido, telefono, direccion, email, password;
				password = "0";
				//formulario registro de cliente:
				vector<string> camposRegistro = { "Nombre", "Apellido", "Direccion" };
				vector<string> camposRegistroTelefono = { "Telefono" };
				vector<string> camposRegistroEmail = { "Email" };
				vector<string> camposRegistroContrasenia = { "Contrasenia" };

				crearFormulario(camposRegistro, [&](const vector<string>& valores) -> bool {
					nombre = valores[0];
					apellido = valores[1];
					telefono = valores[2];
					return false;
					});
				crearFormulario(camposRegistroTelefono, [&](const vector<string>& valores) -> bool {
					bool repetido = false;

					Nodo<Cliente*>* cliente = clientes->searchByValue([&](Cliente* cliente) {
						return cliente->getTelefono() == valores[0];
						});
					repetido = cliente != nullptr;

					if (repetido)
					{
						gotoxy(45, 10); cout << "El telefono es repetido. Intente nuevamente";
						gotoxy(45, 12); system("pause");
						return true;
					}
					telefono = valores[0];
					return false;
					});
				crearFormulario(camposRegistroEmail, [&](const vector<string>& valores) -> bool {
					bool repetido = false;
					Nodo<Cliente*>* cliente = clientes->searchByValue([&](Cliente* cliente) {
						return cliente->getEmail() == valores[0];
						});
					repetido = cliente != nullptr;
					if (repetido)
					{
						gotoxy(45, 10); cout << "El email es repetido. Intente nuevamente";
						gotoxy(45, 12); system("pause");
						return true;
					}
					email = valores[0];
					return false;
					});
				crearFormulario(camposRegistroContrasenia, [&](const vector<string>& valores) -> bool {
					if (valores[0].size() != 6)
					{
						gotoxy(45, 10); cout << "La contrasenia debe de tener 6 digitos";
						gotoxy(43, 12); system("pause");
						return true;
					}
					password = valores[0];
					return false;
					});
				Cliente* cliente = new Cliente(this->getLastId(clientes), nombre, apellido, direccion, telefono, email, password);
				agregar(cliente, clientes);

				this->hashClientes->insertar(cliente->getId(), cliente);
				this->saveHash<HashTabla<Cliente*>*, Cliente>(hashClientes);

				gotoxy(45, 12); cout << LBLUE << "Cuenta Registrada Correctamente" << RESET;
				gotoxy(45, 14); cout << RED << system("pause") << RESET;
				Console::Clear();
				break;
			}
			case 4:
			{
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(40, 12); cout << "Ingrese el id del cliente";
				gotoxy(40, 13); cin >> id;
				Nodo<Cliente*>* aux_cliente = buscarPorId<ListaDoble<Cliente*>*, Cliente>(id, clientes);
				if (aux_cliente == nullptr)
				{
					Console::Clear();
					LogoBCP(18, 1);
					gotoxy(40, 12); cout << "No se encontro el cliente";
					gotoxy(40, 13); system("pause");
					break;
				}
				vector<string> opciones = { "Nombre", "Apellido", "Telefono", "Direccion", "Email", "Contrasenia", "Salir" };
				string aux;
				auto callback = [&](int opcion) {
					switch (opcion)
					{
					case 0:
						Console::Clear();
						LogoBCP(18, 1);
						gotoxy(40, 14); cout << "Nombre anterior: " << aux_cliente->data->getNombre();
						gotoxy(40, 15); cout << "Nuevo Nombre: "; cin >> aux;
						aux_cliente->data->setNombre(aux);
						editar(aux_cliente->data, clientes);
						gotoxy(40, 17); cout << "Nombre cambiado correctamente";
						gotoxy(40, 18); system("pause");
						Console::Clear();
						break;
					case 1:
						Console::Clear();
						LogoBCP(18, 1);
						gotoxy(40, 14); cout << "Apellido anterior: " << aux_cliente->data->getApellido();
						gotoxy(40, 15); cout << "Nuevo telefono: "; cin >> aux;
						aux_cliente->data->setApellido(aux);
						editar(aux_cliente->data, clientes);
						gotoxy(40, 17); cout << "Apellido cambiado correctamente";
						gotoxy(40, 18); system("pause");
						Console::Clear();
						break;
					case 2:
						Console::Clear();
						LogoBCP(18, 1);
						gotoxy(40, 14); cout << "Telefono anterior: " << aux_cliente->data->getTelefono();
						gotoxy(40, 15); cout << "Nuevo telefono: "; cin >> aux;
						aux_cliente->data->setTelefono(aux);
						editar(aux_cliente->data, clientes);
						gotoxy(40, 17); cout << "Telefono cambiado correctamente";
						gotoxy(40, 18); system("pause");
						Console::Clear();
						break;
					case 3:
						Console::Clear();
						LogoBCP(18, 1);
						gotoxy(40, 14); cout << "Direccion anterior: " << aux_cliente->data->getDireccion();
						gotoxy(40, 15); cout << "Nueva direccion: "; cin >> aux;
						aux_cliente->data->setDireccion(aux);
						editar(aux_cliente->data, clientes);
						gotoxy(40, 17); cout << "Direccion cambiada correctamente";
						gotoxy(40, 18); system("pause");
						Console::Clear();
						break;
					case 4:
						Console::Clear();
						LogoBCP(18, 1);
						gotoxy(40, 14); cout << "Email anterior: " << aux_cliente->data->getEmail();
						gotoxy(40, 15); cout << "Nuevo email: "; cin >> aux;
						aux_cliente->data->setEmail(aux);
						editar(aux_cliente->data, clientes);
						gotoxy(40, 17); cout << "Email cambiado correctamente";
						gotoxy(40, 18); system("pause");
						Console::Clear();
						break;
					case 5:
						Console::Clear();
						LogoBCP(18, 1);
						gotoxy(40, 14); cout << "Contrasenia anterior: " << aux_cliente->data->getPassword();
						gotoxy(40, 15); cout << "Nueva contrasenia: "; cin >> aux;
						aux_cliente->data->setPassword(aux);
						editar(aux_cliente->data, clientes);
						gotoxy(40, 17); cout << "Contrasenia cambiada correctamente";
						gotoxy(40, 18); system("pause");
						Console::Clear();
						break;
					case 6:
						return false;
						break;
					}
					return true;
					};
				crearMenu(opciones, callback);
				break;
			}
			case 5:
			{
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(40, 12); cout << "Ingrese el id del cliente";
				gotoxy(40, 13); cin >> id;
				Nodo<Cliente*>* aux = buscarPorId<ListaDoble<Cliente*>*, Cliente>(id, clientes);
				if (aux == nullptr)
				{
					Console::Clear();
					LogoBCP(18, 1);
					gotoxy(40, 12); cout << "No se encontro el cliente";
					gotoxy(40, 13); system("pause");
					break;
				}
				aux->data->print();
				system("pause");
				break;
			}
			case 6:
			{
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(40, 12); cout << "Ingrese el id del cliente a eliminar";
				gotoxy(40, 13); cin >> id;
				Nodo<Cliente*>* aux = buscarPorId<ListaDoble<Cliente*>*, Cliente>(id, clientes);
				if (aux == nullptr)
				{
					Console::Clear();
					LogoBCP(18, 1);
					gotoxy(40, 12); cout << "No se encontro el cliente";
					gotoxy(40, 13); system("pause");
					break;
				}
				Nodo<CuentaBancaria*>* auxCB = cuentas->getFirst();
				while(auxCB!=nullptr)
				{
					if (auxCB->data->getIdCliente() == aux->data->getId())
					{
						eliminar<ListaDoble<Tarjeta*>*, Tarjeta>(auxCB->data->getTarjeta(), tarjetas);
						eliminar<ListaDoble<CuentaBancaria*>*, CuentaBancaria>(auxCB->data, cuentas);
					}
					auxCB = auxCB->next;
				}
				eliminar<ListaDoble<Cliente*>*, Cliente>(aux->data, clientes);
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(40, 12); cout << "Cliente eliminado correctamente";
				gotoxy(40, 13); system("pause");
				break;
			}
			case 7:
				return false;
				break;
		}	
		return true;
	};

	crearMenu(opciones, callback);
}

void Bcp::MenuOperaciones()
{
	vector<string> opciones = {
		"Listar todas las Operaciones",
		"Realizar una operacion",
		"Buscar una Operacion por id",
		"Eliminar una operacion",
		"Salir"
	};

	int id = 0;

	auto callback = [this,&id](int seleccion) {
		switch (seleccion)
		{
		case 0:
			operaciones->print();
			system("pause");
			break;
		case 1:
			//
			break;
		case 2:
		{
			gotoxy(40, 12); cout << "Ingrese el id de la operacion";
			gotoxy(40, 13); cin >> id;
			Nodo<Operacion*>* aux = buscarPorId<ListaDoble<Operacion*>*, Operacion>(id, operaciones);
			if (aux == nullptr)
			{
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(40, 12); cout << "No se encontro la operacion";
				gotoxy(40, 13); system("pause");
				break;
			}
			aux->data->print();
			system("pause");
			break;
		}
		case 3:
		{
			gotoxy(40, 12); cout << "Ingrese el id de la operacion a eliminar";
			gotoxy(40, 13); cin >> id;
			Nodo<Operacion*>* aux = buscarPorId<ListaDoble<Operacion*>*, Operacion>(id, operaciones);
			if (aux == nullptr)
			{
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(40, 12); cout << "No se encontro la operacion";
				gotoxy(40, 13); system("pause");
				break;
			}
			eliminar(aux->data, operaciones);
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Operacion eliminada correctamente";
			gotoxy(40, 13); system("pause");
			break;
		}
		case 4:
			return false;
			break;
		}
		return true;
	};

	crearMenu(opciones, callback);
}

void Bcp::MenuCuentas()
{
	vector<string> opciones = {
		"Listar todas las Cuentas Bancarias",
		"Agregar una Cuenta Bancaria a un Cliente",
		"Editar los datos de una Cuenta Bancaria",
		"Buscar una Cuenta Bancaria por id",
		"Eliminar una Cuenta Bancaria",
		"Salir"
	};

	int id = 0;

	auto callback = [this,&id](int seleccion) {
		switch (seleccion) {
		case 0:
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(0, 12); cuentas->print();
			system("pause");
			break;
		case 1:
		{
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el id del cliente";
			gotoxy(40, 13); cin >> id;
			Nodo<Cliente*>* aux = buscarPorId<ListaDoble<Cliente*>*, Cliente>(id, clientes);
			if (aux == nullptr)
			{
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(40, 12); cout << "No se encontro el cliente";
				gotoxy(40, 13); system("pause");
				break;
			}
			string numero_cuenta;//14 digitos
			string contrasenia;
			bool verificacion;
			do
			{
				numero_cuenta = "";
				verificacion = false;
				Nodo<CuentaBancaria*>* aux_verif = this->cuentas->getFirst();
				for (int i = 0; i < 14; i++)
				{
					Random r;
					numero_cuenta.append(to_string(r.Next(0, 9)));
					_sleep(100);
				}
				while(aux_verif!=nullptr)
				{
					if (numero_cuenta == aux_verif->data->getNumeroCuenta())
					{
						verificacion = true;
					}
					aux_verif = aux_verif->next;
				}
			} while (verificacion);
			do
			{
				gotoxy(30, 12); cout << "Ingrese la contrasenia para la cuenta (4 digitos)";
				gotoxy(50, 14); cin >> contrasenia;
				if (contrasenia.size() != 4)
				{
					Console::Clear();
					LogoBCP(18, 1);
				}
			} while (contrasenia.size() != 4);
			CuentaBancaria* nuevaCuentaBancaria = new CuentaBancaria(this->cuentas->getLastId(), aux->data->getId(), contrasenia, numero_cuenta, 0);
			this->cuentas->push_back(nuevaCuentaBancaria);
			aux->data->agregarCuentaBancaria(nuevaCuentaBancaria);
			agregar(nuevaCuentaBancaria, cuentas);
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Cuenta bancaria agregada correctamente";
			gotoxy(40, 14); system("pause");
			Console::Clear();
			break;
		}
		case 2:
		{
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el id del cliente";
			gotoxy(40, 13); cin >> id;
			Nodo<CuentaBancaria*>* aux = buscarPorId<ListaDoble<CuentaBancaria*>*, CuentaBancaria>(id, cuentas);
			if (aux == nullptr)
			{
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(40, 12); cout << "No se encontro el cliente";
				gotoxy(40, 13); system("pause");
				break;
			}
			Console::Clear();
			vector<string> ops = { "Contrasenia","Saldo","Salir"};
			int aux_int=0;
			auto callback_ops = [&](int seleccion)
				{
					switch (seleccion)
					{
					case 0:
					{
						string aux_str = "";
						do {
							Console::Clear();
							LogoBCP(18, 1);
							gotoxy(40, 12); cout << "Ingrese la nueva contrasenia";
							gotoxy(40, 13); cin >> aux_str;
						} while (aux_str.length() != 4);
						aux->data->setPassword(aux_str);
						editar(aux->data, cuentas);
						Console::Clear();
						LogoBCP(18, 1);
						gotoxy(40, 12); cout << "Contasenia cambiada correctamente";
						gotoxy(40, 13); system("pause");
						break;
					}
					case 1:
					{
						double saldo = 0;
						gotoxy(40, 12); cout << "Ingrese la cantidad de saldo a sumar en la cuenta";
						gotoxy(40, 13); cin >> saldo;
						aux->data->setSaldo(aux->data->getSaldo() + saldo);
						Console::Clear();
						LogoBCP(18, 1);
						gotoxy(40, 12); cout << "Saldo aumentado en " << saldo << " correctamente";
						gotoxy(40, 13); system("pause");
						break;
					}
					case 2:
						return false;
						break;
					}
					return true;
				};
			crearMenu(ops, callback_ops);
			break;
		}
		case 3:
		{
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el id de la cuenta a buscar";
			gotoxy(40, 13); cin >> id;
			if (buscarPorId<ListaDoble<CuentaBancaria*>*, CuentaBancaria>(id, cuentas) == nullptr)
			{
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(40, 12); cout << "No se encontro la cuenta bancaria";
				gotoxy(40, 13); system("pause");
				break;
			}
			buscarPorId<ListaDoble<CuentaBancaria*>*, CuentaBancaria>(id, cuentas)->data->print();
			break;
		}
		case 4:
		{
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el id de la cuenta a eliminar";
			gotoxy(40, 13); cin >> id;
			Nodo<CuentaBancaria*>* aux = buscarPorId<ListaDoble<CuentaBancaria*>*,CuentaBancaria>(id, cuentas);
			if (aux == nullptr)
			{
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(40, 12); cout << "No se encontro la cuenta bancaria";
				gotoxy(40, 13); system("pause");
				break;
			}
			eliminar(aux->data->getTarjeta(), tarjetas);
			eliminar(aux->data, cuentas);
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Cuenta eliminada correctamente";
			gotoxy(40, 13); system("pause");
			break;
		}
		case 5:
			return false;
			break;
		}
		return true;
	};

	crearMenu(opciones, callback);
}

void Bcp::MenuTarjetas()
{
	vector<string> opciones = {
		"Listar todas las Tarjetas",
		"Agregar una Tarjeta a una Cuenta Bancaria",
		"Editar CVV de una Tarjeta",
		"Buscar una Tarjeta por id",
		"Eliminar una Tarjeta",
		"Salir"
	};

	int id = 0;
	int cvv = 0;

	auto callback = [this,&id](int seleccion) {
		switch (seleccion)
		{
		case 0:
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(0, 12); tarjetas->print();
			system("pause");
			break;
		case 1:
		{
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el id del Cliente";
			gotoxy(40, 13); cin >> id;
			Nodo<Cliente*>* aux = buscarPorId<ListaDoble<Cliente*>*, Cliente>(id, clientes);
			if (aux != nullptr)
			{
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(40, 12); cout << "Ingrese el id de la Cuenta Bancaria al que desee agregar una Tarjeta";
				gotoxy(40, 13); cin >> id;
				Nodo<CuentaBancaria*>* aux_CB = aux->data->getCuentasBancarias()->search(id);
				if (aux_CB != nullptr)
				{
					aux_CB->data->loadTarjeta();
					if (aux_CB->data->getTarjeta() == nullptr)
					{
						Tarjeta* auxiliar = new Tarjeta(this->tarjetas->getLastId(), aux_CB->data->getIdCliente(), aux->data->getId());
						this->tarjetas->push_back(auxiliar);
						aux_CB->data->setTarjeta(auxiliar);
						agregar(auxiliar, tarjetas);
						gotoxy(40, 12); cout << "Tarjeta aniadida correctamente";
						gotoxy(40, 13); system("pause");
					}
					else
					{
						Console::Clear();
						LogoBCP(18, 1);
						gotoxy(40, 12); cout << "Esta cuenta ya tiene una tarjeta";
						gotoxy(40, 13); system("pause");
					}
				}
				else
				{
					Console::Clear();
					LogoBCP(18, 1);
					gotoxy(40, 12); cout << "No se encontro la Cuenta Bancaria del cliente";
					gotoxy(40, 13); system("pause");
				}
			}
			else
			{
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(40, 12); cout << "No se encontro el id del cliente";
				gotoxy(40, 13); system("pause");
			}
			break;
		}
		case 2:
		{
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el id de la tarjeta a editar";
			gotoxy(40, 13); cin >> id;
			Nodo<Tarjeta*>* aux = buscarPorId<ListaDoble<Tarjeta*>*, Tarjeta>(id, tarjetas);
			Console::Clear();
			LogoBCP(18, 1);
			if (aux == nullptr)
			{
				gotoxy(40, 12); cout << "No se encontro la tarjeta";
				gotoxy(40, 13); system("pause");
				break;
			}
			Random r(time(NULL));
			aux->data->setCvv(to_string(r.Next(101,1000)-1));
			editar<ListaDoble<Tarjeta*>*, Tarjeta>(aux->data, tarjetas);
			gotoxy(40, 12); cout << "Cvv de la tarjeta editada correctamente";
			gotoxy(40, 13); system("pause");
			break;
		}
		case 3:
		{
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el id de la tarjeta";
			gotoxy(40, 13); cin >> id;
			buscarPorId<ListaDoble<Tarjeta*>*, Tarjeta>(id, tarjetas)->data->print();
			system("pause");
			break;
		}
		case 4:
		{
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el id de la tarjeta a eliminar";
			gotoxy(40, 13); cin >> id;
			Nodo<Tarjeta*>* aux = buscarPorId<ListaDoble<Tarjeta*>*, Tarjeta>(id, tarjetas);
			if (aux == nullptr) {
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(40, 12); cout << "No se encontro la tarjeta";
				gotoxy(40, 13); system("pause");
				break;
			}
			Tarjeta* aux_tarjeta = buscarPorId<ListaDoble<CuentaBancaria*>*, CuentaBancaria>(aux->data->getIdCuentaBancaria(), cuentas)->data->getTarjeta();
			delete aux_tarjeta;
			buscarPorId<ListaDoble<CuentaBancaria*>*, CuentaBancaria>(aux->data->getIdCuentaBancaria(), cuentas)->data->setTarjeta(nullptr);
			eliminar<ListaDoble<Tarjeta*>*, Tarjeta>(aux->data, tarjetas);
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Tarjeta eliminada correctamente";
			gotoxy(40, 13); system("pause");
			break;
		}
		case 5:
			return false;
			break;
		}
		return true;
	};

	crearMenu(opciones, callback);
}

void Bcp::MenuAdmin()
{
	vector<string> opciones = {
		"Lista de Clientes",
		"Lista de Cuentas Bancarias",
		"Lista de Tarjetas",
		"Lista de Operaciones",
		"Lista de Canales",
		"Lista de Sedes",
		"Salir"
	};

	auto callback = [this](int seleccion) {
		switch (seleccion) {
		case 0:
			MenuClientes();
			break;
		case 1:
			MenuCuentas();
			break;
		case 2:
			MenuTarjetas();
			break;
		case 3:
			MenuOperaciones();
			break;
		case 4:
			MenuCanales();
			break;
		case 5:
			MenuSedes();
			break;
		case 6:
			return false;
			break;
		}
		return true;
	};

	crearMenu(opciones, callback);
}

void Bcp::MenuSedes()
{
	vector<string> opciones = {
		"Listar todas las sedes",
		"Listar las sedes por ciudad",
		"Listar las sedes por departamento",
		"Listar las sedes por distrito",
		"Agregar una sede",
		"Editar una sede",
		"Eliminar una sede",
		"Activar una sede",
		"Desactivar una sede",
		"Salir"
	};
	string ciudad = "";
	string departamento = "";
	string distrito = "";
	int id;

	auto callback = [this, &ciudad, &departamento, &id, &distrito](int seleccion) {
		switch (seleccion) {
		case 0:
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(0, 12); sedes->print();
			system("pause");
			break;
		case 1:
		{
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese la ciudad: ";
			gotoxy(40, 13); cin >> ciudad;
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(0, 12);
			Nodo<Sede*>* aux = sedes->getFirst();
			while( aux != nullptr)
			{
				if (aux->data->getCiudad() == ciudad)
				{
					aux->data->print();
				}
				aux = aux->next;
			}
			system("pause");
			break;
		}
		case 2:
		{
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el departamento: ";
			gotoxy(40, 13); cin >> departamento;
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(0, 12);
			Nodo<Sede*>* aux = sedes->getFirst();
			while(aux!=nullptr)
			{
				if (aux->data->getCiudad() == departamento)
				{
					aux->data->print();
				}
				aux = aux->next;
			}
			system("pause");
			break;
		}
		case 3:
		{
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el distrito: ";
			gotoxy(40, 13); cin >> distrito;
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(0, 12);
			Nodo<Sede*>* aux = sedes->getFirst();
			while(aux!=nullptr)
			{
				if (aux->data->getCiudad() == distrito)
				{
					aux->data->print();
				}
				aux = aux->next;
			}
			system("pause");
			break;
		}
		case 4:
		{
			string nombre, direccion, tlf, email;
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el nombre";
			gotoxy(40, 13); cin >> nombre;
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese la direccion";
			gotoxy(40, 13); cin >> direccion;
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese la ciudad";
			gotoxy(40, 13); cin >> ciudad;
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el distrito";
			gotoxy(40, 13); cin >> distrito;
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el departamento";
			gotoxy(40, 13); cin >> departamento;
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el telefono";
			gotoxy(40, 13); cin >> tlf;
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el email";
			gotoxy(40, 13); cin >> email;
			agregar<ListaDoble<Sede*>*, Sede>(new Sede(getLastId(sedes), "nueva sede", "Calle las Amapolas", "lima", "Lince", "Lima", "99857463", "nuevaSede@email.com"), sedes);
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Sede agregada correctamente";
			gotoxy(40, 13); system("pause");
			break;
		}
		case 5:
		{
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el id de la sede a editar";
			gotoxy(40, 13); cin >> id;
			if (buscarPorId<ListaDoble<Sede*>*, Sede>(id, sedes) == nullptr)
			{
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(40, 12); cout << "No se encontro la sede";
				gotoxy(40, 13); system("pause");
				break;
			}
			Nodo<Sede*>* aux_sede = buscarPorId<ListaDoble<Sede*>*, Sede>(id, sedes);
			vector<string> ops = { "Nombre","Direccion","Ciudad","Distrito","Departamento","Telefono","Email","Salir" };
			string aux;
			auto callback_ops = [&](int seleccion)
				{
					switch (seleccion)
					{
					case 0:
						Console::Clear();
						LogoBCP(18, 1);
						gotoxy(40, 12); cout << "Ingrese el nuevo nombre";
						gotoxy(40, 13); cin >> aux;
						aux_sede->data->setNombre(aux);
						Console::Clear();
						LogoBCP(18, 1);
						gotoxy(40, 12); cout << "Nombre cambiado correctamente";
						gotoxy(40, 13); system("pause");
						break;
					case 1:
						Console::Clear();
						LogoBCP(18, 1);
						gotoxy(40, 12); cout << "Ingrese la nueva direccion";
						gotoxy(40, 13); cin >> aux;
						aux_sede->data->setDireccion(aux);
						Console::Clear();
						LogoBCP(18, 1);
						gotoxy(40, 12); cout << "Direccion cambiada correctamente";
						gotoxy(40, 13); system("pause");
						break;
					case 2:
						Console::Clear();
						LogoBCP(18, 1);
						gotoxy(40, 12); cout << "Ingrese la nueva ciudad";
						gotoxy(40, 13); cin >> aux;
						aux_sede->data->setCiudad(aux);
						Console::Clear();
						LogoBCP(18, 1);
						gotoxy(40, 12); cout << "Ciudad cambiada correctamente";
						gotoxy(40, 13); system("pause");
						break;
					case 3:
						Console::Clear();
						LogoBCP(18, 1);
						gotoxy(40, 12); cout << "Ingrese el nuevo distrito";
						gotoxy(40, 13); cin >> aux;
						aux_sede->data->setDistrito(aux);
						Console::Clear();
						LogoBCP(18, 1);
						gotoxy(40, 12); cout << "Distrito cambiado correctamente";
						gotoxy(40, 13); system("pause");
						break;
					case 4:
						Console::Clear();
						LogoBCP(18, 1);
						gotoxy(40, 12); cout << "Ingrese el nuevo departamento";
						gotoxy(40, 13); cin >> aux;
						aux_sede->data->setDepartamento(aux);
						Console::Clear();
						LogoBCP(18, 1);
						gotoxy(40, 12); cout << "Departamento cambiado correctamente";
						gotoxy(40, 13); system("pause");
						break;
					case 5:
						Console::Clear();
						LogoBCP(18, 1);
						gotoxy(40, 12); cout << "Ingrese el nuevo telefono";
						gotoxy(40, 13); cin >> aux;
						aux_sede->data->setTelefono(aux);
						Console::Clear();
						LogoBCP(18, 1);
						gotoxy(40, 12); cout << "Telefono cambiado correctamente";
						gotoxy(40, 13); system("pause");
						break;
					case 6:
						Console::Clear();
						LogoBCP(18, 1);
						gotoxy(40, 12); cout << "Ingrese el nuevo email";
						gotoxy(40, 13); cin >> aux;
						aux_sede->data->setEmail(aux);
						Console::Clear();
						LogoBCP(18, 1);
						gotoxy(40, 12); cout << "Email cambiado correctamente";
						gotoxy(40, 13); system("pause");
						break;
					case 7:
						return false;
						break;
					}
					return true;
				};
			crearMenu(ops, callback_ops);
			editar(aux_sede->data, sedes);
			break;
		}
		case 6:
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el id de la sede a eliminar";
			gotoxy(40, 13); cin >> id;
			if (buscarPorId<ListaDoble<Sede*>*, Sede>(id, sedes) == nullptr)
			{
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(40, 12); cout << "No se encontro la sede";
				gotoxy(40, 13); system("pause");
				break;
			}
			eliminar<ListaDoble<Sede*>*, Sede>(buscarPorId<ListaDoble<Sede*>*, Sede>(id, sedes)->data, sedes);
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Sede eliminada";
			gotoxy(40, 13); system("pause");
			break;
		case 7:
			Console::Clear();
			LogoBCP(18, 1);
			cout << "Ingrese el id de la sede a activar: ";
			cin >> id;
			if (buscarPorId<ListaDoble<Sede*>*, Sede>(id, sedes) == nullptr)
			{
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(40, 12); cout << "No se encontro la sede";
				gotoxy(40, 13); system("pause");
				break;
			}
			buscarPorId<ListaDoble<Sede*>*, Sede>(id, sedes)->data->activar();
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Sede activada";
			gotoxy(40, 13); system("pause");
			break;
		case 8:
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el id de la sede a desactivar: ";
			gotoxy(40, 13); cin >> id;
			if (buscarPorId<ListaDoble<Sede*>*, Sede>(id, sedes) == nullptr)
			{
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(40, 12); cout << "No se encontro la sede";
				gotoxy(40, 13); system("pause");
				break;
			}
			buscarPorId<ListaDoble<Sede*>*, Sede>(id, sedes)->data->desactivar();
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Sede desactivada";
			gotoxy(40, 13); system("pause");
			break;
		case 9:
			return false;
			break;
		}
		return true;
		};

	crearMenu(opciones, callback);
}

template <typename T>
void Bcp::crearMenu(const vector<string>& opciones, T callback) {
	int seleccion = 0;
	char tecla;
	bool continuar = true;

	do {
		Console::Clear();
		LogoBCP(18, 1);

		int screenWidth = 120; // Ancho de la pantalla (puedes ajustar esto según sea necesario)
		int screenHeight = 70; // Alto de la pantalla (puedes ajustar esto según sea necesario)
		int menuHeight = opciones.size();
		int startY = 10;

		for (size_t i = 0; i < opciones.size(); ++i) {
			int startX = (screenWidth - opciones[i].length() - 4) / 2; // -4 para "> " y " <"
			gotoxy(startX, startY + i);
			if (i == seleccion) {
				cout << "> " << opciones[i] << " <" << endl;
			}
			else {
				cout << "  " << opciones[i] << endl;
			}
		}

		tecla = _getch();

		switch (tecla) {
		case 72: // Flecha arriba
			seleccion = (seleccion - 1 + opciones.size()) % opciones.size();
			break;
		case 80: // Flecha abajo
			seleccion = (seleccion + 1) % opciones.size();
			break;
		case 27: // Esc
			continuar = false;
			break;
		case 13: // Enter
			continuar = callback(seleccion);
			break;
		}
	} while (continuar);
}

template<typename T>
void Bcp::crearFormulario(const vector<string>& campos, T callback) {
	vector<string> valores(campos.size());
	bool continuar = true;

	while (continuar) {
		system("cls");
		LogoBCP(18, 1);
		int screenWidth = 120; // Ancho de la pantalla (puedes ajustar esto según sea necesario)
		int screenHeight = 70; // Alto de la pantalla (puedes ajustar esto según sea necesario)
		int menuHeight = campos.size();
		int startY = 10;

		for (size_t i = 0; i < campos.size(); ++i) {
			int startX = (screenWidth - campos[i].length()) / 2;
			gotoxy(startX, startY + i);
			cout << campos[i] << ": ";
			cin >> valores[i];
		}

		continuar = callback(valores);
	}
}

template<class T>
void Bcp::QuickSort(ListaDoble<T>* lista, function<bool(T, T)> comp, int inicio, int fin)
{
	if (inicio < fin)
	{
		int pivote = particion_QS(lista, comp, inicio, fin);
		QuickSort<T>(lista, comp, inicio, pivote - 1);
		QuickSort<T>(lista, comp, pivote + 1, fin);
	}
}

template<class T>
int Bcp::particion_QS(ListaDoble<T>* lista, function<bool(T, T)> comp, int inicio, int fin)
{
	Nodo<T>* pivote = lista->getByPosition(fin - 1);
	int i = inicio - 1;
	for (int j = inicio; j < fin; j++)
	{
		if (comp(lista->getByPosition(j)->data, pivote->data))
		{
			i++;
			T* aux = lista->getByPosition(j)->data;
			lista->getByPosition(j)->data = lista->getByPosition(i)->data;
			lista->getByPosition(i)->data = aux;
		}
	}
	T* aux = lista->getByPosition(fin)->data;
	lista->getByPosition(fin)->data = lista->getByPosition(i + 1)->data;
	lista->getByPosition(i + 1)->data = aux;
}
