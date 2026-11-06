# Simulador de Oficina Mecânica

Projeto em C++ (POO): cadastro de clientes e veículos, orçamentos, compra de peças,
ordens de serviço e caixa. Trabalho em andamento.

## Estrutura

```
include/   cabeçalhos (.h), organizados por assunto (comum/, pessoas/, veiculos/ ...)
src/       implementações (.cpp) e o main.cpp
tests/     programas de teste (cada teste_*.cpp tem o seu próprio main)
```

## Como compilar e executar

### Opção 1 – VS Code (extensões "C/C++" e "CMake Tools")

1. Abra a pasta do projeto (**Arquivo > Abrir Pasta**).
2. Quando o VS Code perguntar qual compilador usar (kit), escolha o g++ ou o do Visual Studio.
3. Use os botões da barra inferior: **Build** para compilar e o ícone de **play** para executar.

### Opção 2 – Terminal (precisa de CMake e de um compilador C++)

```
cmake -S . -B build
cmake --build build
```

Os programas ficam na pasta `build/` (ou `build/Debug/` com o compilador da Microsoft):

```
./build/teste_excecoes
./build/oficina
```

### Opção 3 – Só com o g++ (sem CMake)

O teste das exceções:

```
g++ -std=c++17 -Wall -Wextra -Iinclude tests/teste_excecoes.cpp -o teste_excecoes
./teste_excecoes
```

O programa completo, quando houver arquivos em `src/`:

- Linux, Mac ou Git Bash: `g++ -std=c++17 -Iinclude $(find src -name '*.cpp') -o oficina`
- PowerShell: `g++ -std=c++17 -Iinclude (Get-ChildItem src -Recurse -Filter *.cpp).FullName -o oficina`

## Estado atual

- Concluído: exceções (`include/comum/Excecoes.h`), hierarquia de pessoas e de veículos.
- Pendente: estoque, orçamento, ordem de serviço, caixa, menu e gravação em arquivo.
