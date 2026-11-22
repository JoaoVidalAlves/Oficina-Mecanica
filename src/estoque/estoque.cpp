#include <iostream>
#include <string>

#include "estoque/estoque.h"
#include "comum/excecoes.h"

Peca* Estoque::adicionarPeca(std::unique_ptr<Peca> p)
{
	if (!p)
		throw ErroDeValidacao("A peca nao pode ser nula");
	if (buscarPorCodigo(p->getCodigo()) != nullptr)
		throw ErroDeValidacao("Ja existe uma peca com o codigo " + std::to_string(p->getCodigo()));

	m_pecas.push_back(std::move(p));   // a partir daqui o Estoque e o dono; "p" fica vazio
	return m_pecas.back().get();       // devolve um ponteiro emprestado para a peca guardada
}

int Estoque::proximoCodigo() const
{
	int maior = 0;
	for (const auto& p : m_pecas)
	{
		if (p->getCodigo() > maior)
			maior = p->getCodigo();
	}
	return maior + 1;   // com o estoque vazio, devolve 1
}

Peca* Estoque::buscarPorCodigo(int codigo) const
{
	for (const auto& p : m_pecas)
	{
		if (p->getCodigo() == codigo)
			return p.get();
	}
	return nullptr;   // nao achou: resposta normal, nao e erro
}

Peca& Estoque::obterPeca(int codigo) const
{
	Peca* p = buscarPorCodigo(codigo);
	if (p == nullptr)
		throw NaoEncontrado("Peca nao encontrada: codigo " + std::to_string(codigo));
	return *p;
}

bool Estoque::verificarDisponibilidade(int codigo, int qtd) const
{
	Peca* p = buscarPorCodigo(codigo);
	return p != nullptr && p->getQtdEstoque() >= qtd;
}

void Estoque::darBaixa(int codigo, int qtd)
{
	obterPeca(codigo).darBaixa(qtd);
}

void Estoque::repor(int codigo, int qtd)
{
	obterPeca(codigo).repor(qtd);
}

bool Estoque::removerPeca(int codigo)
{
	for (auto it = m_pecas.begin(); it != m_pecas.end(); ++it)
	{
		if ((*it)->getCodigo() == codigo)
		{
			if ((*it)->getQtdEstoque() > 0)
				throw OperacaoInvalida("Nao e possivel remover uma peca que ainda tem unidades em estoque");
			m_pecas.erase(it);
			return true;
		}
	}
	return false;
}

std::vector<const Peca*> Estoque::listarEstoqueBaixo(int limite) const
{
	std::vector<const Peca*> baixas;
	for (const auto& p : m_pecas)
	{
		if (p->estoqueBaixo(limite))
			baixas.push_back(p.get());
	}
	return baixas;
}

void Estoque::listarPecas() const
{
	if (m_pecas.empty())
	{
		std::cout << "Nenhuma peca cadastrada\n";
		return;
	}
	for (const auto& p : m_pecas)
	{
		p->exibir();
		std::cout << "\n";
	}
}

const std::vector<std::unique_ptr<Peca>>& Estoque::getPecas() const
{
	return m_pecas;
}