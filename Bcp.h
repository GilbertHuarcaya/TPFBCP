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
		hashClientes = new HashTabla<Cliente*>();
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

	void loadAll();

	template <class HT, class E>
	void saveHash(HT hash);

	template<typename T>
	void crearMenu(const vector<string>& opciones, T callback);

	ListaDoble<CuentaBancaria*>* buscarCuentaPorIdCliente(int idCliente);

	ListaDoble<Canal*>* buscarCanalesSinSede();

	void ejecutarOperacion(Operacion* operacion);

	//Menu y relacionados
	void LogoBCP(int x, int y);

	void MenuBCP();

	void MenuSoloCliente(Cliente* cliente);

	void MenuSoloCuentaBancaria(CuentaBancaria* CuentaB);

	void MenuElegirCanalOSede(CuentaBancaria* cuenta);

	void MenuOperacionPorSede(Sede* sede, CuentaBancaria* cuenta);

	void MenuOperacionPorCanal(Canal* canal, CuentaBancaria* cuenta);

	void MenuOperacionPorCliente(Cliente* cliente);

	void MenuOperacionPorCuentaBancaria(CuentaBancaria* cuenta);

	void MenuOperacionPorTarjeta(Tarjeta* tarjeta);

	void MenuAdmin();

	void MenuOperaciones();

	void MenuCuentas();

	void MenuTarjetas();

	void MenuSedes();

	void MenuCanales();

	void MenuClientes();


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

void Bcp::loadAll()
{
	load<ListaDoble<Cliente*>*, Cliente>(clientes);
	load<ListaDoble<CuentaBancaria*>*, CuentaBancaria>(cuentas);
	load<ListaDoble<Tarjeta*>*, Tarjeta>(tarjetas);
	load<ListaDoble<Operacion*>*, Operacion>(operaciones);
	load<ListaDoble<Canal*>*, Canal>(canales);
	load<ListaDoble<Sede*>*, Sede>(sedes);
}

