#include "comum\escecoes.h"

// Teste da Fase 0: confere se as quatro exceções podem ser lançadas e tratadas.
//
// Compilar e rodar (a partir da pasta oficina/):
//     g++ -std=c++17 -Wall -Wextra -Iinclude tests/teste_excecoes.cpp -o teste_excecoes
//     ./teste_excecoes

#include <iostream>
#include <string>

int falhas = 0;

// Mostra OK ou FALHOU para cada verificação.
void conferir(const std::string& nome, bool ok) {
    std::cout << (ok ? "[OK]     " : "[FALHOU] ") << nome << "\n";
    if (!ok) falhas++;
}

int main() {
    // 1) Cada exceção guarda a mensagem recebida e devolve em what().
    try {
        throw ErroDeValidacao("CPF deve ter 11 digitos");
    }
    catch (const ErroDeValidacao& e) {
        conferir("ErroDeValidacao guarda a mensagem",
            std::string(e.what()) == "CPF deve ter 11 digitos");
    }

    try {
        throw EstoqueInsuficiente("Estoque insuficiente");
    }
    catch (const EstoqueInsuficiente& e) {
        conferir("EstoqueInsuficiente guarda a mensagem",
            std::string(e.what()) == "Estoque insuficiente");
    }

    try {
        throw OperacaoInvalida("OS nao foi iniciada");
    }
    catch (const OperacaoInvalida& e) {
        conferir("OperacaoInvalida guarda a mensagem",
            std::string(e.what()) == "OS nao foi iniciada");
    }

    try {
        throw NaoEncontrado("Peca 999 nao existe");
    }
    catch (const NaoEncontrado& e) {
        conferir("NaoEncontrado guarda a mensagem",
            std::string(e.what()) == "Peca 999 nao existe");
    }

    // 2) O Menu poderá tratar qualquer uma delas com um único catch de
    //    std::exception (é isso que torna o try/catch do menu simples).
    int pegas = 0;  // conta quantas exceções o catch de std::exception pegou
    try { throw ErroDeValidacao("a"); }
    catch (const std::exception&) { pegas++; }
    try { throw EstoqueInsuficiente("b"); }
    catch (const std::exception&) { pegas++; }
    try { throw OperacaoInvalida("c"); }
    catch (const std::exception&) { pegas++; }
    try { throw NaoEncontrado("d"); }
    catch (const std::exception&) { pegas++; }
    conferir("Um catch de std::exception pega as quatro", pegas == 4);

    // 3) Um catch específico NÃO pega uma exceção de outro tipo.
    bool tipoCerto = false;
    try {
        try {
            throw EstoqueInsuficiente("x");
        }
        catch (const ErroDeValidacao&) {
            tipoCerto = false;  // não deve entrar aqui
        }
    }
    catch (const EstoqueInsuficiente&) {
        tipoCerto = true;
    }
    conferir("Catch especifico distingue os tipos", tipoCerto);

    std::cout << (falhas == 0 ? "\nTodos os testes passaram.\n"
        : "\nHa testes com falha.\n");
    return falhas == 0 ? 0 : 1;
}
