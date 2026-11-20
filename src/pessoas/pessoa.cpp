#include <iostream>
#include <cctype> 

#include "pessoas/pessoa.h"
#include "comum/excecoes.h"   // necessário para usar o throw
#include "comum/utils.h"

Pessoa::Pessoa(int id, const string& nome, const string& cpfcnpj, const string& telefone)
	:	_id(id), _nome(nome), _CPFCNPJ(cpfcnpj), _telefone(telefone) 
{
	if (id <= 0)
		throw ErroDeValidacao("O id não pode ser menor ou igual a 0");
	if (nome.empty())
		throw ErroDeValidacao("O nome não pode ser vazio");
	if ((cpfcnpj.size() != 11 && cpfcnpj.size() != 14) || !Utils::soDigitos(cpfcnpj))
	{
	    throw ErroDeValidacao("CPF / CNPJ incorreto");
	}
	if (telefone.empty())
		throw ErroDeValidacao("O telefone nao pode ser vazio");
}

int Pessoa::getId() const 
{ 
	return _id;
}
string Pessoa::getNome() const 
{ 
	return _nome;
}
string Pessoa::getCPFCNPJ() const 
{ 
	return _CPFCNPJ;
}
string Pessoa::getTelefone() const 
{ 
	return _telefone;
}

void Pessoa::exibir() const
{
	std::cout << "ID: " << _id << "\n";
	std::cout << "Nome: " << _nome << "\n";
	std::cout << "CPF: " << _CPFCNPJ << "\n";
	std::cout << "Telefone: " << _telefone << "\n";
}

