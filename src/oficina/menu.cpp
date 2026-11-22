#include <iostream>
#include <sstream>
#include <string>

#include "oficina/menu.h"
#include "comum/excecoes.h"
#include "comum/utils.h"
#include "veiculos/carro.h"
#include "veiculos/moto.h"
#include "veiculos/caminhonete.h"

// ---------------------------------------------------------------------------
// Leitura segura do teclado. Funcoes que so existem dentro deste arquivo.
// Quando o seu Utils tiver lerInt/lerDouble/lerLinha, pode mover para la.
// ---------------------------------------------------------------------------
namespace
{
	// Lancada quando a entrada acaba (Ctrl+Z / Ctrl+D ou arquivo de entrada terminou).
	// Nao herda de std::exception de proposito: assim os try/catch das opcoes
	// NAO a engolem, e o menu consegue sair em vez de repetir erros para sempre.
	struct EntradaEncerrada {};

	std::string lerLinha(const std::string& pergunta)
	{
		std::cout << pergunta;
		std::string linha;
		if (!std::getline(std::cin, linha))
			throw EntradaEncerrada();
		return linha;
	}

	// Repete a pergunta ate o usuario digitar um inteiro dentro do intervalo.
	// Texto no lugar de numero ("abc") nao derruba o programa: so pergunta de novo.
	int lerInt(const std::string& pergunta, int minimo, int maximo)
	{
		while (true)
		{
			std::istringstream entrada(lerLinha(pergunta));
			int valor;
			char sobra;
			if ((entrada >> valor) && !(entrada >> sobra) && valor >= minimo && valor <= maximo)
				return valor;
			std::cout << "Valor invalido. Digite um numero inteiro entre " << minimo << " e " << maximo << ".\n";
		}
	}

	// Igual ao lerInt, para numeros com casas decimais. Aceita "12,5" e "12.5".
	double lerDouble(const std::string& pergunta, double minimo)
	{
		while (true)
		{
			std::string texto = lerLinha(pergunta);
			for (char& c : texto)
			{
				if (c == ',')
					c = '.';
			}
			std::istringstream entrada(texto);
			double valor;
			char sobra;
			if ((entrada >> valor) && !(entrada >> sobra) && valor >= minimo)
				return valor;
			std::cout << "Valor invalido. Digite um numero maior ou igual a " << minimo << ".\n";
		}
	}

	bool lerSimNao(const std::string& pergunta)
	{
		while (true)
		{
			std::string resposta = lerLinha(pergunta + " (s/n): ");
			if (resposta == "s" || resposta == "S")
				return true;
			if (resposta == "n" || resposta == "N")
				return false;
			std::cout << "Responda com s ou n.\n";
		}
	}

	const int ID_MAXIMO = 1000000;
}

Menu::Menu(Oficina& oficina) : m_oficina(oficina) {}

// ======================= Menu principal =======================

void Menu::executar()
{
	try
	{
		bool continuar = true;
		while (continuar)
		{
			std::cout << "\n=== OFICINA MECANICA ===\n";
			std::cout << "1 - Clientes e veiculos\n";
			std::cout << "2 - Mecanicos\n";
			std::cout << "3 - Estoque e compras\n";
			std::cout << "4 - Orcamentos\n";
			std::cout << "5 - Ordens de servico\n";
			std::cout << "6 - Caixa\n";
			std::cout << "0 - Sair\n";

			switch (lerInt("Escolha: ", 0, 6))
			{
				case 1: menuClientes(); break;
				case 2: menuMecanicos(); break;
				case 3: menuEstoque(); break;
				case 4: menuOrcamentos(); break;
				case 5: menuOrdens(); break;
				case 6: menuCaixa(); break;
				case 0: continuar = false; break;
			}
		}
		std::cout << "Ate logo!\n";
	}
	catch (const EntradaEncerrada&)
	{
		std::cout << "\nEntrada encerrada. Saindo...\n";
	}
}

// ======================= Clientes e veiculos =======================

