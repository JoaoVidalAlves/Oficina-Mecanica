#ifndef ITEM_PECA_H
#define ITEM_PECA_H

#include "servicos/itemOrcamento.h"

class ItemPeca : public ItemOrcamento
{
private:
	Peca* m_peca;
	int m_quantidade;

public:
	ItemPeca(Peca* peca, int quantidade);
	double calcularSubtotal() const override;
	std::string tipo() const override;
	bool estaDisponivel() const override;
	void darBaixa() override;
	const Peca* getPecaAssociada() const override;
	int getQuantidade() const;
	void exibir() const override;
};

#endif // !ITEM_PECA_H