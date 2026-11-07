#ifndef CLIENTE_H
#define CLIENTE_H

#include <memory>
#include <string>
#include <vector>

#include "pessoas/pessoa.h"
#include "veiculos/veiculo.h"

using std::string;
using std::vector;

class Cliente : public Pessoa
{
private:
	vector<std::unique_ptr<Veiculo>> _veiculos;

public:
	Cliente(int id, const string& nome, const string& cpfcnpj, const string& telefone);

	void AdicionarVeiculo(std::unique_ptr<Veiculo> v);

	Veiculo* BuscarVeiculo(const string& placa) const;

	bool RemoverVeiculo(const string& placa);

	const vector<std::unique_ptr<Veiculo>>& getVeiculos() const;

	void listarVeiculos() const;

	void Exibir() const override;

	~Cliente() = default;
};

#endif