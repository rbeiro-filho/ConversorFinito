#include "ESTRUTURAS.H"

#define BLOCK_SIZE 32

int EntradaPadronizada(char Entrada[]);
int EntradaNula(char Entrada[]);
void ExplicitaSinal(char Entrada[]);
void RemoveZeros(char Entrada[]);

int main(void) {
    size_t M; // Mantissa
    long int L, U; // Expoentes
    char Verificador;
    int ch;

    printf("Parametros:\n - Mantissa: ");
    scanf(" %zu", &M);
    printf("Limite inferior do expoente: ");
    scanf(" %li", &L);
    printf("Limite superior do expoente: ");
    scanf(" %li", &U);
    // Limpa o buffer do stdin para remover qualquer caractere extra
    while ((ch = getchar()) != '\n' && ch != EOF);

    if (L > U) {
        printf("\nErro: limite inferior maior que limite superior\n");
        return 1;
    }

    do {
        size_t Alocado = BLOCK_SIZE; // Tamanho inicial alocado
        size_t Tamanho = 0;          // Tamanho atual da string
        char *Entrada = (char *)malloc(BLOCK_SIZE);  // Aloca o primeiro bloco

        if (Entrada == NULL) {
            perror("Falha na alocação de memória");
            return 1;
        }

        printf("Digite o valor decimal: ");
        while ((ch = getchar()) != '\n' && ch != EOF) {
            if (Tamanho + 2 >= Alocado) {  // Checa se precisa alocar mais espaço
                Alocado += BLOCK_SIZE;
                char *temp = realloc(Entrada, Alocado);
                if (temp == NULL) {
                    perror("Falha ao realocar memória");
                    free(Entrada);
                    return 1;
                }
                Entrada = temp;
            }
            Entrada[Tamanho++] = ch;  // Armazena o caractere atual
        }
        Entrada[Tamanho] = '\0';  // Finaliza a string com '\0'

        // Validação de entrada
        if (EntradaPadronizada(Entrada) || EntradaNula(Entrada) || Entrada[0] == '\0') {
            free(Entrada);
            continue;
        }

        // Deixa o sinal explicito na string
        ExplicitaSinal(Entrada);
        // Remove zeros a esquerda e a direita
        RemoveZeros(Entrada);

        // --- Numero preparado para a conversao ---
        /*
        ESTRUTURA // ESSA ESTRUTURA NAO TEM NADA HAVER COM A FILA DENTRO DA CONVERSAO

        typedef struct ItemFila {
            char Sinal;
            size_t Expoente;
            size_t Ponto;
            char Erros[3]; // Overflow, Underflow e Truncamento
            char *Binario;
        } ItemFila;

        // Leva PosicaoPonto até a posicao referente ao caractere na string e retorna 1 - Possui, 0 - Não possui
        int PossuiPonto(char *Entrada, size_t *i) {
            size_t i = 0;
            char *ProcuraPonto = Entrada;

            while(*ProcuraPonto != '\0' && *ProcuraPonto != '.') {
                i++;
                ProcuraPonto++;
            }
            
            if(*ProcuraPonto == '\0') {
                return 0;
            } else {
                return 1;
            }
        }
        // ESSE NUMERO SERA USADO PARA SABER SE SERA NECESSARIO A CONVERSAO DA PARTE FRACIONARIA
        // TAMBEM SERA USADO PARA DEFINIR O EXPOENTE

        CRIAR FILA PARA CONVERTER A PARTE INTEIRA {
            REMOVE-SE 2 DO NUMERO E RECEBE A DIVISAO DESSE VALOR POR 8 AMBAS AS PARTES (INTEIRA E FRACIONARIA)
            CRIA A PRIMEIRA CELULA E ARMAZENA O VALOR RESTO NO VALOR DE EXPOENTE DE CADA CELULA
            O MAIOR VALOR SERA 8, O QUE SERA FEITO PARA AS OUTRAS CELULAS
            ELAS RECEBERAO A QUANTIDADE DE CARACTERES 
        }

        CONVERTE PARA BINARIO A PARTE INTEIRA {
            DIVIDI-SE OS VALORES POR 2 E O RESTO (DA ULTIMA CELULA) SERA ARMAZENADO NA STRING
            QUANDO DIVIDINDO POR 2 E A PRIMEIRA CELULA NA DIVISAO INTEIRA FOR ZERO
            RECEBER O RESTO DA DIVISAO, MULTIPLICAR POR 10⁸
            DESEMPILHAR A PRIMEIRA
            EXECUTAR A CONTA DE DIVISAO NA PROXIMA CELULA E ENTAO SOMAR O VALOR NA CELULA
            O RESTO DA ULTIMA CELULA SERA ARMAZENADO NA STRING ATE ELA ZERAR OU ESTOURAR A PRECISAO (OVERFLOW OU UNDERFLOW)
        }

        SE RETORNOU 1 A FUNCAO Nº2
        
        PARA SABER A POSICAO INICIAL ATÉ O FINAL RECEBER (STRLEN(DECIMAL - ITEM.PONTO - 1)) //CONFIRMAR CALCULO

        CRIAR PILHA PARA A CONVERSAO DA PARTE FRACIONARIA {
            USAR A MESMA LOGICA DE PILHAS MAS A PARTE DE RESTO FICARA NO TOPO DA PILHA
            IR MULTIPLICANDO E QUANDO PASSAR O VALOR LIMITE SUBIR O NUMERO PARA A PROXIMA NA MESMA LOGICA
            SE A ULTIMA PILHA EM X % 10^RESTO FOR ZERO, DESEMPILHA
            AO LONGO QUE FOR DISTRIBUINDO OS VALORES PARA A STRING, VERIFICAR SE O LIMITE JÁ PASSOU (TRUNCAMENTO)
            SE AINDA HOUVER VALORES DENTRO DAS PILHAS, MAS CHEGOU O LIMITE, MOSTRAR QUE HOUVE TRUNCAMENTO
            SENAO  APENAS O NUMERO SERA ARMAZENADO
        } 
        
        // CRIAR AS BIBLIOTECAS DE PILHA PARA CONVERSAO FRACIONARIA, FILA PARA A CONVERSAO INTEIRA
        */
        

        printf(" %s", Entrada);

        free(Entrada);  // Libera a memória alocada
        return 0;

        printf("Deseja inserir mais um número? (s/n): ");
        scanf(" %c", &Verificador);
        while ((ch = getchar()) != '\n' && ch != EOF);

    } while (Verificador != 'N' && Verificador != 'n');  // Sai do loop se 'N' ou 'n' for digitado

    return 0;
}

