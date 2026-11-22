#ifndef ORDEM_SERVICO_H
#define ORDEM_SERVICO_H

#include <string>

#include "comum/tipos.h"
#include "servicos/orcamento.h"

// A ficha do servico em execucao. Nasce de um orcamento APROVADO e passa por etapas,
// como um pedido de entrega: Aberta -> EmAndamento -> Concluida (ou Cancelada).
class OrdemServico
{
private:
	int m_id;
	Orcamento* m_orcamento;       // ponteiro emprestado: o dono do orcamento e a Oficina
	StatusOS m_status;
	std::string m_dataAbertura;
	std::string m_dataConclusao;  // vazia ate concluir

public:
	OrdemServico(int id, Orcamento* orcamento, const std::string& dataAbertura);
	void iniciar();
	double concluir(const std::string& data);
	void cancelar();
	int getId() const;
	Orcamento* getOrcamento() const;
	StatusOS getStatus() const;
	std::string getDataAbertura() const;
	std::string getDataConclusao() const;
	void exibir() const;
};

#endif // !ORDEM_SERVICO_H
