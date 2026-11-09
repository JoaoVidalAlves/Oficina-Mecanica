#include "veiculos/carro.h"
#include "comum/escecoes.h"   // necessário para poder usar o throw

#include <cctype>
#include <ctime>
#include <iostream>

Carro::Carro(const std::string& placa, const std::string& modelo, int ano, int km, int portas)
    : Veiculo(placa, modelo, ano, km), _portas(portas)
{
    if (portas < 2 || portas > 5)
        throw ErroDeValidacao("Minimo de portas: 2 - Maximo de portas: 5");
}

int Carro::getPortas() const
{
    return _portas;
}

double Carro::fatorMaoDeObra() const
{
    return 1.0f;
}

string Carro::tipo() const
{
    return "Carro";
}

void Carro::exibir() const
{
    Veiculo::exibir();
    std::cout << "Portas: " << Carro::getPortas() << "\n";
}