void Menu::menuClientes()
{
	while (true)
	{
		std::cout << "\n--- CLIENTES E VEICULOS ---\n";
		std::cout << "1 - Cadastrar cliente\n";
		std::cout << "2 - Listar clientes\n";
		std::cout << "3 - Buscar cliente por CPF/CNPJ\n";
		std::cout << "4 - Remover cliente\n";
		std::cout << "5 - Cadastrar veiculo de um cliente\n";
		std::cout << "6 - Listar veiculos de um cliente\n";
		std::cout << "0 - Voltar\n";

		int opcao = lerInt("Escolha: ", 0, 6);
		if (opcao == 0)
			return;

		// Qualquer erro das regras vira uma mensagem e o menu continua.
		try
		{
			switch (opcao)
			{
				case 1:
				{
					std::string nome = lerLinha("Nome: ");
					std::string cpf = lerLinha("CPF (11 digitos) ou CNPJ (14 digitos), so numeros: ");
					std::string tel = lerLinha("Telefone: ");
					Cliente& c = m_oficina.cadastrarCliente(nome, cpf, tel);
					std::cout << "Cliente cadastrado com o id " << c.getId() << "\n";
					break;
				}
				case 2:
					m_oficina.listarClientes();
					break;
				case 3:
				{
					Cliente* c = m_oficina.buscarClientePorCpf(lerLinha("CPF/CNPJ: "));
					if (c != nullptr)
						c->exibir();
					else
						std::cout << "Cliente nao encontrado\n";
					break;
				}
				case 4:
				{
					int id = lerInt("Id do cliente: ", 1, ID_MAXIMO);
					if (!lerSimNao("Confirma a remocao"))
						break;
					if (m_oficina.removerCliente(id))
						std::cout << "Cliente removido\n";
					else
						std::cout << "Cliente nao encontrado\n";
					break;
				}
				case 5:
				{
					int id = lerInt("Id do cliente: ", 1, ID_MAXIMO);
					m_oficina.obterCliente(id);   // confere se o cliente existe antes de perguntar os dados
					m_oficina.cadastrarVeiculo(id, lerNovoVeiculo());
					std::cout << "Veiculo cadastrado\n";
					break;
				}
				case 6:
					m_oficina.obterCliente(lerInt("Id do cliente: ", 1, ID_MAXIMO)).listarVeiculos();
					break;
			}
		}
		catch (const std::exception& e)
		{
			std::cout << "Erro: " << e.what() << "\n";
		}
	}
}

// Pergunta o tipo e os dados e cria o objeto da classe certa (Carro, Moto ou Caminhonete).
// Os intervalos aceitos aqui sao largos de proposito: quem recusa valor ruim e a propria
// classe do veiculo, e a mensagem de erro dela e a que aparece para o usuario.
std::unique_ptr<Veiculo> Menu::lerNovoVeiculo()
{
	std::cout << "Tipo do veiculo:\n1 - Carro\n2 - Moto\n3 - Caminhonete\n";
	int tipo = lerInt("Escolha: ", 1, 3);
	std::string placa = lerLinha("Placa (AAA1234 ou AAA1A23): ");
	std::string modelo = lerLinha("Modelo: ");
	int ano = lerInt("Ano: ", 0, 9999);
	int km = lerInt("Quilometragem: ", 0, 100000000);

	if (tipo == 1)
	{
		int portas = lerInt("Numero de portas: ", 0, 99);
		return std::make_unique<Carro>(placa, modelo, ano, km, portas);
	}
	if (tipo == 2)
	{
		int cilindradas = lerInt("Cilindradas: ", 0, 99999);
		return std::make_unique<Moto>(placa, modelo, ano, km, cilindradas);
	}
	double carga = lerDouble("Capacidade de carga (kg): ", 0.0);
	return std::make_unique<Caminhonete>(placa, modelo, ano, km, carga);
}

// ======================= Mecanicos =======================

