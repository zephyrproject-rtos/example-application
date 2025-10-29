#include <zephyr/ztest.h>
#include "driver_temp.h" // Inclui a interface do driver que vamos testar

/**
 * @brief Define um "conjunto de testes" (Test Suite).
 * Todos os testes relacionados à lógica do driver ficarão agrupados aqui.
 */
ZTEST_SUITE(temp_driver_logic_suite, NULL, NULL, NULL, NULL, NULL);

/**
 * @brief Testa a conversão de um valor bruto positivo para Celsius.
 *
 * Este é o nosso primeiro caso de teste.
 */
ZTEST(temp_driver_logic_suite, test_positive_temp_conversion)
{
	// Dados de entrada: um valor bruto que, segundo o datasheet, representa 25°C.
	uint16_t raw_value = 0x0190;

	// Valor esperado: o resultado que nossa função DEVE retornar.
	float expected_celsius = 25.0f;

	// Executa a função que está sendo testada.
	float actual_celsius = convert_raw_to_celsius(raw_value);

	// Verificação (Asserção): Compara se o valor real está "próximo o suficiente" do esperado.
	// Usamos zassert_within para comparar números de ponto flutuante (float).
	zassert_within(actual_celsius, expected_celsius, 0.001, "A conversão para 25°C falhou");
}

/**
 * @brief Testa a conversão de um valor bruto negativo para Celsius.
 *
 * Este é o nosso segundo caso de teste, para garantir que a lógica de
 * complemento de dois está funcionando.
 */
ZTEST(temp_driver_logic_suite, test_negative_temp_conversion)
{
	// Dados de entrada: um valor bruto que representa -10°C.
	uint16_t raw_value = 0x0F60;

	// Valor esperado.
	float expected_celsius = -10.0f;

	// Executa a função.
	float actual_celsius = convert_raw_to_celsius(raw_value);

	// Verificação (Asserção).
	zassert_within(actual_celsius, expected_celsius, 0.001, "A conversão para -10°C falhou");
}