#include <iostream>

#include "estoque/compra.h"
#include "comum/excecoes.h"

Compra::Compra(int id, const std::string& fornecedor, Peca* peca, int quantidade, double custoUnitario, const std::string& data)
	: m_id(id), m_fornecedor(fornecedor), m_peca(peca), m_quantidade(quantidade),
	m_custoUnitario(custoUnitario), m_data(data), m_registrada(false)
{
	if (id <= 0)
		throw ErroDeValidacao("O id da compra nao pode ser menor ou igual a 0");
	if (fornecedor.empty())
		throw ErroDeValidacao("O fornecedor nao pode ser vazio");
	if (peca == nullptr)
		throw ErroDeValidacao("A peca da compra nao pode ser nula");
	if (quantidade <= 0)
		throw ErroDeValidacao("A quantidade nao pode ser menor ou igual a 0");
	if (custoUnitario <= 0)
		throw ErroDeValidacao("O custo unitario nao pode ser menor ou igual a 0");
	if (data.empty())
		throw ErroDeValidacao("A data nao pode ser vazia");
}

void Compra::registrar()
{
	if (m_registrada)
		throw OperacaoInvalida("Esta compra ja foi registrada");
	m_peca->repor(m_quantidade);   // as pecas entram no estoque
	m_registrada = true;
}

double Compra::custoTotal() const { return m_quantidade * m_custoUnitario; }

int Compra::getId() const { return m_id; }

Peca* Compra::getPeca() const { return m_peca; }

int Compra::getQuantidade() const { return m_quantidade; }

double Compra::getCustoUnitario() const { return m_custoUnitario; }

std::string Compra::getFornecedor() const { return m_fornecedor; }

std::string Compra::getData() const { return m_data; }

bool Compra::foiRegistrada() const { return m_registrada; }

void Compra::exibir() const
{
	std::cout << "Compra #" << m_id << " - " << m_data << "\n";
	std::cout << "Fornecedor: " << m_fornecedor << "\n";
	std::cout << "Peca: " << m_peca->getNome() << "\n";
	std::cout << "Quantidade: " << m_quantidade << " x R$ " << m_custoUnitario
		<< " = R$ " << custoTotal() << "\n";
	std::cout << "Situacao: " << (m_registrada ? "registrada" : "pendente") << "\n";
}