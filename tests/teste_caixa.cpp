// Teste do Caixa (Fase 4).
// O CMake cria este programa sozinho (arquivos tests/teste_*.cpp).

#include <cmath>
#include <iostream>
#include <string>

#include "comum/excecoes.h"
#include "comum/tipos.h"
#include "financeiro/caixa.h"

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
	Caixa caixa;
	conferir("Caixa nasce com saldo 0 e sem lancamentos", quase(caixa.getSaldo(), 0.0) && caixa.getLancamentos().empty());

	caixa.registrarReceita(225.0, "OS #1", "2026-11-23");
	conferir("Receita de 225 deixa o saldo em 225", quase(caixa.getSaldo(), 225.0));
	caixa.registrarDespesa(100.0, "Compra #1 - AutoPecas Silva", "2026-11-22");
	conferir("Despesa de 100 deixa o saldo em 125", quase(caixa.getSaldo(), 125.0));

	conferir("totalReceitas = 225", quase(caixa.totalReceitas(), 225.0));
	conferir("totalDespesas = 100", quase(caixa.totalDespesas(), 100.0));
	conferir("Saldo = receitas - despesas", quase(caixa.getSaldo(), caixa.totalReceitas() - caixa.totalDespesas()));

	const auto& lista = caixa.getLancamentos();
	conferir("Dois lancamentos, na ordem em que aconteceram", lista.size() == 2 && lista[0].descricao == "OS #1" && lista[1].descricao == "Compra #1 - AutoPecas Silva");
	conferir("Tipo de cada lancamento", lista[0].tipo == TipoLancamento::Receita && lista[1].tipo == TipoLancamento::Despesa);
	conferir("Valor e data guardados", quase(lista[0].valor, 225.0) && lista[0].data == "2026-11-23");

	esperaErro<ErroDeValidacao>("Receita de valor 0", [&] { caixa.registrarReceita(0.0, "X", "2026-11-23"); });
	esperaErro<ErroDeValidacao>("Receita de valor negativo", [&] { caixa.registrarReceita(-5.0, "X", "2026-11-23"); });
	esperaErro<ErroDeValidacao>("Despesa de valor 0", [&] { caixa.registrarDespesa(0.0, "X", "2026-11-23"); });
	esperaErro<ErroDeValidacao>("Despesa de valor negativo", [&] { caixa.registrarDespesa(-5.0, "X", "2026-11-23"); });
	conferir("Depois dos erros, saldo e lancamentos continuam iguais", quase(caixa.getSaldo(), 125.0) && caixa.getLancamentos().size() == 2);

	caixa.registrarDespesa(500.0, "Compra grande", "2026-11-23");
	conferir("Despesa maior que o saldo deixa o saldo negativo (-375)", quase(caixa.getSaldo(), -375.0));

	Caixa vazio;
	conferir("Totais de um caixa vazio = 0", quase(vazio.totalReceitas(), 0.0) && quase(vazio.totalDespesas(), 0.0));

	caixa.gerarRelatorio();
	vazio.gerarRelatorio();

	std::cout << (falhas == 0 ? "\nTodos os testes passaram.\n" : "\nHa testes com falha.\n");
	return falhas == 0 ? 0 : 1;
}