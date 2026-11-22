#include <cstddef>
#include <iostream>
#include <string>

#include "servicos/orcamento.h"
#include "servicos/itemPeca.h"
#include "servicos/itemServico.h"
#include "comum/excecoes.h"
#include "comum/utils.h"

namespace
{
	// RN07: um orcamento so pode ser alterado enquanto esta Pendente.
	// Todo metodo que altera o orcamento chama esta funcao primeiro.
	void exigirPendente(StatusOrcamento status)
	{
		if (status != StatusOrcamento::Pendente)
			throw OperacaoInvalida("So e possivel alterar um orcamento pendente (situacao atual: " + paraTexto(status) + ")");
	}
}

Orcamento::Orcamento(int id, Veiculo* v, Mecanico* m, const std::string& data)
	: m_id(id), m_veiculo(v), m_mecanico(m), m_status(StatusOrcamento::Pendente),
	m_descontoPercentual(0.0), m_data(data)
{
	if (id <= 0)
		throw ErroDeValidacao("O id do orcamento nao pode ser menor ou igual a 0");
	if (v == nullptr)
		throw ErroDeValidacao("O veiculo do orcamento nao pode ser nulo");
	if (m == nullptr)
		throw ErroDeValidacao("O mecanico do orcamento nao pode ser nulo");
	if (data.empty())
		throw ErroDeValidacao("A data do orcamento nao pode ser vazia");
}

// Metodo central: todo item (peca ou servico) entra por aqui.
void Orcamento::adicionarItem(std::unique_ptr<ItemOrcamento> item)
{
	exigirPendente(m_status);
	if (!item)
		throw ErroDeValidacao("O item do orcamento nao pode ser nulo");

	// RN08: uma peca aparece no maximo uma vez por orcamento.
	// getPecaAssociada() e polimorfico: servico devolve nullptr, ItemPeca devolve a peca.
	const Peca* peca = item->getPecaAssociada();
	if (peca != nullptr)
	{
		for (const auto& existente : m_itens)
		{
			if (existente->getPecaAssociada() == peca)
				throw ErroDeValidacao("A peca " + peca->getNome() + " ja esta neste orcamento");
		}
	}

	m_itens.push_back(std::move(item));   // o Orcamento passa a ser o dono do item
}

// Atalho: cria o ItemPeca, confere o estoque e entrega para adicionarItem.
void Orcamento::adicionarPeca(Peca* peca, int qtd)
{
	exigirPendente(m_status);

	// O construtor do ItemPeca ja recusa peca nula e quantidade invalida.
	auto item = std::make_unique<ItemPeca>(peca, qtd);

	if (!item->estaDisponivel())
		throw EstoqueInsuficiente("Estoque insuficiente da peca " + peca->getNome() +
			": pedido " + std::to_string(qtd) + ", disponivel " + std::to_string(peca->getQtdEstoque()));

	adicionarItem(std::move(item));
}

// Atalho: o valor da hora vem do mecanico e o fator vem do veiculo.
// m_veiculo->fatorMaoDeObra() e POLIMORFICO: Carro 1.0, Moto 0.8, Caminhonete 1.5.
void Orcamento::adicionarServico(const std::string& desc, double horas)
{
	exigirPendente(m_status);
	adicionarItem(std::make_unique<ItemServico>(desc, horas,
		m_mecanico->getValorHora(), m_veiculo->fatorMaoDeObra()));
}

void Orcamento::removerItem(size_t indice)
{
	exigirPendente(m_status);
	if (indice >= m_itens.size())
		throw NaoEncontrado("Item inexistente no orcamento: posicao " + std::to_string(indice));
	m_itens.erase(m_itens.begin() + static_cast<std::ptrdiff_t>(indice));
}

