#include <iostream>
#include "pessoas/cliente.h"

using std::string;

Cliente::Cliente(string nome, string cpfcnpj, string telefone, int idcliente)
	:	Pessoa::Pessoa(nome, cpfcnpj, telefone), _idcliente(idcliente) { }

void Cliente::Exibir() const
{
	std::cout << "ID: " << _idcliente << "\n";
	std::cout << "Nome: " << _nome << "\n";
	std::cout << "CPF: " << _CPF << "\n";
	std::cout << "Telefone: " << _telefone << "\n";
}
