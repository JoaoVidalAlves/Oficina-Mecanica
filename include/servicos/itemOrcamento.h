#ifndef ITEM_ORCAMENTO_H
#define ITEM_ORCAMENTO_H

#include <string>
#include "estoque/peca.h"

class ItemOrcamento
{
protected:
	std::string m_descricao;

public:
	ItemOrcamento(const std::string& descricao);
	virtual ~ItemOrcamento() = default;
	std::string getDescricao() const;
	virtual double calcularSubtotal() const = 0;
	virtual std::string tipo() const = 0;
	virtual bool estaDisponivel() const;
	virtual void darBaixa();
	virtual const Peca* getPecaAssociada() const;
	virtual void exibir() const;
};

#endif // !ITEM_ORCAMENTO_H
