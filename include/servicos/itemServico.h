#ifndef ITEM_SERVICO_H
#define ITEM_SERVICO_H

#include "servicos/itemOrcamento.h"

class ItemServico : public ItemOrcamento
{
private:
	double m_horas;
	double m_valorHora;
	double m_fator;

public:
	ItemServico(const std::string& descricao, double horas, double valorHora, double fator);
	double calcularSubtotal() const override;
	std::string tipo() const override;
	double getHoras() const;
	double getValorHora() const;
	double getFator() const;
	void exibir() const override;
};

#endif // !ITEM_SERVICO_H