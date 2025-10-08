#pragma once

#include <zephyr/kernel.h> // Para ter acesso a tipos padrão do Zephyr como uint16_t

/**
 * @brief Converte um valor bruto de temperatura para graus Celsius.
 *
 * Esta é a declaração da função. A implementação (o código que faz o trabalho)
 * está no arquivo .c correspondente.
 *
 * @param raw_temp O valor de 16 bits lido diretamente do sensor.
 * @return A temperatura correspondente em graus Celsius (float).
 */
float convert_raw_to_celsius(uint16_t raw_temp);

/*
 * NOTA FUTURA:
 * Quando você for adicionar mais funcionalidades ao seu driver,
 * como inicialização ou leitura completa, você irá declarar
 * as novas funções aqui primeiro. Por exemplo:
 *
 * int at30tse_init(const struct device *dev);
 * int at30tse_read_temp(const struct device *dev, float *temp_c);
 *
 */