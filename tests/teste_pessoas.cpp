#include "pessoas/mecanico.h"
#include "pessoas/cliente.h"
#include "comum/Excecoes.h"

#include <iostream>
#include <string>

int main()
{
	try 
	{
		Mecanico mec1(1, "Joao", "12448017638", "923451267", "Motor", 30);
		mec1.Exibir();
	}
	catch (const ErroDeValidacao& e)
	{
		cout << "ERRO: " << e.what() << std::endl;
	}
	
	try
	{
		Cliente cli1(2, "Pedro", "83671084421", "31932451235");
		cli1.Exibir();
	}
	catch (const ErroDeValidacao& e)
	{
		cout << "ERRO: " << e.what() << std::endl;
	}
	return 0;
}