# Define a versão mínima do CMake e encontra o Zephyr no seu sistema
cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

# Dá um nome ao seu projeto
project(temp_sensor_app)

# Informa ao compilador que ele também deve procurar por arquivos .h na pasta 'include'
target_include_directories(app PRIVATE include)

# Lista todos os arquivos .c que fazem parte da sua aplicação final.
# O sistema de build do Zephyr é inteligente o suficiente para saber que,
# ao construir para testes, ele não deve incluir o main.c.
target_sources(app PRIVATE
	src/main.c
	src/driver_temp.c
)