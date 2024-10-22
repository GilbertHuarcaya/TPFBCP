#include "pch.h"
#include <stdlib.h>
#include "Bcp.h"
#include "ArbolBinario.h"

using namespace System;
using namespace std;

int main()
{
	//Verificacion del arbol aplicado a cuentas bancarias
	/*CuentaBancaria* aux = new CuentaBancaria(1, 1, "1", "1", 1234);
	ArbolBinario<CuentaBancaria*>* AB = new ArbolBinario<CuentaBancaria*>(aux, [](CuentaBancaria* a, CuentaBancaria* b)->bool{
		if (a != nullptr && b != nullptr)
			return a->getSaldo() < b->getSaldo();
		else return false;
	});
	AB->insertar(new CuentaBancaria(2, 2, "2", "2", 1111));
	AB->insertar(new CuentaBancaria(3, 3, "3", "3", 2000));
	AB->insertar(new CuentaBancaria(4, 4, "4", "4", 1222));
	AB->PreOrden([](CuentaBancaria* a)
	{
		a->print();
	});
	system("pause");
	Console::Clear();
	AB->EnOrden([](CuentaBancaria* a)
	{
		a->print();
	});
	system("pause");
	Console::Clear();
	AB->PostOrden([](CuentaBancaria* a)
	{
		a->print();
	});
	system("pause");
	Console::Clear();
	cout << AB->altura();
	AB->buscarNodoPorId(4, AB->getRaiz())->dato->print();
	system("pause");*/
	Bcp* bcp = new Bcp();
	bcp->MenuBCP();
    return 0;
}