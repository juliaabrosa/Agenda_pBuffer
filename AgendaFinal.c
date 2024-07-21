#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OP sizeof(int)
#define NOME 16
#define IDADE sizeof(int)
#define EMAIL 32
#define CONTATO (NOME + IDADE + EMAIL)
#define NEXT (OP + CONTATO + sizeof(void *))

void insere(void *pBuffer, void **head);
void buscar(void *pBuffer, void *head);
void removePessoa(void *pBuffer, void **head);
void listar(void *head);

int main() {
    void *pBuffer = malloc(NEXT);
    void *head = NULL;

    if (!pBuffer) {
        printf("Erro ao alocar memória para pBuffer.\n");
        return 1;
    }

    do {
        printf("Digite a opcao desejada:\n");
        printf("1-Adicionar pessoa\n2-Buscar pessoa\n3-Excluir pessoa\n4-Listar todos\n5-Sair\n");
        scanf("%d", (int *)pBuffer);

        switch (*(int *)pBuffer) {
            case 1:
                printf("Nome: ");
                scanf("%s", (char *)(pBuffer + OP));
                printf("Idade: ");
                scanf("%d", (int *)(pBuffer + OP + NOME));
                printf("Email: ");
                scanf("%s", (char *)(pBuffer + OP + NOME + IDADE));

                insere(pBuffer, &head);
                break;
            case 2:
                printf("\nDigite o nome do contato que voce deseja buscar: ");
                scanf("%s", (char *)(pBuffer + OP));
                buscar(pBuffer, head);
                break;
            case 3:
                printf("\nDigite o nome do contato que voce deseja remover: ");
                scanf("%s", (char *)(pBuffer + OP));
                removePessoa(pBuffer, &head);
                break;
            case 4:
                listar(head);
                break;
            case 5:
                printf("\nAte mais!\n");
                free(pBuffer);
                free(head);
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (*(int *)pBuffer != 5);

    
    return 0;
}

void insere(void *pBuffer, void **head) {
    void *novo = malloc(CONTATO + sizeof(void *));
    if (!novo) {
        printf("Erro ao alocar memória.\n");
        return;
    }

    memcpy(novo, pBuffer + OP, NOME);
    memcpy(novo + NOME, pBuffer + OP + NOME, IDADE);
    memcpy(novo + NOME + IDADE, pBuffer + OP + NOME + IDADE, EMAIL);

    void **next = (void **)(novo + CONTATO);
    *next = NULL;

    void *current = *head;
    void *previous = NULL;

    while (current != NULL && strcmp((char *)current, (char *)novo) < 0) {
        previous = current;
        current = *(void **)((char *)current + CONTATO);
    }

    if (previous == NULL) {
        *next = *head;
        *head = novo;
    } else {
        *next = current;
        *(void **)((char *)previous + CONTATO) = novo;
    }
}

void buscar(void *pBuffer, void *head) {
    void *current = head;

    while (current != NULL) {
        if (strcmp((char *)current, (char *)(pBuffer + OP)) == 0) {
            printf("Nome: %s\n", (char *)current);
            printf("Idade: %d\n", *(int *)(current + NOME));
            printf("Email: %s\n", (char *)(current + NOME + IDADE));
            return;
        }
        current = *(void **)((char *)current + CONTATO);
    }
    printf("Pessoa nao encontrada.\n");
}

void removePessoa(void *pBuffer, void **head) {
    void *current = *head;
    void *previous = NULL;

    while (current != NULL && strcmp((char *)current, (char *)(pBuffer + OP)) != 0) {
        previous = current;
        current = *(void **)((char *)current + CONTATO);
    }

    if (current == NULL) {
        printf("Pessoa nao encontrada.\n");
        return;
    }

    if (previous == NULL) {
        *head = *(void **)((char *)current + CONTATO);
    } else {
        *(void **)((char *)previous + CONTATO) = *(void **)((char *)current + CONTATO);
    }

    free(current);
    printf("Pessoa removida.\n");
}

void listar(void *head) {
    void *current = head;

    while (current != NULL) {
        printf("Nome: %s\n", (char *)current);
        printf("Idade: %d\n", *(int *)(current + NOME));
        printf("Email: %s\n", (char *)(current + NOME + IDADE));
        current = *(void **)((char *)current + CONTATO);
    }
}