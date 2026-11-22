#include <iostream>

#include "servicos/itemOrcamento.h"
#include "comum/excecoes.h"
#include "comum/utils.h"

// O construtor guarda a descricao e recusa texto vazio.
ItemOrcamento::ItemOrcamento(const std::string& descricao)
	: m_descricao(descricao)
{
	if (descricao.empty())
		throw ErroDeValidacao("A descricao do item nao pode ser vazia");
}

// calcularSubtotal() e tipo() sao virtuais PUROS (= 0): nao tem corpo aqui,

std::string ItemOrcamento::getDescricao() const { return m_descricao; }

// ---- Comportamentos PADRAO: valem para um servico. ItemPeca sobrescreve. ----

// Um servico nao depende de estoque, entao esta sempre disponivel.
bool ItemOrcamento::estaDisponivel() const { return true; }

// Um servico nao gasta peca, entao dar baixa nao faz nada.
void ItemOrcamento::darBaixa() {}

// Um servico nao tem peca associada.
const Peca* ItemOrcamento::getPecaAssociada() const { return nullptr; }

// Mostra a linha. Usa tipo() e calcularSubtotal(), que sao POLIMORFICOS:
// cada filha responde do seu jeito.
void ItemOrcamento::exibir() const
{
	std::cout << "  [" << tipo() << "] " << m_descricao << " = " << Utils::formatarMoeda(calcularSubtotal()) << "\n";
}