// Verifica se a entrada esta no padrao a ser analizado
int EntradaPadronizada(char Entrada[]) {
    int Ponto = 0;
    for (size_t i = 0; Entrada[i] != '\0'; i++) {
        if ((Entrada[i] == '+' || Entrada[i] == '-') && i > 0) {
            printf("Sinal em local incorreto\nValor invalidado\n");
            return 1;
        } else if ((Entrada[i] < '0' || Entrada[i] > '9') && Entrada[i] != '.' && Entrada[i] != '+' && Entrada[i] != '-') {
            printf("Caracteres inseridos incoerentes\nValor invalidado\n");
            return 1;
        } else if (Entrada[i] == '.') {
            Ponto++;
            if (Ponto > 1) {
                printf("Pontuacao inadequada\nValor invalidado\n");
                return 1;
            }
        }
    }
    return 0;
}
// Verifica se a entrada eh nula
int EntradaNula(char Entrada[]) {
    for(size_t i = 0; Entrada[i] != '\0'; i++) {
        if(Entrada[i] > '0' && Entrada[i] <= '9') {
            return 0;
        }
    }
    return 1;
}
// Explicita o sinal positivo ou negativo no inicio da string
void ExplicitaSinal(char Entrada[]) {
    // Verifica se o sinal ja esta explicito
    if (Entrada[0] != '+' && Entrada[0] != '-') {
        // Move a string uma posicao para frente para abrir espaco para o sinal
        size_t len = strlen(Entrada);
        for (size_t i = len + 1; i > 0; i--) {
            Entrada[i] = Entrada[i - 1];
        }
        Entrada[0] = '+';  // Define o sinal positivo por padrao
    }
}
// Remove os zeros à esquerda e à direita da string de entrada
void RemoveZeros(char Entrada[]) {
    size_t NumI = 1; // Inicia na posição 1 devido ao sinal em Entrada[0]
    
    // Remove zeros à esquerda
    while (Entrada[NumI] == '0' && Entrada[NumI + 1] != '.' && Entrada[NumI + 1] != '\0') {
        NumI++;
    }
    
    // Desloca a string para a esquerda se houve zeros a esquerda para remover
    if (NumI > 1) {
        memmove(Entrada + 1, Entrada + NumI, strlen(Entrada) - NumI + 1); // Inclui o '\0'
    }

    // Remove zeros a direita da parte fracionária
    size_t len = strlen(Entrada);
    if (strchr(Entrada, '.')) {
        size_t NumF = len - 1;
        while (Entrada[NumF] == '0') { // Remove zeros à direita
            NumF--;
        }
        if (Entrada[NumF] == '.') {
            NumF--; // Remove o ponto final, se ele estiver sozinho
        }
        Entrada[NumF + 1] = '\0'; // Define o final da string
    }
}

/*
Separar de 8 em 8 casas as partes inteiras até o ponto e armazenar em filas
Realizar a conversão antes de receber outro número
Realizar a conversão e se sobrar resto, jogar para a nona casa de inteiros da próxima celula da fila até a conversão total
Verificar se de acordo com o ponto é possível representar ou vai acontecer o estouro, se houver estouro, não será necessário converter a parte binária.
Se isso acontecer, liberar espaço de binário e salvar o erro para ser mostrado "OVERFLOW" ou "UNDERFLOW"
Se for negativo inverter fazendo 0 virar 1 e vice versa
Armazenar as partes fracionarias em pilhas e ir multiplicando a pilha toda por 2 até o limite ou a conversao se feitas
Os valores que ficarem maiores que 10⁷-1 na celula do fundo da pilha, será 1 senao 0 no valor da conversao final
*/