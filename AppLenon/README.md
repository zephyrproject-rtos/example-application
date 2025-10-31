# Sistema de Monitoramento e Comunicação de Sensores

Este projeto, desenvolvido em equipe, consiste na criação de um sistema de monitoramento completo. O firmware principal, baseado no Zephyr RTOS para a placa SAM R21 Xplained Pro, é responsável por ler dados de dois sensores distintos.

Após a coleta e processamento, os dados dos sensores são transmitidos para uma segunda placa receptora. Esta placa, por sua vez, é responsável por receber os sinais e exibi-los em uma interface gráfica para o usuário.

A metodologia TDD (Test-Driven Development) é mantida como pilar para o desenvolvimento dos drivers e da lógica de negócio no transmissor, utilizando o framework Ztest para garantir a robustez e a verificabilidade do código.

## Principais Tecnologias e Metodologias

### Bloco Transmissor (SAM R21)
-   **Microcontrolador:** ATSAMR21G18A
-   **Placa de Desenvolvimento:** Microchip SAM R21 Xplained Pro
-   **Sistema Operacional:** Zephyr RTOS
-   **Metodologia:** Test-Driven Development (TDD)
-   **Framework de Teste:** Ztest
-   **Sensor 1 (Temperatura):** Atmel AT30TSE752A (via I²C) na placa I/O1 Xplained Pro
-   **Sensor 2:** **[Definir Modelo e Barramento do Sensor 2 (ex: LDR via ADC)]**

### Bloco Receptor e Comunicação
-   **Protocolo de Comunicação:** **[Definir Protocolo (ex: UART, BLE, LoRa)]**
-   **Placa Receptora:** **[Definir Placa (ex: ESP32, Raspberry Pi, outra SAM R21)]**
-   **Interface Gráfica (GUI):** **[Definir Tecnologia (ex: LVGL, Aplicação Web, Python/Tkinter)]**

### 🛠️ Hardware Necessário
-   Placa SAM R21 Xplained Pro (Transmissor)
-   Placa de expansão I/O1 Xplained Pro
-   **[Hardware do Sensor 2]**
-   **[Placa Receptora]**
-   Cabos Micro-USB e de conexão entre placas (conforme protocolo)

### ⚙️ Software e Toolchain
-   Zephyr SDK (para o transmissor)
-   **[Ambiente de Desenvolvimento da Placa Receptora e GUI]**

## Roadmap

O projeto foi reestruturado para acomodar a arquitetura de dois sensores e a comunicação entre placas.

#### Fase 1: Integração Sensor 1 (Temperatura)
Foco em validar o primeiro sensor e a base TDD.

- [x] Definição da arquitetura e estrutura de pastas.
- [x] Criação dos arquivos de configuração do Zephyr (CMakeLists.txt, prj.conf).
- [x] Definição da interface do driver do sensor (driver_temp_at30tse.h).
- [x] Desenvolvimento TDD da lógica de conversão de dados brutos para graus Celsius.
- [x] Execução e validação dos testes de lógica no host (native_posix).
- [x] Validação do Device Tree Overlay (boards/samr21_xpro.overlay) para o I²C.
- [x] Realizar testes de integração para ler os dados brutos diretamente do sensor AT30TSE752A.
- [x] Validar a leitura completa da temperatura em Celsius utilizando o hardware.

#### Fase 2: Integração Sensor 2
Incorporar o segundo sensor ao firmware do SAM R21.

- [x] Definição da interface do driver para o **[Sensor 2]**.
- [x] Desenvolvimento TDD da lógica de processamento/conversão dos dados do **[Sensor 2]**.
- [x] Ajustar o Device Tree (se necessário) para o barramento do **[Sensor 2]**.
- [ ] Implementar as chamadas de baixo nível da API Zephyr (ex: ADC, SPI, GPIO) para o driver.
- [ ] Realizar testes de integração para ler os dados brutos do **[Sensor 2]**.
- [ ] Integrar a leitura dos dois sensores no loop principal da aplicação.

#### Fase 3: Comunicação entre Placas
Estabelecer o link de dados entre o transmissor (SAM R21) e o receptor.

- [ ] Definir o formato da mensagem/payload para enviar os dados dos dois sensores.
- [ ] Configurar e implementar o driver de comunicação **[Protocolo]** no SAM R21.
- [ ] Implementar a lógica de recepção dos dados na **[Placa Receptora]**.
- [ ] Realizar testes de transmissão e recepção para validar a integridade dos dados.

#### Fase 4: Interface Gráfica (GUI)
Desenvolver a interface de usuário no bloco receptor.

- [ ] Definir o layout e os componentes da interface gráfica.
- [ ] Desenvolver a GUI utilizando **[Tecnologia da GUI]**.
- [ ] Integrar os dados recebidos (da Fase 3) para atualização em tempo real na GUI.
- [ ] Validar o sistema completo (Sensores -> SAM R21 -> Comunicação -> Receptor -> GUI).