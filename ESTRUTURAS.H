#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//------------------------------------------------------------------------------
//------ ESTRUTURAS E OPERAÇÕES PARA FILA DE BINÁRIOS ------

typedef struct ItemFila {
    char Sinal;             // Armazena o sinal do número (positivo ou negativo)
    long int Expoente;      // Expoente na representação binária
    int Erros[4];           // Indicadores de erros: Overflow, Underflow, Arredondamento, Truncamento
    char *BinarioI;
    char *BinarioF;
} ItemFila;

typedef struct CelulaFila* FilaApontador;

typedef struct CelulaFila {
    ItemFila I;              // Item da fila com os dados binários
    FilaApontador Prox;      // Apontador para a próxima célula
} CelulaFila;

typedef struct Fila {
    FilaApontador Frente;    // Apontador para o primeiro item na fila
    FilaApontador Tras;      // Apontador para o último item na fila
} Fila;

// Verifica se a fila está vazia
int VFVazia(Fila F) {
    return (F.Frente == NULL);
}

// Inicializa uma fila vazia
void FFVazia(Fila *F) {
    F->Frente = NULL;
    F->Tras = NULL;
}

// Insere um novo item no final da fila
void Enfileira(Fila *F, ItemFila I) {
    // Aloca memória para a nova célula
    FilaApontador aux = (FilaApontador)malloc(sizeof(CelulaFila));
    if (aux == NULL) {
        perror("Falha na alocacao de memoria");
        return;
    }

    if (F == NULL) {
        printf("Fila nao inicializada\n");
        free(aux); // Libera a célula alocada se a fila não estiver inicializada
        return;
    }

    // Copia os dados do item para a nova célula
    aux->I.Sinal = I.Sinal;
    aux->I.Expoente = I.Expoente;

    // Aloca e copia a string BinarioI
    if (I.BinarioI != NULL) {
        aux->I.BinarioI = strdup(I.BinarioI);
        if (aux->I.BinarioI == NULL) {
            perror("Falha na alocacao de memoria");
            free(aux); // Libera a célula se a alocação falhar
            return;
        }
    } else {
        aux->I.BinarioI = NULL; // Define como NULL se I.BinarioI for NULL
    }

    // Aloca e copia a string BinarioF
    if (I.BinarioF != NULL) {
        aux->I.BinarioF = strdup(I.BinarioF);
        if (aux->I.BinarioF == NULL) {
            perror("Falha na alocacao de memoria");
            free(aux->I.BinarioI); // Libera a memória de BinarioI se necessário
            free(aux); // Libera a célula se a alocação falhar
            return;
        }
    } else {
        aux->I.BinarioF = NULL; // Define como NULL se I.BinarioF for NULL
    }

    // Copia os erros
    for (int i = 0; i < 4; i++) {
        aux->I.Erros[i] = I.Erros[i];
    }

    // Define o próximo como NULL
    aux->Prox = NULL;

    // Insere na fila
    if (F->Tras == NULL) { // Caso a fila esteja vazia
        F->Frente = aux;
    } else {
        F->Tras->Prox = aux; // Insere no final
    }
    F->Tras = aux; // Atualiza o ponteiro tras
}


// Remove o item da frente da fila
void Desenfileira(Fila *F) {
    if(VFVazia(*F)) {
        printf("Fila vazia, nao e possivel desenfileirar.\n");
        return;
    }

    FilaApontador aux = F->Frente;
    F->Frente = F->Frente->Prox;
    if (F->Frente == NULL) {
        F->Tras = NULL;
    }

    free(aux); // Libera a célula
}
//------------------------------------------------------------------------------
//------ FILA DE CONVERSÃO DE BINÁRIOS (inteiro e fracionario) ------

typedef struct ItemB {
    int Casas;           // Quantidade de dígitos do número decimal
    int Decimal;        // Parte inteira do número decimal
} ItemB;

typedef struct CelulaFB* FBApontador;

typedef struct CelulaFB {
    ItemB I;            // Item com dados do número inteiro decimal
    FBApontador Prox;   // Apontador para o próximo item
} CelulaFB;

typedef struct FilaB {
    FBApontador Frente; // Apontador para o início da fila
    FBApontador Tras;   // Apontador para o final da fila
} FilaB;

// Verifica se a fila está vazia
int VFVaziaB(FilaB F) {
    return (F.Frente == NULL);
}

// Inicializa uma fila vazia
void FFVaziaB(FilaB *F) {
    F->Frente = NULL;
    F->Tras = NULL;
}

// Insere um item na fila
void EnfileiraB(FilaB *F, ItemB I) {
    FBApontador aux = (FBApontador)malloc(sizeof(CelulaFB));
    if (aux == NULL) {
        perror("Falha na alocacao de memoria");
        return;
    }

    aux->I.Casas = I.Casas;
    aux->I.Decimal = I.Decimal;

    aux->Prox = NULL;
    if (F->Tras == NULL) {
        F->Frente = aux;
    } else {
        F->Tras->Prox = aux;
    }
    F->Tras = aux;
}

// Remove o item da frente da fila
void DesenfileiraB(FilaB *F) {
    if(VFVaziaB(*F)) {
        printf("Fila vazia, nao e possivel desenfileirar.\n");
        return;
    }

    FBApontador aux = F->Frente;
    F->Frente = F->Frente->Prox;
    if (F->Frente == NULL) {
        F->Tras = NULL;
    }

    free(aux); // Libera a célula
}

#endif // ESTRUTURAS_H
