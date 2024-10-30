#include "ESTRUTURAS.H"

#define BLOCK_SIZE 32

// Pre visualizacao das funcoes
int EntradaPadronizada(char Entrada[]);
int EntradaNula(char Entrada[]);
void ExplicitaSinal(char Entrada[]);
void RemoveZeros(char Entrada[]);
long int PontoOFinal(char *Entrada);
long int ConverteInteiro(char Entrada[], long int Qtd);
void ConverteBinarioPI(FilaB *F,ItemFila *I, long int *LimiteU, long int *LimiteM, long int *LimiteL);
void ConverteBinarioPF(FilaB *F, ItemFila *I, long int *LimiteL, long int *LimiteM, long int *LimiteU);
void InverteString(char str[]);
void OutputPadrao(ItemFila I);
long int Potenciacao(long int n, long int e);
void LimpaTela(void);
void PausaTela(void);

// -------------------------------------------------------
int main(void) {
    Fila Binarios;
    long int M; // Mantissa
    long int L, U; // Expoentes
    char Verificador;
    char ch;

    printf("Parametros:\n- Mantissa: ");
    scanf(" %li", &M);
    printf("- Limite inferior do expoente: ");
    scanf(" %li", &L);
    printf("- Limite superior do expoente: ");
    scanf(" %li", &U);

    if(M < 1|| M > 1073741824) {
        printf("A mantissa nao pode assumir o valor inserido\n");
        return 1;
    }

    if(L < -(536870912) || U >  536870911) {
        printf("Expoente acima do limite de representacao\n");
        return 1;
    }
    if(L > U) {
        printf("O limite de expoentes da esquerda nao pode ser maior que o da direita\n");
        return 1;
    }

    // Limpa o buffer do stdin para remover qualquer caractere extra
    while ((ch = getchar()) != '\n' && ch != EOF);

    FFVazia(&Binarios);

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
        long int ULimite = U;
        long int LLimite = L;
        long int TamanhoEntrada = strlen(Entrada);
        long int Ponto = PontoOFinal(Entrada);

        // Atribui os valores dos erros (nao possui erros)
        for(int i = 0; i < 4; i++) {
            I.Erros[i] = 0;
        }

        I.Sinal = (Entrada[0] == '-') ? '1' : '0' ; // Atribui o sinal

        if(Entrada[1] != '0') {  // Possui parte inteira
            FilaB FI;
            
            long int QtdNums = (Ponto - 1); // Quantidade de numeros
            int PID = QtdNums / 8; // Parte inteira da divisao 
            int MD = QtdNums % 8; // Modulo da divisao
            char *PonteiroConversaoI = &Entrada[1];
            
            FFVaziaB(&FI); // Faz fila vazia

            if(MD > 0) {
                ItemB Iaux;
                
                Iaux.Casas = MD;
                Iaux.Decimal = ConverteInteiro(PonteiroConversaoI, Iaux.Casas); // Recebe o size_t convertido
                EnfileiraB(&FI,Iaux); // Enfileira 
            }
            for(size_t i = PID; i > 0; i--) {
                ItemB Iaux;
                
                Iaux.Casas = 8;
                Iaux.Decimal = ConverteInteiro(PonteiroConversaoI, 8);
                EnfileiraB(&FI,Iaux);
            }

            ConverteBinarioPI(&FI, &I, &ULimite, &MantissaLimite, &LLimite);
            InverteString(I.BinarioI);
        } else {
            I.BinarioI = NULL;
        }

        if(Ponto != 0 && I.Erros[0] == 0 && I.Erros[1] == 0 &&I.Erros[3] == 0) { // Possui apenas parte fracionaria
            FilaB FF;
            long int QtdNums = (TamanhoEntrada - Ponto - 1);
            int PID = QtdNums / 8; // Parte inteira da divisao 
            int MD = QtdNums % 8; // Modulo da divisao
            char *PonteiroConversaoI = &Entrada[Ponto + 1]; // Ponteiro para o primeiro numero da parte fracionaria

            FFVaziaB(&FF); // Faz pilha vazia

            if(MD > 0) { 
                ItemB Iaux;
                
                Iaux.Casas = MD;
                Iaux.Decimal = ConverteInteiro(PonteiroConversaoI, Iaux.Casas); // Recebe o size_t convertido
                EnfileiraB(&FF,Iaux); // Enfileira 
            }
            for(size_t i = PID; i > 0; i--) {
                ItemB Iaux;
                
                Iaux.Casas = 8;
                Iaux.Decimal = ConverteInteiro(PonteiroConversaoI, 8);
                EnfileiraB(&FF,Iaux);
            }

            ConverteBinarioPF(&FF, &I, &LLimite, &MantissaLimite, &ULimite);
        } else {
            I.BinarioF = NULL;
        }

        Enfileira(&Binarios,I);
        free(Entrada);
        
        printf("Deseja inserir mais um numero? (s/n): ");
        scanf(" %c", &Verificador);
        while ((ch = getchar()) != '\n' && ch != EOF);

    } while (Verificador != 'N' && Verificador != 'n');  // Sai do loop se 'N' ou 'n' for digitado

    while(!VFVazia(Binarios)) {
        OutputPadrao(Binarios.Frente->I);
        Desenfileira(&Binarios);
    }

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
        Convertido += (*Entrada - '0') * (Potenciacao(10, Qtd - 1));
    }

    return Convertido;
}

