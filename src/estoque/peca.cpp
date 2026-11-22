#include <iostream>

#include "estoque/peca.h"	
#include "comum/excecoes.h"   // necessário para usar o throw

Peca::Peca(int codigo, const std::string& nome, double precoCusto, double precoVenda, int qtdEstoque)
	: m_codigo(codigo), m_nome(nome), m_precoCusto(precoCusto), m_precoVenda(precoVenda), m_qtdEstoque(qtdEstoque)
{
	if (codigo <= 0)
		throw ErroDeValidacao("O codigo não pode ser menor ou igual a 0");
	if (nome.empty())
		throw ErroDeValidacao("O nome não pode ser vazio");
	if (precoCusto <= 0)
		throw ErroDeValidacao("O preco de custo não pode ser menor ou igual a 0");
	if (precoVenda < precoCusto)
		throw ErroDeValidacao("O preco de venda precisa ser superior ao preco de custo");
	if (qtdEstoque < 0)
		throw ErroDeValidacao("O estoque não pode ser negativo");
}

int Peca::getCodigo() const { return m_codigo; }

std::string Peca::getNome() const { return m_nome; }

double Peca::getPrecoCusto() const { return m_precoCusto; }

double Peca::getPrecoVenda() const { return m_precoVenda; }

int Peca::getQtdEstoque() const { return m_qtdEstoque; }

void Peca::setPrecoVenda(double v)
{
	if (v < m_precoCusto)
		throw ErroDeValidacao("O preco de venda precisa ser superior ao preco de custo");
	m_precoVenda = v;
}

double Peca::margemUnitaria() const { return m_precoVenda - m_precoCusto; }

void Peca::darBaixa(int qtd)
{
	if (qtd <= 0)
		throw ErroDeValidacao("Quantidade não pode ser menor ou igual a zero");
	if (qtd > m_qtdEstoque)
		throw EstoqueInsuficiente("Estoque insuficiente da peca " + m_nome + ": pedido " + std::to_string(qtd) + ", disponivel " + std::to_string(m_qtdEstoque));

	m_qtdEstoque -= qtd;
}

void Peca::repor(int qtd)
{
	if (qtd <= 0)
		throw ErroDeValidacao("Quantidade não pode ser menor ou igual a zero");
	m_qtdEstoque += qtd;
}

bool Peca::estoqueBaixo(int limite) const
{
	return m_qtdEstoque <= limite;
}

void Peca::exibir() const
{
	std::cout << "Codigo: " << m_codigo << "\n";
	std::cout << "Nome:  " << m_nome << "\n";
	std::cout << "Preco de custo: R$ " << m_precoCusto << "\n";
	std::cout << "Preco de Venda: R$ " << m_precoVenda << "\n";
	std::cout << "Quantidade em estoque: " << m_qtdEstoque << "\n";
}