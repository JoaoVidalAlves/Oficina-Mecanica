#ifndef CAMINHONETE_H
#define CAMINHONETE_H

#include "veiculo.h"

class Caminhonete : public Veiculo
{
private:
	double _capacidadeDeCarga;

public:
	Caminhonete(const string& placa, const string& modelo, int ano, int km, double capacidadeCargaKg);
	double getCapacidadeCargaKg() const;
	double fatorMaoDeObra() const override;
	string tipo() const override;
	void exibir() const override;
};
#endif // !CAMINHONETE_H
