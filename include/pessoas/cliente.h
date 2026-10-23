#ifndef CLIENTE_H
#define CLIENTE_H

#include <iostream>
#include <string>
#include "pessoa.h"

using std::string;

class Cliente : public Pessoa
{
private:
	int _idcliente;

public:
	Cliente(string nome, string cpf, string telefone, int idcliente);

	virtual void Exibir() const override;

	~Cliente()
	{
		std::cout << "Cliente: " << _nome << " removido" << "\n";
	}
};

#endif