#pragma once
#include <conio.h>
#include "colors.h"
#include "CuentaBancaria.h"
#include "Operacion.h"
#include "File.h"
#include "Tarjeta.h"
#include "Cliente.h"
#include "Sede.h"
#include "Canal.h"
#include "Cola.h"

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

		loadAll();
	}
	string getNombre() { return nombre; }
	ListaDoble<Cliente*>* getClientes() { return clientes; }
	ListaDoble<CuentaBancaria*>* getCuentas() { return cuentas; }
	ListaDoble<Tarjeta*>* getTarjetas() { return tarjetas; }
	ListaDoble<Operacion*>* getOperaciones() { return operaciones; }
	ListaDoble<Canal*>* getCanales() { return canales; }
	Cola<Operacion*>* getColaOperaciones() { return colaOperaciones; }
	ListaDoble<Sede*>* getSedes() { return sedes; }

	template <class T>
	int getLastId(T lista)
	{
		if (lista->getSize() == 0)
			return 1;
		else
			return lista->getLastId();
	}

	template <class T>
	Nodo<T>* buscarPorId(int id, T lista)
	{
		Nodo<T>* nodo = lista->head;
		while (nodo != nullptr)
		{
			if (nodo->data->getId() == id)
				return nodo;
			nodo = nodo->next;
		}
		return nullptr;
	}

	template <class T, class E>
	void agregar(E* item, T lista)
	{
		lista->push_back(item);
		this->save<T, E>(lista);
	}

	template <class T, class E>
	void editar(E* item, T lista)
	{
		Nodo<E*>* nodo = buscarPorId(item->getId(), lista);
		if (nodo != nullptr)
		{
			nodo->data = item;
			this->save<T, E>(lista);
		}
	}

	template <class T, class E>
	void eliminar(E* item, T lista)
	{
		Nodo<E*>* nodo = buscarPorId(item->getId(), lista);
		if (nodo != nullptr)
		{
			lista->remove(nodo);
			this->save<T, E>(lista);
		}
	}

	template <class T>
	void listar(T lista) {
		lista->print();
	}

	template <class T, class E>
	void save(T lista)
	{
		File<T, E>::escribir(lista->getNombreArchivo(), lista);
		this->load<T, E>(lista);
	}

	template <class T, class E>
	void load(T lista)
	{
		File<T, E>::leer(lista->getNombreArchivo(), lista);
	}

	void loadAll()
	{
		load<ListaDoble<Cliente*>*, Cliente>(clientes);
		load<ListaDoble<CuentaBancaria*>*, CuentaBancaria>(cuentas);
		load<ListaDoble<Tarjeta*>*, Tarjeta>(tarjetas);
		load<ListaDoble<Operacion*>*, Operacion>(operaciones);
		load<ListaDoble<Canal*>*, Canal>(canales);
		load<ListaDoble<Sede*>*, Sede>(sedes);
	}

	//Cuentas

	ListaDoble<CuentaBancaria*>* buscarCuentaPorIdCliente(int idCliente)
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
	
	//Operaciones
	Operacion* transferir(int id, string numeroCuentaBancariaOrigen, string cuentaDestino, double monto, Canal* canal, Sede* sede = nullptr) {
		CuentaBancaria* cuentaBancariaOrigen = CuentaBancaria::buscarCuentaPorNumero(numeroCuentaBancariaOrigen);
		if (cuentaBancariaOrigen == nullptr) {
			cout << "Cuenta origen no encontrada, intenta escribir sin espacios ni guiones entre los digitos." << endl;
			return nullptr;
		}

		CuentaBancaria* cuentaBancariaDestino = CuentaBancaria::buscarCuentaPorNumero(cuentaDestino);
		if (cuentaBancariaDestino == nullptr) {
			cout << "Cuenta destino no encontrada, intenta escribir sin espacios ni guiones entre los digitos." << endl;
			return nullptr;
		}

		if (monto > cuentaBancariaOrigen->getSaldo()) {
			cout << "Saldo insuficiente" << endl;
			return nullptr;
		}

		return new Operacion(id, cuentaBancariaOrigen->getIdCliente(), cuentaBancariaDestino->getIdCliente(), cuentaBancariaOrigen->getId(), cuentaBancariaDestino->getId(), Transferencia, monto, canal, sede);
	}

	Operacion* depositar(int id, string numeroCuentaBancaria, double monto, Canal* canal, Sede* sede = nullptr) {
		CuentaBancaria* cuentaBancaria = CuentaBancaria::buscarCuentaPorNumero(numeroCuentaBancaria);
		if (cuentaBancaria == nullptr) {
			cout << "Cuenta no encontrada, intenta escribir sin espacios ni guiones entre los digitos." << endl;
			return nullptr;
		}

		return new Operacion(id, 0, cuentaBancaria->getIdCliente(), 0, cuentaBancaria->getId(), Deposito, monto, canal, sede);
	}

	Operacion* retirar(int id, string numeroCuentaBancaria, double monto, Canal* canal, Sede* sede = nullptr) {
		CuentaBancaria* cuentaBancaria = CuentaBancaria::buscarCuentaPorNumero(numeroCuentaBancaria);
		if (cuentaBancaria == nullptr) {
			cout << "Cuenta no encontrada, intenta escribir sin espacios ni guiones entre los digitos." << endl;
			return nullptr;
		}

		if (monto > cuentaBancaria->getSaldo()) {
			cout << "Saldo insuficiente" << endl;
			return nullptr;
		}

		return new Operacion(id, cuentaBancaria->getIdCliente(), 0, cuentaBancaria->getId(), 0, Retiro, monto, canal, sede);
	}

	Operacion* cargarOperacion(int id, string numeroCuentaBancariaOrigen, Canal* canal, Sede* sede = nullptr) {
		double monto;
		string cuentaDestino;
		int opcion;

		CuentaBancaria* cuentaBancariaOrigen = CuentaBancaria::buscarCuentaPorNumero(numeroCuentaBancariaOrigen);
		if (cuentaBancariaOrigen == nullptr)
		{
			cout << "Cuenta origen no encontrada, intenta escribir sin espacios ni guiones entre los digitos." << endl;
			return nullptr;
		}

		cout << "Ingrese el monto a transferir: ";
		cin >> monto;

		do {
			cout << "Seleccione una opcion: " << endl;
			cout << "1. Transferencia" << endl;
			cout << "2. Deposito" << endl;
			cout << "3. Retiro" << endl;
			cin >> opcion;
		} while (opcion < 1 || opcion > 3);

		//actualizar para incluir canal y sede ya sea durante esta operacion o antes de realizarla
		if (opcion == Transferencia) {
			cout << "Ingrese la cuenta de destino: ";
			cin >> cuentaDestino;

			CuentaBancaria* cuentaBancariaDestino = CuentaBancaria::buscarCuentaPorNumero(cuentaDestino);
			if (cuentaBancariaDestino == nullptr)
			{
				cout << "Cuenta destino no encontrada, intenta escribir sin espacios ni guiones entre los digitos." << endl;
				return nullptr;
			}
			if (monto > cuentaBancariaOrigen->getSaldo())
			{
				cout << "Saldo insuficiente" << endl;
				return nullptr;
			}
			return new Operacion(id, cuentaBancariaOrigen->getIdCliente(), cuentaBancariaDestino->getIdCliente(), cuentaBancariaOrigen->getId(), cuentaBancariaDestino->getId(), Transferencia, monto, canal, sede);
		}
		else if (opcion == Deposito) {
			return new Operacion(id, cuentaBancariaOrigen->getIdCliente(), cuentaBancariaOrigen->getIdCliente(), cuentaBancariaOrigen->getId(), cuentaBancariaOrigen->getId(), Deposito, monto, canal, sede);
		}
		else if (opcion == Retiro) {
			if (monto > cuentaBancariaOrigen->getSaldo())
			{
				cout << "Saldo insuficiente" << endl;
				return nullptr;
			}
			return new Operacion(id, cuentaBancariaOrigen->getIdCliente(), 0, cuentaBancariaOrigen->getId(), 0, Retiro, monto, canal, sede);
		}
		else {
			return nullptr;
		}
	}

	void ejecutarOperacion(Operacion* operacion) {
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
	void LogoBCP(int x, int y)
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

	void MenuBCP()
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
					gotoxy(x_selec + 17, y_selec); cout <<" ";
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
					this->clientes->push_back(new Cliente(this->getLastId(clientes), nombre, apellido, direccion, telefono, email, password));
					this->save<ListaDoble<Cliente*>*, Cliente>(clientes);
					gotoxy(45, 12); cout << LBLUE << "Cuenta Registrada Correctamente"<<RESET;
					gotoxy(45, 14); cout << RED << system("pause")<<RESET;
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

	void MenuSoloCliente(Cliente* cliente)
	{
		bool bucle_menu = true;
		int x_selec, y_selec, op;
		op = 1;
		x_selec = 80;
		y_selec = 12;
		cliente->loadCuentasBancarias();
		while(bucle_menu)
		{
			LogoBCP(18, 1);
			gotoxy(x_selec, y_selec); cout << YELLOW << "<" << RESET;
			gotoxy(40, 12); cout << "Ver mis Datos";
			gotoxy(40, 13); cout << "Modificar uno de mis datos";
			gotoxy(40, 14); cout << "Ver mis Cuentas Bancarias";
			gotoxy(40, 15); cout << "Agregar una Cuenta Bancaria Random";
			gotoxy(40, 16); cout << "Acceder a mi Cuenta Bancaria";
			gotoxy(40, 17); cout << "Eliminar una de mis Cuentas Bancarias";
			gotoxy(40, 18); cout << "Salir";
			switch (char c = toupper(_getch()))
			{
			case 80://Abajo
				switch (op)
				{
				case 1:
					gotoxy(x_selec, y_selec); cout <<" ";
					op++;
					y_selec++;
					break;
				case 2:
					gotoxy(x_selec, y_selec); cout << " ";
					op++;
					y_selec++;
					break;
				case 3:
					gotoxy(x_selec, y_selec); cout << " ";
					op++;
					y_selec++;
					break;
				case 4:
					gotoxy(x_selec, y_selec); cout << " ";
					op++;
					y_selec++;
					break;
				case 5:
					gotoxy(x_selec, y_selec); cout << " ";
					op++;
					y_selec++;
					break;
				case 6:
					gotoxy(x_selec, y_selec); cout << " ";
					op++;
					y_selec++;
					break;
				default:
					break;
				}
				break;
			case 72://Arriba
				switch (op)
				{
				case 2:
					gotoxy(x_selec, y_selec); cout << " ";
					op--;
					y_selec--;
					break;
				case 3:
					gotoxy(x_selec, y_selec); cout << " ";
					op--;
					y_selec--;
					break;
				case 4:
					gotoxy(x_selec, y_selec); cout << " ";
					op--;
					y_selec--;
					break;
				case 5:
					gotoxy(x_selec, y_selec); cout << " ";
					op--;
					y_selec--;
					break;
				case 6:
					gotoxy(x_selec, y_selec); cout << " ";
					op--;
					y_selec--;
					break;
				case 7:
					gotoxy(x_selec, y_selec); cout << " ";
					op++;
					y_selec++;
					break;
				default:
					break;
				}
				break;
			case 13://Enter
				switch (op)
				{
				case 1:
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
				case 2:
				{
					Console::Clear();
					string aux;
					int x_selec_dos, y_selec_dos;
					int op_2=1;
					bool bucle_menu_dos=true;
					x_selec_dos = 54;
					y_selec_dos = 14;
					while(bucle_menu_dos)
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
								this->save<ListaDoble<Cliente*>*, Cliente>(clientes);
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
								this->save<ListaDoble<Cliente*>*, Cliente>(clientes);
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
								this->save<ListaDoble<Cliente*>*, Cliente>(clientes);
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
								this->save<ListaDoble<Cliente*>*, Cliente>(clientes);
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
								this->save<ListaDoble<Cliente*>*, Cliente>(clientes);
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
								this->save<ListaDoble<Cliente*>*, Cliente>(clientes);
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
				case 3:
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
				case 4:
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
					CuentaBancaria* aux = new CuentaBancaria(this->cuentas->getLastId(), cliente->getId(), contrasenia, numero_cuenta, 0);
					this->cuentas->push_back(aux);
					cliente->agregarCuentaBancaria(aux);
					Console::Clear();
					LogoBCP(18, 1);
					gotoxy(40, 12); cout << "Cuenta bancaria agregada correctamente";
					gotoxy(40, 14); system("pause");
					Console::Clear();
					break;
				}
				case 5:
				{
					Console::Clear();
					LogoBCP(18, 1);
					int id_aux;
					bool bucle_acceso_cuenta=true;
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
				case 6:
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
							this->save<ListaDoble<CuentaBancaria*>*, CuentaBancaria>(cuentas);
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
				case 7:
					bucle_menu = false;
					break;
				}
			}
		}
		Console::Clear();
	}

	void MenuSoloCuentaBancaria(CuentaBancaria* CuentaB)
	{
		Console::Clear();
		CuentaB->loadTarjeta();
		CuentaB->loadOperaciones();
		bool bucle_menu = true;
		int x_selec, y_selec, op;
		op = 1;
		x_selec = 84;
		y_selec = 12;
		while (bucle_menu)
		{
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ver los Datos de la Cuenta";
			gotoxy(40, 13); cout << "Cambiar contrasenia";
			gotoxy(40, 14); cout << "Ver las operaciones de la cuenta";
			gotoxy(40, 15); cout << "Agregar una Tarjeta a la cuenta";
			gotoxy(40, 16); cout << "Eliminar la Tarjeta de la cuenta";
			gotoxy(40, 17); cout << "Transferir dinero";
			gotoxy(40, 18); cout << "Depositar dinero";
			gotoxy(40, 19); cout << "Retirar dinero";
			gotoxy(40, 20); cout << "Salir";
			gotoxy(x_selec, y_selec); cout << YELLOW << "<"<<RESET;
			switch (char c = toupper(_getch()))
			{
			case 80://Abajo
				switch (op)
				{
				default:
				case 1:
					gotoxy(x_selec, y_selec); cout << " ";
					op++;
					y_selec++;
					break;
				case 2:
					gotoxy(x_selec, y_selec); cout << " ";
					op++;
					y_selec++;
					break;
				case 3:
					gotoxy(x_selec, y_selec); cout << " ";
					op++;
					y_selec++;
					break;
				case 4:
					gotoxy(x_selec, y_selec); cout << " ";
					op++;
					y_selec++;
					break;
				case 5:
					gotoxy(x_selec, y_selec); cout << " ";
					op++;
					y_selec++;
					break;
				case 6:
					gotoxy(x_selec, y_selec); cout << " ";
					op++;
					y_selec++;
					break;
				case 7:
					gotoxy(x_selec, y_selec); cout << " ";
					op++;
					y_selec++;
					break;
				case 8:
					gotoxy(x_selec, y_selec); cout << " ";
					op++;
					y_selec++;
					break;
				}
			case 72://Arriba
				switch (op)
				{
				case 2:
					gotoxy(x_selec, y_selec); cout << " ";
					op--;
					y_selec--;
					break;
				case 3:
					gotoxy(x_selec, y_selec); cout << " ";
					op--;
					y_selec--;
					break;
				case 4:
					gotoxy(x_selec, y_selec); cout << " ";
					op--;
					y_selec--;
					break;
				case 5:
					gotoxy(x_selec, y_selec); cout << " ";
					op--;
					y_selec--;
					break;
				case 6:
					gotoxy(x_selec, y_selec); cout << " ";
					op--;
					y_selec--;
					break;
				case 7:
					gotoxy(x_selec, y_selec); cout << " ";
					op--;
					y_selec--;
					break;
				case 8:
					gotoxy(x_selec, y_selec); cout << " ";
					op--;
					y_selec--;
					break;
				case 9:
					gotoxy(x_selec, y_selec); cout << " ";
					op--;
					y_selec--;
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
					LogoBCP(18, 1);
					gotoxy(45, 14); cout << "Id: " << CuentaB->getId();
					gotoxy(45, 15); cout << "Id Cliente: " << CuentaB->getIdCliente();
					gotoxy(45, 16); cout << "Numero de cuenta: " << CuentaB->getNumeroCuenta();
					gotoxy(45, 17); cout << "Contrasenia: " << CuentaB->getPassword();
					gotoxy(45, 18); cout << "Saldo: " << CuentaB->getSaldo();
					gotoxy(50, 19); cout << YELLOW<<"Tarjeta:"<<RESET;
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
				case 2:
				{
					Console::Clear();
					string auxiliar;
					do{
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
					this->save<ListaDoble<CuentaBancaria*>*, CuentaBancaria>(cuentas);
					Console::Clear();
					LogoBCP(18, 1);
					gotoxy(40, 14); cout << "Contrasenia cambiada correctamente";
					gotoxy(40, 15); system("pause");
					Console::Clear();
					break;
				}
				case 3:
				{
					Console::Clear();
					LogoBCP(18, 1);
					gotoxy(0, 12); CuentaB->getOperaciones()->print();
					system("pause");
					Console::Clear();
					break;
				}
				case 4:
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
						this->save<ListaDoble<Tarjeta*>*, Tarjeta>(tarjetas);
						gotoxy(40, 12); cout << "Tarjeta aniadida correctamente";
						gotoxy(40, 13); system("pause");
					}
					Console::Clear();
					break;
				}
				case 5:
				{	
					Console::Clear();
					LogoBCP(18, 1);
					Nodo<Tarjeta*>* auxiliar = this->tarjetas->search(CuentaB->getTarjeta()->getId());
					if(auxiliar!=nullptr)
					{
						this->tarjetas->remove(auxiliar);
						CuentaB->setTarjeta(nullptr);
						this->save<ListaDoble<Tarjeta*>*, Tarjeta>(tarjetas);
						gotoxy(40, 12); cout << "La tarjeta ha sido removida correctamente";
						system("pause");
						break;
					}
					gotoxy(40, 12); cout << "Tu cuenta no tiene una tarjeta asignada";
					system("pause");
					Console::Clear();
					break;
				}
				//Funciones respecto a las operaciones no incluidas
				//Pense en agregar una funcion de menu extra para las operaciones
				case 6:
					break;
				case 7:
					break;
				case 8:
					break;
				case 9:
					Console::Clear();
					bucle_menu = false;
					break;
				default:
					break;
				}
				break;
			}
		}
		Console::Clear();
	}
};