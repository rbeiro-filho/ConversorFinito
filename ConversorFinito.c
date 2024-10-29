#include "ESTRUTURAS.H"

#define BLOCK_SIZE 32

int main(void) {
    Fila Binarios;
    long int M; // Mantissa
    long int L, U; // Expoentes
    char Verificador;
    char ch;

    printf("Parametros:\n - Mantissa: ");
    scanf(" %li", &M);
    printf("Limite inferior do expoente (sem sinal de negativo): ");
    scanf(" %li", &L);
    printf("Limite superior do expoente: ");
    scanf(" %li", &U);

    if(M < 1|| M > 4294967296) {
        printf("A mantissa nao pode assumir o valor inserido\n");
        return 1;
    }

    if(L < -2147483648 || U >  2147483647) {
        printf("Expoente acima do limite de representacao\n");
        return 1;
    }
    if(L > U) {
        printf("O limite de expoentes da esquerda nao pode ser maior que o da direita\n");
        return 1;
    }

    // Limpa o buffer do stdin para remover qualquer caractere extra
    while ((ch = getchar()) != '\n' && ch != EOF);

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
        
        // Recebe a posicao do ponto ou o fim da string
        
        ItemFila I;
        long int MantissaLimite = M;
        long int TamanhoEntrada = strlen(Entrada);
        long int Ponto = PontoOFinal(Entrada);

        if(Entrada[1] != '0' || Entrada[2] != '.') {  // Possui apenas parte inteira
            FilaB FI;
            
            long int QtdNums = (Ponto - 1); // Quantidade de numeros
            int PID = QtdNums / 8; // Parte inteira da divisao 
            int MD = QtdNums % 8; // Modulo da divisao
            char *PonteiroConversaoI = Entrada + 1;
            
            FFVaziaB(&FI); // Faz fila vazia

            if(MD > 0) {
                ItemB Iaux;
                
                Iaux.Casas = MD;
                Iaux.Decimal = ConverteInteiro(PonteiroConversaoI, Iaux.Casas); // Recebe o size_t convertido
                EnfileiraBI(&FI,Iaux); // Enfileira 
            }
            for(size_t i = PID; i > 0; i--) {
                ItemB Iaux;
                
                Iaux.Casas = 8;
                Iaux.Decimal = ConverteInteiro(PonteiroConversaoI, 8);
                EnfileiraBI(&FI,Iaux);
            }

            //Falta
        } else {
            I.BinarioI = NULL;
        }

        if(Ponto != 0) { // Possui apenas parte fracionaria
            FilaB FF;
            ItemFila IF;
            long int QtdNums = (TamanhoEntrada - Ponto - 1);
            int PID = QtdNums / 8; // Parte inteira da divisao 
            int MD = QtdNums % 8; // Modulo da divisao
            char *PonteiroConversaoI = Entrada + Ponto + 1; // Ponteiro para o primeiro numero da parte fracionaria

            FFVaziaB(&FF); // Faz pilha vazia

            if(MD > 0) { 
                ItemB I;
                
                I.Casas = MD;
                I.Decimal = ConverteInteiro(PonteiroConversaoI, I.Casas); // Recebe o size_t convertido
                EnfileiraB(&FF,I); // Enfileira 
            }
            for(size_t i = PID; i > 0; i--) {
                ItemB I;
                
                I.Casas = 8;
                I.Decimal = ConverteInteiro(PonteiroConversaoI, 8);
                EnfileiraB(&FF,I);
                }

            // Binario
        } else {
            I.BinarioF = NULL;
        }

        // Inserir os outros valores em I antes

        Enfileira(&Binarios,I);

        printf("Deseja inserir mais um número? (s/n): ");
        scanf(" %c", &Verificador);
        while ((ch = getchar()) != '\n' && ch != EOF);

    } while (Verificador != 'N' && Verificador != 'n');  // Sai do loop se 'N' ou 'n' for digitado

    return 0;
}

