#include <iostream>

#include "pessoas/cliente.h"
#include "comum/excecoes.h"

using std::string;

Cliente::Cliente(int id, const string& nome, const string& cpfcnpj, const string& telefone)
	:	Pessoa(id, nome, cpfcnpj, telefone) { }

void Cliente::adicionarVeiculo(std::unique_ptr<Veiculo> v)
{
	if (!v)
		throw ErroDeValidacao("O veiculo nao pode ser nulo");
	if (buscarVeiculo(v->getPlaca()) != nullptr)
		throw ErroDeValidacao("Este cliente ja possui um veiculo com essa placa");
	_veiculos.push_back(std::move(v));
}

Veiculo* Cliente::buscarVeiculo(const string& placa) const
{
	string placaBusca = paraMaiusculas(placa);
	for (const auto& v : _veiculos)
	{
		if (v->getPlaca() == placaBusca)
			return v.get();
	}
	return nullptr;
}

bool Cliente::removerVeiculo(const string& placa)
{
	string placaBusca = paraMaiusculas(placa);
	for (auto it = _veiculos.begin(); it != _veiculos.end(); ++it)
	{
		if ((*it)->getPlaca() == placaBusca)
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
		v->exibir();
}

void Cliente::exibir() const
{
	std::cout << "\n--- CLIENTE --- \n";
	Pessoa::exibir();
}
