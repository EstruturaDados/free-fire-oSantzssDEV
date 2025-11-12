#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_COMP 20
#define MAX_NOME 30
#define MAX_TIPO 20

typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int prioridade; // 1 (mais baixa) a 10 (mais alta)
} Componente;

// Função utilitária: remove newline e espaços nas extremidades
void trim(char *s) {
    int len = (int)strlen(s);
    while (len > 0 && (s[len-1] == '\n' || s[len-1] == '\r')) s[--len] = '\0';
    int i = 0;
    while (s[i] && (s[i] == ' ' || s[i] == '\t')) i++;
    if (i) memmove(s, s + i, strlen(s + i) + 1);
}

// Mostra lista de componentes formatada
void mostrarComponentes(Componente comps[], int n) {
    printf("\n--- Componentes (%d) ---\n", n);
    if (n == 0) { printf("Nenhum componente cadastrado.\n"); return; }
    printf("%-3s %-28s %-18s %-10s\n", "N", "NOME", "TIPO", "PRIORIDADE");
    printf("-----------------------------------------------------------------\n");
    for (int i = 0; i < n; ++i) {
        printf("%-3d %-28s %-18s %-10d\n", i+1, comps[i].nome, comps[i].tipo, comps[i].prioridade);
    }
}

// Algoritmos de ordenação 

void bubbleSortPorNome(Componente comps[], int n, long *comparacoes, double *tempoSecs) {
    *comparacoes = 0;
    clock_t start = clock();

    for (int i = 0; i < n - 1; ++i) {
        int trocou = 0;
        for (int j = 0; j < n - 1 - i; ++j) {
            (*comparacoes)++;
            if (strcmp(comps[j].nome, comps[j+1].nome) > 0) {
                Componente tmp = comps[j];
                comps[j] = comps[j+1];
                comps[j+1] = tmp;
                trocou = 1;
            }
        }
        if (!trocou) break;
    }

    clock_t end = clock();
    *tempoSecs = (double)(end - start) / CLOCKS_PER_SEC;
}

// Ordena por campo 'tipo' (alfabética) usando Insertion Sort.
void insertionSortPorTipo(Componente comps[], int n, long *comparacoes, double *tempoSecs) {
    *comparacoes = 0;
    clock_t start = clock();

    for (int i = 1; i < n; ++i) {
        Componente chave = comps[i];
        int j = i - 1;
        // enquanto j >=0 e comps[j].tipo > chave.tipo, desloca
        while (j >= 0) {
            (*comparacoes)++;
            if (strcmp(comps[j].tipo, chave.tipo) > 0) {
                comps[j+1] = comps[j];
                j--;
            } else break;
        }
        comps[j+1] = chave;
    }

    clock_t end = clock();
    *tempoSecs = (double)(end - start) / CLOCKS_PER_SEC;
}

// Ordena por campo 'prioridade' (inteiro) usando Selection Sort.
void selectionSortPorPrioridade(Componente comps[], int n, long *comparacoes, double *tempoSecs) {
    *comparacoes = 0;
    clock_t start = clock();

    for (int i = 0; i < n - 1; ++i) {
        int idxMax = i;
        for (int j = i + 1; j < n; ++j) {
            (*comparacoes)++;
            if (comps[j].prioridade > comps[idxMax].prioridade) {
                idxMax = j;
            }
        }
        if (idxMax != i) {
            Componente tmp = comps[i];
            comps[i] = comps[idxMax];
            comps[idxMax] = tmp;
        }
    }

    clock_t end = clock();
    *tempoSecs = (double)(end - start) / CLOCKS_PER_SEC;
}

