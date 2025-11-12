#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ITENS 10

// Estrutura que representa um item da mochila
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

  // Adiciona o item na mochila se tiver espaço
void inserirItem(Item mochila[], int *numItens) {
    if (*numItens >= MAX_ITENS) {
        printf("\nMochila cheia! Não é possível adicionar mais itens.\n");
        return;
    }

    Item novo;
    printf("\n=== Cadastro de Novo Item ===\n");
    printf("Nome do item: ");
    scanf(" %[^\n]", novo.nome);
    printf("Tipo (arma, munição, cura, etc): ");
    scanf(" %[^\n]", novo.tipo);
    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);

    mochila[*numItens] = novo;
    (*numItens)++;

    printf("Item '%s' adicionado com sucesso!\n", novo.nome);
}

  // Remove o item da mochila com base no nome
void removerItem(Item mochila[], int *numItens) {
    if (*numItens == 0) {
        printf("\nA mochila está vazia.\n");
        return;
    }

    char nomeRemover[30];
    printf("\nNome do item a remover: ");
    scanf(" %[^\n]", nomeRemover);

    int encontrado = 0;
    for (int i = 0; i < *numItens; i++) {
        if (strcmp(mochila[i].nome, nomeRemover) == 0) {
            encontrado = 1;
            for (int j = i; j < *numItens - 1; j++) {
                mochila[j] = mochila[j + 1];
            }
            (*numItens)--;
            printf("Item '%s' removido da mochila.\n", nomeRemover);
            break;
        }
    }

    if (!encontrado)
        printf("Item '%s' não encontrado.\n", nomeRemover);
}

  // Exibe todos os itens que estão na mochila
void listarItens(Item mochila[], int numItens) {
    printf("\n=== Itens na Mochila ===\n");
    if (numItens == 0) {
        printf("A mochila está vazia.\n");
        return;
    }

    for (int i = 0; i < numItens; i++) {
        printf("%d. Nome: %s | Tipo: %s | Quantidade: %d\n",
               i + 1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}

  // Realiza a busca de itens na mochila
void buscarItem(Item mochila[], int numItens) {
    if (numItens == 0) {
        printf("\n A mochila está vazia.\n");
        return;
    }

    char nomeBusca[30];
    printf("\nDigite o nome do item a buscar: ");
    scanf(" %[^\n]", nomeBusca);

    for (int i = 0; i < numItens; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            printf("\n🔎 Item encontrado!\n");
            printf("Nome: %s\nTipo: %s\nQuantidade: %d\n",
                   mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
            return;
        }
    }

    printf("Item '%s' não encontrado na mochila.\n", nomeBusca);
}

  // Função principal para controlar o sistema de mochila
int main() {
    Item mochila[MAX_ITENS];
    int numItens = 0;
    int opcao;

    do {
        printf("\n========== MOCHILA DO JOGADOR ==========\n");
        printf("1. Adicionar item\n");
        printf("2. Remover item\n");
        printf("3. Listar itens\n");
        printf("4. Buscar item\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                inserirItem(mochila, &numItens);
                listarItens(mochila, numItens);
                break;
            case 2:
                removerItem(mochila, &numItens);
                listarItens(mochila, numItens);
                break;
            case 3:
                listarItens(mochila, numItens);
                break;
            case 4:
                buscarItem(mochila, numItens);
                break;
            case 0:
                printf("\n Saindo do sistema de mochila...\n");
                break;
            default:
                printf("\n Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}
