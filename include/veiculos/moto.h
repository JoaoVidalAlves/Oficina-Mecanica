#ifndef MOTO_H
#define MOTO_H

#include "veiculo.h"

class Moto : public Veiculo
{
private:
	int _cilindradas;

public:
	Moto(const string& placa, const string& modelo, int ano, int km, int cilindradas);
	int getCilindradas() const;
	double fatorMaoDeObra() const override;
	string tipo() const override;
	void exibir() const override;
};
#endif // !MOTO_H
