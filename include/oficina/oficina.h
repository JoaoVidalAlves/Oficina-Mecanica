#ifndef OFICINA_H
#define OFICINA_H

#include <memory>
#include <string>
#include <vector>

#include "pessoas/cliente.h"
#include "pessoas/mecanico.h"
#include "veiculos/veiculo.h"
#include "estoque/estoque.h"
#include "estoque/compra.h"
#include "servicos/orcamento.h"
#include "servicos/ordemservico.h"
#include "financeiro/caixa.h"

// O balcao da oficina. O Menu nunca mexe direto em pecas, orcamentos ou caixa:
// ele pede para a Oficina, que coordena tudo e garante as regras de negocio.
//
// A Oficina e a DONA de clientes, mecanicos, orcamentos, ordens e compras
// (todos em unique_ptr, para o endereco de cada objeto nunca mudar).
class Oficina
{
private:
	std::vector<std::unique_ptr<Cliente>> m_clientes;
	std::vector<std::unique_ptr<Mecanico>> m_mecanicos;
	Estoque m_estoque;
	std::vector<std::unique_ptr<Orcamento>> m_orcamentos;
	std::vector<std::unique_ptr<OrdemServico>> m_ordens;
	std::vector<std::unique_ptr<Compra>> m_compras;
	Caixa m_caixa;

	// Contadores de id: cada novo registro recebe o proximo numero.
	int m_proximoIdPessoa = 1;       // clientes e mecanicos dividem o mesmo contador
	int m_proximoIdOrcamento = 1;
	int m_proximoIdOS = 1;
	int m_proximoIdCompra = 1;

	bool clienteTemOrcamentos(const Cliente& cliente) const;

public:
	// ---- Clientes ----
	Cliente& cadastrarCliente(const std::string& nome, const std::string& cpf, const std::string& tel);
	Cliente* buscarClientePorCpf(const std::string& cpf) const;
	Cliente& obterCliente(int id) const;
	bool removerCliente(int id);
	void listarClientes() const;

	// ---- Veiculos ----
	void cadastrarVeiculo(int idCliente, std::unique_ptr<Veiculo> v);
	Veiculo* buscarVeiculoPorPlaca(const std::string& placa) const;

	// ---- Mecanicos ----
	Mecanico& cadastrarMecanico(const std::string& nome, const std::string& cpf, const std::string& tel,
		const std::string& especialidade, double valorHora);
	Mecanico& obterMecanico(int id) const;
	void listarMecanicos() const;

	// ---- Estoque e compras ----
	Peca& cadastrarPeca(const std::string& nome, double custo, double venda);
	Peca& obterPeca(int codigo) const;
	Compra& comprarPecas(int codigoPeca, int qtd, const std::string& fornecedor, double custoUnitario);
	void listarEstoque() const;
	void listarCompras() const;
	void alertaEstoqueBaixo(int limite = 3) const;

	// ---- Orcamentos ----
	Orcamento& criarOrcamento(const std::string& placa, int idMecanico);
	Orcamento& obterOrcamento(int id) const;
	void aprovarOrcamento(int id);
	void recusarOrcamento(int id);
	void listarOrcamentos() const;

	// ---- Ordens de servico ----
	OrdemServico& abrirOS(int idOrcamento);
	OrdemServico& obterOS(int id) const;
	void iniciarOS(int id);
	void concluirOS(int id);
	void cancelarOS(int id);
	void listarOS() const;

	// ---- Caixa ----
	const Caixa& getCaixa() const;
	void relatorioCaixa() const;
};

#endif // !OFICINA_H
