#include "tester_framework.h"

static int s_testes_executados = 0;

void framework_inicia_contagem() {
    s_testes_executados = 0;
}

void framework_incrementa_contagem() {
    s_testes_executados++;
}

int framework_get_contagem() {
    return s_testes_executados;
}