// Converte parte inteira de um número decimal para binário, considerando limites e sinal
void ConverteBinarioPI(FilaB *F,ItemFila *I, long int *LimiteU, long int *LimiteM, long int *LimiteL) {
    long int Alocado = BLOCK_SIZE;  // Tamanho inicial alocado para a string binária
    long int Andado = 0;
    char *Convertido = (char *)malloc(Alocado);

    if (Convertido == NULL) {  // Verifica erro de alocação
        perror("Falha na alocação de memória");
        exit(1);
    } 

    // Converte os valores da fila para binário
    while (!VFVaziaB(*F)) {
        int Resto = 0;
        FBApontador aux = F->Frente;

        // Divide cada elemento da fila por 2 e armazena o resto (bit)
        while (aux != NULL) {
            long int Num = aux->I.Decimal;
            aux->I.Decimal = Num / 2;

            if (Resto == 1) {  // Corrige resto se necessário
                aux->I.Decimal += 5 * (Potenciacao(10, 8 - 1));
                Resto = 0;
            }

            Resto = Num % 2;
            aux = aux->Prox;
        }

        Convertido[Andado++] = (Resto > 0) ? '1' : '0';  // Adiciona o bit na string

        // Verifica e trata o limite do expoente (U)
        if(Andado + 1 > *LimiteU) { 
            while(!VFVaziaB(*F)) {
                DesenfileiraB(F);
            }
            I->BinarioI = NULL;
            I->Erros[0] = 1;
            free(Convertido);
            return;
        }
       
        

        // Verifica e trata o limite da mantissa (M)
        if(Andado + 1 > *LimiteM) {
            if(Convertido[Andado] == '0' && !VFVaziaB(*F)) {
                Convertido[Andado] = '1';
                I->Erros[2] = 1;        // Marca arredondamento
            }
            Convertido[Andado + 1] = '\0';
            I->Erros[3] = 1;  // Sinaliza truncamento

            while(!VFVaziaB(*F)) {
                DesenfileiraB(F);
            }

            break;
        }

        // Realoca memória se necessário
        if (Andado + 2 >= Alocado) {
            Alocado += BLOCK_SIZE;
            char *temp = realloc(Convertido, Alocado);
            if (temp == NULL) {  // Verifica erro de realocação
                perror("Falha ao realocar memória");
                free(Convertido);
                exit(1);
            }
            Convertido = temp;
        }

        // Remove da fila se valor for menor que 1
        if(F->Frente->I.Decimal < 1) {
            DesenfileiraB(F);
        }

        // Finaliza a string binária
        if(VFVaziaB(*F)) {
            Convertido[Andado] = '\0';
        }
    }

    if(Andado + 1 < *LimiteL) {
        I->BinarioI = NULL;
        I->Erros[1] = 1;
        free(Convertido);
        return;
    }

    // Converte para complemento de dois se o número for negativo
    if (I->Sinal == '-') {
        size_t i = Andado;
        while (i-- > 0) {
            if (Convertido[i] == '0') {
                Convertido[i] = '1';
                break;
            }
            if(i == 0) {  // Se não houver espaço para +1 no valor
                if(((*LimiteM - Andado - 1) > 0) && ((*LimiteU - Andado - 1) > 0)) {
                    Convertido[Andado++] = '1'; Convertido[Andado] = '\0';
                } else if((*LimiteU - Andado - 1) == 0) {  // Se ocorrer limite no expoente
                    I->BinarioI = NULL;
                    I->Erros[0] = 1;
                    free(Convertido);
                    return;
                } else if(*LimiteM != -1){
                    Convertido[Andado + 1] = '\0';
                    I->Erros[2] = 1;
                    I->Erros[3] = 1;
                }                
            }                  
        }
        
        // Ajusta bits invertendo-os para o complemento de dois
        for (long int i = 0; i < Andado + 1; i++) {
            Convertido[i] = (Convertido[i] == '0') ? '1' : '0';
        }
    }

    // Atualiza limites restantes e retorna a string binária
    if(I->Erros[4] == 0) {
        *LimiteM -= (Andado + 1);
    }
    I->Expoente = Andado + 1;
    I->BinarioI = (char *)malloc(strlen(Convertido) + 1);
    if (I->BinarioI == NULL) {
        perror("Falha na alocacao de memoria");
        return;
    }
    memcpy(I->BinarioI, Convertido, strlen(Convertido) + 1);
    free(Convertido);
}

