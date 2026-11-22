#ifndef TIPOS_H
#define TIPOS_H

#include <string>
#include "comum/excecoes.h"

enum class StatusOrcamento { Pendente, Aprovado, Recusado };

inline std::string paraTexto(StatusOrcamento s)
{
	switch (s)
	{
	case StatusOrcamento::Pendente:  return "Pendente";
	case StatusOrcamento::Aprovado:  return "Aprovado";
	case StatusOrcamento::Recusado:  return "Recusado";
	}
	return "Desconhecido";
}

inline StatusOrcamento deTextoStatusOrcamento(const std::string& texto)
{
	if (texto == "Pendente") return StatusOrcamento::Pendente;
	if (texto == "Aprovado") return StatusOrcamento::Aprovado;
	if (texto == "Recusado") return StatusOrcamento::Recusado;
	throw ErroDeValidacao("Status de orcamento invalido: " + texto);
}

enum class StatusOS { Aberta, EmAndamento, Concluida, Cancelada };

inline std::string paraTexto(StatusOS s)
{
	switch (s)
	{
	case StatusOS::Aberta:  return "Aberta";
	case StatusOS::EmAndamento:  return "EmAndamento";
	case StatusOS::Concluida:  return "Concluida";
	case StatusOS::Cancelada:  return "Cancelada";
	}
	return "Desconhecido";
}

inline StatusOS deTextoStatusOS(const std::string& texto)
{
	if (texto == "Aberta") return StatusOS::Aberta;
	if (texto == "EmAndamento") return StatusOS::EmAndamento;
	if (texto == "Concluida") return StatusOS::Concluida;
	if (texto == "Cancelada") return StatusOS::Cancelada;
	throw ErroDeValidacao("Status de OS invalido: " + texto);
}

enum class TipoLancamento {	Receita, Despesa };

inline std::string paraTexto(TipoLancamento s)
{
	switch (s)
	{
	case TipoLancamento::Receita: return "Receita";
	case TipoLancamento::Despesa: return "Despesa";
	}
	return "Desconhecido";
}

inline TipoLancamento deTextoTipoLancamento(const std::string& texto)
{
	if (texto == "Receita") return TipoLancamento::Receita;
	if (texto == "Despesa") return TipoLancamento::Despesa;
	throw ErroDeValidacao("Status de OS invalido: " + texto);
}

#endif // !TIPOS_H