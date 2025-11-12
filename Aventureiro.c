#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITENS 10

// Estruturas

typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

typedef struct No {
    Item dados;
    struct No *proximo;
} No;

// Funções globais
int comparacoesSequencial = 0;
int comparacoesBinaria = 0;

// Funções para vetor

// Inserção no vetor
void inserirItemVetor(Item mochila[], int *numItens) {
    if (*numItens >= MAX_ITENS) {
        printf("\n Mochila cheia!\n");
        return;
    }

    Item novo;
    printf("Nome: ");
    scanf(" %[^\n]", novo.nome);
    printf("Tipo: ");
    scanf(" %[^\n]", novo.tipo);
    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);

    mochila[*numItens] = novo;
    (*numItens)++;
    printf("Item '%s' adicionado.\n", novo.nome);
}

// Remoção no vetor
void removerItemVetor(Item mochila[], int *numItens) {
    if (*numItens == 0) {
        printf("\n Mochila vazia!\n");
        return;
    }

    char nome[30];
    printf("Nome do item para remover: ");
    scanf(" %[^\n]", nome);

    int encontrado = 0;
    for (int i = 0; i < *numItens; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            encontrado = 1;
            for (int j = i; j < *numItens - 1; j++) {
                mochila[j] = mochila[j + 1];
            }
            (*numItens)--;
            printf("Item '%s' removido.\n", nome);
            break;
        }
    }

    if (!encontrado) printf("Item não encontrado.\n");
}

