cmake_minimum_required(VERSION 3.16)
project(oficina CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Avisos do compilador (a sintaxe muda entre o compilador da Microsoft e o g++/clang)
if(MSVC)
    add_compile_options(/W4 /utf-8)
else()
    add_compile_options(-Wall -Wextra)
endif()

# Todos os .cpp de src/, menos o main.cpp, formam a "biblioteca" do projeto.
# CONFIGURE_DEPENDS faz o CMake perceber arquivos novos sozinho.
file(GLOB_RECURSE FONTES CONFIGURE_DEPENDS src/*.cpp)
list(FILTER FONTES EXCLUDE REGEX "/main\\.cpp$")

if(FONTES)
    add_library(oficina_lib STATIC ${FONTES})
    target_include_directories(oficina_lib PUBLIC include)
else()
    # Ainda não há classes com .cpp: a biblioteca só leva a pasta include.
    add_library(oficina_lib INTERFACE)
    target_include_directories(oficina_lib INTERFACE include)
endif()

# Programa principal (criado quando existir src/main.cpp)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
    add_executable(oficina src/main.cpp)
    target_link_libraries(oficina PRIVATE oficina_lib)
endif()

# Cada tests/teste_*.cpp vira um programa de teste próprio (com o seu main)
file(GLOB TESTES CONFIGURE_DEPENDS tests/teste_*.cpp)
foreach(arquivo ${TESTES})
    get_filename_component(nome ${arquivo} NAME_WE)
    add_executable(${nome} ${arquivo})
    target_link_libraries(${nome} PRIVATE oficina_lib)
endforeach()
