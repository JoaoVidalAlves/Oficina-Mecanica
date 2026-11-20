// Em compiladores da Microsoft, evita um aviso sobre std::localtime.
// Em outros compiladores, esta linha não faz nada.
#define _CRT_SECURE_NO_WARNINGS

#include "veiculos/veiculo.h"
#include "comum/excecoes.h"   // necessário para poder usar o throw
#include "comum/utils.h"

#include <cctype>
#include <ctime>
#include <iostream>

Veiculo::Veiculo(const std::string& placa, const std::string& modelo, int ano, int km)
    : placa(Utils::paraMaiusculas(placa)), modelo(modelo), ano(ano), km(km) 
{

    // Dentro do construtor, "placa", "modelo", "ano" e "km" (sem this->)
    // são os PARAMETROS recebidos. Os atributos da classe já foram
    // preenchidos pela lista acima, por isso a placa já está em maiúsculas.

    if (!Utils::placaValida(this->placa))
    {
        throw ErroDeValidacao("Placa invalida: use o formato AAA1234 ou AAA1A23");
    }

    if (modelo.empty()) 
    {
        throw ErroDeValidacao("O modelo nao pode ser vazio");
    }

    int anoMaximo = Utils::anoAtual() + 1;
    if (ano < 1950 || ano > anoMaximo) 
    {
        throw ErroDeValidacao("Ano invalido: deve estar entre 1950 e " +
            std::to_string(anoMaximo));
    }

    if (km < 0) 
    {
        throw ErroDeValidacao("A quilometragem nao pode ser negativa");
    }
}

std::string Veiculo::getPlaca() const 
{ 
    return placa; 
}

std::string Veiculo::getModelo() const 
{ 
    return modelo; 
}

int Veiculo::getAno() const 
{ 
    return ano;
}

int Veiculo::getKm() const 
{ 
    return km;
}

void Veiculo::atualizarKm(int novoKm) 
{
    if (novoKm < km) 
    {
        throw ErroDeValidacao("A nova quilometragem nao pode ser menor que a atual");
    }
    km = novoKm;
}

void Veiculo::exibir() const 
{
    std::cout << tipo() << " | Placa: " << placa << " | Modelo: " << modelo
        << " | Ano: " << ano << " | Km: " << km << "\n";
}