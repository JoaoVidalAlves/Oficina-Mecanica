#ifndef MECANICO_H
#define MECANICO_H

#include <iostream>
#include <string>
#include "pessoa.h"

using std::string;
using std::cout;

class Mecanico : public Pessoa
{
private:
	int _idmecanico;

public:
	Mecanico(string nome, string cpfcnpj, string telefone, int idmecanico);

	virtual void Exibir() const override;

	~Mecanico()
	{
		std::cout << "Mecanico(a): " << _nome << " removido(a)" << "\n";
	}
};

#endif