void Menu::menuMecanicos()
{
	while (true)
	{
		std::cout << "\n--- MECANICOS ---\n";
		std::cout << "1 - Cadastrar mecanico\n";
		std::cout << "2 - Listar mecanicos\n";
		std::cout << "0 - Voltar\n";

		int opcao = lerInt("Escolha: ", 0, 2);
		if (opcao == 0)
			return;

		try
		{
			if (opcao == 1)
			{
				std::string nome = lerLinha("Nome: ");
				std::string cpf = lerLinha("CPF (11 digitos), so numeros: ");
				std::string tel = lerLinha("Telefone: ");
				std::string especialidade = lerLinha("Especialidade: ");
				double valorHora = lerDouble("Valor da hora (R$): ", 0.0);
				Mecanico& m = m_oficina.cadastrarMecanico(nome, cpf, tel, especialidade, valorHora);
				std::cout << "Mecanico cadastrado com o id " << m.getId() << "\n";
			}
			else
			{
				m_oficina.listarMecanicos();
			}
		}
		catch (const std::exception& e)
		{
			std::cout << "Erro: " << e.what() << "\n";
		}
	}
}

// ======================= Estoque e compras =======================

void Menu::menuEstoque()
{
	while (true)
	{
		std::cout << "\n--- ESTOQUE E COMPRAS ---\n";
		std::cout << "1 - Cadastrar peca\n";
		std::cout << "2 - Comprar pecas\n";
		std::cout << "3 - Listar estoque\n";
		std::cout << "4 - Alerta de estoque baixo\n";
		std::cout << "5 - Listar compras\n";
		std::cout << "0 - Voltar\n";

		int opcao = lerInt("Escolha: ", 0, 5);
		if (opcao == 0)
			return;

		try
		{
			switch (opcao)
			{
				case 1:
				{
					std::string nome = lerLinha("Nome da peca: ");
					double custo = lerDouble("Preco de custo (R$): ", 0.0);
					double venda = lerDouble("Preco de venda (R$): ", 0.0);
					Peca& p = m_oficina.cadastrarPeca(nome, custo, venda);
					std::cout << "Peca cadastrada com o codigo " << p.getCodigo() << "\n";
					break;
				}
				case 2:
				{
					int codigo = lerInt("Codigo da peca: ", 1, ID_MAXIMO);
					int qtd = lerInt("Quantidade: ", 0, ID_MAXIMO);
					std::string fornecedor = lerLinha("Fornecedor: ");
					double custo = lerDouble("Custo unitario (R$): ", 0.0);
					Compra& c = m_oficina.comprarPecas(codigo, qtd, fornecedor, custo);
					std::cout << "Compra #" << c.getId() << " registrada: "
						<< Utils::formatarMoeda(c.custoTotal()) << " lancados como despesa\n";
					break;
				}
				case 3:
					m_oficina.listarEstoque();
					break;
				case 4:
					m_oficina.alertaEstoqueBaixo();
					break;
				case 5:
					m_oficina.listarCompras();
					break;
			}
		}
		catch (const std::exception& e)
		{
			std::cout << "Erro: " << e.what() << "\n";
		}
	}
}

// ======================= Orcamentos =======================

