#ifndef CAIXA_H
#define CAIXA_H

#include <string>
#include <vector>

#include "financeiro/lancamento.h"

class Caixa
{
private:
	double m_saldo =0.0;
	std::vector<Lancamento> m_lancamentos;

public:
	void registrarReceita(double valor, const std::string& desc, const std::string& data);
	void registrarDespesa(double valor, const std::string& desc, const std::string& data);
	double getSaldo() const;
	double totalReceitas() const;
	double totalDespesas() const;
	const std::vector<Lancamento>& getLancamentos() const;
	void gerarRelatorio() const;
};

#endif // !CAIXA_H
