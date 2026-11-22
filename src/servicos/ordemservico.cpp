#include <iostream>

#include "servicos/ordemServico.h"
#include "comum/excecoes.h"
#include "comum/utils.h"

// RN12: so nasce de orcamento aprovado.
OrdemServico::OrdemServico(int id, Orcamento* orcamento, const std::string& dataAbertura)
	: m_id(id), m_orcamento(orcamento), m_status(StatusOS::Aberta), m_dataAbertura(dataAbertura)
{
	if (id <= 0)
		throw ErroDeValidacao("O id da ordem de servico nao pode ser menor ou igual a 0");
	if (orcamento == nullptr)
		throw ErroDeValidacao("O orcamento da ordem de servico nao pode ser nulo");
	if (orcamento->getStatus() != StatusOrcamento::Aprovado)
		throw OperacaoInvalida("So e possivel abrir uma ordem de servico a partir de um orcamento aprovado");
	if (dataAbertura.empty())
		throw ErroDeValidacao("A data de abertura nao pode ser vazia");
}

// RN13: Aberta -> EmAndamento
void OrdemServico::iniciar()
{
	if (m_status != StatusOS::Aberta)
		throw OperacaoInvalida("So e possivel iniciar uma ordem de servico aberta (situacao atual: " + paraTexto(m_status) + ")");
	m_status = StatusOS::EmAndamento;
}

// RN13 e RN14: EmAndamento -> Concluida. Da baixa nas pecas e devolve o total a cobrar.
// Quem lanca a receita no caixa e a Oficina: a OS nao precisa conhecer o Caixa.
double OrdemServico::concluir(const std::string& data)
{
	if (m_status != StatusOS::EmAndamento)
		throw OperacaoInvalida("So e possivel concluir uma ordem de servico em andamento (situacao atual: " + paraTexto(m_status) + ")");
	if (data.empty())
		throw ErroDeValidacao("A data de conclusao nao pode ser vazia");

	// Se faltar peca, esta chamada lanca EstoqueInsuficiente ANTES de mexer em qualquer coisa
	// (darBaixaNosItens e "tudo ou nada"), entao o status so muda se a baixa deu certo.
	m_orcamento->darBaixaNosItens();

	m_status = StatusOS::Concluida;
	m_dataConclusao = data;
	return m_orcamento->calcularTotal();
}

// RN13: Aberta ou EmAndamento -> Cancelada. Nao mexe no estoque.
void OrdemServico::cancelar()
{
	if (m_status == StatusOS::Concluida || m_status == StatusOS::Cancelada)
		throw OperacaoInvalida("Nao e possivel cancelar uma ordem de servico " + paraTexto(m_status));
	m_status = StatusOS::Cancelada;
}

int OrdemServico::getId() const { return m_id; }

Orcamento* OrdemServico::getOrcamento() const { return m_orcamento; }

StatusOS OrdemServico::getStatus() const { return m_status; }

std::string OrdemServico::getDataAbertura() const { return m_dataAbertura; }

std::string OrdemServico::getDataConclusao() const { return m_dataConclusao; }

void OrdemServico::exibir() const
{
	std::cout << "\n--- ORDEM DE SERVICO #" << m_id << " ---\n";
	std::cout << "Orcamento: #" << m_orcamento->getId() << "\n";
	std::cout << "Veiculo: " << m_orcamento->getVeiculo()->tipo() << " "
		<< m_orcamento->getVeiculo()->getModelo()
		<< " (placa " << m_orcamento->getVeiculo()->getPlaca() << ")\n";
	std::cout << "Mecanico: " << m_orcamento->getMecanico()->getNome() << "\n";
	std::cout << "Situacao: " << paraTexto(m_status) << "\n";
	std::cout << "Aberta em: " << m_dataAbertura << "\n";
	if (!m_dataConclusao.empty())
		std::cout << "Concluida em: " << m_dataConclusao << "\n";
	std::cout << "Total: " << Utils::formatarMoeda(m_orcamento->calcularTotal()) << "\n";
}
