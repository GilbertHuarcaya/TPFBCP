#pragma once
#include <conio.h>
#include "colors.h"
#include "Tarjeta.h"
#include "Cliente.h"
#include "Sede.h"
#include "Canal.h"
#include "Cola.h"
#include "HashTabla.h"
#include "ListaDoble.h"
#include "functional"
#include "vector"
#include "HashTablaLista.h"
#include "ArbolBinario.h"
#include "ArbolAVL.h"
#include "iostream"
#include "algorithm"
#include "Grafo.h"
#define SCREEN_WIDTH 120
#define SCREEN_HEIGHT 70

using namespace std;
using namespace System;

enum ToastType 
{
	TSUCCESS,
	TERROR,
	TWARNING,
	TINFO
};

void gotoxy(int x, int y)
{
	Console::SetCursorPosition(x, y);
}

class Bcp
{

private:
	string nombre;
	time_t fechaParaCvvDinamico;

	ListaDoble<Operacion*>* operaciones;

	Cola<Operacion*>* colaOperaciones;

	HashTablaLista<Cliente*>* hashClientes;
	HashTablaLista<Sede*>* hashSedes;

	ArbolBinario<Canal*>* canales;

	ArbolAVL<CuentaBancaria*>* cuentas;
	ArbolAVL<Tarjeta*>* tarjetas;
public:
	Bcp()
	{
		this->nombre = "Banco de Credito del Peru";
		this->fechaParaCvvDinamico = time(0);
		operaciones = new ListaDoble<Operacion*>("Datos/Operaciones.csv");
		colaOperaciones = new Cola<Operacion*>("Datos/ColaOperaciones.csv");

		hashClientes = new HashTablaLista<Cliente*>("Datos/Clientes.csv");
		hashSedes = new HashTablaLista<Sede*>("Datos/Sedes.csv");

		//Arboles
		canales = new ArbolBinario<Canal*>([&](Canal* a, Canal* b) {return a->getId() > b->getId(); }, "Datos/Canales.csv");

		cuentas = new ArbolAVL<CuentaBancaria*>("Datos/Cuentas.csv");
		tarjetas = new ArbolAVL<Tarjeta*>("Datos/Tarjetas.csv");

		loadAll();
	}

	~Bcp()
	{
		delete cuentas;
		delete tarjetas;
		delete operaciones;
		delete canales;
		delete colaOperaciones;
		delete hashClientes;
		delete hashSedes;
	}

	string getNombre();

	//Metodos para listas / colas
	template <class T>
	int getNextId(T lista);

	template <class T, class E>
	E* buscar(int identificador, T lista);

	template <class T, class E>
	void agregar(E* item, T lista);

	template <class T, class E>
	void insertar(E* item, T lista);

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


	//métodos generales
	void loadAll();

	void reloadAll();


	//helpers
	ArbolAVL<CuentaBancaria*>* buscarCuentaPorIdCliente(int idCliente);

	ArbolBinario<Canal*>* buscarCanalesSinSede();

	void ejecutarOperacion(Operacion* operacion);

	bool compararConValor(string campo, string valor);

	bool printToast(string mensaje, ToastType tipoToast );

	void generarCvvDinamico(Tarjeta* tarjeta);

	//teamplates para Menus

	template<typename T>
	void crearMenu(const vector<string>& opciones, T callback);

	template<typename T>
	void crearFormulario(const vector<string>& campos, T callback);

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

	//MENU GRAFO
	void MenuGrafoCuentasBancariasYOperaciones();


	//MENU DE ELECCION DE VALOR DE BUSQUEDA

	HashTablaLista<Cliente*>* MenuElegirBusquedaDeClientePorValorOPorId();

	HashTablaLista<Sede*>* MenuElegirBusquedaDeSedePorValorOPorId();

};

string Bcp::getNombre() { return nombre; }

template <class T>
int Bcp::getNextId(T lista)
{
	if (lista->getSize() == 0)
		return 1;
	else
		return lista->getNextId();
}

template <class T, class E>
E* Bcp::buscar(int id, T lista)
{
	if (lista->search(id) != nullptr) {
		return lista->search(id)->data;
	}
	return nullptr;
}

template <class L, class N>
L* buscarPorCantidadAleatorio(L* lista, int cantidad)
{
	L* listaAleatoria = new L();
	int size = lista->getSize();
	if (size < cantidad)
	{
		return nullptr;
	}
	for (int i = 0; i < cantidad; i++)
	{
		int random = rand() % size;
		N nodo = lista->search(random);

		if (nodo != nullptr)
			listaAleatoria->push_back(nodo->data, nodo->key);
	}
	return listaAleatoria;
}

/*
Para las estructuras que usan ListaDoble como base
*/
template <class T, class E>
void Bcp::agregar(E* item, T lista)
{
	lista->push_back(item, item->getId());
	this->save<T, E>(lista);
}

/*
Para las estructuras que usan Arbol como base
*/
template<class T, class E>
inline void Bcp::insertar(E* item, T lista)
{
	lista->insertar(item);
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
	E* existingItem = buscar<T,E>(item->getId(), lista);
	if (existingItem != nullptr)
	{
		lista->updateElement(item);
		this->save<T, E>(lista);
	}
}

template <class T, class E>
void Bcp::eliminar(E* item, T lista)
{
	E* existingItem = buscar<T, E>(item->getId(), lista);
	if (existingItem != nullptr)
	{
		lista->removeElement(item);
		this->save<T, E>(lista);
	}
}

template <class T>
void Bcp::listar(T lista) {
	lista->print();
	system("pause");
}

template <class T, class E>
void Bcp::save(T lista)
{
	lista->escribir();
}

template <class T, class E>
void Bcp::load(T lista)
{
	lista->leer();
}

template <class T, class E>
void Bcp::reload(T lista)
{
	lista->recargar();
}

void Bcp::loadAll()
{
	load<ArbolAVL<CuentaBancaria*>*, CuentaBancaria>(cuentas);
	load<ArbolAVL<Tarjeta*>*, Tarjeta>(tarjetas);
	load<ListaDoble<Operacion*>*, Operacion>(operaciones);
	load<ArbolBinario<Canal*>*, Canal>(canales);

	load<HashTablaLista<Cliente*>*, Cliente>(hashClientes);
	load<HashTablaLista<Sede*>*, Sede>(hashSedes);
	
}

void Bcp::reloadAll()
{
	reload<ArbolAVL<CuentaBancaria*>*, CuentaBancaria>(cuentas);
	reload<ArbolAVL<Tarjeta*>*, Tarjeta>(tarjetas);
	reload<ListaDoble<Operacion*>*, Operacion>(operaciones);
	reload<ArbolBinario<Canal*>*, Canal>(canales);

	reload<HashTablaLista<Cliente*>*, Cliente>(hashClientes);
	reload<HashTablaLista<Sede*>*, Sede>(hashSedes);
}

//Cuentas

ArbolAVL<CuentaBancaria*>* Bcp::buscarCuentaPorIdCliente(int idCliente)
{
	return cuentas->searchMultipleByValue([&](CuentaBancaria* cb) {
		return cb->getIdCliente() == idCliente;
		});
}

inline ArbolBinario<Canal*>* Bcp::buscarCanalesSinSede()
{

	NodoArbol<Canal*>* temp = canales->getRaiz();

	return canales->searchMultipleByValue([&](Canal* canal) {
		return canal->getIdSede() == 0;
		});
    
}

void Bcp::ejecutarOperacion(Operacion* operacion) {
	NodoArbol<CuentaBancaria*>* cuentaBancariaOrigen = cuentas->search(operacion->getIdCuentaBancariaOrigen());
	NodoArbol<CuentaBancaria*>* cuentaBancariaDestino = cuentas->search(operacion->getIdCuentaBancariaDestino());
	operacion->validarOperacion();
	if (operacion->getTipo() == Transferencia) {
		// Transferir
		if (cuentaBancariaOrigen == nullptr || cuentaBancariaDestino == nullptr) {
			operacion->setEstado(Rechazada);
			printToast("Cuenta origen o destino no encontrada", TERROR);
			return;
		}

		if (cuentaBancariaOrigen->data->getSaldo() >= operacion->getMonto()) {
			cuentaBancariaOrigen->data->transferir(cuentaBancariaDestino->data, operacion->getMonto());
			operacion->setEstado(Realizada);
		}
		else {
			operacion->setEstado(Rechazada);
			printToast("Saldo insuficiente", TERROR);
		}
	}
	else if (operacion->getTipo() == Deposito) {
		// Depositar
		if (cuentaBancariaDestino == nullptr) {
			operacion->setEstado(Rechazada);
			printToast("Cuenta destino no encontrada", TERROR);
			return;
		}
		cuentaBancariaDestino->data->depositar(operacion->getMonto());
		operacion->setEstado(Realizada);
	}
	else if (operacion->getTipo() == Retiro) {
		// Retirar
		if (cuentaBancariaOrigen == nullptr) {
			operacion->setEstado(Rechazada);
			printToast("Cuenta destino no encontrada", TERROR);
			return;
		}
		if (cuentaBancariaOrigen->data->getSaldo() >= operacion->getMonto()) {
			cuentaBancariaOrigen->data->retirar(operacion->getMonto());
			operacion->setEstado(Realizada);
		}
		else {
			operacion->setEstado(Rechazada);
			printToast("Saldo insuficiente", TERROR);
		}
	}

	//recargar cuentas bancarias de los clientes
	if (cuentaBancariaOrigen != nullptr)
		buscar<HashTablaLista<Cliente*>*, Cliente>(cuentaBancariaOrigen->data->getIdCliente(), hashClientes)->loadCuentasBancarias();
	if (cuentaBancariaDestino != nullptr)
		buscar<HashTablaLista<Cliente*>*, Cliente>(cuentaBancariaDestino->data->getIdCliente(), hashClientes)->loadCuentasBancarias();
}

bool Bcp::compararConValor(string campo, string valor) {
	string campoLower = campo;
	string valorLower = valor;
	transform(campoLower.begin(), campoLower.end(), campoLower.begin(), ::tolower);
	transform(valorLower.begin(), valorLower.end(), valorLower.begin(), ::tolower);
	return campoLower.find(valorLower) != string::npos;
}
inline bool Bcp::printToast(string mensaje, ToastType tipoToast)
{
    Console::Clear();
    LogoBCP(18, 1);
	int startX = ((SCREEN_WIDTH - mensaje.length()) / 2) - 10;
    gotoxy(startX, 12);
    switch (tipoToast)
    {
    case TSUCCESS:
        cout << GREEN << "SUCCESS: " << mensaje << RESET;
        break;
    case TERROR:
		cout << RED << "ERROR: " << mensaje << RESET;
        break;
    case TWARNING:
		cout << YELLOW << "WARNING: " << mensaje << RESET;
        break;
    case TINFO:
		cout << LBLUE << "INFO: " << mensaje << RESET;
        break;
    default:
        cout << mensaje;
        break;
    }
    gotoxy(startX, 14);
	_sleep(3000);
    Console::Clear();
    return true;
}

