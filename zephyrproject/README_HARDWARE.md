# README_HARDWARE — Semana 1 (HAL)

Este repositório contém o **app multitarefa mínimo** (duas threads) para validar o ambiente de desenvolvimento do projeto final de **Sistemas Embarcados** usando **Zephyr RTOS**.

## 1) Pré-requisitos (Windows)
- Python 3.12 + pip
- Git, CMake (>=3.20), Ninja
- Zephyr SDK **0.17.4** (bundle minimal) + toolchain **arm-zephyr-eabi**
- `west` instalado na venv do projeto
- **QEMU** instalado e no PATH (para simulação)
- (Quando com a placa) **OpenOCD** no PATH para `west flash`

## 2) Preparar workspace (uma vez)
```powershell
# venv
& %USERPROFILE%\zephyrproject\.venv\Scripts\Activate.ps1

# inicializar/atualizar
west init %USERPROFILE%\zephyrproject
cd %USERPROFILE%\zephyrproject
west update
west zephyr-export
```

## 3) Simulação (QEMU — recomendado para a Semana 1)
### Hello World
```powershell
cd %USERPROFILE%\zephyrproject\zephyr
west build -p always -b qemu_cortex_m3 samples\hello_world
west build -t run
# Esperado:
# *** Booting Zephyr OS build ...
# Hello World! qemu_cortex_m3
```

### App com duas threads (este repositório)
```powershell
cd %USERPROFILE%\zephyrproject
west build -p always -b qemu_cortex_m3 app
west build -t run
# Esperado:
# *** Booting Zephyr OS build ...
# [BLINK] toggle
# [HEART] alive N
```

## 4) Execução na placa — SAMD21 Xplained Pro
> Requer OpenOCD no PATH e a placa conectada via USB (EDBG).

```powershell
cd %USERPROFILE%\zephyrproject\zephyr
west boards | findstr /I samd21    # confirma nome do board
west build -p always -b samd21_xpro samples\basic\blinky
west flash

# app deste repo
cd %USERPROFILE%\zephyrproject
west build -p always -b samd21_xpro app
west flash
# Console serial: 115200 8N1 (porta COM da placa)
```

## 5) Prints para o PR da Semana 1
- `docs/prints/week1/print_hello_qemu.png` — com as linhas:
  - `*** Booting Zephyr OS build ...`
  - `Hello World! qemu_cortex_m3`
- `docs/prints/week1/print_app_threads.png` — com:
  - `[BLINK] toggle` (≥4 linhas)
  - `[HEART] alive N` (≥3 linhas)
- (Opcional) `docs/prints/week1/log_app_threads.txt` — saída completa capturada com:
```powershell
# Exemplo (no diretório do app)
west build -t run | Tee-Object "%USERPROFILE%\zephyrproject\docs\prints\week1\log_app_threads.txt"
```

## 6) Estrutura recomendada do repo
```
app/
  CMakeLists.txt
  prj.conf
  src/main.c
drivers/
  imu_bno055/README.md
  light_temp_io1/README.md
docs/
  prints/week1/ (imagens aqui)
.github/
  pull_request_template.md
README_HARDWARE.md
```

## 7) Observações
- Para sair do QEMU no Windows: `Ctrl+C` no terminal; ou `Ctrl+A, C` → `quit`.
- Caso `west` não seja reconhecido, ative a venv novamente.
- Se o build reclamar de toolchain, instale com `west sdk install -t arm-zephyr-eabi`.
