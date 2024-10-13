#include "pch.h"
#include <stdlib.h>
#include "Bcp.h"

using namespace System;
using namespace std;

void mostrarMenuCuentaBancaria(Bcp* bcp, Cliente* cliente, CuentaBancaria* cuenta) {
	int opcion;
	double monto;
	string cuentaDestino;
	string cuentaOrigen = cuenta->getNumeroCuenta();
	Operacion* operacion = new Operacion();
	do {
		cout << "------------------------------------" << endl; // "------------------------------------
		cout << "Bienvenido " << cliente->getNombre() << " " << cliente->getApellido() << endl;
		cout << "Menu Cuentas:" << endl;
		cout << "------------------------------------" << endl; // "------------------------------------
		cout << "1. Consultar Saldo" << endl;
		cout << "2. Transferir" << endl;
		cout << "3. Retirar" << endl;
		cout << "4. Depositar" << endl;
		cout << "5. Salir" << endl;
		cout << "Seleccione una opcion: ";
		cin >> opcion;

		switch (opcion) {
		case 1:
			cout << "Consultando saldo..." << endl;
			cout << "Saldo: " << cuenta->getSaldo() << endl;
			break;
		case 2:
			cout << "Transferir..." << endl;

			cout << "Ingrese el monto a transferir: ";
			cin >> monto;

			cout << "Ingrese la cuenta de destino: ";
			cin >> cuentaDestino;

			operacion = bcp->transferir(bcp->getLastId(bcp->getColaOperaciones()), cuentaOrigen, cuentaDestino, monto, nullptr, nullptr);
			if (operacion != nullptr) bcp->getColaOperaciones()->encolar(operacion);
			system("pause");
			break;
		case 3:
			cout << "Retirar..." << endl;

			cout << "Ingrese el monto a retirar: ";
			cin >> monto;

			operacion = bcp->retirar(bcp->getLastId(bcp->getColaOperaciones()), cuentaOrigen, monto, nullptr, nullptr);
			if (operacion != nullptr) bcp->getColaOperaciones()->encolar(operacion);
			system("pause");
			break;
		case 4:
			cout << "Depositar..." << endl;

			cout << "Ingrese el monto a depositar: ";
			cin >> monto;

			cout << "Ingrese la cuenta de destino: ";
			cin >> cuentaDestino;

			operacion = bcp->depositar(bcp->getLastId(bcp->getColaOperaciones()), cuentaDestino, monto, nullptr, nullptr);
			if(operacion != nullptr) bcp->getColaOperaciones()->encolar(operacion);
			system("pause");
			break;
		case 5:
			cout << "Saliendo..." << endl;
			break;
		default:
			cout << "Opcion invalida. Intente de nuevo." << endl;
		}

	// desencolar
		Operacion* opDesencolada = bcp->getColaOperaciones()->desencolar();
		if (opDesencolada != nullptr) {
			bcp->ejecutarOperacion(opDesencolada);
			opDesencolada->setId(bcp->getLastId(bcp->getOperaciones()));
			bcp->agregar(opDesencolada, bcp->getOperaciones());
		}

	} while (opcion != 5);
}

void mostrarMenuCliente(Bcp* bcp, Cliente* cliente) {
	int opcion;
	ListaDoble<CuentaBancaria*>* cuentasCliente = bcp->buscarCuentaPorIdCliente(cliente->getId());
	int opcionCuenta;
	CuentaBancaria* cuenta = new CuentaBancaria();
	do {
		cout << "------------------------------------" << endl; // "------------------------------------
		cout << "Bienvenido " << cliente->getNombre() << " " << cliente->getApellido() << endl;
		cout << "Menu Cliente:" << endl;
		cout << "------------------------------------" << endl; // "------------------------------------
		cout << "1. Ver mis cuentas" << endl;
		cout << "2. Acceder a una cuenta" << endl;
		cout << "3. Crear una cuenta" << endl;
		cout << "4. Eliminar una cuenta" << endl;
		cout << "5. Salir" << endl;
		cout << "Seleccione una opcion: ";
		cin >> opcion;

		switch (opcion) {
		case 1:
			cout << "Ver mis cuentas..." << endl;
			cuentasCliente = bcp->buscarCuentaPorIdCliente(cliente->getId());
			cuentasCliente->print();
			break;
		case 2:
			cout << "Acceder a una cuenta..." << endl;
			cuentasCliente->print();
			cout << "Seleccione una cuenta: ";
			cin >> opcionCuenta;
			cuenta = cuentasCliente->search(opcionCuenta)->data;
			if (cuenta != nullptr)
				mostrarMenuCuentaBancaria(bcp, cliente, cuenta);

			break;
		case 3:
			cout << "Crear una cuenta..." << endl;
			break;
		case 4:
			cout << "Eliminar una cuenta..." << endl;
			break;
		case 5:
			cout << "Saliendo..." << endl;
			break;
		default:
			cout << "Opcion invalida. Intente de nuevo." << endl;
		}
	} while (opcion != 5);
}