inline void Bcp::generarCvvDinamico(Tarjeta* tarjeta)
{
	time_t now = time(0);
	double seconds = difftime(now, fechaParaCvvDinamico);
	if (seconds >= 300)
	{
		Random r(time(NULL));
		tarjeta->setCvv(to_string(r.Next(101, 1000) - 1));
		editar(tarjeta, tarjetas);
		fechaParaCvvDinamico = now;

		printToast("Su CVV ha sido actualizado", TINFO);

		Console::Clear();
		gotoxy(40, 12); cout << "Su nuevo CVV es: " << tarjeta->getCvv();
		_sleep(3000);
		Console::Clear();
	}
}

void imprimirTarjetaVirtual(Tarjeta* tarjeta)
{
	string numero = tarjeta->getNumero();
	string fechaVencimiento = tarjeta->getFechaVencimientoMMAA();
	string cvv = tarjeta->getCvv();
	int startX = ((SCREEN_WIDTH - 30) / 2) - 10;
	
	gotoxy(startX, 16); cout << "*-------------------------------*" << endl;
	gotoxy(startX, 17); cout << "|                               |" << endl;
	gotoxy(startX, 18); cout << "|        TARJETA VIRTUAL        |" << endl;
	gotoxy(startX, 19); cout << "|                               |" << endl;
	gotoxy(startX, 20); cout << "|  Numero: " << numero << "     |" << endl;
	gotoxy(startX, 21); cout << "|                               |" << endl;
	gotoxy(startX, 22); cout << "|  Vencimiento: " << fechaVencimiento << "           |" << endl;
	gotoxy(startX, 23); cout << "|                               |" << endl;
	gotoxy(startX, 24); cout << "|  CVV: " << cvv << "                     |" << endl;
	gotoxy(startX, 25); cout << "|                               |" << endl;
	gotoxy(startX, 26); cout << "*-------------------------------*" << endl;
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
			vector<string> campos = { "Correo", "Contrasenia" };
			bool encontrado = false;
			HashEntidadNodo<Cliente*>* ClienteEncontrado = nullptr;
			crearFormulario(campos, [&](const vector<string>& valores) -> bool {
				HashEntidadNodo<Cliente*>* cliente = hashClientes->getFirst();
		
				while (cliente != nullptr)
				{
					if (cliente->data->getEmail() == valores[0] && cliente->data->getPassword() == valores[1])
					{
						ClienteEncontrado = cliente;
						encontrado = true;
					}
					cliente = cliente->next;
				}

				if (!encontrado)
				{
					printToast("No se pudo encontrar la Cuenta del Cliente...", TERROR);

					Console::Clear();

					char opcion;
					gotoxy(40, 13); cout << "Desea seguir intentando? (s/n): ";
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
				MenuAdmin();
				return true;
				break;
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
				direccion = valores[2];
				return false;
				});
			crearFormulario(camposRegistroTelefono, [&](const vector<string>& valores) -> bool {
				bool repetido = false;
				
				HashEntidadNodo<Cliente*>* cliente = hashClientes->searchByValue([&](Cliente* cliente) {
					return cliente->getTelefono() == valores[0];
					});
				repetido = cliente != nullptr;

				if (repetido)
				{
					printToast("El telefono es repetido. Intente nuevamente", TINFO);
					return true;
				}
				telefono = valores[0];
				return false;
				});
			crearFormulario(camposRegistroEmail, [&](const vector<string>& valores) -> bool {
				bool repetido = false;
				HashEntidadNodo<Cliente*>* cliente = hashClientes->searchByValue([&](Cliente* cliente) {
					return cliente->getEmail() == valores[0];
					});
				repetido = cliente != nullptr;
				if (repetido)
				{
					printToast("El email es repetido. Intente nuevamente", TINFO);
					return true;
				}
				email = valores[0];
				return false;
				});
			crearFormulario(camposRegistroContrasenia, [&](const vector<string>& valores) -> bool {
				if (valores[0].size() != 6)
				{
					printToast("La contrasenia debe tener 6 caracteres", TINFO);
					return true;
				}
				password = valores[0];
				return false;
				});
			Cliente* cliente = new Cliente(getNextId(hashClientes), nombre, apellido, direccion, telefono, email, password);
			agregar(cliente, hashClientes);

			printToast("Cliente registrado correctamente", TSUCCESS);

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
			vector<string> opciones = {
				"Nombre - (Actualmente - " + cliente->getNombre() + ")",
				"Apellid - (Actualmente - " + cliente->getApellido() + ")",
				"Telefono - (Actualmente - " + cliente->getTelefono() + ")",
				"Direccion - (Actualmente - " + cliente->getDireccion() + ")",
				"Email - (Actualmente - " + cliente->getEmail() + ")",
				"Contrasenia - (Actualmente - " + cliente->getPassword() + ")",
				"Salir"
			};
			string aux;
			auto callback = [&](int opcion) {
				switch (opcion)
				{
				case 0:
					Console::Clear();
					LogoBCP(18, 1);
					gotoxy(40, 14); cout << "Nombre anterior: " << cliente->getNombre();
					gotoxy(40, 15); cout << "Nuevo Nombre: "; 
					getline(cin >> ws, aux);
					cliente->setNombre(aux);
					editar(cliente, hashClientes);
					gotoxy(40, 17); cout << "Nombre cambiado correctamente";
					gotoxy(40, 18); system("pause");
					Console::Clear();
					break;
				case 1:
					Console::Clear();
					LogoBCP(18, 1);
					gotoxy(40, 14); cout << "Apellido anterior: " << cliente->getApellido();
					gotoxy(40, 15); cout << "Nuevo telefono: "; 
					getline(cin >> ws, aux);
					cliente->setApellido(aux);
					editar(cliente, hashClientes);
					gotoxy(40, 17); cout << "Apellido cambiado correctamente";
					gotoxy(40, 18); system("pause");
					Console::Clear();
					break;
				case 2:
					Console::Clear();
					LogoBCP(18, 1);
					gotoxy(40, 14); cout << "Telefono anterior: " << cliente->getTelefono();
					gotoxy(40, 15); cout << "Nuevo telefono: "; 
					getline(cin >> ws, aux);
					cliente->setTelefono(aux);
					editar(cliente, hashClientes);
					gotoxy(40, 17); cout << "Telefono cambiado correctamente";
					gotoxy(40, 18); system("pause");
					Console::Clear();
					break;
				case 3:
					Console::Clear();
					LogoBCP(18, 1);
					gotoxy(40, 14); cout << "Direccion anterior: " << cliente->getDireccion();
					gotoxy(40, 15); cout << "Nueva direccion: "; 
					getline(cin >> ws, aux);
					cliente->setDireccion(aux);
					editar(cliente, hashClientes);
					gotoxy(40, 17); cout << "Direccion cambiada correctamente";
					gotoxy(40, 18); system("pause");
					Console::Clear();
					break;
				case 4:
					Console::Clear();
						LogoBCP(18, 1);
					gotoxy(40, 14); cout << "Email anterior: " << cliente->getEmail();
					gotoxy(40, 15); cout << "Nuevo email: "; 
					getline(cin >> ws, aux);
					cliente->setEmail(aux);
					editar(cliente, hashClientes);
					gotoxy(40, 17); cout << "Email cambiado correctamente";
					gotoxy(40, 18); system("pause");
					Console::Clear();
					break;
				case 5:
					Console::Clear();
					LogoBCP(18, 1);
					gotoxy(40, 14); cout << "Contrasenia anterior: " << cliente->getPassword();
					gotoxy(40, 15); cout << "Nueva contrasenia: "; 
					getline(cin >> ws, aux);
					cliente->setPassword(aux);
					editar(cliente, hashClientes);
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
			printToast("No tiene cuentas bancarias", TINFO);
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
				for (int i = 0; i < 14; i++)
				{
					Random r;
					numero_cuenta.append(to_string(r.Next(0, 9)));
					_sleep(100);
				}

				//validar si ya existe sino volvera a crear un nuevo numero de 14 digitos
				NodoArbol<CuentaBancaria*>* verificacionCB = cuentas->searchMultipleByValue([&](CuentaBancaria* cb) {
					return cb->getNumeroCuenta() == numero_cuenta;
					})->getRaiz();
				if (verificacionCB != nullptr)
					verificacion = true;
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
			CuentaBancaria* nuevaCuentaBancaria = new CuentaBancaria(getNextId(cuentas), cliente->getId(), contrasenia, numero_cuenta, 0);
			cliente->agregarCuentaBancaria(nuevaCuentaBancaria);
			insertar(nuevaCuentaBancaria, cuentas);

			printToast("Cuenta bancaria agregada correctamente", TSUCCESS);

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

			if (misCuentas.size() == 0)
			{
				printToast("No tiene cuentas bancarias", TINFO);
				Console::Clear();
				break;
			}

			auto callback1 = [&](int opcion) {

				if (cuentasDelCliente->head != nullptr) {
					MenuSoloCuentaBancaria(cuentas->search(cuentasDelCliente->getByPosition(opcion)->data->getId())->data);
					return false;
				}
				else {
					printToast("No tiene cuentas bancarias", TINFO);
					Console::Clear();
					return false;
				};
		
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

				CuentaBancaria* auxiliar = buscar<ArbolAVL<CuentaBancaria*>*, CuentaBancaria>(n, cuentas);

				if (auxiliar != nullptr)
				{
					Console::Clear();
					LogoBCP(18, 1);
					gotoxy(50, 14); cout << "Id: " << auxiliar->getId();
					gotoxy(50, 15); cout << "Id Cliente: " << auxiliar->getIdCliente();
					gotoxy(50, 16); cout << "Numero cuenta: " << auxiliar->getNumeroCuenta();
					gotoxy(50, 17); cout << "Contrasenia: " << auxiliar->getPassword();
					gotoxy(50, 18); cout << "Saldo: " << auxiliar->getSaldo();
					if (auxiliar->getTarjeta() != nullptr)
					{
						gotoxy(50, 19); cout << "Nro Tarjeta: " << auxiliar->getTarjeta()->getNumero();
						gotoxy(50, 20); cout << "CVV: " << auxiliar->getTarjeta()->getCvv();
					}
					else
					{
						gotoxy(50, 19); cout << "La cuenta no tiene tarjeta";
					}
					gotoxy(45, 22); cout << YELLOW << "Esta cuenta ha sido eliminada";
					gotoxy(45, 23); system("pause");
					cliente->getCuentasBancarias()->removeElement(auxiliar);
					eliminar<ArbolAVL<CuentaBancaria*>*, CuentaBancaria>(auxiliar, cuentas);
					Console::Clear();
					break;
				}
				printToast("No se encontro la cuenta bancaria con el id ingresado", TERROR);
				
				Console::Clear();
				break;
			}
			printToast("No tiene cuentas bancarias", TINFO);
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
	vector<string> opciones = { 
		"Ver los Datos de la Cuenta", 
		"Cambiar contrasenia", 
		"Ver las operaciones de la cuenta", 
		"Agregar una Tarjeta a la cuenta", 
		"Eliminar la Tarjeta de la cuenta", 
		"Renovar Tarjeta", 
		"Realizar una operacion",
		"Realizar una compra con tarjeta",
		"Salir" };

	auto callback = [&](int opcion) {
		switch (opcion)
		{
		case 0:
		{
			if (CuentaB->getTarjeta() != nullptr) 
			{
				generarCvvDinamico(CuentaB->getTarjeta());
			}
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(45, 14); cout << "Id: " << CuentaB->getId();
			gotoxy(45, 15); cout << "Id Cliente: " << CuentaB->getIdCliente();
			gotoxy(45, 16); cout << "Numero de cuenta: " << CuentaB->getNumeroCuenta();
			gotoxy(45, 17); cout << "Contrasenia: " << CuentaB->getPassword();
			gotoxy(45, 18); cout << "Saldo: " << CuentaB->getSaldo();
			gotoxy(50, 19); cout << YELLOW << "Tarjeta:" << RESET;

			cout << endl;
			if (CuentaB->getTarjeta() != nullptr)
			{
				gotoxy(50, 21); cout << "Numero de tarjeta: " << CuentaB->getTarjeta()->getNumero();
				gotoxy(50, 22); cout << "Fecha de vencimiento: " << CuentaB->getTarjeta()->getFechaVencimientoMMAA();
				gotoxy(50, 23); cout << "CVV: " << CuentaB->getTarjeta()->getCvv();
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
					printToast("La contrasenia debe tener 4 digitos", TINFO);
					Console::Clear();
				}
			} while (auxiliar.size() != 4);
			CuentaB->setPassword(auxiliar);
			editar(CuentaB, cuentas);

			printToast("Contrasenia cambiada correctamente", TSUCCESS);
			Console::Clear();
			break;
		}
		case 2:
		{
			if (CuentaB->getOperaciones()->getSize() == 0) {
				printToast("No hay operaciones", TINFO);
			}
			else {
				gotoxy(0, 12); CuentaB->getOperaciones()->printPaginado();
			}
			Console::Clear();
			break;
		}
		case 3:
		{
			Console::Clear();
			LogoBCP(18, 1);
			if (CuentaB->getTarjeta() != nullptr)
			{
				printToast("La cuenta ya tiene una tarjeta asignada", TERROR);
			}
			else
			{
				Tarjeta* auxiliar = new Tarjeta(getNextId(tarjetas), CuentaB->getIdCliente(), CuentaB->getId());
				CuentaB->setTarjeta(auxiliar);
				insertar(auxiliar, tarjetas);
				printToast("Tarjeta agregada correctamente", TSUCCESS);
			}
			Console::Clear();
			break;
		}
		case 4:
		{
			Console::Clear();
			LogoBCP(18, 1);
			Tarjeta* auxiliar = buscar<ArbolAVL<Tarjeta*>*, Tarjeta>(CuentaB->getTarjeta()->getId(), tarjetas);
			if (auxiliar != nullptr)
			{
				CuentaB->borrarTarjeta();
				eliminar<ArbolAVL<Tarjeta*>*, Tarjeta>(auxiliar, tarjetas);
				printToast("La tarjeta ha sido removida correctamente", TSUCCESS);
				break;
			}
			printToast("Tu cuenta no tiene una tarjeta asignada", TERROR);
			Console::Clear();
			break;
		}
		case 5:
			Console::Clear();
			LogoBCP(18, 1);
			if (CuentaB->getTarjeta() != nullptr)
			{
				Tarjeta* auxiliar = new Tarjeta(getNextId(tarjetas), CuentaB->getIdCliente(), CuentaB->getId());
				CuentaB->setTarjeta(auxiliar);
				insertar<ArbolAVL<Tarjeta*>*, Tarjeta>(auxiliar, tarjetas);
				printToast("Tarjeta renovada correctamente", TSUCCESS);
			}
			else
			{
				printToast("No tiene tarjeta para renovar", TERROR);
			}
			Console::Clear();
			break;
		case 6:
			MenuElegirCanalOSede(CuentaB);
			break;
		case 7:
		{
			double monto;
			if (CuentaB->getTarjeta() != nullptr) {
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(45, 10); cout << "USAR TARJETA" << RESET;

				gotoxy(45, 12); cout << "Ingrese el monto a pagar: ";

				cin >> monto;

				if (CuentaB->getSaldo() < monto) {
					printToast("Saldo insuficiente", TERROR);
					break;
				}

				string contraseniaCB;

				Console::Clear();
				LogoBCP(18, 1);
				if (CuentaB->getTarjeta() != nullptr)
				{
					generarCvvDinamico(CuentaB->getTarjeta());
				}

				gotoxy(20, 10); cout << "Para continuar ingrese los datos de su tarjeta :" << endl;
				imprimirTarjetaVirtual(CuentaB->getTarjeta());

				system("pause");

				string numeroTarjeta, vencimiento, cvv;
				vector<string> opcionesTarjeta = { "Numero de tarjeta", "Vencimiento (MM/AA)", "CVV" };
				auto callback = [&](vector<string> valores) {
					numeroTarjeta = valores[0];

					vencimiento = valores[1];

					cvv = valores[2];

					return false;
					};
				crearFormulario(opcionesTarjeta, callback);
				if (!CuentaB->getTarjeta()->validarSiEstaVencida() && CuentaB->getTarjeta()->validarTarjeta(numeroTarjeta, vencimiento, cvv)) {
					Operacion* operacion = CuentaB->crearRetiro(getNextId(colaOperaciones), CuentaB->getNumeroCuenta(), monto, OTROCANAL, 0);
					if (operacion != nullptr)
					{
						encolar<Cola<Operacion*>*, Operacion>(operacion, colaOperaciones);
						printToast("Operacion en cola", TINFO);
					}
				}
				else {
					printToast("Tarjeta invalida o vencida", TERROR);

				}
			}
			else {
				printToast("No tiene tarjeta", TERROR);
			}
			//desencolar luego de cada encolamiento
			Operacion* opDesencolada = desencolar<Cola<Operacion*>*, Operacion>(colaOperaciones);
			if (opDesencolada != nullptr) {
				ejecutarOperacion(opDesencolada);
				opDesencolada->setId(getNextId(operaciones));
				agregar(opDesencolada, operaciones);

				if (opDesencolada->getEstado() == Realizada)
					printToast("Operacion realizada correctamente. Su SALDO actual es: " + to_string(CuentaB->getSaldo()), TSUCCESS);
				else
					printToast("Operacion no realizada. Su SALDO actual es: " + to_string(CuentaB->getSaldo()), TERROR);
			}
			break;
		}
		case 8:
			Console::Clear();
			return false;
			break;
		default:
			break;
		}
		if (CuentaB->getTarjeta() != nullptr)
		{
			generarCvvDinamico(CuentaB->getTarjeta());
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
			HashTablaLista<Sede*>* sedesDisponibles = buscarPorCantidadAleatorio<HashTablaLista<Sede*>, HashEntidadNodo<Sede*>*>(hashSedes, hashSedes->getSize() < 5 ? hashSedes->getSize() : 5);
			if (sedesDisponibles->head != nullptr) {
				vector<string> opcionesSedes;
				for (int i = 0; i < sedesDisponibles->getSize(); i++)
				{
					opcionesSedes.push_back(sedesDisponibles->getByPosition(i)->data->getNombre());
				}
				auto callback = [sedesDisponibles, &cuenta, this](int seleccion) {
					HashEntidadNodo<Sede*>* sede = sedesDisponibles->getByPosition(seleccion);
					if (sede != nullptr)
						MenuOperacionPorSede(sede->data, cuenta);
					return false;
					};

				crearMenu(opcionesSedes, callback);
			}
			else
			{
				printToast("No hay sedes disponibles", TERROR);
			}

			break;
		}
		case 1:
		{
			vector<Canal*> vectorCanales;
			vector<string> opcionesCanales;

			ArbolBinario<Canal*>* canalesSinSedeDisponibles = buscarCanalesSinSede();

			if (canalesSinSedeDisponibles->getRaiz() == nullptr)
			{
				printToast("No hay canales disponibles", TERROR);
				break;
			}
			
			// Lambda para insertar los datos del árbol en el vector
			auto insertarEnVector = [&](ArbolBinario<Canal*>* canalesSinSedeDisponibles) {
				if (canalesSinSedeDisponibles->getSize() == 0)
					return;
				canalesSinSedeDisponibles->EnOrden([&](Canal* canal) {
					vectorCanales.push_back(canal);
					opcionesCanales.push_back(canal->getNombre() + " - " + canal->getEstadoStr());
					});
				};

			//llamar lambda
			insertarEnVector(canalesSinSedeDisponibles);

			auto callback1 = [vectorCanales, &cuenta, this](int seleccion) {
				Canal* canal = vectorCanales[seleccion];
				if (canal != nullptr)
					MenuOperacionPorCanal(canal, cuenta);
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
        "Ver informacion de la sede - " + to_string(sede->getCanales()->getSize()) + " cajeros o ventanillas",
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
			if (ventanillas->head != nullptr) {
				vector<string> opcionesVentanillas;
				for (int i = 0; i < ventanillas->getSize(); i++)
				{
					opcionesVentanillas.push_back(
						ventanillas->getByPosition(i)->data->getNombre() + 
						" - " + 
						ventanillas->getByPosition(i)->data->getEstadoStr()
					);
				}
				auto callback1 = [&cuenta, ventanillas, this](int seleccion1) {
					Nodo<Canal*>* canal = ventanillas->getByPosition(seleccion1);
					if (canal != nullptr)
						MenuOperacionPorCanal(canal->data, cuenta);
					return false;
					};
				crearMenu(opcionesVentanillas, callback1);
			}
			else {
				printToast("No hay ventanillas disponibles", TERROR);
			}

			break;
		}
		case 2:
		{
			ListaDoble<Canal*>* cajeros = sede->buscarCanalesPorTipo(CAJERO);
			if (cajeros->head != nullptr) {
				vector<string> opcionesCajeros;
				for (int i = 0; i < cajeros->getSize(); i++)
				{
					opcionesCajeros.push_back(
						cajeros->getByPosition(i)->data->getNombre() + 
						" - " 
						+ cajeros->getByPosition(i)->data->getEstadoStr()
					);
				}
				auto callback2 = [&cuenta, cajeros, this](int seleccion2) {
					Nodo<Canal*>* canal = cajeros->getByPosition(seleccion2);
					if (canal != nullptr)
						MenuOperacionPorCanal(canal->data, cuenta);
					return false;
					};
				crearMenu(opcionesCajeros, callback2);
			}
			else {
				printToast("No hay cajeros disponibles", TERROR);
			}

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

	vector<string> opciones;
	if(canal->isActivo()) {
		opciones = {
			"Ver informacion del canal",
			"Transferir",
			"Depositar",
			"Retirar",
			"Usar Tarjeta para una operacion",
			"Salir"
		};
		auto menuActivo = [canal, &monto, &SN, &cuentaDestino, &cuentaOrigen, &operacion, cuenta, this](int seleccion) {
			if (cuenta->getTarjeta() != nullptr)
			{
				generarCvvDinamico(cuenta->getTarjeta());
			}
			switch (seleccion) {
			case 0:
				canal->print();
				gotoxy(45, 22); cout << RED << system("pause") << RESET;
				break;
			case 1:
			{
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(45, 10); cout << "TRANSFERENCIA" << RESET;

				gotoxy(45, 12); cout << "Su SALDO actual es: " << cuenta->getSaldo();


				gotoxy(45, 14); cout << "Transferir..." << RESET;


				gotoxy(45, 16); cout << "Ingrese el monto a transferir: ";

				cin >> monto;

				Console::Clear();
				LogoBCP(18, 1);

				gotoxy(45, 10); cout << "Ingrese la cuenta de destino: ";
				cin >> cuentaDestino;

				string contraseniaCB;
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(20, 10); cout << "Para continuar, ingrese la contrasenia de 4 digitos de su cuenta bancaria: ";

				cin >> contraseniaCB;

				if (cuenta->validarPassword(contraseniaCB) == false) {
					printToast("Contrasenia incorrecta", TERROR);
					break;
				}

				operacion = cuenta->crearTransferencia(getNextId(colaOperaciones), cuentaOrigen, cuentaDestino, monto, canal->getId(), canal->getIdSede());
				if (operacion != nullptr) 
				{
					colaOperaciones->encolar(operacion);
					printToast("Operacion en cola", TINFO);
				}
				
				break;
			}
			case 2:
			{
				// Lambda para generar un monto aleatorio
				auto generarMontoAleatorio = []() -> double {
					Random r;
					return r.Next(1, 1000);
					};
				double dineroEnMisBolsillos = generarMontoAleatorio();

				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(45, 10); cout << "DEPOSITO" << RESET;

				gotoxy(45, 12); cout << "Dinero en mis bolsillos: " << dineroEnMisBolsillos << endl;

				gotoxy(45, 14); cout << "Ingrese el monto a depositar: ";

				cin >> monto;

				if (monto > dineroEnMisBolsillos) {
					printToast("No tienes suficiente dinero en tus bolsillos", TERROR);
					break;
				}

				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(25, 10); cout << "Desea depositar a esta misma cuenta? (s: Si , n: A otra cuenta)";

				cin >> SN;
				if (SN == 's' || SN == 'S') {
					operacion = cuenta->crearDeposito(getNextId(colaOperaciones), cuentaOrigen, monto, canal->getId(), canal->getIdSede());
					if (operacion != nullptr) 
					{
						colaOperaciones->encolar(operacion);
						printToast("Operacion en cola", TINFO);
					}
				}
				else {
					Console::Clear();
					LogoBCP(18, 1);
					gotoxy(45, 10); cout << "Ingrese la cuenta de destino: ";

					cin >> cuentaDestino;

					operacion = cuenta->crearDeposito(getNextId(colaOperaciones), cuentaDestino, monto, canal->getId(), canal->getIdSede());
					if (operacion != nullptr) 
					{
						colaOperaciones->encolar(operacion);
						printToast("Operacion en cola", TINFO);
					}
				}
				
				break;
			}
			case 3:
			{
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(45, 10); cout << "RETIRO" << RESET;

				gotoxy(45, 12); cout << "Su SALDO actual es: " << cuenta->getSaldo();


				gotoxy(45, 14); cout << "Retirar..." << RESET;


				gotoxy(45, 16); cout << "Ingrese el monto a retirar: ";

				cin >> monto;

				string contraseniaCB;
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(20, 10); cout << "Para continuar, ingrese la contrasenia de 4 digitos de su cuenta bancaria: ";

				cin >> contraseniaCB;

				if (cuenta->validarPassword(contraseniaCB) == false) {
					printToast("Contrasenia incorrecta", TERROR);
					break;
				}

					operacion = cuenta->crearRetiro(getNextId(colaOperaciones), cuentaOrigen, monto, canal->getId(), canal->getIdSede());
				if (operacion != nullptr)
				{
					encolar<Cola<Operacion*>*, Operacion>(operacion, colaOperaciones);
					printToast("Operacion en cola", TINFO);
				}
				break;
			}
			case 4: {
				if (cuenta->getTarjeta() != nullptr) {
					Console::Clear();
					LogoBCP(18, 1);
					gotoxy(45, 10); cout << "USAR TARJETA" << RESET;

					gotoxy(45, 12); cout << "Ingrese el monto a pagar: ";

					cin >> monto;

					if (cuenta->getSaldo() < monto) {
						printToast("Saldo insuficiente", TERROR);
						break;
					}

					string contraseniaCB;

					Console::Clear();
					LogoBCP(18, 1);
					if (cuenta->getTarjeta() != nullptr)
					{
						generarCvvDinamico(cuenta->getTarjeta());
					}

					gotoxy(20, 10); cout << "Para continuar ingrese los datos de su tarjeta :" << endl;
					imprimirTarjetaVirtual(cuenta->getTarjeta());

					system("pause");

					gotoxy(20, 10); cout << "Para continuar ingrese los datos de su tarjeta :";
					string numeroTarjeta, vencimiento, cvv;
					vector<string> opcionesTarjeta = { "Numero de tarjeta", "Vencimiento (MM/AA)", "CVV" };
					auto callback = [&](vector<string> valores) {
						
						numeroTarjeta = valores[0];

						vencimiento = valores[1];

						cvv = valores[2];
							
						return false;
						};
					crearFormulario(opcionesTarjeta, callback);
					if (!cuenta->getTarjeta()->validarSiEstaVencida() && cuenta->getTarjeta()->validarTarjeta(numeroTarjeta, vencimiento, cvv)) {
						operacion = cuenta->crearRetiro(getNextId(colaOperaciones), cuentaOrigen, monto, canal->getId(), canal->getIdSede());
						if (operacion != nullptr)
						{
							encolar<Cola<Operacion*>*, Operacion>(operacion, colaOperaciones);
							printToast("Operacion en cola", TINFO);
						}
					}
					else {
						printToast("Tarjeta invalida o vencida", TERROR);

					}
				}
				else {
					printToast("No tiene tarjeta", TERROR);
				}
				break;
			}
			case 5:
				return false;
				break;
			};

			//desencolar luego de cada encolamiento
			Operacion* opDesencolada = desencolar<Cola<Operacion*>*, Operacion>(colaOperaciones);
			if (opDesencolada != nullptr) {
				ejecutarOperacion(opDesencolada);
				opDesencolada->setId(getNextId(operaciones));
				agregar(opDesencolada, operaciones);

				if (opDesencolada->getEstado() == Realizada)
					printToast("Operacion realizada correctamente. Su SALDO actual es: " + to_string(cuenta->getSaldo()), TSUCCESS);
				else
					printToast("Operacion no realizada. Su SALDO actual es: " + to_string(cuenta->getSaldo()), TERROR);
			}

			return true;
			};

		crearMenu(opciones, menuActivo);
	} else {
		opciones = {
			"Ver informacion del canal",
			"Este canal esta INACTIVO - Salir"
		};
		auto menuInactivo = [canal, &monto, &SN, &cuentaDestino, &cuentaOrigen, &operacion, cuenta, this](int seleccion) {
			switch (seleccion) {
			case 0:
				canal->print();
				gotoxy(45, 22); cout << RED << system("pause") << RESET;
				break;
			case 1:
				return false;
				break;
			};
			return true;
			};

		crearMenu(opciones, menuInactivo);
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
		"Ordenar por nombre",
		"Ordenar por estado",
		"Salir"
	};

	auto callback = [&](int seleccion) {
		switch (seleccion) {
		case 0: //Listar todos los canales
			canales->printTree(canales->getRaiz(), 1, [&](Canal* data){
				cout << "ID: " << data->getId() << " - " << data->getNombre() << endl;
			});
			cout << "-----------------------------------------------------------------" << endl;
			canales->EnOrden([&](Canal* data) {
				data->print();
				cout << endl;
				cout << "-----------------------------------------------------------------" << endl;
				});
			system("pause");
			break;
		case 1: //Listar los canales por tipo
		{
			ETipoDeCanal tipo;
			Sede* sede;
			cout << "Elija el tipo: ";

			vector<string> tipos = { "Ventanilla", "Agente", "Web", "App", "Yape", "Cajero" };

			auto callback = [&](int seleccion) {
				switch (seleccion) {
				case 0:
					tipo = VENTANILLA;
					break;
				case 1:
					tipo = AGENTE;
					break;
				case 2:
					tipo = WEB;
					break;
				case 3:
					tipo = APP;
					break;
				case 4:
					tipo = YAPE;
					break;
				case 5:
					tipo = CAJERO;
					break;
				}
				return false;
				};

			crearMenu(tipos, callback);

			//callback paa buscar por valor

			auto searchByValue = [this, &tipo](Canal* canal) {
				return canal->getTipoDeCanal() == tipo;
				};
			ArbolBinario<Canal*>* temp = canales->searchMultipleByValue(searchByValue);
			if (temp != nullptr) {
				if (temp->getRaiz() != nullptr) {
					temp->printTree(temp->getRaiz(), 1, [&](Canal* data) {
						cout << "ID: " << data->getId() << " - " << data->getNombre() << endl;
						});
					cout << "-----------------------------------------------------------------" << endl;
					temp->EnOrden([&](Canal* data) {
						data->print();
						cout << endl;
						cout << "-----------------------------------------------------------------" << endl;
						});
					system("pause");
				}
				else 
				{
					printToast("No se encontraron canales con ese tipo", TERROR);
				}
			}
			else {
				printToast("No se encontraron canales con ese tipo", TERROR);
			}
			delete temp;
			break;
		}
		case 2: //Listar los canales por sede
		{
			ETipoDeCanal tipo;
			Sede* sede;
			cout << "Elija la sede: ";
			vector<string> opcionesSedes;
			for (int i = 0; i < hashSedes->getSize(); i++)
			{
				opcionesSedes.push_back(hashSedes->getFormattedByPos([&](Sede* sede) { return to_string(sede->getId()) + " - " + sede->getNombre(); }, i));
			}
			auto callback = [&](int seleccion) {
				HashEntidadNodo<Sede*>* TempSede = hashSedes->getByPosition(seleccion);
				if (TempSede != nullptr)
					sede = TempSede->data;
				return false;
				};
			crearMenu(opcionesSedes, callback);

			auto searchByValue = [this, &sede](Canal* canal) {
				return canal->getIdSede() == sede->getId();
				};
			ArbolBinario<Canal*>* temp = canales->searchMultipleByValue(searchByValue);
			if (temp != nullptr) {
				if (temp->getRaiz() != nullptr) {
					temp->printTree(temp->getRaiz(), 1, [&](Canal* data) {
						cout << "ID: " << data->getId() << " - " << data->getNombre() << endl;
						});
					cout << "-----------------------------------------------------------------" << endl;
					temp->EnOrden([&](Canal* data) {
						data->print();
						cout << endl;
						cout << "-----------------------------------------------------------------" << endl;
						});
					system("pause");
				}
				else
				{
					printToast("No se encontraron canales con ese tipo", TERROR);
				}
			}
			else {
				printToast("No se encontraron canales con esa sede", TERROR);
			}
			delete temp;
			break;
		}
		case 3: //Agregar un canal
		{
			string nombre;
			ETipoDeCanal tipo = OTROCANAL;
			Sede* sede = nullptr;

			vector<string> campos = { "Nombre" };
			auto callback = [&](vector<string> valores) {
				nombre = valores[0];
				return false;
				};
			crearFormulario(campos, callback);
			
			cout << "Elija el tipo: ";

			vector<string> tipos = { "Ventanilla", "Agente", "Web", "App", "Yape", "Cajero" };

			auto callback1 = [&](int seleccion) {
				switch (seleccion) {
				case 0:
					tipo = VENTANILLA;
					break;
				case 1:
					tipo = AGENTE;
					break;
				case 2:
					tipo = WEB;
					break;
				case 3:
					tipo = APP;
					break;
				case 4:
					tipo = YAPE;
					break;
				case 5:
					tipo = CAJERO;
					break;
				}
				return false;
				};

			crearMenu(tipos, callback1);

			if (tipo != VENTANILLA && tipo != CAJERO) {
				cout << "Elija la sede: ";
				vector<string> opcionesSedes;
				for (int i = 0; i < hashSedes->getSize(); i++)
				{
					opcionesSedes.push_back(hashSedes->getFormattedByPos([&](Sede* sede) { return to_string(sede->getId()) + sede->getNombre(); }, i));
				}
				auto callback2 = [&](int seleccion) {
					HashEntidadNodo<Sede*>* TempSede = hashSedes->getByPosition(seleccion);
					if (TempSede != nullptr)
						sede = TempSede->data;
					return false;
					};
				crearMenu(opcionesSedes, callback2);
			}

			if (sede != nullptr) {
				Canal* nuevo_canal = new Canal(getNextId(canales), nombre, tipo, sede->getId());
				insertar(nuevo_canal, canales);
			}
			else {
				Canal* nuevo_canal_sin_sede = new Canal(getNextId(canales), nombre, tipo, 0);
				insertar(nuevo_canal_sin_sede, canales);
			}
			break;
		}
		case 4: //Editar un canal
		{
			string nombre;
			ETipoDeCanal tipo = OTROCANAL;
			Sede* sede;
			vector<string> nombresCanales;
			vector<Canal*> opcionesCanales;

			//lambda
			auto insertarEnVector = [&](Canal* data) {
				nombresCanales.push_back(data->getNombre());
				opcionesCanales.push_back(data);
				};

			canales->EnOrden(insertarEnVector);

			auto callback = [&](int seleccion) {
				Canal* canal = opcionesCanales[seleccion];
				if (canal != nullptr)
				{
					vector<string> campos = { 
						"Nombre - (Actualmente - " + canal->getNombre() + ")",
						"Tipo de Canal - (Actualmente - " + canal->getTipoDeCanalStr() + ")",
						"Agregar a una sede", 
						"Quitar de la sede"};
					auto menuOpcionesEdditables = [&](int seleccion) {

						switch (seleccion) {
						case 0:
						{
							vector<string> campoEditable;
							campoEditable.push_back(campos[seleccion]);
							auto callback = [&](vector<string> valores) {
								canal->setNombre(valores[0]);
								return false;
								};
							crearFormulario(campoEditable, callback);
							break;
						}
						case 1:
						{
							cout << "Elija el tipo: ";

							vector<string> tipos = { "Ventanilla", "Agente", "Web", "App", "Yape", "Cajero" };

							auto callback1 = [&](int seleccion) {
								switch (seleccion) {
								case 0:
									canal->setTipoDeCanal(VENTANILLA);
									break;
								case 1:
									canal->setTipoDeCanal(AGENTE);
									break;
								case 2:
									canal->setTipoDeCanal(WEB);
									break;
								case 3:
									canal->setTipoDeCanal(APP);
									break;
								case 4:
									canal->setTipoDeCanal(YAPE);
									break;
								case 5:
									canal->setTipoDeCanal(CAJERO);
									break;
								}
								return false;
								};

							crearMenu(tipos, callback1);
							break;
						}
						case 2:
						{
							if (tipo != VENTANILLA && tipo != CAJERO) {
								cout << "Elija la sede: ";
								vector<string> opcionesSedes;
								for (int i = 0; i < hashSedes->getSize(); i++)
								{
									opcionesSedes.push_back(hashSedes->getFormattedByPos([&](Sede* sede) { return to_string(sede->getId()) + " - " + sede->getNombre(); }, i));
								}
								auto callback2 = [&](int seleccion) {
									HashEntidadNodo<Sede*>* TempSede = hashSedes->getByPosition(seleccion);
									if (TempSede != nullptr)
									{
										canal->setIdSede(TempSede->data->getId());
									}
									return false;
									};
								crearMenu(opcionesSedes, callback2);
							}
							else {
								printToast("Este canal no puede tener sede", TINFO);
							}
							break;
						}
						case 3:
							if (tipo != VENTANILLA && tipo != CAJERO) canal->setIdSede(0);
							else {
								printToast("Este canal no tiene sede", TERROR);
							}
							break;
						}
						editar(canal, canales);
						printToast("Canal editado correctamente", TSUCCESS);


						return false;
						};
					crearMenu(campos, menuOpcionesEdditables);

				}
				
				return false;
				};
			crearMenu(nombresCanales, callback);
			break;
		}
		case 5: //Eliminar un canal
		{
			vector<string> nombresCanales;
			vector<Canal*> opcionesCanales;

			//lambda
			auto insertarEnVector = [&](Canal* data) {
				nombresCanales.push_back(data->getNombre());
				opcionesCanales.push_back(data);
				};

			canales->EnOrden(insertarEnVector);

			auto callback = [&](int seleccion) {
				Canal* canal = opcionesCanales[seleccion];
				if (canal != nullptr)
				{
					canales->eliminar(canal);
					canales->escribir();
					printToast("Canal eliminado correctamente", TSUCCESS);
				}
				return false;
				};

			crearMenu(nombresCanales, callback);
			break;
		}
		case 6: //Activar un canal
		{
			vector<string> nombresCanales;
			vector<Canal*> opcionesCanales;

			//lambda
			auto insertarEnVector = [&](Canal* data) {
				nombresCanales.push_back(data->getNombre());
				opcionesCanales.push_back(data);
				};

			canales->EnOrden(insertarEnVector);

			auto callback = [&](int seleccion) {
				Canal* canal = opcionesCanales[seleccion];
				if (canal != nullptr)
				{
					canal->activar();
					canales->escribir();
				}
				return false;
				};

			crearMenu(nombresCanales, callback);
			break;
		}
		case 7: //Desactivar un canal
		{
			vector<string> nombresCanales;
			vector<Canal*> opcionesCanales;

			//lambda
			auto insertarEnVector = [&](Canal* data) {
				nombresCanales.push_back(data->getNombre());
				opcionesCanales.push_back(data);
				};

			canales->EnOrden(insertarEnVector);

			auto callback = [&](int seleccion) {
				Canal* canal = opcionesCanales[seleccion];
				if (canal != nullptr)
				{
					canal->desactivar();
					canales->escribir();
				}
				return false;
				};

			crearMenu(nombresCanales, callback);
			break;
		}
		case 8: //Ordenar por nombre
			Canal::ordenarPorNombre<ArbolBinario<Canal*>*>(canales, true);
			canales->printTree(canales->getRaiz(), 1, [&](Canal* data) {
				cout << "ID: " << data->getId() << " - " << data->getNombre() << endl;
				});
			cout << "-----------------------------------------------------------------" << endl;
			canales->EnOrden([&](Canal* data) {
				data->print();
				cout << endl;
				cout << "-----------------------------------------------------------------" << endl;
				});
			system("pause");
			break;
		case 9: //Ordenar por estado
			Canal::ordenarPorEstado<ArbolBinario<Canal*>*>(canales, true);
			canales->printTree(canales->getRaiz(), 1, [&](Canal* data) {
				cout << "ID: " << data->getId() << " - " << data->getEstadoStr() << endl;
				});
			cout << "-----------------------------------------------------------------" << endl;
			canales->EnOrden([&](Canal* data) {
				data->print();
				cout << endl;
				cout << "-----------------------------------------------------------------" << endl;
				});
			system("pause");
			break;
		case 10:
			return false;
			break;
		}
		return true;
		};

	crearMenu(opciones, callback);
}

inline void Bcp::MenuOperaciones()
{
	vector<string> opciones = {
		"Listar todas las operaciones",
		"Listar las operaciones por tipo",
		"Listar las operaciones por estado",
		"Ordenar por fecha",
		"Ordenar por monto",
		"Salir"
	};

	TipoOperacion tipo;
	string cuenta;
	string canal;
	string sede;
	string fecha;
	double monto;
	EstadoOperacion estado;

	auto callback = [this, &tipo, &cuenta, &canal, &sede, &fecha, &monto, &estado](int seleccion) {
		switch (seleccion) {
		case 0:
			operaciones->printPaginado();
			system("pause");
			break;
		case 1:
		{
			cout << "Elija el tipo: ";

			vector<string> tipos = { "Transferencia", "Deposito", "Retiro" };

			auto callback = [&](int seleccion) {
				switch (seleccion) {
				case 0:
					tipo = Transferencia;
					break;
				case 1:
					tipo = Deposito;
					break;
				case 2:
					tipo = Retiro;
					break;
				}
				return false;
				};

			crearMenu(tipos, callback);

			//callback paa buscar por valor

			auto searchByValue = [this, &tipo](Operacion* op) {
				return op->getTipo() == tipo;
				};
			ListaDoble<Operacion*>* temp = operaciones->searchMultipleByValue(searchByValue);
			if (temp != nullptr) {
				if (temp->head != nullptr) temp->printPaginado(5);
				else printToast("No se encontraron operaciones con ese tipo", TERROR);
			}
			else {
				printToast("No se encontraron operaciones con ese tipo", TERROR);
			}
			delete temp;
			break;
		}
		case 2:
		{
			cout << "Elija el estado: ";

			vector<string> estados = { "Pendiente", "Aprobado", "Rechazado" };

			auto callback = [&](int seleccion) {
				switch (seleccion) {
				case 0:
					estado = Pendiente;
					break;
				case 1:
					estado = Realizada;
					break;
				case 2:
					estado = Rechazada;
					break;
				}
				return false;
				};

			crearMenu(estados, callback);

			//callback paa buscar por valor

			auto searchByValue = [this, &estado](Operacion* op) {
				return op->getEstado() == estado;
				};
			ListaDoble<Operacion*>* temp = operaciones->searchMultipleByValue(searchByValue);
			if (temp != nullptr) {
				if (temp->head != nullptr) temp->printPaginado(5);
				else printToast("No se encontraron operaciones con ese estado", TERROR);
			} else {
				printToast("No se encontraron operaciones con ese estado", TERROR);
			}
			delete temp;

			break;
		}
		case 3:
			Operacion::ordenarPorFecha<ListaDoble<Operacion*>*>(operaciones, true);
			operaciones->printPaginado(5);
			break;
		case 4:
			Operacion::ordenarPorMonto<ListaDoble<Operacion*>*>(operaciones, true);
			operaciones->printPaginado(5);
			break;
		case 5:
			return false;
			break;
		}

		return true;
		};

	crearMenu(opciones, callback);

}

inline void Bcp::MenuAdmin()
{
	vector<string> opciones = {
		"Clientes",
		"Cuentas Bancarias",
		"Tarjetas",
		"Operaciones",
		"Sedes",
		"Canales",
		"Salir"
	};

	auto callback = [&](int seleccion) {
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
			MenuSedes();
			break;
		case 5:
			MenuCanales();
			break;
		case 6:
			return false;
			break;
		};
		return true;
	};
	crearMenu(opciones, callback);
}

void Bcp::MenuClientes()
{
	vector<string> opciones = {
		"Listar todos los Clientes",
		"Ordenar los clientes por nombre ascendente",
		"Ordenar los clientes por apellido ascendente",
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
				gotoxy(0, 12); hashClientes->printPaginado();
				system("pause");
				break;
			case 1:
			{
				function<bool(Cliente*, Cliente*)> comparacion = [&](Cliente* aux1, Cliente* aux2)->bool
					{
						return aux1->getNombre() < aux2->getNombre();
					};
				hashClientes->QuickSort(comparacion, 0, hashClientes->getSize() - 1);
				break;
			}
			case 2:
			{
				function<bool(Cliente*, Cliente*)> comparacion = [&](Cliente* aux1, Cliente* aux2)->bool
					{
						return aux1->getApellido() < aux2->getApellido();
					};
				hashClientes->QuickSort(comparacion, 0, hashClientes->getSize() - 1);
				break;
			}
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

					HashEntidadNodo<Cliente*>* cliente = hashClientes->searchByValue([&](Cliente* cliente) {
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
					HashEntidadNodo<Cliente*>* cliente = hashClientes->searchByValue([&](Cliente* cliente) {
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
				Cliente* cliente = new Cliente(getNextId(hashClientes), nombre, apellido, direccion, telefono, email, password);
				agregar(cliente, hashClientes);

				printToast("Cuenta Registrada Correctamente", TSUCCESS);
				Console::Clear();
				break;
			}
			case 4:
			{
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(40, 12); cout << "Ingrese el id del cliente";
				gotoxy(40, 13); cin >> id;
				Cliente* cliente = buscar<HashTablaLista<Cliente*>*, Cliente>(id, hashClientes);

				if (cliente == nullptr)
				{
					printToast("No se encontro el cliente", TERROR);
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
						gotoxy(40, 14); cout << "Nombre anterior: " << cliente->getNombre();
						gotoxy(40, 15); cout << "Nuevo Nombre: ";
						getline(cin >> ws, aux);
						cliente->setNombre(aux);
						editar(cliente, hashClientes);
						gotoxy(40, 17); cout << "Nombre cambiado correctamente";
						gotoxy(40, 18); system("pause");
						Console::Clear();
						break;
					case 1:
						Console::Clear();
						LogoBCP(18, 1);
						gotoxy(40, 14); cout << "Apellido anterior: " << cliente->getApellido();
						gotoxy(40, 15); cout << "Nuevo telefono: "; 
						getline(cin >> ws, aux);
						cliente->setApellido(aux);
						editar(cliente, hashClientes);
						gotoxy(40, 17); cout << "Apellido cambiado correctamente";
						gotoxy(40, 18); system("pause");
						Console::Clear();
						break;
					case 2:
						Console::Clear();
						LogoBCP(18, 1);
						gotoxy(40, 14); cout << "Telefono anterior: " << cliente->getTelefono();
						gotoxy(40, 15); cout << "Nuevo telefono: "; 
						getline(cin >> ws, aux);
						cliente->setTelefono(aux);
						editar(cliente, hashClientes);
						gotoxy(40, 17); cout << "Telefono cambiado correctamente";
						gotoxy(40, 18); system("pause");
						Console::Clear();
						break;
					case 3:
						Console::Clear();
						LogoBCP(18, 1);
						gotoxy(40, 14); cout << "Direccion anterior: " << cliente->getDireccion();
						gotoxy(40, 15); cout << "Nueva direccion: ";
						getline(cin >> ws, aux);
						cliente->setDireccion(aux);
						editar(cliente, hashClientes);
						gotoxy(40, 17); cout << "Direccion cambiada correctamente";
						gotoxy(40, 18); system("pause");
						Console::Clear();
						break;
					case 4:
						Console::Clear();
						LogoBCP(18, 1);
						gotoxy(40, 14); cout << "Email anterior: " << cliente->getEmail();
						gotoxy(40, 15); cout << "Nuevo email: "; 
						getline(cin >> ws, aux);
						cliente->setEmail(aux);
						editar(cliente, hashClientes);
						gotoxy(40, 17); cout << "Email cambiado correctamente";
						gotoxy(40, 18); system("pause");
						Console::Clear();
						break;
					case 5:
						Console::Clear();
						LogoBCP(18, 1);
						gotoxy(40, 14); cout << "Contrasenia anterior: " << cliente->getPassword();
						gotoxy(40, 15); cout << "Nueva contrasenia: ";
						getline(cin >> ws, aux);
						cliente->setPassword(aux);
						editar(cliente, hashClientes);
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
				Cliente* aux = buscar<HashTablaLista<Cliente*>*, Cliente>(id, hashClientes);
				if (aux == nullptr)
				{
					printToast("No se encontro el cliente", TERROR);
					break;
				}
				aux->print();
				system("pause");
				break;
			}
			case 6:
			{
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(40, 12); cout << "Ingrese el id del cliente del que desea eliminar la tarjeta";
				gotoxy(40, 13); cin >> id;
				Cliente* aux = buscar<HashTablaLista<Cliente*>*, Cliente>(id, hashClientes);
				if (aux == nullptr)
				{
					printToast("No se encontro el cliente", TERROR);
					break;
				}
				ListaDoble<CuentaBancaria*>* cuentasDelCliente = aux->getCuentasBancarias();
				Nodo<CuentaBancaria*>* cuenta = cuentasDelCliente->head;
				while (cuenta != nullptr)
				{
					if (cuenta->data->getTarjeta() != nullptr)
					{
						eliminar(cuenta->data->getTarjeta(), tarjetas);
					}
					eliminar(cuenta->data, cuentas);
					cuenta = cuenta->next;
				}
				eliminar(aux, hashClientes);
				printToast("Cliente eliminado correctamente", TSUCCESS);
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

void Bcp::MenuCuentas()
{
	vector<string> opciones = {
		"Listar todas las Cuentas Bancarias",
		"Ordenar las cuentas por saldo ascendente",
		"Ordenar las cuentas por saldo descendente",
		"Agregar una Cuenta Bancaria a un Cliente",
		"Editar los datos de una Cuenta Bancaria",
		"Buscar una Cuenta Bancaria por id",
		"Eliminar una Cuenta Bancaria",
		"Menu Grafo Cuentas Bancarias y Operaciones",
		"Salir"
	};

	int id = 0;

	auto callback = [this,&id](int seleccion) {
		switch (seleccion) {
		case 0:
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(0, 12);
			cuentas->printTree(cuentas->getRaiz(), 1, [&](CuentaBancaria* data) {
				cout << "ID: " << data->getId() << " - " << data->getNumeroCuenta() << " - " << data->getSaldo() << endl;
				});
			cout << "-----------------------------------------------------------------" << endl;
			cuentas->EnOrden([&](CuentaBancaria* data) {
				data->print();
				cout << endl;
				cout << "-----------------------------------------------------------------" << endl;
				});
			system("pause");
			break;
		case 1:
		{
			function<bool(CuentaBancaria*, CuentaBancaria*)> comparacion = [&](CuentaBancaria* aux1, CuentaBancaria* aux2)->bool {
				return aux1->getSaldo() < aux2->getSaldo();
				};
			cuentas->ordenar(comparacion);
			break;
		}
		case 2:
		{
			function<bool(CuentaBancaria*, CuentaBancaria*)> comparacion = [&](CuentaBancaria* aux1, CuentaBancaria* aux2)->bool {
				return aux1->getSaldo() > aux2->getSaldo();
				};
			cuentas->ordenar(comparacion);
			break;
		}
		case 3://crear cuenta bancaria
		{
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el id del cliente";
			gotoxy(40, 13); cin >> id;
			Cliente* aux = buscar<HashTablaLista<Cliente*>*, Cliente>(id, hashClientes);
			if (aux == nullptr)
			{
				printToast("No se encontro el cliente", TERROR);
				break;
			}
			string numero_cuenta;//14 digitos
			string contrasenia;
			bool verificacion;

			do
			{
				numero_cuenta = "";
				verificacion = false;
				for (int i = 0; i < 14; i++)
				{
					Random r;
					numero_cuenta.append(to_string(r.Next(0, 9)));
					_sleep(100);
				}

				//validar si ya existe sino volvera a crear un nuevo numero de 14 digitos
				NodoArbol<CuentaBancaria*>* verificacionCB = cuentas->searchMultipleByValue([&](CuentaBancaria* cb) {
					return cb->getNumeroCuenta() == numero_cuenta;
					})->getRaiz();
				if (verificacionCB != nullptr)
					verificacion = true;
			} while (verificacion);
			do
			{
				Console::Clear();
				gotoxy(30, 12); cout << "Ingrese la contrasenia para la cuenta (4 digitos)";
				gotoxy(50, 14); cin >> contrasenia;
				if (contrasenia.size() != 4)
				{
					Console::Clear();
					LogoBCP(18, 1);
				}
			} while (contrasenia.size() != 4);
			CuentaBancaria* nuevaCuentaBancaria = new CuentaBancaria(getNextId(cuentas), aux->getId(), contrasenia, numero_cuenta, 0);
			aux->agregarCuentaBancaria(nuevaCuentaBancaria);
			insertar(nuevaCuentaBancaria, cuentas);
			printToast("Cuenta bancaria creada correctamente", TSUCCESS);
			Console::Clear();
			break;
		}
		case 4: //actualizar cuenta bancaria
		{
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el id del cliente";
			gotoxy(40, 13); cin >> id;
			Cliente* aux = buscar<HashTablaLista<Cliente*>*, Cliente>(id, hashClientes);
			if (aux == nullptr)
			{
				printToast("No se encontro el cliente", TERROR);
				break;
			}
			Console::Clear();


			vector<string> nombreCuentas;

			ListaDoble<CuentaBancaria*>* cuentasDelCliente = aux->getCuentasBancarias();

			Nodo<CuentaBancaria*>* temp = cuentasDelCliente->head;
			while (temp != nullptr)
			{
				nombreCuentas.push_back(to_string(temp->data->getId()) + " - " + temp->data->getNumeroCuenta() + ", " + "SALDO: " + to_string(temp->data->getSaldo()));
				temp = temp->next;
			}
			if (nombreCuentas.size() > 0) {
				auto callback1 = [&](int opcion) {
					CuentaBancaria* cuentaBancaria = cuentas->search(cuentasDelCliente->getByPosition(opcion)->data->getId())->data;
					vector<string> ops = { 
						"Contrasenia",
						"Saldo - (Actualmente - " + to_string(cuentaBancaria->getSaldo()) + ")",
						"Salir"};
					int aux_int = 0;

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
									gotoxy(40, 12); cout << "Ingrese la nueva contrasenia de 4 digitos";
									gotoxy(40, 13); cin >> aux_str;
								} while (aux_str.length() != 4);
								cuentaBancaria->setPassword(aux_str);
								editar(cuentaBancaria, cuentas);

								printToast("Contrasenia cambiada correctamente", TSUCCESS);

								break;
							}
							case 1:
							{
								double saldo = 0;
								Console::Clear();
								gotoxy(40, 12); cout << "Ingrese la cantidad de saldo a sumar en la cuenta";
								gotoxy(40, 13); cin >> saldo;
								cuentaBancaria->setSaldo(cuentaBancaria->getSaldo() + saldo);
								editar(cuentaBancaria, cuentas);

								printToast("Saldo aumentado en " + to_string(saldo) + " correctamente", TSUCCESS);

								break;
							}
							case 2:
								return false;
								break;
							}
							return true;
						};
					crearMenu(ops, callback_ops);
					return false;
					};
				crearMenu(nombreCuentas, callback1);
			}
			else {
				printToast("No se encontraron cuentas bancarias", TERROR);
			}
			break;
		}
		case 5:
		{
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el id de la cuenta a buscar";
			gotoxy(40, 13); cin >> id;
			CuentaBancaria* aux = buscar<ArbolAVL<CuentaBancaria*>*, CuentaBancaria>(id, cuentas);
			if (aux == nullptr)
			{
				printToast("No se encontro la cuenta bancaria", TERROR);
				break;
			}
			aux->print();
			system("pause");
			break;
		}
		case 6:
		{
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el id de la cuenta a eliminar";
			gotoxy(40, 13); cin >> id;
			CuentaBancaria* aux = buscar<ArbolAVL<CuentaBancaria*>*,CuentaBancaria>(id, cuentas);
			if (aux == nullptr)
			{
				printToast("No se encontro la cuenta bancaria", TERROR);
				break;
			}
			Tarjeta* tarjeta = aux->getTarjeta();
			if (tarjeta != nullptr)
			{
				eliminar(tarjeta, tarjetas);
				aux->borrarTarjeta();
			}
			eliminar(aux, cuentas);
			printToast("Cuenta eliminada correctamente", TSUCCESS);
			break;
		}
		case 7:
			MenuGrafoCuentasBancariasYOperaciones();
			break;
		case 8:
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
		"Ordenar tarjetas por fecha de vencimiento ascendente",
		"Ordenar tarjetas por fecha de vencimiento descendente",
		"Ordenar tarjetas por fecha de creacion ascendente",
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
			gotoxy(0, 12);
			tarjetas->printTree(tarjetas->getRaiz(), 1, [&](Tarjeta* data) {
				cout << "ID: " << data->getId() << " - " << data->getNumero() << " - " << data->getFechaVencimientoStr() << endl;
				});
			cout << "-----------------------------------------------------------------" << endl;
			tarjetas->EnOrden([&](Tarjeta* data) {
				data->print();
				cout << endl;
				cout << "-----------------------------------------------------------------" << endl;
				});
			system("pause");
			break;
		case 1:
		{
			auto comparacion = [&](Tarjeta* aux1, Tarjeta* aux2) {
				return aux1->getFechaVencimiento() > aux2->getFechaVencimiento();
				};
			tarjetas->ordenar(comparacion);
			break;
		}
		case 2:
		{
			auto comparacion = [&](Tarjeta* aux1, Tarjeta* aux2) {
				return aux1->getFechaVencimiento() < aux2->getFechaVencimiento();
				};
			tarjetas->ordenar(comparacion);
			break;
		}
		case 3:
		{
			auto comparacion = [&](Tarjeta* aux1, Tarjeta* aux2) {
				return aux1->getFechaCreacion() > aux2->getFechaCreacion();
			};
			tarjetas->ordenar(comparacion);
			break;
		}
		case 4:
		{
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el id del Cliente";
			gotoxy(40, 13); cin >> id;
			Cliente* aux = buscar<HashTablaLista<Cliente*>*, Cliente>(id, hashClientes);
			if (aux != nullptr)
			{
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(40, 12); cout << "Ingrese el id de la Cuenta Bancaria al que desee agregar una Tarjeta";
				gotoxy(40, 13); cin >> id;
				Nodo<CuentaBancaria*>* aux_CB = aux->getCuentasBancarias()->search(id);
				if (aux_CB != nullptr)
				{
					aux_CB->data->loadTarjeta();
					if (aux_CB->data->getTarjeta() == nullptr)
					{
						Tarjeta* auxiliar = new Tarjeta(getNextId(tarjetas), aux_CB->data->getIdCliente(), aux->getId());
						aux_CB->data->setTarjeta(auxiliar);
						insertar(auxiliar, tarjetas);
						printToast("Tarjeta agregada correctamente", TSUCCESS);
					}
					else
					{
						printToast("La cuenta bancaria ya tiene una tarjeta", TERROR);

					}
				}
				else
				{
					printToast("No se encontro la cuenta bancaria", TERROR);
				}
			}
			else
			{
				printToast("No se encontro el cliente", TERROR);
			}
			break;
		}
		case 5:
		{
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el id de la tarjeta a editar";
			gotoxy(40, 13); cin >> id;
			Tarjeta* tarjeta = buscar<ArbolAVL<Tarjeta*>*, Tarjeta>(id, tarjetas);
			Console::Clear();
			LogoBCP(18, 1);
			if (tarjeta == nullptr)
			{
				printToast("No se encontro la tarjeta", TERROR);
				break;
			}
			Random r(time(NULL));
			tarjeta->setCvv(to_string(r.Next(101,1000)-1));
			editar(tarjeta, tarjetas);
			printToast("CVV cambiado correctamente", TSUCCESS);
			break;
		}
		case 6:
		{
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el id de la tarjeta";
			gotoxy(40, 13); cin >> id;
			Tarjeta* tarjeta = buscar<ArbolAVL<Tarjeta*>*, Tarjeta>(id, tarjetas);
			if (tarjeta == nullptr)
			{
				printToast("No se encontro la tarjeta", TERROR);
				break;
			}
	
			tarjeta->print();
			system("pause");
			break;
		}
		case 7:
		{
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el id de la tarjeta a eliminar";
			gotoxy(40, 13); cin >> id;
			Tarjeta* tarjeta = buscar<ArbolAVL<Tarjeta*>*, Tarjeta>(id, tarjetas);
			if (tarjeta == nullptr) {
				printToast("No se encontro la tarjeta", TERROR);
				break;
			}
			CuentaBancaria* aux_cuenta = buscar<ArbolAVL<CuentaBancaria*>*, CuentaBancaria>(tarjeta->getIdCuentaBancaria(), cuentas);

			if (aux_cuenta != nullptr)
			{
				if (tarjeta != nullptr)
				{
					aux_cuenta->borrarTarjeta();
				}
			}
			eliminar(tarjeta, tarjetas);

			printToast("Tarjeta eliminada correctamente", TSUCCESS);
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

inline void Bcp::MenuGrafoCuentasBancariasYOperaciones()
{
	auto validacionArco = [](CuentaBancaria*a, CuentaBancaria* b, Operacion* arista) {
		return a->getId() == arista->getIdCuentaBancariaOrigen() && b->getId() == arista->getIdCuentaBancariaDestino();
		};
	Grafo<CuentaBancaria*, Operacion*>* grafo = new Grafo<CuentaBancaria*, Operacion*>(validacionArco);

	cuentas->EnOrden([&](CuentaBancaria* data) {
		grafo->insertar(data);
		});

	operaciones->recorridoEnOrden([&](Operacion* data) {
		grafo->enlazarAutomatico(data);
		});

	vector<string> opciones = {
		"Listar todas las Cuentas Bancarias",
		"Listar todas las Operaciones",
		"Listar todas las Operaciones de una Cuenta Bancaria",
		"Ver Grafo de Cuentas Bancarias y Operaciones",
		"Salir"
	};

	auto callback = [&](int seleccion) {
		switch (seleccion)
		{
		case 0:
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(0, 12);
			cuentas->printTree(cuentas->getRaiz(), 1, [&](CuentaBancaria* data) {
				cout << "ID: " << data->getId() << " - " << data->getNumeroCuenta() << " - " << data->getSaldo() << endl;
				});
			cout << "-----------------------------------------------------------------" << endl;
			cuentas->EnOrden([&](CuentaBancaria* data) {
				data->print();
				cout << endl;
				cout << "-----------------------------------------------------------------" << endl;
				});
			system("pause");
			break;
		case 1:
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(0, 12);
			operaciones->printPaginado(5);
			break;
		case 2:
		{
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el id de la cuenta bancaria";
			gotoxy(40, 13); int id; cin >> id;
			CuentaBancaria* cuenta = buscar<ArbolAVL<CuentaBancaria*>*, CuentaBancaria>(id, cuentas);
			if
				(cuenta == nullptr)
			{
				printToast("No se encontro la cuenta bancaria", TERROR);
				break;
			}
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(0, 12);
			cuenta->print();
			cout << "-----------------------------------------------------------------" << endl;
			grafo->printAristasDeVertice(cuenta, [&](Operacion* op) {return "Transferencia: ID: " + to_string(op->getId()) + ", Monto: " + to_string(op->getMonto()); });
			system("pause");
			break;
		}
		case 3:
		{
			auto callbackGrafo = [&](int seleccion) {
				switch (seleccion)
				{
				case 0:
					Console::Clear();
					LogoBCP(18, 1);
					grafo->printVertices([&](CuentaBancaria* cuenta) {
						return "Cuenta Bancaria: ID: " + to_string(cuenta->getId()) + ", Nro: " + cuenta->getNumeroCuenta();
						});
					system("pause");
					break;
				case 1:
					Console::Clear();
					LogoBCP(18, 1);
					grafo->printAristas([&](Operacion* op) {return "Transferencia: ID: " + to_string(op->getId()) + ", Monto: " + to_string(op->getMonto()); });
					system("pause");
					break;
				case 2:
					Console::Clear();
					LogoBCP(18, 1);
					grafo->printGrafo([&](Operacion* op) {return "Transferencia: ID: " + to_string(op->getId()) + ", Monto: " + to_string(op->getMonto()); });
					system("pause");
					break;
				case 3:
					return false;
					break;
				}
				return true;
				};

			vector<string> opcionesGrafo = {
				"Listar Vertices",
				"Listar Aristas",
				"Listar Grafo",
				"Salir"
			};

			crearMenu(opcionesGrafo, callbackGrafo);
			break;
		}
		case 4:
			return false;
			break;
		}
		return true;
		};

	crearMenu(opciones, callback);

	delete grafo;
}

inline HashTablaLista<Cliente*>* Bcp::MenuElegirBusquedaDeClientePorValorOPorId()
{
	HashTablaLista<Cliente*>* clientesFiltrados = new HashTablaLista<Cliente*>("");
	vector<string> opciones = {
		"Buscar por id",
		"Buscar por valor",
		"Salir"
	};

	auto callback = [&](int seleccion) {
		switch (seleccion)
		{
		case 0:
		{
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el id a buscar";
			gotoxy(40, 13); int id; cin >> id;
			Cliente* cliente = buscar<HashTablaLista<Cliente*>*, Cliente>(id, hashClientes);
			if (cliente != nullptr)
			{
				clientesFiltrados->push_back(cliente, cliente->getId());
			}
			else
			{
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(40, 12); cout << "No se encontro el cliente";
				gotoxy(40, 13); system("pause");
			}
			break;
		}
		case 1:
		{
			Console::Clear();
			string valor;
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el valor a buscar";
			gotoxy(40, 13); getline(cin >> ws, valor);
			clientesFiltrados = hashClientes->searchMultipleByValue([&](Cliente* cliente) {
				return compararConValor(cliente->getNombre(), valor) ||
					compararConValor(cliente->getApellido(), valor) ||
					compararConValor(cliente->getDireccion(), valor) ||
					compararConValor(cliente->getTelefono(), valor) ||
					compararConValor(cliente->getEmail(), valor);
				});
			if (clientesFiltrados->getSize() == 0)
			{
				printToast("No se encontraron clientes", TERROR);
			}
			break;
		}
		case 2:
			return false;
			break;
		}
		return false;
		};

	crearMenu(opciones, callback);

	return clientesFiltrados;
}

inline HashTablaLista<Sede*>* Bcp::MenuElegirBusquedaDeSedePorValorOPorId()
{
	HashTablaLista<Sede*>* sedesFiltradas = new HashTablaLista<Sede*>("");
	vector<string> opciones = {
		"Buscar por id",
		"Buscar por valor",
		"Salir"
	};

	auto callback = [&](int seleccion) {
		switch (seleccion)
		{
		case 0:
		{
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el id a buscar";
			gotoxy(40, 13); int id; cin >> id;
			Sede* sede = buscar<HashTablaLista<Sede*>*, Sede>(id, hashSedes);
			if (sede != nullptr)
			{
				sedesFiltradas->push_back(sede, sede->getId());
			}
			else
			{
				printToast("No se encontro la sede", TERROR);
			}
			break;
		}
		case 1:
		{
			Console::Clear();
			string valor;
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese el valor a buscar";
			gotoxy(40, 13); getline(cin >> ws, valor);
			sedesFiltradas = hashSedes->searchMultipleByValue([&](Sede* sede) {
				return compararConValor(sede->getNombre(), valor) ||
					compararConValor(sede->getDireccion(), valor) ||
					compararConValor(sede->getCiudad(), valor) ||
					compararConValor(sede->getDistrito(), valor) ||
					compararConValor(sede->getDepartamento(), valor) ||
					compararConValor(sede->getTelefono(), valor) ||
					compararConValor(sede->getEstadoStr(), valor) ||
					compararConValor(sede->getEmail(), valor);
				});
			if (sedesFiltradas->getSize() == 0)
			{
				Console::Clear();
				LogoBCP(18, 1);
				gotoxy(40, 12); cout << "No se encontraron sedes";
				gotoxy(40, 13); system("pause");
			}
			break;
		}
		case 2:
			return false;
			break;
		}
		return false;
		};

	crearMenu(opciones, callback);

	return sedesFiltradas;
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
		"Ordenar por nombre",
		"Salir"
	};
	string ciudad = "";
	string departamento = "";
	int id;

	auto callback = [this, &ciudad, &departamento, &id](int seleccion) {
		switch (seleccion) {
		case 0:
			hashSedes->printPaginado();
			system("pause");
			break;
		case 1:
		{
			Console::Clear();
			LogoBCP(18, 1);
			gotoxy(40, 12); cout << "Ingrese nombre de la ciudad: ";

			getline(cin >> ws, ciudad);
			HashTablaLista<Sede*>* sedesPorCiudad = hashSedes->searchMultipleByValue([ciudad](Sede* sede) { return sede->getCiudad() == ciudad; });

			if (sedesPorCiudad->getSize() == 0)
			{
				printToast("No se encontraron sedes", TERROR);
			}
			else {
				sedesPorCiudad->printPaginado(10);
			}

			break;
		}

		case 2:
		{
			cout << "Ingrese el departamento: ";

			getline(cin >> ws, departamento);
			HashTablaLista<Sede*>* sedesPorDepartamento = hashSedes->searchMultipleByValue([ciudad](Sede* sede) { return sede->getDepartamento() == ciudad; });

			if (sedesPorDepartamento->getSize() == 0)
			{
				printToast("No se encontraron sedes", TERROR);
			}
			else {
				sedesPorDepartamento->printPaginado(10);
			}
			break;
		}

		case 3:
		{
			vector<string> campos = { 
				"Nombre", 
				"Direccion", 
				"Ciudad", 
				"Distrito", 
				"Departamento", 
				"Telefono", 
				"Email" };
			auto callback = [this](vector<string> valores) {
				agregar(new Sede(getNextId(hashSedes), valores[0], valores[1], valores[2], valores[3], valores[4], valores[5], valores[6]), hashSedes);
				return false;
				};
			crearFormulario(campos, callback);
			break;
		}
		case 4: //editar sede
		{
			HashTablaLista<Sede*>* sedesFiltradas = MenuElegirBusquedaDeSedePorValorOPorId();
			if (sedesFiltradas->getSize() == 0)
			{
				break;
			}

			vector<string> opcionesSedes;
			for (int i = 0; i < sedesFiltradas->getSize(); i++)
			{
				opcionesSedes.push_back(sedesFiltradas->getFormattedByPos([&](Sede* sede) { return to_string(sede->getId()) + " - " + sede->getNombre(); }, i));
			}
			auto callback = [&](int seleccion) {
				HashEntidadNodo<Sede*>* sede = sedesFiltradas->getByPosition(seleccion);
				if (sede != nullptr)
				{
					vector<string> campos = { 
						"Nombre - (Actualmente - " + sede->data->getNombre() + ")",
						"Direccion - (Actualmente - " + sede->data->getDireccion() + ")",
						"Ciudad - (Actualmente - " + sede->data->getCiudad() + ")",
						"Distrito - (Actualmente - " + sede->data->getDistrito() + ")",
						"Departamento - (Actualmente - " + sede->data->getDepartamento() + ")",
						"Telefono - (Actualmente - " + sede->data->getTelefono() + ")",
						"Email - (Actualmente - " + sede->data->getEmail() + ")"
					};
					auto menuOpcionesEdditables = [&](int seleccion) {
						vector<string> campoEditable;
						campoEditable.push_back(campos[seleccion]);
						auto callback = [&](vector<string> valores) {
							switch (seleccion) {
							case 0:
								sede->data->setNombre(valores[0]);
								break;
							case 1:
								sede->data->setDireccion(valores[0]);
								break;
							case 2:
								sede->data->setCiudad(valores[0]);
								break;
							case 3:
								sede->data->setDistrito(valores[0]);
								break;
							case 4:
								sede->data->setDepartamento(valores[0]);
								break;
							case 5:
								sede->data->setTelefono(valores[0]);
								break;
							case 6:
								sede->data->setEmail(valores[0]);
								break;
							}
							editar(sede->data, hashSedes);

							printToast("Sede editada correctamente", TSUCCESS);

							return false;
							};
						crearFormulario(campoEditable, callback);
						return false;
						};
					crearMenu(campos, menuOpcionesEdditables);
				}
				return false;
				};
			crearMenu(opcionesSedes, callback);

			break;
		}
		case 5:
		{
			HashTablaLista<Sede*>* sedesFiltradas = MenuElegirBusquedaDeSedePorValorOPorId();
			if (sedesFiltradas->getSize() == 0)
			{
				break;
			}

			vector<string> opcionesSedes;
			for (int i = 0; i < sedesFiltradas->getSize(); i++)
			{
				opcionesSedes.push_back(sedesFiltradas->getFormattedByPos([&](Sede* sede) { return to_string(sede->getId()) + " - " + sede->getNombre(); }, i));
			}
			auto callback = [&](int seleccion) {
				HashEntidadNodo<Sede*>* sede = sedesFiltradas->getByPosition(seleccion);
				if (sede != nullptr)
					eliminar(sede->data, hashSedes);
				return false;
				};
			crearMenu(opcionesSedes, callback);

			break;
		}
		case 6:
		{

			HashTablaLista<Sede*>* sedesFiltradas = MenuElegirBusquedaDeSedePorValorOPorId();
			if (sedesFiltradas->getSize() == 0)
			{
				break;
			}

			vector<string> opcionesSedes;
			for (int i = 0; i < sedesFiltradas->getSize(); i++)
			{
				opcionesSedes.push_back(sedesFiltradas->getFormattedByPos([&](Sede* sede) { return to_string(sede->getId()) + " - " + sede->getNombre(); }, i));
			}
			auto callback = [&](int seleccion) {
				HashEntidadNodo<Sede*>* sede = sedesFiltradas->getByPosition(seleccion);
				if (sede != nullptr)
				{
					sede->data->activar();
					editar(sede->data, hashSedes);

					printToast("Sede activada correctamente", TSUCCESS);
				}

				return false;
				};
			crearMenu(opcionesSedes, callback);

			break;
		}
		case 7:
		{
			HashTablaLista<Sede*>* sedesFiltradas = MenuElegirBusquedaDeSedePorValorOPorId();
			if (sedesFiltradas->getSize() == 0)
			{
				break;
			}

			vector<string> opcionesSedes;
			for (int i = 0; i < sedesFiltradas->getSize(); i++)
			{
				opcionesSedes.push_back(sedesFiltradas->getFormattedByPos([&](Sede* sede) { return to_string(sede->getId()) + " - " + sede->getNombre(); }, i));
			}
			auto callback = [&](int seleccion) {
				HashEntidadNodo<Sede*>* sede = sedesFiltradas->getByPosition(seleccion);
				if (sede != nullptr) {
					sede->data->desactivar();
					editar(sede->data, hashSedes);

					printToast("Sede desactivada correctamente", TSUCCESS);
				}
					
				return false;
				};
			crearMenu(opcionesSedes, callback);

			break;
		}
		case 8:
			Sede::ordenarPorNombre<HashTablaLista<Sede*>*>(hashSedes, true);
			hashSedes->printPaginado(5);
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

		int screenWidth = SCREEN_WIDTH; // Ancho de la pantalla (puedes ajustar esto seg�n sea necesario)
		int screenHeight = SCREEN_HEIGHT; // Alto de la pantalla (puedes ajustar esto seg�n sea necesario)
		int menuHeight = opciones.size();
		int startY = 11;

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
		int screenWidth = SCREEN_WIDTH; // Ancho de la pantalla (puedes ajustar esto seg�n sea necesario)
		int screenHeight = SCREEN_HEIGHT; // Alto de la pantalla (puedes ajustar esto seg�n sea necesario)
		int menuHeight = campos.size();
		int startY = 11;

		for (size_t i = 0; i < campos.size(); ++i) {
			int startX = ((screenWidth - campos[i].length()) / 2) - 10;
			gotoxy(startX, startY + i);
			cout << campos[i] << ": ";
			getline(cin >> ws, valores[i]);
		}
		

		continuar = callback(valores);

	}
}
