// main.cpp
// Ponto de entrada do Simulador de Oficina Mecanica.
//
// Dois modos de uso:
//   oficina          -> abre o menu interativo (uso normal)
//   oficina --demo   -> executa uma demonstracao automatica, passo a passo,
//                       mostrando o fluxo completo, o polimorfismo e os erros

#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "oficina.h"
#include "menu.h"

#include "comum/excecoes.h"
#include "pessoas/pessoa.h"
#include "pessoas/cliente.h"
#include "pessoas/mecanico.h"
#include "veiculos/carro.h"
#include "veiculos/moto.h"
#include "veiculos/caminhonete.h"
#include "estoque/peca.h"
#include "servicos/orcamento.h"
#include "servicos/ordemservico.h"

// Funcoes auxiliares que so existem dentro deste arquivo.
namespace
{
	// Imprime o titulo de cada passo da demonstracao.
	void passo(int numero, const std::string& titulo)
	{
		std::cout << "\n=== PASSO " << numero << ": " << titulo << " ===\n";
	}

	// Formata um valor como dinheiro: 195 vira "R$ 195.00".
	std::string moeda(double valor)
	{
		std::ostringstream saida;
		saida << "R$ " << std::fixed << std::setprecision(2) << valor;
		return saida.str();
	}

	// Executa uma acao que DEVE dar erro e mostra a mensagem do erro.
	// Serve para mostrar na apresentacao que as validacoes funcionam.
	template <typename Acao>
	void esperarErro(const std::string& descricao, Acao acao)
	{
		try
		{
			acao();
			std::cout << "[FALHOU] " << descricao << " deveria ter dado erro\n";
		}
		catch (const std::exception& e)
		{
			std::cout << "[erro esperado] " << descricao << ": " << e.what() << "\n";
		}
	}

	// Demonstracao automatica do fluxo completo da oficina.
	void executarDemonstracao(Oficina& oficina)
	{
		// ---------------------------------------------------------------
		passo(1, "Cadastrar mecanico e cliente");
		// ---------------------------------------------------------------
		Mecanico& joao = oficina.cadastrarMecanico("Joao Silva", "12448017638", "31999990001", "Motor", 60.0);
		Cliente& ana = oficina.cadastrarCliente("Ana Souza", "83671084421", "31988880002");

		// ---------------------------------------------------------------
		passo(2, "Polimorfismo com Pessoa: um ponteiro da classe base, varios comportamentos");
		// ---------------------------------------------------------------
		// O vetor guarda ponteiros para Pessoa, mas cada objeto e de um tipo
		// diferente. Na chamada de exibir(), o C++ escolhe a versao certa
		// (Mecanico::exibir ou Cliente::exibir) sozinho.
		std::vector<const Pessoa*> pessoas = { &joao, &ana };
		for (const Pessoa* p : pessoas)
			p->exibir();

		// ---------------------------------------------------------------
		passo(3, "Cadastrar veiculos (Carro, Moto e Caminhonete) da cliente");
		// ---------------------------------------------------------------
		oficina.cadastrarVeiculo(ana.getId(), std::make_unique<Carro>("ABC1D23", "Onix", 2021, 35000, 4));
		oficina.cadastrarVeiculo(ana.getId(), std::make_unique<Moto>("DEF4567", "CG 160", 2020, 12000, 160));
		oficina.cadastrarVeiculo(ana.getId(), std::make_unique<Caminhonete>("GHI8J90", "Hilux", 2019, 80000, 1000.0));

		// Polimorfismo com Veiculo: a lista guarda Veiculo*, mas cada um
		// responde com o seu tipo e o seu fator de mao de obra.
		ana.listarVeiculos();
		for (const auto& v : ana.getVeiculos())
			std::cout << v->tipo() << " -> fator de mao de obra: " << v->fatorMaoDeObra() << "\n";

		// ---------------------------------------------------------------
		passo(4, "Cadastrar pecas e comprar para o estoque");
		// ---------------------------------------------------------------
		Peca& filtro = oficina.cadastrarPeca("Filtro de oleo", 20.0, 40.0);
		Peca& oleo = oficina.cadastrarPeca("Oleo 5W30", 18.0, 25.0);

		// Cada compra aumenta o estoque e lanca uma despesa no caixa.
		oficina.comprarPecas(filtro.getCodigo(), 5, "AutoPecas Silva", 20.0);
		oficina.comprarPecas(oleo.getCodigo(), 5, "AutoPecas Silva", 18.0);
		oficina.listarEstoque();

		// ---------------------------------------------------------------
		passo(5, "Criar o orcamento (o total e calculado por polimorfismo)");
		// ---------------------------------------------------------------
		// O orcamento guarda varios itens (pecas e servicos). Para calcular
		// o total ele chama calcularSubtotal() em cada item, sem saber de
		// que tipo e cada um.
		Orcamento& orc = oficina.criarOrcamento("ABC1D23", joao.getId());
		orc.adicionarPeca(&filtro, 2);                // 2 x R$ 40 = R$ 80
		orc.adicionarPeca(&oleo, 1);                  // 1 x R$ 25 = R$ 25
		orc.adicionarServico("Troca de oleo", 1.5);   // 1,5 h x R$ 60 x fator do Carro (1.0) = R$ 90
		orc.exibir();
		std::cout << "Total do orcamento: " << moeda(orc.calcularTotal()) << "\n";

		// ---------------------------------------------------------------
		passo(6, "Aprovar o orcamento e executar a ordem de servico");
		// ---------------------------------------------------------------
		oficina.aprovarOrcamento(orc.getId());
		OrdemServico& os = oficina.abrirOS(orc.getId());
		oficina.iniciarOS(os.getId());
		oficina.concluirOS(os.getId());    // da baixa nas pecas e lanca a receita no caixa

		// ---------------------------------------------------------------
		passo(7, "Conferir o caixa e o estoque");
		// ---------------------------------------------------------------
		oficina.relatorioCaixa();          // receita 195 - despesas 190 = saldo 5
		oficina.listarEstoque();           // filtro 5 - 2 = 3, oleo 5 - 1 = 4
		oficina.alertaEstoqueBaixo();      // o filtro (3 unidades) deve aparecer

		// ---------------------------------------------------------------
		passo(8, "Excecoes: erros que o sistema precisa recusar");
		// ---------------------------------------------------------------
		esperarErro("cliente com CPF invalido", [&]() {
			oficina.cadastrarCliente("Pedro", "123", "31977770003");
			});
		esperarErro("carro com placa invalida", []() {
			Carro c("AB12", "Gol", 2018, 1000, 4);
			});
		esperarErro("concluir a mesma OS de novo", [&]() {
			oficina.concluirOS(os.getId());
			});
		esperarErro("comprar uma peca que nao existe", [&]() {
			oficina.comprarPecas(999, 1, "Fornecedor", 10.0);
			});

		std::cout << "\nDemonstracao concluida.\n";
	}
}

int main(int argc, char* argv[])
{
	Oficina oficina;

	// Quando a persistencia existir, carregue os dados salvos aqui:
	// oficina.carregar("data");

	try
	{
		if (argc > 1 && std::string(argv[1]) == "--demo")
		{
			executarDemonstracao(oficina);
		}
		else
		{
			Menu menu(oficina);
			menu.executar();
		}
	}
	catch (const std::exception& e)
	{
		// Rede de seguranca: qualquer erro que o menu nao tenha tratado.
		std::cout << "Erro inesperado: " << e.what() << "\n";
		return 1;
	}

	// E salve os dados antes de sair:
	// oficina.salvar("data");

	return 0;
}