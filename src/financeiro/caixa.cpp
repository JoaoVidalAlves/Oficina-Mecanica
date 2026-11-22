#include <iostream>

#include "financeiro/caixa.h"
#include "comum/excecoes.h"
#include "comum/utils.h"

namespace
{
	// Soma os valores dos lancamentos de um tipo (so receitas ou so despesas).
	double somarPorTipo(const std::vector<Lancamento>& lista, TipoLancamento tipo)
	{
		double soma = 0.0;
		for (const auto& l : lista)
		{
			if (l.tipo == tipo)
				soma += l.valor;
		}
		return soma;
	}
}

void Caixa::registrarReceita(double valor, const std::string& desc, const std::string& data)
{
	if (valor <= 0)
		throw ErroDeValidacao("O valor da receita deve ser maior que zero");

	m_lancamentos.push_back(Lancamento{ data, TipoLancamento::Receita, valor, desc });
	m_saldo += valor;   // receita ENTRA no caixa
}

void Caixa::registrarDespesa(double valor, const std::string& desc, const std::string& data)
{
	if (valor <= 0)
		throw ErroDeValidacao("O valor da despesa deve ser maior que zero");

	m_lancamentos.push_back(Lancamento{ data, TipoLancamento::Despesa, valor, desc });
	m_saldo -= valor;   // despesa SAI do caixa
}

double Caixa::getSaldo() const { return m_saldo; }

double Caixa::totalReceitas() const
{
	return somarPorTipo(m_lancamentos, TipoLancamento::Receita);
}

double Caixa::totalDespesas() const
{
	return somarPorTipo(m_lancamentos, TipoLancamento::Despesa);
}

const std::vector<Lancamento>& Caixa::getLancamentos() const { return m_lancamentos; }

void Caixa::gerarRelatorio() const
{
	std::cout << "\n--- RELATORIO DO CAIXA ---\n";
	if (m_lancamentos.empty())
		std::cout << "(nenhum lancamento)\n";

	for (const auto& l : m_lancamentos)
	{
		std::cout << l.data << " | " << paraTexto(l.tipo) << " | "
			<< l.descricao << " | " << Utils::formatarMoeda(l.valor) << "\n";
	}

	std::cout << "Total de receitas: " << Utils::formatarMoeda(totalReceitas()) << "\n";
	std::cout << "Total de despesas: " << Utils::formatarMoeda(totalDespesas()) << "\n";
	std::cout << "SALDO: " << Utils::formatarMoeda(m_saldo) << "\n";
}