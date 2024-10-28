#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ItemFila {
    char Sinal;
    size_t Expoente;
    char *Binario;
} ItemFila;

typedef struct CelulaFila* FilaApontador; 

typedef struct CelulaFila {
    ItemFila I;
    FilaApontador Prox;
} CelulaFila;

typedef struct Fila {
    FilaApontador Frente;
    FilaApontador Tras;
} Fila;

// FUNÇÕES
// Verifica se a fila está vazia
int VFVazia(Fila F) {
    return (F.Frente == NULL);
}

// Cria uma fila vazia
void FFVazia(Fila *F) {
    F->Frente = NULL;
    F->Tras = NULL;
}

// Enfileira um item na fila
void Enfileira(Fila *F, ItemFila I) {
    FilaApontador aux = (FilaApontador)malloc(sizeof(CelulaFila));
    if (aux == NULL) {
        perror("Falha na alocacao de memoria");
        return;
    }
    
    aux->I.Sinal = I.Sinal;
    aux->I.Expoente = I.Expoente;
    aux->I.Binario = (char *)malloc(strlen(I.Binario) + 1);
    memcpy(aux->I.Binario, I.Binario, strlen(I.Binario) + 1);
    
    aux->Prox = NULL;
    if (F->Tras == NULL) {
        F->Frente = aux;
    } else {
        F->Tras->Prox = aux;
    }
    F->Tras = aux;
}

// Desenfileira um item da fila
void Desenfileira(Fila *F, ItemFila *I) {
    if (VFVazia(*F)) {
        printf("Fila vazia, nao e possivel desenfileirar.\n");
        return;
    }
    
    FilaApontador aux = F->Frente;
    F->Frente = F->Frente->Prox;
    if (F->Frente == NULL) {
        F->Tras = NULL; // A fila está vazia agora
    }
    I->Sinal = aux->I.Sinal;
    I->Expoente = aux->I.Expoente;
    I->Binario = (char *)malloc(strlen(aux->I.Binario) + 1);
    memcpy(I->Binario, aux->I.Binario, strlen(aux->I.Binario) + 1);

    free(aux->I.Binario); // Libera a memória da string
    free(aux); // Libera a célula
}

//------------------------------------------------------------------------------

typedef struct ItemLista {
    // Item
} ItemLista;

typedef struct CelulaLista* ListaApontador;

typedef struct CelulaLista {
    ItemLista I;
    ListaApontador Prox;
} CelulaLista;

typedef struct Lista {
    ListaApontador Primeiro;
    ListaApontador Ultimo;
} Lista;

// FUNÇÕES
// Verifica se a lista está vazia
int VLVazia(Lista L) {
    return (L.Primeiro == NULL);
}

// Faz uma lista vazia
void FLVazia(Lista *L) {
    L->Primeiro = NULL;
    L->Ultimo = NULL;
}

// Insere um item na lista
void Insere(Lista *L, ListaApontador Posicao_Anterior, ItemLista I) {
    if (Posicao_Anterior == NULL) {
        printf("Nao e possivel inserir um item nessa posicao\n");
        return;
    }

    ListaApontador aux = (ListaApontador)malloc(sizeof(CelulaLista));
    if (aux == NULL) {
        perror("Falha na alocacao de memoria");
        return;
    }

    aux->I = I;
    aux->Prox = Posicao_Anterior->Prox;
    Posicao_Anterior->Prox = aux;

    if (Posicao_Anterior == L->Ultimo) {
        L->Ultimo = aux; // Atualiza o último
    }
}

// Remove um item da lista
void Remove(Lista *L, ListaApontador Posicao_Anterior, ItemLista *I) {
    if (Posicao_Anterior == NULL || Posicao_Anterior->Prox == NULL) {
        printf("Nao e possível remover este item\n");
        return;
    }

    ListaApontador aux = Posicao_Anterior->Prox;
    *I = aux->I;
    Posicao_Anterior->Prox = aux->Prox;

    if (aux == L->Ultimo) {
        L->Ultimo = Posicao_Anterior; // Atualiza o último
    }

    free(aux); // Libera a célula removida
}

//------------------------------------------------------------------------------

typedef struct ItemPilha {
    // Item
} ItemPilha;

typedef struct CelulaPilha* PilhaApontador;

typedef struct CelulaPilha {
    ItemPilha I;
    PilhaApontador Prox;
} CelulaPilha;

typedef struct Pilha {
    PilhaApontador Topo;
} Pilha;

// FUNÇÕES
// Verifica se a pilha está vazia
int VPVazia(Pilha P) {
    return (P.Topo == NULL);
}

// Faz uma pilha vazia
void FPVazia(Pilha *P) {
    P->Topo = NULL;
}

// Empilha um item na pilha
void Empilha(Pilha *P, ItemPilha I) {
    PilhaApontador aux = (PilhaApontador)malloc(sizeof(CelulaPilha));
    if (aux == NULL) {
        perror("Falha na alocacao de memoria");
        return;
    }

    aux->I = I;
    aux->Prox = P->Topo;
    P->Topo = aux;
}

// Desempilha um item da pilha
void Desempilha(Pilha *P, ItemPilha *I) {
    if (VPVazia(*P)) {
        printf("Pilha vazia, nao e possível desempilhar.\n");
        return;
    }

    PilhaApontador aux = P->Topo;
    *I = aux->I;
    P->Topo = P->Topo->Prox;
    free(aux); // Libera a célula
}

#endif // ESTRUTURAS_H