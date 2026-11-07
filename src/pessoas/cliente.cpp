#include <iostream>

#include "pessoas/cliente.h"
#include "comum/Excecoes.h"

using std::string;

Cliente::Cliente(int id, const string& nome, const string& cpfcnpj, const string& telefone)
	:	Pessoa(id, nome, cpfcnpj, telefone) { }

void Cliente::AdicionarVeiculo(std::unique_ptr<Veiculo> v)
{
	if (!v)
		throw ErroDeValidacao("O veiculo nao pode ser nulo");
	if (BuscarVeiculo(v->getPlaca()) != nullptr)
		throw ErroDeValidacao("Este cliente ja possui um veiculo com essa placa");
	_veiculos.push_back(std::move(v));
}

Veiculo* Cliente::BuscarVeiculo(const string& placa) const
{
	for (const auto& v : _veiculos)
	{
		if (v->getPlaca() == placa)
			return v.get();
	}
	return nullptr;
}

bool Cliente::RemoverVeiculo(const string& placa)
{
	for (auto it = _veiculos.begin(); it != _veiculos.end(); ++it)
	{
		if ((*it)->getPlaca() == placa)
		{
			_veiculos.erase(it);
			return true;
		}
	}
	return false;
}

const std::vector<std::unique_ptr<Veiculo>>& Cliente::getVeiculos() const
{
	return _veiculos;
}

void Cliente::listarVeiculos() const
{
	if (_veiculos.empty())
	{
		std::cout << "Nenhum veiculo cadastrado\n";
		return;
	}
	for (const auto& v : _veiculos)
		v->Exibir();
}

void Cliente::Exibir() const
{
	std::cout << "ID: " << _id << "\n";
	std::cout << "Nome: " << _nome << "\n";
	std::cout << "CPF: " << _CPFCNPJ << "\n";
	std::cout << "Telefone: " << _telefone << "\n\n";
}
