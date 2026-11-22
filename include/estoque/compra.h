#ifndef COMPRA_H
#define COMPRA_H

#include <string>

#include "estoque/peca.h"

// A nota de uma compra feita a um fornecedor. Quando "registrada",
// as pecas entram no estoque.
class Compra
{
private:

	int m_id;
	std::string m_fornecedor;
	Peca* m_peca;              // ponteiro emprestado: o dono da peca e o Estoque
	int m_quantidade;
	double m_custoUnitario;
	std::string m_data;
	bool m_registrada;

public:

	Compra(int id, const std::string& fornecedor, Peca* peca, int quantidade, double custoUnitario, const std::string& data);
	void registrar();
	double custoTotal() const;
	int getId() const;
	Peca* getPeca() const;
	int getQuantidade() const;
	double getCustoUnitario() const;
	std::string getFornecedor() const;
	std::string getData() const;
	bool foiRegistrada() const;
	void exibir() const;
};

#endif // !COMPRA_H