// Realiza busca binária por 'nome' (aplicável apenas se o vetor estiver ordenado por nome em ordem crescente).
int buscaBinariaPorNome(Componente comps[], int n, const char *chave, long *comparacoes) {
    *comparacoes = 0;
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        (*comparacoes)++;
        int cmp = strcmp(comps[mid].nome, chave);
        if (cmp == 0) return mid;
        else if (cmp < 0) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

// Funções auxiliares
int lerPrioridade() {
    char buffer[32];
    int p = 0;
    while (1) {
        printf("Prioridade (1 a 10): ");
        if (!fgets(buffer, sizeof(buffer), stdin)) continue;
        trim(buffer);
        p = atoi(buffer);
        if (p >= 1 && p <= 10) return p;
        printf("Valor inválido. Informe número entre 1 e 10.\n");
    }
}

// cadastra componentes (até MAX_COMP)
int cadastrarComponentes(Componente comps[], int atual) {
    if (atual >= MAX_COMP) {
        printf("Limite máximo (%d) já atingido.\n", MAX_COMP);
        return atual;
    }
    char buffer[128];
    printf("Quantos componentes deseja adicionar? (0-%d): ", MAX_COMP - atual);
    if (!fgets(buffer, sizeof(buffer), stdin)) return atual;
    int q = atoi(buffer);
    if (q <= 0) return atual;
    if (q > (MAX_COMP - atual)) q = MAX_COMP - atual;

    for (int i = 0; i < q; ++i) {
        printf("\nComponente %d de %d:\n", i+1, q);
        printf("Nome: ");
        if (!fgets(comps[atual].nome, MAX_NOME, stdin)) comps[atual].nome[0] = '\0';
        trim(comps[atual].nome);

        printf("Tipo: ");
        if (!fgets(comps[atual].tipo, MAX_TIPO, stdin)) comps[atual].tipo[0] = '\0';
        trim(comps[atual].tipo);

        comps[atual].prioridade = lerPrioridade();
        atual++;
    }
    return atual;
}

// Interface e fluxo
int main() {
    Componente comps[MAX_COMP];
    int n = 0;
    int opcao;
    int ordenadoPorNome = 0; // flag para permitir busca binária

    printf("=== Torre de Resgate: Organização de Componentes ===\n");
    printf("Cadastre até %d componentes necessários para montar a torre.\n", MAX_COMP);

   // Menu principal
    do {
        printf("\n--- MENU ---\n");
        printf("1. Cadastrar componentes\n");
        printf("2. Mostrar componentes\n");
        printf("3. Ordenar por NOME (Bubble Sort)  - para busca binária\n");
        printf("4. Ordenar por TIPO (Insertion Sort)\n");
        printf("5. Ordenar por PRIORIDADE (Selection Sort)\n");
        printf("6. Busca binária por NOME (após ordenar por nome)\n");
        printf("7. Montagem final (confirma componente-chave)\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");

        if (scanf("%d", &opcao) != 1) {
            // limpar buffer e continuar
            int ch; while ((ch = getchar()) != '\n' && ch != EOF);
            printf("Entrada inválida.\n");
            continue;
        }
        int ch; while ((ch = getchar()) != '\n' && ch != EOF); // limpa buffer após scanf

        switch (opcao) {
            case 1:
                n = cadastrarComponentes(comps, n);
                ordenadoPorNome = 0;
                mostrarComponentes(comps, n);
                break;

            case 2:
                mostrarComponentes(comps, n);
                break;

            case 3: {
                if (n == 0) { printf("Nenhum componente cadastrado.\n"); break; }
                long compCount = 0;
                double tsec = 0.0;
                bubbleSortPorNome(comps, n, &compCount, &tsec);
                printf("\n[Bubble Sort por NOME] Comparações: %ld | Tempo: %.6f s\n", compCount, tsec);
                ordenadoPorNome = 1;
                mostrarComponentes(comps, n);
                break;
            }

            case 4: {
                if (n == 0) { printf("Nenhum componente cadastrado.\n"); break; }
                long compCount = 0;
                double tsec = 0.0;
                insertionSortPorTipo(comps, n, &compCount, &tsec);
                printf("\n[Insertion Sort por TIPO] Comparações: %ld | Tempo: %.6f s\n", compCount, tsec);
                ordenadoPorNome = 0;
                mostrarComponentes(comps, n);
                break;
            }

            case 5: {
                if (n == 0) { printf("Nenhum componente cadastrado.\n"); break; }
                long compCount = 0;
                double tsec = 0.0;
                selectionSortPorPrioridade(comps, n, &compCount, &tsec);
                printf("\n[Selection Sort por PRIORIDADE] Comparações: %ld | Tempo: %.6f s\n", compCount, tsec);
                ordenadoPorNome = 0;
                mostrarComponentes(comps, n);
                break;
            }

            case 6: {
                if (!ordenadoPorNome) {
                    printf("Atenção: a busca binária exige que os componentes estejam ordenados por NOME.\n");
                    printf("Use a opção 3 (Ordenar por NOME) antes.\n");
                    break;
                }
                if (n == 0) { printf("Nenhum componente cadastrado.\n"); break; }
                char chave[MAX_NOME];
                printf("Digite o NOME do componente a buscar (busca binária): ");
                if (!fgets(chave, sizeof(chave), stdin)) { chave[0] = '\0'; }
                trim(chave);
                long compsCount = 0;
                clock_t start = clock();
                int idx = buscaBinariaPorNome(comps, n, chave, &compsCount);
                clock_t end = clock();
                double tsec = (double)(end - start) / CLOCKS_PER_SEC;
                if (idx >= 0) {
                    printf("Componente encontrado no índice %d:\n", idx);
                    printf(" Nome: %s | Tipo: %s | Prioridade: %d\n",
                           comps[idx].nome, comps[idx].tipo, comps[idx].prioridade);
                } else {
                    printf("Componente '%s' não encontrado.\n", chave);
                }
                printf("Comparações (busca binária): %ld | Tempo: %.6f s\n", compsCount, tsec);
                break;
            }

            case 7: {
                if (n == 0) { printf("Nenhum componente cadastrado.\n"); break; }
                char chave[MAX_NOME];
                printf("Digite o NOME do componente-chave para ativar a torre: ");
                if (!fgets(chave, sizeof(chave), stdin)) { chave[0] = '\0'; }
                trim(chave);

                // Para montar, é conveniente ter lista visível; se está ordenado por nome, usar binária, caso contrário, faz busca sequencial simples e conta comparações (não obrigatório).
                int encontrado = -1;
                long compsSeq = 0;
                for (int i = 0; i < n; ++i) {
                    compsSeq++;
                    if (strcmp(comps[i].nome, chave) == 0) { encontrado = i; break; }
                }
                if (encontrado >= 0) {
                    printf("\nComponente-chave encontrado: %s | Tipo: %s | Prioridade: %d\n",
                           comps[encontrado].nome, comps[encontrado].tipo, comps[encontrado].prioridade);
                    printf("Montagem iniciada... Torre de resgate ativada!\n");
                } else {
                    printf("\nComponente-chave '%s' NÃO encontrado. Montagem falhou.\n", chave);
                }
                printf("Comparações realizadas (busca sequencial na montagem): %ld\n", compsSeq);
                break;
            }

            case 0:
                printf("Encerrando. Boa sorte na fuga!\n");
                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
