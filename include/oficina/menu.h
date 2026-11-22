#ifndef MENU_H
#define MENU_H

#include <memory>

#include "oficina.h"
#include "veiculos/veiculo.h"

// A "tela" do programa: mostra as opcoes, le o que o usuario digita e entrega
// para a Oficina. O Menu NAO tem regras de negocio: quem decide e a Oficina.
class Menu
{
private:
	Oficina& m_oficina;   // referencia para a Oficina que esta sendo usada

	void menuClientes();
	void menuMecanicos();
	void menuEstoque();
	void menuOrcamentos();
	void menuOrdens();
	void menuCaixa();
	std::unique_ptr<Veiculo> lerNovoVeiculo();

public:
	explicit Menu(Oficina& oficina);
	void executar();
};

#endif // !MENU_H
