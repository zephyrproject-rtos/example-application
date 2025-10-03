Entrega 1: Instalação e validação do ambiente Zephyr + Renode

1. Objetivo
O objetivo desta fase inicial foi a criação e validação de um ambiente de desenvolvimento completo no Windows para o Zephyr RTOS, com foco na simulação de firmware para a placa SAMR21, utilizando o simulador de hardware Renode.

2. Etapas Concluídas
As seguintes etapas foram concluídas com sucesso:

a. Instalação do Ambiente de Desenvolvimento
Foram instaladas e configuradas todas as ferramentas de base necessárias para o desenvolvimento com Zephyr, incluindo:

Ferramentas de Host: CMake, Python, Git, Ninja, etc., gerenciadas via Chocolatey.

Workspace Zephyr: O ambiente foi inicializado com a ferramenta west, incluindo o download do código-fonte do Zephyr e de seus módulos.

Zephyr SDK: A toolchain (compilador ARM, bibliotecas, etc.) foi instalada e integrada ao ambiente.

b. Instalação do Ambiente de Simulação
O simulador de sistemas Renode foi instalado com sucesso na máquina local e está totalmente funcional.

c. Validação do Ambiente com Exemplo Pré-compilado
Para validar a integridade do ambiente de simulação, foram obtidos os artefatos de um exemplo blinky pré-compilado a partir de uma fonte oficial (Zephyr/Antmicro).

Os arquivos incluíam o executável (blinky.elf) e os scripts de configuração do Renode (.resc e .repl).

A simulação foi executada com sucesso no ambiente local, confirmando que a instalação do Renode é capaz de carregar e rodar firmware para a placa de destino (samr21_xpro) de forma correta.

3. Status Atual
O ambiente de desenvolvimento e simulação está instalado, configurado e validado. O próximo passo será a compilação de uma aplicação customizada a partir do código-fonte, utilizando a toolchain local do Zephyr (west build), para então simular o binário gerado.