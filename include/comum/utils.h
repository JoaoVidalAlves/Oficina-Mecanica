#ifndef UTILS_H
#define UTILS_H

#include <string>

class Utils
{
	using std::string;

	int anoAtual();

	bool ehLetra(char c);

	bool ehDigito(char c);

	std::string paraMaiusculas(std::string texto);

	bool placaValida(const std::string& p);

	bool soDigitos(const string& texto);
};
#endif // !UTILS_H
