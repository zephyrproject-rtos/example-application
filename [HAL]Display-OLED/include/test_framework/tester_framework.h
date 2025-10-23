#ifndef TESTER_FRAMEWORK_H
#define TESTER_FRAMEWORK_H

void framework_inicia_contagem();
void framework_incrementa_contagem();
int framework_get_contagem();

#define verifica(mensagem, teste) do { \
    if (!(teste)) return mensagem; \
} while (0)

#define executa_teste(teste) do { \
    char *mensagem = teste(); \
    framework_incrementa_contagem(); \
    if (mensagem) return mensagem; \
} while (0)

#endif // TESTER_FRAMEWORK_H