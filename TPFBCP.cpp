#include "pch.h"
#include "stdlib.h"
#include "Bcp.h"
#include "ArbolBinario.h"

using namespace System;
using namespace std;

int main()
{
	cout << "Bienvenido" << endl;
	cout << "--------------------------------------" << endl;

	cout << "--------------------------------------" << endl;
	cout << "Cargando datos..." << endl;
	cout << "--------------------------------------" << endl;

	Bcp* bcp = new Bcp();
	bcp->MenuBCP();

	Console::Clear();
	cout << "--------------------------------------" << endl;
	cout << "Gracias por usar el sistema" << endl;
	cout << "--------------------------------------" << endl;
	system("pause");
    return 0;
}