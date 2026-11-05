#include <iostream>
#include "pessoas/mecanico.h"

using std::string;

Mecanico::Mecanico(string nome, string cpfcnpj, string telefone, int idmecanico)
	:	Pessoa::Pessoa(nome, cpfcnpj, telefone), _idmecanico(idmecanico) { }

void Mecanico::Exibir() const
{
	std::cout << "ID: " << _idmecanico << "\n";
	std::cout << "Nome: " << _nome << "\n";
	std::cout << "CPF: " << _CPF << "\n";
	std::cout << "Telefone: " << _telefone << "\n";
}

