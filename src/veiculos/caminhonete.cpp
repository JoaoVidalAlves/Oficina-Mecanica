#include "veiculos/caminhonete.h"
#include "comum/escecoes.h"   // necessário para poder usar o throw

#include <cctype>
#include <ctime>
#include <iostream>

Caminhonete::Caminhonete(const std::string& placa, const std::string& modelo, int ano, int km, double capacidadeCargaKg)
    : Veiculo(placa, modelo, ano, km), _capacidadeDeCarga(capacidadeCargaKg)
{
    if (capacidadeCargaKg <= 0)
        throw ErroDeValidacao("Capacidade de carga nao pode ser menor ou igual a 0");
}

double Caminhonete::getCapacidadeCargaKg() const
{
    return _capacidadeDeCarga;
}

double Caminhonete::fatorMaoDeObra() const
{
    return 1.5f;
}
string Caminhonete::tipo() const
{
    return "Caminhonete";
}

void Caminhonete::exibir() const
{
    Veiculo::exibir();
    std::cout << "Capacidade de carga em Kg: " << Caminhonete::getCapacidadeCargaKg() << "\n";
}
