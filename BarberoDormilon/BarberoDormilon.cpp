#include "stdafx.h"
#include <iostream>
#include <time.h>
#include <string.h>
#include <string>
#include <limits.h>
#include <cstdlib>
#include <thread>
#include <windows.h>
#include <stdio.h>

using namespace std;

typedef struct Barbero Barbero;
typedef struct Cliente Cliente;

struct Barbero {
	int nBarbero;
	string nombre;
	Cliente * clienteAtendiendo = NULL;
	Barbero * sigBarbero = NULL;
};

struct Cliente {
	string nombre;
	time_t fecha_llegada = NULL;
	time_t fecha_inicio_atencion = NULL;
	int tiempoProceso;
	bool atendido = false;
	Cliente * sigCliente;
};

const int CantidadBarbero = 3;
const int CantidadSillas = 5;
int sillasUsada = 0;
int barberosDurmiendo = 0;

void menu();

void Validar(string, string, string &);
void Validar(string, string, int &);

void adicionarBarberos(int);
void adicionarCliente();
void cargarBarberos();
void HilosBarberos();
void validarBarbero(Barbero * );
string ConvertirFecha(time_t );
int cantidadClientesActivos();
void mostarClientesBarberos();

void limpiarConsola();

Barbero * Barberos = NULL , * Barbero1 = NULL, * Barbero2 = NULL, * Barbero3 = NULL;
Cliente * Clientes = NULL;

int main() {

	cout << " \n\t\tBarbero Dormilon ";
	cargarBarberos();
	// HilosBarberos();

	int cantidad = 0;
	Barbero * aux = Barberos;
	while (aux != NULL) {
		cantidad++;
		if (cantidad == 1)
			Barbero1 = aux;
		if (cantidad == 2)
			Barbero2 = aux;
		if (cantidad == 3)
			Barbero3 = aux;
		aux = aux->sigBarbero;
	}
	thread hilo1(validarBarbero, Barbero1);
	thread hilo2(validarBarbero, Barbero2);
	thread hilo3(validarBarbero, Barbero3);
	thread hilomenu(menu);
	hilomenu.join();
	hilo1.join();
	hilo2.join();
	hilo3.join();
	// menu();
	return 0;
}

void menu() {
	int valMenu = 0;
	bool esNumeroPositivo = 0;
	limpiarConsola();
	do {
		limpiarConsola();
		cout << " \n\t\tBarbero Dormilon ";
		do {
			esNumeroPositivo = true;
			Validar("\n\t 1) Registrar llegada Cliente sillas disponibles ( " + to_string( 5 -cantidadClientesActivos()) +  ") \n\t 2) Mostar informacion barberos y clientes \n\t 3) Salir del Programa\n\t", "¡Solo se resiben numeros!", valMenu);
			if (valMenu <= 0)
				esNumeroPositivo = false;
		} while (!esNumeroPositivo);
		switch (valMenu) {
		case 1:
			adicionarCliente();
			break;
		case 2:
			mostarClientesBarberos();
			break;
		}
	} while (valMenu < 3);

}

void HilosBarberos() {
	int cantidad = 0;
	Barbero * aux = Barberos;
	while (aux != NULL) {
		cantidad++;
		if (cantidad == 1)
			Barbero1 = aux;
		if (cantidad == 2)
			Barbero2 = aux;
		if (cantidad == 3)
			Barbero3 = aux;
		aux = aux->sigBarbero;
	}
	thread hilo1(validarBarbero, Barbero1);
	thread hilo2(validarBarbero, Barbero2);
	thread hilo3(validarBarbero, Barbero3);
	hilo1.join();
	hilo2.join();
	hilo3.join();
	system("pause");
}

void validarBarbero(Barbero * hiloBarbero) {
	bool atendido = 0;
	for (;;) { // ciclo infinito de validaciones de nuevos usuarios
		Sleep(2500);
		if(atendido)
			Sleep(1100);
		if (hiloBarbero->clienteAtendiendo == NULL) {
			for (;;) { // ciclo infinito de validaciones de nuevos usuarios
				Sleep(500);
				Cliente * aux = Clientes;
				while (aux != NULL)
				{
					if (aux->fecha_inicio_atencion == NULL && aux->atendido == false) {
						hiloBarbero->clienteAtendiendo = aux;
						aux->fecha_inicio_atencion = time(NULL);
						cout << "\n El cliente " << aux->nombre << " esta empezando a ser atendido  !  " <<   endl;
						atendido = 1;
						break;
					}
					aux = aux->sigCliente;
				}
				if (atendido)
					break;
			}
		}
		else {
			if (time(NULL) - hiloBarbero->clienteAtendiendo->fecha_inicio_atencion >= hiloBarbero->clienteAtendiendo->tiempoProceso) {
				cout << "\n El barbero " << hiloBarbero->nombre << " termino de atender al cliente " << hiloBarbero->clienteAtendiendo->nombre << endl;
				hiloBarbero->clienteAtendiendo->atendido = true;
				hiloBarbero->clienteAtendiendo = NULL;
				atendido = false;
				mostarClientesBarberos();
			}
			else 
				cout << "\n El barbero " << hiloBarbero->nombre << " Esta atendiendo a cliente " << hiloBarbero->clienteAtendiendo->nombre << " Que llego " << ConvertirFecha(hiloBarbero->clienteAtendiendo->fecha_inicio_atencion) << " faltan " << ConvertirFecha( ( hiloBarbero->clienteAtendiendo->fecha_inicio_atencion + hiloBarbero->clienteAtendiendo->tiempoProceso ) - time(NULL)) ;
		}
	}
}

