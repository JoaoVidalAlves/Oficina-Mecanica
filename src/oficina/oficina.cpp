// Em compiladores da Microsoft, evita um aviso sobre std::localtime.
// Em outros compiladores, esta linha nao faz nada.
#define _CRT_SECURE_NO_WARNINGS

#include <ctime>
#include <iostream>

#include "oficina/oficina.h"
#include "comum/excecoes.h"
#include "comum/utils.h"

namespace
{
	// Data de hoje no formato AAAA-MM-DD. Quando o seu Utils tiver dataHoje(),
	// troque esta funcao por Utils::dataHoje().
	std::string dataHoje()
	{
		std::time_t agora = std::time(nullptr);
		std::tm* t = std::localtime(&agora);
		char buffer[11];
		std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", t);
		return buffer;
	}
}

// ======================= Clientes =======================

// RN01: o CPF/CNPJ nao se repete entre clientes.
Cliente& Oficina::cadastrarCliente(const std::string& nome, const std::string& cpf, const std::string& tel)
{
	if (buscarClientePorCpf(cpf) != nullptr)
		throw ErroDeValidacao("Ja existe um cliente com este CPF/CNPJ");

	// Cria primeiro (o construtor valida os dados) e so DEPOIS gasta um id:
	// se os dados forem invalidos, nenhum numero de id e desperdicado.
	auto cliente = std::make_unique<Cliente>(m_proximoIdPessoa, nome, cpf, tel);
	m_proximoIdPessoa++;
	m_clientes.push_back(std::move(cliente));
	return *m_clientes.back();
}

Cliente* Oficina::buscarClientePorCpf(const std::string& cpf) const
{
	for (const auto& c : m_clientes)
	{
		if (c->getCPFCNPJ() == cpf)
			return c.get();
	}
	return nullptr;   // nao achou: resposta normal, nao e erro
}

Cliente& Oficina::obterCliente(int id) const
{
	for (const auto& c : m_clientes)
	{
		if (c->getId() == id)
			return *c;
	}
	throw NaoEncontrado("Cliente nao encontrado: id " + std::to_string(id));
}

// Um orcamento guarda um PONTEIRO para o veiculo do cliente. Se o cliente fosse
// removido, os veiculos seriam destruidos e o orcamento ficaria apontando para o nada.
bool Oficina::clienteTemOrcamentos(const Cliente& cliente) const
{
	for (const auto& orc : m_orcamentos)
	{
		Veiculo* v = orc->getVeiculo();
		if (cliente.buscarVeiculo(v->getPlaca()) == v)
			return true;
	}
	return false;
}

// RN16 (versao mais segura): cliente que ja tem orcamento ou OS nao pode ser removido.
bool Oficina::removerCliente(int id)
{
	for (auto it = m_clientes.begin(); it != m_clientes.end(); ++it)
	{
		if ((*it)->getId() == id)
		{
			if (clienteTemOrcamentos(**it))
				throw OperacaoInvalida("Este cliente tem orcamentos ou ordens de servico registrados e nao pode ser removido");
			m_clientes.erase(it);
			return true;
		}
	}
	return false;
}

void Oficina::listarClientes() const
{
	if (m_clientes.empty())
	{
		std::cout << "Nenhum cliente cadastrado\n";
		return;
	}
	for (const auto& c : m_clientes)
		c->exibir();   // polimorfismo: Cliente::exibir()
}

// ======================= Veiculos =======================

// RN02: a placa e unica em todo o sistema (nao so dentro de um cliente).
void Oficina::cadastrarVeiculo(int idCliente, std::unique_ptr<Veiculo> v)
{
	Cliente& cliente = obterCliente(idCliente);
	if (!v)
		throw ErroDeValidacao("O veiculo nao pode ser nulo");
	if (buscarVeiculoPorPlaca(v->getPlaca()) != nullptr)
		throw ErroDeValidacao("Ja existe um veiculo cadastrado com a placa " + v->getPlaca());
	cliente.adicionarVeiculo(std::move(v));
}

Veiculo* Oficina::buscarVeiculoPorPlaca(const std::string& placa) const
{
	for (const auto& c : m_clientes)
	{
		Veiculo* v = c->buscarVeiculo(placa);
		if (v != nullptr)
			return v;
	}
	return nullptr;
}

// ======================= Mecanicos =======================

Mecanico& Oficina::cadastrarMecanico(const std::string& nome, const std::string& cpf, const std::string& tel,
	const std::string& especialidade, double valorHora)
{
	for (const auto& m : m_mecanicos)
	{
		if (m->getCPFCNPJ() == cpf)
			throw ErroDeValidacao("Ja existe um mecanico com este CPF");
	}
	auto mecanico = std::make_unique<Mecanico>(m_proximoIdPessoa, nome, cpf, tel, especialidade, valorHora);
	m_proximoIdPessoa++;
	m_mecanicos.push_back(std::move(mecanico));
	return *m_mecanicos.back();
}

Mecanico& Oficina::obterMecanico(int id) const
{
	for (const auto& m : m_mecanicos)
	{
		if (m->getId() == id)
			return *m;
	}
	throw NaoEncontrado("Mecanico nao encontrado: id " + std::to_string(id));
}

void Oficina::listarMecanicos() const
{
	if (m_mecanicos.empty())
	{
		std::cout << "Nenhum mecanico cadastrado\n";
		return;
	}
	for (const auto& m : m_mecanicos)
		m->exibir();
}

// ======================= Estoque e compras =======================

Peca& Oficina::cadastrarPeca(const std::string& nome, double custo, double venda)
{
	int codigo = m_estoque.proximoCodigo();
	Peca* peca = m_estoque.adicionarPeca(std::make_unique<Peca>(codigo, nome, custo, venda));
	return *peca;
}

