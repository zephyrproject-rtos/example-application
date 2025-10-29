# Define a versão mínima do CMake e encontra o Zephyr no seu sistema
cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})


project(blinktest)

# Informa ao compilador que ele também deve procurar por arquivos .h na pasta 'include'
target_include_directories(app PRIVATE include)


target_sources(app PRIVATE
	src/main.c
)