# Entrega 1 - Instalação e Validação do Ambiente Zephyr + Renode

## 1. Objetivo
O objetivo desta fase inicial foi a criação e validação de um ambiente de desenvolvimento completo no **Windows** para o **Zephyr RTOS**, com foco na simulação de firmware para a placa **SAMR21**, utilizando o simulador de hardware **Renode**.

## 2. Etapas Concluídas

### a. Instalação do Ambiente de Desenvolvimento
Foram instaladas e configuradas todas as ferramentas necessárias para o desenvolvimento com Zephyr:

- **Ferramentas de Host**: CMake, Python, Git, Ninja, etc. (gerenciadas via Chocolatey)  
- **Workspace Zephyr**: inicializado com a ferramenta `west`, incluindo o download do código-fonte do Zephyr e de seus módulos  
- **Zephyr SDK**: instalação da toolchain ARM (compilador, bibliotecas, etc.) integrada ao ambiente  

### b. Instalação do Ambiente de Simulação
- Instalação e configuração do **Renode**, garantindo pleno funcionamento do simulador localmente.

### c. Validação do Ambiente com Exemplo Pré-compilado
- Obtenção de artefatos de um exemplo **blinky** pré-compilado (Zephyr/Antmicro).  
- Arquivos utilizados:
  - `blinky.elf`  
  - Scripts de configuração do Renode (`.resc` e `.repl`)  
- A simulação foi executada com sucesso no Renode, validando a instalação e confirmando que o ambiente é capaz de carregar e rodar firmware para a placa **samr21_xpro**.

---

# Entrega 2 - Compilação de uma aplicação Zephyr e simulação com o Renode / Pesquisa sobre os periféricos da SAMR21 já portados para o Zephyr

- Durante esta semana foi compilada uma aplicação Zephyr que consiste em um simples contador utilizando a função printk, a aplicação foi simulada com sucesso com o Renode.

- Além disso, foram iniciadas pesquisas sobre os periféricos da placa de desenvolvimento SAMR21 para análise de possíveis aplicações Zephyr utilizando os seus drivers já portados para o RTOS.

---


# Entrega 3 - Escolha de projeto -Comunicação entre duas SAMR21 com OpenThread-

- Durante esta semana foi escolhida como parte do meu desenvolvimento para a aplicação zephyr a comunicação com openthread entre duas placas utilizando as antenas padrão IEEE802154.

➡️ **Próximos passos:** Compilar o exemplo coap openthread do zephyr para a placa SAMR21.