// Verifica se a entrada esta no padrao a ser analizado
// Verifica se a entrada está em um formato padrão
int EntradaPadronizada(char Entrada[]) {
    int Ponto = 0;
    for (size_t i = 0; Entrada[i] != '\0'; i++) {
        // Verifica sinal em posição incorreta
        if ((Entrada[i] == '+' || Entrada[i] == '-') && i > 0) {
            printf("Sinal em local incorreto\nValor invalidado\n");
            return 1;
        // Verifica caracteres inválidos
        } else if ((Entrada[i] < '0' || Entrada[i] > '9') && Entrada[i] != '.' && Entrada[i] != '+' && Entrada[i] != '-') {
            printf("Caracteres inseridos incoerentes\nValor invalidado\n");
            return 1;
        // Verifica se há mais de um ponto decimal
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

// Verifica se a entrada é nula (apenas zeros ou vazia)
int EntradaNula(char Entrada[]) {
    for(size_t i = 0; Entrada[i] != '\0'; i++) {
        if(Entrada[i] > '0' && Entrada[i] <= '9') {
            return 0;
        }
    }
    printf(" Entrada nula\n");
    return 1;
}

// Adiciona sinal '+' no início, se não houver
void ExplicitaSinal(char Entrada[]) {
    // Verifica se o sinal já está presente
    if (Entrada[0] != '+' && Entrada[0] != '-') {
        // Move a string para a direita para abrir espaço para o sinal
        size_t len = strlen(Entrada);
        for (size_t i = len + 1; i > 0; i--) {
            Entrada[i] = Entrada[i - 1];
        }
        Entrada[0] = '+';  // Define o sinal como positivo
    }
}

// Remove zeros desnecessários (à esquerda e à direita) na string de entrada
void RemoveZeros(char Entrada[]) {
    size_t NumI = 1; // Começa após o sinal

    // Remove zeros à esquerda
    while (Entrada[NumI] == '0' && Entrada[NumI + 1] != '.' && Entrada[NumI + 1] != '\0') {
        NumI++;
    }

    // Realoca a string se houver zeros à esquerda
    if (NumI > 1) {
        memmove(Entrada + 1, Entrada + NumI, strlen(Entrada) - NumI + 1);
    }

    // Remove zeros à direita da parte fracionária
    size_t len = strlen(Entrada);
    if (strchr(Entrada, '.')) {
        size_t NumF = len - 1;
        while (Entrada[NumF] == '0') { // Zeros à direita
            NumF--;
        }
        if (Entrada[NumF] == '.') {
            NumF--; // Remove ponto final isolado
        }
        Entrada[NumF + 1] = '\0'; // Ajusta o final da string
    }
}

// Busca a posição do ponto decimal na string
long int PontoOFinal(char *Entrada) {
    long int i = 0;
    char *ProcuraPonto = Entrada;

    // Procura o ponto ou fim da string
    while (*ProcuraPonto != '\0' && *ProcuraPonto != '.') {
        i++;
        ProcuraPonto++;
    }

    return (*ProcuraPonto == '\0') ? 0 : i;
}

// Converte uma string para um número inteiro
long int ConverteInteiro(char Entrada[], long int Qtd) {
    long int Convertido = 0; // Inicializa o valor

    // Converte cada caractere para inteiro e acumula
    for (; Qtd > 0; Qtd--, Entrada++) {
        Convertido += (*Entrada - '0') * Potenciacao(10, Qtd - 1);
    }

    return Convertido;
}

// Converte parte inteira de um número decimal para binário
void ConverteBinarioPI(FilaB *F, char **Binario, char Sinal, long int *LimiteU, long int *LimiteM) {
    size_t Alocado = BLOCK_SIZE;
    long int Andado = 0;
    char *Convertido = (char *)malloc(Alocado);

    if (Convertido == NULL) {
        perror("Falha na alocação de memória");
        exit(1);
    }

    // Converte a fila de inteiros para binário
    while (!VFVaziaB(*F)) {
        int Resto = 0;
        FBApontador aux = F->Frente;

        // Divide cada elemento por 2 e coleta o resto (bit)
        while (aux != NULL) {
            long int Num = aux->I.Decimal;
            aux->I.Decimal = Num / 2;

            if (Resto == 1) {
                aux->I.Decimal += 5 * Potenciacao(10, 8 - 1);
                Resto = 0;
            }

            Resto = Num % 2;
            aux = aux->Prox;
        }

        // Realoca memória se necessário
        if (Andado + 2 >= Alocado) {
            Alocado += BLOCK_SIZE;
            char *temp = realloc(Convertido, Alocado);
            if (temp == NULL) {
                perror("Falha ao realocar memória");
                free(Convertido);
                exit(1);
            }
            Convertido = temp;
        }

        Convertido[Andado++] = Resto + '0';

        if(F->Frente->I.Decimal < 1) {
            Desenfileira(&F);
        }
        // Erro caso estoure o limite do expoente da direita
        if(Andado > *LimiteU) {
            while(!VFVaziaB(*F)) {
                DesenfileiraB(F);
            }
            *Binario = NULL;
            *LimiteU = -1;
            return;
        }
        // Erro caso estoure o limite da mantissa
        if(Andado > LimiteM) {
            if(Binario[Andado] == '1' && !VFVaziaB(*F)) {
                Binario[Andado - 1] = '1';
            }
            Binario[Andado] = '\0';
            *LimiteM = -1;
            return;
        }

        // Finaliza a string quando a fila estiver vazia
        if(VFVaziaB(*F)) {
            Convertido[Andado] = '\0';
        }
    }

    // Ajusta para números negativos (complemento de dois)
    if (Sinal == '-') {
        for (size_t i = 0; i < Andado; i++) {
            Convertido[i] = (Convertido[i] == '0') ? '1' : '0';
        }

        // Adiciona 1 ao final, se necessário
        size_t i = Andado;
        while (i-- > 0) {
            if (Convertido[i] == '0') {
                Convertido[i] = '1';
                break;
            }
            Convertido[i] = '0';
        }

        // Expande a string se necessário
        if (i == -1) {
            if (Andado + 1 >= Alocado) {
                Alocado += BLOCK_SIZE;
                char *temp = realloc(Convertido, Alocado);
                if (temp == NULL) {
                    perror("Falha ao realocar memória");
                    free(Convertido);
                    exit(1);
                }
                Convertido = temp;
            }
            memmove(Convertido + 1, Convertido, Andado + 1);
            Convertido[0] = '1';
            Andado++;
        }
    }

    *Binario = Convertido;
}

// Converte parte fracionaria de um número decimal para binário
void ConverteBinarioPF(FilaB *F, char **Binario) {
    size_t Alocado = BLOCK_SIZE, Andado = 0;
    char *Convertido = (char *)malloc(Alocado);

    if (Convertido == NULL) {
        perror("Falha na alocação de memória");
        exit(1);
    }

    // Converte a fila de inteiros para binário
    while (!VFVaziaB(*F)) {
        int Excedente = 0;
        FBApontador aux = F->Frente;

        // Divide cada elemento por 2 e coleta o resto (bit)
        while (aux != NULL) {
            long int Num = aux->I.Decimal * 2;
            aux->I.Decimal = Num % Potenciacao(10, aux->I.Casas);

            if (Excedente > 0) {
                aux->I.Decimal += Excedente;
                Excedente = 0;
            }

            Excedente = Num / Potenciacao(10, aux->I.Casas);
            aux = aux->Prox;
        }

        // Realoca memória se necessário
        if (Andado + 2 >= Alocado) {
            Alocado += BLOCK_SIZE;
            char *temp = realloc(Convertido, Alocado);
            if (temp == NULL) {
                perror("Falha ao realocar memória");
                free(Convertido);
                exit(1);
            }
            Convertido = temp;
        }

        Convertido[Andado++] = Excedente + '0';

        // Verifica se é necessário remover os itens nulos da lista
        while(F->Frente->I.Decimal < 1) {
            Desenfileira(&F);
        }

        // Finaliza a string quando a fila estiver vazia
        if (VFVaziaB(*F)) {
            Convertido[Andado] = '\0';
        }
    }

    *Binario = Convertido;
}

// Inverte a string
void InverteString(char str[]) {
    char aux; // Auxiliar para troca
    size_t esq = 0, dir = strlen(str) - 1; // Índices das extremidades

    while (esq < dir) { // Enquanto os índices não se cruzarem
        aux = str[esq]; // Guarda caractere à esquerda
        str[esq++] = str[dir]; // Troca com o da direita
        str[dir--] = aux; // Coloca o guardado à direita
    }
}

// Pontenciacao
long int Potenciacao(long int n, long int e) {
    if (e == 0) {
        return 1;
    }
    
    long int half = Potenciacao(n, e / 2); // calcula n^(e/2)
    
    if (e % 2 == 0) {
        return half * half; // Se 'e' é par
    } else {
        return n * half * half; // Se 'e' é ímpar
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

        CONVERTE PARA BINARIO A PARTE FRACIONARIA {
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