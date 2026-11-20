#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

using std::string;

namespace Utils
{
	string lerLinha(const string& pergunta);
	int lerInt(const string& pergunta, int min);
	double lerDouble(const string& pergunta, double min, double max);
	bool lerSimNao(const string& pergunta);
	bool cpfValido(const string& cpf);
	bool placaValida(const string& placa);
	string dataHoje();
	string formatarMoeda(double v);
	// std::vector<string> dividir(const string& s, char sep); Sem uso até o momento
	int anoAtual();
	bool ehLetra(char c);
	bool ehDigito(char c);
	std::string paraMaiusculas(std::string texto);
	bool placaValida(const std::string& p);
	bool soDigitos(const string& texto);
}
#endif // !UTILS_H
