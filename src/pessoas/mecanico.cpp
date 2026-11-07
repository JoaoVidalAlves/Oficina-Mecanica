#include <iostream>

#include "pessoas/mecanico.h"
#include "comum/Excecoes.h"

using std::string;

Mecanico::Mecanico(int id, const string& nome, const string& cpfcnpj, const string& telefone, const string& especialidade, double valorHora)
	:	Pessoa(id, nome, cpfcnpj, telefone), _especialidade(especialidade), _valorHora(valorHora)
{
	if (especialidade.empty())
		throw ErroDeValidacao("A especialidade nao pode ser vazia");
	if (valorHora <= 0)
		throw ErroDeValidacao("O valor da hora deve ser maior que zero");
}

string Mecanico::getEspecialidade() const 
{ 
	return _especialidade;
}

double Mecanico::getValorHora() const 
{ 
	return _valorHora;
}

void Mecanico::setValorHora(double v)
{
	if (v <= 0)
		throw ErroDeValidacao("O valor da hora deve ser maior que zero");
	_valorHora = v;
}

void Mecanico::Exibir() const
{
	std::cout << "ID: " << _id << "\n";
	std::cout << "Nome: " << _nome << "\n";
	std::cout << "CPF: " << _CPFCNPJ << "\n";
	std::cout << "Telefone: " << _telefone << "\n";
	std::cout << "Especialidade: " << _especialidade << "\n";
	std::cout << "Valor da hora: " << _valorHora << "\n\n";
}

