// Teste da OrdemServico (Fase 4).
// O CMake cria este programa sozinho (arquivos tests/teste_*.cpp).

#include <cmath>
#include <iostream>
#include <string>

#include "comum/excecoes.h"
#include "comum/tipos.h"
#include "estoque/peca.h"
#include "pessoas/mecanico.h"
#include "servicos/orcamento.h"
#include "servicos/ordemservico.h"
#include "veiculos/carro.h"

int falhas = 0;

void conferir(const std::string& nome, bool ok)
{
	std::cout << (ok ? "[OK]     " : "[FALHOU] ") << nome << "\n";
	if (!ok) falhas++;
}

bool quase(double a, double b) { return std::fabs(a - b) < 0.001; }

template <typename Excecao, typename Acao>
void esperaErro(const std::string& nome, Acao acao)
{
	try
	{
		acao();
		conferir(nome + " (deveria lancar erro)", false);
	}
	catch (const Excecao&)
	{
		conferir(nome, true);
	}
	catch (...)
	{
		conferir(nome + " (lancou o tipo errado de erro)", false);
	}
}

int main()
{
	Mecanico joao(1, "Joao Silva", "12448017638", "31999990001", "Motor", 60.0);
	Carro carro("ABC1234", "Gol", 2018, 10000, 4);
	Peca filtro(1, "Filtro de oleo", 20.0, 40.0, 10);

	// Orcamento de teste: 2 filtros (80) + 2 h de servico (120) = 200
	auto novoOrcamentoAprovado = [&](int id)
	{
		auto o = std::make_unique<Orcamento>(id, &carro, &joao, "2026-11-23");
		o->adicionarPeca(&filtro, 2);
		o->adicionarServico("Troca de oleo", 2.0);
		o->aprovar();
		return o;
	};

	std::cout << "--- Criacao (RN12) ---\n";
	Orcamento pendente(1, &carro, &joao, "2026-11-23");
	pendente.adicionarServico("Revisao", 1.0);
	esperaErro<OperacaoInvalida>("OS de orcamento Pendente", [&] { OrdemServico os(1, &pendente, "2026-11-23"); });
	esperaErro<ErroDeValidacao>("OS sem orcamento", [] { OrdemServico os(1, nullptr, "2026-11-23"); });
	auto orc = novoOrcamentoAprovado(2);
	esperaErro<ErroDeValidacao>("OS com id 0", [&] { OrdemServico os(0, orc.get(), "2026-11-23"); });
	esperaErro<ErroDeValidacao>("OS com data vazia", [&] { OrdemServico os(1, orc.get(), ""); });
	OrdemServico os(1, orc.get(), "2026-11-23");
	conferir("OS nasce Aberta, sem data de conclusao", os.getStatus() == StatusOS::Aberta && os.getDataConclusao().empty() && os.getDataAbertura() == "2026-11-23");

	std::cout << "\n--- Transicoes (RN13) ---\n";
	esperaErro<OperacaoInvalida>("Concluir sem iniciar", [&] { os.concluir("2026-11-24"); });
	conferir("Estoque intacto depois do erro", filtro.getQtdEstoque() == 10);
	os.iniciar();
	conferir("iniciar(): Aberta -> EmAndamento", os.getStatus() == StatusOS::EmAndamento);
	esperaErro<OperacaoInvalida>("Iniciar de novo", [&] { os.iniciar(); });

	std::cout << "\n--- Conclusao (RN14) ---\n";
	esperaErro<ErroDeValidacao>("Concluir com data vazia", [&] { os.concluir(""); });
	double total = os.concluir("2026-11-24");
	conferir("concluir() devolve o total do orcamento (200)", quase(total, 200.0));
	conferir("Status Concluida e data de conclusao guardada", os.getStatus() == StatusOS::Concluida && os.getDataConclusao() == "2026-11-24");
	conferir("Baixa nas pecas: 10 - 2 = 8", filtro.getQtdEstoque() == 8);
	esperaErro<OperacaoInvalida>("Concluir de novo", [&] { os.concluir("2026-11-25"); });
	conferir("Estoque nao baixou duas vezes (continua 8)", filtro.getQtdEstoque() == 8);
	esperaErro<OperacaoInvalida>("Cancelar OS concluida", [&] { os.cancelar(); });

	std::cout << "\n--- Cancelamento ---\n";
	Peca vela(2, "Vela", 5.0, 10.0, 10);
	auto orc2 = std::make_unique<Orcamento>(3, &carro, &joao, "2026-11-23");
	orc2->adicionarPeca(&vela, 3);
	orc2->aprovar();
	OrdemServico os2(2, orc2.get(), "2026-11-23");
	os2.cancelar();
	conferir("cancelar() Aberta -> Cancelada, sem mexer no estoque", os2.getStatus() == StatusOS::Cancelada && vela.getQtdEstoque() == 10);
	esperaErro<OperacaoInvalida>("Cancelar de novo", [&] { os2.cancelar(); });
	esperaErro<OperacaoInvalida>("Iniciar OS cancelada", [&] { os2.iniciar(); });

	auto orc3 = std::make_unique<Orcamento>(4, &carro, &joao, "2026-11-23");
	orc3->adicionarPeca(&vela, 1);
	orc3->aprovar();
	OrdemServico os3(3, orc3.get(), "2026-11-23");
	os3.iniciar();
	os3.cancelar();
	conferir("cancelar() tambem funciona EmAndamento", os3.getStatus() == StatusOS::Cancelada);

	std::cout << "\n--- Falta de peca na conclusao ---\n";
	Peca rara(3, "Peca rara", 10.0, 30.0, 5);
	auto orc4 = std::make_unique<Orcamento>(5, &carro, &joao, "2026-11-23");
	orc4->adicionarPeca(&rara, 4);
	orc4->aprovar();
	OrdemServico os4(4, orc4.get(), "2026-11-23");
	os4.iniciar();
	rara.darBaixa(3);   // alguem usou as pecas depois da aprovacao: sobram 2, a OS precisa de 4
	esperaErro<EstoqueInsuficiente>("Concluir sem estoque suficiente", [&] { os4.concluir("2026-11-24"); });
	conferir("Status continua EmAndamento e estoque intacto", os4.getStatus() == StatusOS::EmAndamento && rara.getQtdEstoque() == 2);
	rara.repor(3);
	os4.concluir("2026-11-24");
	conferir("Depois de repor, a OS conclui", os4.getStatus() == StatusOS::Concluida && rara.getQtdEstoque() == 1);

	os.exibir();

	std::cout << (falhas == 0 ? "\nTodos os testes passaram.\n" : "\nHa testes com falha.\n");
	return falhas == 0 ? 0 : 1;
}