template <class HT, class E>
void Bcp::saveHash(HT hash)
{
	File<HT, E>::escribirHash("hashTabla.csv", hash);
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

	if (cuentaBancariaOrigen != nullptr) {

	}
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
	System::Console::CursorVisible = false;
	int x_selec, y_selec, op;
	y_selec = 10;
	x_selec = 50;
	op = 1;
	while (1)
	{
		LogoBCP(18, 1);
		gotoxy(50, 10); cout << "Iniciar Sesion";
		gotoxy(50, 12); cout << "Registrarse";
		gotoxy(50, 14); cout << "Salir";
		gotoxy(x_selec + 17, y_selec); cout << ORANGE << "<" << RESET;
		switch (char c = toupper(_getch()))
		{
		case 80://Abajo
			switch (op)
			{
			case 1:
				gotoxy(x_selec + 17, y_selec); cout << " ";
				op++;
				y_selec += 2;
				break;
			case 2:
				gotoxy(x_selec + 17, y_selec); cout << " ";
				op++;
				y_selec += 2;
				break;
			default:
				break;
			}
			break;
		case 72://Arriba
			switch (op)
			{
			case 2:
				gotoxy(x_selec + 17, y_selec); cout << " ";
				op--;
				y_selec -= 2;
				break;
			case 3:
				gotoxy(x_selec + 17, y_selec); cout << " ";
				op--;
				y_selec -= 2;
				break;
			default:
				break;
			}
			break;
		case 13://Enter
			switch (op)
			{
			case 1:
			{
				Console::Clear();
				bool encontrado = false;
				string correo, contrasenia;
				LogoBCP(18, 1);
				gotoxy(45, 10); cout << "Correo: "; cin >> correo;
				gotoxy(45, 12); cout << "Contrasenia: "; cin >> contrasenia;
				Nodo<Cliente*>* Clientes = this->clientes->getFirst();
				Nodo<Cliente*>* ClienteEncontrado = nullptr;
				while (Clientes != nullptr)
				{
					if (Clientes->data->getEmail() == correo && Clientes->data->getPassword() == contrasenia)
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
					gotoxy(47, 14); system("pause");
					Console::Clear();
					break;
				}
				Console::Clear();
				if (ClienteEncontrado->data->getEmail() == "admin@bcp.pe") {
					//AGREGAR MENU DE ADMINISTRACION / GESTION DE TODAS LAS TABLAS/LISTAS
				}
				MenuSoloCliente(ClienteEncontrado->data);
				break;
			}
			case 2:
			{
				Console::Clear();
				Nodo<Cliente*>* Clientes = this->clientes->getFirst();
				string nombre, apellido, telefono, direccion, email, password;
				password = "0";
				bool repetido = false;
				LogoBCP(18, 1);
				gotoxy(50, 10); cout << "Ingrese su nombre:";
				gotoxy(55, 12); cin >> nombre;
				gotoxy(50, 10); cout << "                   ";
				gotoxy(55, 12); cout << "                        ";
				gotoxy(50, 10); cout << "Ingrese su apellido:";
				gotoxy(55, 12); cin >> apellido;
				gotoxy(50, 10); cout << "                     ";
				gotoxy(55, 12); cout << "                        ";
				cin.ignore(100, '\n');
				gotoxy(50, 10); cout << "Ingrese su direccion:";
				gotoxy(50, 12); getline(cin, direccion);
				gotoxy(50, 10); cout << "                      ";
				gotoxy(50, 12); cout << "                        ";
				gotoxy(50, 10); cout << "Ingrese su telefono:";
				gotoxy(55, 12); cin >> telefono;
				do
				{
					repetido = false;
					gotoxy(50, 10); cout << "                    ";
					gotoxy(55, 12); cout << "                        ";
					gotoxy(50, 10); cout << "Ingrese su email:";
					gotoxy(50, 12); cin >> email;
					cin.ignore(100, '\n');
					gotoxy(50, 10); cout << "                 ";
					gotoxy(50, 12); cout << "                          ";
					do
					{
						gotoxy(40, 10); cout << "Ingrese la contrasenia (6 digitos):";
						gotoxy(50, 12); cin >> password;
						gotoxy(40, 10); cout << "                                   ";
						gotoxy(50, 12); cout << "               ";
						if (password.size() != 6)
						{
							gotoxy(45, 10); cout << "La contrasenia debe de tener 6 digitos";
							gotoxy(43, 12); system("pause");
							gotoxy(45, 10); cout << "                                      ";
							gotoxy(43, 12); cout << "                                            ";
						}
					} while (password.size() != 6);
					while (Clientes != nullptr)
					{
						if (Clientes->data->getEmail() == email)
						{
							repetido = true;
						}
						Clientes = Clientes->next;
					}
					if (repetido)
					{
						gotoxy(45, 10); cout << "El email es repetido. Intente nuevamente";
						gotoxy(45, 12); system("pause");
						gotoxy(45, 10); cout << "                                        ";
						gotoxy(45, 12); cout << "                                             ";
					}
				} while (repetido);
				Cliente* cliente = new Cliente(this->getLastId(clientes), nombre, apellido, direccion, telefono, email, password);
				agregar(cliente, clientes);
				this->hashClientes->insertar(cliente->getId(), cliente);
				this->saveHash<HashTabla<Cliente*>*, Cliente>(hashClientes);
				gotoxy(45, 12); cout << LBLUE << "Cuenta Registrada Correctamente" << RESET;
				gotoxy(45, 14); cout << RED << system("pause") << RESET;
				Console::Clear();
				break;
			}
			case 3:
			{
				System::Console::Clear();
				exit(1);
				break;
			}
			}
		}
	}
}