// Listagem no vetor
void listarItensVetor(Item mochila[], int numItens) {
    printf("\n=== Itens na Mochila ===\n");
    for (int i = 0; i < numItens; i++) {
        printf("%d. %s | %s | x%d\n",
               i + 1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
    if (numItens == 0) printf("(vazia)\n");
}

// Busca sequencial no vetor
int buscarSequencialVetor(Item mochila[], int numItens, char nome[]) {
    comparacoesSequencial = 0;
    for (int i = 0; i < numItens; i++) {
        comparacoesSequencial++;
        if (strcmp(mochila[i].nome, nome) == 0)
            return i;
    }
    return -1;
}

// Ordenação por nome
void ordenarVetor(Item mochila[], int numItens) {
    Item temp;
    for (int i = 0; i < numItens - 1; i++) {
        for (int j = 0; j < numItens - i - 1; j++) {
            if (strcmp(mochila[j].nome, mochila[j + 1].nome) > 0) {
                temp = mochila[j];
                mochila[j] = mochila[j + 1];
                mochila[j + 1] = temp;
            }
        }
    }
    printf("Itens ordenados alfabeticamente.\n");
}

// Busca binária no vetor (após ordenação)
int buscarBinariaVetor(Item mochila[], int numItens, char nome[]) {
    int inicio = 0, fim = numItens - 1;
    comparacoesBinaria = 0;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        comparacoesBinaria++;
        int cmp = strcmp(mochila[meio].nome, nome);

        if (cmp == 0)
            return meio;
        else if (cmp < 0)
            inicio = meio + 1;
        else
            fim = meio - 1;
    }
    return -1;
}

// Funções para lista encadeada

// Inserção na lista
No* inserirItemLista(No *inicio) {
    No *novo = (No*) malloc(sizeof(No));
    if (!novo) return inicio;

    printf("Nome: ");
    scanf(" %[^\n]", novo->dados.nome);
    printf("Tipo: ");
    scanf(" %[^\n]", novo->dados.tipo);
    printf("Quantidade: ");
    scanf("%d", &novo->dados.quantidade);

    novo->proximo = inicio;
    printf("Item '%s' inserido na lista.\n", novo->dados.nome);
    return novo;
}

// Remoção da lista
No* removerItemLista(No *inicio) {
    if (!inicio) {
        printf("\nLista vazia!\n");
        return NULL;
    }

    char nome[30];
    printf("Nome do item para remover: ");
    scanf(" %[^\n]", nome);

    No *atual = inicio, *anterior = NULL;
    while (atual && strcmp(atual->dados.nome, nome) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (!atual) {
        printf("Item não encontrado.\n");
        return inicio;
    }

    if (!anterior) inicio = atual->proximo;
    else anterior->proximo = atual->proximo;

    free(atual);
    printf("Item '%s' removido.\n", nome);
    return inicio;
}

// Listagem da lista
void listarItensLista(No *inicio) {
    printf("\n=== Itens na Mochila (Lista Encadeada) ===\n");
    if (!inicio) {
        printf("(vazia)\n");
        return;
    }

    int i = 1;
    while (inicio) {
        printf("%d. %s | %s | x%d\n",
               i++, inicio->dados.nome, inicio->dados.tipo, inicio->dados.quantidade);
        inicio = inicio->proximo;
    }
}

// Busca sequencial na lista
void buscarSequencialLista(No *inicio, char nome[]) {
    comparacoesSequencial = 0;

    while (inicio) {
        comparacoesSequencial++;
        if (strcmp(inicio->dados.nome, nome) == 0) {
            printf("\nItem encontrado: %s | %s | x%d\n",
                   inicio->dados.nome, inicio->dados.tipo, inicio->dados.quantidade);
            printf("Comparações realizadas: %d\n", comparacoesSequencial);
            return;
        }
        inicio = inicio->proximo;
    }

    printf("Item '%s' não encontrado. Comparações: %d\n",
           nome, comparacoesSequencial);
}

// Função principal
int main() {
    Item mochilaVetor[MAX_ITENS];
    int numItensVetor = 0;

    No *inicioLista = NULL;

    int modo, opcao;

    printf("\n=== COMPARATIVO DE ESTRUTURAS ===\n");
    printf("Escolha a estrutura para testar:\n");
    printf("1. Vetor (lista sequencial)\n");
    printf("2. Lista encadeada\n");
    printf("Opção: ");
    scanf("%d", &modo);

    if (modo == 1) {
        do {
            printf("\n===== MOCHILA =====\n");
            printf("1. Inserir\n2. Remover\n3. Listar\n4. Buscar Sequencial\n5. Ordenar\n6. Buscar Binária\n0. Sair\n");
            printf("Escolha: ");
            scanf("%d", &opcao);

            char nome[30];
            int pos;

            switch (opcao) {
                case 1: inserirItemVetor(mochilaVetor, &numItensVetor); break;
                case 2: removerItemVetor(mochilaVetor, &numItensVetor); break;
                case 3: listarItensVetor(mochilaVetor, numItensVetor); break;
                case 4:
                    printf("Nome para buscar: ");
                    scanf(" %[^\n]", nome);
                    pos = buscarSequencialVetor(mochilaVetor, numItensVetor, nome);
                    if (pos >= 0)
                        printf("Encontrado: %s | %s | x%d\n", mochilaVetor[pos].nome, mochilaVetor[pos].tipo, mochilaVetor[pos].quantidade);
                    else
                        printf("Não encontrado.\n");
                    printf("Comparações: %d\n", comparacoesSequencial);
                    break;
                case 5: ordenarVetor(mochilaVetor, numItensVetor); break;
                case 6:
                    printf("Nome para busca binária: ");
                    scanf(" %[^\n]", nome);
                    pos = buscarBinariaVetor(mochilaVetor, numItensVetor, nome);
                    if (pos >= 0)
                        printf("Encontrado: %s | %s | x%d\n", mochilaVetor[pos].nome, mochilaVetor[pos].tipo, mochilaVetor[pos].quantidade);
                    else
                        printf("Não encontrado.\n");
                    printf("Comparações: %d\n", comparacoesBinaria);
                    break;
            }
        } while (opcao != 0);
    }

    else if (modo == 2) {
        do {
            printf("\n===== MOCHILA =====\n");
            printf("1. Inserir\n2. Remover\n3. Listar\n4. Buscar Sequencial\n0. Sair\n");
            printf("Escolha: ");
            scanf("%d", &opcao);

            char nome[30];

            switch (opcao) {
                case 1: inicioLista = inserirItemLista(inicioLista); break;
                case 2: inicioLista = removerItemLista(inicioLista); break;
                case 3: listarItensLista(inicioLista); break;
                case 4:
                    printf("Nome para buscar: ");
                    scanf(" %[^\n]", nome);
                    buscarSequencialLista(inicioLista, nome);
                    break;
            }
        } while (opcao != 0);
    }

    else {
        printf("Opção inválida.\n");
    }

    return 0;
}
