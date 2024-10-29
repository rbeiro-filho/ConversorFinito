#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//------------------------------------------------------------------------------
//------ ESTRUTURAS E OPERAÇÕES PARA FILA DE BINÁRIOS ------

typedef struct ItemFila {
    char Sinal;              // Armazena o sinal do número (positivo ou negativo)
    long int Expoente;       // Expoente na representação binária
    long int Ponto;          // Posição do ponto decimal
    char Erros[3];           // Indicadores de erros: Overflow, Underflow e Truncamento
    char *Binario;           // Representação binária do número
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
    FilaApontador aux = (FilaApontador)malloc(sizeof(CelulaFila));
    if (aux == NULL) {
        perror("Falha na alocacao de memoria");
        return;
    }

    // Copia os dados do item para a nova célula
    aux->I.Sinal = I.Sinal;
    aux->I.Expoente = I.Expoente;
    aux->I.Binario = (char *)malloc(strlen(I.Binario) + 1);
    memcpy(aux->I.Binario, I.Binario, strlen(I.Binario) + 1);

    aux->Prox = NULL;
    if (F->Tras == NULL) {   // Caso a fila esteja vazia
        F->Frente = aux;
    } else {
        F->Tras->Prox = aux; // Insere no final
    }
    F->Tras = aux;
}

// Remove o item da frente da fila
void Desenfileira(Fila *F) {
    if (VFVazia(*F)) {
        printf("Fila vazia, nao e possivel desenfileirar.\n");
        return;
    }

    FilaApontador aux = F->Frente;
    F->Frente = F->Frente->Prox;
    if (F->Frente == NULL) { // Se a fila ficou vazia
        F->Tras = NULL;
    }

    free(aux->I.Binario);    // Libera memória da string binária
    free(aux);               // Libera a célula
}

//------------------------------------------------------------------------------
//------ FILA DE CONVERSÃO DE BINÁRIOS (Parte Inteira) ------

typedef struct ItemBI {
    int Casas;           // Quantidade de dígitos do número decimal
    int DecimalI;        // Parte inteira do número decimal
} ItemBI;

typedef struct CelulaFBI* FBIApontador;

typedef struct CelulaFBI {
    ItemBI I;            // Item com dados do número inteiro decimal
    FBIApontador Prox;   // Apontador para o próximo item
} CelulaFBI;

typedef struct FilaBI {
    FBIApontador Frente; // Apontador para o início da fila
    FBIApontador Tras;   // Apontador para o final da fila
} FilaBI;

// Verifica se a fila está vazia
int VFVaziaBI(FilaBI FBI) {
    return (FBI.Frente == NULL);
}

// Inicializa uma fila vazia
void FFVaziaBI(FilaBI *FBI) {
    FBI->Frente = NULL;
    FBI->Tras = NULL;
}

// Insere um item na fila
void EnfileiraBI(FilaBI *FBI, ItemBI I) {
    FBIApontador aux = (FBIApontador)malloc(sizeof(CelulaFBI));
    if (aux == NULL) {
        perror("Falha na alocacao de memoria");
        return;
    }

    aux->I.Casas = I.Casas;
    aux->I.DecimalI = I.DecimalI;

    aux->Prox = NULL;
    if (FBI->Tras == NULL) {
        FBI->Frente = aux;
    } else {
        FBI->Tras->Prox = aux;
    }
    FBI->Tras = aux;
}

// Remove o item da frente da fila
void DesenfileiraBI(FilaBI *FBI) {
    if (VFVaziaBI(*FBI)) {
        printf("Fila vazia, nao e possivel desenfileirar.\n");
        return;
    }

    FBIApontador aux = FBI->Frente;
    FBI->Frente = FBI->Frente->Prox;
    if (FBI->Frente == NULL) {
        FBI->Tras = NULL;
    }

    free(aux); // Libera a célula
}

//------------------------------------------------------------------------------
//------ PILHA DE CONVERSÃO DE BINÁRIOS (Parte Fracionária) ------

typedef struct ItemPBF {
    int Casas;         // Quantidade de dígitos na parte fracionária
    int DecimalF;      // Parte fracionária do número decimal
} ItemPBF;

typedef struct CelulaPBF* PBFApontador;

typedef struct CelulaPBF {
    ItemPBF I;         // Item com dados da parte fracionária
    PBFApontador Prox; // Apontador para o próximo item
} CelulaPBF;

typedef struct PilhaBF {
    PBFApontador Topo; // Apontador para o topo da pilha
} PilhaBF;

// Verifica se a pilha está vazia
int VPVaziaBF(PilhaBF P) {
    return (P.Topo == NULL);
}

// Inicializa uma pilha vazia
void FPVaziaBF(PilhaBF *P) {
    P->Topo = NULL;
}

// Insere um item no topo da pilha
void EmpilhaBF(PilhaBF *P, ItemPBF I) {
    PBFApontador aux = (PBFApontador)malloc(sizeof(CelulaPBF));
    if (aux == NULL) {
        perror("Falha na alocacao de memoria");
        return;
    }

    aux->I.Casas = I.Casas;
    aux->I.DecimalF = I.DecimalF;

    aux->Prox = P->Topo;
    P->Topo = aux;
}

// Remove o item do topo da pilha
void DesempilhaBF(PilhaBF *P, ItemPBF *I) {
    if (VPVaziaBF(*P)) {
        printf("Pilha vazia, nao e possível desempilhar.\n");
        return;
    }

    PBFApontador aux = P->Topo;
    I->Casas = aux->I.Casas;
    I->DecimalF = aux->I.DecimalF;

    P->Topo = P->Topo->Prox;
    free(aux); // Libera a célula
}

#endif // ESTRUTURAS_H
