// Teste da Fase 2: Peca, Estoque e Compra.
// O CMake cria este programa sozinho (arquivos tests/teste_*.cpp).

#include <iostream>
#include <memory>
#include <string>

#include "comum/excecoes.h"
#include "estoque/compra.h"
#include "estoque/estoque.h"
#include "estoque/peca.h"

int falhas = 0;

void conferir(const std::string& nome, bool ok)
{
	std::cout << (ok ? "[OK]     " : "[FALHOU] ") << nome << "\n";
	if (!ok) falhas++;
}

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
	std::cout << "--- Peca ---\n";
	Peca filtro(1, "Filtro de oleo", 20.0, 40.0, 3);
	conferir("Peca valida: codigo, nome e estoque", filtro.getCodigo() == 1 && filtro.getNome() == "Filtro de oleo" && filtro.getQtdEstoque() == 3);
	conferir("Margem unitaria = venda - custo", filtro.margemUnitaria() == 20.0);
	esperaErro<ErroDeValidacao>("Preco de venda menor que o custo (RN04)", [] { Peca p(1, "X", 50.0, 40.0); });
	esperaErro<ErroDeValidacao>("Codigo 0", [] { Peca p(0, "X", 10.0, 20.0); });
	esperaErro<ErroDeValidacao>("Nome vazio", [] { Peca p(1, "", 10.0, 20.0); });
	esperaErro<ErroDeValidacao>("Custo 0", [] { Peca p(1, "X", 0.0, 20.0); });
	esperaErro<ErroDeValidacao>("Estoque inicial negativo", [] { Peca p(1, "X", 10.0, 20.0, -1); });
	esperaErro<ErroDeValidacao>("setPrecoVenda abaixo do custo", [&] { filtro.setPrecoVenda(10.0); });

	esperaErro<EstoqueInsuficiente>("darBaixa(5) com 3 em estoque (RN05)", [&] { filtro.darBaixa(5); });
	conferir("Estoque continua 3 depois do erro", filtro.getQtdEstoque() == 3);
	esperaErro<ErroDeValidacao>("darBaixa(0)", [&] { filtro.darBaixa(0); });
	filtro.darBaixa(2);
	conferir("darBaixa(2) deixa 1", filtro.getQtdEstoque() == 1);
	filtro.repor(10);
	conferir("repor(10) deixa 11", filtro.getQtdEstoque() == 11);
	esperaErro<ErroDeValidacao>("repor(-1)", [&] { filtro.repor(-1); });
	conferir("estoqueBaixo: 11 unidades, limite 3 = false", !filtro.estoqueBaixo(3));
	Peca poucas(2, "Vela", 5.0, 9.0, 2);
	conferir("estoqueBaixo: 2 unidades, limite 3 = true", poucas.estoqueBaixo(3));

	std::cout << "\n--- Estoque ---\n";
	Estoque estoque;
	conferir("proximoCodigo com estoque vazio = 1", estoque.proximoCodigo() == 1);
	Peca* p1 = estoque.adicionarPeca(std::make_unique<Peca>(1, "Filtro", 20.0, 40.0, 3));
	estoque.adicionarPeca(std::make_unique<Peca>(5, "Oleo", 18.0, 25.0, 10));
	conferir("proximoCodigo = maior codigo + 1", estoque.proximoCodigo() == 6);
	esperaErro<ErroDeValidacao>("Codigo repetido (RN03)", [&] { estoque.adicionarPeca(std::make_unique<Peca>(1, "Outra", 1.0, 2.0)); });
	esperaErro<ErroDeValidacao>("Adicionar peca nula", [&] { estoque.adicionarPeca(nullptr); });
	conferir("buscarPorCodigo(5) acha", estoque.buscarPorCodigo(5) != nullptr);
	conferir("buscarPorCodigo(999) devolve nullptr", estoque.buscarPorCodigo(999) == nullptr);
	esperaErro<NaoEncontrado>("obterPeca(999)", [&] { estoque.obterPeca(999); });
	conferir("verificarDisponibilidade(1, 3) = true", estoque.verificarDisponibilidade(1, 3));
	conferir("verificarDisponibilidade(1, 4) = false", !estoque.verificarDisponibilidade(1, 4));
	conferir("verificarDisponibilidade de peca inexistente = false", !estoque.verificarDisponibilidade(999, 1));
	estoque.darBaixa(1, 2);
	conferir("Estoque::darBaixa(1, 2) deixa 1", estoque.obterPeca(1).getQtdEstoque() == 1);
	estoque.repor(1, 4);
	conferir("Estoque::repor(1, 4) deixa 5", estoque.obterPeca(1).getQtdEstoque() == 5);
	esperaErro<NaoEncontrado>("Estoque::darBaixa de codigo inexistente", [&] { estoque.darBaixa(999, 1); });
	esperaErro<EstoqueInsuficiente>("Estoque::darBaixa maior que o estoque", [&] { estoque.darBaixa(1, 50); });

	estoque.adicionarPeca(std::make_unique<Peca>(7, "Correia", 30.0, 60.0, 2));
	conferir("listarEstoqueBaixo(3) acha so a Correia", estoque.listarEstoqueBaixo(3).size() == 1);

	for (int i = 100; i < 200; i++)
		estoque.adicionarPeca(std::make_unique<Peca>(i, "Peca " + std::to_string(i), 1.0, 2.0));
	conferir("Ponteiro continua valido depois de 100 pecas novas (unique_ptr)", p1->getCodigo() == 1);

	esperaErro<OperacaoInvalida>("removerPeca com unidades em estoque", [&] { estoque.removerPeca(1); });
	conferir("removerPeca de peca com estoque 0 remove", estoque.removerPeca(100));
	conferir("removerPeca de codigo inexistente = false", !estoque.removerPeca(999));

	std::cout << "\n--- Compra ---\n";
	Peca* oleo = estoque.buscarPorCodigo(5);
	Compra compra(1, "AutoPecas Silva", oleo, 5, 18.0, "2026-11-23");
	conferir("Compra nasce nao registrada", !compra.foiRegistrada());
	conferir("custoTotal = 5 x 18 = 90", compra.custoTotal() == 90.0);
	compra.registrar();
	conferir("registrar() soma ao estoque: 10 + 5 = 15 (RN06)", oleo->getQtdEstoque() == 15 && compra.foiRegistrada());
	esperaErro<OperacaoInvalida>("registrar() de novo", [&] { compra.registrar(); });
	conferir("Estoque continua 15 depois do registro duplo", oleo->getQtdEstoque() == 15);
	esperaErro<ErroDeValidacao>("Compra com peca nula", [] { Compra c(1, "F", nullptr, 1, 1.0, "2026-11-23"); });
	esperaErro<ErroDeValidacao>("Compra com quantidade 0", [&] { Compra c(2, "F", oleo, 0, 1.0, "2026-11-23"); });
	esperaErro<ErroDeValidacao>("Compra com fornecedor vazio", [&] { Compra c(2, "", oleo, 1, 1.0, "2026-11-23"); });
	esperaErro<ErroDeValidacao>("Compra com custo 0", [&] { Compra c(2, "F", oleo, 1, 0.0, "2026-11-23"); });

	std::cout << (falhas == 0 ? "\nTodos os testes passaram.\n" : "\nHa testes com falha.\n");
	return falhas == 0 ? 0 : 1;
}