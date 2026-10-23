#ifndef PESSOA_H
#define PESSOA_H

#include <iostream>
#include <string>

using std::string;	
using std::cout;

class Pessoa
{
protected:
	string _nome;
	string _CPF;
	string _telefone;

public:
	Pessoa(string nome, string cpf, string telefone);
	
	virtual void Exibir() const;

	~Pessoa()
	{
		std::cout << "Pessoa: " << _nome << " removida" << "\n";
	}
};

#endif