Peca& Oficina::obterPeca(int codigo) const
{
	return m_estoque.obterPeca(codigo);
}

// RN15: a compra repoe o estoque E lanca uma despesa no caixa.
Compra& Oficina::comprarPecas(int codigoPeca, int qtd, const std::string& fornecedor, double custoUnitario)
{
	Peca& peca = m_estoque.obterPeca(codigoPeca);   // NaoEncontrado se nao existir
	std::string data = dataHoje();

	// O construtor da Compra valida quantidade, custo e fornecedor.
	auto compra = std::make_unique<Compra>(m_proximoIdCompra, fornecedor, &peca, qtd, custoUnitario, data);
	compra->registrar();   // as pecas entram no estoque
	m_caixa.registrarDespesa(compra->custoTotal(),
		"Compra #" + std::to_string(m_proximoIdCompra) + " - " + fornecedor, data);

	m_proximoIdCompra++;
	m_compras.push_back(std::move(compra));
	return *m_compras.back();
}

void Oficina::listarEstoque() const
{
	m_estoque.listarPecas();
}

void Oficina::listarCompras() const
{
	if (m_compras.empty())
	{
		std::cout << "Nenhuma compra registrada\n";
		return;
	}
	for (const auto& c : m_compras)
	{
		c->exibir();
		std::cout << "\n";
	}
}

void Oficina::alertaEstoqueBaixo(int limite) const
{
	std::vector<const Peca*> baixas = m_estoque.listarEstoqueBaixo(limite);
	if (baixas.empty())
	{
		std::cout << "Nenhuma peca com estoque baixo (limite: " << limite << " unidades)\n";
		return;
	}
	std::cout << "\n--- ESTOQUE BAIXO (" << limite << " unidades ou menos) ---\n";
	for (const Peca* p : baixas)
		std::cout << p->getNome() << " (codigo " << p->getCodigo() << "): " << p->getQtdEstoque() << " un.\n";
}

// ======================= Orcamentos =======================

Orcamento& Oficina::criarOrcamento(const std::string& placa, int idMecanico)
{
	Veiculo* veiculo = buscarVeiculoPorPlaca(placa);
	if (veiculo == nullptr)
		throw NaoEncontrado("Veiculo nao encontrado: placa " + placa);
	Mecanico& mecanico = obterMecanico(idMecanico);

	auto orcamento = std::make_unique<Orcamento>(m_proximoIdOrcamento, veiculo, &mecanico, dataHoje());
	m_proximoIdOrcamento++;
	m_orcamentos.push_back(std::move(orcamento));
	return *m_orcamentos.back();
}

Orcamento& Oficina::obterOrcamento(int id) const
{
	for (const auto& o : m_orcamentos)
	{
		if (o->getId() == id)
			return *o;
	}
	throw NaoEncontrado("Orcamento nao encontrado: id " + std::to_string(id));
}

void Oficina::aprovarOrcamento(int id)
{
	obterOrcamento(id).aprovar();
}

void Oficina::recusarOrcamento(int id)
{
	obterOrcamento(id).recusar();
}

void Oficina::listarOrcamentos() const
{
	if (m_orcamentos.empty())
	{
		std::cout << "Nenhum orcamento criado\n";
		return;
	}
	for (const auto& o : m_orcamentos)
	{
		std::cout << "Orcamento #" << o->getId() << " | placa " << o->getVeiculo()->getPlaca()
			<< " | " << paraTexto(o->getStatus())
			<< " | total " << Utils::formatarMoeda(o->calcularTotal()) << "\n";
	}
}

// ======================= Ordens de servico =======================

// RN12: so nasce de orcamento aprovado (a OrdemServico confere) e no maximo uma por orcamento.
OrdemServico& Oficina::abrirOS(int idOrcamento)
{
	Orcamento& orcamento = obterOrcamento(idOrcamento);
	for (const auto& os : m_ordens)
	{
		if (os->getOrcamento() == &orcamento)
			throw OperacaoInvalida("Este orcamento ja tem uma ordem de servico (OS #" + std::to_string(os->getId()) + ")");
	}

	auto os = std::make_unique<OrdemServico>(m_proximoIdOS, &orcamento, dataHoje());
	m_proximoIdOS++;
	m_ordens.push_back(std::move(os));
	return *m_ordens.back();
}

OrdemServico& Oficina::obterOS(int id) const
{
	for (const auto& os : m_ordens)
	{
		if (os->getId() == id)
			return *os;
	}
	throw NaoEncontrado("Ordem de servico nao encontrada: id " + std::to_string(id));
}

void Oficina::iniciarOS(int id)
{
	obterOS(id).iniciar();
}

// RN14: concluir da baixa nas pecas (feito pela OS) e lanca a receita no caixa.
void Oficina::concluirOS(int id)
{
	OrdemServico& os = obterOS(id);
	std::string data = dataHoje();
	double total = os.concluir(data);   // se der erro aqui, o caixa nao e alterado
	m_caixa.registrarReceita(total, "OS #" + std::to_string(id), data);
}

void Oficina::cancelarOS(int id)
{
	obterOS(id).cancelar();
}

void Oficina::listarOS() const
{
	if (m_ordens.empty())
	{
		std::cout << "Nenhuma ordem de servico aberta\n";
		return;
	}
	for (const auto& os : m_ordens)
		os->exibir();
}

// ======================= Caixa =======================

const Caixa& Oficina::getCaixa() const { return m_caixa; }

void Oficina::relatorioCaixa() const
{
	m_caixa.gerarRelatorio();
}
