# UFSM00292 – Projeto de Sistemas Embarcados (Equipe APP)

**Aluno:** Gabriel Fronza Schuster  
**Professor:** Carlos Henrique Barriquello  
**Período:** 2025/2  

---

## Semana 1 – Configuração do Ambiente Zephyr RTOS

### Objetivo
Instalar e configurar o ambiente **Zephyr RTOS** e **Zephyr SDK**, testar o emulador **QEMU** com o exemplo `hello_world`, e comprovar o funcionamento do ambiente de desenvolvimento.

---

### Ambiente Utilizado
- **Sistema Operacional:** Windows 10 x64  
- **Zephyr SDK:** 0.17.4  
- **Python:** 3.10+  
- **QEMU:** 8.x  
- **Gerenciador:** `west`  
- **Editor:** VS Code  

---

### Passos Realizados

1. Instalação do **Zephyr SDK** e definição das variáveis de ambiente:
   ```bash
   setx ZEPHYR_TOOLCHAIN_VARIANT zephyr
   setx ZEPHYR_SDK_INSTALL_DIR "C:\Users\Gabe\zephyr-sdk-0.17.4"

2. Inicialização do workspace:
    west init -m https://github.com/ufsm-barriquello/zephyr
    west update
    west zephyr-export

3. Compilação e execução do exemplo hello_world:
    cd zephyr/samples/hello_world
    west build -b qemu_x86 --pristine
    qemu-system-x86_64 -machine accel=tcg -nographic -kernel build/zephyr/zephyr.elf

Resultado:
    *** Booting Zephyr OS build ***
    Hello World! qemu_x86
