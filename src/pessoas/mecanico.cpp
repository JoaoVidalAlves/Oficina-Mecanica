#include <iostream>
#include "pessoas/mecanico.h"

using std::string;

Mecanico::Mecanico(string nome, string cpf, string telefone, int idmecanico)
	:	Pessoa::Pessoa(nome, cpf, telefone), _idmecanico(idmecanico) { }

void Mecanico::Exibir() const
{
	std::cout << "ID: " << _idmecanico << "\n";
	Pessoa::Exibir();
}