void mostrarMenuPrincipal(Bcp* bcp) {
	int opcion;
	Cliente* cliente = new Cliente();
	do {
		cout << "------------------------------------" << endl; // "------------------------------------
		cout << "Bienvenido al BCP" << endl;
		cout << "Menu Principal:" << endl;
		cout << "------------------------------------" << endl; // "------------------------------------
		cout << "1. Iniciar Sesion" << endl;
		cout << "2. Registrarse" << endl;
		cout << "3. Salir" << endl;
		cout << "Seleccione una opcion: ";
		cin >> opcion;

		switch (opcion) {
		case 1:
			cout << "Iniciando sesion..." << endl;
			cliente = bcp->getClientes()->search(12)->data;
			mostrarMenuCliente(bcp, cliente);
			break;
		case 2:
			cliente = new Cliente(bcp->getLastId(bcp->getClientes()), "Juan", "Perez", "Av. Los Alamos", "987654321", "d", "1234");
			bcp->agregar(cliente, bcp->getClientes());
			break;
		case 3:
			cout << "Saliendo..." << endl;
			break;
		default:
			cout << "Opcion invalida. Intente de nuevo." << endl;
		}
	} while (opcion != 3);
}


int main()
{
	Bcp* bcp = new Bcp();


	mostrarMenuPrincipal(bcp);

	/*
	//canales
	Canal* cajero = new Canal(bcp->getLastId(bcp->getCanales()), "Cajero", "", "", "", "", CAJERO);

	bcp->agregar(cajero, bcp->getCanales());
	bcp->agregar(new Canal(bcp->getLastId(bcp->getCanales()), "Web", "", "", "", "", WEB), bcp->getCanales());
	bcp->agregar(new Canal(bcp->getLastId(bcp->getCanales()), "App", "", "", "", "", APP), bcp->getCanales());
	bcp->agregar(new Canal(bcp->getLastId(bcp->getCanales()), "Agente", "", "", "", "", AGENTE), bcp->getCanales());
	bcp->agregar(new Canal(bcp->getLastId(bcp->getCanales()), "Yape", "", "", "", "", YAPE), bcp->getCanales());

	bcp->listar(bcp->getCanales());

	//sedes
	Sede* sede = new Sede(bcp->getLastId(bcp->getSedes()), "Sede 1", "Av. Los Alamos", "987654321", "email@gmail.com");
	bcp->agregar(sede, bcp->getSedes());
	bcp->agregar(new Sede(bcp->getLastId(bcp->getSedes()), "Sede 2", "Av. Los Pinos", "123456789", "correo@gmail.com"), bcp->getSedes());

	bcp->listar(bcp->getSedes());


	//Proceso desde 0

	// agregar clientes
	bcp->agregar(new Cliente(bcp->getLastId(bcp->getClientes()), "Juan", "Perez", "Av. Los Alamos", "987654321", "d", "1234"), bcp->getClientes());
	bcp->agregar(new Cliente(bcp->getLastId(bcp->getClientes()), "Maria", "Gomez", "Av. Los Pinos", "123456789", "d", "1234"), bcp->getClientes());

	Cliente* cliente = new Cliente(bcp->getLastId(bcp->getClientes()), "Juan", "Perez", "Av. Los Alamos", "987654321", "d", "1234");
	bcp->agregar(cliente, bcp->getClientes());
	
	// agregar cuentas
	CuentaBancaria* cuentaBancaria = new CuentaBancaria(bcp->getLastId(bcp->getCuentas()), cliente->getId(), "password", "1000100010001000", 0.01);
	bcp->agregar(cuentaBancaria, bcp->getCuentas());

	// agregar tarjeta a una cuenta
	Tarjeta* tarjeta = new Tarjeta(bcp->getLastId(bcp->getTarjetas()), cliente->getId(), cuentaBancaria->getId());
	bcp->agregar(tarjeta, bcp->getTarjetas());

	cuentaBancaria->setTarjeta(tarjeta);

	// agregar cuenta a cliente
	cliente->agregarCuentaBancaria(cuentaBancaria);

	// operaciones
	Operacion* nuevaOperacion = bcp->cargarOperacion(cuentaBancaria->getNumeroCuenta(), cajero, sede);

	if (nuevaOperacion != nullptr) {
		// agregar operacion a la cola
		bcp->getColaOperaciones()->encolar(nuevaOperacion);

		bcp->listar(bcp->getColaOperaciones());

		// desencolar operacion
		Operacion* opDesencolada = bcp->getColaOperaciones()->desencolar();

		bcp->listar(bcp->getColaOperaciones());

		bcp->ejecutarOperacion(opDesencolada);
		// agregar operacion desencolada a la lista de operaciones
		bcp->agregar(opDesencolada, bcp->getOperaciones());
	}


	bcp->listar(bcp->getOperaciones());


	bcp->listar(bcp->getClientes());

		*/

	system("pause");
    return 0;
}
