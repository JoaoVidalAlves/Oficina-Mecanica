#include <iostream>
#include "pessoas/pessoa.h"

Pessoa::Pessoa(string nome, string cpfcnpj, string telefone)
	:	_nome(nome), _CPFCNPJ(cpfcnpj), _telefone(telefone) {}

/*
void Pessoa::Exibir() const
{
	std::cout << "Nome: " << _nome << "\n";
	std::cout << "CPF: " << _CPF << "\n";
	std::cout << "Telefone: " << _telefone << "\n";
}
*/

