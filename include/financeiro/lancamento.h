#ifndef LANCAMENTO_H
#define LANCAMENTO_H

#include <string>

#include "comum/tipos.h"

struct Lancamento
{
	std::string data;
	TipoLancamento tipo;
	double valor;
	std::string descricao;
};

#endif // !LANCAMENTO_H

