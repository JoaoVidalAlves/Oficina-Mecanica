#include <iostream>
#include <cctype> 

#include "pessoas/pessoa.h"
#include "comum/Excecoes.h"   // necessário para usar o throw

namespace
{
	bool soDigitos(const string& texto)
	{
		for (char c : texto)
		{
			if (!std::isdigit(static_cast<unsigned char>(c)))
				return false;
		}
		return true;
	}
}

Pessoa::Pessoa(int id, const string& nome, const string& cpfcnpj, const string& telefone)
	:	_id(id), _nome(nome), _CPFCNPJ(cpfcnpj), _telefone(telefone) 
{
	if (id <= 0)
		throw ErroDeValidacao("O id não pode ser menor ou igual a 0");
	if (nome.empty())
		throw ErroDeValidacao("O nome não pode ser vazio");
	if ((cpfcnpj.size() != 11 && cpfcnpj.size() != 14) || !soDigitos(cpfcnpj))
	{
	    throw ErroDeValidacao("CPF / CNPJ incorreto");
	}
	if (telefone.empty())
		throw ErroDeValidacao("O telefone nao pode ser vazio");
}

