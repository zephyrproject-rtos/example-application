# Entrega 1 - Instalação e Validação do Ambiente Zephyr + Renode

## 1. Objetivo
O objetivo desta fase inicial foi a criação e validação de um ambiente de desenvolvimento completo no **Windows** para o **Zephyr RTOS**, com foco na simulação de firmware para a placa **SAMR21**, utilizando o simulador de hardware **Renode**.

---

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

## 3. Status Atual
✅ Ambiente de desenvolvimento e simulação instalado, configurado e validado.  

➡️ **Próximos passos:** compilar uma aplicação customizada a partir do código-fonte utilizando `west build`, e simular o binário gerado no Renode.

---