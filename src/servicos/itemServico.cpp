#include <iostream>

#include "servicos/itemServico.h"
#include "comum/excecoes.h"
#include "comum/utils.h"

ItemServico::ItemServico(const std::string& descricao, double horas, double valorHora, double fator)
	: ItemOrcamento(descricao), m_horas(horas), m_valorHora(valorHora), m_fator(fator)
{
	if (horas <= 0)
		throw ErroDeValidacao("As horas do servico nao podem ser menores ou iguais a 0");
	if (valorHora <= 0)
		throw ErroDeValidacao("O valor da hora nao pode ser menor ou igual a 0");
	if (fator <= 0)
		throw ErroDeValidacao("O fator de mao de obra nao pode ser menor ou igual a 0");
}

// Mao de obra: horas x valor da hora do mecanico x fator do tipo de veiculo.
// O valor da hora e o fator sao COPIADOS para o item na criacao: se o mecanico
// mudar o valor da hora depois, este orcamento nao muda (como uma nota fiscal).
double ItemServico::calcularSubtotal() const
{
	return m_horas * m_valorHora * m_fator;
}

std::string ItemServico::tipo() const { return "SERVICO"; }

double ItemServico::getHoras() const { return m_horas; }

double ItemServico::getValorHora() const { return m_valorHora; }

double ItemServico::getFator() const { return m_fator; }

void ItemServico::exibir() const
{
	std::cout << "  [SERVICO] " << m_descricao << ": " << m_horas 
	<< " h x " << Utils::formatarMoeda(m_valorHora) << " x fator " 
	<< m_fator << " = " << Utils::formatarMoeda(calcularSubtotal()) << "\n";
}