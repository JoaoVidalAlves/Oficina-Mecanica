#include "pessoas/mecanico.h"
#include "pessoas/cliente.h"

#include <iostream>
#include <string>

int main()
{
	Mecanico mec1("Joao", "124.480.176-38", "(31) 98622-0355", 1);
	Cliente cli1("Pedro", "836.710.844-21", "(31) 93137-8374", 1);

	mec1.Exibir();
	cli1.Exibir();

	return 0;
}