// POLIMORFISMO: percorre itens de tipos diferentes e chama o MESMO metodo.
// Cada item calcula o proprio valor, e o orcamento nao precisa saber qual e qual.
double Orcamento::calcularSubtotal() const
{
	double soma = 0.0;
	for (const auto& item : m_itens)
		soma += item->calcularSubtotal();
	return soma;
}

void Orcamento::aplicarDesconto(double percentual)
{
	exigirPendente(m_status);
	if (percentual < 0 || percentual > 20)
		throw ErroDeValidacao("O desconto deve ficar entre 0% e 20%");
	m_descontoPercentual = percentual;
}

// Total = subtotal menos o desconto. Ex.: 225 com 10% = 225 x 0.90 = 202.50
double Orcamento::calcularTotal() const
{
	return calcularSubtotal() * (1.0 - m_descontoPercentual / 100.0);
}

// True so se TODOS os itens estao disponiveis (servico sempre esta).
bool Orcamento::pecasDisponiveis() const
{
	for (const auto& item : m_itens)
	{
		if (!item->estaDisponivel())
			return false;
	}
	return true;
}

void Orcamento::aprovar()
{
	if (m_status != StatusOrcamento::Pendente)
		throw OperacaoInvalida("Apenas orcamentos pendentes podem ser aprovados");
	if (m_itens.empty())
		throw OperacaoInvalida("Nao e possivel aprovar um orcamento sem itens");

	// Procura o primeiro item sem estoque para dizer QUAL esta faltando.
	for (const auto& item : m_itens)
	{
		if (!item->estaDisponivel())
			throw EstoqueInsuficiente("Estoque insuficiente para aprovar: " + item->getDescricao());
	}

	m_status = StatusOrcamento::Aprovado;
}

void Orcamento::recusar()
{
	if (m_status != StatusOrcamento::Pendente)
		throw OperacaoInvalida("Apenas orcamentos pendentes podem ser recusados");
	m_status = StatusOrcamento::Recusado;
}

// Usado pela OrdemServico ao concluir. Ou da baixa em TODOS os itens, ou em nenhum:
// confere tudo antes de mexer em qualquer peca, para nao ficar pela metade.
void Orcamento::darBaixaNosItens()
{
	if (m_status != StatusOrcamento::Aprovado)
		throw OperacaoInvalida("Apenas orcamentos aprovados dao baixa no estoque");
	if (!pecasDisponiveis())
		throw EstoqueInsuficiente("Estoque insuficiente para concluir o orcamento " + std::to_string(m_id));

	for (const auto& item : m_itens)
		item->darBaixa();   // servico: nao faz nada; peca: tira do estoque
}

int Orcamento::getId() const { return m_id; }

Veiculo* Orcamento::getVeiculo() const { return m_veiculo; }

Mecanico* Orcamento::getMecanico() const { return m_mecanico; }

StatusOrcamento Orcamento::getStatus() const { return m_status; }

const std::vector<std::unique_ptr<ItemOrcamento>>& Orcamento::getItens() const { return m_itens; }

void Orcamento::exibir() const
{
	std::cout << "\n--- ORCAMENTO #" << m_id << " (" << m_data << ") ---\n";
	std::cout << "Veiculo: " << m_veiculo->tipo() << " " << m_veiculo->getModelo()
		<< " (placa " << m_veiculo->getPlaca() << ")\n";
	std::cout << "Mecanico: " << m_mecanico->getNome() << "\n";
	std::cout << "Situacao: " << paraTexto(m_status) << "\n";
	std::cout << "Itens:\n";
	if (m_itens.empty())
		std::cout << "  (nenhum item)\n";
	for (const auto& item : m_itens)
		item->exibir();   // polimorfismo: cada item se mostra do seu jeito
	std::cout << "Subtotal: " << Utils::formatarMoeda(calcularSubtotal()) << "\n";
	std::cout << "Desconto: " << m_descontoPercentual << "%\n";
	std::cout << "TOTAL: " << Utils::formatarMoeda(calcularTotal()) << "\n";
}