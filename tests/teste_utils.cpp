#include <iostream>
#include <string>
#include <ctime>

#include "comum/utils.h"

using namespace std;

// CONTROLE DOS TESTES
int totalPositivos = 0;
int totalNegativos = 0;
int aprovados = 0;
int reprovados = 0;

// TESTES POSITIVOS
void testarPositivo(const string& nome, bool resultado)
{
    totalPositivos++;

    if (resultado)
    {
        aprovados++;
        cout << "[APROVADO] " << nome << "\n";
    }
    else
    {
        reprovados++;
        cout << "[FALHOU] " << nome << "\n";
    }
}

// TESTES NEGATIVOS
void testarNegativo(const string& nome, bool resultado)
{
    totalNegativos++;

    if (!resultado)
    {
        aprovados++;
        cout << "[REPROVADO - ENTRADA REJEITADA] " << nome << "\n";
    }
    else
    {
        reprovados++;
        cout << "[FALHA - ENTRADA ACEITA] " << nome << "\n";
    }
}

void testarUtils()
{
    cout << "\n========== TESTES UTILS ==========\n";

    // =====================================
    // EHLETRA
    // =====================================

    cout << "\n--- ehLetra ---\n";

    testarPositivo("Letra maiuscula", Utils::ehLetra('A'));
    testarPositivo("Letra minuscula", Utils::ehLetra('z'));

    testarNegativo("Numero", Utils::ehLetra('1'));
    testarNegativo("Simbolo", Utils::ehLetra('@'));
    testarNegativo("Espaco", Utils::ehLetra(' '));

    // =====================================
    // EHDIGITO
    // =====================================

    cout << "\n--- ehDigito ---\n";

    testarPositivo("Numero 1", Utils::ehDigito('1'));
    testarPositivo("Numero 0", Utils::ehDigito('0'));

    testarNegativo("Letra", Utils::ehDigito('A'));
    testarNegativo("Simbolo", Utils::ehDigito('#'));

    // =====================================
    // SODIGITOS
    // =====================================

    cout << "\n--- soDigitos ---\n";

    testarPositivo("Somente numeros", Utils::soDigitos("123456"));
    testarPositivo("Numero com zero", Utils::soDigitos("001234"));

    testarNegativo("Numeros com letras", Utils::soDigitos("123ABC"));
    testarNegativo("Somente letras", Utils::soDigitos("ABC"));
    testarNegativo("Caracteres especiais", Utils::soDigitos("123@45"));

    // =====================================
    // PARAMAIUSCULAS
    // =====================================

    cout << "\n--- paraMaiusculas ---\n";

    testarPositivo("Texto minusculo",
        Utils::paraMaiusculas("oficina") == "OFICINA");

    testarPositivo("Texto misto",
        Utils::paraMaiusculas("OfIcInA") == "OFICINA");

    testarPositivo("Texto com numeros",
        Utils::paraMaiusculas("abc123") == "ABC123");

    // =====================================
    // PLACAVALIDA
    // =====================================

    cout << "\n--- placaValida ---\n";

    testarPositivo("Placa antiga",
        Utils::placaValida("ABC1234"));

    testarPositivo("Placa Mercosul",
        Utils::placaValida("ABC1D23"));

    testarPositivo("Placa minuscula",
        Utils::placaValida("abc1234"));

    testarNegativo("Tamanho incorreto",
        Utils::placaValida("ABC123"));

    testarNegativo("Primeira posicao numerica",
        Utils::placaValida("1BC1234"));

    testarNegativo("Quarta posicao letra",
        Utils::placaValida("ABCA234"));

    testarNegativo("Ultima posicao letra",
        Utils::placaValida("ABC123A"));

    testarNegativo("Caracteres especiais",
        Utils::placaValida("ABC-234"));

    // =====================================
    // CPFVALDIO
    // true = documento invalido
    // false = formato aceito
    // =====================================

    cout << "\n--- cpfValido ---\n";

    testarPositivo("CPF com 11 digitos",
        !Utils::cpfValido("12345678901"));

    testarPositivo("CNPJ com 14 digitos",
        !Utils::cpfValido("12345678000199"));

    testarNegativo("Tamanho incorreto",
        !Utils::cpfValido("123456"));

    testarNegativo("Letras",
        !Utils::cpfValido("1234567890A"));

    testarNegativo("Caracteres especiais",
        !Utils::cpfValido("123.456.789"));

    // =====================================
    // DATAHOJE
    // =====================================

    cout << "\n--- dataHoje ---\n";

    string data = Utils::dataHoje();

    testarPositivo("Tamanho correto",
        data.size() == 10);

    testarPositivo("Separadores corretos",
        data[2] == '/' && data[5] == '/');

    // =====================================
    // ANOATUAL
    // =====================================

    cout << "\n--- anoAtual ---\n";

    time_t agora = time(nullptr);
    tm* dataAtual = localtime(&agora);

    int anoEsperado = dataAtual->tm_year + 1900;

    testarPositivo("Ano atual correto",
        Utils::anoAtual() == anoEsperado);

    // =====================================
    // FORMATARMOEDA
    // =====================================

    cout << "\n--- formatarMoeda ---\n";

    testarPositivo("Valor inteiro",
        Utils::formatarMoeda(100) == "R$ 100.00");

    testarPositivo("Valor decimal",
        Utils::formatarMoeda(25.50) == "R$ 25.50");

    testarPositivo("Valor zero",
        Utils::formatarMoeda(0) == "R$ 0.00");

    testarPositivo("Arredondamento",
        Utils::formatarMoeda(10.999) == "R$ 11.00");

    // =====================================
    // RESULTADO FINAL
    // =====================================

    cout << "\n========== RESULTADO FINAL ==========\n";

    cout << "Testes positivos: " << totalPositivos << "\n";
    cout << "Testes negativos: " << totalNegativos << "\n";

    cout << "Testes aprovados: " << aprovados << "\n";
    cout << "Testes reprovados: " << reprovados << "\n";

    cout << "Total de testes: "
        << totalPositivos + totalNegativos << "\n";

    cout << "=====================================\n";
}

int main()
{
    testarUtils();

    return 0;
}