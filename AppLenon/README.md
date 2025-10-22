
# Monitoramento de temperatura

Este projeto consiste no desenvolvimento de um firmware para a placa SAM R21 Xplained Pro, utilizando o Zephyr RTOS, com o objetivo de ler a temperatura de um sensor AT30TSE752A presente na placa de expansão I/O1 Xplained Pro.

O grande diferencial deste projeto é a adoção da metodologia TDD (Test-Driven Development), garantindo que a lógica de negócio seja robusta e verificável de forma independente do hardware, utilizando o framework de testes Ztest nativo do Zephyr.
## Principais Tecnologias e Metodologias

-   Microcontrolador: ATSAMR21G18A

-   Placa de Desenvolvimento: Microchip SAM R21 Xplained Pro

-   Sensor: Atmel AT30TSE752A (via I²C) na placa I/O1 Xplained Pro

-   Sistema Operacional: Zephyr RTOS

-   Metodologia: Test-Driven Development (TDD)

Framework de Teste: Ztest

### 🛠️ Hardware Necessário
-  Placa SAM R21 Xplained Pro

-   Placa de expansão I/O1 Xplained Pro

-   Cabo Micro-USB

### ⚙️ Software e Toolchain

Para compilar e testar este projeto, você precisará ter o ambiente de desenvolvimento do Zephyr completamente configurado.

-   Zephyr SDK (incluindo west, toolchain, etc.)
## Roadmap

Este projeto foi estruturado em fases claras para garantir um desenvolvimento progressivo e testável.

#### Fase 1: Fundação e Teste da Lógica (Desenvolvimento no Host)
O objetivo desta fase é construir e validar toda a lógica de software no computador local, sem depender do hardware físico.

- [x] Definição da arquitetura e estrutura de pastas do projeto.

- [x] Criação dos arquivos de configuração do Zephyr (CMakeLists.txt, prj.conf).

- [x] Definição da interface do driver do sensor (driver_temp_at30tse.h).

- [x] Desenvolvimento TDD da lógica de conversão de dados brutos para graus Celsius.

- [x] Execução e validação dos testes de lógica no host com o alvo native_posix.

#### Fase 2: Integração com o Hardware
Com a lógica validada, o próximo passo é integrar o software com o hardware real e garantir a comunicação.

- [x] Sanity Check: Compilar e gravar o exemplo "Blinky" na SAM R21 Xplained Pro para validar o toolchain e a conexão com a placa.

- [x] Validar e ajustar o Device Tree Overlay (boards/samr21_xpro.overlay) para a correta configuração do barramento I²C.

- [x] Implementar as chamadas de baixo nível da API I²C do Zephyr no driver do sensor.

- [ ] Realizar testes de integração para ler os dados brutos diretamente do sensor.

- [ ] Validar a leitura completa da temperatura em Celsius utilizando o hardware.

#### Fase 3: Funcionalidades da Aplicação
Com o driver funcionando, esta fase foca em construir a aplicação final.

- [ ] Criar um loop principal (main.c) para realizar leituras de temperatura periódicas.

- [ ] Utilizar o sistema de logs do Zephyr para exibir as temperaturas lidas no console.

- [ ] (Opcional) Implementar um shell customizado para solicitar leituras de temperatura sob demanda.

- [ ] (Opcional) Enviar os dados de temperatura via interface UART.