#include <cctype>
#include <ctime>

#include "comum/utils.h"

namespace Utils 
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

    bool soDigitos(const std::string& texto)
    {
        for (char c : texto)
        {
            if (!std::isdigit(static_cast<unsigned char>(c)))
                return false;
        }
        return true;
    }
}