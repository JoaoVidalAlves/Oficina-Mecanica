#ifndef ESTOQUE_H
#define ESTOQUE_H

#include <memory>
#include <vector>

#include "estoque/peca.h"

class Estoque
{
private:
	std::vector<std::unique_ptr<Peca>> m_pecas;

public:

	Peca* adicionarPeca(std::unique_ptr<Peca> p);
	int proximoCodigo() const;
	Peca* buscarPorCodigo(int codigo) const;
	Peca& obterPeca(int codigo) const;
	bool verificarDisponibilidade(int codigo, int qtd) const;
	void darBaixa(int codigo, int qtd);
	void repor(int codigo, int qtd);
	bool removerPeca(int codigo);
	std::vector<const Peca*> listarEstoqueBaixo(int limite = 3) const;
	void listarPecas() const;
	const std::vector<std::unique_ptr<Peca>>& getPecas() const;

};

#endif // ESTOQUE_H