// Converte parte fracionária de um número decimal para binário
void ConverteBinarioPF(FilaB *F, ItemFila *I, long int *LimiteL, long int *LimiteM, long int *LimiteU) {
    long int Alocado = BLOCK_SIZE;
    long int Andado = 0;
    long int PrimeiroNaoNulo = 0; // Marca a aparição do primeiro número diferente de zero
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
            aux->I.Decimal = Num % (Potenciacao(10, aux->I.Casas));

            if (Excedente > 0) {
                aux->I.Decimal += Excedente;
                Excedente = 0;
            }

            Excedente = Num / (Potenciacao(10, aux->I.Casas));
            aux = aux->Prox;
        }
        
        if (PrimeiroNaoNulo == 0 && Excedente > 0) {
            PrimeiroNaoNulo = Andado;
            // Verifica e trata o limite do expoente (U)
            if ((-1 * PrimeiroNaoNulo) > *LimiteU && I->BinarioI == NULL) {
                I->BinarioF = NULL;
                I->Erros[0] = 1;
                free(Convertido);
                return;
            }
            // Verifica e trata o limite do expoente (L)
            if ((-1 * PrimeiroNaoNulo) < *LimiteL && I->BinarioI == NULL) {
                I->BinarioF = NULL;
                I->Erros[1] = 1;
                free(Convertido);
                return;
            }
        }
        
        Convertido[Andado++] = (Excedente > 0) ? '1' : '0';

        // Verifica e trata o limite da mantissa (M)
        if (Andado + 1 > *LimiteM) {
            if (Convertido[Andado - 1] == '0' && !VFVaziaB(*F)) {
                Convertido[Andado - 1] = '1';
                I->Erros[2] = 1; // Marca arredondamento
            }
            Convertido[Andado] = '\0'; // Certifique-se de que está finalizando corretamente
            I->Erros[3] = 1; // Sinaliza truncamento

            while (!VFVaziaB(*F)) {
                DesenfileiraB(F);
            }

            break;
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

        // Verifica se é necessário remover o item nulo da lista
        if (F->Frente->I.Decimal < 1) {
            DesenfileiraB(F);
        }

        // Finaliza a string quando a fila estiver vazia
        if (VFVaziaB(*F)) {
            Convertido[Andado] = '\0';
        }
    }

    if (I->BinarioI == NULL) {
        I->Expoente = ((-1) * PrimeiroNaoNulo);
        size_t len = strlen(Convertido);
        size_t j = 0;

        // Altere o loop para garantir que não acessamos além da string
        for (size_t i = PrimeiroNaoNulo; i < len; i++, j++) {
            Convertido[j] = Convertido[i];
        }
        Convertido[j] = '\0'; // Adicione o terminador nulo após a cópia
    }

    I->BinarioF = (char *)malloc(strlen(Convertido) + 1);
    if (I->BinarioF == NULL) {
        perror("Falha na alocação de memória");
        free(Convertido);
        return;
    }
    memcpy(I->BinarioF, Convertido, strlen(Convertido) + 1);

    // Libera a memória de Convertido após a cópia
    free(Convertido);
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

// Mostra a saida no padrao desejado
void OutputPadrao(ItemFila I) {
    printf("Numero normalizado armazenado em binario:");
    if(I.Erros[0] == 1) {
        printf(" Overflow\n");
    } else if(I.Erros[1] == 1) {
        printf(" Underflow\n");
    } else {
        printf(" %c ", I.Sinal);
        if(I.BinarioI != NULL) {
            printf("%s", I.BinarioI);
        } else {
            printf("0.");
        }
        if(I.BinarioF != NULL) {
            printf("%s", I.BinarioF);
        }
        printf(" * 2 ^ %li\n", I.Expoente);
        if(I.Erros[2] == 1) {
            printf("Arredondamento\n");
        } else if(I.Erros[3] == 1) {
            printf("Truncamento\n");
        }   
    }
    printf("\n");
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

// Limpa a tela
void LimpaTela(void) {
    system("clear");
}
// Pausa a tela
void PausaTela(void) {
    getchar(); getchar();
    printf("Aperte Enter para continuar ");
}