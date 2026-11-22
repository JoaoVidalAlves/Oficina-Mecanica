// Teste da Oficina (Fase 5): as regras de negocio que o Menu usa.
// O CMake cria este programa sozinho (arquivos tests/teste_*.cpp).

#include <cmath>
#include <iostream>
#include <memory>
#include <string>

#include "comum/excecoes.h"
#include "comum/tipos.h"
#include "oficina/oficina.h"
#include "veiculos/caminhonete.h"
#include "veiculos/carro.h"
#include "veiculos/moto.h"

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
	Oficina of;

	std::cout << "--- Clientes (RN01) ---\n";
	Cliente& ana = of.cadastrarCliente("Ana Souza", "83671084421", "31988880002");
	Cliente& bia = of.cadastrarCliente("Bia Lima", "11122233344", "31977770003");
	conferir("Ids em sequencia: 1 e 2", ana.getId() == 1 && bia.getId() == 2);
	esperaErro<ErroDeValidacao>("CPF repetido", [&] { of.cadastrarCliente("Outra", "83671084421", "3133334444"); });
	esperaErro<ErroDeValidacao>("CPF invalido", [&] { of.cadastrarCliente("Carlos", "123", "3133334444"); });
	Cliente& carlos = of.cadastrarCliente("Carlos Reis", "55566677788", "3133334444");
	conferir("Cadastro invalido nao gasta id (proximo cliente recebe 3)", carlos.getId() == 3);
	conferir("buscarClientePorCpf acha", of.buscarClientePorCpf("11122233344") == &bia);
	conferir("buscarClientePorCpf de CPF desconhecido = nullptr", of.buscarClientePorCpf("00000000000") == nullptr);
	esperaErro<NaoEncontrado>("obterCliente(999)", [&] { of.obterCliente(999); });

	std::cout << "\n--- Mecanicos ---\n";
	Mecanico& joao = of.cadastrarMecanico("Joao Silva", "12448017638", "31999990001", "Motor", 60.0);
	conferir("Mecanico recebe o proximo id (4)", joao.getId() == 4);
	esperaErro<ErroDeValidacao>("Mecanico com CPF repetido", [&] { of.cadastrarMecanico("Outro", "12448017638", "3133334444", "Freio", 50.0); });
	esperaErro<NaoEncontrado>("obterMecanico(999)", [&] { of.obterMecanico(999); });

	std::cout << "\n--- Veiculos (RN02) ---\n";
	of.cadastrarVeiculo(ana.getId(), std::make_unique<Carro>("ABC1D23", "Onix", 2021, 35000, 4));
	of.cadastrarVeiculo(ana.getId(), std::make_unique<Moto>("DEF4567", "CG 160", 2020, 12000, 160));
	of.cadastrarVeiculo(bia.getId(), std::make_unique<Caminhonete>("GHI8J90", "Hilux", 2019, 80000, 1000.0));
	esperaErro<ErroDeValidacao>("Mesma placa em OUTRO cliente", [&] { of.cadastrarVeiculo(bia.getId(), std::make_unique<Carro>("abc1d23", "Gol", 2018, 1000, 4)); });
	esperaErro<NaoEncontrado>("Veiculo para cliente inexistente", [&] { of.cadastrarVeiculo(999, std::make_unique<Carro>("JKL1234", "Gol", 2018, 1000, 4)); });
	esperaErro<ErroDeValidacao>("Veiculo nulo", [&] { of.cadastrarVeiculo(ana.getId(), nullptr); });
	conferir("buscarVeiculoPorPlaca acha em qualquer cliente, mesmo em minusculas", of.buscarVeiculoPorPlaca("ghi8j90") != nullptr && of.buscarVeiculoPorPlaca("ABC1D23") != nullptr);
	conferir("buscarVeiculoPorPlaca desconhecida = nullptr", of.buscarVeiculoPorPlaca("ZZZ9999") == nullptr);

	std::cout << "\n--- Pecas e compras (RN15) ---\n";
	Peca& filtro = of.cadastrarPeca("Filtro de oleo", 20.0, 40.0);
	Peca& oleo = of.cadastrarPeca("Oleo 5W30", 18.0, 25.0);
	conferir("Codigos em sequencia: 1 e 2, estoque comecando em 0", filtro.getCodigo() == 1 && oleo.getCodigo() == 2 && filtro.getQtdEstoque() == 0);
	esperaErro<ErroDeValidacao>("Peca com venda menor que o custo", [&] { of.cadastrarPeca("Ruim", 50.0, 40.0); });
	of.comprarPecas(filtro.getCodigo(), 5, "AutoPecas Silva", 20.0);
	of.comprarPecas(oleo.getCodigo(), 5, "AutoPecas Silva", 18.0);
	conferir("Compras repoem o estoque (5 e 5)", filtro.getQtdEstoque() == 5 && oleo.getQtdEstoque() == 5);
	conferir("Despesas no caixa: 100 + 90 = 190", quase(of.getCaixa().totalDespesas(), 190.0) && quase(of.getCaixa().getSaldo(), -190.0));
	esperaErro<NaoEncontrado>("Comprar peca que nao existe", [&] { of.comprarPecas(999, 1, "Fornecedor", 10.0); });
	esperaErro<ErroDeValidacao>("Comprar quantidade 0", [&] { of.comprarPecas(filtro.getCodigo(), 0, "Fornecedor", 10.0); });
	esperaErro<ErroDeValidacao>("Comprar com fornecedor vazio", [&] { of.comprarPecas(filtro.getCodigo(), 1, "", 10.0); });
	conferir("Compras recusadas nao mexem no estoque nem no caixa", filtro.getQtdEstoque() == 5 && quase(of.getCaixa().totalDespesas(), 190.0));

	std::cout << "\n--- Orcamento e OS: fluxo completo (RN12, RN14) ---\n";
	esperaErro<NaoEncontrado>("Orcamento para placa inexistente", [&] { of.criarOrcamento("ZZZ9999", joao.getId()); });
	esperaErro<NaoEncontrado>("Orcamento com mecanico inexistente", [&] { of.criarOrcamento("ABC1D23", 999); });
	Orcamento& orc = of.criarOrcamento("ABC1D23", joao.getId());
	orc.adicionarPeca(&filtro, 2);                // 80
	orc.adicionarPeca(&oleo, 1);                  // 25
	orc.adicionarServico("Troca de oleo", 1.5);   // 1,5 h x 60 x 1.0 = 90
	conferir("Total do orcamento = 195", quase(orc.calcularTotal(), 195.0));
	esperaErro<OperacaoInvalida>("abrirOS de orcamento Pendente", [&] { of.abrirOS(orc.getId()); });
	of.aprovarOrcamento(orc.getId());
	OrdemServico& os = of.abrirOS(orc.getId());
	conferir("OS aberta com id 1, ligada ao orcamento", os.getId() == 1 && os.getOrcamento() == &orc);
	esperaErro<OperacaoInvalida>("Segunda OS para o mesmo orcamento", [&] { of.abrirOS(orc.getId()); });
	esperaErro<OperacaoInvalida>("concluirOS sem iniciar", [&] { of.concluirOS(os.getId()); });
	conferir("Caixa e estoque intactos depois do erro", quase(of.getCaixa().getSaldo(), -190.0) && filtro.getQtdEstoque() == 5);
	of.iniciarOS(os.getId());
	of.concluirOS(os.getId());
	conferir("Receita de 195 lancada: saldo = 195 - 190 = 5", quase(of.getCaixa().totalReceitas(), 195.0) && quase(of.getCaixa().getSaldo(), 5.0));
	conferir("Estoque baixou: filtro 3 e oleo 4", filtro.getQtdEstoque() == 3 && oleo.getQtdEstoque() == 4);
	esperaErro<OperacaoInvalida>("concluirOS de novo", [&] { of.concluirOS(os.getId()); });
	conferir("Receita nao foi lancada duas vezes", quase(of.getCaixa().totalReceitas(), 195.0) && filtro.getQtdEstoque() == 3);
	esperaErro<NaoEncontrado>("obterOS(999)", [&] { of.obterOS(999); });
	esperaErro<NaoEncontrado>("obterOrcamento(999)", [&] { of.obterOrcamento(999); });

	std::cout << "\n--- Recusar e cancelar ---\n";
	Orcamento& orc2 = of.criarOrcamento("DEF4567", joao.getId());
	orc2.adicionarServico("Revisao da moto", 1.0);
	of.recusarOrcamento(orc2.getId());
	conferir("recusarOrcamento muda para Recusado", orc2.getStatus() == StatusOrcamento::Recusado);
	esperaErro<OperacaoInvalida>("abrirOS de orcamento recusado", [&] { of.abrirOS(orc2.getId()); });
	Orcamento& orc3 = of.criarOrcamento("GHI8J90", joao.getId());
	orc3.adicionarServico("Revisao da Hilux", 2.0);
	of.aprovarOrcamento(orc3.getId());
	OrdemServico& os3 = of.abrirOS(orc3.getId());
	of.cancelarOS(os3.getId());
	conferir("cancelarOS muda para Cancelada, sem receita", os3.getStatus() == StatusOS::Cancelada && quase(of.getCaixa().totalReceitas(), 195.0));

	std::cout << "\n--- Remover cliente (RN16) ---\n";
	esperaErro<OperacaoInvalida>("Remover cliente que tem orcamentos", [&] { of.removerCliente(ana.getId()); });
	esperaErro<OperacaoInvalida>("Remover cliente com OS (Bia, Hilux)", [&] { of.removerCliente(bia.getId()); });
	conferir("Remover cliente sem orcamentos remove (Carlos)", of.removerCliente(carlos.getId()));
	conferir("Remover cliente que nao existe = false", !of.removerCliente(999));
	conferir("Cliente removido some da busca", of.buscarClientePorCpf("55566677788") == nullptr);
	conferir("Os orcamentos dos outros clientes continuam validos", orc.getVeiculo()->getPlaca() == "ABC1D23");

	std::cout << "\n--- Alertas e listagens ---\n";
	of.alertaEstoqueBaixo();
	of.listarOrcamentos();
	of.relatorioCaixa();

	std::cout << (falhas == 0 ? "\nTodos os testes passaram.\n" : "\nHa testes com falha.\n");
	return falhas == 0 ? 0 : 1;
}
