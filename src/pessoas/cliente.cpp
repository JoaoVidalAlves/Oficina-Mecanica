#include <iostream>
#include "pessoas/cliente.h"

using std::string;

Cliente::Cliente(string nome, string cpf, string telefone, int idcliente)
	:	Pessoa::Pessoa(nome, cpf, telefone), _idcliente(idcliente) { }

void Cliente::Exibir() const
{
	std::cout << "ID: " << _idcliente << "\n";
	Pessoa::Exibir();
}
