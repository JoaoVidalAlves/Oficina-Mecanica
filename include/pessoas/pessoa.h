#ifndef PESSOA_H
#define PESSOA_H

#include <iostream>
#include <string>

using std::string;	
using std::cout;

class Pessoa
{
protected:
	int _id;
	string _nome;
	string _CPFCNPJ;
	string _telefone;

public:
	Pessoa(int id, const string& nome, const string& cpfcnpj, const string& telefone);
	
	virtual void exibir() const = 0;

	virtual ~Pessoa() = default;
};

#endif