void Bcp::MenuSoloCliente(Cliente* cliente)
{

	vector<string> opciones = { "Ver mis Datos", "Modificar uno de mis datos", "Ver mis Cuentas Bancarias", "Agregar una Cuenta Bancaria Random", "Acceder a mi Cuenta Bancaria", "Eliminar una de mis Cuentas Bancarias", "Salir" };

	auto callback = [this, cliente](int opcion, bool salir) {
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
			Console::Clear();
			string aux;
			int x_selec_dos, y_selec_dos;
			int op_2 = 1;
			bool bucle_menu_dos = true;
			x_selec_dos = 54;
			y_selec_dos = 14;
			while (bucle_menu_dos)
			{
				LogoBCP(18, 1);
				gotoxy(40, 12); cout << "Que dato desea modificar?";
				gotoxy(40, 14); cout << "Nombre";
				gotoxy(40, 15); cout << "Apellido";
				gotoxy(40, 16); cout << "Telefono";
				gotoxy(40, 17); cout << "Direccion";
				gotoxy(40, 18); cout << "Email";
				gotoxy(40, 19); cout << "Contrasenia";
				gotoxy(40, 20); cout << "Salir";
				gotoxy(x_selec_dos, y_selec_dos); cout << YELLOW << "<" << RESET;
				switch (char c = toupper(_getch()))
				{
				case 80://Abajo
					switch (op_2)
					{
					case 1:
						gotoxy(x_selec_dos, y_selec_dos); cout << " ";
						op_2++;
						y_selec_dos++;
						break;
					case 2:
						gotoxy(x_selec_dos, y_selec_dos); cout << " ";
						op_2++;
						y_selec_dos++;
						break;
					case 3:
						gotoxy(x_selec_dos, y_selec_dos); cout << " ";
						op_2++;
						y_selec_dos++;
						break;
					case 4:
						gotoxy(x_selec_dos, y_selec_dos); cout << " ";
						op_2++;
						y_selec_dos++;
						break;
					case 5:
						gotoxy(x_selec_dos, y_selec_dos); cout << " ";
						op_2++;
						y_selec_dos++;
						break;
					case 6:
						gotoxy(x_selec_dos, y_selec_dos); cout << " ";
						op_2++;
						y_selec_dos++;
						break;
					default:
						break;
					}
					break;
				case 72://Arriba
					switch (op_2)
					{
					case 2:
						gotoxy(x_selec_dos, y_selec_dos); cout << " ";
						op_2--;
						y_selec_dos--;
						break;
					case 3:
						gotoxy(x_selec_dos, y_selec_dos); cout << " ";
						op_2--;
						y_selec_dos--;
						break;
					case 4:
						gotoxy(x_selec_dos, y_selec_dos); cout << " ";
						op_2--;
						y_selec_dos--;
						break;
					case 5:
						gotoxy(x_selec_dos, y_selec_dos); cout << " ";
						op_2--;
						y_selec_dos--;
						break;
					case 6:
						gotoxy(x_selec_dos, y_selec_dos); cout << " ";
						op_2--;
						y_selec_dos--;
						break;
					case 7:
						gotoxy(x_selec_dos, y_selec_dos); cout << " ";
						op_2--;
						y_selec_dos--;
						break;
					default:
						break;
					}
					break;
				case 13://Enter
					switch (op_2)
					{
					case 1:
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
					case 2:
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
					case 3:
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
					case 4:
						Console::Clear();
						LogoBCP(18, 1);
						cin.ignore(100, '\n');
						gotoxy(40, 14); cout << "Direccion anterior: " << cliente->getEmail();
						gotoxy(40, 15); cout << "Nueva Direccion: "; getline(cin, aux);
						cliente->setDireccion(aux);
						editar(cliente, clientes);
						gotoxy(40, 17); cout << "Direccion cambiada correctamente";
						gotoxy(40, 18); system("pause");
						Console::Clear();
						break;
					case 5:
						Console::Clear();
						LogoBCP(18, 1);
						gotoxy(40, 14); cout << "Email anterior: " << cliente->getEmail();
						gotoxy(40, 15); cout << "Nuevo Email: "; cin >> aux;
						cliente->setEmail(aux);
						editar(cliente, clientes);
						gotoxy(40, 17); cout << "Email cambiado correctamente";
						gotoxy(40, 18); system("pause");
						gotoxy(40, 15);
						Console::Clear();
						break;
					case 6:
						Console::Clear();
						LogoBCP(18, 1);
						gotoxy(40, 14); cout << "Contrasenia anterior: " << cliente->getPassword();
						gotoxy(40, 15); cout << "Nueva contrasenia: "; cin >> aux;
						if (aux.size() != 6)
						{
							gotoxy(40, 17); cout << "La contrasenia introducida debe de ser de 6 digitos";
							gotoxy(40, 18); system("pause");
							Console::Clear();
							break;
						}
						cliente->setPassword(aux);
						editar(cliente, clientes);
						gotoxy(40, 17); cout << "Contrasenia cambiada correctamente";
						gotoxy(40, 18); system("pause");
						Console::Clear();
						break;
					case 7:
						bucle_menu_dos = false;
						Console::Clear();
						break;
					}
					break;
				}
			}

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
				while (aux_verif != nullptr)
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
			Console::Clear();
			LogoBCP(18, 1);
			int id_aux;
			bool bucle_acceso_cuenta = true;
			if (cliente->getCuentasBancarias()->getSize() != 0)
			{
				gotoxy(0, 12); cliente->getCuentasBancarias()->print();
				while (bucle_acceso_cuenta)
				{
					cout << "\nIngrese el numero de id de la cuenta que desee acceder: ";
					cin >> id_aux;
					Nodo<CuentaBancaria*>* aux = cliente->getCuentasBancarias()->search(id_aux);
					if (aux == nullptr)
					{
						cout << "\nNo se encontro la cuenta bancaria ingresada\n";
					}
					else
					{
						bucle_acceso_cuenta = false;
						Console::Clear();
						MenuSoloCuentaBancaria(aux->data);
						break;
					}
				}
				Console::Clear();
				break;
			}
			gotoxy(40, 14); cout << LBLUE << "Usted no tiene ninguna cuenta bancaria a su nombre" << RESET;
			gotoxy(40, 16); system("pause");
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
			salir = true;
			break;
		}
		};
	crearMenu(opciones, callback);
	Console::Clear();
}

