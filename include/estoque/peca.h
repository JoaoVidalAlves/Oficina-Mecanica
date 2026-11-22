#ifndef PECA_H
#define PECA_H

#include <string>

class Peca
{
private:

	int m_codigo;
	std::string m_nome;
	double m_precoCusto;
	double m_precoVenda;
	int m_qtdEstoque;

public:

	Peca(int codigo, const std::string& nome, double precoCusto, double precoVenda, int qtdEstoque = 0);
	int getCodigo() const;
	std::string getNome() const;
	double getPrecoCusto() const;
	double getPrecoVenda() const;
	int getQtdEstoque() const;
	void setPrecoVenda(double v);
	double margemUnitaria() const;
	void darBaixa(int qtd);
	void repor(int qtd);
	bool estoqueBaixo(int limite = 3) const;
	void exibir() const;
};

#endif // !PECA_H


