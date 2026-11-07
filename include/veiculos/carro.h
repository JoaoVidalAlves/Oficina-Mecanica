#ifndef CARRO_H
#define CARRO_H

#include "veiculo.h"

class Carro : public Veiculo
{
private:
	int _portas;

public:
	Carro(const string& placa, const string& modelo, int ano, int km, int portas);
	int getPortas() const;
	double fatorMaoDeObra() const override;
	string tipo() const override;
	void exibir() const override;
};
#endif // !CARRO_H