void Bcp::MenuSoloCuentaBancaria(CuentaBancaria* CuentaB)
{

	vector<string> opciones = { "Ver los Datos de la Cuenta", "Cambiar contrasenia", "Ver las operaciones de la cuenta", "Agregar una Tarjeta a la cuenta", "Eliminar la Tarjeta de la cuenta", "Renovar Tarjeta", "Hacer una operacion", "Ver Cola de operaciones", "Salir" };

	auto callback = [this, CuentaB](int opcion, bool salir) {
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
			salir = true;
			break;
		default:
			break;
		}
	};
	crearMenu(opciones, callback);
}

inline void Bcp::MenuElegirCanalOSede(CuentaBancaria* cuenta)
{
	ListaDoble<Sede*>* sedesDisponibles = buscarPorCantidadAleatorio<ListaDoble<Sede*>*, Sede>(sedes, sedes->getSize() < 5 ? sedes->getSize() : 5);
	vector<string> opcionesSedes;
	for (int i = 0; i < sedesDisponibles->getSize(); i++)
	{
		opcionesSedes.push_back(sedesDisponibles->getByPosition(i)->data->getNombre());
	}
	auto callback = [sedesDisponibles, cuenta, this](int seleccion, bool salir) {
		Nodo<Sede*>* sede = sedesDisponibles->getByPosition(seleccion);
		if (sede != nullptr)
			MenuOperacionPorSede(sede->data, cuenta);
		};


	ListaDoble<Canal*>* canalesSinSedeDisponibles = buscarCanalesSinSede();

	vector<string> opcionesCanales;
	for (int i = 0; i < canalesSinSedeDisponibles->getSize(); i++)
	{
		opcionesCanales.push_back(canalesSinSedeDisponibles->getByPosition(i)->data->getNombre());
	}

	auto callback1 = [canalesSinSedeDisponibles, cuenta, this](int seleccion, bool salir) {
		Nodo<Canal*>* canal = canalesSinSedeDisponibles->getByPosition(seleccion);
		if (canal != nullptr)
			MenuOperacionPorCanal(canal->data, cuenta);
		};

	vector<string> opciones = {
		"Ver las sedes disponibles",
		"Ver los canales disponibles",
		"Salir"
	};

	auto callback2 = [opcionesSedes, opcionesCanales, callback, callback1, this](int seleccion, bool salir) {
		switch (seleccion) {
		case 0:
			crearMenu(opcionesSedes, callback);
			salir = true;
			break;
		case 1:
			crearMenu(opcionesCanales, callback1);
			salir = true;
			break;
		case 2:
			salir = true;
			break;
		}
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

	ListaDoble<Canal*>* ventanillas = sede->buscarCanalesPorTipo(VENTANILLA);
	vector<string> opcionesVentanillas;
	for (int i = 0; i < ventanillas->getSize(); i++)
	{
		opcionesVentanillas.push_back(ventanillas->getByPosition(i)->data->getNombre());
	}

	ListaDoble<Canal*>* cajeros = sede->buscarCanalesPorTipo(CAJERO);
	vector<string> opcionesCajeros;
	for (int i = 0; i < cajeros->getSize(); i++)
	{
		opcionesCajeros.push_back(cajeros->getByPosition(i)->data->getNombre());
	}

	auto callback1 = [cuenta, ventanillas, this](int seleccion1, bool salir) {
		Nodo<Canal*>* canal = ventanillas->getByPosition(seleccion1);
		if (canal != nullptr)
			MenuOperacionPorCanal(canal->data, cuenta);
		salir = true;
		};

	auto callback2 = [cuenta, cajeros, this](int seleccion2, bool salir) {
		Nodo<Canal*>* canal = cajeros->getByPosition(seleccion2);
		if (canal != nullptr)
			MenuOperacionPorCanal(canal->data, cuenta);
		salir = true;
		};



	auto callback = [sede, cuenta, ventanillas, opcionesVentanillas, opcionesCajeros, callback1, callback2, this](int seleccion, bool salir) {
		switch (seleccion) {
		case 0:
			sede->print();
			break;
		case 1:
			crearMenu(opcionesVentanillas, callback1);
			salir = true;
			break;
		case 2:
			crearMenu(opcionesCajeros, callback2);
			salir = true;
			break;
		case 3:
			salir = true;
			break;
		}
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
	auto callback = [canal, &monto, &SN, &cuentaDestino, &cuentaOrigen, &operacion, cuenta, this](int seleccion, bool salir) {
		switch (seleccion) {
		case 0:
			canal->print();
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
			if (operacion != nullptr) colaOperaciones->encolar(operacion);
			system("pause");
			break;
		case 4:
			salir = true;
			break;
		};
	};
}

void Bcp::MenuCanales()
{
	vector<string> opciones = {
		"Listar todos los canales",
		"Listar los canales por tipo",
		"Listar los canales por sede",
		"Agregar un canal",
		"Editar un canal",
		"Eliminar un canal",
		"Activar un canal",
		"Desactivar un canal",
		"Salir"
	};

	string tipo;
	string sede;
	int id;

	auto callback = [this, &tipo, &sede, &id](int seleccion, bool salir) {
		switch (seleccion) {
		case 0:
			canales->print();
			break;
		case 1:
			cout << "Ingrese el tipo: ";

			cin >> tipo;
			//buscarPorTipo(canales, tipo)->print();
			break;
		case 2:
			cout << "Ingrese la sede: ";

			cin >> sede;
			//buscarPorSede(canales, sede)->print();
			break;
		case 3:
			agregar<ListaDoble<Canal*>*, Canal>(new Canal(getLastId(canales), "Nuevo cajero", CAJERO, 1), canales); // CAJERO por defecto
			agregar<ListaDoble<Canal*>*, Canal>(new Canal(getLastId(canales),"Web", WEB), canales);
			agregar<ListaDoble<Canal*>*, Canal>(new Canal(getLastId(canales), "APP", APP), canales);
			agregar<ListaDoble<Canal*>*, Canal>(new Canal(getLastId(canales), "YAPE", YAPE), canales);
			break;
		case 4:
			cout << "Ingrese el id del canal a editar: ";

			cin >> id;
			editar<ListaDoble<Canal*>*, Canal>(buscarPorId<ListaDoble<Canal*>*, Canal>(id, canales)->data, canales);
			break;
		case 5:
			cout << "Ingrese el id del canal a eliminar: ";

			cin >> id;
			eliminar<ListaDoble<Canal*>*, Canal>(buscarPorId<ListaDoble<Canal*>*, Canal>(id, canales)->data, canales);
			break;
		case 6:
			cout << "Ingrese el id del canal a activar: ";

			cin >> id;
			buscarPorId<ListaDoble<Canal*>*, Canal>(id, canales)->data->activar();
			break;
		case 7:
			cout << "Ingrese el id del canal a desactivar: ";

			cin >> id;
			buscarPorId<ListaDoble<Canal*>*, Canal>(id, canales)->data->desactivar();
			break;
		case 8:
			salir = true;
			break;
		}
		};

	crearMenu(opciones, callback);
}

void Bcp::MenuSedes()
{
	vector<string> opciones = {
		"Listar todas las sedes",
		"Listar las sedes por ciudad",
		"Listar las sedes por departamento",
		"Agregar una sede",
		"Editar una sede",
		"Eliminar una sede",
		"Activar una sede",
		"Desactivar una sede",
		"Salir"
	};
	string ciudad;
	string departamento;
	int id;

	auto callback = [this, &ciudad, &departamento, &id](int seleccion, bool salir) {
		switch (seleccion) {
		case 0:
			sedes->print();
			break;
		case 1:
			cout << "Ingrese la ciudad: ";
	
			cin >> ciudad;
			//buscarPorCiudad(sedes, ciudad)->print();
			break;
		case 2:
			cout << "Ingrese el departamento: ";

			cin >> departamento;
			//buscarPorDepartamento(sedes, departamento)->print();
			break;
		case 3:
			agregar<ListaDoble<Sede*>*, Sede>(new Sede(getLastId(sedes), "nueva sede","Calle las Amapolas", "lima", "Lince", "Lima", "99857463", "nuevaSede@email.com"), sedes);
			break;
		case 4:
			cout << "Ingrese el id de la sede a editar: ";
			
			cin >> id;
			editar<ListaDoble<Sede*>*, Sede>(buscarPorId<ListaDoble<Sede*>*, Sede>(id, sedes)->data, sedes);
			break;
		case 5:
			cout << "Ingrese el id de la sede a eliminar: ";

			cin >> id;
			eliminar<ListaDoble<Sede*>*, Sede>(buscarPorId<ListaDoble<Sede*>*, Sede>(id, sedes)->data, sedes);
			break;
		case 6:
			cout << "Ingrese el id de la sede a activar: ";

			cin >> id;
			buscarPorId<ListaDoble<Sede*>*, Sede>(id, sedes)->data->activar();
			break;
		case 7:
			cout << "Ingrese el id de la sede a desactivar: ";

			cin >> id;
			buscarPorId<ListaDoble<Sede*>*, Sede>(id, sedes)->data->desactivar();
			break;
		case 8:
			salir = true;
			break;
		}
		};

	crearMenu(opciones, callback);
}

template <typename T>
void Bcp::crearMenu(const vector<string>& opciones, T callback) {
	int seleccion = 0;
	char tecla;
	bool salir = false;

	do {
		system("cls");
		LogoBCP(0, 0);

		int screenWidth = 80; // Ancho de la pantalla (puedes ajustar esto según sea necesario)
		int screenHeight = 25; // Alto de la pantalla (puedes ajustar esto según sea necesario)
		int menuHeight = opciones.size();
		int startY = (screenHeight - menuHeight) / 2;

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
		case 13: // Enter
			callback(seleccion, salir);
			break;
		}
	} while (tecla != 27 && salir == false); // Esc para salir o si continuar es false
}