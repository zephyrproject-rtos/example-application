# Week 1 — HAL bring-up (Zephyr) — HAL Team

## 📸 Evidências (anexe aqui os prints/imagens)
- [ ] **Hello World (QEMU)** — print com:
  - `*** Booting Zephyr OS build ...`
  - `Hello World! qemu_cortex_m3`
- [ ] **App multitarefa (duas threads)** — print com:
  - `[BLINK] toggle` (≥ 4 linhas)
  - `[HEART] alive N` (≥ 3 linhas)

## ▶️ Como reproduzir (Windows, PowerShell)
> Pré-requisitos: Python 3.12, west, Zephyr SDK 0.17.4, toolchain **arm-zephyr-eabi**, QEMU no PATH.

### Hello World (QEMU Cortex-M3)
```powershell
# venv ativa
& $Env:HOMEPATH\zephyrproject\.venv\Scripts\Activate.ps1

cd $Env:HOMEPATH\zephyrproject\zephyr
west zephyr-export
west build -p always -b qemu_cortex_m3 samples\hello_world
west build -t run
```

### App com 2 threads (QEMU)
```powershell
cd $Env:HOMEPATH\zephyrproject
west build -p always -b qemu_cortex_m3 app
west build -t run
```

### (Quando com a placa) SAMD21 Xplained Pro
```powershell
cd $Env:HOMEPATH\zephyrproject\zephyr
west boards | findstr /I samd21    # confirma o nome do board
west build -p always -b samd21_xpro samples\basic\blinky
west flash
# Depois:
cd $Env:HOMEPATH\zephyrproject
west build -p always -b samd21_xpro app
west flash
```

## ✅ Checklist Week 1
- [ ] Ambiente Windows + Zephyr SDK 0.17.4 instalado
- [ ] Toolchain **arm-zephyr-eabi** presente
- [ ] QEMU instalado e no PATH
- [ ] **Hello World** executa em `qemu_cortex_m3`
- [ ] **App com duas threads** executa em `qemu_cortex_m3`
- [ ] `README_HARDWARE.md` atualizado com passos de build/run
- [ ] PR aberto nesta semana (**WIP**) conforme regra da disciplina

## 📦 Estrutura deste PR
```
app/                         # app Zephyr com duas threads (blink + heartbeat)
drivers/
  imu_bno055/README.md       # skeleton HAL
  light_temp_io1/README.md   # skeleton HAL
docs/
  prints/
    week1/
      print_hello_qemu.png   # anexar
      print_app_threads.png  # anexar
      log_app_threads.txt    # opcional
README_HARDWARE.md
```

## 🔧 Ambiente
- Windows 10/11
- Zephyr: 4.2.x
- Zephyr SDK: 0.17.4
- west: 1.5.0
- Python: 3.12.10
- Board(s): `qemu_cortex_m3` (simulação) / `samd21_xpro` (HW)
