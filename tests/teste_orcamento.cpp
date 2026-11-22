// Teste da Fase 3: ItemOrcamento, ItemPeca, ItemServico e Orcamento.
// O CMake cria este programa sozinho (arquivos tests/teste_*.cpp).

#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "comum/excecoes.h"
#include "comum/tipos.h"
#include "estoque/peca.h"
#include "pessoas/mecanico.h"
#include "servicos/itemPeca.h"
#include "servicos/itemServico.h"
#include "servicos/orcamento.h"
#include "veiculos/carro.h"
#include "veiculos/moto.h"

int falhas = 0;

void conferir(const std::string& nome, bool ok)
{
	std::cout << (ok ? "[OK]     " : "[FALHOU] ") << nome << "\n";
	if (!ok) falhas++;
}

// Compara numeros com casas decimais (evita problema de arredondamento).
bool quase(double a, double b) { return std::fabs(a - b) < 0.001; }

// Confere que a acao lanca EXATAMENTE o tipo de erro esperado.
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
	// Material de teste: um mecanico (R$ 60/h), um carro, uma moto e duas pecas.
	Mecanico joao(1, "Joao Silva", "12448017638", "31999990001", "Motor", 60.0);
	Carro carro("ABC1234", "Gol", 2018, 10000, 4);
	Moto moto("DEF5678", "CG 160", 2020, 5000, 160);
	Peca filtro(1, "Filtro de oleo", 20.0, 40.0, 10);
	Peca oleo(2, "Oleo 5W30", 18.0, 25.0, 10);

	std::cout << "--- Itens (polimorfismo) ---\n";
	ItemPeca itemPeca(&filtro, 2);
	ItemServico itemServico("Troca de oleo", 2.0, 60.0, 1.0);
	conferir("ItemPeca: 2 x R$ 40 = 80", quase(itemPeca.calcularSubtotal(), 80.0));
	conferir("ItemServico: 2 h x 60 x 1.0 = 120", quase(itemServico.calcularSubtotal(), 120.0));
	conferir("tipo() de cada item", itemPeca.tipo() == "PECA" && itemServico.tipo() == "SERVICO");
	std::vector<std::unique_ptr<ItemOrcamento>> lista;   // tipos diferentes na mesma lista
	lista.push_back(std::make_unique<ItemPeca>(&oleo, 1));
	lista.push_back(std::make_unique<ItemServico>("Alinhamento", 1.0, 60.0, 1.0));
	double soma = 0;
	for (const auto& i : lista) soma += i->calcularSubtotal();
	conferir("Mesma chamada em itens diferentes: 25 + 60 = 85", quase(soma, 85.0));
	conferir("ItemPeca sem estoque: estaDisponivel() = false", !ItemPeca(&filtro, 11).estaDisponivel());
	conferir("ItemServico: estaDisponivel() = true e sem peca associada", itemServico.estaDisponivel() && itemServico.getPecaAssociada() == nullptr);
	esperaErro<ErroDeValidacao>("ItemPeca com peca nula", [] { ItemPeca i(nullptr, 1); });
	esperaErro<ErroDeValidacao>("ItemPeca com quantidade 0", [&] { ItemPeca i(&filtro, 0); });
	esperaErro<ErroDeValidacao>("ItemServico com descricao vazia", [] { ItemServico s("", 1.0, 60.0, 1.0); });
	esperaErro<ErroDeValidacao>("ItemServico com 0 horas", [] { ItemServico s("X", 0.0, 60.0, 1.0); });
	esperaErro<ErroDeValidacao>("ItemServico com valor da hora 0", [] { ItemServico s("X", 1.0, 0.0, 1.0); });

	std::cout << "\n--- Orcamento: totais ---\n";
	Orcamento orc(1, &carro, &joao, "2026-11-23");
	conferir("Orcamento nasce Pendente, sem itens, total 0", orc.getStatus() == StatusOrcamento::Pendente && orc.getItens().empty() && quase(orc.calcularTotal(), 0.0));
	orc.adicionarPeca(&filtro, 2);                // 2 x 40 = 80
	orc.adicionarPeca(&oleo, 1);                  // 1 x 25 = 25
	orc.adicionarServico("Troca de oleo", 2.0);   // 2 h x 60 x 1.0 (Carro) = 120
	conferir("Total em Carro = 80 + 25 + 120 = 225", quase(orc.calcularTotal(), 225.0));

	Orcamento orcMoto(2, &moto, &joao, "2026-11-23");
	orcMoto.adicionarPeca(&filtro, 2);
	orcMoto.adicionarPeca(&oleo, 1);
	orcMoto.adicionarServico("Troca de oleo", 2.0);   // 2 h x 60 x 0.8 (Moto) = 96
	conferir("Mesmo servico em Moto: 80 + 25 + 96 = 201", quase(orcMoto.calcularTotal(), 201.0));

	orc.aplicarDesconto(10.0);
	conferir("Desconto de 10% sobre 225 = 202.50 (RN10)", quase(orc.calcularTotal(), 202.5));
	esperaErro<ErroDeValidacao>("Desconto de 25%", [&] { orc.aplicarDesconto(25.0); });
	esperaErro<ErroDeValidacao>("Desconto negativo", [&] { orc.aplicarDesconto(-1.0); });
	orc.aplicarDesconto(0.0);

	std::cout << "\n--- Orcamento: regras ---\n";
	Peca rara(9, "Peca rara", 10.0, 30.0, 1);
	esperaErro<EstoqueInsuficiente>("adicionarPeca maior que o estoque", [&] { orc.adicionarPeca(&rara, 5); });
	esperaErro<ErroDeValidacao>("Peca repetida no orcamento (RN08)", [&] { orc.adicionarPeca(&filtro, 1); });
	esperaErro<ErroDeValidacao>("adicionarPeca com peca nula", [&] { orc.adicionarPeca(nullptr, 1); });
	esperaErro<ErroDeValidacao>("adicionarItem nulo", [&] { orc.adicionarItem(nullptr); });
	esperaErro<ErroDeValidacao>("Orcamento sem veiculo", [&] { Orcamento o(3, nullptr, &joao, "2026-11-23"); });
	esperaErro<ErroDeValidacao>("Orcamento sem mecanico", [&] { Orcamento o(3, &carro, nullptr, "2026-11-23"); });

	Orcamento vazio(3, &carro, &joao, "2026-11-23");
	esperaErro<OperacaoInvalida>("Aprovar orcamento vazio (RN09)", [&] { vazio.aprovar(); });

	Orcamento remocao(4, &carro, &joao, "2026-11-23");
	remocao.adicionarPeca(&filtro, 1);
	remocao.adicionarServico("Revisao", 1.0);
	remocao.removerItem(0);
	conferir("removerItem(0) deixa 1 item", remocao.getItens().size() == 1);
	esperaErro<NaoEncontrado>("removerItem de posicao inexistente", [&] { remocao.removerItem(7); });

	std::cout << "\n--- Aprovar, recusar e alterar (RN07) ---\n";
	conferir("pecasDisponiveis() = true", orc.pecasDisponiveis());
	orc.aprovar();
	conferir("aprovar() muda para Aprovado", orc.getStatus() == StatusOrcamento::Aprovado);
	esperaErro<OperacaoInvalida>("adicionarServico em orcamento aprovado", [&] { orc.adicionarServico("X", 1.0); });
	esperaErro<OperacaoInvalida>("adicionarPeca em orcamento aprovado", [&] { orc.adicionarPeca(&oleo, 1); });
	esperaErro<OperacaoInvalida>("aplicarDesconto em orcamento aprovado", [&] { orc.aplicarDesconto(5.0); });
	esperaErro<OperacaoInvalida>("removerItem em orcamento aprovado", [&] { orc.removerItem(0); });
	esperaErro<OperacaoInvalida>("aprovar() de novo", [&] { orc.aprovar(); });
	esperaErro<OperacaoInvalida>("recusar() de orcamento aprovado", [&] { orc.recusar(); });

	Orcamento recusado(5, &carro, &joao, "2026-11-23");
	recusado.adicionarServico("Revisao", 1.0);
	recusado.recusar();
	conferir("recusar() muda para Recusado", recusado.getStatus() == StatusOrcamento::Recusado);
	esperaErro<OperacaoInvalida>("aprovar() de orcamento recusado", [&] { recusado.aprovar(); });

	Orcamento mudou(6, &carro, &joao, "2026-11-23");
	mudou.adicionarPeca(&oleo, 8);
	oleo.darBaixa(5);   // alguem usou pecas depois de montar o orcamento: sobram 5, o orcamento pede 8
	conferir("pecasDisponiveis() = false quando o estoque mudou", !mudou.pecasDisponiveis());
	esperaErro<EstoqueInsuficiente>("aprovar() com estoque que mudou", [&] { mudou.aprovar(); });
	oleo.repor(5);

	std::cout << "\n--- Baixa nos itens (usada pela OS) ---\n";
	esperaErro<OperacaoInvalida>("darBaixaNosItens em orcamento pendente", [&] { remocao.darBaixaNosItens(); });
	int filtroAntes = filtro.getQtdEstoque();
	int oleoAntes = oleo.getQtdEstoque();
	orc.darBaixaNosItens();
	conferir("darBaixaNosItens: filtro -2 e oleo -1 (servico nao mexe no estoque)",
		filtro.getQtdEstoque() == filtroAntes - 2 && oleo.getQtdEstoque() == oleoAntes - 1);

	// Tudo ou nada: se UMA peca falhar, nenhuma pode ser baixada.
	Peca a(10, "Peca A", 5.0, 10.0, 5);
	Peca b(11, "Peca B", 5.0, 10.0, 5);
	Orcamento parcial(7, &carro, &joao, "2026-11-23");
	parcial.adicionarPeca(&a, 2);
	parcial.adicionarPeca(&b, 3);
	parcial.aprovar();
	b.darBaixa(4);   // B fica com 1, mas o orcamento precisa de 3
	esperaErro<EstoqueInsuficiente>("darBaixaNosItens com falta de uma peca", [&] { parcial.darBaixaNosItens(); });
	conferir("Nada foi baixado (a peca A continua com 5)", a.getQtdEstoque() == 5 && b.getQtdEstoque() == 1);

	std::cout << "\n--- Tipos (enums) ---\n";
	conferir("paraTexto dos enums", paraTexto(StatusOrcamento::Aprovado) == "Aprovado" && paraTexto(StatusOS::EmAndamento) == "EmAndamento" && paraTexto(TipoLancamento::Receita) == "Receita");
	conferir("deTexto desfaz o paraTexto", deTextoStatusOrcamento("Recusado") == StatusOrcamento::Recusado && deTextoStatusOS("Concluida") == StatusOS::Concluida && deTextoTipoLancamento("Despesa") == TipoLancamento::Despesa);
	esperaErro<ErroDeValidacao>("deTexto com texto desconhecido", [] { deTextoStatusOrcamento("xyz"); });

	std::cout << "\n--- exibir() ---\n";
	orcMoto.exibir();

	std::cout << (falhas == 0 ? "\nTodos os testes passaram.\n" : "\nHa testes com falha.\n");
	return falhas == 0 ? 0 : 1;
}