void Validar(string mensaje, string mensajeError, int &valor) {
	bool validar = false;
	do {
		validar = true;
		cout << "\n\t\t" << mensaje.c_str() << " : ";
		cin >> valor;
		if (cin.fail())
		{
			limpiarConsola();
			cout << "\n" << mensajeError.c_str() << "\n";
			cin.ignore();
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			validar = false;
		}
	} while (!validar);
}

void Validar(string mensaje, string mensajeError, string &valor) {
	bool validar = false;
	do {
		validar = true;
		cout << "\n\t\t" << mensaje.c_str() << " : ";
		cin >> valor;
		if (cin.fail())
		{
			limpiarConsola();
			cout << "\n" << mensajeError.c_str() << "\n";
			cin.ignore();
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			validar = false;
		}
	} while (!validar);
}

void cargarBarberos() {
	for (int i = 0; i < CantidadBarbero; i++)
		adicionarBarberos((i + 1));
}

void adicionarBarberos(int numeroBarbero) {
	Barbero * nuevobarbero = new Barbero();
	Validar("Digite el nombre del barbero " + to_string(numeroBarbero),
		"\n¡Hay un error en lo digitado por favor vuelva a intentar! ", nuevobarbero->nombre);
	if (Barberos == NULL)
		Barberos = nuevobarbero;
	else {
		Barbero * auxbarbero = Barberos;
		while (auxbarbero != NULL) {
			if (auxbarbero->sigBarbero == NULL) {
				auxbarbero->sigBarbero = nuevobarbero;
				break;
			}
			auxbarbero = auxbarbero->sigBarbero;
		}
	}
}

void adicionarCliente() {
	Cliente * nuevocliente = new Cliente();
	if (cantidadClientesActivos() == CantidadSillas) {
		cout << "\n No hay sillas disponibles ";
		return;
	}

	Validar("Digite el nombre de cliente ",
		"\n¡Hay un error en lo digitado por favor vuelva a intentar! ", nuevocliente->nombre);
	int operacion = NULL;
	do {
		if (operacion != NULL && (operacion < 1 || operacion > 4))
			cout << "\n No es una opcion valida intente otra vez \n";
		Validar("\nDigite lo que desea que se le realice : \n1) Corte masculino (20 MIN) \n2) Corte fenemino (30 MIN)\n3) Sepillado(60 MIN) \n4) Barba (15 MIN) ",
			"\n¡Hay un error en lo digitado por favor vuelva a intentar! ", operacion);
	} while (operacion < 1 || operacion > 4);
	int min = 60;
	switch (operacion)
	{
		case 1:
			nuevocliente->tiempoProceso = min * 2;
			break;
		case 2:
			nuevocliente->tiempoProceso = min * 3;
			break;
		case 3:
			nuevocliente->tiempoProceso = min * 6;
			break;
		case 4:
			nuevocliente->tiempoProceso = min * 1;
			break;
	}
	if (Clientes == NULL)
		Clientes = nuevocliente;
	else {
		Cliente * auxclientes = Clientes;
		while (auxclientes != NULL) {
			if (auxclientes->sigCliente == NULL) {
				auxclientes->sigCliente = nuevocliente;
				break;
			}
			auxclientes = auxclientes->sigCliente;
		}
	}
}

void limpiarConsola() {
	system("cls");
	system("clear");
}

int cantidadClientesActivos() {
	int conteoPorAtender = 0;
	Cliente * auxclientes = Clientes;
	while (auxclientes != NULL) {
		if (auxclientes->fecha_inicio_atencion == NULL)
			conteoPorAtender++;
		auxclientes = auxclientes->sigCliente;
	}
	return conteoPorAtender;
}

string ConvertirFecha(time_t fecha) {
	if (fecha != 60 && fecha != (60 * 60) && fecha != (60 * 60 * 24)) {
		struct tm timeinfo;
		localtime_s(&timeinfo, &fecha);
		if (fecha < 60)
			return  to_string(timeinfo.tm_sec);
		else if (fecha > 60 && fecha < 60 * 60 )
			return  to_string(timeinfo.tm_min) + " : " + to_string(timeinfo.tm_sec);
		else if (fecha > 60 * 60 && fecha < 60 * 60 * 24 )
			return  to_string(timeinfo.tm_mday) + " " + to_string(timeinfo.tm_hour) + " : " + to_string(timeinfo.tm_min) + " : " + to_string(timeinfo.tm_sec);
		else if (fecha > 60 * 60 * 24)
			return  to_string((1900 + timeinfo.tm_year)) + "-" +to_string(timeinfo.tm_mon) + "-" + 
			to_string(timeinfo.tm_mday) + " " + to_string(timeinfo.tm_hour) + ":" + to_string(timeinfo.tm_min) + ":" + to_string(timeinfo.tm_sec);
	}
	else
		return "";
}

void mostarClientesBarberos() {
	Barbero * aux = Barberos;
	while (aux != NULL) {
		if (aux->clienteAtendiendo == NULL)
			cout << "\n El barbero " << aux->nombre << " esta durmiendo.";
		else 
			cout << "\n El barbero " << aux->nombre << " esta atendiendo al cliente " << aux->clienteAtendiendo->nombre << endl;
		aux = aux->sigBarbero;
	}
	Cliente * auxclientes = Clientes;
	while (auxclientes != NULL) {
		if (auxclientes->fecha_inicio_atencion == NULL)
			cout << " El cliente esta " << auxclientes->nombre << " esta esperando ha ser atendido";
		auxclientes = auxclientes->sigCliente;
	}
}