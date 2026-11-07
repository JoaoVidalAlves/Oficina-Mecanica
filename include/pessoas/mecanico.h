#ifndef MECANICO_H
#define MECANICO_H

#include <iostream>
#include <string>
#include "pessoas/pessoa.h"

using std::string;

class Mecanico : public Pessoa
{
private:
	string _especialidade;
	double _valorHora;

public:
	Mecanico(int id, const string& nome, const string& cpf, const string& telefone, const string& especialidade, double valorHora);

	string getEspecialidade() const;

	double getValorHora() const;

	void setValorHora(double v);

	void exibir() const override;

	~Mecanico() = default;
};
#endif