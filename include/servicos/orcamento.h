#ifndef ORCAMENTO_H
#define ORCAMENTO_H

#include <memory>
#include <vector>
#include "servicos/itemOrcamento.h"
#include "veiculos/veiculo.h"
#include "pessoas/mecanico.h"
#include "comum/tipos.h"

class Orcamento
{
private:
	int m_id;
	Veiculo* m_veiculo;
	Mecanico* m_mecanico;
	std::vector<std::unique_ptr<ItemOrcamento>> m_itens;
	StatusOrcamento m_status;
	double m_descontoPercentual;
	std::string m_data;

public:
	Orcamento(int id, Veiculo* v, Mecanico* m, const std::string& data);
	void adicionarItem(std::unique_ptr<ItemOrcamento> item);
	void adicionarPeca(Peca* peca, int qtd);
	void adicionarServico(const std::string& desc, double horas);
	void removerItem(size_t indice);
	double calcularSubtotal() const;
	void aplicarDesconto(double percentual);
	double calcularTotal() const;
	bool pecasDisponiveis() const;
	void aprovar();
	void recusar();
	void darBaixaNosItens();
	int getId() const;
	Veiculo* getVeiculo() const;
	Mecanico* getMecanico() const;
	StatusOrcamento getStatus() const;
	const std::vector<std::unique_ptr<ItemOrcamento>>& getItens() const;
	void exibir() const;
};

#endif // !ORCAMENTO_H