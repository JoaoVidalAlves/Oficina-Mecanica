#ifndef EXCECOES_H
#define EXCECOES_H

// ---------------------------------------------------------------------------
// Excecoes.h
//
// Os "alarmes" do sistema. Quando algo dá errado (um CPF curto, uma peça que
// não existe, uma OS concluída fora de hora), a classe que percebe o problema
// "dispara o alarme" com throw, e o Menu "atende" com try/catch, mostra a
// mensagem na tela e continua funcionando.
//
// Todas herdam de std::runtime_error, que já guarda a mensagem de erro e
// oferece o método what() para lê-la. Por isso cada classe abaixo só precisa
// de um construtor que repassa o texto para a classe mãe.
//
// Como usar (exemplo):
//     if (qtd > qtdEstoque) {
//         throw EstoqueInsuficiente("Estoque insuficiente da peca " + nome);
//     }
//
// Como tratar (exemplo, no Menu):
//     try {
//         oficina.concluirOS(id);
//     } catch (const std::exception& e) {
//         std::cout << "Erro: " << e.what() << "\n";
//     }
//
// Arquivo só de cabeçalho: não existe Excecoes.cpp.
// ---------------------------------------------------------------------------

#include <stdexcept>
#include <string>

// Um dado digitado ou recebido não obedece às regras.
// Exemplos: CPF sem 11 dígitos, preço negativo, placa inválida, nome vazio.
class ErroDeValidacao : public std::runtime_error {
public:
    explicit ErroDeValidacao(const std::string& mensagem)
        : std::runtime_error(mensagem) {}
};

// Tentativa de usar mais peças do que existem no estoque.
// Exemplo: darBaixa(5) em uma peça que tem apenas 3 unidades.
class EstoqueInsuficiente : public std::runtime_error {
public:
    explicit EstoqueInsuficiente(const std::string& mensagem)
        : std::runtime_error(mensagem) {}
};

// A ação não é permitida no estado atual do objeto.
// Exemplos: concluir uma OS que não foi iniciada, alterar um orçamento que
// já foi aprovado, registrar a mesma compra duas vezes.
class OperacaoInvalida : public std::runtime_error {
public:
    explicit OperacaoInvalida(const std::string& mensagem)
        : std::runtime_error(mensagem) {}
};

// Um id, código ou placa que o sistema não conhece.
// Exemplos: obterPeca(999), obterCliente(42), orçamento inexistente.
class NaoEncontrado : public std::runtime_error {
public:
    explicit NaoEncontrado(const std::string& mensagem)
        : std::runtime_error(mensagem) {}
};

#endif

