#include <iostream>

#include "servicos/itemPeca.h"
#include "comum/excecoes.h"
#include "comum/utils.h"

namespace
{
	// A descricao do item e o nome da peca. Esta funcao existe para recusar
	// peca nula ANTES de chamar peca->getNome(), que derrubaria o programa.
	std::string descricaoDa(const Peca* peca)
	{
		if (peca == nullptr)
			throw ErroDeValidacao("A peca do item nao pode ser nula");
		return peca->getNome();
	}
}

ItemPeca::ItemPeca(Peca* peca, int quantidade)
	: ItemOrcamento(descricaoDa(peca)), m_peca(peca), m_quantidade(quantidade)
{
	if (quantidade <= 0)
		throw ErroDeValidacao("A quantidade do item nao pode ser menor ou igual a 0");
}

// Subtotal de uma peca: preco de VENDA (o que o cliente paga) x quantidade.
double ItemPeca::calcularSubtotal() const
{
	return m_peca->getPrecoVenda() * m_quantidade;
}

std::string ItemPeca::tipo() const { return "PECA"; }

// Disponivel se o estoque atual da peca cobre a quantidade pedida.
bool ItemPeca::estaDisponivel() const
{
	return m_peca->getQtdEstoque() >= m_quantidade;
}

// Tira do estoque a quantidade usada neste item.
void ItemPeca::darBaixa()
{
	m_peca->darBaixa(m_quantidade);
}

const Peca* ItemPeca::getPecaAssociada() const { return m_peca; }

int ItemPeca::getQuantidade() const { return m_quantidade; }

void ItemPeca::exibir() const
{
	std::cout << "  [PECA] " << m_quantidade << " x " << m_descricao << " (" << Utils::formatarMoeda(m_peca->getPrecoVenda()) << " cada) = " << Utils::formatarMoeda(calcularSubtotal()) << "\n";
}