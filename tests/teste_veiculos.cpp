#include "veiculos/carro.h"
#include "veiculos/moto.h"
#include "veiculos/caminhonete.h"
#include "comum/Excecoes.h"

#include <iostream>
#include <string>

int main()
{
	try 
	{
		Carro carro1("AAA1234", "HB20", 2020, 0, 4);
		carro1.exibir();
	}
	catch (const ErroDeValidacao& e)
	{
		std::cout << "ERRO: " << e.what() << std::endl;
	}
	
	try
	{
		Moto moto1("AAA1234", "HB20", 2020, 0, 250);
		moto1.exibir();
	}
	catch (const ErroDeValidacao& e)
	{
		std::cout << "ERRO: " << e.what() << std::endl;
	}

	try
	{
		Caminhonete caminhonete1("AAA1234", "HB20", 2020, 0, 3000);
		caminhonete1.exibir();
	}
	catch (const ErroDeValidacao& e)
	{
		std::cout << "ERRO: " << e.what() << std::endl;
	}

	return 0;
}