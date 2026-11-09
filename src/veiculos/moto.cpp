#include "veiculos/moto.h"
#include "comum/escecoes.h"   // necessário para poder usar o throw

#include <cctype>
#include <ctime>
#include <iostream>

Moto::Moto(const std::string& placa, const std::string& modelo, int ano, int km, int cilindradas)
    : Veiculo(placa, modelo, ano, km), _cilindradas(cilindradas)
{
    if (cilindradas < 50)
        throw ErroDeValidacao("Cilindradas precisa ser maior ou igual a 50");
}

int Moto::getCilindradas() const
{
    return _cilindradas;
}

double Moto::fatorMaoDeObra() const
{
    return 0.8f;
}

string Moto::tipo() const
{
    return "Moto";
}

void Moto::exibir() const
{
    Veiculo::exibir();
    std::cout << "Cilindradas: " << Moto::getCilindradas() << "\n";
}


