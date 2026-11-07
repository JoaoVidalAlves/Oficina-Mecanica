// Em compiladores da Microsoft, evita um aviso sobre std::localtime.
// Em outros compiladores, esta linha não faz nada.
#define _CRT_SECURE_NO_WARNINGS

#include "veiculos/Veiculo.h"
#include "comum/Excecoes.h"   // necessário para poder usar o throw

#include <cctype>
#include <ctime>
#include <iostream>

// Funções auxiliares que só existem dentro deste arquivo.
namespace 
{
    int anoAtual() 
    {
        std::time_t agora = std::time(nullptr);
        std::tm* data = std::localtime(&agora);
        return data->tm_year + 1900;
    }

    bool ehLetra(char c) 
    { 
        return std::isalpha(static_cast<unsigned char>(c)) != 0;
    }

    bool ehDigito(char c) 
    { 
        return std::isdigit(static_cast<unsigned char>(c)) != 0;
    }

    std::string paraMaiusculas(std::string texto) 
    {
        for (char& c : texto) 
        {
            c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        }
        return texto;
    }

    // Aceita os dois formatos de placa:
    //   antigo:   AAA1234   (3 letras + 4 números)
    //   Mercosul: AAA1A23   (3 letras + número + letra + 2 números)
    bool placaValida(const std::string& p) 
    {
        if (p.size() != 7) return false;
        for (int i = 0; i < 3; i++) 
        {
            if (!ehLetra(p[i])) return false;
        }
        if (!ehDigito(p[3])) return false;
        if (!ehDigito(p[4]) && !ehLetra(p[4])) return false;
        return ehDigito(p[5]) && ehDigito(p[6]);
    }

}  // namespace

Veiculo::Veiculo(const std::string& placa, const std::string& modelo, int ano, int km)
    : placa(paraMaiusculas(placa)), modelo(modelo), ano(ano), km(km) 
{

    // Dentro do construtor, "placa", "modelo", "ano" e "km" (sem this->)
    // são os PARAMETROS recebidos. Os atributos da classe já foram
    // preenchidos pela lista acima, por isso a placa já está em maiúsculas.

    if (!placaValida(this->placa)) 
    {
        throw ErroDeValidacao("Placa invalida: use o formato AAA1234 ou AAA1A23");
    }

    if (modelo.empty()) 
    {
        throw ErroDeValidacao("O modelo nao pode ser vazio");
    }

    int anoMaximo = anoAtual() + 1;
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

void Veiculo::Exibir() const 
{
    std::cout << tipo() << " | Placa: " << placa << " | Modelo: " << modelo
        << " | Ano: " << ano << " | Km: " << km << "\n";
}