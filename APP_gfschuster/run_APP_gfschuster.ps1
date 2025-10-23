# ============================================================
# Script: run_APP_local.ps1
# Autor: Gabriel Fronza Schuster
# Descrição: Executa build e run do APP_gfschuster localmente.
# ============================================================

Write-Host "=== UFSM00292 - Execucao da aplicacao APP_gfschuster ===" -ForegroundColor Cyan

# --- Caminho do Zephyr ---
$ZEPHYR_BASE = "C:\zephyrproject\zephyr"

# --- Verifica se o Zephyr existe ---
if (!(Test-Path $ZEPHYR_BASE)) {
    Write-Host "ERRO: Caminho do Zephyr não encontrado: $ZEPHYR_BASE" -ForegroundColor Red
    exit
}

# --- Exporta o Zephyr ---
Write-Host ""
Write-Host "Exportando ambiente Zephyr..." -ForegroundColor Yellow
$env:ZEPHYR_BASE = $ZEPHYR_BASE
Push-Location "C:\zephyrproject"
west zephyr-export | Out-Null
Pop-Location

# --- Compila o projeto atual ---
Write-Host ""
Write-Host "Compilando APP_gfschuster..." -ForegroundColor Yellow
if (!(Test-Path "build")) {
    New-Item -ItemType Directory -Path "build" | Out-Null
}
west build -b qemu_x86 --pristine

if ($LASTEXITCODE -ne 0) {
    Write-Host ""
    Write-Host "ERRO: Falha na compilação!" -ForegroundColor Red
    exit
}

# --- Executa o QEMU ---
Write-Host ""
Write-Host "Executando no QEMU..." -ForegroundColor Yellow
qemu-system-x86_64 -machine accel=tcg -nographic -kernel build/zephyr/zephyr.elf

Write-Host ""
Write-Host "Execução concluída. Para sair do QEMU: pressione Ctrl + A e depois X." -ForegroundColor Green