void Menu::menuOrcamentos()
{
	while (true)
	{
		std::cout << "\n--- ORCAMENTOS ---\n";
		std::cout << "1 - Criar orcamento\n";
		std::cout << "2 - Listar orcamentos\n";
		std::cout << "3 - Ver detalhes de um orcamento\n";
		std::cout << "4 - Adicionar peca\n";
		std::cout << "5 - Adicionar servico\n";
		std::cout << "6 - Aplicar desconto\n";
		std::cout << "7 - Aprovar orcamento\n";
		std::cout << "8 - Recusar orcamento\n";
		std::cout << "0 - Voltar\n";

		int opcao = lerInt("Escolha: ", 0, 8);
		if (opcao == 0)
			return;

		try
		{
			switch (opcao)
			{
				case 1:
				{
					std::string placa = lerLinha("Placa do veiculo: ");
					int idMecanico = lerInt("Id do mecanico: ", 1, ID_MAXIMO);
					Orcamento& o = m_oficina.criarOrcamento(placa, idMecanico);
					std::cout << "Orcamento criado com o id " << o.getId() << "\n";
					break;
				}
				case 2:
					m_oficina.listarOrcamentos();
					break;
				case 3:
					m_oficina.obterOrcamento(lerInt("Id do orcamento: ", 1, ID_MAXIMO)).exibir();
					break;
				case 4:
				{
					int idOrc = lerInt("Id do orcamento: ", 1, ID_MAXIMO);
					int codigo = lerInt("Codigo da peca: ", 1, ID_MAXIMO);
					int qtd = lerInt("Quantidade: ", 0, ID_MAXIMO);
					m_oficina.obterOrcamento(idOrc).adicionarPeca(&m_oficina.obterPeca(codigo), qtd);
					std::cout << "Peca adicionada ao orcamento\n";
					break;
				}
				case 5:
				{
					int idOrc = lerInt("Id do orcamento: ", 1, ID_MAXIMO);
					std::string descricao = lerLinha("Descricao do servico: ");
					double horas = lerDouble("Horas de trabalho: ", 0.0);
					m_oficina.obterOrcamento(idOrc).adicionarServico(descricao, horas);
					std::cout << "Servico adicionado ao orcamento\n";
					break;
				}
				case 6:
				{
					int idOrc = lerInt("Id do orcamento: ", 1, ID_MAXIMO);
					double percentual = lerDouble("Desconto (%), de 0 a 20: ", 0.0);
					m_oficina.obterOrcamento(idOrc).aplicarDesconto(percentual);
					std::cout << "Desconto aplicado\n";
					break;
				}
				case 7:
					m_oficina.aprovarOrcamento(lerInt("Id do orcamento: ", 1, ID_MAXIMO));
					std::cout << "Orcamento aprovado\n";
					break;
				case 8:
					m_oficina.recusarOrcamento(lerInt("Id do orcamento: ", 1, ID_MAXIMO));
					std::cout << "Orcamento recusado\n";
					break;
			}
		}
		catch (const std::exception& e)
		{
			std::cout << "Erro: " << e.what() << "\n";
		}
	}
}

// ======================= Ordens de servico =======================

void Menu::menuOrdens()
{
	while (true)
	{
		std::cout << "\n--- ORDENS DE SERVICO ---\n";
		std::cout << "1 - Abrir ordem de servico (de um orcamento aprovado)\n";
		std::cout << "2 - Iniciar\n";
		std::cout << "3 - Concluir\n";
		std::cout << "4 - Cancelar\n";
		std::cout << "5 - Listar ordens de servico\n";
		std::cout << "0 - Voltar\n";

		int opcao = lerInt("Escolha: ", 0, 5);
		if (opcao == 0)
			return;

		try
		{
			switch (opcao)
			{
				case 1:
				{
					OrdemServico& os = m_oficina.abrirOS(lerInt("Id do orcamento: ", 1, ID_MAXIMO));
					std::cout << "Ordem de servico aberta com o id " << os.getId() << "\n";
					break;
				}
				case 2:
					m_oficina.iniciarOS(lerInt("Id da ordem de servico: ", 1, ID_MAXIMO));
					std::cout << "Ordem de servico iniciada\n";
					break;
				case 3:
					m_oficina.concluirOS(lerInt("Id da ordem de servico: ", 1, ID_MAXIMO));
					std::cout << "Ordem de servico concluida: pecas baixadas e receita lancada no caixa\n";
					break;
				case 4:
					m_oficina.cancelarOS(lerInt("Id da ordem de servico: ", 1, ID_MAXIMO));
					std::cout << "Ordem de servico cancelada\n";
					break;
				case 5:
					m_oficina.listarOS();
					break;
			}
		}
		catch (const std::exception& e)
		{
			std::cout << "Erro: " << e.what() << "\n";
		}
	}
}

// ======================= Caixa =======================

void Menu::menuCaixa()
{
	while (true)
	{
		std::cout << "\n--- CAIXA ---\n";
		std::cout << "1 - Relatorio do caixa\n";
		std::cout << "0 - Voltar\n";

		if (lerInt("Escolha: ", 0, 1) == 0)
			return;
		m_oficina.relatorioCaixa();
	}
}
