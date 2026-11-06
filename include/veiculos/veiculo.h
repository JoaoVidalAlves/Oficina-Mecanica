#ifndef VEICULO_H
#define VEICULO_H

#include <string>

using std::string;

class Veiculo {
protected:
    std::string placa;
    std::string modelo;
    int ano;
    int km;

public:
    // Valida os dados. Se algum estiver errado, lança ErroDeValidacao
    // e o objeto NÃO chega a ser criado.
    Veiculo(const std::string& placa, const std::string& modelo, int ano, int km);
    virtual ~Veiculo() = default;

    std::string getPlaca() const;
    std::string getModelo() const;
    int getAno() const;
    int getKm() const;

    // A quilometragem só pode aumentar.
    void atualizarKm(int novoKm);

    // Cada tipo de veículo escreve a sua versão (= 0 obriga as filhas).
    virtual double fatorMaoDeObra() const = 0;
    virtual std::string tipo() const = 0;

    // Mostra os dados comuns. As filhas podem chamar Veiculo::exibir()
    // e acrescentar os dados próprios.
    virtual void exibir() const;
};

#endif // !VEICULO_H
