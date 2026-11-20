#include <iostream>
#include <cctype>
#include <ctime>
#include <limits>
#include <sstream>
#include <iomanip>
#include <format>

#include "comum/utils.h"

namespace Utils 
{ 
    void limparConsole() 
    {
        #if defined(_WIN32) || defined(_WIN64)
        system("cls");
        #elif defined(__linux__)
        system("clear");
        #else
        std::cout << "\033[2J\033[1;1H";
        #endif
    }

    string lerLinha(const string& pergunta)
    {
        std::cout << pergunta << " ";
        std::string resposta;
        std::getline(std::cin, resposta);
        return resposta;
    }
    
    int lerInt(const string& pergunta, int min, int max)
    {
        int resposta;

        while (true)
        {
            std::cout << pergunta << " ";

            // Verifica se a entrada é um número inteiro válido E está dentro do intervalo
            if (std::cin >> resposta && resposta >= min && resposta <= max) 
            {
                return resposta;
            }

            // Se falhou, limpa o estado de erro do cin e descarta o lixo digitado
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            limparConsole();
            std::cout << "Entrada invalida! Valor minimo: " << min << " -- Valor maximo: " << max << "\n";
        }
    }

    int lerInt(const string& pergunta, int min)
    {
        int resposta;

        while (true)
        {
            std::cout << pergunta << " ";

            // Verifica se a entrada é um número inteiro válido E está dentro do intervalo
            if (std::cin >> resposta && resposta >= min)
            {
                return resposta;
            }

            // Se falhou, limpa o estado de erro do cin e descarta o lixo digitado
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            limparConsole();
            std::cout << "Entrada invalida! Valor minimo: " << min << "\n";
        }
    }

    double lerDouble(const string& pergunta, double min, double max)
    {
        double resposta;

        while (true)
        {
            std::cout << pergunta << " ";

            // Verifica se a entrada é um número inteiro válido E está dentro do intervalo
            if (std::cin >> resposta && resposta >= min && resposta <= max) {
                return resposta;
            }

            // Se falhou, limpa o estado de erro do cin e descarta o lixo digitado
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            limparConsole();
            std::cout << "Entrada invalida! Valor minimo: " << min << " -- Valor maximo: " << max << "\n";
        }
    }

    double lerDouble(const string& pergunta, double min)
    {
        double resposta;

        while (true)
        {
            std::cout << pergunta << " ";

            // Verifica se a entrada é um número inteiro válido E está dentro do intervalo
            if (std::cin >> resposta && resposta >= min) {
                return resposta;
            }

            // Se falhou, limpa o estado de erro do cin e descarta o lixo digitado
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            limparConsole();
            std::cout << "Entrada invalida! Valor minimo: " << min << "\n";
        }
    }

    bool lerSimNao(const string& pergunta)
    {
        int resposta = -1; // Inicializa com um valor inválido por segurança

        while (true)
        {
            std::cout << pergunta << " (1 - Sim / 0 - Nao): ";

            // Verifica primeiro se a leitura de um inteiro deu certo
            if (std::cin >> resposta) 
            {
                if (resposta == 1) return true;
                if (resposta == 0) return false;
            }

            // Se chegou aqui, ou digitou letra ou um número diferente de 0 e 1
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            limparConsole();
            std::cout << "Entrada invalida! Digite apenas 1 ou 0.\n\n";
        }
    }
   
    bool cpfValido(const string& cpfcnpj)
    {
        if ((cpfcnpj.size() != 11 && cpfcnpj.size() != 14) || !soDigitos(cpfcnpj))
            return true;
        else
            return false;
    }
   
    string dataHoje()
    {
        std::time_t agora = std::time(nullptr);
        std::tm* data = std::localtime(&agora);
        
        std::ostringstream resultado;

        resultado << std::setfill('0')
            << std::setw(2) << data->tm_mday << "/"
            << std::setw(2) << data->tm_mon + 1 << "/"
            << data->tm_year + 1900;

        return resultado.str();
    }
    
    string formatarMoeda(double v)
    {
        //return std::format("R$ {:.2f}", v);
        //return "R$ " + std::to_string(v);
        std::ostringstream resultado;

        resultado << "R$ "
            << std::fixed
            << std::setprecision(2)
            << v;

        return resultado.str();
    }
    
    // std::vector<string> dividir(const string& s, char sep); Sem uso até o momento

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