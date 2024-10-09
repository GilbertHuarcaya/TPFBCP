#pragma once
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
		ListaDoble<CuentaBancaria*>* cuentas = new ListaDoble<CuentaBancaria*>();
		File<ListaDoble<CuentaBancaria*>*, CuentaBancaria>::leer("Cuentas.csv", cuentas);

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
	}

};