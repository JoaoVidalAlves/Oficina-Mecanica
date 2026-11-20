#include "comum/utils.h"

int Utils::anoAtual()
{
    std::time_t agora = std::time(nullptr);
    std::tm* data = std::localtime(&agora);
    return data->tm_year + 1900;
}

bool Utils::ehLetra(char c)
{
    return std::isalpha(static_cast<unsigned char>(c)) != 0;
}

bool Utils::ehDigito(char c)
{
    return std::isdigit(static_cast<unsigned char>(c)) != 0;
}

std::string Utils::paraMaiusculas(std::string texto)
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
bool Utils::placaValida(const std::string& p)
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

bool Utils::soDigitos(const string& texto)
{
    for (char c : texto)
    {
        if (!std::isdigit(static_cast<unsigned char>(c)))
            return false;
    }
    return true;
}